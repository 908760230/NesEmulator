#include "Bus.h"

Bus::Bus()
{
    cpu.connectBus(this);
}

Bus::~Bus()
{
}

void Bus::cpuWrite(uint16_t addr, uint8_t data)
{
    if (cart->cpuWrite(addr, data)) {

    }
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        // ���ܵ�ַ�� 0~8091 ����ʵ������ mask 0x07ff ���߼������,
        // �ѵ�ַ�������� 0~2047��Χ�ڣ���2kb�ĵ�ַ��ΧΪһ��mirror
        cpuRam[addr & 0x07FF] = data;
    }
    else if (addr >= 0x2000 && addr <= 0x3fff) {
        // ppuʵ����ֻʹ��8����Ҫ�ļĴ����������൱�� addr % 8
        ppu.cpuWrite(addr & 0x0007, data);
    }
    else if (addr >= 0x4016 && addr <= 0x4017) {
        controlller_state[addr & 0x0001] = controlller[addr & 0x0001];
    }

}

uint8_t Bus::cpuRead(uint16_t addr, bool readOnly)
{
    uint8_t data = 0x00;
    if (cart->cpuRead(addr, data)) {

    }
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        // ϵͳRam�ĵ�ַ��Χ�� ÿ2KBһ��mirror
        data = cpuRam[addr & 0x07FF];
    }
    else if (addr >= 0x2000 && addr <= 0x3fff) {
        // ppu �Ĵ�����ֵ��ÿ8һ��mirror
        data = ppu.cpuRead(addr & 0x0007, readOnly);
    }
    else if (addr >= 0x4016 && addr <= 0x4017) {
       /* data = (controlller_state[addr & 0x0001] & 0x80) > 0;
        controlller_state[addr & 0x0001] <<= 1;*/
    }

    return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
    this->cart = cartridge;
    ppu.connectCartridge(cartridge);
}

void Bus::reset()
{
    cpu.reset();
    ppu.reset();
    nSystemClockCounter = 0;
}

void Bus::clock()
{
    ppu.clock();
    if (nSystemClockCounter % 3 == 0) cpu.clock();
    if (ppu.nmi) {
        ppu.nmi = false;
        cpu.nmi();
    }
    nSystemClockCounter++;
}
