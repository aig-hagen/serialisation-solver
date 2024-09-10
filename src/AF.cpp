#include "AF.h"


AF::AF() : args(0) {}

void AF::set_arguments(uint32_t n_args) {
	args = n_args;
	attackers.resize(args);
	attacked.resize(args);
}

void AF::add_attack(uint32_t source, uint32_t target) {
	attackers[target].push_back(source);
	attacked[source].push_back(target);
}

int32_t AF::accepted_var(uint32_t arg) {
	return arg+1;
}

int32_t AF::rejected_var(uint32_t arg) {
	return (args + arg + 1);
}