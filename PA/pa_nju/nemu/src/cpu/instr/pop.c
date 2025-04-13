#include "cpu/instr.h"
static void instr_execute_1op() {
    opr_dest.addr=cpu.esp;
	opr_dest.type=OPR_MEM;
	opr_dest.sreg=SREG_DS;
	opr_dest.data_size=data_size;
	operand_read(&opr_dest);
	opr_src.val=opr_dest.val;
	operand_write(&opr_src);
	cpu.esp+=data_size/8;
}
make_instr_impl_1op(pop, r, v)
make_instr_impl_1op(pop, rm, v)
make_instr_impl_1op(pop, i, v)
make_instr_impl_1op(pop, i, b)

make_instr_func(popa) {
    opr_src.data_size = data_size;
	opr_src.sreg = SREG_SS;
    
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    operand_read(&opr_src);
    opr_src.addr = 7;
    opr_src.type = OPR_REG;
    operand_write(&opr_src);
    cpu.esp += opr_src.data_size / 8;
    
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    operand_read(&opr_src);
    opr_src.addr = 6;
    opr_src.type = OPR_REG;
    operand_write(&opr_src);
    cpu.esp += opr_src.data_size / 8;
    
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    operand_read(&opr_src);
    opr_src.addr = 5;
    opr_src.type = OPR_REG;
    operand_write(&opr_src);
    cpu.esp += opr_src.data_size / 8;
    
    cpu.esp += opr_src.data_size / 8;
    
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    operand_read(&opr_src);
    opr_src.addr = 3;
    opr_src.type = OPR_REG;
    operand_write(&opr_src);
    cpu.esp += opr_src.data_size / 8;
    
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    operand_read(&opr_src);
    opr_src.addr = 2;
    opr_src.type = OPR_REG;
    operand_write(&opr_src);
    cpu.esp += opr_src.data_size / 8;
    
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    operand_read(&opr_src);
    opr_src.addr = 1;
    opr_src.type = OPR_REG;
    operand_write(&opr_src);
    cpu.esp += opr_src.data_size / 8;
    
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    operand_read(&opr_src);
    opr_src.addr = 0;
    opr_src.type = OPR_REG;
    operand_write(&opr_src);
    cpu.esp += opr_src.data_size / 8;
    
    return 1;
}