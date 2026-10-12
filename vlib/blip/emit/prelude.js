// blip runtime. Compiled into `blip` via $embed_file and prepended to every
// module `blip emit-js` writes, so the emitted JavaScript is self-contained.
//
// This is an ASSET, not V source, on purpose. It used to live in a single-quoted
// V string, where every `\"` had to be escaped twice and a stray replacement
// could terminate the string silently. Keeping it as real JavaScript means the
// file can be read as what it is.

const blip = {
  add: (...a) => a.reduce((x, y) => x + y, 0),
  sub: (...a) => a.length === 1 ? -a[0] : a.reduce((x, y) => x - y),
  mul: (...a) => a.reduce((x, y) => x * y, 1),
  div: (a, b) => a / b,
  lt: (a, b) => a < b,
  gt: (a, b) => a > b,
  lte: (a, b) => a <= b,
  gte: (a, b) => a >= b,
  eq: (...a) => {
    for (let i = 1; i < a.length; i++) if (!blip_equal(a[i - 1], a[i])) return false;
    return true;
  },
  print: (...a) => {
    for (const x of a) console.log(blip_show(x));
    return null;
  },
  missing: (...a) => {
    throw new Error("blip: unimplemented primitive");
  },
  car: (x) => x[0],
  cdr: (x) => x.slice(1),
  cons: (x, y) => Array.isArray(y) ? [x].concat(y) : [x, y],
  p_null_q: (x) => x === null || x === undefined || (Array.isArray(x) && x.length === 0),
  not: (x) => !blip_truthy(x),
  p_equal_q: (a, b) => blip_equal(a, b),
  apply: (f, ...a) => f(...a),
  length: (x) => (Array.isArray(x) || typeof x === "string") ? x.length : null,
  vector_ref: (v, i) => v[i],
  vector_length: (v) => v.length,
  string_upcase: (s) => s.toUpperCase(),
  string_downcase: (s) => s.toLowerCase(),
  string_append: (...a) => a.join(""),
  // A rational is a tagged pair, not a JavaScript number. JavaScript has no
  // exact rational, and widening 1/3 to 0.3333333333333333 would make the
  // emitted program disagree with `blip run` on the value the numeric tower
  // exists to keep exact.
  rat: (n, d) => ({ blip_rat: [n, d] }),
};

// blip_show is the printer. It is separate from console.log because blip prints
// a string WITH its quotes, the way the blip printer does -- printing one without
// them would make emitted output disagree with `blip run`.
function blip_show(x) {
  if (x === null || x === undefined) return "null";
  // A rational prints as n/d, the way the blip printer prints one.
  if (x && x.blip_rat) return x.blip_rat[0] + "/" + x.blip_rat[1];
  if (typeof x === "string") return "\"" + x + "\"";
  if (Array.isArray(x)) return "(" + x.map(blip_show).join(" ") + ")";
  return String(x);
}

// blip_truthy: only #f is false, and everything else is true. JavaScript
// disagrees about null -- falsy there, true here -- so this is not `!x`.
function blip_truthy(x) {
  return !(x === false);
}

function blip_equal(a, b) {
  if (a === null || a === undefined) return b === null || b === undefined;
  if (Array.isArray(a) && Array.isArray(b)) {
    if (a.length !== b.length) return false;
    for (let i = 0; i < a.length; i++) if (!blip_equal(a[i], b[i])) return false;
    return true;
  }
  return a === b;
}
