const binding = require('../binding')
const registry = require('bare-foundation-registry')

module.exports = exports = class CoreTextFramesetter {
  constructor(opts = {}) {
    const { tag = null } = opts

    this._tag = tag === null ? binding.framesetterInit() : tag

    this._token = binding.claim(this._tag, this)
  }

  get [registry.tag]() {
    return this._tag
  }

  get [registry.handle]() {
    return binding.handle(this._tag)
  }

  measure(string, font = {}, width = Infinity, height = Infinity) {
    const { family = null, size = 13 } = font

    return binding.framesetterMeasure(this._tag, string, family, size, width, height)
  }

  measureAttributed(text, width = Infinity, height = Infinity) {
    return binding.framesetterMeasureAttributed(
      this._tag,
      registry.adopt(binding, text),
      width,
      height
    )
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: CoreTextFramesetter }
    }
  }
}
