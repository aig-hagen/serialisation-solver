#include "Explanation.h"

#ifdef EXPLAIN

namespace Explanations {
    void get_explanation_for_set(const AF & af, const std::vector<uint32_t> arguments) {
        for (const uint32_t a1 : arguments) {
            for (const uint32_t a2 : af.attacked[a1]) {
                for (const uint32_t a3 : arguments) {
                    if (a2 == a3) {
                        Explanations::explain_conflict(a1, a2);
                    }
                }
            }
        }

        for (const uint32_t a1 : arguments) {
            for (const uint32_t a2 : af.attackers[a1]) {
                bool is_defended = false;
                for (const uint32_t a3 : arguments) {
                    for (const uint32_t a4 : af.attacked[a3]) {
                        if (a4 == a2) {
                            is_defended = true;
                            break;
                        }
                    }
                    if (is_defended) {
                        break;
                    }
                }
                if (!is_defended) {
                    Explanations::explain_undefended(a1, a2);
                }
            }
        }
                    

    }

    void explain_conflict(uint32_t a1, uint32_t a2) {
        std::cout << "The argument" << a1 << "attacks the argument" << a2 << ". ";
    }

    void explain_undefended(uint32_t a1, uint32_t a2) {
        std::cout << "The argument" << a1 << "is attacked by the argument" << a2 << ", but it is not defended. ";
    }
}

#endif