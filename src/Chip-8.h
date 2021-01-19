#ifndef CHIP_8_C
#define CHIP_8_C

typedef struct chip8_t{
        unsigned char memory[4096];
}Chip8;

void C8_loadFile(char* fileName);
Chip8* createChip8();
void C8_emulateCycle(Chip8* cpu);

#endif
