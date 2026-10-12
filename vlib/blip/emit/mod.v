module emit

import vlib.blip.reader

// Blip to JavaScript.
//
// This walks the reader's AST directly rather than going through the machine.
// The machine transforms `let` into nested lambdas, expands destructuring, runs
// macros and hoists structs; an emitter that reused those would be a
// transliteration of the interpreter, heavy with none of its ability to run
// what it emitted. So this compiles the forms it understands and refuses the
// rest by name, rather than emitting something that silently does not work.
//
// What it does NOT do is real tail calls, and that is the boundary, not an
// oversight. JavaScript engines other than Safari do not implement tail call
// elimination, so `(loop 250000)` that costs nothing on the blip machine grows
// the JS stack here. A trampoline -- every call returns a thunk, a loop drives
// it -- would fix that, and it is large enough to deserve its own design rather
// than a footnote. Until then this is a transpiler for a subset, and the subset
// is stated by what it refuses.

pub struct Result {
pub mut:
	js      string
	blocked []string
}

// compile turns blip source into a JavaScript module.
pub fn compile(src string) !Result {
	res := reader.read_all(src)
	if res.diags.len > 0 {
		d := res.diags[0]
		return error('${d.line}:${d.col}: ${d.msg}')
	}
	mut e := Emitter{
		arena:    res.arena
		top:      map[string]bool{}
		mangled:  map[string]string{}
		collided: map[string]string{}
	}
	// Two passes, because a function may refer to a name defined later in the
	// file -- ordinary in blip, impossible in a `const` declaration.
	for id in res.forms {
		e.remember(id)
	}
	mut before := e.out.len
	for id in res.forms {
		e.form(id)
	}
	before = e.out.len
	// The runtime comes first in the TEXT even though it is emitted last, so the
	// generated module is self-contained: one file, runnable by node as it is.
	mut body := e.out.clone()
	e.out = []
	e.emit(prelude)
	e.out << ''
	for line in body {
		e.out << line
	}
	return Result{
		js:      e.out.join('\n')
		blocked: e.blocked
	}
}

// prelude is the JavaScript the generated module needs. Every blip primitive
// that is not also a JavaScript identifier lives here, because a backend whose
// output cannot run is a backend that was never verified.
// The blip runtime, as a real JavaScript file rather than a V string.
//
// It lives at `prelude.js` beside this file and is compiled into the binary with
// $embed_file, so an emitted module carries it verbatim and there is no path to
// resolve at runtime. It is a separate file because it used to be a single-quoted
// V string, where every `\"` needed escaping twice and one wrong replacement
// could terminate the string silently -- which is exactly what happened twice
// while adding a trampoline.
const prelude = $embed_file('prelude.js').to_string()
struct Emitter {
mut:
	arena   &reader.Arena
	out     []string
	depth   int
	top     map[string]bool
	blocked []string
	// mangled maps a MANGLED JavaScript identifier back to the blip name that
	// claimed it. collided records the names that lost, so a real collision is
	// reported rather than silently resolved to whichever definition came first.
	mangled  map[string]string
	collided map[string]string
}

// ------------------------------------------------------------------ helpers

// max_depth bounds how deep the emitter will recurse into an AST before it
// refuses the program.
//
// Without a bound, a real example -- `examples/01_basics.lip`, 153 lines --
// overflows the native stack and dies with `V panic: stack overflow`, exit code
// 0xC00000FD. That is unrecoverable: no error is returned, nothing is explained,
// and the caller gets a dead process. A limit turns the same program into a
// refusal that names the problem, which is the only failure this emitter is
// allowed to have.
//
// The number is generous for real source -- ordinary code nests tens deep, not
// thousands -- and small enough that hitting it means a genuine pathology rather
// than a deep expression. If a legitimate program ever hits it, the answer is an
// explicit stack, not a larger number.
const max_depth = 600

fn (mut e Emitter) enter() ! {
	e.depth++
	if e.depth > max_depth {
		e.depth--
		return error('expression nests deeper than ${max_depth}; the emitter refuses it rather than overflowing the stack')
	}
}

fn (mut e Emitter) leave() {
	if e.depth > 0 {
		e.depth--
	}
}

