#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg)
{
    /* TODO: perform segment translation from virtual address to linear address
     * by reading the invisible part of the segment register 'sreg'
     */
    return cpu.segReg[sreg].base + offset;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg)
{
    uint32_t addr = cpu.gdtr.base + cpu.segReg[sreg].index * 8;
    assert(((SegDesc*)(hw_mem + addr))->granularity == 1);
    cpu.segReg[sreg].privilege_level = ((SegDesc*)(hw_mem + addr))->privilege_level;
    cpu.segReg[sreg].soft_use = ((SegDesc*)(hw_mem + addr))->soft_use;
    cpu.segReg[sreg].type = ((SegDesc*)(hw_mem + addr))->type;
    cpu.segReg[sreg].limit = (((SegDesc*)(hw_mem + addr))->limit_15_0 + ((((SegDesc*)(hw_mem + addr))->limit_19_16) << 16)) & 0xfffff;
    cpu.segReg[sreg].base = (((SegDesc*)(hw_mem + addr))->base_15_0 + ((((SegDesc*)(hw_mem + addr))->base_23_16) << 16) + ((((SegDesc*)(hw_mem + addr))->base_31_24) << 24));
    assert(cpu.segReg[sreg].limit == 0x000fffff);
    assert(cpu.segReg[sreg].base == 0);
    
}

