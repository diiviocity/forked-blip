# Changelog

Recent changes and the work behind them, newest first. Each entry names its
gate: a change without a test that fails without it does not land. The plan
lives in `docs/010-roadmap.md`; this file is the record.

## Unreleased

### The JavaScript backend, measured example by example (`blip emit-js`)

- The bar is not "it emits" -- it is that the emitted JavaScript, run on
  node, prints the SAME bytes as `blip run` on the same source. Each new
  form lands only with that comparison, because the previous failure mode
  was acceptance with no output: a green run that means nothing.
- `02_data.lip` is the first example the backend reproduces byte for byte:
  `(1 "two" :three)`, `[1 2 3]`, `{:a 1 :b [2 3]}`. It needed the table,
  array and buffer literals; `list`/`append` and the data batch
  (`array-length`, `array-ref`, `array-set!`, `array-push!`, `put!`,
  `string-split`, `string-trim`, `string-index-of`, `take`); the type
  predicates; `format`; `has-key?`; `string->number`/`number->string`;
  and two rules the emitter had no spelling for -- a KEYWORD in call
  position is a lookup (`(:a {:a 1})`, not a call to a string), and a
  keyword inside quoted data stays a keyword, not a string.
- `->`/`->>`/`|>` thread through `tramp` steps, and an inner thread
  splices into the outer one instead of nesting; `as->` and `cond->` are
  refused by name rather than emitted wrong. `match` handles literals,
  `_` and bare-symbol binding; `struct`, `even?`/`odd?` and tagged
  vectors (so the printer keeps `[1 2 3]` and `(1 2 3)` apart) are in.
- `form()` no longer spells out a call a second time: the statement path
  delegates to `application()`, so a callee routing fix cannot apply to
  expressions and miss statements -- which is exactly how the keyword
  lookup broke in one position and not the other.
- Gate: `tests/emit_js.v`, 18 checks, the last of which is `02_data`
  output against the interpreter and was verified to FAIL when the
  keyword-tag fix is removed. Refusal list is still enforced per example:
  `03_match`, `04_errors`, `05_pipelines`, `06_modules` refuse by name
  rather than compile to something wrong.
- Known red for pre-existing reasons: `tools/genprims.vsh --check`
  reports `stale gen_math.v` on a clean tree as well (a `v ast` failure
  on the runner's toolchain, the same class as the documented
  `src.c:1280` flake), so that gate is unverified on this host.

### Generated prims from vlib (`tools/genprims.vsh`)

- `tools/genprims.vsh` reads `v ast -p` JSON for allowlisted (module, file,
  fn) triples, classifies each signature, and emits
  `vlib/blip/prims/gen_<module>.v` plus a `register_gen_*` function. Only
  exact mappings generate (int family, f64, string, bool, void); options,
  results, collections, and f32 are reported deferred, methods/`mut`
  params/generics/tuples/pointers are skipped with reasons.
- The AST reports no visibility, so the allowlist is curated, not bulk:
  each name is verified against the AST, `pub` is checked in the source
  text, collisions with hand-written prims fail, and missing names fail
  (the upstream-rename signal). Convertible-but-unlisted fns are reported
  for the sync review to bless.
- Pilot: 8 `math-*` prims (`math-sin`, `math-cos`, `math-floor`,
  `math-log`, `math-log10`, `math-log2`, `math-factorial`, `math-exp`).
- New `code->string` prim (JSON `\b`/`\f`/`\u` escapes forced it into
  existence: no other way to materialize a character the reader cannot
  write). Registered the implemented-but-never-registered `string->rune`,
  whose absence silently broke brainfuck's `,` path.
- Gate: `tests/genprims.v` (values, arity/type errors, skipped-stays-unbound).
- Sync: roadmap §7.1 step 4 + CI `Generated prims are current` step
  (`--check` fails on drift; CRLF-insensitive).

### JSON parse + emit (`programs/json.lip`)

- Recursive-descent parser returning `(ok value pos)` / `(err reason pos)`;
  objects become tables with string keys, arrays become vectors, full
  escape and `\u` support, hand-checked number grammar (leading zeros,
  bare points, and trailing commas all fail). `json-emit` with round-trip
  self-tests; ends with the `json ok` marker.
- Gate: `tests/programs_run.v` (brainfuck + json markers in fresh machines).

### `use` single-form body fix (machine)

- `enter_clause_body` destructured any bare list body as a head-plus-
  sequence: `(use (three) (+ r1 r2))` answered 20 instead of 30, and a
  `let` in the same position reported its own name unbound. Nested
  single-`use` bodies in the examples passed anyway (re-evaluating the
  inner call is idempotent there), which is why no suite caught it. Only
  `begin`-headed lists destructure now; everything else evaluates as one
  form under the frame. kont=0 behavior unchanged.
- Regression tests in `examples/04_errors.lip`.

### Rust-style error frames

- Errors carry file, line, column, the source line, and a `^^^` underline
  (reader records line/col per datum; `Machine.src_text` +
  `decorate_error`). REPL, `run_files`, and `test_file` print them;
  the golden fixture expects the `error: ` prefix.

### Playground (`site/playground.html` + `server.v`)

- veb server: `POST /api/run` evaluates blip and answers JSON; the page
  posts form-encoded code, like the tour. Static serving needed dotted
  MIME keys (`app.static_mime_types['.ps1']`) — an unknown type aborts the
  whole scan, silently unrouting every later file.
- Verified by `tools/check_playground.vsh` (build, serve, API, page,
  error case).

### Brainfuck tape fix

- `make-tape` was O(n²) `vector-append` recursion (heap crash on Windows);
  now `array-push!` + `dotimes`.

### Roadmap: language server + CLI plans (§8, §9)

- §8: language server + VS Code extension as the last phase before WASM,
  reusing `../vls` (transport/handlers/diagnostics shape, not its
  analysis) and `../vscode-vlang` (grammar, client, config). v1 is
  diagnostics + highlighting + snippets.
- §9: CLI shaped like `v`'s (`cmd/v/v.v` dispatch), ordered
  version/help/run/test/lsp/fmt/vet/doc/new/build; `build` waits for WASM.
