// value_to_datum — reverse of datum_to_value (machine/mod.v 2596).
// Status: stub (design memo docs/macros-design.md). Gate passes for nil only.

module blip

import vlib.blip.machine

pub fn value_to_datum(mut m machine.Machine, v Value) !NodeId {
	match v.tag {
		.nil {
			return m.node_of('nil', [])
		}
		else {
			return error('value_to_datum: stub — only nil implemented (see docs/macros-design.md)')
		}
	}
}
