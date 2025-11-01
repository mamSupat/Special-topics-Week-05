#include <stdio.h>
#include <string.h>
#include <esp_system.h>
#include <esp_heap_caps.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ===== Global Variables =====
static DRAM_ATTR char sram_buffer[1024];  // วางไว้ใน DRAM โดยตรง
static const char flash_string[] = "Hello from Flash Memory!";
static char *heap_ptr;

// ===== Function to Display Memory Info =====
void print_memory_info() {
    printf("\n=== ESP32 Memory Layout Analysis ===\n");

    int stack_var = 42;
    printf("Stack variable address: 0x%08lx\n", (unsigned long)&stack_var);
    printf("SRAM buffer address:    0x%08lx\n", (unsigned long)sram_buffer);
    printf("Flash string address:   0x%08lx\n", (unsigned long)flash_string);

    // Heap allocation
    heap_ptr = malloc(512);
    if (heap_ptr != NULL) {
        printf("Heap allocation:        0x%08lx\n", (unsigned long)heap_ptr);
    } else {
        printf("Heap allocation failed!\n");
    }

    printf("\n=== Heap Information ===\n");
    printf("Free heap size:         %lu bytes\n", (unsigned long)esp_get_free_heap_size());
    printf("Min free heap size:     %lu bytes\n", (unsigned long)esp_get_minimum_free_heap_size());
    printf("Largest free block:     %lu bytes\n", (unsigned long)heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT));

    printf("\n=== Memory Usage by Type ===\n");
    printf("Internal SRAM:          %lu bytes\n", (unsigned long)heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
    printf("SPI RAM (if available): %lu bytes\n", (unsigned long)heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    printf("DMA capable memory:     %lu bytes\n", (unsigned long)heap_caps_get_free_size(MALLOC_CAP_DMA));

    free(heap_ptr);
}

// ===== Main Application =====
void app_main() {
    // ✅ หน่วงเวลาให้ QEMU เปิด UART ทัน
    vTaskDelay(pdMS_TO_TICKS(1000));

    // ✅ ปิด buffering ของ stdout เพื่อให้ printf แสดงผลทันที
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("ESP32 Memory Architecture Analysis\n");
    printf("==================================\n");

    // ทดสอบการใช้งานหน่วยความจำ
    strcpy(sram_buffer, "SRAM Test Data");
    printf("Flash string: %s\n", flash_string);
    printf("SRAM buffer: %s\n", sram_buffer);

    // แสดงข้อมูลหน่วยความจำทั้งหมด
    print_memory_info();

    printf("\nMemory analysis complete!\n");
}
