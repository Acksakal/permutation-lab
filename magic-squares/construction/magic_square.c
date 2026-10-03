#include <stdio.h>
#include <stdlib.h>

/* --- Generator 1: Odd N (Siamese Method) --- */
void solve_odd(int **grid, int n) {
    int r = 0, c = n / 2;
    int num, i, j;

    for (num = 1; num <= n * n; num++) {
        grid[r][c] = num;
        i = (r - 1 + n) % n;
        j = (c + 1) % n;

        if (grid[i][j] != 0) {
            r = (r + 1) % n;
        } else {
            r = i;
            c = j;
        }
    }
}

/* --- Generator 2: Doubly Even N (N % 4 == 0) --- */
void solve_doubly_even(int **grid, int n) {
    int i, j, val = 1;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            /* Keep numbers on 4x4 block diagonals, invert others */
            if ((i % 4 == j % 4) || ((i % 4) + (j % 4) == 3)) {
                grid[i][j] = val;
            } else {
                grid[i][j] = (n * n + 1) - val;
            }
            val++;
        }
    }
}

/* --- Generator 3: Singly Even N (Strachey / LUX Method) --- */
void solve_singly_even(int **grid, int n) {
    int k = n / 2;
    int sub_grid_size = k * k;
    int **sub;
    int i, j, temp;
    int shift = (k - 1) / 2;

    /* Allocate sub-matrix for odd quadrant */
    sub = (int **)malloc(k * sizeof(int *));
    for (i = 0; i < k; i++) {
        sub[i] = (int *)calloc(k, sizeof(int));
    }

    /* 1. Build K x K odd magic square for quadrant A */
    solve_odd(sub, k);

    /* 2. Copy quadrant patterns to A, B, C, D with offsets */
    for (i = 0; i < k; i++) {
        for (j = 0; j < k; j++) {
            grid[i][j]         = sub[i][j];                     /* Top-Left (A) */
            grid[i + k][j + k] = sub[i][j] + sub_grid_size;     /* Bottom-Right (B) */
            grid[i][j + k]     = sub[i][j] + 2 * sub_grid_size; /* Top-Right (C) */
            grid[i + k][j]     = sub[i][j] + 3 * sub_grid_size; /* Bottom-Left (D) */
        }
    }

    /* 3. Swap elements between A and D */
    for (i = 0; i < k; i++) {
        for (j = 0; j < shift; j++) {
            if (i == shift) {
                /* Special middle row offset */
                temp = grid[i][j + 1];
                grid[i][j + 1] = grid[i + k][j + 1];
                grid[i + k][j + 1] = temp;
            } else {
                temp = grid[i][j];
                grid[i][j] = grid[i + k][j];
                grid[i + k][j] = temp;
            }
        }
    }

    /* 4. Swap rightmost elements between C and B for larger matrices */
    for (i = 0; i < k; i++) {
        for (j = n - (shift - 1); j < n; j++) {
            temp = grid[i][j];
            grid[i][j] = grid[i + k][j];
            grid[i + k][j] = temp;
        }
    }

    for (i = 0; i < k; i++) free(sub[i]);
    free(sub);
}

/* --- Universal Entry Point --- */
void generate_magic_square(int n) {
    int **grid;
    int i, j;

    if (n < 3) {
        printf("Magic square is impossible for N < 3.\n");
        return;
    }

    grid = (int **)malloc(n * sizeof(int *));
    for (i = 0; i < n; i++) {
        grid[i] = (int *)calloc(n, sizeof(int));
    }

    /* Route to appropriate method */
    if (n % 2 != 0) {
        solve_odd(grid, n);
    } else if (n % 4 == 0) {
        solve_doubly_even(grid, n);
    } else {
        solve_singly_even(grid, n);
    }

    /* Output result */
    printf("%dx%d Magic Square (Sum = %d):\n", n, n, n * (n * n + 1) / 2);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%5d ", grid[i][j]);
        }
        printf("\n");
    }

    /* Free memory */
    for (i = 0; i < n; i++) free(grid[i]);
    free(grid);
}

int main(void) {
    generate_magic_square(3); /* Odd */
    printf("\n");
    generate_magic_square(4); /* Doubly even */
    printf("\n");
    generate_magic_square(6); /* Singly even */

    return 0;
}