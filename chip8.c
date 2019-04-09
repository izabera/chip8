#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t mem[4096];
uint8_t reg[18];
enum {
    V0 = 0x0, V1 = 0x1, V2 = 0x2, V3 = 0x3,
    V4 = 0x4, V5 = 0x5, V6 = 0x6, V7 = 0x7,
    V8 = 0x8, V9 = 0x9, VA = 0xA, VB = 0xB,
    VC = 0xC, VD = 0xD, VE = 0xE, VF = 0xF,
    DT = 16,  ST = 17
};
uint16_t PC, SP, I, stack[16];

static inline uint16_t getinstr(void) {
    uint16_t instr = ((uint16_t) mem[PC] << 8) | mem[PC+1];
    PC += 2;
    return instr;
}

#define OOOF (instr & 0xF)
#define OOFO ((instr & 0xF0) >> 4)
#define OOFF (instr & 0xFF)
#define OFOO ((instr & 0xF00) >> 8)
#define OFFF (instr & 0x0FFF)

#define Case break; case

void interpret(void) {
    while (1) {
        uint16_t instr = getinstr();
        switch (instr) {
            Case 0x00EE: PC = stack[SP--];
            Case 0x1000 ... 0x1FFF: PC = OFFF;
            Case 0x2000 ... 0x2FFF: stack[++SP] = PC; PC = OFFF;
            Case 0x3000 ... 0x3FFF: if (reg[OFOO] == OOFF) PC += 2;
            Case 0x4000 ... 0x4FFF: if (reg[OFOO] != OOFF) PC += 2;
            Case 0x5000 ... 0x5FFF: if (reg[OFOO] == reg[OOFO]) PC += 2;
            Case 0x6000 ... 0x6FFF: reg[OFOO] = OOFF;
            Case 0x7000 ... 0x7FFF: reg[OFOO] += OOFF;
            Case 0x8000 ... 0x8FFF:
                switch (OOOF) {
                    Case 0x00: reg[OFOO] = reg[OOFO];
                    Case 0x01: reg[OFOO] |= reg[OOFO];
                    Case 0x02: reg[OFOO] &= reg[OOFO];
                    Case 0x03: reg[OFOO] ^= reg[OOFO];
                    Case 0x04: reg[VF] = (reg[OFOO] + reg[OOFO]) > 255; reg[OFOO] += reg[OOFO];
                    Case 0x05: reg[VF] = reg[OFOO] >= reg[OOFO]; reg[OFOO] -= reg[OOFO];
                    Case 0x06: reg[VF] = reg[OFOO] & 1; reg[OFOO] >>= 1;
                    Case 0x07: reg[VF] = reg[OOFO] >= reg[OFOO]; reg[OFOO] = reg[OOFO] - reg[OFOO];
                    Case 0x0E: reg[VF] = reg[OFOO] & 128; reg[OFOO] <<= 1;
                }
            Case 0x9000 ... 0x9FFF: if (reg[OFOO] != reg[OOFO]) PC += 2;
            Case 0xA000 ... 0xAFFF: I = OFFF;
            Case 0xB000 ... 0xBFFF: PC = OFFF + reg[V0];
            Case 0xC000 ... 0xCFFF: reg[OFOO] = rand() & OOFF;
            Case 0xD000 ... 0xDFFF: // todo
            Case 0xE000 ... 0xEFFF: // todo
            Case 0xF000 ... 0xFFFF:
                switch (OOFF) {
                    Case 0x07: reg[OFOO] = reg[DT];
                    Case 0x0A: // todo
                    Case 0x15: reg[DT] = reg[OFOO];
                    Case 0x18: reg[ST] = reg[OFOO];
                    Case 0x1E: I += reg[OFOO];
                    Case 0x29: // todo
                    Case 0x33: // todo
                    Case 0x55: memcpy(mem + I, reg, OFOO);
                    Case 0x65: memcpy(reg, mem + I, OFOO);
                }
        }
    }   
}

int main() {
    interpret();
}
