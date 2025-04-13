#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	uint32_t index1 = (cpu.cr3.pdbr << 12) + (((laddr >> 22) & 0x3ff) << 2);
	assert(((PDE*)(hw_mem + index1))->present == 1);
	uint32_t base_of_page = ((PDE*)(hw_mem + index1))->page_frame;
	uint32_t index2 = (base_of_page << 12) + (((laddr >> 12) & 0x3ff) << 2);
	assert(((PTE*)(hw_mem + index2))->present == 1);
	uint32_t base_of_phy = ((PTE*)(hw_mem + index2))->page_frame;
	paddr_t p_addr = (base_of_phy << 12) + (laddr & 0xfff);
	return p_addr;
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
