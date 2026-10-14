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
  car: (x) => unvec(x)[0],
  cdr: (x) => unvec(x).slice(1),
  cons: (x, y) => [x].concat(unvec(y)),
  p_null_q: (x) => x === null || x === undefined
    || (Array.isArray(unvec(x)) && unvec(x).length === 0),
  not: (x) => !blip_truthy(x),
  p_equal_q: (a, b) => blip_equal(a, b),
  apply: (f, ...a) => f(...a),
  length: (x) => {
    const items = unvec(x);
    return Array.isArray(items) || typeof items === "string" ? items.length : null;
  },
  vector_ref: (v, i) => unvec(v)[i],
  vector_length: (v) => unvec(v).length,
  string_upcase: (s) => s.toUpperCase(),
  string_downcase: (s) => s.toLowerCase(),
  string_append: (...a) => a.join(""),
  string_length: (s) => (typeof s === "string" ? s.length : 0),
  // A rational is a tagged pair, not a JavaScript number. JavaScript has no
  // exact rational, and widening 1/3 to 0.3333333333333333 would make the
  // emitted program disagree with `blip run` on the value the numeric tower
  // exists to keep exact.
  rat: (n, d) => ({ blip_rat: [n, d] }),
  // A symbol is tagged so the printer can show it bare, the way `blip run`
  // does. Emitting one as a plain string printed `done` as `"done"`.
  sym: (name) => ({ blip_sym: name }),
  // A vector is a tagged array, not a bare array. `[1 2 3]` and `(list 1 2 3)`
  // are different values in blip -- the printer writes one `[1 2 3]` and the
  // other `(1 2 3)` -- so the literal needs a marker the printer can see.
  // Without it every vector printed as a list, which is the mismatch that
  // stopped the two threading checks from passing.
  vec: (...items) => ({ blip_vec: items }),
  // Higher-order forms. Each drives its callback through `tramp`, because a
  // user function emitted by this backend returns a thunk when its body ends in
  // a tail call -- and most of them do. Using the thunk directly would hand it
  // to `blip.mul` instead of the value, and the failure would appear wherever the
  // result was first used rather than at the call.
  //
  // `map` and `filter` preserve the container, which is the rule blip settled:
  // `(map f [1 2 3])` is `[1 4 9]`, a vector, not a list. Each unwraps a tagged
  // vector first and rewraps after, so the container survives the call.
  map: (f, coll) => {
    const items = unvec(coll);
    return Array.isArray(items) ? rewrap(coll, items.map((x) => tramp(f(x)))) : coll;
  },
  filter: (f, coll) => {
    const items = unvec(coll);
    return Array.isArray(items)
      ? rewrap(coll, items.filter((x) => blip_truthy(tramp(f(x)))))
      : coll;
  },
  vector_map: (f, coll) => {
    const items = unvec(coll);
    return Array.isArray(items)
      ? rewrap(coll, items.map((x) => tramp(f(x))))
      : coll;
  },
  fold: (f, init, coll) => {
    const items = unvec(coll);
    return Array.isArray(items)
      ? items.reduce((acc, x) => tramp(f(acc, x)), init)
      : init;
  },
  // `|>` is a pipe: (x |> f) is (f x). It is also the most common bare-symbol
  // step inside `->>`, so it has to be a real function and not only syntax.
  // Called with one argument it is the identity: a bare `|>` step becomes the
  // single-arg call `(pipe acc)`, and blip's own `|>` on one argument returns
  // the argument unchanged (machine/mod.v pipe_one: a non-call step becomes
  // `(step acc)`, so `(|> "hello")` is a call whose value is `"hello"`).
  pipe: (x, f) => (f === undefined ? x : tramp(f(x))),
  reverse: (coll) => {
    const items = unvec(coll);
    return Array.isArray(items) ? rewrap(coll, items.slice().reverse()) : coll;
  },
  // `sort` is ascending, and it copies rather than sorting in place: blip's
  // collections are values, and sorting the caller's array would make
  // `(->> [3 1 2] |> sort)` change what the caller holds.
  sort: (coll) => {
    const items = unvec(coll);
    return Array.isArray(items)
      ? rewrap(coll, items.slice().sort((a, b) => (a < b ? -1 : a > b ? 1 : 0)))
      : coll;
  },
  // `even?` and `odd?` are primitives in blip (prims/mod.v). They are not the
  // `letrec` bindings in examples/01_basics.lip -- those shadow them locally,
  // and the top-level `(filter even? ...)` uses the primitive. An emitter with
  // no binding for them emits a call to a name nobody defined.
  even_q: (n) => unvec(n) % 2 === 0,
  odd_q: (n) => unvec(n) % 2 !== 0,
};

