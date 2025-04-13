#include "cpu/instr.h"
make_instr_func(lidt) {
    int len = 1;
    opr_src.data_size = data_size;
    len += modrm_rm(eip + 1, &opr_src);
    opr_src.type = OPR_MEM;
    opr_src.data_size = 16;
    operand_read(&opr_src);
    cpu.idtr.limit = opr_src.val & 0xffff;
    opr_src.data_size = 32;
    opr_src.addr +=2;
    operand_read(&opr_src);
    cpu.idtr.base = opr_src.val;
    if (data_size == 16) {
        cpu.idtr.base = cpu.idtr.base & 0xffffff;
    }
    return len;
}