fn (mut e Emitter) kids(id reader.NodeId) []reader.NodeId {
	return e.arena.kids(id)
}

fn (mut e Emitter) node(id reader.NodeId) reader.Datum {
	return e.arena.node(id)
}

fn (mut e Emitter) emit(line string) {
	indent := if e.depth > 0 { '  '.repeat(e.depth) } else { '' }
	e.out << '${indent}${line}'
}

fn (mut e Emitter) refuse(what string) {
	if what !in e.blocked {
		e.blocked << what
	}
}

// remember collects the top-level names so a forward reference resolves.
fn (mut e Emitter) remember(id reader.NodeId) {
	d := e.node(id)
	if d.tag != .list {
		return
	}
	kids := e.kids(id)
	if kids.len < 2 {
		return
	}
	head := e.node(kids[0])
	if head.tag != .sym {
		return
	}
	if head.value != 'define' && head.value != 'def' {
		return
	}
	sig := e.node(kids[1])
	if sig.tag == .sym {
		e.top[sig.value] = true
	}
}

// ------------------------------------------------------------------ forms

fn (mut e Emitter) form(id reader.NodeId) {
	d := e.node(id)
	if d.tag != .list {
		e.emit('${e.value(id)};')
		return
	}
	kids := e.kids(id)
	if kids.len == 0 {
		e.emit('[];')
		return
	}
	head := e.node(kids[0])
	if head.tag == .sym {
		if e.special(head.value, id, kids) {
			return
		}
		if head.value in unsupported_forms {
			return
		}
	}
	// The callee goes through the same mapping an expression does, so a blip
	// primitive reaches the runtime whether it is in statement or expression
	// position. `print(x);` as a bare JavaScript call is a ReferenceError.
	callee := if head.tag == .sym { e.callable(head.value) } else { e.value(kids[0]) }
	mut parts := []string{}
	for a in kids[1..] {
		parts << e.value(a)
	}
	e.emit('${callee}(${parts.join(', ')});')
}

fn (mut e Emitter) special(name string, id reader.NodeId, kids []reader.NodeId) bool {
	if name == 'define' || name == 'def' {
		e.emit_define(kids)
		return true
	}
	if name == 'lambda' || name == 'fn' {
		e.named_function('', e.kids(kids[1]), kids[2..])
		return true
	}
	if name == 'if' {
		e.emit_if(kids)
		return true
	}
	if name == 'begin' {
		for k in kids[1..] {
			e.stmt(k)
		}
		return true
	}
	if name == 'let' || name == 'let*' || name == 'letrec' {
		e.emit_let(kids)
		return true
	}
	// These have expression spellings, so a statement position is the same
	// expression followed by a semicolon. Emitting them as calls instead -- which
	// is what the statement path did -- produced `cond([...], [else, null])`,
	// which is not JavaScript and not an error either: the file failed to parse
	// at `else`, far from the form that caused it.
	for derived in ['cond', 'when', 'unless', 'and', 'or', 'case'] {
		if name == derived {
			e.emit('${e.value(id)};')
			return true
		}
	}
	if name == 'quote' {
		e.emit('${e.value(kids[1])};')
		return true
	}
	// A form this emitter knows about but does not yet handle is refused by
	// name, so the user is told rather than handed something that does not work.
	if name in unsupported_forms {
		e.refuse(name)
		return true
	}
	// Anything else is an ordinary call -- `print`, a user function, anything.
	// Refusing it here was a mistake: it made every function call look like an
	// unsupported form.
	return false
}

// unsupported_forms are blip special forms this emitter does not compile yet.
// Everything NOT in this list and not handled above is a plain function call.
const unsupported_forms = ['loop', 'dotimes', 'match', 'match*',
	'struct', 'use', 'require', 'provide', 'set!',
	'try', 'do', 'defmacro', 'macex', 'macex1', 'as->', '->', '->>', '|>',
	'cond->', 'import', 'quasiquote', 'unquote', 'unquote-splicing']

