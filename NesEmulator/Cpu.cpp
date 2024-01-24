#include "Cpu.h"
#include "Bus.h"
#include "Utils.h"

#define LOGMODE

Cpu::Cpu() {
    instructions = { 
        { "BRK", &Cpu::BRK, &Cpu::IMM, 7 },{ "ORA", &Cpu::ORA, &Cpu::IZX, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 8 },{ "???", &Cpu::NOP, &Cpu::IMP, 3 },{ "ORA", &Cpu::ORA, &Cpu::ZP0, 3 },{ "ASL", &Cpu::ASL, &Cpu::ZP0, 5 },{ "???", &Cpu::XXX, &Cpu::IMP, 5 },{ "PHP", &Cpu::PHP, &Cpu::IMP, 3 },{ "ORA", &Cpu::ORA, &Cpu::IMM, 2 },{ "ASL", &Cpu::ASL, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "???", &Cpu::NOP, &Cpu::IMP, 4 },{ "ORA", &Cpu::ORA, &Cpu::ABS, 4 },{ "ASL", &Cpu::ASL, &Cpu::ABS, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 6 },
        { "BPL", &Cpu::BPL, &Cpu::REL, 2 },{ "ORA", &Cpu::ORA, &Cpu::IZY, 5 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 8 },{ "???", &Cpu::NOP, &Cpu::IMP, 4 },{ "ORA", &Cpu::ORA, &Cpu::ZPX, 4 },{ "ASL", &Cpu::ASL, &Cpu::ZPX, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 6 },{ "CLC", &Cpu::CLC, &Cpu::IMP, 2 },{ "ORA", &Cpu::ORA, &Cpu::ABY, 4 },{ "???", &Cpu::NOP, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 7 },{ "???", &Cpu::NOP, &Cpu::IMP, 4 },{ "ORA", &Cpu::ORA, &Cpu::ABX, 4 },{ "ASL", &Cpu::ASL, &Cpu::ABX, 7 },{ "???", &Cpu::XXX, &Cpu::IMP, 7 },
        { "JSR", &Cpu::JSR, &Cpu::ABS, 6 },{ "AND", &Cpu::AND, &Cpu::IZX, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 8 },{ "BIT", &Cpu::BIT, &Cpu::ZP0, 3 },{ "AND", &Cpu::AND, &Cpu::ZP0, 3 },{ "ROL", &Cpu::ROL, &Cpu::ZP0, 5 },{ "???", &Cpu::XXX, &Cpu::IMP, 5 },{ "PLP", &Cpu::PLP, &Cpu::IMP, 4 },{ "AND", &Cpu::AND, &Cpu::IMM, 2 },{ "ROL", &Cpu::ROL, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "BIT", &Cpu::BIT, &Cpu::ABS, 4 },{ "AND", &Cpu::AND, &Cpu::ABS, 4 },{ "ROL", &Cpu::ROL, &Cpu::ABS, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 6 },
        { "BMI", &Cpu::BMI, &Cpu::REL, 2 },{ "AND", &Cpu::AND, &Cpu::IZY, 5 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 8 },{ "???", &Cpu::NOP, &Cpu::IMP, 4 },{ "AND", &Cpu::AND, &Cpu::ZPX, 4 },{ "ROL", &Cpu::ROL, &Cpu::ZPX, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 6 },{ "SEC", &Cpu::SEC, &Cpu::IMP, 2 },{ "AND", &Cpu::AND, &Cpu::ABY, 4 },{ "???", &Cpu::NOP, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 7 },{ "???", &Cpu::NOP, &Cpu::IMP, 4 },{ "AND", &Cpu::AND, &Cpu::ABX, 4 },{ "ROL", &Cpu::ROL, &Cpu::ABX, 7 },{ "???", &Cpu::XXX, &Cpu::IMP, 7 },
        { "RTI", &Cpu::RTI, &Cpu::IMP, 6 },{ "EOR", &Cpu::EOR, &Cpu::IZX, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 8 },{ "???", &Cpu::NOP, &Cpu::IMP, 3 },{ "EOR", &Cpu::EOR, &Cpu::ZP0, 3 },{ "LSR", &Cpu::LSR, &Cpu::ZP0, 5 },{ "???", &Cpu::XXX, &Cpu::IMP, 5 },{ "PHA", &Cpu::PHA, &Cpu::IMP, 3 },{ "EOR", &Cpu::EOR, &Cpu::IMM, 2 },{ "LSR", &Cpu::LSR, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "JMP", &Cpu::JMP, &Cpu::ABS, 3 },{ "EOR", &Cpu::EOR, &Cpu::ABS, 4 },{ "LSR", &Cpu::LSR, &Cpu::ABS, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 6 },
        { "BVC", &Cpu::BVC, &Cpu::REL, 2 },{ "EOR", &Cpu::EOR, &Cpu::IZY, 5 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 8 },{ "???", &Cpu::NOP, &Cpu::IMP, 4 },{ "EOR", &Cpu::EOR, &Cpu::ZPX, 4 },{ "LSR", &Cpu::LSR, &Cpu::ZPX, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 6 },{ "CLI", &Cpu::CLI, &Cpu::IMP, 2 },{ "EOR", &Cpu::EOR, &Cpu::ABY, 4 },{ "???", &Cpu::NOP, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 7 },{ "???", &Cpu::NOP, &Cpu::IMP, 4 },{ "EOR", &Cpu::EOR, &Cpu::ABX, 4 },{ "LSR", &Cpu::LSR, &Cpu::ABX, 7 },{ "???", &Cpu::XXX, &Cpu::IMP, 7 },
        { "RTS", &Cpu::RTS, &Cpu::IMP, 6 },{ "ADC", &Cpu::ADC, &Cpu::IZX, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 8 },{ "???", &Cpu::NOP, &Cpu::IMP, 3 },{ "ADC", &Cpu::ADC, &Cpu::ZP0, 3 },{ "ROR", &Cpu::ROR, &Cpu::ZP0, 5 },{ "???", &Cpu::XXX, &Cpu::IMP, 5 },{ "PLA", &Cpu::PLA, &Cpu::IMP, 4 },{ "ADC", &Cpu::ADC, &Cpu::IMM, 2 },{ "ROR", &Cpu::ROR, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "JMP", &Cpu::JMP, &Cpu::IND, 5 },{ "ADC", &Cpu::ADC, &Cpu::ABS, 4 },{ "ROR", &Cpu::ROR, &Cpu::ABS, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 6 },
        { "BVS", &Cpu::BVS, &Cpu::REL, 2 },{ "ADC", &Cpu::ADC, &Cpu::IZY, 5 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 8 },{ "???", &Cpu::NOP, &Cpu::IMP, 4 },{ "ADC", &Cpu::ADC, &Cpu::ZPX, 4 },{ "ROR", &Cpu::ROR, &Cpu::ZPX, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 6 },{ "SEI", &Cpu::SEI, &Cpu::IMP, 2 },{ "ADC", &Cpu::ADC, &Cpu::ABY, 4 },{ "???", &Cpu::NOP, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 7 },{ "???", &Cpu::NOP, &Cpu::IMP, 4 },{ "ADC", &Cpu::ADC, &Cpu::ABX, 4 },{ "ROR", &Cpu::ROR, &Cpu::ABX, 7 },{ "???", &Cpu::XXX, &Cpu::IMP, 7 },
        { "???", &Cpu::NOP, &Cpu::IMP, 2 },{ "STA", &Cpu::STA, &Cpu::IZX, 6 },{ "???", &Cpu::NOP, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 6 },{ "STY", &Cpu::STY, &Cpu::ZP0, 3 },{ "STA", &Cpu::STA, &Cpu::ZP0, 3 },{ "STX", &Cpu::STX, &Cpu::ZP0, 3 },{ "???", &Cpu::XXX, &Cpu::IMP, 3 },{ "DEY", &Cpu::DEY, &Cpu::IMP, 2 },{ "???", &Cpu::NOP, &Cpu::IMP, 2 },{ "TXA", &Cpu::TXA, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "STY", &Cpu::STY, &Cpu::ABS, 4 },{ "STA", &Cpu::STA, &Cpu::ABS, 4 },{ "STX", &Cpu::STX, &Cpu::ABS, 4 },{ "???", &Cpu::XXX, &Cpu::IMP, 4 },
        { "BCC", &Cpu::BCC, &Cpu::REL, 2 },{ "STA", &Cpu::STA, &Cpu::IZY, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 6 },{ "STY", &Cpu::STY, &Cpu::ZPX, 4 },{ "STA", &Cpu::STA, &Cpu::ZPX, 4 },{ "STX", &Cpu::STX, &Cpu::ZPY, 4 },{ "???", &Cpu::XXX, &Cpu::IMP, 4 },{ "TYA", &Cpu::TYA, &Cpu::IMP, 2 },{ "STA", &Cpu::STA, &Cpu::ABY, 5 },{ "TXS", &Cpu::TXS, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 5 },{ "???", &Cpu::NOP, &Cpu::IMP, 5 },{ "STA", &Cpu::STA, &Cpu::ABX, 5 },{ "???", &Cpu::XXX, &Cpu::IMP, 5 },{ "???", &Cpu::XXX, &Cpu::IMP, 5 },
        { "LDY", &Cpu::LDY, &Cpu::IMM, 2 },{ "LDA", &Cpu::LDA, &Cpu::IZX, 6 },{ "LDX", &Cpu::LDX, &Cpu::IMM, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 6 },{ "LDY", &Cpu::LDY, &Cpu::ZP0, 3 },{ "LDA", &Cpu::LDA, &Cpu::ZP0, 3 },{ "LDX", &Cpu::LDX, &Cpu::ZP0, 3 },{ "???", &Cpu::XXX, &Cpu::IMP, 3 },{ "TAY", &Cpu::TAY, &Cpu::IMP, 2 },{ "LDA", &Cpu::LDA, &Cpu::IMM, 2 },{ "TAX", &Cpu::TAX, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "LDY", &Cpu::LDY, &Cpu::ABS, 4 },{ "LDA", &Cpu::LDA, &Cpu::ABS, 4 },{ "LDX", &Cpu::LDX, &Cpu::ABS, 4 },{ "???", &Cpu::XXX, &Cpu::IMP, 4 },
        { "BCS", &Cpu::BCS, &Cpu::REL, 2 },{ "LDA", &Cpu::LDA, &Cpu::IZY, 5 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 5 },{ "LDY", &Cpu::LDY, &Cpu::ZPX, 4 },{ "LDA", &Cpu::LDA, &Cpu::ZPX, 4 },{ "LDX", &Cpu::LDX, &Cpu::ZPY, 4 },{ "???", &Cpu::XXX, &Cpu::IMP, 4 },{ "CLV", &Cpu::CLV, &Cpu::IMP, 2 },{ "LDA", &Cpu::LDA, &Cpu::ABY, 4 },{ "TSX", &Cpu::TSX, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 4 },{ "LDY", &Cpu::LDY, &Cpu::ABX, 4 },{ "LDA", &Cpu::LDA, &Cpu::ABX, 4 },{ "LDX", &Cpu::LDX, &Cpu::ABY, 4 },{ "???", &Cpu::XXX, &Cpu::IMP, 4 },
        { "CPY", &Cpu::CPY, &Cpu::IMM, 2 },{ "CMP", &Cpu::CMP, &Cpu::IZX, 6 },{ "???", &Cpu::NOP, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 8 },{ "CPY", &Cpu::CPY, &Cpu::ZP0, 3 },{ "CMP", &Cpu::CMP, &Cpu::ZP0, 3 },{ "DEC", &Cpu::DEC, &Cpu::ZP0, 5 },{ "???", &Cpu::XXX, &Cpu::IMP, 5 },{ "INY", &Cpu::INY, &Cpu::IMP, 2 },{ "CMP", &Cpu::CMP, &Cpu::IMM, 2 },{ "DEX", &Cpu::DEX, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "CPY", &Cpu::CPY, &Cpu::ABS, 4 },{ "CMP", &Cpu::CMP, &Cpu::ABS, 4 },{ "DEC", &Cpu::DEC, &Cpu::ABS, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 6 },
        { "BNE", &Cpu::BNE, &Cpu::REL, 2 },{ "CMP", &Cpu::CMP, &Cpu::IZY, 5 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 8 },{ "???", &Cpu::NOP, &Cpu::IMP, 4 },{ "CMP", &Cpu::CMP, &Cpu::ZPX, 4 },{ "DEC", &Cpu::DEC, &Cpu::ZPX, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 6 },{ "CLD", &Cpu::CLD, &Cpu::IMP, 2 },{ "CMP", &Cpu::CMP, &Cpu::ABY, 4 },{ "NOP", &Cpu::NOP, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 7 },{ "???", &Cpu::NOP, &Cpu::IMP, 4 },{ "CMP", &Cpu::CMP, &Cpu::ABX, 4 },{ "DEC", &Cpu::DEC, &Cpu::ABX, 7 },{ "???", &Cpu::XXX, &Cpu::IMP, 7 },
        { "CPX", &Cpu::CPX, &Cpu::IMM, 2 },{ "SBC", &Cpu::SBC, &Cpu::IZX, 6 },{ "???", &Cpu::NOP, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 8 },{ "CPX", &Cpu::CPX, &Cpu::ZP0, 3 },{ "SBC", &Cpu::SBC, &Cpu::ZP0, 3 },{ "INC", &Cpu::INC, &Cpu::ZP0, 5 },{ "???", &Cpu::XXX, &Cpu::IMP, 5 },{ "INX", &Cpu::INX, &Cpu::IMP, 2 },{ "SBC", &Cpu::SBC, &Cpu::IMM, 2 },{ "NOP", &Cpu::NOP, &Cpu::IMP, 2 },{ "???", &Cpu::SBC, &Cpu::IMP, 2 },{ "CPX", &Cpu::CPX, &Cpu::ABS, 4 },{ "SBC", &Cpu::SBC, &Cpu::ABS, 4 },{ "INC", &Cpu::INC, &Cpu::ABS, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 6 },
        { "BEQ", &Cpu::BEQ, &Cpu::REL, 2 },{ "SBC", &Cpu::SBC, &Cpu::IZY, 5 },{ "???", &Cpu::XXX, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 8 },{ "???", &Cpu::NOP, &Cpu::IMP, 4 },{ "SBC", &Cpu::SBC, &Cpu::ZPX, 4 },{ "INC", &Cpu::INC, &Cpu::ZPX, 6 },{ "???", &Cpu::XXX, &Cpu::IMP, 6 },{ "SED", &Cpu::SED, &Cpu::IMP, 2 },{ "SBC", &Cpu::SBC, &Cpu::ABY, 4 },{ "NOP", &Cpu::NOP, &Cpu::IMP, 2 },{ "???", &Cpu::XXX, &Cpu::IMP, 7 },{ "???", &Cpu::NOP, &Cpu::IMP, 4 },{ "SBC", &Cpu::SBC, &Cpu::ABX, 4 },{ "INC", &Cpu::INC, &Cpu::ABX, 7 },{ "???", &Cpu::XXX, &Cpu::IMP, 7 },

    };
}

