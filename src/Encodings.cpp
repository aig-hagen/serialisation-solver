#include "Encodings.h"

namespace Encodings {
	void admissible(const AF & af, SAT_Solver & solver) {
		for (uint32_t i = 0; i < af.args; i++) {
			solver.add_clause_2(-af.accepted_var[i], -af.rejected_var[i]);
			if (af.unattacked[i]) { // TODO grounded extension
				solver.add_clause_1(af.accepted_var[i]);
				continue;
			} // TODO attacked by grounded/unattacked
			std::vector<int32_t> out_clause(af.attackers[i].size()+1);
			for (uint32_t j = 0; j < af.attackers[i].size(); j++) {
				solver.add_clause_2(-af.accepted_var[i], af.rejected_var[af.attackers[i][j]]);
				out_clause[j] = af.accepted_var[af.attackers[i][j]];
			}
			out_clause[out_clause.size()-1] = -af.rejected_var[i];
			solver.add_clause(out_clause);
		}
	}
}