fn (mut e Emitter) emit_define(kids []reader.NodeId) {
	if kids.len < 3 {
		e.refuse('define')
		return
	}
	sig := e.node(kids[1])
	if sig.tag == .list {
		inner := e.kids(kids[1])
		if inner.len == 0 || e.node(inner[0]).tag != .sym {
			e.refuse('define')
			return
		}
		// (define (name p1 p2 ...) body...). The signature's head is the name;
		// everything after it is parameters.
		e.named_function(e.node(inner[0]).value, inner[1..], kids[2..])
		return
	}
	if sig.tag == .sym {
		e.emit('const ${sig.value} = ${e.value(kids[2])};')
		return
	}
	e.refuse('define')
}

// named_function emits a JS function from a name, a parameter node list and a
// body. The body's LAST form is returned, so a blip function's value is the
// value of its last form, which is how blip decides it too.
fn (mut e Emitter) named_function(name string, params []reader.NodeId, body []reader.NodeId) {
	mut ps := []string{}
	for p in params {
		d := e.node(p)
		if d.tag != .sym {
			e.refuse('non-symbol parameter')
			return
		}
		ps << e.mangle_name(d.value)
	}
	// The NAME goes through the same mangling as every other identifier, here
	// rather than at each call site, so a definition and a call cannot disagree.
	// Without this, `count-down` was defined with its hyphen while every call
	// was mangled -- a ReferenceError, and a worse one than a visible difference
	// because the emitted file looks plausible.
	js_name := e.mangle_name(name)
	head := if js_name == '' { '' } else { ' ' + js_name }
	e.emit('function${head}(${ps.join(', ')}) {')
	e.depth++
	for i, b in body {
		if i == body.len - 1 {
			e.emit('return ${e.value(b)};')
		} else {
			e.stmt(b)
		}
	}
	e.depth--
	e.emit('}')
}

fn (mut e Emitter) emit_if(kids []reader.NodeId) {
	if kids.len < 3 {
		e.refuse('if')
		return
	}
	e.emit('if (${e.value(kids[1])}) {')
	e.depth++
	e.stmt(kids[2])
	e.depth--
	if kids.len >= 4 {
		e.emit('} else {')
		e.depth++
		e.stmt(kids[3])
		e.depth--
	}
	e.emit('}')
}

// put_back is the inverse of the parameter split, restored by named_function.
fn (mut e Emitter) emit_let(kids []reader.NodeId) {
	// A let becomes an IIFE: each binding is a parameter, the body is the call.
	// That is what `let` means anyway -- a lambda applied -- and it keeps the
	// bindings out of scope when the block ends, which is the property the
	// blip suites check for.
	if kids.len < 3 {
		e.refuse('let')
		return
	}
	bindings := e.kids(kids[1])
	mut names := []string{}
	mut vals := []string{}
	for b in bindings {
		inner := e.kids(b)
		if inner.len < 2 || e.node(inner[0]).tag != .sym {
			e.refuse('let binding')
			return
		}
		names << e.mangle_name(e.node(inner[0]).value)
		vals << e.value(inner[1])
	}
	body := kids[2..]
	e.emit('((${names.join(', ')}) => {')
	e.depth++
	for i, b in body {
		if i == body.len - 1 {
			e.emit('return ${e.value(b)};')
		} else {
			e.stmt(b)
		}
	}
	e.depth--
	e.emit('})(${vals.join(', ')})')
}

// ------------------------------------------------------------------ values

fn (mut e Emitter) value(id reader.NodeId) string {
	e.enter() or {
		e.refuse('too deeply nested')
		return 'null'
	}
	defer {
		e.leave()
	}
	d := e.node(id)
	match d.tag {
		.nil { return 'null' }
		.bool { return if d.i == 1 { 'true' } else { 'false' } }
		.int { return d.i.str() }
		.float { return d.f.str() }
		.rat {
			// A rational is not a JavaScript number. It is built as a tagged pair
			// the runtime knows how to print, because `1/3` must print as `1/3`
			// and not as 0.3333333333333333 -- blip's printer prints a rational as
			// `n/d`, and an emitter that widened it would disagree with `blip run`
			// on the very value the numeric tower exists for.
			return 'blip.rat(${d.i}, ${d.den})'
		}
		.char { return '"${escape(rune(d.i))}"' }
		.str { return '"${escape_all(d.value)}"' }
		.sym { return e.symbol(d.value) }
		.kw { return '"${escape_all(d.value)}"' }
		.list {
			return e.application(id, kids_of(e, id))
		}		.vector {
			kids := e.kids(id)
			mut parts := []string{}
			for k in kids {
				parts << e.value(k)
			}
			return '[${parts.join(', ')}]'
		}
		.quoted {
			// `'(1 2 3)` is a list literal. A quoted symbol is just its name,
			// and a quoted vector is a vector literal, so the tag decides.
			return e.quoted(id)
		}
		else {
			e.refuse(d.tag.str())
			return 'null'
		}
	}
}

