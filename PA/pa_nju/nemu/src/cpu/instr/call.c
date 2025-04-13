#include "cpu/instr.h"
make_instr_func(call_direct)
{
    opr_src.type = OPR_IMM;
    opr_src.data_size = data_size;
    opr_src.sreg=SREG_CS;
    opr_src.addr = eip + 1;
    operand_read(&opr_src);
    cpu.esp-=opr_src.data_size/8;
    opr_dest.data_size=data_size;
    opr_dest.type=OPR_MEM;
    opr_dest.sreg=SREG_SS;
    opr_dest.addr=cpu.esp;
    opr_dest.val=eip+1+opr_src.data_size/8;
    operand_write(&opr_dest);
	cpu.eip+=sign_ext(opr_src.val, data_size)+1+data_size/8;
	return 0;
}

make_instr_func(call_indirect){
	opr_src.type = OPR_IMM;
    opr_src.data_size = data_size;
    opr_src.sreg=SREG_CS;
    opr_dest.val=eip+modrm_rm(eip+1, &opr_src)+1;
    operand_read(&opr_src);
	cpu.esp-=data_size/8;
	opr_dest.data_size=data_size;
	opr_dest.type=OPR_MEM;
	opr_dest.sreg=SREG_SS;
	opr_dest.addr=cpu.esp;
	operand_write(&opr_dest);
	cpu.eip=opr_src.val;
	return 0;
}