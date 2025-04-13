#include "cpu/instr.h"
make_instr_func(leave)
{
    int len=1;
    cpu.esp=cpu.ebp;
    opr_src.type=OPR_MEM;
    opr_src.addr=cpu.esp;
    opr_src.sreg=SREG_DS;
    opr_src.data_size=data_size;
    operand_read(&opr_src);
    cpu.ebp=opr_src.val;
    cpu.esp+=opr_src.data_size/8;
    return len;
}
