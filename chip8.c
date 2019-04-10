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
#define FOOO (instr & 0xF000 >> 12)

#define Case break; case

void interpret(void) {
    while (1) {
        uint16_t instr = getinstr();
        switch (FOOO) {
            Case 0x0: PC = stack[SP--];
            Case 0x1: PC = OFFF;
            Case 0x2: stack[++SP] = PC; PC = OFFF;
            Case 0x3: if (reg[OFOO] == OOFF) PC += 2;
            Case 0x4: if (reg[OFOO] != OOFF) PC += 2;
            Case 0x5: if (reg[OFOO] == reg[OOFO]) PC += 2;
            Case 0x6: reg[OFOO] = OOFF;
            Case 0x7: reg[OFOO] += OOFF;
            Case 0x8:
                switch (OOOF) {
                    Case 0x0: reg[OFOO] = reg[OOFO];
                    Case 0x1: reg[OFOO] |= reg[OOFO];
                    Case 0x2: reg[OFOO] &= reg[OOFO];
                    Case 0x3: reg[OFOO] ^= reg[OOFO];
                    Case 0x4: reg[VF] = reg[OFOO] + reg[OOFO] > 255; reg[OFOO] += reg[OOFO];
                    Case 0x5: reg[VF] = reg[OFOO] >= reg[OOFO]; reg[OFOO] -= reg[OOFO];
                    Case 0x6: reg[VF] = reg[OFOO] & 1; reg[OFOO] >>= 1;
                    Case 0x7: reg[VF] = reg[OOFO] >= reg[OFOO]; reg[OFOO] = reg[OOFO] - reg[OFOO];
                    Case 0xE: reg[VF] = reg[OFOO] & 128; reg[OFOO] <<= 1;
                }
            Case 0x9: if (reg[OFOO] != reg[OOFO]) PC += 2;
            Case 0xA: I = OFFF;
            Case 0xB: PC = OFFF + reg[V0];
            Case 0xC: reg[OFOO] = rand() & OOFF;
            Case 0xD: // todo
            Case 0xE: // todo
            Case 0xF:
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
