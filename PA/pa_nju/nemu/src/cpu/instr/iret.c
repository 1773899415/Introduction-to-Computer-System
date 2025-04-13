#include "cpu/instr.h"
make_instr_func(iret) {
    opr_src.type = OPR_MEM;
    opr_src.data_size = 32;
    opr_src.addr = cpu.esp;
    opr_src.sreg = SREG_SS;
    operand_read(&opr_src);
    cpu.eip = opr_src.val;
    cpu.esp += 4;
    
    opr_src.type = OPR_MEM;
    opr_src.data_size = 16;
    opr_src.addr = cpu.esp;
    opr_src.sreg = SREG_SS;
    operand_read(&opr_src);
    cpu.cs.val = opr_src.val;
    cpu.esp += 4;
    
    opr_src.type = OPR_MEM;
    opr_src.data_size = 32;
    opr_src.addr = cpu.esp;
    opr_src.sreg = SREG_SS;
    operand_read(&opr_src);
    cpu.eflags.val = opr_src.val;
    cpu.esp += 4;
    
    return 0;
}
