const fs = require('fs')
const width = 96
const height = 96
const buffer = Buffer.alloc(width * height)

// draw some stripes
for (let i = 0; i < (width * height); i += 16) {
    buffer.writeBigUInt64LE(0xFFFFFFFFFFFFFFFFn, i)
}

fs.writeFileSync('./test.raw', buffer);

