import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'

/**
 * Measures text with Core Text. A framesetter remembers the last text it measured, so measuring
 * the same text at several sizes is cheap.
 */
interface CoreTextFramesetter {
  /**
   * Measure `string` in one font, fitted into `width` by `height` points.
   * @param font - The font `family`, or `null` for the system font, and its `size` in points.
   * Defaults to the system font at 13 points.
   * @param width - Defaults to `Infinity`.
   * @param height - Defaults to `Infinity`.
   */
  measure(
    string: string | null,
    font?: CoreTextFramesetter.Font,
    width?: number,
    height?: number
  ): CoreTextFramesetter.Measurement

  /**
   * Measure an attributed string from another addon, such as an `NSAttributedString`, fitted into
   * `width` by `height` points. Use this for text with more than one font or style.
   * @param width - Defaults to `Infinity`.
   * @param height - Defaults to `Infinity`.
   */
  measureAttributed(text: Wrapper, width?: number, height?: number): CoreTextFramesetter.Measurement

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class CoreTextFramesetter {
  /** Create a new framesetter. */
  constructor()
}

declare namespace CoreTextFramesetter {
  export interface Font {
    family?: string | null
    size?: number
  }

  /**
   * The size of the text and the number of lines it takes up. The width is rounded up to whole
   * points. The height is what Core Text reports, because each control places its lines in its
   * own way.
   */
  export interface Measurement {
    width: number
    height: number
    lines: number
  }
}

export = CoreTextFramesetter