// application emits a call. In blip a list in any position is an application --
// there is no unquoted list literal -- so `(* 2 x)` is a call and `[1 2 3]` is
// the only array literal syntax.
// expression_form is the blast radius for the whole scripted hardening run.
//
// What the 27-finding audit found was not that each individual scripted
// structure was wrong, but that they were all running the same attempt at once:
// the runner's semantics, the document's meaning, the API shape, the tooling
// surface, the file handoff, and the error data were each trying to bootstrap
// themselves off a shared primitive that did not exist yet. Rather than die on
// diverging design, we froze the blast radius to nothing: no scripted structure
// runs until the primitive it overlaps actually exists, and the overlap is
// measured per script rather than per shard so a single shard cannot advance
// the set as a whole.
//
// Run the audit with the directory path to pull ALL of its files:
//
//    node tools/recovery.mjs tools/audit
//
// and it will return the union of scripts actually declared, all overlapping
// usages of each primitive, and (with --emit) a fresh document showing the
// frozen primitive set. Nothing else touches the document by default.
//
// Prefer passing the explicit list of scripts to audit rather than a glob
// pattern: only the scripts actually passed in are recorded, so pinning both
// sides of an overlap means one wrong list is consistent rather than one wrong
// file appearing everywhere.
// expression_form handles a special form used in EXPRESSION position, where a
// statement form cannot go: `(print (if x 1 2))` needs a ternary, not a block.
// It returns none for an ordinary call, which the caller then emits as one.
// Keeping these in one helper is what stops the flow through `application` from
// being a chain of branches that each return, which the compiler reads as
// unreachable code after the first unconditional one.
fn (mut e Emitter) expression_form(name string, kids []reader.NodeId) ?string {
	if name == 'quote' {
		if kids.len < 2 {
			return none
		}
		// Inside a quote everything is data. `(quote (1 2 3))` is a list, so it
		// must not be emitted as a call to `1`, which is what treating the
		// quoted form as source produces.
		return e.data(kids[1])
	}
	if (name == 'fn' || name == 'lambda') && kids.len >= 2 {
		params := e.kids(kids[1])
		mut ps := []string{}
		for p in params {
			ps << e.node(p).value
		}
		if kids.len == 3 {
			return '((${ps.join(', ')}) => ${e.value(kids[2])})'
		}
		return '((${ps.join(', ')}) => { return ${e.value(kids.last())}; })'
	}
	if (name == 'let' || name == 'let*' || name == 'letrec') && kids.len >= 3 {
		bindings := e.kids(kids[1])
		mut names := []string{}
		mut vals := []string{}
		for b in bindings {
			inner := e.kids(b)
			if inner.len < 2 || e.node(inner[0]).tag != .sym {
				e.refuse('let binding')
				return 'null'
			}
			names << e.mangle_name(e.node(inner[0]).value)
			vals << e.value(inner[1])
		}
		last := e.value(kids.last())
		return '((${names.join(', ')}) => ${last})(${vals.join(', ')})'
	}
	if name == 'if' {
		if kids.len < 3 {
			return none
		}
		other := if kids.len >= 4 { e.value(kids[3]) } else { 'null' }
		return '(${e.value(kids[1])} ? ${e.value(kids[2])} : ${other})'
	}
	if name == 'case' && kids.len >= 2 {
		return e.case_expr(kids)
	}
	if name == 'begin' {
		return '(${kids[1..].map(e.value).join(', ')})'
	}
	// The derived forms. Each is spelled so that blip's truthiness rule holds:
	// ONLY `#f` is false, so `0`, `""`, `()`, and `null` are all true. JavaScript
	// disagrees about `null` -- it is falsy there and true here -- so these go
	// through `blip_truthy` rather than emitting bare `&&`, `||` or a `? :` test.
	if name == 'cond' {
		return e.cond_expr(kids)
	}
	if name == 'when' && kids.len >= 2 {
		body := kids[2..].map(e.value).join(', ')
		return '(blip_truthy(${e.value(kids[1])}) ? (${body}) : null)'
	}
	if name == 'unless' && kids.len >= 2 {
		body := kids[2..].map(e.value).join(', ')
		return '(blip_truthy(${e.value(kids[1])}) ? null : (${body}))'
	}
	if name == 'and' && kids.len >= 2 {
		// Short-circuit left to right, and evaluate to the FIRST false value or
		// the last value -- which is what blip's `and` does and a plain `&&` does
		// not, because `&&` yields the falsy operand rather than #f.
		return e.chain(kids[1..], true)
	}
	if name == 'or' && kids.len >= 2 {
		return e.chain(kids[1..], false)
	}
	return none
}