Cpu::~Cpu()
{
}

void Cpu::write(uint16_t addr, uint8_t data)
{
    busPtr->cpuWrite(addr, data);
}

uint8_t Cpu::read(uint16_t addr)
{
    return busPtr->cpuRead(addr);
}

uint8_t Cpu::getFlag(FLAGS flag)
{
    if ((status & flag) > 0) return 1;
    return 0;
}

void Cpu::setFlag(FLAGS flag, bool value)
{
    if (value) {
        status |= flag;
    }
    else status &= ~flag;  //���
}

// ������������ת��������ɶ�����ʽ
std::map<uint16_t, std::string> Cpu::disassemble(uint16_t start, uint16_t end)
{
    uint32_t addr = start;
    uint8_t value = 0x00, high = 0x00, low = 0x00;
    std::map<uint16_t, std::string> mapLines;
    uint16_t lineAddr = 0;

    while (addr <= (uint32_t)end)
    {
        lineAddr = addr;

        // Prefix line with instruction address
        std::string sInst = "$" + Utils::toHex(addr, 4) + ": ";

        // Read instruction, and get its readable name
        uint8_t opcode = busPtr->cpuRead(addr, true); addr++;
        sInst += instructions[opcode].name + " ";

        // Get oprands from desired locations, and form the
        // instruction based upon its addressing mode. These
        // routines mimmick the actual fetch routine of the
        // 6502 in order to get accurate data as part of the
        // instruction
        if (instructions[opcode].mode == &Cpu::IMP)
        {
            sInst += " {IMP}";
        }
        else if (instructions[opcode].mode == &Cpu::IMM)
        {
            value = busPtr->cpuRead(addr, true); addr++;
            sInst += "#$" + Utils::toHex(value, 2) + " {IMM}";
        }
        else if (instructions[opcode].mode == &Cpu::ZP0)
        {
            low = busPtr->cpuRead(addr, true); addr++;
            high = 0x00;
            sInst += "$" + Utils::toHex(low, 2) + " {ZP0}";
        }
        else if (instructions[opcode].mode == &Cpu::ZPX)
        {
            low = busPtr->cpuRead(addr, true); addr++;
            high = 0x00;
            sInst += "$" + Utils::toHex(low, 2) + ", X {ZPX}";
        }
        else if (instructions[opcode].mode == &Cpu::ZPY)
        {
            low = busPtr->cpuRead(addr, true); addr++;
            high = 0x00;
            sInst += "$" + Utils::toHex(low, 2) + ", Y {ZPY}";
        }
        else if (instructions[opcode].mode == &Cpu::IZX)
        {
            low = busPtr->cpuRead(addr, true); addr++;
            high = 0x00;
            sInst += "($" + Utils::toHex(low, 2) + ", X) {IZX}";
        }
        else if (instructions[opcode].mode == &Cpu::IZY)
        {
            low = busPtr->cpuRead(addr, true); addr++;
            high = 0x00;
            sInst += "($" + Utils::toHex(low, 2) + "), Y {IZY}";
        }
        else if (instructions[opcode].mode == &Cpu::ABS)
        {
            low = busPtr->cpuRead(addr, true); addr++;
            high = busPtr->cpuRead(addr, true); addr++;
            sInst += "$" + Utils::toHex((uint16_t)(high << 8) | low, 4) + " {ABS}";
        }
        else if (instructions[opcode].mode == &Cpu::ABX)
        {
            low = busPtr->cpuRead(addr, true); addr++;
            high = busPtr->cpuRead(addr, true); addr++;
            sInst += "$" + Utils::toHex((uint16_t)(high << 8) | low, 4) + ", X {ABX}";
        }
        else if (instructions[opcode].mode == &Cpu::ABY)
        {
            low = busPtr->cpuRead(addr, true); addr++;
            high = busPtr->cpuRead(addr, true); addr++;
            sInst += "$" + Utils::toHex((uint16_t)(high << 8) | low, 4) + ", Y {ABY}";
        }
        else if (instructions[opcode].mode == &Cpu::IND)
        {
            low = busPtr->cpuRead(addr, true); addr++;
            high = busPtr->cpuRead(addr, true); addr++;
            sInst += "($" + Utils::toHex((uint16_t)(high << 8) | low, 4) + ") {IND}";
        }
        else if (instructions[opcode].mode == &Cpu::REL)
        {
            value = busPtr->cpuRead(addr, true); addr++;
            sInst += "$" + Utils::toHex(value, 2) + " [$" + Utils::toHex(addr + (int8_t)value, 4) + "] {REL}";
        }

        // Add the formed string to a std::map, using the instruction's
        // address as the key. This makes it convenient to look for later
        // as the instructions are variable in length, so a straight up
        // incremental index is not sufficient.
        mapLines[lineAddr] = sInst;
    }

    return mapLines;
}

