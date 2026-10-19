import assert from 'node:assert/strict'
import { createServer } from 'node:http'
import { readFile } from 'node:fs/promises'
import { chromium } from 'playwright'
import { fixture, launchOptions } from './fixture.mjs'

const demo = await fixture()
const html = await readFile(new URL(demo.url), 'utf8')
const server = createServer((req, res) => {
  res.writeHead(200, { 'Content-Type': 'text/html' })
  res.end(html)
})
await new Promise(resolve => server.listen(0, '127.0.0.1', resolve))
const host = '127.0.0.1:' + server.address().port
const browser = await chromium.launch(launchOptions)
try {
  const page = await browser.newPage({ viewport: { width: 1440, height: 1000 } })
  await page.addInitScript(() => localStorage.setItem('cc2-fixture-preferences', '{"language":"en"}'))
  await page.route(/^http:\/\/(pandabreath\.local|dragonbreath\.local|heater\.local)(:\d+)?\//, route =>
    route.fulfill({ contentType: 'text/html', body: '<p>Heater fixture</p>' }))
  await page.goto('http://' + host + '/#settings/integrations')
  const menu = () => page.locator('aside a[href="#pandabreath"]')
  assert.equal(await menu().count(), 0)
  const toggle = () => page.getByRole('switch', { name: /^(Panda Breath|DragonBreath) page$/ })
  await toggle().click()
  await menu().waitFor()
  const firmware = page.getByRole('switch', { name: 'DragonBreath firmware', exact: true })
  const input = page.locator('#pandabreath-host')
  assert.equal(await input.inputValue(), 'pandabreath.local')
  await firmware.click()
  assert.equal(await input.inputValue(), 'dragonbreath.local')
  assert.equal(await menu().textContent(), 'DragonBreath')
  await input.fill('heater.local:8082')
  await page.getByRole('button', { name: 'Save address', exact: true }).click()
  await firmware.click()
  assert.equal(await input.inputValue(), 'heater.local:8082')
  await menu().click()
  await page.frameLocator('iframe').getByText('Heater fixture').waitFor()
  assert.equal(await page.locator('iframe').getAttribute('src'), 'http://heater.local:8082/')
  assert.deepEqual(await page.evaluate(() => window.__CC2_PREVIEW.commands), [])
  await page.reload()
  await page.frameLocator('iframe').getByText('Heater fixture').waitFor()
  await page.goto('http://' + host + '/#settings/integrations')
  for (const invalid of ['http://heater.local', 'heater.local/path', 'user@heater.local', '999.999.999.999', 'heater.local:0', 'heater.local:65536', host]) {
    await input.fill(invalid)
    await page.getByRole('button', { name: 'Save address', exact: true }).click()
    assert.equal(await page.evaluate(() => localStorage.getItem('cc2-pandabreath-host')), 'heater.local:8082', invalid)
  }
  await page.setViewportSize({ width: 390, height: 844 })
  await input.fill('heater.local')
  await page.getByRole('button', { name: 'Save address', exact: true }).click()
  await toggle().click()
  assert.equal(await menu().count(), 0)
  await page.goto('http://' + host + '/#pandabreath')
  await page.getByText('The Panda Breath page is turned off.', { exact: true }).waitFor()
  assert.equal(await page.locator('iframe').count(), 0)
  assert.deepEqual(await page.evaluate(() => window.__CC2_PREVIEW.commands), [])
  console.log('PASS: optional heater page, defaults, custom host, reload, invalid addresses, self-embedding refusal, mobile controls and disabled route; no printer commands')
} finally {
  await browser.close()
  await new Promise(resolve => server.close(resolve))
  await demo.cleanup()
}
