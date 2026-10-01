# bare-core-text

Core Text for Bare. Use it to find out how much room a piece of text needs, for example to size a label before laying out a view.

A framesetter remembers the last text it measured, so measuring the same text at several widths is cheap.

```
npm i bare-core-text
```

## Usage

```js
const { Framesetter } = require('bare-core-text')

const framesetter = new Framesetter()

const { width, height, lines } = framesetter.measure(
  'The quick brown fox jumps over the lazy dog',
  { size: 15 },
  120
)
```

Text with more than one font or style is measured as an attributed string from another addon, such as `bare-app-kit`:

```js
const { AttributedString } = require('bare-app-kit')

const text = new AttributedString({ string: 'Hello, world' })

framesetter.measureAttributed(text, 120)
```

## License

Apache-2.0