// unvec unwraps a tagged vector to its items, and returns anything else
// unchanged. Every collection function goes through it, because a tagged vector
// is not an Array and `Array.isArray` is false for one -- which would make
// `sort`, `map` and `reverse` silently pass a vector through untouched.
function unvec(x) { return x && x.blip_vec ? x.blip_vec : x; }

// rewrap puts the items back into a tagged vector when the input was one, so
// `sort` of a vector is still a vector. Without this the thread step would
// change the container and the printed spelling would drift.
function rewrap(orig, items) {
  return orig && orig.blip_vec ? { blip_vec: items } : items;
}

// blip_show is the printer. It is separate from console.log because blip prints
// a string WITH its quotes, the way the blip printer does -- printing one without
// them would make emitted output disagree with `blip run`.
function blip_show(x) {
  // `null`/`undefined` is blip's `nil`, and the printer spells it `nil`. Returning
  // `null` here made `(match 7 [1 'one])` print `null` where blip prints `nil`.
  if (x === null || x === undefined) return "nil";
  // A rational prints as n/d, the way the blip printer prints one.
  if (x && x.blip_rat) return x.blip_rat[0] + "/" + x.blip_rat[1];
  // A symbol prints bare: `done`, not `"done"` and not `'"done"'`.
  if (x && x.blip_sym !== undefined) return x.blip_sym;
  // A vector prints `[...]`, the way the blip printer writes one.
  if (x && x.blip_vec) return "[" + x.blip_vec.map(blip_show).join(" ") + "]";
  // A struct prints the way `blip run` prints one: (Name field field ...).
  if (x && x.blip_struct !== undefined) {
    const vals = Object.keys(x)
      .filter((k) => k !== "blip_struct")
      .map((k) => blip_show(x[k]));
    return "(" + x.blip_struct + " " + vals.join(" ") + ")";
  }
  if (typeof x === "string") return "\"" + x + "\"";
  if (Array.isArray(x)) return "(" + x.map(blip_show).join(" ") + ")";
  // A boolean prints `#t` or `#f`, the way the blip printer spells it.
  if (x === true) return "#t";
  if (x === false) return "#f";
  return String(x);
}

// blip_truthy: only #f is false, and everything else is true. JavaScript
// disagrees about null -- falsy there, true here -- so this is not `!x`.
function blip_truthy(x) {
  return !(x === false);
}

function blip_equal(a, b) {
  if (a === null || a === undefined) return b === null || b === undefined;
  const ua = unvec(a);
  const ub = unvec(b);
  if (Array.isArray(ua) && Array.isArray(ub)) {
    if (ua.length !== ub.length) return false;
    for (let i = 0; i < ua.length; i++) if (!blip_equal(ua[i], ub[i])) return false;
    return true;
  }
  return a === b;
}

// A trampoline, which is the only way to give a backend tail calls on an engine
// that does not have them.
//
// JavaScript engines other than Safari do not implement tail call elimination,
// so `(loop 250000)`, which costs nothing on the blip machine, grows the JS
// stack and dies. A trampoline fixes it by making a tail call return a THUNK
// instead of calling: the caller returns it verbatim, so no frame is added, and
// `tramp` drives the loop.
//
// The rule the emitter follows: a call in TAIL position returns
// `thunk(() => ...)`, and a call in any other position is wrapped in `tramp`,
// because that caller's frame is still needed to receive the value. The
// difference between those two spellings is the whole mechanism.
function thunk(f) { return { __blip_thunk: f }; }

function tramp(first) {
  let step = first;
  let guard = 0;
  while (step && step.__blip_thunk) {
    step = step.__blip_thunk();
    if (++guard > 1e8) {
      throw new Error("blip: trampoline did not terminate");
    }
  }
  return step;
}
