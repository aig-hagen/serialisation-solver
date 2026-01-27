#include "AF.h"
#include "IterableBitSet.h"

/** Parses an i23 file into an argumentation framework.
 * 
 * @param af pointer to the argumentation framework to populate.
 * @param file path to the i23 file.
 * @return the active arguments in the AF.
*/
IterableBitSet parse_af(AF* af, std::string file);

/** Parses an extension from a given string
 * The extension must be given in the form: "[i,j,k,...]", where each entry is an integer within the bounds (1,n_args)
 * 
 * @param n_args number of arguments in the AF.
 * @param extension extension in string format.
 * @return the parsed extension.
*/
IterableBitSet parse_extension(uint32_t n_args, std::string extension);

/** Parses a serialisation sequence from a given string
 * The sequence must be given in the form: "[[i1,i2],[j1,j2,...],...]", where each entry is an integer within the bounds (1,n_args)
 * 
 * @param n_args number of arguments in the AF.
 * @param sequence sequence in string format.
 * @return the parsed sequence.
*/
std::vector<std::vector<uint32_t>> parse_sequence(uint32_t n_args, std::string sequence);