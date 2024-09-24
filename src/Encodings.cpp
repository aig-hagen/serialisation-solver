#include "Encodings.h"


namespace Encodings {
	void admissible_nonempty(AF & af, const IterableBitSet & active_arguments, SAT_Solver & solver) {
		std::vector<int32_t> non_empty_clause(active_arguments._array.size());
		for (size_t i = 0; i < active_arguments._array.size(); i++) {
			non_empty_clause[i] = af.accepted_var(active_arguments._array[i]);
			solver.add_clause_2(-af.accepted_var(active_arguments._array[i]), -af.rejected_var(active_arguments._array[i]));

			std::vector<int32_t> out_clause(af.attackers[active_arguments._array[i]].size()+1);
			for (size_t j = 0; j < af.attackers[active_arguments._array[i]].size(); j++) {
				if (active_arguments._bitset[af.attackers[active_arguments._array[i]][j]]) {
					solver.add_clause_2(-af.accepted_var(active_arguments._array[i]), af.rejected_var(af.attackers[active_arguments._array[i]][j]));
					out_clause[j] = af.accepted_var(af.attackers[active_arguments._array[i]][j]);
				}
			}
			out_clause[af.attackers[active_arguments._array[i]].size()] = -af.rejected_var(active_arguments._array[i]);
			solver.add_clause(out_clause);
		}
		solver.add_clause(non_empty_clause);
	}
}