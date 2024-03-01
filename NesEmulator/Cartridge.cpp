#include "Cartridge.h"
#include <fstream>
#include "Mapper_000.h"
#include "MapperTwo.h"
#include "MapperThree.h"
#include "MapperFour.h"

struct sHeader {
	char name[4];
	uint8_t prg_rom_chunks;
	uint8_t chr_rom_chunks;
	uint8_t mapper1;
	uint8_t mapper2;
	uint8_t prg_ram_size;
	uint8_t tv_system1;
	uint8_t tv_system2;
	char unused[5];
};

Cartridge::Cartridge(const std::string &fileName)
{
	sHeader header;
	std::ifstream ifs;

	ifs.open(fileName, std::ifstream::binary);
	
	if (ifs.is_open()) {
		ifs.read((char*)&header, sizeof(sHeader));

		if (header.mapper1 & 0x04) 
			ifs.seekg(512, std::ios_base::cur);

		nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
		mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

		// 有三种文件格式
		uint8_t fileType = 1;

		if (fileType == 1) {
			nRPGBanks = header.prg_rom_chunks;
			vPRGMemory.resize(nRPGBanks * 16384);
			ifs.read((char*)vPRGMemory.data(), vPRGMemory.size());

			nCHRBanks = header.chr_rom_chunks;
			vCHRMemory.resize(nCHRBanks * 8192);
			ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());
		}

		switch (nMapperID)
		{
			case 0: pMapper = std::make_shared<Mapper_000>(nRPGBanks, nCHRBanks); break;
			case 2: pMapper = std::make_shared<MapperTwo>(nRPGBanks, nCHRBanks); break;
			case 3: pMapper = std::make_shared<MapperThree>(nRPGBanks, nCHRBanks); break;
			case 4: pMapper = std::make_shared<MapperFour>(nRPGBanks, nCHRBanks); break;
			default:
				break;
		}
		bImageValid = true;
		ifs.close();
	}
}

bool Cartridge::ImageValid()
{
	return bImageValid;
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t& data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->cpuMapRead(addr, mapped_addr, data)) {
		if (mapped_addr == 0xFFFFFFFF)
		{
			// Mapper has actually set the data value, 
			// for example cartridge based RAM
			return true;
		}
		data = vPRGMemory[mapped_addr];
		return true;
	}
	return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->cpuMapWrite(addr, mapped_addr)) {
		vPRGMemory[mapped_addr] = data;
		return true;
	}
	return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t& data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->ppuMapRead(addr, mapped_addr)) {
		data = vCHRMemory[mapped_addr];
		return true;
	}
	return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->ppuMapWrite(addr, mapped_addr)) {
		vCHRMemory[mapped_addr] = data;
		return true;
	}
	return false;
}

void Cartridge::reset()
{
	if (pMapper) pMapper->reset();
}
