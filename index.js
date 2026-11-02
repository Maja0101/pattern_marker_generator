const path = require('path');

const candidates = [
  path.join(__dirname, 'build', 'Release', 'pmg_engine.node'),
  path.join(__dirname, 'build', 'pmg_engine.node'),
];

let native;
for (const file of candidates) {
  try { native = require(file); break; } catch (e) { if (e.code !== 'MODULE_NOT_FOUND') throw e; }
}
if (!native) throw new Error('pmg_engine.node not found - run "npm run build" first');

module.exports = native;
