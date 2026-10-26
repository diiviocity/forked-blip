// Macro embedded-machine gate — fails until embedded machine at emit time works.
// Per docs/macros-design.md Option 1: machine.new_standalone inside emitter.
module main

import vlib.blip

fn main() {
	println('FAIL macro gate: embedded machine at emit time not implemented')
	exit(1)
}