//ִ�м򵥵Ĳ�������������״̬λ���������ø�ֵΪ�ۼ����Ի�ȡ����PHAָ�
inline uint8_t Cpu::IMP() {
    fetched = accumulator;
    return 0;
}

//ֱ��ȡһ���ֽڵ�ֵ
inline uint8_t Cpu::IMM() {
    addrABS = pc++;
    return 0;
}

// �Ծ��Ե�ַ����ʽ���� 0x0000~0x00FF
uint8_t Cpu::ZP0()
{   
    addrABS = read(pc);
    pc++;
    addrABS &= 0x00FF;
    return 0;
}

// ����ҳѰַ��ͬ����x�Ĵ��������ݻ���ӵ����ֽڵ�ַ��
uint8_t Cpu::ZPX()
{
    addrABS = read(pc) + x;
    pc++;
    addrABS &= 0x00FF;
    return 0;
}


//y�Ĵ��������ݻ���ӵ����ֽڵ�ַ��
uint8_t Cpu::ZPY()
{
    addrABS = read(pc) + y;
    pc++;
    addrABS &= 0x00FF;

    return 0;
}

// ��ָ֧����е�Ѱַģʽ�����ַ����λ�ڷ�ָ֧���-128 ~ 127֮��
uint8_t Cpu::REL()
{
    addrREL = read(pc);
    pc++;
    if (addrREL & 0x80) addrREL |= 0xFF00;
    return 0;
}

