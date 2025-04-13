#include "cpu/instr.h"
#include "device/port_io.h"
make_instr_func(out_b) {
    uint16_t index = cpu.edx & 0xffff;
    pio_write(index, 1, cpu.eax & 0xff);
    return 1;
}
make_instr_func(out_v) {
    uint16_t index = cpu.edx & 0xffff;
    if (data_size == 16)
        pio_write(index, 2, cpu.eax & 0xffff);
    else
        pio_write(index, 4, cpu.eax);
    return 1;
}