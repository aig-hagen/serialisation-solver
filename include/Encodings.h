#ifndef ENCODINGS_H
#define ENCODINGS_H

#include "AF.h"
#include "IterableBitSet.h"

#include "CadicalSatSolver.h"
typedef CadicalSatSolver SAT_Solver;


namespace Encodings {
    void admissible_nonempty(AF & af, const IterableBitSet & active_arguments, SAT_Solver & solver);
}
#endif