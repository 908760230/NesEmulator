#ifndef BUS_H
#define BUS_H

#include<cstdint>
#include <array>
#include"Cpu.h"
#include "olc2c02.h"

class Bus
{
public:
    Bus();
    ~Bus();

    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool readOnly = false);

    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void reset();
    void clock();

    //Devices on bus
    Cpu cpu;
    olc2c02 ppu;

    // fake RAM 64KB
    std::array<uint8_t,2048> cpuRam;
    std::shared_ptr<Cartridge> cart;

    uint8_t controlller[2];

private:
    uint32_t nSystemClockCounter = 0;
    uint8_t controlller_state[2];

};

#endif