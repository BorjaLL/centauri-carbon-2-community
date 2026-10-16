import assert from 'node:assert/strict'
import { chromium } from 'playwright'
import { fixture, launchOptions } from './fixture.mjs'

const demo = await fixture()
const browser = await chromium.launch(launchOptions)
try {
  const page = await browser.newPage({ viewport: { width: 1440, height: 1000 } })
  await page.addInitScript(() => localStorage.setItem('cc2-fixture-preferences', '{"language":"en"}'))
  await page.goto(demo.url + '#canvas')
  const load = page.locator('[data-canvas-action="canvas.load"]')
  const unload = page.locator('[data-canvas-action="canvas.unload"]')
  const card = slot => page.locator(`[data-canvas-slot="${slot}"]`)
  await load.waitFor()
  assert.equal(await load.isDisabled(), true)
  assert.equal(await unload.isDisabled(), true)
  await card(0).getByText('Empty', { exact: true }).waitFor()
  await card(1).getByRole('button', { name: 'Select slot', exact: true }).click()
  assert.equal(await load.isDisabled(), false)
  assert.equal(await unload.isDisabled(), true)
  await card(1).getByText('Present · unloaded from head', { exact: true }).waitFor()
  await page.evaluate(() => {
    Object.assign(window.__CC2_PREVIEW, { activeTray: 1, headLoaded: true })
    window.__CC2_PREVIEW.trays[1].status = 2
  })
  await card(1).getByText('Loaded to head', { exact: true }).waitFor()
  assert.equal(await load.isDisabled(), true)
  assert.equal(await unload.isDisabled(), false)
  await card(2).getByRole('button', { name: 'Select slot', exact: true }).click()
  assert.equal(await load.isDisabled(), true, 'another loaded channel prevents loading')
  assert.equal(await unload.isDisabled(), true, 'only the loaded channel can unload')
  await page.evaluate(() => { window.__CC2_PREVIEW.headLoaded = false })
  await card(2).getByText('Transition · waiting for confirmation', { exact: true }).waitFor()
  assert.equal(await load.isDisabled(), true)
  await page.evaluate(() => { window.__CC2_PREVIEW.activeTray = -1 })
  await page.waitForFunction(() => !document.querySelector('[data-canvas-action="canvas.load"]').disabled)
  await page.evaluate(() => { window.__CC2_PREVIEW.status = 2 })
  await page.waitForFunction(() => document.querySelector('[data-canvas-action="canvas.load"]').disabled)
  await page.evaluate(() => { window.__CC2_PREVIEW.status = 1; window.__CC2_PREVIEW.canvasFresh = false })
  await card(2).getByText('State unavailable', { exact: true }).waitFor()
  assert.equal(await load.isDisabled(), true)
  assert.equal(await unload.isDisabled(), true)
  assert.deepEqual(await page.evaluate(() => window.__CC2_PREVIEW.commands), [], 'state changes and disabled actions send no commands')
  for (const width of [1440, 768, 390]) {
    await page.setViewportSize({ width, height: 1000 })
    await page.waitForTimeout(200)
    const overflow = await page.evaluate(() => ({ width: innerWidth, scroll: document.documentElement.scrollWidth,
      elements: [...document.querySelectorAll('main *')].filter(e => e.getBoundingClientRect().right > innerWidth + 1).slice(0, 8).map(e => [e.tagName, e.className, e.textContent.slice(0, 70)]) }))
    assert(overflow.scroll <= overflow.width, JSON.stringify(overflow))
  }
  console.log('PASS: Canvas slot state and action eligibility across empty, parked, loaded, other channel, transition, print and unavailable state')
} finally {
  await browser.close()
  await demo.cleanup()
}
