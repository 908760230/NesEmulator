#ifndef OLC2C02_H
#define OLC2C02_H

#include <cstdint>
#include "Cartridge.h"
#include <memory>

#include "olcPixelGameEngine.h"


union loopy_register
{
	struct {
		uint16_t coarse_x : 5;
		uint16_t coarse_y : 5;
		uint16_t nametable_x : 1;
		uint16_t nametable_y : 1;
		uint16_t fine_y : 3;
		uint16_t unused : 1;
	};
	uint16_t reg = 0x0000;
};



class olc2c02
{
public:
	olc2c02();

	uint8_t cpuRead(uint16_t addr, bool readOnly = false);
	void cpuWrite(uint16_t addr, uint8_t data);
	   
	uint8_t ppuRead(uint16_t addr, bool readOnly = false);
	void ppuWrite(uint16_t addr, uint8_t data);

	void connectCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void clock();
	void reset();

	bool frameComplete = false;
	bool nmi = false;
	uint8_t tblName[2][1024];      // name table: 保存图片的布局

private :
	uint8_t tblPattern[2][4096];   // spirte 图片
	uint8_t tblPalette[32];        // 颜色值

	int16_t scanLine = 0;
	int16_t cycle = 0;

	std::shared_ptr<Cartridge> cart;

public:
	olc::Sprite& getScreen();
	olc::Sprite& getNameTable(uint8_t i);
	olc::Sprite& getPatternTable(uint8_t i, uint8_t palette);
	olc::Pixel& GetColourFromPaletteRam(uint8_t palette, uint8_t pixel);
private:
	// for debugging
	olc::Pixel palScreen[64];
	olc::Sprite sprScreen = olc::Sprite(256, 240);
	olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240) ,olc::Sprite(256, 240) };
	olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128) ,olc::Sprite(128, 128) };


	union 
	{
		struct {
			uint8_t unused : 5;
			uint8_t sprite_overflow : 1;
			uint8_t sprite_zero_hit : 1;
			uint8_t vertical_blank : 1; // 判断是否在屏幕区域，还是超越屏幕的空白区域
			// vertical_blank + NMI中断 用于 ppu 与cpu 之间同步
		};
		uint8_t reg;
	} status;

	union
	{
		struct {
			uint8_t grayscale : 1;
			uint8_t render_background_left : 1;
			uint8_t render_sprites_left : 1;
			uint8_t render_background : 1;
			uint8_t render_sprites : 1;
			uint8_t enhance_red : 1;
			uint8_t enhance_green : 1;
			uint8_t enhance_blue: 1;
		};
		uint8_t reg;
	} mask;

	union
	{
		struct {
			uint8_t nametable_x : 1;
			uint8_t nametable_y : 1;
			uint8_t increment_mode : 1;
			uint8_t pattern_sprite : 1;
			uint8_t pattern_background : 1;
			uint8_t sprite_size : 1;
			uint8_t slave_mode : 1;
			uint8_t enable_nmi : 1;
		};
		uint8_t reg;
	} control;

	uint8_t address_latch = 0x00; // 判断高低字节
	uint8_t ppu_data_buffer = 0x00; // 读取ppu数据时有一个周期的延迟，所以缓存数据

	loopy_register vram_addr;
	loopy_register tram_addr;
	uint8_t fine_x = 0;

	uint8_t bg_next_tile_id = 0;
	uint8_t bg_next_tile_attrib = 0;
	uint8_t bg_next_tile_lsb = 0;
	uint8_t bg_next_tile_msb = 0;
	
	uint16_t bg_shifter_pattern_lo = 0x0000;
	uint16_t bg_shifter_pattern_hi = 0x0000;
	uint16_t bg_shifter_attrib_lo = 0x0000;
	uint16_t bg_shifter_attrib_hi = 0x0000;
};

#endif

