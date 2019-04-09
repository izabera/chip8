#include <stdint.h>

uint8_t mem[4096];
uint8_t reg[18];
enum {
    V0 = 0x0, V1 = 0x1, V2 = 0x2, V3 = 0x3,
    V4 = 0x4, V5 = 0x5, V6 = 0x6, V7 = 0x7,
    V8 = 0x8, V9 = 0x9, VA = 0xA, VB = 0xB,
    VC = 0xC, VD = 0xD, VE = 0xE, VF = 0xF,
    DT = 16,  ST = 17
};
uint16_t PC, SP, stack[16];

static inline uint16_t getinstr(void) {
    uint16_t instr = ((uint16_t) mem[PC] << 8) | mem[PC+1];
    PC += 2;
    return instr;
}

void interpret(void) {
    while (1) {
        uint16_t instr = getinstr();
        switch (instr) {
            case 0x00EE:
                PC = stack[SP--];
                break;
            case 0x1000 ... 0x1FFF:
                PC = instr & 0xFFF;
                break;
            case 0x2000 ... 0x2FFF:
                stack[++SP] = PC;
                PC = instr & 0xFFF;
                break;
            case 0x3000 ... 0x3FFF:
                if (reg[(instr & 0x0F00) >> 8] == instr & 0xFF)
                    PC += 2;
                break;
            case 0x4000 ... 0x4FFF:
                if (reg[(instr & 0x0F00) >> 8] != instr & 0xFF)
                    PC += 2;
                break;
            case 0x5000 ... 0x5FFF:
                if (reg[(instr & 0x0F00) >> 8] == reg[(instr & 0x00F0)>>4])
                    PC += 2;
                break;
            case 0x6000 ... 0x6FFF:
                reg[(instr & 0x0F00) >> 8] == instr & 0xFF;
                break;
            case 0x7000 ... 0x7FFF:
                reg[(instr & 0x0F00) >> 8] += instr & 0xFF;
                break;
            case 0x8000 ... 0x8FFF:
                // todo
                break;
            case 0x9000 ... 0x9FFF:
                // 9xxx is the opposite of 5xxx BECAUSE THIS MAKES SENSE
                if (reg[(instr & 0x0F00) >> 8] != reg[(instr & 0x00F0)>>4])
                    PC += 2;
                break;
            case 0xA000 ... 0xAFFF:
                // what is I?
                break;
            case 0xB000 ... 0xBFFF:
                PC = (instr & 0x0FFF) + reg[V0];
                break;
            case 0xC000 ... 0xCFFF:
                reg[(instr & 0x0F00) >> 8] = rand() & (instr & 0xFF);
                break;
            case 0xD000 ... 0xDFFF:
                // todo
                break;
        }
    }   
}

int main() {
    interpret();
}
