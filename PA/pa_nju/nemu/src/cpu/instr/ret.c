#include "cpu/instr.h"
make_instr_func(ret){
    opr_src.type=OPR_MEM;
    opr_src.data_size=data_size;
    opr_src.addr=cpu.esp;
    opr_src.sreg=SREG_SS;
    operand_read(&opr_src);
    cpu.eip=opr_src.val;
    cpu.esp+=opr_src.data_size/8;
    return 0;
}
make_instr_func(ret_i){
    opr_src.type=OPR_MEM;
    opr_src.data_size=data_size;
    opr_src.addr=cpu.esp;
    opr_src.sreg=SREG_SS;
    operand_read(&opr_src);
    cpu.eip=opr_src.val;
    opr_dest.type=OPR_IMM;
    opr_dest.data_size=16;
    opr_dest.addr=eip+1;
    opr_dest.sreg=SREG_CS;
    operand_read(&opr_dest);
	cpu.esp+=opr_dest.data_size/8+sign_ext(opr_dest.val,16);
    return 0;
}