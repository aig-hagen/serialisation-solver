# serialisation-solver v1.0 (08-2024)

Implements algorithms for computing serialisation sequences for argumentation frameworks.

Supports TGF and i23 format for abstract argumentation frameworks.


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

```
./serial-solver -p <task> -f <file> -fo <format>
  
  <task>      computational problem
  <file>      input argumentation framework
  <format>    file format for input AF

Options:
  --help      Displays this help message.
  --version   Prints version and author information.
  --formats   Prints available file formats.
  --problems  Prints available computational problems.
```

Example usage:

To enumerate the preferred serialisation sequences, use
```
  ./serial-solver -p ES-PR -fo tgf -f <file in TGF format>
```

