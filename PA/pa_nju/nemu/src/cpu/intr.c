#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

void raise_intr(uint8_t intr_no)
{
    cpu.esp -= 4;
    opr_src.data_size = 32;
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    opr_src.sreg = SREG_SS;
    opr_src.val = cpu.eflags.val;
    operand_write(&opr_src);
    
    cpu.esp -= 4;//为何2字节栈帧却要-4
    opr_src.data_size = 16;
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    opr_src.sreg = SREG_SS;
    opr_src.val = cpu.cs.val;
    operand_write(&opr_src);
    
    cpu.esp -= 4;
    opr_src.data_size = 32;
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    opr_src.sreg = SREG_SS;
    opr_src.val = cpu.eip;
    operand_write(&opr_src);
    
    uint32_t paddr1 = page_translate(cpu.idtr.base + 8 * intr_no);
    if (((GateDesc*)(hw_mem + paddr1))->type == 15)
        cpu.eflags.IF = 0;
    uint16_t seg = ((GateDesc*)(hw_mem + paddr1))->selector;
    uint32_t offset = ((GateDesc*)(hw_mem + paddr1))->offset_15_0 + (((GateDesc*)(hw_mem + paddr1))->offset_31_16 << 16);
    cpu.cs.val = seg;
    cpu.eip = offset;   //eip里装入的是虚拟地址
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
