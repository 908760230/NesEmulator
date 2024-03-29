#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <cstdint>
#include <vector>
#include <memory>
#include <string>
#include "Mapper.h"

class Cartridge
{
public:
	Cartridge(const std::string &fileName);

	bool ImageValid();

	enum MIRROR
	{
		HORIZONTAL,
		VERTICAL,
		ONESCREEN_LO,
		ONESCREEN_HI,
	} mirror = HORIZONTAL;


	bool cpuRead(uint16_t addr, uint8_t& data);
	bool cpuWrite(uint16_t addr, uint8_t data);

	bool ppuRead(uint16_t addr, uint8_t& data);
	bool ppuWrite(uint16_t addr, uint8_t data);

	void reset();
private:
	bool bImageValid = false;
	uint8_t nMapperID = 0;
	uint8_t nRPGBanks = 0;
	uint8_t nCHRBanks = 0;

	std::vector<uint8_t> vPRGMemory;
	std::vector<uint8_t> vCHRMemory;

	std::shared_ptr<Mapper> pMapper;
};

#endif