// value_to_datum — reverse of datum_to_value (machine/mod.v 2596).
// Status: pure-shapes implemented; complex shapes (closure/table/primitive)
// remain stub (see docs/macros-design.md — requires embedded machine).

module blip

import vlib.blip.machine

pub fn value_to_datum(mut m machine.Machine, v Value) !NodeId {
	match v.tag {
		.nil {
			return m.node_of('nil', [])
		}
		.boolean {
			return m.node_of(if v.i == 1 { 'true' } else { 'false' }, [])
		}
		.integer {
			return m.node_of('${v.i}', [])
		}
		.float {
			return m.node_of('${v.f}', [])
		}
		.string {
			s := v.as_string()
			return m.node_of('"${s}"', [])
		}
		.symbol {
			s := v.as_string()
			return m.node_of(s, [])
		}
		.keyword {
			s := v.as_string()
			return m.node_of(':${s}', [])
		}
		.pair {
			car_id := value_to_datum(mut m, v.as_pair().car)!
			cdr_id := value_to_datum(mut m, v.as_pair().cdr)!
			return m.node_of('', [car_id, cdr_id])
		}
		.vector {
			vec := v.as_vector()
			mut kids := []NodeId{}
			for elem in vec.data {
				kids << value_to_datum(mut m, elem)!
			}
			return m.node_of('', kids)
		}
		.emptylist {
			return m.node_of('()', [])
		}
		else {
			return error('value_to_datum: complex shape ${v.tag} not yet implemented')
		}
	}
}
