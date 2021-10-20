#ifndef BUS_H
#define BUS_H

#include<cstdint>
#include <array>
#include"Cpu.h"

    class Bus
    {
    public:
        Bus();
        ~Bus();
    public:
        //Devices on bus
        Cpu cpu;

        // fake RAM 64KB
        std::array<uint8_t, 64 * 1024> ram;
    public:
        void write(uint16_t addr, uint8_t data);
        uint8_t read(uint16_t addr, bool readOnly = false);
    };

#endif