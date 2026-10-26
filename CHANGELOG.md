# Changelog

Recent changes and the work behind them, newest first. Each entry names its
gate: a change without a test that fails without it does not land. The plan
lives in `docs/010-roadmap.md`; this file is the record.

## Unreleased

### Hostile input is an error, not a process death (`tests/hostile.v`)

- The promise "errors as values" had a hole every wrong call fell through:
  V's **assert** is not recoverable, so `(car)`, `(format 42 "~a")`,
  `({:a 1} 5)`, `(raise (err 42))`, and nine more measured wrong calls
  printed `V panic:` -- or died with an index-out-of-range -- and took the
  whole `blip run` process down. A configuration language has one job above
  all: a config file that says something wrong must be able to say it as an
  error and let the user fix it, never abort a build/REPL.
- The fix has two walls and a gate. The `Value` accessors
  (`as_string/as_pair/as_vector/as_table/as_struct/as_closure/as_int/...`)
  now check with a RECOVERABLE `panic(...)` that names the accessor and the
  tag it met -- V assertions kill the process and no defer can intercept
  them, which is the measured fact this is built on. And `drive`, the one
  funnel every evaluation flows through, carries a `defer`/`recover` safety
  net exactly one frame deep: a panic anywhere inside the machine becomes
  `error: internal error: as_table: called on a integer, not a table`
  through the same source-context decoration every other machine error
  gets. The net lives NOWHERE ELSE, so the reader, the emitter, and the
  suites that call step functions directly still die loudly, and a
  machinery bug above the boundary is not hidden behind a recovered value.
- The gate is `tests/hostile.v`: 38 hostile sources + 4 CLI misuse cases,
  one child process per case (an in-process probe would die at the first
  panic and never name the culprit), asserting no crash exit code and no
  `V panic` in the output. 12/38 crashed when it was written; all 43 pass
  now. Registered in ci.yml.
- One CLI case, deliberately not a machine matter: `blip frobnicate`
  (neither subcommand nor file) said "cannot read frobnicate" and exited 1;
  it now names both readings and exits 2. The `blip file.lip` shorthand
  still runs files without `blip run`.

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
`