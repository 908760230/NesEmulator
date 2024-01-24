#include <iostream>
#include <sstream>
#include "Bus.h"
#include "Cpu.h"
#include "Utils.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Application : public olc::PixelGameEngine 
{
private:
    Bus nes;
    std::map<uint16_t, std::string> mapAsm;
    std::shared_ptr<Cartridge> cart;
    float residualTime = 0;
    bool emulationRun = false;

    uint8_t nSelectedPalette = 0x00;
public:
    Application() {
        sAppName = "Nes Emulator";
    }

private:

    void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns)
    {
        int nRamX = x, nRamY = y;
        for (int row = 0; row < nRows; row++)
        {
            std::string sOffset = "$" + Utils::toHex(nAddr, 4) + ":";
            for (int col = 0; col < nColumns; col++)
            {
                sOffset += " " + Utils::toHex(nes.cpuRead(nAddr, true), 2);
                nAddr += 1;
            }
            DrawString(nRamX, nRamY, sOffset);
            nRamY += 10;
        }
    }
  
    void DrawCpu(int x, int y)
    {
        std::string status = "STATUS: ";
        DrawString(x, y, "STATUS:", olc::WHITE);
        DrawString(x + 64, y, "N", nes.cpu.status & Cpu::N ? olc::GREEN : olc::RED);
        DrawString(x + 80, y, "V", nes.cpu.status & Cpu::V ? olc::GREEN : olc::RED);
        DrawString(x + 96, y, "-", nes.cpu.status & Cpu::U ? olc::GREEN : olc::RED);
        DrawString(x + 112, y, "B", nes.cpu.status & Cpu::B ? olc::GREEN : olc::RED);
        DrawString(x + 128, y, "D", nes.cpu.status & Cpu::D ? olc::GREEN : olc::RED);
        DrawString(x + 144, y, "I", nes.cpu.status & Cpu::I ? olc::GREEN : olc::RED);
        DrawString(x + 160, y, "Z", nes.cpu.status & Cpu::Z ? olc::GREEN : olc::RED);
        DrawString(x + 178, y, "C", nes.cpu.status & Cpu::C ? olc::GREEN : olc::RED);
        DrawString(x, y + 10, "PC: $" + Utils::toHex(nes.cpu.pc, 4));
        DrawString(x, y + 20, "A: $" + Utils::toHex(nes.cpu.accumulator, 2) + "  [" + std::to_string(nes.cpu.accumulator) + "]");
        DrawString(x, y + 30, "X: $" + Utils::toHex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]");
        DrawString(x, y + 40, "Y: $" + Utils::toHex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]");
        DrawString(x, y + 50, "Stack P: $" + Utils::toHex(nes.cpu.stackPointer, 4));
    }

    void DrawCode(int x, int y, int nLines)
    {
        auto it_a = mapAsm.find(nes.cpu.pc);
        int nLineY = (nLines >> 1) * 10 + y;
        if (it_a != mapAsm.end())
        {
            DrawString(x, nLineY, (*it_a).second, olc::CYAN);
            while (nLineY < (nLines * 10) + y)
            {
                nLineY += 10;
                if (++it_a != mapAsm.end())
                {
                    DrawString(x, nLineY, (*it_a).second);
                }
            }
        }

        it_a = mapAsm.find(nes.cpu.pc);
        nLineY = (nLines >> 1) * 10 + y;
        if (it_a != mapAsm.end())
        {
            while (nLineY > y)
            {
                nLineY -= 10;
                if (--it_a != mapAsm.end())
                {
                    DrawString(x, nLineY, (*it_a).second);
                }
            }
        }
    }

    bool OnUserCreate()
    {
        cart = std::make_shared<Cartridge>("nestest.nes");
        if (!cart->ImageValid()) return false;

        nes.insertCartridge(cart);

        
        ///*  https://www.masswerk.at/6502/assembler.html
        //    *=$8000
        //    LDX #10
        //    STX $0000
        //    LDX #3
        //    STX $0001
        //    LDY $0000
        //    LDA #0
        //    CLC
        //    loop
        //    ADC $0001
        //    DEY
        //    BNE loop
        //    STA $0002
        //    NOP
        //    NOP
        //    NOP
        //*/
        //std::stringstream ss;
        //ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
        //uint16_t nOffset = 0x8000;
        //while (!ss.eof())
        //{
        //    std::string b;
        //    ss >> b;
        //    nes.cpuRam[nOffset++] = (uint8_t)std::stoul(b, nullptr, 16);
        //}
        //nes.cpuRam[0xFFFC] = 0x00;
        //nes.cpuRam[0xFFFD] = 0x80;

        // Extract dissassembly
        mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

        // Reset
        nes.reset();
        return true;
    }

    bool OnUserUpdate(float fElapsedTime)
    {
        Clear(olc::DARK_BLUE);

        nes.controlller[0] = 0x00;
        nes.controlller[0] |= GetKey(olc::Key::X).bHeld ? 0x80 : 0x00;
        nes.controlller[0] |= GetKey(olc::Key::Z).bHeld ? 0x40 : 0x00;
        nes.controlller[0] |= GetKey(olc::Key::A).bHeld ? 0x20 : 0x00;
        nes.controlller[0] |= GetKey(olc::Key::S).bHeld ? 0x10 : 0x00;
        nes.controlller[0] |= GetKey(olc::Key::UP).bHeld ? 0x08 : 0x00;
        nes.controlller[0] |= GetKey(olc::Key::DOWN).bHeld ? 0x04 : 0x00;
        nes.controlller[0] |= GetKey(olc::Key::LEFT).bHeld ? 0x02 : 0x00;
        nes.controlller[0] |= GetKey(olc::Key::RIGHT).bHeld ? 0x01 : 0x00;
       

        if (emulationRun) {
            if (residualTime > 0.0f) {
                residualTime -= fElapsedTime;
            }
            else {
                residualTime += (1.0f / 60.0f) - fElapsedTime;
                do {
                    nes.clock();
                } while (!nes.ppu.frameComplete);
                nes.ppu.frameComplete = false;
            }

        }
        else {
            // һ��һ����ģ�����
            if(GetKey(olc::Key::C).bPressed)
            {
                // ִ��������cpuָ��
                do {
                    nes.clock();
                } while (!nes.cpu.complete());
                
                //  ִ��������clock
                do {
                    nes.clock();
                } while (!nes.cpu.complete());

            }

            // ִ����һ֡
            if (GetKey(olc::Key::F).bPressed) {
                // �㹻��clock ȥ����һ֡
                do {
                    nes.clock();
                } while (!nes.ppu.frameComplete);

                // ʣ���ʱ������ȥ��ɵ�ǰָ��
                do {
                    nes.clock();
                } while (!nes.cpu.complete());

                nes.ppu.frameComplete = false;
            }

        }

        if (GetKey(olc::Key::SPACE).bPressed) emulationRun = !emulationRun;
        if (GetKey(olc::Key::R).bPressed) nes.reset();
        if (GetKey(olc::Key::P).bPressed) (++nSelectedPalette) &= 0x07;
        
        //DrawRam(2, 2, 0x0000, 16, 16);
        //DrawRam(2, 182, 0x8000, 16, 16);
        //DrawCpu(448, 2);
        //DrawCode(448, 72, 26);
        
        DrawCpu(516, 2);
        DrawCode(516, 72, 26);

        //DrawString(10, 370, "SPACE = Step Instruction R = Reset, I = IRQ N = NMI");
        const int nSwatchSize = 6;
        for (int p = 0; p < 8; p++)
        {
            for (int s = 0; s < 4; s++) {
                FillRect(516 + p * (nSwatchSize * 5) + s * nSwatchSize, 340, nSwatchSize, nSwatchSize, nes.ppu.GetColourFromPaletteRam(p, s));
            }
        }
        DrawRect(516 + nSelectedPalette * (nSwatchSize * 5) - 1, 339, (nSwatchSize * 4), nSwatchSize, olc::WHITE);
        DrawSprite(516, 348, &nes.ppu.getPatternTable(0, nSelectedPalette));
        DrawSprite(648, 348, &nes.ppu.getPatternTable(1, nSelectedPalette));
        DrawSprite(0, 0, &nes.ppu.getScreen(), 2);

        //olc::Sprite& s = nes.ppu.getPatternTable(0, nSelectedPalette);
        //for (uint8_t y = 0; y < 30; y++)
        //{
        //    for (uint8_t x = 0; x < 32; x++)
        //    {
        //        //DrawString(x * 16, y * 16, Utils::toHex(uint32_t(nes.ppu.tblName[0][y * 32 + x]), 2));
        //        uint8_t id = uint32_t(nes.ppu.tblName[0][y * 32 + x]);
        //        DrawPartialSprite(x * 16, y * 16, &s, (id & 0x0F) << 3,
        //            ((id >> 4) & 0x0F) << 3, 8, 8, 2);
        //    }
        //}
        return true;
    }
};

int main() {
    Application app;
    app.Construct(750, 480, 2, 2);
    app.Start();
    return 0;
}