// 16 bit ���ᱻʹ��
uint8_t Cpu::ABS()
{
    uint16_t low = read(pc);
    pc++;
    uint16_t high = read(pc);
    pc++;
    addrABS = (high << 8) | low;
    return 0;
}

// �ھ��Ե�ַ�Ļ����ϼ��� �Ĵ���x��ֵ
uint8_t Cpu::ABX()
{
    uint16_t low = read(pc);
    pc++;
    uint16_t high = read(pc);
    pc++;
    addrABS = (high << 8) | low;
    addrABS += x;

    if ((addrABS & 0xFF00) != (high << 8)) return 1; // ͨ���ۼӽ�λ�ж��Ƿ�Խ��
    return 0;
}

uint8_t Cpu::ABY()
{
    uint16_t low = read(pc);
    pc++;
    uint16_t high = read(pc);
    pc++;
    addrABS = (high << 8) | low;
    addrABS += y;

    if ((addrABS & 0xFF00) != (high << 8)) return 1; // �ж�Խ��
    return 0;
}

uint8_t Cpu::IND()
{
    uint16_t ptr_low = read(pc);
    pc++;
    uint16_t ptr_high = read(pc);
    pc++;

    uint16_t ptr = (ptr_high << 8) | ptr_low;

    if (ptr_low == 0x00FF) // ģ��Ӳ��bug
        addrABS = (read(ptr_high & 0xFF00) << 8) | read(ptr + 0); // ��Ч�ĵ�ַ
    else
        addrABS = (read(ptr + 1) << 8) | read(ptr + 0);

    return 0;
}

