#include "cpu/instr.h"
static void instr_execute_1op()
{
    operand_read(&opr_src);
    opr_src.data_size=data_size;
    cpu.esp -= opr_src.data_size / 8;
    if (opr_src.data_size == 8){
        opr_src.val = sign_ext(opr_src.val, 8);
    }
    opr_dest.type = OPR_MEM;
    opr_dest.data_size = data_size;
    opr_dest.val = opr_src.val;
    opr_dest.sreg = SREG_SS;
    opr_dest.addr = cpu.esp;
    operand_write(&opr_dest);
}
make_instr_impl_1op(push, rm, v)
make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, i, b)
make_instr_impl_1op(push, i, v)

make_instr_func(pusha){
    uint32_t tmp = cpu.esp;
    opr_src.data_size = data_size;
	opr_src.sreg = SREG_SS;
    
    cpu.esp -= opr_src.data_size / 8;
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    opr_src.val = cpu.eax;
    operand_write(&opr_src);
    
    cpu.esp -= opr_src.data_size / 8;
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    opr_src.val = cpu.ecx;
    operand_write(&opr_src);
    
    cpu.esp -= opr_src.data_size / 8;
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    opr_src.val = cpu.edx;
    operand_write(&opr_src);
    
    cpu.esp -= opr_src.data_size / 8;
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    opr_src.val = cpu.ebx;
    operand_write(&opr_src);
    
    cpu.esp -= opr_src.data_size / 8;
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    opr_src.val = tmp;
    operand_write(&opr_src);
    
    cpu.esp -= opr_src.data_size / 8;
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    opr_src.val = cpu.ebp;
    operand_write(&opr_src);
    
    cpu.esp -= opr_src.data_size / 8;
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    opr_src.val = cpu.esi;
    operand_write(&opr_src);
    
    cpu.esp -= opr_src.data_size / 8;
    opr_src.addr = cpu.esp;
    opr_src.type = OPR_MEM;
    opr_src.val = cpu.edi;
    operand_write(&opr_src);
    
    return 1;
}