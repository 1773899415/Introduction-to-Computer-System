#include "cpu/instr.h"
make_instr_func(lea_1) {
	int len = 1; 
	opr_src.data_size = data_size;
	opr_dest.data_size = data_size;
	len += modrm_r_rm(eip + 1, &opr_dest, &opr_src);
	opr_dest.val = opr_src.addr;
	operand_write(&opr_dest);
	return len; 
}