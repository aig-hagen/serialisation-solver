#include "Algorithms.h"
#include "json.hpp"
#include <unordered_set>

using json = nlohmann::json;

namespace Algorithms {
    void explain_extension(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments, const std::vector<std::vector<uint32_t>> sequence, semantics semantics) {
        std::unordered_set<uint32_t> attackers_total = {};
        for (uint32_t a : arguments._array) {
            for (uint32_t b : af.attackers[a]) {
                if (active_arguments._bitset[b]) {
                    attackers_total.insert(b);
                }
            }
        }
        json steps = json::array();
        int i = 1;

        IterableBitSet reduct = std::get_reduct(af, active_arguments, {});
        IterableBitSet prior = IterableBitSet({}, std::vector<uint8_t>(af.args, false));

        for (std::vector<uint32_t> set : sequence) {
            json step;
            step["id"] = i++;
            step = explain_step(af, reduct, attackers_total, prior, set, semantics);
            steps.push_back(step);
            reduct = std::get_reduct(af,reduct,set);
            for (uint32_t arg : set) {
                if (prior._bitset[arg]) continue;
                prior._bitset[arg] = true;
                prior._array.push_back(arg);
            }
        }

        json j;
        j["extension"] = json::array();
        for (uint32_t arg : arguments._array) {
            j["extension"].push_back(arg+1);
        }
        j["semantics"] = semantics;
        j["steps"] = steps;
        j["conflictfree"] = Algorithms::is_conflict_free(af, active_arguments, arguments);
        j["admissible"] = Algorithms::is_admissible(af, active_arguments, arguments);
        j["complete"] = Algorithms::is_complete(af, active_arguments, arguments);
        j["preferred"] = Algorithms::is_preferred(af, active_arguments, arguments);
        j["stable"] = Algorithms::is_stable(af, active_arguments, arguments);

        std::cout << std::setw(4) << j << std::endl;
    }

    json explain_step(AF & af, const IterableBitSet & active_arguments, const std::unordered_set<uint32_t> & attackers_total, const IterableBitSet & prior, const std::vector<uint32_t> & set, semantics semantics) {
        json step;
        json defeats = json::array();
        std::unordered_set<uint32_t> attackers_set;
        for (uint32_t a : set) {
            for (uint32_t b : af.attackers[a]) {
                if (!active_arguments._bitset[b]) continue;
                attackers_set.insert(b);
            }
        }

        std::vector<uint8_t> arguments_bitset(af.args, false);
        for (uint32_t arg : set) {
            arguments_bitset[arg] = true;
        }
        IterableBitSet arguments = IterableBitSet(set, arguments_bitset);
        // for each argument in the set, list all defeats it contributes
        for (uint32_t a : arguments._array) {
            for (uint32_t arg: af.attacked[a]) {
                if (!active_arguments._bitset[arg]) continue;
                json defeat;
                defeat["source"] = a+1;
                defeat["target"] = arg+1;
                
                bool relevant = false;
                for (uint32_t attacker : attackers_total) {
                    if (arg==attacker) {    
                        defeat["necessary"] = attackers_set.count(attacker)>0;
                        relevant = true;
                        break;
                    }
                }
                if (relevant || semantics == ST) {
                    defeats.push_back(defeat);
                }
            }
        }
        json problems = json::array();
        // check if the arguments has already been added to the set
        for (uint32_t arg : arguments._array) {
            // check if arg is already in extension
            if (prior._bitset[arg]) {
                json problem;
                problem["type"] = "already_included";
                problem["argument"] = arg+1;
                problems.push_back(problem);
            }

            // check for conflcit with already included arguments
            for (uint32_t a : prior._array) {
                for (uint32_t b : af.attacked[a]) {
                    if (b == arg) {
                        json problem;
                        problem["type"] = "prior_conflict";
                        problem["source"] = a+1;
                        problem["target"] = b+1;
                        problems.push_back(problem);
                    }
                }
            }

            // check for internal conflicts
            for (uint32_t a : af.attacked[arg]) {
                if (!active_arguments._bitset[a]) continue;
                for (uint32_t b : arguments._array) {
                    if (a == arg) {
                        json problem;
                        problem["type"] = "internal_conflict";
                        problem["source"] = b+1;
                        problem["target"] = a+1;
                        problems.push_back(problem);
                    }
                }
            }

            // check for missing defense
            for (const uint32_t a2 : af.attackers[arg]) {
                if (!active_arguments._bitset[a2]) continue;
                bool is_defended = false;
                for (const uint32_t a3 : arguments._array) {
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
                    json problem;
                    problem["type"] = "missing_defense";
                    problem["attacker"] = a2+1;
                    problem["argument"] = arg+1;
                    problems.push_back(problem);
                }
            }
        }
        if (!problems.empty()) {
            step["valid"] = false;
            step["problems"] = problems;
        } else {
            step["valid"] = true;
        }
        step["defeats"] = defeats;

        step["unattacked"] = attackers_set.empty();
        step["arguments"] = json::array();
        for (uint32_t arg : set) {
            step["arguments"].push_back(arg+1);
        }
        return step;
    }
}