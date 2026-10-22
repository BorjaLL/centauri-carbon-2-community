import assert from 'node:assert/strict'
import { fileURLToPath } from 'node:url'
import path from 'node:path'
import { createServer } from 'vite'
import { chromium } from 'playwright'

const root = fileURLToPath(new URL('..', import.meta.url))
process.env.CC2_BACKEND = 'http://127.0.0.1:1'
const server = await createServer({ root, configFile: path.join(root, 'vite.config.ts'), mode: 'demo', server: { port: 0, host: '127.0.0.1' } })
let browser
// machine: 1 idle, 2 printing, 3 paused, 0 disconnected; mode is what /api/printer reports.
let machine = 1, mode = null, reject = false
const commands = [], errors = []
const names = { 1: 'Idle', 2: 'Printing', 3: 'Paused' }
try {
  await server.listen()
  browser = await chromium.launch({ headless: true, ...(process.env.CC2_BROWSER_PATH ? { executablePath: process.env.CC2_BROWSER_PATH, args: ['--no-sandbox', '--disable-gpu', '--disable-software-rasterizer', '--single-process', '--no-zygote'] } : {}) })
  const page = await browser.newPage({ viewport: { width: 1440, height: 1000 } })
  page.on('pageerror', e => errors.push(e.message))
  await page.route('**/api/preferences', r => r.fulfill({ json: { language: 'en', theme: 'dark' } }))
  await page.route('**/api/printer', r => r.fulfill({ json: machine === 0 ? { connected: false } : {
    connected: true, last_message_age: 0, motor_mode: mode,
    machine: { status: machine, status_name: names[machine], progress: 20 },
    print: { filename: machine === 1 ? '' : 'fixture.gcode', state: machine === 1 ? 'idle' : machine === 3 ? 'paused' : 'printing' },
    extruder: { temperature: 25, target: 0 }, heater_bed: { temperature: 25, target: 0 },
  } }))
  await page.route('**/api/control', r => {
    const action = r.request().postData()
    commands.push(action)
    if (reject) return r.fulfill({ status: 409, body: 'printer telemetry is stale' })
    // Telemetry reports the requested mode only after the backend accepted it.
    if (action === 'motors:quiet') mode = 'quiet'
    if (action === 'motors:normal') mode = 'normal'
    return r.fulfill({ status: 202, json: { accepted: true } })
  })
  await page.goto(`${server.resolvedUrls.local[0]}#control`)
  const quiet = page.getByRole('button', { name: 'Quiet', exact: true })
  const normal = page.getByRole('button', { name: 'Normal', exact: true })
  const pressed = async (button, on) => {
    await page.waitForFunction(([name, on]) => [...document.querySelectorAll('button[aria-pressed]')]
      .some(b => b.textContent.trim() === name && b.getAttribute('aria-pressed') === String(on)), [await button.textContent().then(s => s.trim()), on])
  }
  const warning = page.getByText('Quiet mode is on for this print', { exact: false })
  await quiet.waitFor()
  await page.waitForFunction(() => [...document.querySelectorAll('button')].some(b => b.textContent.trim() === 'Quiet' && !b.disabled))
  await pressed(quiet, false)
  await pressed(normal, false)

  // Cancel sends nothing; the confirmation names the cap.
  await quiet.click()
  await page.getByText('Speed is capped at 80 mm/s and acceleration at 2000 mm/s²', { exact: false }).waitFor()
  await page.getByRole('button', { name: 'Cancel', exact: true }).click()
  await page.waitForTimeout(300)
  assert.deepEqual(commands, [])
  await pressed(quiet, false)

  // Confirm sends exactly one action and the button follows telemetry.
  await quiet.click()
  await page.getByRole('button', { name: 'Confirm', exact: true }).click()
  await pressed(quiet, true)
  assert.deepEqual(commands, ['motors:quiet'])
  assert.equal(await warning.count(), 0, 'no print warning while idle')

  // Printing and paused: both buttons unavailable, the print warning shows.
  for (const state of [2, 3]) {
    machine = state
    await page.waitForFunction(() => [...document.querySelectorAll('button')].filter(b => ['Quiet', 'Normal'].includes(b.textContent.trim())).every(b => b.disabled))
    await warning.waitFor()
  }
  // Disconnected: unavailable.
  machine = 0
  await page.waitForFunction(() => [...document.querySelectorAll('button')].filter(b => ['Quiet', 'Normal'].includes(b.textContent.trim())).every(b => b.disabled))

  // Print end: the backend restores Normal and the page follows.
  machine = 1
  mode = 'normal'
  await pressed(normal, true)
  await pressed(quiet, false)
  assert.equal(await warning.count(), 0)

  // Normal confirmation names the fixed restore values; cancel sends nothing.
  await normal.click()
  await page.getByText('restore the stock 500 mm/s and 20000 mm/s²', { exact: false }).waitFor()
  await page.getByRole('button', { name: 'Cancel', exact: true }).click()
  await page.waitForTimeout(300)
  assert.deepEqual(commands, ['motors:quiet'])

  // A refused request (e.g. stale telemetry) shows an error and leaves the mode unchanged.
  reject = true
  await quiet.click()
  await page.getByRole('button', { name: 'Confirm', exact: true }).click()
  await page.getByText('printer telemetry is stale', { exact: false }).waitFor()
  assert.deepEqual(commands, ['motors:quiet', 'motors:quiet'])
  await pressed(normal, true)
  await pressed(quiet, false)

  for (const width of [768, 390]) {
    await page.setViewportSize({ width, height: 1000 })
    await page.waitForTimeout(100)
    assert.ok(await page.evaluate(() => document.documentElement.scrollWidth <= innerWidth), 'no horizontal overflow')
  }
  assert.deepEqual(errors, [])
  console.log('PASS: motor mode cancel/confirm, telemetry-driven state, unavailable while printing/paused/disconnected, print warning, print-end Normal, refused request')
} finally {
  await browser?.close()
  await server.close()
}
