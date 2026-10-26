// P1 multi-source use gate: loading a second source must see definitions
// from the first, because use evaluates into the running machine (section 2).

import vlib.blip
import vlib.blip.machine
import vlib.blip.host

fn main() {
	h := &host.ConsoleHost{}
	mut m := machine.new_standalone(h)

	// First source defines a name.
	m.run_str('(define x 42)')!

	// Second source uses it; if definitions do not cross the boundary,
	// evaluating (+ x 1) fails with an unbound identifier.
	m.run_str('(+ x 1)')!
	println('PASS multi-source use: x visible across load')
}
