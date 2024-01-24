#ifndef MAPPERFOUR_H
#define MAPPERFOUT_H

#include "Mapper.h"
#include <vector>

class MapperFour : public Mapper
{
public:
	MapperFour(uint8_t prgBanks, uint8_t chrBanks);

	//将cpu地址 转化成 PRG rom的偏移量
	bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr, uint8_t &data) override;
	bool cpuMapWrite(uint16_t addr, uint32_t mapped_addr, uint8_t data = 0) override;

	// 将ppu地址转化成 CHR Rom的偏移量
	bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
	bool ppuMapWrite(uint16_t addr, uint32_t mapped_addr) override;

	void reset() override;
	
	bool irqState() override;
	void irqClear() override;

	void scanline() override;
	MIRROR mirror() override;

private:
	uint8_t nTargetRegister = 0x00;
	bool bPRGBankMode = false;
	bool bCHRInversion = false;
	MIRROR mirrormode = MIRROR::HORIZONTAL;

	uint32_t pRegister[8];
	uint32_t pCHRBank[8];
	uint32_t pPRGBank[4];

	bool bIRQActive = false;
	bool bIRQEnable = false;
	bool bIRQUpdate = false;

	uint16_t nIRQCounter = 0x0000;
	uint16_t nIRQReload = 0x0000;

	std::vector<uint8_t> vRAMStatic;
};


#endif

