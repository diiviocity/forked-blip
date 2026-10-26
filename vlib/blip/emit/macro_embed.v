// Macro emit-time embedded machine — Option 1 (design memo docs/macros-design.md).
// Status: stub wired to pure-shape conversion; complex shapes deferred.
// expand_macro_at_embed is exercised by tests/macros_gate.v but not yet
// called from emit/mod.v statement dispatch — defmacro/macex/macex1 stay in
// unsupported_forms until the emitter can render expanded forms as JS.

module emit

import vlib.blip
import vlib.blip.machine
import vlib.blip.host

// expand_macro_at_embed evaluates a macro call using a fresh embedded machine,
// then converts the expanded Value back to AST via value_to_datum.
// Not called from emit/mod.v yet — requires the emitter to render NodeId as JS.
pub fn expand_macro_at_embed(src string, h host.Host) !string {
	mut m := machine.new_standalone(h)
	expanded := m.run_str(src)!
	// Pure-shape conversion: nil/boolean/int/float/string/symbol/keyword/pair/vector
	_ = blip.value_to_datum(mut m, expanded)!
	return 'embedded-machine macro expansion: pure-shape converted (complex deferred)'
}
