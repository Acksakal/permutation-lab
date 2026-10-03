#include <stdio.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int is_magic_square(int nums[3][3]) {
    int sum = nums[0][0] + nums[0][1] + nums[0][2];
    int i, j;

    for (i = 1; i < 3; i++) {
        if (nums[i][0] + nums[i][1] + nums[i][2] != sum) return 0;
    }

    for (j = 0; j < 3; j++) {
        if (nums[0][j] + nums[1][j] + nums[2][j] != sum) return 0;
    }

    if (nums[0][0] + nums[1][1] + nums[2][2] != sum) return 0;
    if (nums[0][2] + nums[1][1] + nums[2][0] != sum) return 0;

    return 1;
}

void permute(int *arr, int start, int end, int *found) {
    int i, j;

    if (start == end) {
        int nums[3][3];
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                nums[i][j] = arr[i * 3 + j];
            }
        }

        if (is_magic_square(nums)) {
            *found = 1;
            printf("Winning combination found (Sum = %d):\n", nums[0][0] + nums[0][1] + nums[0][2]);
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                    printf("%d ", nums[i][j]);
                }
                printf("\n");
            }
        }
        return;
    }

    for (i = start; i <= end; i++) {
        /* if (*found) return; */
        swap(&arr[start], &arr[i]);
        permute(arr, start + 1, end, found);
        swap(&arr[start], &arr[i]);
    }
}

int main(void) {
    int digits[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int found = 0;

    permute(digits, 0, 8, &found);

    if (!found) {
        printf("No winning combination exists.\n");
    }

    return 0;
}