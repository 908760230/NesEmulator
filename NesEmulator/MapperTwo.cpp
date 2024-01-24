#include "MapperTwo.h"

MapperTwo::MapperTwo(uint8_t prgBanks, uint8_t chrBanks): Mapper(prgBanks, chrBanks)
{
}

bool MapperTwo::cpuMapRead(uint16_t addr, uint32_t& mapped_addr, uint8_t& data)
{
	return false;
}

bool MapperTwo::cpuMapWrite(uint16_t addr, uint32_t mapped_addr, uint8_t data)
{
	return false;
}

bool MapperTwo::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
	return false;
}

bool MapperTwo::ppuMapWrite(uint16_t addr, uint32_t mapped_addr)
{
	return false;
}

void MapperTwo::reset()
{
}
