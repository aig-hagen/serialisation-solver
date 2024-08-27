#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Encodings.h"
#include <iostream>			//std::cout

namespace Algorithms {
    std::vector<std::vector<uint32_t>> enumerate_initial(const AF & af, const IterableBitSet & active_arguments);
};
#endif