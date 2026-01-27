#include "Algorithms.h"		// Algorithms for all supported Problems
#include "Parse.h"			// Parsing of AFs
#include <getopt.h>			// Parsing commandline options

#include <iostream>			//std::cout
#include <sstream>
#include <fstream>

static int version_flag = 0;
static int usage_flag = 0;
static int formats_flag = 0;
static int problems_flag = 0;

/**
 * Tasks:
 * GF : Generate False sequence
 * XE :	eXplain Extension
 * XS : eXplain Sequence Step
 * ES :	Enumerate Serialisation Sequences
 * EE : Enumerate Extensions
 * AS : Sequences for Argument
 * SE : Sequences for Extension
 * MA : Minimal Sequences for Argument
 * VE : Verify Extension
 */
enum task { GF, XE, XS, ES, EE, AS, SE, MA, VE, UNKNOWN_TASK };


task string_to_task(std::string problem) {
	std::string tmp = problem.substr(0, problem.find("-"));
	if (tmp == "GF") return GF;
	if (tmp == "XE") return XE;
	if (tmp == "XS") return XS;
	if (tmp == "ES") return ES;
	if (tmp == "EE") return EE;
	if (tmp == "AS") return AS;
	if (tmp == "SE") return SE;
	if (tmp == "MA") return MA;
	if (tmp == "VE") return VE;
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
	std::cout << " <query>		query argument or extension\n";
	std::cout << "Options:\n";
	std::cout << "  --help      Displays this help message.\n";
	std::cout << "  --version   Prints version and author information.\n";
	std::cout << "  --formats   Prints available file formats.\n";
	std::cout << "  --problems  Prints available computational tasks.\n";
}

void print_version(std::string solver_name) {
	std::cout << solver_name << " (version 1.2)\n" << "Lars Bengel, University of Hagen <lars.bengel@fernuni-hagen.de>\n";
}

void print_formats() {
	std::cout << "[i23]\n";
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
		{"a", required_argument, 0, 'a'},
		{"s", required_argument, 0, 's'},
		{0, 0, 0, 0}
	};

	int option_index = 0;
	int opt = 0;
	std::string task, file, fileformat, query, seq;

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
			case 'a':
				query = optarg;
				break;
			case 's':
				seq = optarg;
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
	
	AF aaf = AF();
	IterableBitSet active_arguments = parse_af(&aaf, file);
	std::vector<std::vector<std::vector<uint32_t>>> sequences;
	std::vector<std::vector<uint32_t>> sequence;
	std::vector<std::vector<uint32_t>> extensions;
	IterableBitSet arguments;
	
	// parse AF to string
	std::stringstream buffer;
	std:: ifstream f(file);
	buffer << f.rdbuf();
	std::string afstring = buffer.str();

	switch (string_to_task(task)) {
		case GF:
			sequence = Algorithms::generate_false_sequence(aaf, active_arguments, string_to_sem(task));
			arguments = IterableBitSet({}, std::vector<uint8_t>(aaf.args, false));
			for (std::vector<uint32_t> set : sequence) {
				for (uint32_t arg : set) {
					if (arguments._bitset[arg]) continue;
					arguments._array.push_back(arg);
					arguments._bitset[arg] = true;
				}
			}
			std::print_extension(aaf, arguments._array);
			Algorithms::explain_extension(aaf, active_arguments, arguments, sequence, string_to_sem(task), afstring);
			break;
		case XE: {
			arguments = parse_extension(aaf.args, query);
			sequences = Algorithms::enumerate_sequences_admissible_for_set(aaf, active_arguments, arguments);
			for (std::vector<std::vector<uint32_t>> seq : sequences) {
				json j = Algorithms::explain_extension(aaf, active_arguments, arguments, seq, string_to_sem(task), afstring);
				std::cout << std::setw(4) << j << std::endl;
				break;
			}
			if (sequences.empty()) {
				std::cout << "NO\n";
			}
			break;
		}
		case XS: {
			arguments = parse_extension(aaf.args, query);
			sequence = parse_sequence(aaf.args, seq);
			json j = Algorithms::explain_sequence_step(aaf, active_arguments, arguments, sequence, string_to_sem(task), afstring);
			std::cout << std::setw(4) << j << std::endl;
			break;
		}
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
					std::cerr << argv[0] << ": Semantics not supported!\n";
					return 1;
			}
			break;
		case EE:
			switch (string_to_sem(task)) {
				case IT:
					extensions = Algorithms::enumerate_initial(aaf, active_arguments);
					for (const std::vector<uint32_t> & ext : extensions) {
						print_extension(aaf, ext);
						std::cout << ",";
					}
					break;
				case AD:
					Algorithms::enumerate_extensions_admissible(aaf, active_arguments);
					break;
				default:
					std::cerr << argv[0] << ": Semantics not supported!\n";
					return 1;
			}
			break;
		case AS: {
			uint32_t query_argument;
			if (query.empty()) {
				std::cerr << argv[0] << ": Query argument must be specified via -a flag\n";
				return 1;
			} else {
				query_argument = std::stoi(query) - 1;
			}
			switch (string_to_sem(task)) {
				case AD:
					Algorithms::sequences_argument_admissible(aaf, active_arguments, query_argument);
					break;
				default:
					std::cerr << argv[0] << ": Semantics not supported!\n";
			return 1;
			}
			break;
		}
		case VE: {
			arguments = parse_extension(aaf.args, query);
			bool result = false;
			switch (string_to_sem(task)) {
				case AD:
					result = Algorithms::is_admissible(aaf, active_arguments, arguments);
					break;
				case CO:
					result = Algorithms::is_complete(aaf, active_arguments, arguments);
					break;
				case ST:
					result = Algorithms::is_stable(aaf, active_arguments, arguments);
					break;
				case PR:
					result = Algorithms::is_preferred(aaf, active_arguments, arguments);
					break;
				default:
					std::cerr << argv[0] << ": Semantics not supported!\n";
					return 1;
			}
			if (result) {
				std::cout << "YES\n";
			} else {
				std::cout << "NO\n";
			}
			break;
		}
		default:
			std::cerr << argv[0] << ": Problem not supported!\n";
			return 1;
	}
	std::cout << "\n";

	return 0;
}