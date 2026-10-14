// Node.js hosts compiled Xu; all arithmetic, loops and recursion run in WASM.
import assert from 'node:assert/strict';
import {readFile} from 'node:fs/promises';
import {fileURLToPath} from 'node:url';

const path = process.argv[2] || fileURLToPath(new URL('../../build/52_wasm.wasm', import.meta.url));
const module = await WebAssembly.compile(await readFile(path));
assert.deepEqual(WebAssembly.Module.imports(module), []);
const exp = new WebAssembly.Instance(module).exports;
const slots = new DataView(exp.memory.buffer);
function call(name, ...args) {
  args.forEach((value, i) => slots.setBigInt64(800 - i * 8, BigInt(value), true));
  exp[name + '_frame'](800, 768);
  return slots.getBigInt64(768, true);
}
for (const [name, args, expected] of [
  ['add', [20, 22], 42n],
  ['add', [-20, -22], -42n],
  ['fibonacci', [10], 55n],
  ['sum_to', [100], 5050n],
  ['main', [], 0n]
]) {
  const actual = call(name, ...args);
  assert.equal(actual, expected);
  console.log(`${name}(${args.join(', ')}) = ${actual}`);
}