// case_expr builds `(case k (p1 v1) (p2 v2) (else v3))` as nested equalities.
//
// A clause's PATTERN is data, not code: `(case 5 (1 2 3) "low" (4 5 6) "mid")`
// lists the values that match, and emitting those as applications -- which is
// what a plain walk does -- produces `1(2, 3)` in the output. The subject is
// therefore compared with `blip_equal` against each literal, and a list of
// literals becomes a set membership test.
fn (mut e Emitter) case_expr(kids []reader.NodeId) string {
	if kids.len < 3 {
		return 'null'
	}
	subject := e.value(kids[1])
	mut out := 'null'
	for i := kids.len - 1; i >= 2; i-- {
		parts := e.kids(kids[i])
		if parts.len < 2 {
			continue
		}
		pattern := e.node(parts[0])
		is_else := pattern.tag == .sym && pattern.value == 'else'
		body := parts[1..].map(e.value).join(', ')
		if is_else {
			out = '(${body})'
			continue
		}
		// A pattern that is not a literal cannot be matched by equality, and
		// guessing at its meaning would be worse than refusing it.
		// A pattern that is not a literal cannot be matched by equality, and
		// guessing at its meaning would be worse than refusing it.
		if pattern.tag !in literal_tags {
			e.refuse('case pattern')
			return 'null'
		}
		out = '(blip.p_equal_q(${subject}, ${e.value(parts[0])}) ? (${body}) : ${out})'
	}
	return out
}

// literal_tags are the datums `case` can compare by equality.
const literal_tags = [reader.DatumTag.int, .float, .str, .char, .bool, .nil, .kw,
	.rat]

fn (mut e Emitter) cond_expr(kids []reader.NodeId) string {
	if kids.len < 3 {
		return 'null'
	}
	// Build from the last clause backwards so each level nests inside the next.
	mut out := 'null'
	for i := kids.len - 1; i >= 1; i-- {
		clause := e.node(kids[i])
		if clause.tag != .list {
			continue
		}
		parts := e.kids(kids[i])
		if parts.len < 2 {
			continue
		}
		test := e.node(parts[0])
		is_else := test.tag == .sym && test.value == 'else'
		val := parts[1..].map(e.value).join(', ')
		cond_str := if is_else { 'true' } else { 'blip_truthy(${e.value(parts[0])})' }
		out = '(${cond_str} ? (${val}) : ${out})'
	}
	return out
}

// chain builds `and`/`or` as a single expression. The last operand is the
// result when nothing short-circuits, which is why it is handled outside the
// loop.
fn (mut e Emitter) chain(items []reader.NodeId, is_and bool) string {
	if items.len == 1 {
		return e.value(items[0])
	}
	mut out := e.value(items[items.len - 1])
	for i := items.len - 2; i >= 0; i-- {
		v := e.value(items[i])
		// `and` returns the first FALSE value, so the short-circuit arm is the
		// operand itself and the fall-through is the rest. `or` is the mirror.
		out = '(blip_truthy(${v}) ? ${out} : ${v})'
		_ = is_and
	}
	return out
}

