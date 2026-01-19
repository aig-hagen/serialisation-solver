#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Encodings.h"

#include <stack>

namespace Algorithms {
    // for GF
    std::vector<std::vector<uint32_t>> generate_false_sequence(AF & af, const IterableBitSet & active_arguments, semantics semantics);


    // for EE
    void enumerate_extensions_admissible(AF & af, const IterableBitSet & active_arguments);
    // for ES
    /** Enumerates all initial sets in the given AF.
     * @param af                The argumentation framework.
     * @param active_arguments  The active arguments in the current reduct.
     * @return                  A vector of all initial sets found.
     */
    std::vector<std::vector<uint32_t>> enumerate_initial(AF & af, const IterableBitSet & active_arguments);
    /** Enumerates all admissible/preferred serialisation sequences in the given AF.
     * 
     * @param af                The argumentation framework.
     * @param active_arguments  The active arguments in the current reduct.
     * @param preferred         Whether to enumerate only the preferred sequences.
     */
    void enumerate_sequences_admissible(AF & af, const IterableBitSet & active_arguments, bool preferred);
    /** Enumerates all strongly admissible serialisation sequences in the given AF.
     * 
     * @param af                The argumentation framework.
     * @param active_arguments  The active arguments in the current reduct.
     * @param grounded          Whether to enumerate only the grounded serialisation sequences.
     */
    void enumerate_sequences_strong_admissible(AF & af, const IterableBitSet & active_arguments, bool grounded);
    /** Enumerates all complete serialisation sequences in the given AF.
     * 
     * @param af                The argumentation framework.
     * @param active_arguments  The active arguments in the current reduct.
     */
    void enumerate_sequences_complete(AF & af, const IterableBitSet & active_arguments);
    /** Enumerates all stable serialisation sequences in the given AF.
     * 
     * @param af                The argumentation framework.
     * @param active_arguments  The active arguments in the current reduct.
     */
    void enumerate_sequences_stable(AF & af, const IterableBitSet & active_arguments);
    /** Enumerates all unchallenged serialisation sequences in the given AF.
     * 
     * @param af                The argumentation framework.
     * @param active_arguments  The active arguments in the current reduct.
     * @param maximal           Whether to enumerate only the maximal serialisation sequences.
     */
    void enumerate_sequences_unchallenged(AF & af, const IterableBitSet & active_arguments, bool maximal);

    // for AS
    void sequences_argument_admissible(AF & af, const IterableBitSet & active_arguments, uint32_t argument);

    // for SE
    std::vector<std::vector<uint32_t>> enumerate_initial_subsets(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments);
    std::vector<std::vector<std::vector<uint32_t>>> enumerate_sequences_admissible_for_set(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments);

    // for XE
    void explain_extension(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments, const std::vector<std::vector<uint32_t>> sequence, semantics semantics);

    // for VE
    /** Checks if a set of arguments is conflict-free in the given AF.
     * A set is conflict-free if no two arguments in the set attack each other.
     * 
     * @param af                The argumentation framework.
     * @param active_arguments  The active arguments in the current reduct.
     * @param arguments         The set of arguments to check.
     * @return                  'true' if the set is conflict-free, 'false' otherwise.
     */
    bool is_conflict_free(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments);
    /** Checks if a set of arguments is defended in the given AF.
     * An argument is defended by a set if all its attackers are attacked by at least one argument in the set.
     * 
     * @param af                The argumentation framework.
     * @param active_arguments  The active arguments in the current reduct.
     * @param arguments         The set of arguments.
     * @param argument          The argument to check.
     * @return                  'true' if the argument is defended by the set, 'false' otherwise.
     */
    bool is_defended(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments, uint32_t argument);
    /** Checks if a set of arguments is admissible in the given AF.
     * A set is admissible if it is conflict-free and defends all its arguments.
     * 
     * @param af                The argumentation framework.
     * @param active_arguments  The active arguments in the current reduct.
     * @param arguments         The set of arguments to check.
     * @return                  'true' if the set is admissible, 'false' otherwise.
     */
    bool is_admissible(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments);
    /** Checks if a set of arguments is complete in the given AF.
     * A set is complete if it is admissible and contains all arguments it defends.
     * 
     * @param af                The argumentation framework.
     * @param active_arguments  The active arguments in the current reduct.
     * @param arguments         The set of arguments to check.
     * @return                  'true' if the set is complete, 'false' otherwise.
     */
    bool is_complete(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments);
    /** Checks if a set of arguments is stable in the given AF.
     * A set is stable if it is conflict-free and attacks all arguments not in the set.
     * 
     * @param af                The argumentation framework.
     * @param active_arguments  The active arguments in the current reduct.
     * @param arguments         The set of arguments to check.
     * @return                  'true' if the set is stable, 'false' otherwise.
     */
    bool is_stable(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments);
    bool is_grounded(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments);
    /** Checks if a set of arguments is preferred in the given AF.
     * A set is preferred if there is no admissible superset the AF.
     * Alternatively, a set is preferred if the reduct with respect to the set has no initial sets.
     * 
     * @param af                The argumentation framework.
     * @param active_arguments  The active arguments in the current reduct.
     * @param arguments         The set of arguments to check.
     * @return                  'true' if the set is preferred, 'false' otherwise.
     */
    bool is_preferred(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments);
};

namespace std {
    inline IterableBitSet get_reduct(const AF & af, const IterableBitSet & active_arguments, const std::vector<uint32_t> & arguments) {
        std::vector<uint32_t> reduct_array;
        std::vector<uint8_t> reduct_bitset;
        reduct_bitset.resize(af.args);
        reduct_array.reserve(active_arguments._array.size());

        for (const uint32_t arg1 : arguments) {
            reduct_bitset[arg1] = true;
            for (const uint32_t arg2 : af.attacked[arg1]) {
                reduct_bitset[arg2] = true;
            }
        }

        for (const uint32_t arg : active_arguments._array) {
            if (!reduct_bitset[arg]) {
                reduct_bitset[arg] = true;
                reduct_array.push_back(arg);
            } else {
                reduct_bitset[arg] = false;
            }
        }

        return IterableBitSet(reduct_array, reduct_bitset);
    }

    inline IterableBitSet get_cone(const AF & af, const IterableBitSet & active_arguments, uint32_t argument) {
        if (!active_arguments._bitset[argument]) {
            std::cerr << "Argument is not part of the framework!\n";
			exit(1);
        }
        std::vector<uint8_t> cone_bitset(af.args, false);
        std::vector<uint32_t> cone_array;
        cone_array.reserve(active_arguments._array.size());

		std::stack<uint32_t> stack;
		stack.push(argument);
		uint32_t arg;

		while (!stack.empty()) {
			arg = stack.top();
			stack.pop();
			if(!cone_bitset[arg]) {
				cone_bitset[arg] = true;
                cone_array.push_back(arg);
			}
			for (uint32_t i = 0; i < af.attackers[arg].size(); i++) {
				if (!cone_bitset[af.attackers[arg][i]] && active_arguments._bitset[af.attackers[arg][i]]) {
					stack.push(af.attackers[arg][i]);
				}
			}
		}

		return IterableBitSet(cone_array, cone_bitset);
    }
}

#endif