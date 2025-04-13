#include "cpu/instr.h"
#include "device/port_io.h"
make_instr_func(in_b) {
    uint16_t index = cpu.edx & 0xffff;
    cpu.eax = pio_read(index, 1) & 0xff;
    return 1;
}
make_instr_func(in_v) {
    uint16_t index = cpu.edx & 0xffff;
    cpu.eax = pio_read(index, data_size / 8);
    if(data_size == 16)
        cpu.eax = cpu.eax & 0xffff;
    return 1;
}