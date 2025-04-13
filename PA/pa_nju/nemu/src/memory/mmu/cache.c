#include "memory/mmu/cache.h"
#include "memory/memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

uint8_t cache_mem[128][8][64];

uint32_t tag[128][8];

bool valid[128][8];

// init the cache
void init_cache()
{
    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 8; j++) {
            valid[i][j] = false;
            tag[i][j] = 0;
        }
    }
    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 64; k++)
                cache_mem[i][j][k] = 0;
        }
    }
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data)
{
    memcpy(hw_mem + paddr, &data, len);
    uint32_t tag0 = (paddr >> 13) & 0x7ffff;
    uint32_t index = (paddr >> 6) & 0x7f;
    uint32_t offset = paddr & 0x3f;
    for (int i = 0; i < 8; i++) {
        if (valid[index][i] == true && tag[index][i] == tag0) {
            if (offset + len <= 64)
                memcpy(cache_mem[index][i] + offset, &data, len);
            else
            {
                cache_write(paddr, 64 - offset, data);
                cache_write(paddr + 64 - offset, len + offset - 64, data >> (8 * (64 - offset)));
            }
            break;
        }
    }
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len)
{
    uint32_t ret = 0;
    uint32_t tag0 = (paddr >> 13) & 0x7ffff;
    uint32_t index = (paddr >> 6) & 0x7f;
    uint32_t offset = paddr & 0x3f;
    int have_empty = -1;
    for (int i = 0; i < 8; i++) {
        if (valid[index][i] == true && tag[index][i] == tag0) {
            if (offset + len <= 64)
                memcpy(&ret, cache_mem[index][i] + offset, len);
            else
            {
                uint32_t left = 0, right = 0;
                memcpy(&left, cache_mem[index][i] + offset, 64 - offset);
                right = cache_read(paddr + 64 - offset, offset + len - 64) << (8 * (64 - offset));
                ret = left | right;
            }
            return ret;
        }
        if (valid[index][i] == false)
            have_empty = i;
    }
    memcpy(&ret, hw_mem + paddr, len);
    if (have_empty == -1) {
        srand((unsigned int)time(NULL));
        int r = rand() % 8;
        memcpy(cache_mem[index][r], hw_mem + paddr - offset, 64);
        valid[index][r] = true;
        tag[index][r] = tag0;
    }
    else {
        memcpy(cache_mem[index][have_empty], hw_mem + paddr - offset, 64);
        valid[index][have_empty] = true;
        tag[index][have_empty] = tag0;
    }
    return ret;
}

