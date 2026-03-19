#include <stdio.h>
#include <stdint.h>

uint8_t mem[256] = {0};
uint8_t reg[4] = {0};
uint8_t pc = 0, zf = 0, running = 1;
int ciclo = 0;

void fetch(uint8_t *op, uint8_t *a, uint8_t *b) {
    *op = mem[pc]; *a = mem[pc+1]; *b = mem[pc+2];
    pc += 3;
}

void decode_execute(uint8_t op, uint8_t a, uint8_t b) {
    switch (op) {
        case 0x01: reg[a] = mem[b]; break;
        case 0x02: mem[b] = reg[a]; break;
        case 0x03: reg[a] = reg[a] + reg[b]; break;
        case 0x04: reg[a] = reg[a] - reg[b]; break;
        case 0x05: reg[a] = b; break;
        case 0x06: zf = (reg[a] == reg[b]) ? 1 : 0; break;
        case 0x07: pc = a; break;
        case 0x08: if (zf) pc = a; break;
        case 0x09: if (!zf) pc = a; break;
        case 0x0A: running = 0; break;
    }
}

void trace(uint8_t op, uint8_t a, uint8_t b) {
    const char *nomes[] = {"","LOAD","STORE","ADD",
        "SUB","MOV","CMP","JMP","JZ","JNZ","HALT"};
        printf("Ciclo %d: %-5s %d,%d | R0=%3d R1=%3d"
            " R2=%3d R3=%3d | PC=%3d ZF=%d\n",
            ciclo, nomes[op], a, b,
            reg[0], reg[1], reg[2], reg[3], pc, zf);
        }

void cifraDeCesar(){
    mem[0x08] = 3; 

    mem[0x10] = 65;
    mem[0x11] = 66;
    mem[0x12] = 67;
    mem[0x13] = 68;
    mem[0x14] = 69;
    mem[0x15] = 70;
    mem[0x16] = 71;
    mem[0x17] = 72;

    pc = 0x30;
    uint8_t i = 0x30;

    uint8_t src, dst;
    for (int k = 0; k < 8; k++) {
        src = 0x10 + k;
        dst = 0x20 + k;

        mem[i++] = 0x01; mem[i++] = 0x00; mem[i++] = src;  // LOAD R0, src
        mem[i++] = 0x01; mem[i++] = 0x01; mem[i++] = 0x08; // LOAD R1, 0x08
        mem[i++] = 0x03; mem[i++] = 0x00; mem[i++] = 0x01; // ADD R0, R1
        mem[i++] = 0x02; mem[i++] = 0x00; mem[i++] = dst;  // STORE R0, dst
    }

    mem[i++] = 0x0A; mem[i++] = 0x00; mem[i++] = 0x00;     // HALT

    // mem[0x00] = 0x05; mem[0x01] = 0x00; mem[0x02] = 0x0A; /* MOV R0, 10 */
    // mem[0x03] = 0x05; mem[0x04] = 0x01; mem[0x05] = 0x01; /* MOV R1, 1 */
    // mem[0x06] = 0x03; mem[0x07] = 0x00; mem[0x08] = 0x01; /* ADD R0, R1 */
    // mem[0x06] = 0x03; mem[0x07] = 0x00; mem[0x08] = 0x02; /* STORAGE R0, R1 */
    // mem[0x09] = 0x0A; mem[0x0A] = 0x00; mem[0x0B] = 0x00; /* HALT */

}
        
int main() {
    cifraDeCesar();
    while (running && pc < 256) {
        uint8_t op, a, b;
        ciclo++;
        fetch(&op, &a, &b);
        decode_execute(op, a, b);
        trace(op, a, b);
    }
    printf("\nResultado em 0x20-0x27:\n");
    for (int i = 0x20; i <= 0x27; i++) {
        printf("mem[0x%02X] = %d\n", i, mem[i]);
    }
    return 0;
}
