#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define OFFSET_MASK 0x000000ff
#define PAGE_MASK 0x0000ff00

#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define TLB_SIZE 16

#define NUMBER_OF_PAGES 256
#define NUMBER_OF_FRAMES 256
#define MEMORY_SIZE (FRAME_SIZE * NUMBER_OF_FRAMES)

#define BACKING_STORAGE_FILE "BACKING_STORE.bin"
#define ADDRESS_FILE "addresses.txt"

uint32_t victim_frm();
void pg_fault_handler(uint32_t pg_num);
int find_in_TLB(uint32_t pg_num, uint32_t *frm_num);
void update_TLB(uint32_t pg_num, uint32_t frm_num);
uint32_t get_add(uint32_t logical_add);
char read_memory(uint32_t physical_add);

typedef struct {
    uint8_t valid;
    uint32_t page, frame;
} Table;
Table TLB[TLB_SIZE];

int8_t memory[MEMORY_SIZE];
uint32_t page_table[NUMBER_OF_PAGES], next_available_frame, next_available_TLB;
uint32_t total, page_fault, TLB_miss;
uint8_t page_valid[NUMBER_OF_PAGES];
FILE *backing_storage, *input_file;

int init(int argc, char **argv) {
    if(argc != 2) {
        printf("Incorrect number of arguments.\n");
        return -1;
    }
    backing_storage = fopen(BACKING_STORAGE_FILE, "rb");
    if(backing_storage == NULL) {
        printf("Unable to open the backing storage file.");
        return -2;
    }
    input_file = fopen(argv[1], "r");
    if(input_file == NULL) {
        printf("Unable to open the input file.");
        return -3;
    }
    memset(page_valid, 0, sizeof(uint8_t) * NUMBER_OF_PAGES);
    next_available_frame = next_available_TLB = 0;
    return 0;
}

int main(int argc, char **argv) {
    char line[8];
    while(fgets(line, 8, input_file)) {
        uint32_t logical_add, physical_add;
        int8_t value;
        sscanf(line, "%u", &logical_add);
        physical_add = get_add(logical_add);
        value = read_memory(physical_add);
        printf("Virtual address: %u Physical address: %u Value: %d\n", logical_add, physical_add, value);
    }
    int fault_rate, tlb_rate;
    fault_rate = (page_fault/total) * 100;
    tlb_rate = ((total-TLB_miss)/total) * 100;
    printf("Page Fault Rate = %i\n", fault_rate );
    printf("TLB Hit Rate = %i\n", tlb_rate);
    fclose(input_file);
    fclose(backing_storage);
    return 0;
}

uint32_t victim_frm() {

    if(next_available_frame < NUMBER_OF_FRAMES) {
        return next_available_frame++;
    }
    uint32_t victim = (next_available_frame++) % NUMBER_OF_FRAMES;
    for(size_t i = 0; i != NUMBER_OF_PAGES; ++i) {
        if(page_valid[i] && page_table[i] == victim) {
            page_valid[i] = 0;
            break;
        }
    }
    return victim;
}

void pg_fault_handler(uint32_t pg_num) {
    page_table[pg_num] = victim_frm();
    fseek(backing_storage, pg_num * PAGE_SIZE, SEEK_SET);
    fread(memory + page_table[pg_num] * PAGE_SIZE, sizeof(int8_t), PAGE_SIZE, backing_storage);
    page_valid[pg_num] = 1;
    ++page_fault;
}

int find_in_TLB(uint32_t pg_num, uint32_t *frm_num) {
    for(size_t i = 0; i != TLB_SIZE; ++i) {
        if(TLB[i].valid && TLB[i].page == pg_num) {
            *frm_num = TLB[i].frame;
            return 1;
        }
    }
    return 0;
}

void update_TLB(uint32_t pg_num, uint32_t frm_num) {

    size_t victim = next_available_TLB % TLB_SIZE;
    next_available_TLB = (next_available_TLB + 1) % TLB_SIZE;
    TLB[victim].valid = 1;
    TLB[victim].page = pg_num, TLB[victim].frame = frm_num;
}

uint32_t get_add(uint32_t logical_add) {
    ++total;
    uint32_t pg_num, offset, frm_num;
    pg_num = (logical_add & PAGE_MASK) >> 8;
    offset = logical_add & OFFSET_MASK;
    if(!find_in_TLB(pg_num, &frm_num)) { 
        ++TLB_miss;
        if(page_valid[pg_num] == 0) { 
            pg_fault_handler(pg_num);
        }
        frm_num = page_table[pg_num];
        update_TLB(pg_num, frm_num);
    }
    uint32_t physical_add = ((frm_num << 8) | offset);
    return physical_add;
}

char read_memory(uint32_t physical_add) {
    return memory[physical_add];
}

