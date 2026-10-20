import { chromium } from 'playwright'
import assert from 'node:assert/strict'
import { fixture, launchOptions } from './fixture.mjs'
const demo=await fixture()
const browser=await chromium.launch(launchOptions)
const unload=page=>page.evaluate(()=>({...window.__CC2_PREVIEW.unload}))
try {
 const page=await browser.newPage({viewport:{width:1920,height:1080}})
 await page.goto(demo.url+'#canvas')
 let savePosts=0
 let restartPosts=0
 await page.route('**/api/canvas/unload', async route=>{
  if(route.request().method()==='POST') savePosts++
  await route.continue()
 })
 await page.route('**/api/canvas/unload/restart', async route=>{
  if(route.request().method()==='POST') restartPosts++
  await route.continue()
 })
 const box=page.locator('[data-canvas-unload]')
 const input=box.locator('#canvas-unload')
 const save=box.getByRole('button',{name:'Salva',exact:true})
 await input.waitFor()
 assert.equal(await input.inputValue(),'85')
 assert.match(await box.textContent(),/85 mm, predefinito della stampante/)
 // A value above the range set outside CC2 Control is explained, not flagged as an input error.
 await page.evaluate(()=>{window.__CC2_PREVIEW.unload.configured=560;location.hash='#control'})
 await page.evaluate(()=>{location.hash='#canvas'})
 await box.getByText(/più dei 350 mm/).waitFor()
 assert.equal(await input.inputValue(),'560')
 assert.doesNotMatch(await box.textContent(),/Inserisci un numero intero/)
 assert.equal(await save.isDisabled(),true)
 await page.evaluate(()=>{window.__CC2_PREVIEW.unload.configured=85;location.hash='#control'})
 await page.evaluate(()=>{location.hash='#canvas'})
 await input.waitFor()
 await page.waitForFunction(()=>document.getElementById('canvas-unload').value==='85')
 // Out of range or not a whole number: no request, the range is explained.
 for (const value of ['59','351','80.5']) {
  await input.fill(value)
  assert.equal(await save.isDisabled(),true,value)
  assert.match(await box.textContent(),/Inserisci un numero intero da 60 a 350 mm/)
 }
 await input.fill('85')
 assert.equal(await save.isDisabled(),false)
 // Saving offers the restart; declining leaves it pending with its own button.
 await input.fill('300')
 await save.click()
 await page.getByRole('button',{name:'Annulla',exact:true}).click()
 assert.deepEqual(await unload(page),{configured:85,override:300,pending:true,restarts:0})
 await box.getByText('Salvata. Riavvia la stampante per applicarla.').waitFor()
 assert.match(await box.textContent(),/300 mm, impostato qui/)
 assert.equal(savePosts,1,'only one save POST before explicit restart')
 // A disabled Save button must not remain reachable through the Enter shortcut.
 assert.equal(await save.isDisabled(),true)
 await input.press('Enter')
 await page.waitForTimeout(150)
 assert.equal(savePosts,1,'Enter must not re-save an unchanged override')
 assert.equal(restartPosts,0,'Enter must not prompt/restart without a new value')
 assert.equal(await page.getByRole('button',{name:'Conferma',exact:true}).count(),0)
 const restartButton=box.getByRole('button',{name:'Riavvia stampante',exact:true})
 await restartButton.click()
 assert.equal(await restartButton.isDisabled(),true,'restart button locked during confirmation')
 await page.getByRole('button',{name:'Conferma',exact:true}).click()
 await page.waitForFunction(()=>window.__CC2_PREVIEW.unload.restarts===1)
 assert.equal(restartPosts,1,'one restart POST for one confirmation')
 // Back to the printer's own value.
 await box.getByRole('button',{name:'Usa il valore predefinito della stampante',exact:true}).click()
 await page.getByRole('button',{name:'Annulla',exact:true}).click()
 assert.equal((await unload(page)).override,null)
 assert.equal(await input.inputValue(),'85')
 // A changed value still saves using Enter, with the same validation as the button.
 await input.fill('95')
 await input.press('Enter')
 await page.getByRole('button',{name:'Annulla',exact:true}).click()
 assert.equal((await unload(page)).override,95)
 assert.equal(savePosts,3,'button, restore default and changed Enter each save once')
 // A busy printer locks the field.
 await page.evaluate(()=>{window.__CC2_PREVIEW.status=2})
 await page.waitForFunction(()=>document.getElementById('canvas-unload').disabled)
 console.log('PASS Canvas pull-back: range checks, unchanged Enter no-op, single-flight save/restart, changed Enter save, default, busy lock')
}finally{await browser.close();await demo.cleanup()}
