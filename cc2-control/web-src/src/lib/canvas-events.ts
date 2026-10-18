import { store } from './store'

// These are observations of CC2's existing Canvas slot readbacks. They are
// NOT confirmations of physical filament movement, purge success or failure.
export type ObservedState = 'empty' | 'present' | 'loaded' | 'transition'
export type CanvasObservation = {
  id: number
  time: number
  slot: number
  from: ObservedState
  to: ObservedState
}

const LIMIT = 24
let previous: (ObservedState | null)[] | null = null
let sequence = 0

const normalize = (value: unknown): ObservedState | null =>
  value === 'empty' || value === 'present' || value === 'loaded' || value === 'transition' ? value : null

export const canvasTimeline = store({ events: [] as CanvasObservation[] })

export function observeCanvasSlots(connected: boolean, controls: unknown, now = Date.now()) {
  // A stale/disconnected/unrecognized readback invalidates our comparison
  // baseline. Never invent a slot change after telemetry was unavailable.
  if (!connected || !Array.isArray(controls)) {
    previous = null
    return
  }
  const rows = controls.filter(row => row && typeof row === 'object' && !Array.isArray(row))
  const current = Array.from({ length: 4 }, (_, slot) => {
    const matching = rows.filter(item => (item as { slot?: unknown }).slot === slot)
    const row = matching.length === 1 ? (matching[0] as { state?: unknown }) : undefined
    return normalize(row?.state)
  })
  if (previous) {
    const changes: CanvasObservation[] = []
    for (let slot = 0; slot < 4; slot++) {
      const from = previous[slot]
      const to = current[slot]
      if (from && to && from !== to) changes.push({ id: ++sequence, time: now, slot, from, to })
    }
    if (changes.length) {
      canvasTimeline.set(({ events }) => ({ events: [...changes.reverse(), ...events].slice(0, LIMIT) }))
    }
  }
  previous = current
}

export function clearCanvasTimeline() {
  canvasTimeline.set({ events: [] })
}