// ��ͨ��pc�õ���Ե�ַ����ͨ��X��ƫ���� ����ȡ���յ�ַ
uint8_t Cpu::IZX()
{
    uint16_t addr = read(pc);
    pc++;
    addr += (uint16_t)x;
    
    uint16_t addr_low = read( addr & 0x00FF);
    uint16_t addr_high = read((uint16_t)(addr + 1) & 0x00FF);
    
    addrABS = (addr_high << 8) | addr_low;
    return 0;
}

// �ȼ��Ѱַ�ߵ�λ���õ����Ӧ��16bit��ַ��Ȼ�������üĴ���Y��ֵ�õ����յĵ�ַ
uint8_t Cpu::IZY()
{
    uint16_t addr = read(pc);
    pc++;

    uint16_t addr_low = read(addr & 0x00FF);
    uint16_t addr_high = read((addr + 1) & 0x00FF);

    addrABS = (addr_high << 8) | addr_low;
    addrABS += y;

    if ((addrABS & 0xFF00) != addr_high) return 1; // Խ���˱߽�
    return 0;
}

// ��ӽ�λ
// a = a + M + C
// ��־�� C V N Z
uint8_t Cpu::ADC()
{
    // ��ȡ���ݣ���ӵ��ۼ�����
    fetch();

    uint16_t tmp = (uint16_t)accumulator + (uint16_t)fetched + (uint16_t)getFlag(C);
    
    setFlag(C, tmp > 255); // ���ý�λ��־
    setFlag(Z, (tmp & 0x00FF == 0));
    setFlag(N, tmp & 0x80);

    // v = (A ^ R) & ~(A ^ M)
    uint16_t flag = ~(uint16_t(accumulator) ^ uint16_t(fetched)) & uint16_t(accumulator) ^ uint16_t(tmp);
    // �����з������
    setFlag(V, flag & 0x0080);
    // ���ø�����־

    accumulator = tmp & 0x00FF; // ��������ص��ۼ�����

    return 1;// ��Ҫ�����һ��ʱ������
}

// �߼������
// A = A & M
// N Z
uint8_t Cpu::AND()
{
    fetch();
    accumulator &= fetched;
    setFlag(Z, accumulator == 0x00);
    setFlag(N, accumulator & 0x80);
    return 1;
}

// �㷨����
// A = C <- (A<<1) <- 0
// N Z C
uint8_t Cpu::ASL()
{
    fetch();
    uint16_t tmp = fetched << 1;
    setFlag(C, (tmp & 0xFF00) > 0);
    setFlag(Z, (tmp & 0x00FF) == 0x00);
    setFlag(N, tmp & 0x80);

    if (instructions[optCode].mode == &Cpu::IMP)
        accumulator = tmp & 0x00FF;
    else write(addrABS, tmp & 0x00FF);
    return 0;
}

// ��ָ֧������λ�����
uint8_t Cpu::BCC()
{
    if (getFlag(C) == 0) {
        cycles++;
        addrABS = pc + addrREL;

        if ((addrABS & 0xFF00) != (pc & 0xFF00)) cycles++;

        pc = addrABS;
    }
    return 0;
}

// ��ָ֧� �����λ������
uint8_t Cpu::BCS()
{
    if (getFlag(C) == 1) {
        cycles++;
        addrABS = pc + addrREL;

        if ((addrABS & 0xFF00) != (pc & 0xFF00)) cycles++;

        pc = addrABS;
    }
    return 0;
}

uint8_t Cpu::BEQ()
{
    if (getFlag(Z) == 1) {
        cycles++;
        addrABS = pc + addrREL;

        if ((addrABS & 0xFF00) != (pc & 0xFF00)) cycles++;

        pc = addrABS;
    }
    return 0;
}

