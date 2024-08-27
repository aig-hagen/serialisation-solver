/*!
 * The following is adapted from the mu-toksia argumentation-solver
 * and is subject to the following licence.
 *
 * 
 * Copyright (c) <2020> <Andreas Niskanen, University of Helsinki>
 * 
 * 
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * 
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Algorithms.h"		// Algorithms for all supported Problems
#include <getopt.h>			// parsing commandline options

#include <iostream>			//std::cout
#include <fstream>			//ifstream
#include <sstream>			//istringstream
#include <algorithm>
#include <stack>

static int version_flag = 0;
static int usage_flag = 0;
static int formats_flag = 0;
static int problems_flag = 0;

task string_to_task(std::string problem) {
	std::string tmp = problem.substr(0, problem.find("-"));
	if (tmp == "ES") return ES;
	if (tmp == "EE") return EE;
	return UNKNOWN_TASK;
}

semantics string_to_sem(std::string problem) {
	problem.erase(0, problem.find("-") + 1);
	std::string tmp = problem.substr(0, problem.find("-"));
	if (tmp == "IT") return IT;
	if (tmp == "CO") return CO;
	if (tmp == "ST") return ST;
	if (tmp == "PR") return PR;
	if (tmp == "AD") return AD;
	if (tmp == "GR") return GR;
	if (tmp == "SA") return SA;
	if (tmp == "UC") return UC;

	return UNKNOWN_SEM;
}

void print_usage(std::string solver_name) {
	std::cout << "Usage: " << solver_name << " -p <task> -f <file> -fo <format> \n\n";
	std::cout << "  <task>      computational problem; for a list of available problems use option --problems\n";
	std::cout << "  <file>      input argumentation framework\n";
	std::cout << "  <format>    file format for input AF; for a list of available formats use option --formats\n";
	std::cout << "Options:\n";
	std::cout << "  --help      Displays this help message.\n";
	std::cout << "  --version   Prints version and author information.\n";
	std::cout << "  --formats   Prints available file formats.\n";
	std::cout << "  --problems  Prints available computational tasks.\n";
}

void print_version(std::string solver_name) {
	std::cout << solver_name << " (version 1.0)\n" << "TODO\n";
}

void print_formats() {
	std::cout << "[tgf,i23]\n";
}

void print_problems() {
	std::cout << "[ES-AD,ES-PR,ES-SA,ES-GR,ES-CO,ES-ST,EE-IT]\n";
}

int main(int argc, char ** argv) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	if (argc == 1) {
		print_version(argv[0]);
		return 0;
	}

	const struct option longopts[] = {
		{"help", no_argument, &usage_flag, 1},
		{"version", no_argument, &version_flag, 1},
		{"formats", no_argument, &formats_flag, 1},
		{"problems", no_argument, &problems_flag, 1},
		{"p", required_argument, 0, 'p'},
		{"f", required_argument, 0, 'f'},
		{"fo", required_argument, 0, 'o'},
		{"s", required_argument, 0, 's'},
		{0, 0, 0, 0}
	};

	int option_index = 0;
	int opt = 0;
	std::string task, file, fileformat, query, sat_path;

	while ((opt = getopt_long_only(argc, argv, "", longopts, &option_index)) != -1) {
		switch (opt) {
			case 0:
				break;
			case 'p':
				task = optarg;
				break;
			case 'f':
				file = optarg;
				break;
			case 'o':
				fileformat = optarg;
				break;
			case 's':
				sat_path = optarg;
				break;
			default:
				return 1;
		}
	}

	if (version_flag) {
		print_version(argv[0]);
		return 0;
	}

	if (usage_flag) {
		print_usage(argv[0]);
		return 0;
	}

	if (formats_flag) {
		print_formats();
		return 0;
	}

	if (problems_flag) {
		print_problems();
		return 0;
	}

	if (task.empty()) {
		std::cerr << argv[0] << ": Task must be specified via -p flag\n";
		return 1;
	}

	if (file.empty()) {
		std::cerr << argv[0] << ": Input file must be specified via -f flag\n";
		return 1;
	}

	if (fileformat.empty()) {
		//std::cerr << argv[0] << ": File format must be specified via -fo flag\n";
		fileformat = "i23";
		//return 1;
	}

	std::ifstream input;
	input.open(file);

	if (!input.good()) {
		std::cerr << argv[0] << ": Cannot open input file\n";
		return 1;
	}

	std::vector<uint32_t> active_array;
	std::vector<u_int8_t> active_bitset;
	AF aaf = AF();
	aaf.sem = string_to_sem(task);
	int32_t n_args = 0;
	std::string line, arg, source, target;
	if (fileformat == "i23") {
		while (!input.eof()) {
			getline(input, line);
			if (line.length() == 0 || line[0] == '#') continue;
			std::istringstream iss(line);
			if (line[0] == 'p') {
				std::string p, af;
				iss >> p >> af >> n_args;
				active_bitset.resize(n_args, true);
				active_array.reserve(n_args);
				for (int i = 1; i <= n_args; i++) {
					aaf.add_argument(std::to_string(i));
					active_array.push_back(aaf.arg_to_int[std::to_string(i)]);
				}
				aaf.initialize_attackers();
			} else {
				iss >> source >> target;
				aaf.add_attack(make_pair(source, target));
			}
		}
	} else if (fileformat == "tgf") {
		while (input >> arg) {
			if (arg == "#") {
				aaf.initialize_attackers();
				break;
			}
			aaf.add_argument(arg);
			active_array.push_back(aaf.arg_to_int[arg]);
		}
		active_bitset.resize(aaf.args, true);
		while (input >> source >> target) {
			aaf.add_attack(make_pair(source, target));
		}
	} else {
		std::cerr << argv[0] << ": Unsupported file format\n";
		return 1;
	}
	input.close();

	aaf.initialize_vars();

	IterableBitSet active_arguments = IterableBitSet(active_array, active_bitset);
	std::vector<std::vector<uint32_t>> result;
	switch (string_to_task(task)) {
		case ES: // TODO what if st(F) = \emptyset
			switch (string_to_sem(task)) {
				case AD:
					Algorithms::enumerate_sequences_admissible(aaf, active_arguments, false);
					break;
				case PR:
					Algorithms::enumerate_sequences_admissible(aaf, active_arguments, true);
					break;
				case SA:
					Algorithms::enumerate_sequences_strong_admissible(aaf, active_arguments, false);
					break;
				case GR:
					Algorithms::enumerate_sequences_strong_admissible(aaf, active_arguments, true);
					break;
				case CO:
					Algorithms::enumerate_sequences_complete(aaf, active_arguments);
					break;
				case ST:
					Algorithms::enumerate_sequences_stable(aaf, active_arguments);
					break;
				case UC:
					Algorithms::enumerate_sequences_unchallenged(aaf, active_arguments, true);
					break;
				default:
					break;
			}
			break;
		case EE:
			switch (string_to_sem(task)) {
			case IT:
				result = Algorithms::enumerate_initial(aaf, active_arguments);
				for (const std::vector<uint32_t> & ext : result) {
					print_extension(aaf, ext);
					std::cout << ",";
				}
				break;
			
			default:
				break;
			}
			break;
		default:
			std::cerr << argv[0] << ": Problem not supported!\n";
			return 1;
	}
	std::cout << "\n";

	return 0;
}