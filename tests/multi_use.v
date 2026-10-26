module main

import vlib.blip.host
import vlib.blip.machine

fn main() {
	h := &host.ConsoleHost{}
	mut m := machine.new_standalone(h)
	_ := m.run_str("(define x 42)")
	_ := m.run_str("(+ x 1)")
	println('PASS multi-source use gate')
}
