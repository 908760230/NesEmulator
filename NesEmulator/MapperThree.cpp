#include "MapperThree.h"

MapperThree::MapperThree(uint8_t prgBanks, uint8_t chrBanks):Mapper(prgBanks,chrBanks){
}

bool MapperThree::cpuMapRead(uint16_t addr, uint32_t& mapped_addr, uint8_t& data)
{
	if (addr >= 0x8000 && addr <= 0xFFFF) {
		// 16K ROM
		if (nPRGBanks == 1)
			mapped_addr = addr & 0x3FFF;
		if (nPRGBanks == 2)
			mapped_addr = addr & 0x7FFF;
		return true;
	}
	return false;
}

bool MapperThree::cpuMapWrite(uint16_t addr, uint32_t mapped_addr, uint8_t data)
{
	if (addr >= 0x8000 && addr <= 0xFFFF) {
		nCHRBankSelect = data & 0x03;
		mapped_addr = addr;
	}
	return false;
}

bool MapperThree::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
	if (addr < 0x2000) {
		mapped_addr = nCHRBankSelect * 0x2000 + addr;
		return true;
	}
	return false;
}

bool MapperThree::ppuMapWrite(uint16_t addr, uint32_t mapped_addr)
{
	return false;
}

void MapperThree::reset()
{
	nCHRBankSelect = 0;
}