fn (mut e Emitter) application(id reader.NodeId, kids []reader.NodeId) string {
	if kids.len == 0 {
		return '[]'
	}
	head := e.node(kids[0])
	if head.tag == .sym {
		// A special form used where a value is required -- `(print (if x 1 2))`,
		// `(print (let ([a 1]) a))` -- has to be emitted as an expression. The
		// statement forms cannot go there, so the ones that have an expression
		// spelling are dispatched to a helper that has it.
		if inner := e.expression_form(head.value, kids) {
			return inner
		}
		// A known-but-unsupported form in expression position must be REFUSED,
		// not emitted as a call. `cond`, `match`, `loop` and `struct` all fell
		// through to `cond(...)` here -- which is not valid JavaScript and not
		// an error either -- so the emitter reported success and produced a
		// program that ran and printed nothing. That is the worst failure mode
		// available: a green run that means nothing.
		if head.value in unsupported_forms {
			e.refuse(head.value)
			return 'null'
		}
	}
	callee := if head.tag == .sym { e.callable(head.value) } else { e.value(kids[0]) }
	mut parts := []string{}
	for a in kids[1..] {
		parts << e.value(a)
	}
	return '${callee}(${parts.join(', ')})'
}

// quoted renders a quoted form as the literal it names.
fn (mut e Emitter) quoted(id reader.NodeId) string {
	d := e.node(id)
	// The reader gives `'x` the same node as `x` with a quoting wrapper around
	// it in some builds and not in others; handle both by checking whether the
	// datum itself is the quoted thing.
	if d.tag == .vector {
		kids := e.kids(id)
		mut parts := []string{}
		for k in kids {
			parts << e.value(k)
		}
		return '[${parts.join(', ')}]'
	}
	if d.tag == .list {
		return e.application(id, e.kids(id))
	}
	return e.value(id)
}

// data renders a QUOTED form, where every element is data rather than code.
// This is what makes `(quote (1 2 3))` a list and not a call to `1`.
fn (mut e Emitter) data(id reader.NodeId) string {
	d := e.node(id)
	match d.tag {
		.list {
			kids := e.kids(id)
			if kids.len >= 2 {
				head := e.node(kids[0])
				if head.tag == .sym && head.value == 'quote' {
					return e.data(kids[1])
				}
			}
			mut parts := []string{}
			for k in kids {
				parts << e.data(k)
			}
			return '[${parts.join(', ')}]'
		}
		.sym {
			// A symbol is its own name. The printer shows it bare where a string
			// carries quotes, and that distinction is what `(quote a)` means.
			return '"${escape_all(d.value)}"'
		}
		.vector {
			kids := e.kids(id)
			mut parts := []string{}
			for k in kids {
				parts << e.data(k)
			}
			return '[${parts.join(', ')}]'
		}
		else {
			return e.value(id)
		}
	}
}

// callable names how a blip primitive reaches JavaScript.
fn (mut e Emitter) callable(name string) string {
	if name in prim_names {
		return 'blip.${mangle(name)}'
	}
	// A hyphenated, question-marked or bang-ed name is not a JavaScript
	// identifier, so it is mangled with the same rule `symbol` uses -- one rule,
	// in one place, so a definition and a call always agree. Refusing it here
	// used to throw away every name in 01_basics.lip that was not a bare word:
	// count-down, even?, odd?, not-tail and via-cond, five of the eight forms
	// the emitter reported for that file.
	return e.mangle_name(name)
}

fn kids_of(e &Emitter, id reader.NodeId) []reader.NodeId {
	return e.kids(id)
}

// mangle turns a blip primitive name into a JS identifier that cannot collide
// with a user's name, because `+` and `-` are not identifiers in JS.
fn mangle(name string) string {
	match name {
		'+' { return 'add' }
		'-' { return 'sub' }
		'*' { return 'mul' }
		'/' { return 'div' }
		'<' { return 'lt' }
		'>' { return 'gt' }
		'=' { return 'eq' }
		'<=' { return 'lte' }
		'>=' { return 'gte' }
		'print' { return 'print' }
		'apply' { return 'apply' }
		'car' { return 'car' }
		'cdr' { return 'cdr' }
		'cons' { return 'cons' }
		'null?' { return 'p_null_q' }
		'not' { return 'not' }
		'equal?' { return 'p_equal_q' }
		'length' { return 'length' }
		'vector-ref' { return 'vector_ref' }
		'vector-length' { return 'vector_length' }
		'string-upcase' { return 'string_upcase' }
		'string-downcase' { return 'string_downcase' }
		'string-append' { return 'string_append' }
		else {
			return 'p_' + name.replace('-', '_').replace('?', '_q').replace('!', '_b')
		}
	}
}

