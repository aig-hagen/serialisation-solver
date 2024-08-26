#ifndef ENCODINGS_H
#define ENCODINGS_H

#include "AF.h"

#include "CadicalSatSolver.h"
typedef CadicalSatSolver SAT_Solver;


namespace Encodings {
    void admissible(const AF & af, SAT_Solver & solver, bool isAttackedSet);
    void attacks(const AF & af, SAT_Solver & solver);
    void complete(const AF & af, SAT_Solver & solver);
    void stable(const AF & af, SAT_Solver & solver);
    void add_rejected_clauses(const AF & af, SAT_Solver & solver);
    void add_conflict_free(const AF & af, SAT_Solver & solver);
    void add_admissible(const AF & af, SAT_Solver & solver);
    void add_complete(const AF & af, SAT_Solver & solver);
}
#endif