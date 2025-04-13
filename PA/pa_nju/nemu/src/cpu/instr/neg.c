#include "cpu/instr.h"
static void instr_execute_1op() {
	operand_read(&opr_src);
	int count=sign_ext(opr_src.val,opr_src.data_size);
	if(opr_src.val==0){
		cpu.eflags.CF=0;
		cpu.eflags.ZF=1;
		cpu.eflags.SF=0;
		cpu.eflags.OF=0;
	}
	else{
		cpu.eflags.CF=1;
		cpu.eflags.ZF=0;
		if(count>0)
		    cpu.eflags.SF=1;
		else
		    cpu.eflags.SF=0;
		cpu.eflags.OF=0;
	}
	opr_src.val=-opr_src.val;
	operand_write(&opr_src);
}

make_instr_impl_1op(neg, rm, b)
make_instr_impl_1op(neg, rm, v)
