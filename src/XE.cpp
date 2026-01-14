#include "Algorithms.h"
#include "json.hpp"
#include <unordered_set>

using json = nlohmann::json;

namespace Algorithms {
    void explain_extension(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments, const std::vector<std::vector<uint32_t>> sequence) {
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
        for (std::vector<uint32_t> set : sequence) {
            json step;
            step["id"] = i++;
            json defeats = json::array();
            std::unordered_set<uint32_t> attackers_set;
            for (uint32_t a : set) {
                for (uint32_t b : af.attackers[a]) {
                    if (!reduct._bitset[b]) continue;
                    attackers_set.insert(b);
                }
            }
            
            for (uint32_t attacker : attackers_total) {
                for(uint32_t arg : af.attackers[attacker]) {
                    if (!reduct._bitset[arg]) {continue;}
                    for (uint32_t a : set) {
                        if (arg==a) {
                            json defeat;
                            defeat["source"] = arg+1;
                            defeat["target"] = attacker+1;
                            defeat["necessary"] = attackers_set.count(attacker)>0;
                            defeats.push_back(defeat);
                        }
                    }
                }
            }
            step["unattacked"] = attackers_set.empty();
            step["defeats"] = defeats;
            step["arguments"] = json::array();
            for (uint32_t arg : set) {
                step["arguments"].push_back(arg+1);
            }

            steps.push_back(step);
            reduct = std::get_reduct(af,reduct,set);
        }

        json j;
        j["extension"] = json::array();
        for (uint32_t arg : arguments._array) {
            j["extension"].push_back(arg+1);
        }
        j["steps"] = steps;

        std::cout << std::setw(4) << j << std::endl;
    }


}