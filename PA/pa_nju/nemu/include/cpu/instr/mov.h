#ifndef __INSTR_MOV_H__
#define __INSTR_MOV_H__
int mov_r2rm_b(uint32_t eip, uint8_t opcode);
int mov_r2rm_v(uint32_t eip, uint8_t opcode);
int mov_rm2r_b(uint32_t eip, uint8_t opcode);
int mov_rm2r_v(uint32_t eip, uint8_t opcode);
int mov_i2rm_b(uint32_t eip, uint8_t opcode);
int mov_i2rm_v(uint32_t eip, uint8_t opcode);
int mov_i2r_b(uint32_t eip, uint8_t opcode);
int mov_i2r_v(uint32_t eip, uint8_t opcode);
int mov_o2a_b(uint32_t eip, uint8_t opcode);
int mov_o2a_v(uint32_t eip, uint8_t opcode);
int mov_a2o_b(uint32_t eip, uint8_t opcode);
int mov_a2o_v(uint32_t eip, uint8_t opcode);
int mov_zrm82r_v(uint32_t eip, uint8_t opcode);
int mov_zrm162r_l(uint32_t eip, uint8_t opcode);
int mov_srm82r_v(uint32_t eip, uint8_t opcode);
int mov_srm162r_l(uint32_t eip, uint8_t opcode);
int movs_b(uint32_t eip, uint8_t opcode);
int movs_v(uint32_t eip, uint8_t opcode);
int cmova_rm2r_v(uint32_t eip, uint8_t opcode);
int cmovae_rm2r_v(uint32_t eip, uint8_t opcode);
int cmovb_rm2r_v(uint32_t eip, uint8_t opcode);
int cmovbe_rm2r_v(uint32_t eip, uint8_t opcode);
int cmovg_rm2r_v(uint32_t eip, uint8_t opcode);
int cmovge_rm2r_v(uint32_t eip, uint8_t opcode);
int cmovl_rm2r_v(uint32_t eip, uint8_t opcode);
int cmovle_rm2r_v(uint32_t eip, uint8_t opcode);
int cmovne_rm2r_v(uint32_t eip, uint8_t opcode);
int cmovno_rm2r_v(uint32_t eip, uint8_t opcode);
int cmovnp_rm2r_v(uint32_t eip, uint8_t opcode);
int cmovns_rm2r_v(uint32_t eip, uint8_t opcode);
int cmove_rm2r_v(uint32_t eip, uint8_t opcode);
int cmovo_rm2r_v(uint32_t eip, uint8_t opcode);
int cmovp_rm2r_v(uint32_t eip, uint8_t opcode);
int cmovs_rm2r_v(uint32_t eip, uint8_t opcode);
int mov_c2r_l(uint32_t eip, uint8_t opcode);
int mov_r2c_l(uint32_t eip, uint8_t opcode);
int mov_rm2s_w(uint32_t eip, uint8_t opcode);
#endif





#ifndef __INSTR_MOV_H__
#define __INSTR_MOV_H__
make_instr_func(mov_r2rm_b);
make_instr_func(mov_r2rm_v);
make_instr_func(mov_rm2r_b);
make_instr_func(mov_rm2r_v);
make_instr_func(mov_i2rm_b);
make_instr_func(mov_i2rm_v);
make_instr_func(mov_i2r_b);
make_instr_func(mov_i2r_v);
make_instr_func(mov_o2a_b);
make_instr_func(mov_o2a_v);
make_instr_func(mov_a2o_b);
make_instr_func(mov_a2o_v);
make_instr_func(mov_zrm82r_v);
make_instr_func(mov_zrm162r_l);
make_instr_func(mov_srm82r_v);
make_instr_func(mov_srm162r_l);
make_instr_func(movs_v);
make_instr_func(movs_b);
make_instr_func(cmova_rm2r_v);
make_instr_func(cmovae_rm2r_v);
make_instr_func(cmovb_rm2r_v);
make_instr_func(cmovbe_rm2r_v);
make_instr_func(cmovg_rm2r_v);
make_instr_func(cmovge_rm2r_v);
make_instr_func(cmovl_rm2r_v);
make_instr_func(cmovle_rm2r_v);
make_instr_func(cmovne_rm2r_v);
make_instr_func(cmovno_rm2r_v);
make_instr_func(cmovnp_rm2r_v);
make_instr_func(cmovns_rm2r_v);
make_instr_func(cmovo_rm2r_v);
make_instr_func(cmovp_rm2r_v);
make_instr_func(cmovs_rm2r_v);
make_instr_func(cmove_rm2r_v);
make_instr_func(mov_c2r_l);
make_instr_func(mov_r2c_l);
make_instr_func(mov_rm2s_l);
#endif
