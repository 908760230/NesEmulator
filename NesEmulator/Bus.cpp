#include "Bus.h"

Bus::Bus()
{
    for (auto& val : ram) val = 0x00;
    cpu.connectBus(this);
}

Bus::~Bus()
{
}

void Bus::write(uint16_t addr, uint8_t data)
{
    if (addr >= 0x0000 && addr <= 0xffff) ram[addr] = data;
}

uint8_t Bus::read(uint16_t addr, bool readOnly)
{
    if (addr >= 0x0000 && addr <= 0xffff) return ram[addr];
    return 0;
}
