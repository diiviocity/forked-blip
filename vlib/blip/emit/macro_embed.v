// Macro emit-time embedded machine — Option 1 (design memo docs/macros-design.md).
// Status: stub. Not yet wired into emit/mod.v unsupported_forms removal.

module emit

import vlib.blip
import vlib.blip.machine
import vlib.blip.host

// expand_macro_at_emit evaluates a macro call using a fresh embedded machine,
// then converts the expanded Value back to AST via value_to_datum.
// Not called from emit/mod.v yet — requires gate to pass first.
pub fn expand_macro_at_emit(src string, h host.Host) !string {
	mut m := machine.new_standalone(h)
	_ = m.run_str(src)!
	// Conversion from expanded Value -> NodeId would go here (value_to_datum).
	return 'embedded-machine stub: macro expansion not yet emitted'
}
