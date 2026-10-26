# Changelog

Recent changes and the work behind them, newest first. Each entry names its
gate: a change without a test that fails without it does not land. The plan
lives in `docs/010-roadmap.md`; this file is the record.

### CI and the Docker images now pass -d use_bundled_libgc to Makefile

- The `make -f Makefile v` step used the wrong static library lookup path:
  `thirdparty/tcc/lib/libgc.a`. The CI runner and Docker images installed
  libgc-dev (gcc's bundled Boehm GC) via system package managers but V looked
  in tcc's lib directory. Passing `-d use_bundled_libgc` to Makefile's VFLAGS
  tells V to build Boehm GC from the source in `thirdparty/libgc`, which is
  the expected workflow and no longer requires system libgc at a wrong path.
  Both the `test` job and the `docker-images` job now get past the "Build V"
  step.

### Reading and writing configuration: `read-file`, `write-file`, `file-exists?`, `env` (`programs/config.lip`)

### Reading and writing configuration: `read-file`, `write-file`, `file-exists?`, `env` (`programs/config.lip`)

- blip had no way to touch the filesystem or the environment, so a "config
  language" could not read a config file. Four hand-written primitives land
  in `vlib/blip/prims/mod.v`: `read-file` and `write-file` answer Result
  values (`(ok ...)` / `(err ...)`), so a missing file is a value that
  composes with `use` rather than an abort; `file-exists?` is a boolean;
  `env` returns the variable's value or nil when it is unset.
- `programs/config.lip` exercises all four end to end: it writes a small
  key: value file, reads it back, folds the lines into a table, reports each
  value, and prints `config ok`. Registered in `tests/programs_run.v`.

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

### The install pins the compiler, like CI does

- `install.sh` and `install.ps1` (and the `site/` copies the website hands
  out) built V from `git clone https://github.com/vlang/v` at whatever HEAD
  was that morning. That is the same unpinned-Makefile hazard CI documented
  and fixed in ci.yml, moved to the user's machine: between the pin
  `0137eb5` and upstream master sit 1123 commits, and somewhere in there the
  checker rejects source that the pinned compiler accepts -- so the install
  works on Friday and fails on Monday, with nothing in either repo to blame.
- Both scripts now build V and vc at exactly the pair ci.yml pins
  (`V_COMMIT` / `VC_COMMIT`), record the pin in a marker file so a re-run
  does not rebuild what is already right, fail fast with per-platform
  guidance when no C compiler is installed, and locate `blip`-relative things
  the same way CI does. The two pins now appear in four files; the gate
  `tests/unit.v install_pins` reads ci.yml and all four scripts and fails on
  any of them going missing or unpinned.

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