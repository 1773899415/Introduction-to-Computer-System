#include "cpu/cpu.h"

void set_CF_add(uint32_t result, uint32_t src, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = result < src;
}

void set_ZF(uint32_t result, size_t data_size) {
	result = result & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.ZF = (result == 0);
}

void set_SF(uint32_t result, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.SF = sign(result);
}

void set_PF(uint32_t result) {
    uint32_t count1 = 1,count2=0;
    for(uint32_t i = 0;i < 8;i++){
        if((result&count1)==count1)
            count2++;
        count1*=2;//为什么不能count1<<1;???
    }
    if((count2%2)==0)
        cpu.eflags.PF=1;
    else
        cpu.eflags.PF=0;
}

void set_OF_add(uint32_t result, uint32_t src, uint32_t dest, size_t data_size) {
	switch(data_size) {
		case 8: 
			result = sign_ext(result & 0xFF, 8); 
			src = sign_ext(src & 0xFF, 8); 
			dest = sign_ext(dest & 0xFF, 8); 
			break;
		case 16: 
			result = sign_ext(result & 0xFFFF, 16); 
			src = sign_ext(src & 0xFFFF, 16); 
			dest = sign_ext(dest & 0xFFFF, 16); 
			break;
		default: break;// do nothing
	}
	if(sign(src) == sign(dest)) {
		if(sign(src) != sign(result))
			cpu.eflags.OF = 1;
		else
			cpu.eflags.OF = 0;
	} else {
		cpu.eflags.OF = 0;
	}
}

void set_CF_adc(uint32_t result, uint32_t src, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(cpu.eflags.CF==1)
	    cpu.eflags.CF = result <= src;
	else if(cpu.eflags.CF==0)
	    cpu.eflags.CF = result < src;
}

void set_CF_sub(uint32_t result, uint32_t dest, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(result>dest)
	    cpu.eflags.CF=1;
	else
	    cpu.eflags.CF=0;
}

void set_OF_sub(uint32_t result, uint32_t src, uint32_t dest, size_t data_size) {
	switch(data_size) {
		case 8: 
			result = sign_ext(result & 0xFF, 8); 
			src = sign_ext(src & 0xFF, 8); 
			dest = sign_ext(dest & 0xFF, 8); 
			break;
		case 16: 
			result = sign_ext(result & 0xFFFF, 16); 
			src = sign_ext(src & 0xFFFF, 16); 
			dest = sign_ext(dest & 0xFFFF, 16); 
			break;
		default: break;// do nothing
	}
	if(sign(src) != sign(dest)) {
		if(sign(dest)==1){
			if(sign(result)==1)
			    cpu.eflags.OF = 0;
			else
			    cpu.eflags.OF = 1;
		}
		else{
		    if(sign(result)==1)
			    cpu.eflags.OF = 1;
			else
			    cpu.eflags.OF = 0;
		}
	} 
	else
	    cpu.eflags.OF = 0;
}

void set_CF_sbb(uint32_t result, uint32_t dest, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(cpu.eflags.CF==1){
	    if(result>=dest)
    	    cpu.eflags.CF=1;
    	else
    	    cpu.eflags.CF=0;
	}
	else{
    	if(result>dest)
    	    cpu.eflags.CF=1;
    	else
    	    cpu.eflags.CF=0;
	}
}

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest + src;
	set_CF_add(res, src, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_add(res, src, dest, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest + src+cpu.eflags.CF;
	set_CF_adc(res, src, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_add(res, src, dest, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest - src;
	set_CF_sub(res,dest,data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_sub(res, src, dest, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest - src-cpu.eflags.CF;
	set_CF_sbb(res,dest,data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_sub(res, src, dest, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	uint64_t res=(uint64_t)src*dest;
	if((res>>data_size)==0){
	    cpu.eflags.CF=0;
	    cpu.eflags.OF=0;
	}
	else{
	    cpu.eflags.CF=1;
	    cpu.eflags.OF=1;
	}
	return res;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	int64_t res=(int64_t)src*dest;
	int64_t temp=sign_ext(src*dest,32);
	if(res==temp){
	    cpu.eflags.CF=0;
	    cpu.eflags.OF=0;
	}
	else{
	    cpu.eflags.CF=1;
	    cpu.eflags.OF=1;
	}
	return res;
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
    uint32_t res=dest/src;
    return res;
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	int32_t res=dest/src;
	return res;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	uint32_t res=dest%src;
	return res;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest) {
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	int32_t res=dest%src;
	return res;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
    uint32_t res=src&dest;
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_PF(res);
    cpu.eflags.OF=0;
    cpu.eflags.CF=0;
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	uint32_t res=src^dest;
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_PF(res);
	cpu.eflags.OF=0;
    cpu.eflags.CF=0;
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	uint32_t res=src|dest;
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_PF(res);
	cpu.eflags.OF=0;
    cpu.eflags.CF=0;
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	uint32_t res=(dest<<src)& (0xFFFFFFFF >> (32 - data_size));
	set_ZF(res,data_size);
    set_SF(res,data_size);
    set_PF(res);
    cpu.eflags.CF=sign(dest<<(32 - data_size+src-1));
	return res;
#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
    uint32_t res=(((dest<<(32-data_size))>>(32-data_size))>>src)& (0xFFFFFFFF >> (32 - data_size));
	set_ZF(res,data_size);
    set_SF(res,data_size);
    set_PF(res);
    cpu.eflags.CF=(((dest<<(32-data_size))>>(32-data_size))>>(src-1))&1;
	return res;
#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	uint32_t res=((int32_t)((int32_t)(dest<<(32-data_size))>>(32-data_size))>>src)& (0xFFFFFFFF >> (32 - data_size));
	set_ZF(res,data_size);
    set_SF(res,data_size);
    set_PF(res);
    cpu.eflags.CF=((int32_t)((int32_t)(dest<<(32-data_size))>>(32-data_size))>>(src-1))&1; 
	return res;
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	return alu_shl(src,dest,data_size);
#endif
}