uint8_t Cpu::BIT()
{
    fetch();
    uint16_t tmp = accumulator & fetched;
    setFlag(Z, (tmp & 0x00FF) == 0x00);
    setFlag(N, fetched & (1 << 7));
    setFlag(V, fetched & (1 << 6));
    return 0;
}

//��ָ֧� ����Ǹ���
uint8_t Cpu::BMI()
{
    if (getFlag(N) == 1) {
        cycles++;
        addrABS = pc + addrREL;

        if ((addrABS & 0xFF00) != (pc & 0xFF00)) cycles++;

        pc = addrABS;
    }
    return uint8_t();
}

// ��ָ֧���������
uint8_t Cpu::BNE()
{
    if (getFlag(Z) == 0) {
        cycles++;
        addrABS = pc + addrREL;

        if ((addrABS & 0xFF00) != (pc & 0xFF00)) cycles++;

        pc = addrABS;
    }
    return 0;
}

// 如果负数符号为假，则地址为pc + 相对偏移地址
uint8_t Cpu::BPL()
{
    if (getFlag(N) == 0) {
        cycles++;
        addrABS = pc + addrREL;

        if ((addrABS & 0xFF00) != (pc & 0xFF00)) cycles++;

        pc = addrABS;
    }
    return 0;
}

// �ж�ָ��
uint8_t Cpu::BRK()
{
    pc++;
    setFlag(I, 1); // �����ж�
    write(0x0100 + stackPointer, (pc >> 8) & 0x00FF); //д���λ
    stackPointer--;
    write(0x0100 + stackPointer, pc & 0x00FF); //д���λ
    stackPointer--;

    setFlag(B, 1); //�ж�
    write(0x0100 + stackPointer, status);
    stackPointer--;
    setFlag(B, 0);

    pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
    return 0;
}

// ��ָ֧������������
uint8_t Cpu::BVC()
{
    if (getFlag(V) == 0) {
        cycles++;
        addrABS = pc + addrREL;

        if ((addrABS & 0xFF00) != (pc & 0xFF00)) cycles++;

        pc = addrABS;
    }
    return 0;
}
// ������������
uint8_t Cpu::BVS()
{
    if (getFlag(V) == 1) {
        cycles++;
        addrABS = pc + addrREL;

        if ((addrABS & 0xFF00) != (pc & 0xFF00)) cycles++;

        pc = addrABS;
    }

    return 0;
}

// �����λ��־
uint8_t Cpu::CLC()
{
    setFlag(C, false);
    return 0;
}
// 关闭十进制模式
uint8_t Cpu::CLD()
{
    setFlag(D, false);
    return 0;
}
// �ر��ж�
uint8_t Cpu::CLI()
{
    setFlag(I, false);
    return 0;
}
//��� �����־
uint8_t Cpu::CLV()
{
    setFlag(V, false);
    return 0;
}
// �Ƚ��ۼ���   
uint8_t Cpu::CMP()
{
    fetch();
    uint16_t tmp = (uint16_t)accumulator - (uint16_t)fetched;
    setFlag(C, accumulator >= fetched);
    setFlag(Z, (tmp & 0x00FF) == 0x0000);
    setFlag(N, tmp & 0x0080);
    return 1;
}

uint8_t Cpu::CPX()
{
    fetch();
    uint16_t tmp = (uint16_t)x - (uint16_t)fetched;
    setFlag(C, x >= fetched);
    setFlag(Z, (tmp & 0x00FF) == 0x0000);
    setFlag(N, tmp & 0x0080);
    return 0;
}

uint8_t Cpu::CPY()
{
    fetch();
    uint16_t tmp = (uint16_t)y - (uint16_t)fetched;
    setFlag(C, y >= fetched);
    setFlag(Z, (tmp & 0x00FF) == 0x0000);
    setFlag(N, tmp & 0x0080);
    return 0;
}

// �ڴ�λ�õ�ֵ�Լ�һ
uint8_t Cpu::DEC()
{
    fetch();
    uint16_t tmp = fetched -1;
    write(addrABS, tmp & 0x00FF);
    setFlag(Z, (tmp & 0x00FF) == 0x0000);
    setFlag(N, tmp & 0x0080);
    return 1;
}
// x�Ĵ�����ֵ��һ
uint8_t Cpu::DEX()
{
    x--;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}
// y�Ĵ�����ֵ��һ
uint8_t Cpu::DEY()
{
    y--;
    setFlag(Z, y == 0x00);
    setFlag(N, y & 0x80);
    return 0;
}

// ������
uint8_t Cpu::EOR()
{
    fetch();
    accumulator ^= fetched;
    setFlag(Z, accumulator == 0x00);
    setFlag(N, accumulator & 0x80);
    return 1;
}
// �ڴ�λ�õ�ֵ��һ
uint8_t Cpu::INC()
{
    fetch();
    uint16_t tmp = fetched + 1;
    write(addrABS, tmp & 0x00FF);
    setFlag(Z, (tmp & 0x00FF) == 0x0000);
    setFlag(N, tmp & 0x0080);
    return 0;
}
// x �Ĵ�����ֵ��һ
uint8_t Cpu::INX()
{
    x++;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}

