import { chromium } from 'playwright'
import assert from 'node:assert/strict'
import { fixture, launchOptions } from './fixture.mjs'
const demo=await fixture()
const browser=await chromium.launch(launchOptions)
const unload=page=>page.evaluate(()=>({...window.__CC2_PREVIEW.unload}))
try {
 const page=await browser.newPage({viewport:{width:1920,height:1080}})
 await page.goto(demo.url+'#canvas')
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
 await box.getByRole('button',{name:'Riavvia stampante',exact:true}).click()
 await page.getByRole('button',{name:'Conferma',exact:true}).click()
 await page.waitForFunction(()=>window.__CC2_PREVIEW.unload.restarts===1)
 // Back to the printer's own value.
 await box.getByRole('button',{name:'Usa il valore predefinito della stampante',exact:true}).click()
 await page.getByRole('button',{name:'Annulla',exact:true}).click()
 assert.equal((await unload(page)).override,null)
 assert.equal(await input.inputValue(),'85')
 // A busy printer locks the field.
 await page.evaluate(()=>{window.__CC2_PREVIEW.status=2})
 await page.waitForFunction(()=>document.getElementById('canvas-unload').disabled)
 console.log('PASS Canvas pull-back: printer value, range checks, save, restart now or later, default, busy lock')
}finally{await browser.close();await demo.cleanup()}
