#include "MapperFour.h"

MapperFour::MapperFour(uint8_t prgBanks, uint8_t chrBanks):Mapper(prgBanks, chrBanks)
{
    vRAMStatic.resize(32 * 1024);
}

bool MapperFour::cpuMapRead(uint16_t addr, uint32_t& mapped_addr, uint8_t& data)
{
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        // д�뵽������ ��̬ram��
        mapped_addr = 0xFFFFFFFF;
        // RAM�ж�ȡ����
        data = vRAMStatic[addr & 0x1FFF];
        return true;
    }

    if (addr >= 0x8000 && addr <= 0x9FFF) {
        mapped_addr = pPRGBank[0] + (addr & 0x1FFF);
        return true;
    }

    if (addr >= 0xA000 && addr <= 0xBFFF) {
        mapped_addr = pPRGBank[1] + (addr & 0x1FFF);
        return true;
    }

    if (addr >= 0xC000 && addr <= 0xDFFF) {
        mapped_addr = pPRGBank[2] + (addr & 0x1FFF);
        return true;
    }

    if (addr >= 0xE000 && addr <= 0xFFFF) {
        mapped_addr = pPRGBank[3] + (addr & 0x1FFF);
        return true;
    }
    return false;
}

bool MapperFour::cpuMapWrite(uint16_t addr, uint32_t mapped_addr, uint8_t data)
{
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        // д�뵽������ ��̬ram��
        mapped_addr = 0xFFFFFFFF;
        // ������д��RAM��
        vRAMStatic[addr & 0x1FFF] = data;
        return true;
    }

    if (addr >= 0x8000 && addr <= 0x9FFF) {
        // bank select 
        if (!(addr & 0x0001)) {
            nTargetRegister = data & 0x07;
        }
        else{
            pRegister[nTargetRegister] = data;

            if (bCHRInversion) {
                pCHRBank[0] = pRegister[2] * 0x0400;
                pCHRBank[1] = pRegister[3] * 0x0400;
                pCHRBank[2] = pRegister[4] * 0x0400;
                pCHRBank[3] = pRegister[5] * 0x0400;
                pCHRBank[4] = (pRegister[0] & 0xFE) * 0x0400;
                pCHRBank[5] = pRegister[0] * 0x0400 + 0X0400;
                pCHRBank[6] = (pRegister[1] & 0xFE)* 0x0400;
                pCHRBank[7] = pRegister[1] * 0x0400 + 0x0400;
            }
            else {
                pCHRBank[0] = (pRegister[0] & 0xFE) * 0x0400;
                pCHRBank[1] = pRegister[0] * 0x0400 + 0X0400;
                pCHRBank[2] = (pRegister[1] & 0xFE) * 0x0400;
                pCHRBank[3] = pRegister[1] * 0x0400 + 0x0400;
                pCHRBank[4] = pRegister[2] * 0x0400;
                pCHRBank[5] = pRegister[3] * 0x0400;
                pCHRBank[6] = pRegister[4] * 0x0400;
                pCHRBank[7] = pRegister[5] * 0x0400;
            }

            if (bPRGBankMode) {
                pPRGBank[2] = (pRegister[6] & 0x3F) * 0x2000;
                pPRGBank[0] = (nPRGBanks * 2 - 2) * 0X2000;
            }else{
                pPRGBank[0] = (pRegister[6] & 0x3F) * 0x2000;
                pPRGBank[2] = (nPRGBanks * 2 - 2) * 0x2000;
            }

            pPRGBank[1] = (pRegister[7] & 0x3F) * 0x2000;
            pPRGBank[3] = (nPRGBanks * 2 - 1) * 0X2000;
        }
        return false;
    }

    if (addr >= 0xA000 && addr <= 0xBFFF) {
        if (!(addr & 0x0001)) {
            if (data & 0x01) mirrormode = MIRROR::HORIZONTAL;
            else mirrormode = MIRROR::VERTICAL;
        }
        else {
            //PRG RAM protect
        }
        return false;
    }

    if (addr >= 0xC000 && addr <= 0xDFFF) {
        if (!(addr & 0x0001)) nIRQReload = data;
        else nIRQCounter = 0x0000;
        return false;
    }

    if (addr >= 0xE000 && addr <= 0xFFFF) {
        if (!(addr & 0x0001)) {
            bIRQEnable = false;
            bIRQActive = false;
        }
        else bIRQEnable = true;
        return false;
    }
    return false;
}

bool MapperFour::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr >= 0x0000 && addr <= 0x03FF) {
        mapped_addr = pCHRBank[0] + (addr & 0x03FF);
        return true;
    }

    if (addr >= 0x0400 && addr <= 0x07FF) {
        mapped_addr = pCHRBank[1] + (addr & 0x03FF);
        return true;
    }

    if (addr >= 0x0800 && addr <= 0x0BFF) {
        mapped_addr = pCHRBank[2] + (addr & 0x03FF);
        return true;
    }

    if (addr >= 0x0C00 && addr <= 0x0FFF) {
        mapped_addr = pCHRBank[3] + (addr & 0x03FF);
        return true;
    }

    if (addr >= 0x1000 && addr <= 0x13FF) {
        mapped_addr = pCHRBank[4] + (addr & 0x03FF);
        return true;
    }

    if (addr >= 0x1400 && addr <= 0x17FF) {
        mapped_addr = pCHRBank[5] + (addr & 0x03FF);
        return true;
    }
    if (addr >= 0x1800 && addr <= 0x1BFF) {
        mapped_addr = pCHRBank[6] + (addr & 0x03FF);
        return true;
    }
    if (addr >= 0x1C00 && addr <= 0x1FFF) {
        mapped_addr = pCHRBank[7] + (addr & 0x03FF);
        return true;
    }
    return false;
}

bool MapperFour::ppuMapWrite(uint16_t addr, uint32_t mapped_addr)
{
    return false;
}

void MapperFour::reset()
{
    nTargetRegister = 0x00;
    bPRGBankMode = false;
    bCHRInversion = false;
    mirrormode = MIRROR::HORIZONTAL;

    bIRQActive = false;
    bIRQEnable = false;
    bIRQUpdate = false;
    nIRQCounter = 0x0000;
    nIRQReload = 0x0000;

    for (int i = 0; i < 4; i++) pPRGBank[i] = 0;
    for (int i = 0; i < 8; i++) {
        pCHRBank[i] = 0;
        pRegister[i] = 0;
    }

    pPRGBank[0] = 0;
    pPRGBank[1] = 0x2000;
    pPRGBank[2] = (nPRGBanks * 2 - 2) * 0x2000;
    pPRGBank[3] = (nPRGBanks * 2 - 1) * 0x2000;
}

bool MapperFour::irqState()
{
    return bIRQActive;
}

void MapperFour::irqClear()
{
    bIRQActive = false;
}

void MapperFour::scanline()
{
    if (nIRQCounter == 0) {
        nIRQCounter = nIRQReload;
    }
    else nIRQCounter--;

    if (nIRQCounter == 0 && bIRQEnable) bIRQActive = true;
}

MIRROR MapperFour::mirror()
{
    return mirrormode;
}
