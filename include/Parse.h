#include "AF.h"
#include "IterableBitSet.h"


IterableBitSet parse_i23(AF* af, std::string file);
/** Parses an extension from a given string
 * The extension must be given in the form: "[i,j,k,...]", where each entry is an integer within the bounds (1,n_args)
 * 
 * @param n_args number of arguments in the AF.
 * @param extension extension in string format.
 * @return the parsed extension.
*/
IterableBitSet parse_extension(uint32_t n_args, std::string extension);