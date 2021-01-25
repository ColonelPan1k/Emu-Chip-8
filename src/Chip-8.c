#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>


Chip8*
chip8_init(){
        // Load fontset into memory
        // initialize stack, registers and graphics with memset
        // Initialize pc = 0x200, sp &= 0, opcode = 0x200
        
}

void
chip8_loadRom(chip*c8, char* romPath){
        FILE* rom = fopen(romPath, "rb");
        if (rom == NULL){
                printf("Error loading game rom!");
                exit(1);
        }

        fread(c8->memory[i], MEMSIZE - 0x200, rom);
}

void
chip8_draw(Chip8* c8){
        
}

void
chip8_emulateCycle(Chip8* c8){
        
}

void chip8_processInput(SDL_Event* event){
        
}
        
