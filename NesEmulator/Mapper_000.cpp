#include "Mapper_000.h"

Mapper_000::Mapper_000(uint8_t prgBanks, uint8_t chrBanks):Mapper(prgBanks,chrBanks)
{
}

bool Mapper_000::cpuMapRead(uint16_t addr, uint32_t& mapped_addr, uint8_t& data)
{
	/* PRGRom is 16kb
		cpu addr bus			PRG ROM
		0X8000 -> 0Xbfff   MAP   0x0000 -> 0x3fff
		0xc000 -> 0xffff   mirror 0x0000 -> 0x3fff

		PRGRom os 32kb
		0x8000 -> 0xFFF    Map  0x0000 -> 0x7FFF
	*/
	if (addr >= 0x8000 && addr <= 0xffff) {
		mapped_addr = addr & (nPRGBanks > 1 ? 0x7fff : 0x3fff);
		return true;
	}

	return false;
}

bool Mapper_000::cpuMapWrite(uint16_t addr, uint32_t mapped_addr, uint8_t data)
{
	if (addr >= 0x8000 && addr <= 0xFFFF) {
		mapped_addr = addr & (nPRGBanks > 1 ? 0X7FFF : 0X3FFF);
		return true;
	}
	return false;
}

bool Mapper_000::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
	if (addr >= 0x0000 && addr <= 0x1FFF) {
		mapped_addr = addr;
		return true;
	}
	return false;
}

bool Mapper_000::ppuMapWrite(uint16_t addr, uint32_t mapped_addr)
{
	if (addr >= 0x0000 && addr <= 0x1FFF) {
		if (nCHRBanks == 0) {
			mapped_addr = addr;
			return true;
		}
	}
	return false;
}

void Mapper_000::reset()
{
}
