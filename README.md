![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/aig-hagen/serialisation-solver?include_prereleases)
[![License: GPL3.0](https://img.shields.io/badge/License-GPL3-blue.svg)](https://opensource.org/license/gpl-3-0)

# serialisation-solver v1.2 (09-2024)

Implements algorithms for computing serialisation sequences for argumentation frameworks.

Supports i23 format for abstract argumentation frameworks.


## Installation

### Building SAT Solver
This solver works with [CaDiCal](https://github.com/arminbiere/cadical) (version 1.9.5)
The relevant source files are included and the solver can easily be built as follows

```
  make cadical
```

## Compile and Build Solver
To compile the solver use
```
  make
```

The binary can be found under `build/bin/serial-solver`

## Command-line usage
The solver follows the standard ICCMA/probo2 interface.
```
./serial-solver -p <task> -f <file> -a <additional>
  
  <task>        computational problem
  <file>         input argumentation framework
  <additional>  an argument or an extension (optional)

Options:
  --help        Displays this help message.
  --version     Prints version and author information.
  --formats     Prints available file formats (only i23).
  --problems    Prints available computational problems.
```

Example usage:

To enumerate the preferred serialisation sequences, use
```
  ./serial-solver -p ES-PR -f <file in i23 format>
```

## Suported Problems
- ### ES (Enumerate Sequences)
  Enumerate all $\sigma$-serialisation sequences of the AF
- ### EE (Enumerate Extensions)
  Enumerate all $\sigma$-extensions of the AF
- ### VE (Verify Extension)
  Verifies whether the given set is a $\sigma$-extension (requires the flag -a)
- ### AS (Argument Sequences)
  Enumerate all serialisation sequences that accept the given argument in the final step (requires the flag -a)
- ### SE (Sequences for Extension)
  Enumerates all serialisation sequences for the given extension (requires the flag -a)
- ### XE (eXplain Extension)
  Computes an explanation for the given extension via some serialisation sequence. Output in JSON-format. (requires the flag -a)
- ### GF (Generate False sequence)
  generates an explanation for a randomly generated false serialisation sequence. Output in JSON-format.