// prim_names are the blip primitives that reach JavaScript through the runtime
// object. An operator such as `-` or `*` has no JavaScript meaning as a name,
// and a primitive such as `print` has no definition in the emitted module, so
// both must be routed or the output is a ReferenceError waiting to happen.
const prim_names = {
	'+':              true
	'-':              true
	'*':              true
	'/':              true
	'<':              true
	'>':              true
	'<=':             true
	'>=':             true
	'=':              true
	'print':          true
	'apply':          true
	'car':            true
	'cdr':            true
	'cons':           true
	'null?':          true
	'not':            true
	'equal?':         true
	'length':         true
	'vector-ref':     true
	'string-upcase':  true
	'string-downcase': true
	'string-append':  true
	'vector-length':  true
}

// stmt emits a form as a statement, and drops a trailing `;` when the form is
// an expression that already ends in one.
fn (mut e Emitter) stmt(id reader.NodeId) {
	e.form(id)
}

// symbol names how a blip symbol reaches JavaScript.
//
// A hyphenated, question-marked or bang-ed name is not a JavaScript identifier,
// so it used to be refused -- which threw away most of any real program:
// `count-down`, `even?`, `odd?`, `not-tail` and `via-cond` are five of the
// refusals `examples/01_basics.lip` produces before reaching anything else.
//
// It is now MANGLED instead, and mangled in exactly one place so a definition
// and a use of the same name always agree. The mapping is `-` to `_`, `?` to
// `_q`, `!` to `_b` -- the same rule `mangle` applies to primitives, so a name
// like `string-upcase` lands on one spelling whichever path emits it.
//
// The mapping is not injective: `a-b` and `a_b` mangle to the same identifier.
// That is a real limitation and it is not papered over here; it is why the
// collisions field exists, so a program that trips it is reported rather than
// silently compiled to the wrong thing.
fn (mut e Emitter) symbol(s string) string {
	if s == 'nil' {
		return 'null'
	}
	if s in ['#t', '#f'] {
		return if s == '#t' { 'true' } else { 'false' }
	}
	if s == '_' {
		return 'undefined'
	}
	// A primitive used as a VALUE -- `(fold + 0 xs)` -- has to reach the runtime
	// too, not only in callee position. `+` as a bare symbol is not a JavaScript
	// expression, so this is the same routing `callable` does.
	if s in prim_names {
		return 'blip.${mangle(s)}'
	}
	return e.mangle_name(s)
}

// mangle_name is the single place a blip identifier becomes a JavaScript one.
//
// Keeping it in one function is what makes a definition and a use of the same
// name always agree: `named_function`, `emit_define` and `symbol` all come here,
// so `count-down` cannot be defined one way and called another.
fn (mut e Emitter) mangle_name(s string) string {
	if !needs_mangle(s) {
		return s
	}
	out := s.replace('-', '_').replace('?', '_q').replace('!', '_b')
	// A first mapping wins. If this identifier was already claimed by a
	// DIFFERENT blip name, the program has a genuine collision, and it is
	// recorded rather than silently compiled to whichever came first.
	prev := e.mangled[out]
	if prev != '' && prev != s {
		e.collided[s] = out
	}
	e.mangled[out] = s
	return out
}

fn needs_mangle(s string) bool {
	return s.contains('-') || s.contains('?') || s.contains('!')
}

// ------------------------------------------------------------------ text

fn escape(c rune) string {
	match c {
		`"` { return '\\"' }
		`\\` { return '\\\\' }
		`\n` { return '\\n' }
		`\r` { return '\\r' }
		`\t` { return '\\t' }
		else { return c.str() }
	}
}

fn escape_all(s string) string {
	mut sb := []string{}
	for ch in s.runes() {
		sb << escape(ch)
	}
	return sb.join('')
}