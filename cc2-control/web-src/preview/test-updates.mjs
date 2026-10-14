import assert from 'node:assert/strict'
import { createServer } from 'vite'
import { chromium } from 'playwright'
import { fileURLToPath } from 'node:url'
import path from 'node:path'
const root = fileURLToPath(new URL('..', import.meta.url))
const server = await createServer({ root, configFile: path.join(root, 'vite.config.ts'), mode: 'demo', server: { port: 0, host: '127.0.0.1' } })
let browser
try {
  await server.listen()
  browser = await chromium.launch({ headless: true, ...(process.env.CC2_BROWSER_PATH ? { executablePath: process.env.CC2_BROWSER_PATH } : {}) })
  const page = await browser.newPage({ viewport: { width: 1440, height: 1000 } })
  const errors = []; page.on('pageerror', e => errors.push(e.message))
  let downloads = 0, installs = 0, checks = 0
  const status = { enabled: true, ready: true, busy: false, installed: '1.2.8', firmware: '4.4', job: '', phase: 'none', version: '' }
  const index = { schema: 1, expires: Math.floor(Date.now()/1000)+86400, releases: [
    { version: '1.2.9', firmware: '4.4', channel: 'stable', url: 'https://github.com/Neme77/centauri-carbon-2-community/releases/download/control-1.2.9/Control.cc2update', size: 1500000, notes: 'Stable fixture — retained settings' },
    { version: '9.0.0', firmware: '4.3', channel: 'stable', url: 'https://github.com/Neme77/centauri-carbon-2-community/releases/download/control-9.0.0/Control.cc2update', size: 1500000, notes: 'WRONG FIRMWARE — must be hidden' },
    { version: '1.3.0', firmware: '4.4', channel: 'canvas-uds', url: 'https://github.com/Neme77/centauri-carbon-2-community/releases/download/control-1.3.0/Control.cc2update', size: 1500000, notes: 'Experimental fixture' },
  ] }
  let cache = { schema: 1, expires: 0, releases: [] }
  await page.route('**/api/preferences', r => r.fulfill({ json: { language: 'en', theme: 'dark' } }))
  await page.route('https://raw.githubusercontent.com/**/update-channel.cc2catalog', r => { checks++; return r.fulfill({ body: Buffer.from('signed-catalog-fixture'), contentType: 'application/octet-stream', headers: { 'Access-Control-Allow-Origin': '*' } }) })
  await page.route('**/api/app-update/status', r => r.fulfill({ json: status }))
  await page.route('**/api/app-update/catalog', r => {
    if (r.request().method() === 'POST') cache = index
    return r.fulfill({ json: cache })
  })
  await page.route('**/api/app-update/download', r => {
    assert.equal(r.request().headers()['x-cc2-request'], '1')
    if (r.request().headers()['x-cc2-lan'] !== 'test-confirmation') return r.fulfill({ status: 401, json: { error: 'LAN code rejected' } })
    assert.equal(r.request().postData(), index.releases[0].url)
    downloads++; Object.assign(status, { phase: 'ready', job: 'a'.repeat(32), version: '1.2.9' })
    return r.fulfill({ status: 201, json: { verified: true, job: status.job } })
  })
  await page.route('**/api/app-update/install', r => {
    assert.equal(r.request().headers()['x-cc2-lan'], 'test-confirmation')
    assert.equal(r.request().postData(), 'a'.repeat(32))
    installs++; Object.assign(status, { phase: 'complete', installed: '1.2.9' })
    return r.fulfill({ status: 202, json: { accepted: true } })
  })
  await page.goto(server.resolvedUrls.local[0] + '#settings')
  await page.getByRole('tab', { name: 'Updates', exact: true }).click()
  await page.getByText('Compatible update available: 1.2.9', { exact: true }).waitFor()
  assert.equal(checks, 1)
  assert.equal(await page.getByText('WRONG FIRMWARE — must be hidden').count(), 0)
  const download = page.getByRole('button', { name: 'Download and verify', exact: true })
  const install = page.getByRole('button', { name: 'Install verified update', exact: true })
  assert.equal(await download.isEnabled(), false)
  assert.equal(await install.isEnabled(), false)
  await page.getByLabel('LAN Access Code', { exact: true }).fill('bad-code')
  await download.click(); await page.getByRole('dialog').getByRole('button',{name:'Confirm',exact:true}).click()
  await page.getByRole('alert').filter({hasText:'LAN code rejected'}).waitFor()
  assert.equal(downloads, 0); assert.equal(installs, 0)
  await page.getByLabel('LAN Access Code', { exact: true }).fill('test-confirmation')
  await download.click(); await page.getByRole('dialog').getByRole('button',{name:'Confirm',exact:true}).click()
  await page.getByText('Selected version: 1.2.9', { exact: true }).waitFor()
  assert.equal(downloads, 1); assert.equal(installs, 0, 'verification must not install')
  assert.equal(await install.isEnabled(), true)
  await install.click(); await page.getByRole('dialog').getByRole('button',{name:'Cancel',exact:true}).click()
  assert.equal(installs, 0)
  await install.click(); await page.getByRole('dialog').getByRole('button',{name:'Confirm',exact:true}).click()
  await page.getByRole('button',{name:'Reload CC2 Control',exact:true}).waitFor()
  assert.equal(installs, 1)
  assert.equal(await page.getByLabel('LAN Access Code', { exact: true }).inputValue(), '')
  await page.getByLabel('Update channel', { exact: true }).selectOption('canvas-uds')
  await page.getByRole('dialog').getByRole('button',{name:'Cancel',exact:true}).click()
  assert.equal(await page.getByLabel('Update channel', { exact: true }).inputValue(), 'stable')
  await page.getByLabel('Update channel', { exact: true }).selectOption('canvas-uds')
  await page.getByRole('dialog').getByRole('button',{name:'Confirm',exact:true}).click()
  await page.getByText('Experimental fixture', { exact: true }).waitFor()
  for (const width of [1440,768,390]) {
    await page.setViewportSize({width,height:1000})
    assert(await page.evaluate(() => document.documentElement.scrollWidth <= innerWidth), 'settings overflow '+width)
  }
  assert.deepEqual(errors, [])
  console.log('PASS: actual update controls, compatible channels, confirmations, auth failure, verify-before-install and mobile layout')
} finally { await browser?.close(); await server.close() }
