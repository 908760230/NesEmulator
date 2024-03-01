#ifndef CPU_H
#define CPU_H

#include<cstdint>
#include<string>
#include<vector>
#include<fstream>
#include<map>

#ifdef LOGMODE
#include<stdio.h>
#endif


class Bus;


    class Cpu
    {
    public:
        Cpu();
        ~Cpu();

        enum FLAGS
        {
            C = (1 << 0), // Carry Bit(进位标志)
            Z = (1 << 1), // zero
            I = (1 << 2), // interruption
            D = (1 << 3), // Decimal Model
            B = (1 << 4), // Break
            U = (1 << 5), // ��ʱ��ʹ��
            V = (1 << 6), // 溢出标志
            N = (1 << 7), // 负数标志
        };
        uint8_t accumulator = 0x00;     // 累加器
        uint8_t x = 0x00;               // X 寄存器
        uint8_t y = 0x00;               // Y 寄存器
        uint8_t stackPointer = 0x00;    // 栈指针
        uint16_t pc = 0x0000;           // 程序计数器
        uint8_t status = 0x00;          // 状态

        void connectBus(Bus* ptr) {
            busPtr = ptr;
        };
        std::map<uint16_t, std::string> disassemble(uint16_t start, uint16_t end);

        // (6502型号)Addressing Modes
        uint8_t IMP();
        uint8_t IMM();
        uint8_t ZP0();
        uint8_t ZPX();
        uint8_t ZPY();
        uint8_t REL();
        uint8_t ABS();
        uint8_t ABX();
        uint8_t ABY();
        uint8_t IND();
        uint8_t IZX();
        uint8_t IZY();

        // Opcodes
        uint8_t ADC();
        uint8_t AND();
        uint8_t ASL();
        uint8_t BCC();
        uint8_t BCS();
        uint8_t BEQ();
        uint8_t BIT();
        uint8_t BMI();
        uint8_t BNE();
        uint8_t BPL();
        uint8_t BRK();
        uint8_t BVC();
        uint8_t BVS();
        uint8_t CLC();
        uint8_t CLD();
        uint8_t CLI();
        uint8_t CLV();
        uint8_t CMP();
        uint8_t CPX();
        uint8_t CPY();
        uint8_t DEC();
        uint8_t DEX();
        uint8_t DEY();
        uint8_t EOR();
        uint8_t INC();
        uint8_t INX();
        uint8_t INY();
        uint8_t JMP();
        uint8_t JSR();
        uint8_t LDA();
        uint8_t LDY();
        uint8_t LDX(); 
        uint8_t LSR();
        uint8_t NOP();
        uint8_t ORA();
        uint8_t PHA();
        uint8_t PHP();
        uint8_t PLA();
        uint8_t PLP();
        uint8_t ROL();
        uint8_t ROR();
        uint8_t RTI();
        uint8_t RTS();
        uint8_t SBC();
        uint8_t SEC();
        uint8_t SED();
        uint8_t SEI();
        uint8_t STA();
        uint8_t STX();
        uint8_t STY();
        uint8_t TAX();
        uint8_t TAY();
        uint8_t TSX();
        uint8_t TXA();
        uint8_t TXS();
        uint8_t TYA();

        uint8_t XXX(); // ��ʾ�Ƿ��Ĳ�����

        void clock();  // 
        void reset();  // ����CPU״̬
        void irq();    // ��ʾ�ж����󣬿ɱ�����
        void nmi();    //��ʾ�������ж����󣬲��ɱ�����
        bool complete();

        uint8_t fetch();  // ������� IMPģʽ,��Ӿ��Ե�ַ�л�ȡ����
        uint8_t fetched = 0x00; //��ʾALU������ֵ

        uint16_t addrABS = 0x0000;  // ���������ľ��Ե�ַ
        uint16_t addrREL = 0x0000;
        uint8_t optCode = 0x00;
        uint8_t cycles = 0;         //ʱ������
        uint32_t clockCount = 0;    // ʱ��������ȫ���ۼ�

    private:
        Bus* busPtr;

        void write(uint16_t addr, uint8_t data);
        uint8_t read(uint16_t addr);

        uint8_t getFlag(FLAGS flag);
        void setFlag(FLAGS flag, bool value);

        struct INSTRUCTION
        {
            std::string name;
            uint8_t(Cpu::* operate)() = nullptr; // ע������ ��Ա�����ͺ���ָ��
            uint8_t(Cpu::* mode)() = nullptr;
            uint8_t cycles = 0;
        };

        std::vector<INSTRUCTION> instructions;

    private:
        FILE* logFile = nullptr;
    };

#endif