uint8_t Cpu::INY()
{
    y++;
    setFlag(Z, y == 0x00);
    setFlag(N, y & 0x80);
    return 0;
}
// ��ת��addr_absλ��
uint8_t Cpu::JMP()
{
    pc = addrABS;
    return 0;
}
// ��ת�������г���
// ����ǰPC ѹ��ջ�У�pc = addr_abs
uint8_t Cpu::JSR()
{
    pc--;
    write(0x0100 + stackPointer, (pc >> 8) & 0x00FF);
    stackPointer--;
    write(0x0100 + stackPointer, pc & 0x00FF);
    stackPointer--;

    pc = addrABS;
    return 0;
}
// ��ֵ���ۼ���
uint8_t Cpu::LDA()
{
    fetch();
    accumulator = fetched;
    setFlag(Z, accumulator == 0x00);
    setFlag(N, accumulator & 0x80);
    return 1;
}

// 加载数据到 寄存器Y
uint8_t Cpu::LDY()
{
    fetch();
    y = fetched;
    setFlag(Z, y == 0x00);
    setFlag(N, y & 0x80);
    return 1;
}

// 加载数据到 寄存器X
uint8_t Cpu::LDX()
{
    fetch();
    x = fetched;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 1;
}

uint8_t Cpu::LSR()
{
    fetch();
    setFlag(C, fetched & 0x0001);
    uint16_t tmp = fetched >> 1;
    setFlag(Z, (tmp & 0x00FF) == 0x0000);
    setFlag(N, tmp & 0x0080);
    if (instructions[optCode].mode == &Cpu::IMP) accumulator = tmp & 0x00FF;
    else write(addrABS, tmp & 0x00FF);

    return 0;
}

uint8_t Cpu::NOP()
{
    switch (optCode)
    {
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            return 1;
        default: break;
    }
    return 0;
}

uint8_t Cpu::XXX()
{
    return 0;
}
// ģ��ʱ������
void Cpu::clock()
{
    if (cycles == 0) {
        optCode = read(pc);

#ifdef LOGMODE
        uint16_t log_pc = pc;
#endif // LOGMODE


        setFlag(U, true); // unused ״̬��־ ʼ������Ϊ1
        pc++;

        cycles = instructions[optCode].cycles;

        uint16_t addressModeAddtionalCycle = (this->*instructions[optCode].mode)();
        uint16_t operateAdditionCycle = (this->*instructions[optCode].operate)();

        cycles += (addressModeAddtionalCycle & operateAdditionCycle);

        setFlag(U, true);

#ifdef LOGMODE
        if (logFile == nullptr) logFile = fopen("olc6502.txt", "wt");
        if (logFile != nullptr)
        {
            fprintf(logFile, "%10d:%02d PC:%04X %s A:%02X X:%02X Y:%02X %s%s%s%s%s%s%s%s STKP:%02X\n",
                clockCount, 0, log_pc, "XXX", accumulator, x, y,
                getFlag(N) ? "N" : ".", getFlag(V) ? "V" : ".", getFlag(U) ? "U" : ".",
                getFlag(B) ? "B" : ".", getFlag(D) ? "D" : ".", getFlag(I) ? "I" : ".",
                getFlag(Z) ? "Z" : ".", getFlag(C) ? "C" : ".", stackPointer);
        }
#endif // LOGMODE

       
    }
    clockCount++;

    cycles--;
}

void Cpu::reset()
{
    addrABS = 0xFFFC;
    uint16_t low = read(addrABS);
    uint16_t high = read(addrABS + 1);

    pc = (high << 8) | low;

    accumulator = 0;
    x = 0;
    y = 0;
    stackPointer = 0xFD;
    status = 0x00 | U;

    addrREL = 0x0000;
    addrABS = 0x0000;
    fetched = 0x00;

    cycles = 8;
}
// �ж�����
void Cpu::irq()
{
    // ���û�йر��ж�
    if (getFlag(I) == 0) {
        // ��pcѹ��ջ��
        write(0x0100 + stackPointer, (pc >> 8) & 0x00FF);
        stackPointer--;
        write(0x0100 + stackPointer, pc & 0x00FF);
        stackPointer--;
    
        setFlag(B, 0);
        setFlag(U, 1);
        setFlag(I, 1);
        write(0x0100 + stackPointer, status);
        stackPointer--;

        addrABS = 0xFFFE;
        uint16_t high = read(addrABS);
        uint16_t low = read(addrABS + 1);
        pc = (high << 8) | low;

        cycles = 7;
    }
}
// ���ܱ����Եķ���ʽ�жϣ�����IRQһ�������Ǵ�λ�� 0xFFFA����ȡ���µĳ���������ĵ�ַ
void Cpu::nmi()
{
    write(0x0100 + stackPointer, (pc >> 8) & 0x00FF);
    stackPointer--;
    write(0x0100 + stackPointer, pc & 0x00FF);
    stackPointer--;

    setFlag(B, false);
    setFlag(U, true);
    setFlag(I, true);
    write(0x0100 + stackPointer, status);
    stackPointer--;

    addrABS = 0xFFFA;
    uint16_t high = read(addrABS);
    uint16_t low = read(addrABS + 1);
    pc = (high << 8) | low;
    cycles = 8;
}

bool Cpu::complete()
{
    return cycles == 0;
}

