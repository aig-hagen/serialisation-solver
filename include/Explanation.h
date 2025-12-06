#ifndef EXPLANATIONS_H
#define EXPLANATIONS_H

#include "AF.h"

namespace Explanations {
    void Explanations::get_explanation_for_set(const AF & af, const std::vector<uint32_t> arguments);
    void Explanations::explain_conflict(uint32_t a1, uint16_t a2);
}

#endif