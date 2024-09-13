#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Constants
#define SIZE 5 // 5x5 matrix for SHA-3
#define LANE_BITS 64 // Each lane is 64 bits

// Function prototypes
void print_state(uint64_t state[SIZE][SIZE]);
bool all_nonzero(uint64_t state[SIZE][SIZE]);

int main() {
    uint64_t state[SIZE][SIZE] = {0}; // Initial state: all zeros
    uint64_t message_block[SIZE][SIZE] = {0}; // Example message block with non-zero bits

    // Initialize the message block with some nonzero values
    // Here we just set arbitrary non-zero values for the sake of simulation
    message_block[0][0] = 0x1;
    message_block[1][1] = 0x2;
    message_block[2][2] = 0x4;
    message_block[3][3] = 0x8;
    message_block[4][4] = 0x10;

    int iteration = 0;
    while (!all_nonzero(state)) {
        // Simulate the process by XORing the message block with the state
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                state[i][j] ^= message_block[i][j];
            }
        }

        // Print the state and check if all lanes are non-zero
        printf("Iteration %d:\n", ++iteration);
        print_state(state);
    }

    printf("All lanes have at least one non-zero bit after %d iterations.\n", iteration);
    return 0;
}

// Print the state matrix
void print_state(uint64_t state[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%016llx ", state[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Check if all lanes are non-zero
bool all_nonzero(uint64_t state[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (state[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