uint8_t Cpu::fetch()
{
    if (!(instructions[optCode].mode == &Cpu::IMP)) fetched = read(addrABS);
    return fetched;
}
// �߼���
uint8_t Cpu::ORA()
{
    fetch();
    accumulator |= fetched;
    setFlag(Z, accumulator == 0x00);
    setFlag(N, accumulator & 0x80);
    return 1;
}
// ���ۼ�����ֵѹ��ջ��
uint8_t Cpu::PHA()
{
    write(0x0100 + stackPointer, accumulator);
    stackPointer--;
    return 0;
}
//��״̬�Ĵ���ѹ��ջ�У���ѹ��֮ǰ �жϱ�־Ӧ������Ϊ1
uint8_t Cpu::PHP()
{
    write(0x0100 + stackPointer, status | B | U);
    setFlag(B, 0);
    setFlag(U, 0);
    stackPointer--;
    return 0;
}
// ��ջ�е���ֵ �����ۼ���
uint8_t Cpu::PLA()
{
    stackPointer++;
    accumulator = read(0x0100 + stackPointer);
    setFlag(Z, accumulator == 0x00);
    setFlag(N, accumulator & 0x80);
    return 0;
}

// ��ջ�е���ֵ ����״̬�Ĵ���
uint8_t Cpu::PLP()
{
    stackPointer++;
    status = read(0x0100 + stackPointer);
    setFlag(U, 1);
    return 0;
}
// ����
uint8_t Cpu::ROL()
{
    fetch();
    uint16_t tmp = (uint16_t)(fetched << 1) | getFlag(C);
    setFlag(C, tmp & 0xFF00);
    setFlag(Z, (tmp & 0x00FF) == 0x0000);
    setFlag(N, tmp & 0x0080);
    if (instructions[optCode].mode == &Cpu::IMP) accumulator = tmp & 0x00FF;
    else write(addrABS, tmp & 0x00FF);
    return 0;
}
// ����
uint8_t Cpu::ROR()
{
    fetch();
    uint16_t tmp = (uint16_t)(getFlag(C) << 7) | (fetched >> 1);
    setFlag(C, tmp & 0xFF00);
    setFlag(Z, (tmp & 0x00FF) == 0x0000);
    setFlag(N, tmp & 0x0080);
    if (instructions[optCode].mode == &Cpu::IMP) accumulator = tmp & 0x00FF;
    else write(addrABS, tmp & 0x00FF);
    return 0;
}

// �жϷ���
uint8_t Cpu::RTI()
{
    stackPointer++;
    status = read(0x0100 + stackPointer);
    status &= ~B;
    status &= ~U;

    stackPointer++;
    pc = (uint16_t)read(0x0100 + stackPointer);
    stackPointer++;
    pc |= (uint16_t)read(0x0100 + stackPointer) << 8;
    pc++;
    return 0;
}

uint8_t Cpu::RTS()
{
    stackPointer++;
    pc = (uint16_t)read(0x0100 + stackPointer);
    stackPointer++;

    pc |= (uint16_t)read(0x0100 + stackPointer) << 8;
    pc++;
    return 0;
}

// �������
// A = A - M - ( 1 - C )
// C V N Z 
uint8_t Cpu::SBC()
{
    fetch();
    uint16_t value = ((uint16_t)fetched) ^ 0x00FF; // ��ת����8λ

    uint16_t tmp = (uint16_t)accumulator + value + (uint16_t)getFlag(C);

    setFlag(C, tmp & 0xFF00);
    setFlag(Z, (tmp & 0x00FF) == 0);
    setFlag(V, (tmp ^(uint16_t)accumulator) & (tmp ^value) &0x0080);
    setFlag(N, tmp & 0x0080);

    accumulator = tmp & 0x00FF;
    return 1;
}
// ���ý�λ��־λ1
uint8_t Cpu::SEC()
{
    setFlag(C, true);
    return 0;
}
// ����ʮ����ģʽ
uint8_t Cpu::SED()
{
    setFlag(D, true);
    return 0;
}

// 开启中断模式
uint8_t Cpu::SEI()
{
    setFlag(I, true);
    return 0;
}
// �ھ��Ե�ַ���洢�ۼ�����ֵ
uint8_t Cpu::STA()
{
    write(addrABS, accumulator);
    return 0;
}
// �ھ��Ե�ַ���洢X�Ĵ�����ֵ
uint8_t Cpu::STX()
{
    write(addrABS, x);
    return 0;
}
// �ھ��Ե�ַ���洢Y�Ĵ�����ֵ
uint8_t Cpu::STY()
{
    write(addrABS, y);
    return 0;
}
// ���ۼ�����ֵ���� x�Ĵ�����
uint8_t Cpu::TAX()
{
    x = accumulator;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}
// ���ۼ�����ֵ���� y�Ĵ���
uint8_t Cpu::TAY()
{
    y = accumulator;
    setFlag(Z, y == 0x00);
    setFlag(N, y & 0x80);
    return 0;
}
// ��ջָ���ֵ����x�Ĵ���
uint8_t Cpu::TSX()
{
    x = stackPointer;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}
// �� x�Ĵ��� ��ֵ ���� �ۼ���
uint8_t Cpu::TXA()
{
    accumulator = x;
    setFlag(Z, accumulator == 0x00);
    setFlag(N, accumulator & 0x80);
    return 0;
}

// 将X寄存器（Index Register X）的内容复制到堆栈指针（Stack Pointer，SP）中
uint8_t Cpu::TXS()
{
    stackPointer = x;
    return 0;
}
// ��Y�Ĵ�����ֵ�����ۼ���
uint8_t Cpu::TYA()
{
    accumulator = y;
    setFlag(Z, accumulator == 0x00);
    setFlag(N, accumulator & 0x80);
    return 0;
}
