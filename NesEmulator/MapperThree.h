#ifndef MAPPERTHREE_H
#define MAPPERTHREE_H

#include "Mapper.h"
class MapperThree : public Mapper
{
public:
	MapperThree(uint8_t prgBanks, uint8_t chrBanks);
	
	//将cpu地址 转化成 PRG rom的偏移量
	bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr, uint8_t &data) override;
	bool cpuMapWrite(uint16_t addr, uint32_t mapped_addr, uint8_t data = 0) override;

	// 将ppu地址转化成 CHR Rom的偏移量
	bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
	bool ppuMapWrite(uint16_t addr, uint32_t mapped_addr) override;

	void reset() override;

private:
	uint8_t nCHRBankSelect = 0x00;
};

#endif