# permutation-lab

Learning permutations by brute force, then seeing why you usually shouldn't. The repo solves the magic square problem two ways: a brute-force search over every permutation of 1–9, and a constructive generator that builds a magic square of any size N ≥ 3 directly. It also includes an interactive animation of the brute-force search.

## The problem

Place the numbers 1..N² in an N×N grid so that every row, every column and both diagonals add up to the same sum, the magic sum N(N²+1)/2 (15 for N = 3, 34 for N = 4, 111 for N = 6).

## What's in this repo

```
permutation-lab/
└── magic-squares/
    ├── bruteforce/
    │   ├── magic_square.c
    │   └── permutation.html
    └── construction/
        └── magic_square.c
```

| File | What it is |
| --- | --- |
| `magic-squares/bruteforce/magic_square.c` | Brute force for 3×3: a recursive permutation generator plus a magic square check. |
| `magic-squares/bruteforce/permutation.html` | A single-file visualizer (HTML, CSS, JS, no dependencies) that animates the brute-force program step by step. Open it in a browser. |
| `magic-squares/construction/magic_square.c` | Constructive generator for any N ≥ 3, using a different known method for each kind of N. |

## Build and run

From the repo root:

```sh
cd magic-squares/bruteforce
cc magic_square.c -std=c90
./a.out

cd ../construction
cc magic_square.c -std=c90
./a.out
```

The brute-force program prints 8 grids, each with sum 15. The construction program prints one 3×3, one 4×4 and one 6×6 square.

---

## Part 1: Brute force (`bruteforce/`)

The idea: generate **every** ordering of the nine digits and test each one. There are 9! = **362,880** of them.

`permute(arr, start, end)` builds a permutation by fixing one position at a time:

1. **Base case:** if `start == end`, every position is fixed. Check the grid.
2. **Otherwise:** for each `i` from `start` to `end`, swap `arr[start]` with `arr[i]`. This puts a different value into position `start`.
3. **Recurse:** call `permute` for `start + 1`. Everything before `start` is now fixed.
4. **Backtrack:** swap back, restoring `arr` so the next `i` starts from the same state.

Step 4 is the key idea. Because every swap is undone, each recursive call sees the same array it was given, and the recursion tree enumerates every ordering exactly once.

```
permute(0)  choose a value for position 0   (9 choices)
 └ permute(1)  choose for position 1         (8 choices)
    └ permute(2)  choose for position 2      (7 choices)
       └ ...
          └ permute(8)  only 1 choice left → check the grid
```

### Counting the work

| What | Count |
| --- | --- |
| Complete orderings checked (leaves) | 9! = 362,880 |
| `permute` calls (all tree nodes) | 986,410 |
| `swap` calls | 1,972,818 (two per loop iteration) |

The number of tree nodes at depth *k* is 9!/(9−*k*)!, and the leaves dominate. The work grows as O(n!), which is why brute force stops being practical around n = 11 or 12. A 4×4 square would need 16! ≈ 2.1 × 10¹³ orderings.

### Notes

- The program finds **8** solutions. They are all rotations and reflections of a single square, so there is really only one 3×3 magic square.
- The commented-out `if (*found) return;` in `permute` would stop at the first solution. Left as is, the search always runs through all 362,880 orderings.
- `is_magic_square` returns early on the first failing line, and most orderings fail on a row. The visualizer counts these.

### The visualizer

`bruteforce/permutation.html` runs the same algorithm as a step-by-step animation:

- the array, with fixed positions (before `start`) shaded and the two cells being swapped highlighted
- the `start` and `i` pointers, and `temp` during a swap
- the recursion call stack
- the sums being checked and why an ordering fails
- your code with the running line highlighted
- counters and every solution found

Controls: Play/Pause, Step, Next permutation, Skip to next solution, Reset, and a speed slider up to turbo. Tip: step slowly through the first few orderings and watch position 8 change first, then 7, then 6, as the recursion unwinds.

---

## Part 2: Building it directly (`construction/`)

Instead of searching, construct the square. The method depends on N, and `generate_magic_square(n)` picks the right one. The work is about O(N²), one step per cell, instead of O(N²!).

| N | Kind | Method | Function |
| --- | --- | --- | --- |
| 3, 5, 7, … | Odd | Siamese method | `solve_odd` |
| 4, 8, 12, … | Doubly even (N % 4 == 0) | Complement on 4×4 diagonals | `solve_doubly_even` |
| 6, 10, 14, … | Singly even (N % 4 == 2) | Strachey / LUX method | `solve_singly_even` |

N < 3 is rejected. No 2×2 magic square exists, and N = 1 is trivial.

### Odd N: Siamese method

Put 1 in the middle of the top row. For each next number, move one step up and one step right, wrapping around the edges. If that cell is already taken, move one step down from the current cell instead.

For N = 3 this produces:

```
8 1 6
3 5 7
4 9 2
```

### Doubly even N: complement on diagonals

Fill the grid with 1..N² in reading order. Treat the grid as 4×4 tiles. In each tile, keep the numbers on the two diagonals. Everywhere else replace the number `v` with its complement N² + 1 − `v`. Complement pairs sum to N² + 1, which balances every line.

### Singly even N: Strachey / LUX method

1. Let k = N/2 (always odd). Build a k×k magic square with the odd method.
2. Copy it into the four quadrants of the N×N grid, adding 0, k², 2k² and 3k² to the quadrants (top-left, bottom-right, top-right, bottom-left respectively).
3. Swap the leftmost (k−1)/2 columns between the top-left and bottom-left quadrants. In the middle row, shift that swap one column to the right.
4. For N ≥ 10, also swap the rightmost (k−3)/2 columns between the top-right and bottom-right quadrants. For N = 6 there are none to swap.

I checked the generator for every N from 3 to 30: all rows, columns and both diagonals add up to N(N²+1)/2.

---

## Brute force vs construction

| | Brute force | Construction |
| --- | --- | --- |
| Idea | Try everything, keep what passes | Follow a rule that guarantees the result |
| Work | O(N²!) orderings | O(N²) cell writes |
| Practical up to | N = 3 | Any N that fits in memory |
| Finds | All 8 squares for N = 3 | One square per N |
| Teaches | Permutations, recursion, backtracking | Using structure to avoid search |

Brute force finds every solution but cannot scale. Construction scales but finds only one solution, which makes the two parts a good pair to compare.

## Things to try next

- **Prune early:** in the brute-force search, reject partial grids as soon as a completed row can't match the target sum. Count how many orderings you skip.
- **Fix the sum:** the magic sum is always 15, so test rows against 15 instead of the first row.
- **Other generators:** compare with Heap's algorithm, lexicographic `next_permutation`, and the Steinhaus–Johnson–Trotter algorithm.
- **Verify the construction:** write `is_magic_square(grid, n)` for general N and assert it passes for every N the generator builds.
- **Other problems:** use the same swap-and-backtrack skeleton for the n-queens puzzle or for anagram generation.