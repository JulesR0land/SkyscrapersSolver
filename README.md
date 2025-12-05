# Skyscrapers Solver

A fast and efficient solver for Skyscrapers logic puzzles, written in C. Uses constraint propagation and backtracking to find solutions.

## What is Skyscrapers?

Skyscrapers is a logic puzzle where you fill an NxN grid with numbers 1 to N (each appearing once per row and column) based on visibility clues around the edges. Each clue indicates how many "buildings" you can see from that direction, where taller buildings block the view of shorter ones behind them.

**Example (4x4 puzzle):**
```
    2 1 3 2            2 1 3 2
  ┌─────────┐        ┌─────────┐
2 │ ? ? ? ? │ 2    2 │ 3 4 1 2 │ 2
3 │ ? ? ? ? │ 1    3 │ 2 1 3 4 │ 1
1 │ ? ? ? ? │ 3    1 │ 4 3 2 1 │ 3
3 │ ? ? ? ? │ 2    3 │ 1 2 4 3 │ 2
  └─────────┘        └─────────┘
    2 3 1 2            2 3 1 2
   (Puzzle)           (Solution)
```

From the top-left corner (clue = 2), you see buildings of height 3, 2, 4, 1.
The 3 hide the 2 and the 4 hide whatever is behind it since it's a size 4 puzzle.
So only 2 buildings are visible for the clue, it's correct.

## Features

- Solves Skyscrapers puzzles of any size
- Efficient constraint propagation algorithm
- Backtracking with intelligent guessing
- Fast performance even on difficult puzzles
- Clean, modular C codebase

## Building

### Requirements
- GCC compiler
- Make

### Compilation
```bash
make
```

This produces an executable named `skyscrapers`.

## Usage

```bash
./skyscrapers <size> "<clues>"
```

### Parameters
- `<size>`: The dimension of the grid (e.g., 5 for a 5x5 puzzle)
- `<clues>`: Comma-separated list of clues, starting from top-left and going clockwise

### Clue Order
Clues are provided in this order:
1. **Top edge**: Left to right (indices 0 to N-1)
2. **Right edge**: Top to bottom (indices N to 2N-1)
3. **Bottom edge**: Right to left (indices 2N to 3N-1)
4. **Left edge**: Bottom to top (indices 3N to 4N-1)

Use `0` for empty clues (no constraint).

### Example
```bash
# Solve a 5x5 puzzle
./skyscrapers 5 "0,0,0,3,3,2,0,0,0,2,0,0,0,4,0,0,5,0,0,2"

# Output:
    0 0 0 3 3 
  ┌───────────┐
0 │ 4 3 5 1 2 │ 2
0 │ 3 5 4 2 1 │ 0
0 │ 2 4 1 5 3 │ 0
3 │ 1 2 3 4 5 │ 0
3 │ 5 1 2 3 4 │ 2
  └───────────┘
    0 4 0 0 0 
```

Test this puzzle at: https://www.brainbashers.com/showskyscrapers.asp?date=1019&size=5&diff=3

## How It Works

The solver uses a two-phase approach:

1. **Constraint Propagation**
   - Eliminates impossible values based on clues
   - Identifies cells with only one possible value
   - Generates all valid line combinations for aggressive elimination
   - Propagates changes across rows and columns

2. **Backtracking Search**
   - When constraint propagation is exhausted, makes educated guesses
   - Recursively solves from the guessed state
   - Backtracks if contradiction is found
   - Continues until solution is validated

## Project Structure

```
SkyscrapersSolver/
├── include/
│   ├── skyscrapers_structs.h       # Core data structures
│   ├── skyscrapers_cells.h         # Cell operations
│   ├── skyscrapers_lines.h         # Row/column operations
│   ├── skyscrapers_eliminations.h  # Constraint propagation
│   ├── skyscrapers_solver.h        # Main solving algorithm
│   ├── skyscrapers_data.h          # Memory management
│   └── skyscrapers_utils.h         # Utility functions
├── src/
│   ├── skyscrapers_cells.c
│   ├── skyscrapers_lines.c
│   ├── skyscrapers_eliminations.c
│   ├── skyscrapers_solver.c
│   ├── skyscrapers_data.c
│   └── skyscrapers_utils.c
├── main.c                          # Program entry point
├── Makefile                        # Build configuration
└── README
```

## Algorithm Complexity

- **Time**: Worst case O(N! × N²) for backtracking, but constraint propagation dramatically reduces the search space
- **Space**: O(N³) for storing possibilities in each cell

## Contributing

Contributions are welcome! Areas for improvement:
- Additional constraint propagation techniques
- Performance optimizations
- Better heuristics for cell selection during backtracking
- Puzzle difficulty analysis

## License

This project is open source. Feel free to use and modify as needed.

## Author

Created by Jules

## References

- [BrainBashers Skyscrapers](https://www.brainbashers.com/skyscrapers.asp) - Play Skyscrapers puzzles online
