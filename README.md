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

## Command-line usage TODO update

```
./solver.sh <algorithm> -p <task> -f <file> -fo <format>
  
  <algorithm> the algorithm for computation, one of: [iaq,eee,see,seem,fudge]
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
```
  ./solver.sh iaq -p EC-PR -fo tgf -f <file in TGF format>
```

Alternatively, you can also directly call the binary for each algorithm, e.g.
```
  ./build/bin/<algorithm>/solver -p EC-PR -fo tgf -f <file in TGF format>
```

