#include "cpu/instr.h"

make_instr_func(jmp_near)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, data_size);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("jmp", "", 1 + data_size / 8, &rel);

        cpu.eip += offset;

        return 1 + data_size / 8;
}

make_instr_func(jmp_indirect) {
	int len = 1;
	opr_src.data_size = data_size;
	len += modrm_rm(eip + 1, &opr_src);
	operand_read(&opr_src);
	if(data_size == 16)
		cpu.eip = opr_src.val & 0xFFFF;
	else
		cpu.eip = opr_src.val;
	return 0;
}

make_instr_func(jmp_short)
{
    opr_src.type = OPR_IMM;
    opr_src.sreg = SREG_CS;
    opr_src.data_size = 8;
    opr_src.addr = eip + 1;
    operand_read(&opr_src);
    cpu.eip += sign_ext(opr_src.val, opr_src.data_size);
    return 2;
}

make_instr_func(ljmp) {
    opr_src.type = OPR_IMM;
    opr_src.sreg = SREG_CS;
    opr_src.data_size = data_size;
    opr_src.addr = eip + 1;
    operand_read(&opr_src);
    
    opr_dest.type = OPR_IMM;
    opr_dest.sreg = SREG_CS;
    opr_dest.data_size = 16;
    opr_dest.addr = eip + 1 + data_size/8;
    operand_read(&opr_dest);
    
    if (data_size == 16)
        cpu.eip = opr_src.val & 0xffff;
    else
        cpu.eip = opr_src.val;
    cpu.cs.val = opr_dest.val;
    load_sreg(SREG_CS);
    return 0;
}


