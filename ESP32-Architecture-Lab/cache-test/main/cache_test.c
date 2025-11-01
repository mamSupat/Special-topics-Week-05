#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esp_timer.h>
#include <esp_heap_caps.h>

#define ARRAY_SIZE 4096
#define ITERATIONS 100
#define TEST_RUNS 5

// Test arrays in different memory locations
static uint32_t sram_array[ARRAY_SIZE];
static uint32_t *psram_array = NULL;

// =============================
// Measure Sequential Access
// =============================
uint64_t measure_sequential_access(uint32_t *array, const char* memory_type) {
    uint64_t start_time = esp_timer_get_time();
    uint32_t sum = 0;

    for (int run = 0; run < TEST_RUNS; run++) {
        for (int iter = 0; iter < ITERATIONS; iter++) {
            for (int i = 0; i < ARRAY_SIZE; i++) {
                sum += array[i];
            }
        }
    }

    uint64_t end_time = esp_timer_get_time();
    uint64_t duration = end_time - start_time;

    printf("%s Sequential Access: %llu µs (sum=%lu)\n", memory_type, duration, (unsigned long)sum);
    fflush(stdout);
    return duration;
}

// =============================
// Measure Random Access
// =============================
uint64_t measure_random_access(uint32_t *array, const char* memory_type) {
    uint64_t start_time = esp_timer_get_time();
    uint32_t sum = 0;

    for (int run = 0; run < TEST_RUNS; run++) {
        for (int iter = 0; iter < ITERATIONS; iter++) {
            for (int i = 0; i < ARRAY_SIZE; i++) {
                int index = (i * 2654435761U) % ARRAY_SIZE;  // Break cache locality
                sum += array[index];
            }
        }
    }

    uint64_t end_time = esp_timer_get_time();
    uint64_t duration = end_time - start_time;

    printf("%s Random Access: %llu µs (sum=%lu)\n", memory_type, duration, (unsigned long)sum);
    fflush(stdout);
    return duration;
}

// =============================
// Measure Stride Access
// =============================
uint64_t measure_stride_access(uint32_t *array, int stride, const char* test_name) {
    uint64_t start_time = esp_timer_get_time();
    uint32_t sum = 0;

    for (int run = 0; run < TEST_RUNS; run++) {
        for (int iter = 0; iter < ITERATIONS; iter++) {
            for (int i = 0; i < ARRAY_SIZE; i += stride) {
                sum += array[i];
            }
        }
    }

    uint64_t end_time = esp_timer_get_time();
    uint64_t duration = end_time - start_time;

    printf("%s (stride %d): %llu µs (sum=%lu)\n", test_name, stride, duration, (unsigned long)sum);
    fflush(stdout);
    return duration;
}

// =============================
// Initialize Arrays
// =============================
void initialize_arrays() {
    printf("Initializing test arrays...\n");
    fflush(stdout);

    // Initialize SRAM array
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sram_array[i] = i * 7 + 13;
    }

    // Try allocate PSRAM array
    psram_array = heap_caps_malloc(ARRAY_SIZE * sizeof(uint32_t), MALLOC_CAP_SPIRAM);
    if (psram_array) {
        printf("PSRAM array allocated successfully\n");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            psram_array[i] = i * 7 + 13;
        }
    } else {
        printf("PSRAM not available, using internal memory\n");
        psram_array = heap_caps_malloc(ARRAY_SIZE * sizeof(uint32_t), MALLOC_CAP_INTERNAL);
        if (psram_array) {
            for (int i = 0; i < ARRAY_SIZE; i++) {
                psram_array[i] = i * 7 + 13;
            }
        }
    }
    fflush(stdout);
}

// =============================
// Main Application
// =============================
void app_main() {
    printf("\n>>> ESP32 Cache Performance Analysis <<<\n");
    printf("========================================\n");
    printf("Array size: %d elements (%d KB)\n", ARRAY_SIZE, (ARRAY_SIZE * 4) / 1024);
    printf("Iterations per test: %d\n", ITERATIONS);
    printf("Test runs: %d\n\n", TEST_RUNS);
    fflush(stdout);

    initialize_arrays();

    // Test 1: SRAM Sequential vs Random
    printf("\n=== Test 1: Sequential vs Random Access (Internal SRAM) ===\n");
    fflush(stdout);
    uint64_t sram_seq = measure_sequential_access(sram_array, "SRAM");
    uint64_t sram_rand = measure_random_access(sram_array, "SRAM");
    printf("SRAM Random/Sequential Ratio: %.2fx\n", (double)sram_rand / sram_seq);
    fflush(stdout);

    // Test 2: External Memory (if available)
    if (psram_array) {
        printf("\n=== Test 2: External Memory Access ===\n");
        fflush(stdout);
        uint64_t psram_seq = measure_sequential_access(psram_array, "PSRAM");
        uint64_t psram_rand = measure_random_access(psram_array, "PSRAM");
        printf("PSRAM Random/Sequential Ratio: %.2fx\n", (double)psram_rand / psram_seq);
        printf("External/Internal Speed Ratio: %.2fx\n", (double)psram_seq / sram_seq);
        fflush(stdout);
    }

    // Test 3: Stride Access Patterns
    printf("\n=== Test 3: Stride Access Patterns ===\n");
    fflush(stdout);
    uint64_t stride1 = measure_stride_access(sram_array, 1, "Stride 1");
    uint64_t stride2 = measure_stride_access(sram_array, 2, "Stride 2");
    uint64_t stride4 = measure_stride_access(sram_array, 4, "Stride 4");
    uint64_t stride8 = measure_stride_access(sram_array, 8, "Stride 8");
    uint64_t stride16 = measure_stride_access(sram_array, 16, "Stride 16");

    printf("\nStride Ratios:\n");
    printf("Stride 2/1: %.2fx\n", (double)stride2 / stride1);
    printf("Stride 4/1: %.2fx\n", (double)stride4 / stride1);
    printf("Stride 8/1: %.2fx\n", (double)stride8 / stride1);
    printf("Stride 16/1: %.2fx\n", (double)stride16 / stride1);
    fflush(stdout);

    if (psram_array) {
        free(psram_array);
    }

    printf("\n✅ Cache performance analysis complete!\n");
    fflush(stdout);
}
