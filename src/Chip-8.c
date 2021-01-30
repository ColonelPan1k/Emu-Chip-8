#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include "Chip-8.h"

unsigned char chip8_fontset[80] =
        {
         0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
         0x20, 0x60, 0x20, 0x20, 0x70, // 1
         0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
         0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
         0x90, 0x90, 0xF0, 0x10, 0x10, // 4
         0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
         0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
         0xF0, 0x10, 0x20, 0x40, 0x40, // 7
         0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
         0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
         0xF0, 0x90, 0xF0, 0x90, 0x90, // A
         0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
         0xF0, 0x80, 0x80, 0x80, 0xF0, // C
         0xE0, 0x90, 0x90, 0x90, 0xE0, // D
         0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
         0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };


Chip8*
chip8_init(){

        // No need to initialize with memset when using calloc
        Chip8* c8 = (Chip8*)calloc(1, sizeof(Chip8));
        if (c8 == NULL){
                printf("Failed to initialize Chip8\n");
                exit(1);
        }

        // Load fontset into memory
        /* Could I use memcpy here to just directly 
         * copy it to the memory instead of looping
         * through the entire block? */
        for (int i = 0; i < 80; ++i){
                c8->memory[i] = chip8_fontset[i];
        }
        
        // Initialize pc = 0x200, sp &= 0, opcode = 0x200
        c8->pc = 0x200;
        c8->sp &= 0;
        c8->opcode = 0x200;

        printf("Chip8 initialized successfully\n");
        return c8;
}

void
chip8_loadRom(Chip8* c8, char* romPath){
        FILE* rom = fopen(romPath, "rb");
        if (rom == NULL){
                printf("Error loading game rom!\n");
                exit(1);
        }
        
        fread(c8->memory + 0x200, 1, MEMSIZE - 0x200, rom);

        printf("File loaded into memory successfully\n");
}

void
chip8_draw(Chip8* c8, unsigned char Vx, unsigned char Vy, unsigned n){
        
}

/* Don't know a better way to move the pc other than
 * just adding c8->pc += 2 at the end of each case block.
 * Not all instructions advance the pc the same amount */

/* Might be able to add a function chip8_advancePc(Chip8* c8, int pc)
 * which could advance the pc a given amount at the end.
 * It would only require setting var = 2 and changing it if needed.*/
void
chip8_doOpcode(Chip8* c8, uint16_t opcode){

        // Break down the opcode so it's easier to work with
        unsigned u = (opcode >> 12) & 0xF; //0xF000
        unsigned x = (opcode >> 8 ) & 0xF; //0x0F00
        unsigned y = (opcode >> 4 ) & 0xF; //0x00F0
        unsigned p = opcode & 0xF;         //0x000F
        unsigned kk = opcode & 0x00FF;     //0x00FF
        unsigned nnn = opcode & 0x0FFF;    //0x0FFF

        printf("OPCODE: 0x%x\n",opcode);
        
        /* Still looking for a better way to handle the opcodes.
         * This seems like the most widely used way to do it though */
        
        switch(u){
        case 0x0:
                {
                        switch(p){
                        case 0x0:
                                printf("Clear screen\n");
                                memset(c8->graphics, 0, sizeof(c8->graphics));
                                c8->pc += 2;
                                break;
                        case 0xE:
                                printf("Return from subroutine\n");
                                c8->pc = c8->stack[(--c8->sp) & 0xF] + 2;
                                break;
                        }
                } break;

        case 0x1:
                printf("goto %x\n", nnn);
                c8->pc = nnn;
                break;
        case 0x2:
                printf("Call subroutine %x\n", nnn);
                c8->stack[(c8->sp) & 0xF] = c8->pc;
                c8->pc = nnn;
                break;
        case 0x3:
                printf("Skip next instruction if V[%i] (%i) == %i\n", x, c8->V[x], kk);
                if (c8->V[x] == kk){
                        c8->pc += 4;
                } else {
                        c8->pc += 2;
                }
                break;
        case 0x4:
                printf("Skip next instruction if V[%i] (%i) != %i\n", x, c8->V[x], kk);
                if (c8->V[x] != kk){
                        c8->pc += 4;
                } else {
                        c8->pc += 2;
                }
                break;
        case 0x5:
                printf("Skip next instruction if V[%i] (%i) == (%i) V[%i\n]", x, c8->V[x], c8->V[y], y);
                if (c8->V[x] == c8->V[y]){
                        c8->pc += 4;
                } else {
                        c8->pc += 2;
                }
                break;
        case 0x6:
                printf("Set V[%i] = %i\n", x, kk);
                c8->V[x] = kk;
                printf("V[%i] (%i) = %i\n", x, c8->V[x], kk);
                c8->pc += 2;
                break;
        case 0x7:
                printf("Add %i to V[%i]\n", kk, x);
                c8->V[x] += kk;
                c8->pc += 2;
                break;
        case 0x8:
                switch(p){
                case 0x0:
                        printf("V[%i] = V[%i]\n", x, y);
                        c8->V[x] = c8->V[y];
                        c8->pc += 2;
                        break;
                case 0x1:
                        printf("V[%i] = V[%i] ^ V[%i]\n", x, x, y);
                        c8->V[x] = c8->V[x] | c8->V[y];
                        c8->pc += 2;
                        break;
                case 0x2:
                        printf("V[%i] = V[%i] & V[%i]\n", x, x, y);
                        c8->V[x] = c8->V[x] & c8->V[y];
                        c8->pc += 2;
                        break;
                case 0x3:
                        printf("V[%i] = V[%i] ^ V[%i]\n", x, x, y);
                        c8->V[x] = c8->V[x] ^ c8->V[y];
                        c8->pc += 2;
                        break;
                case 0x4:
                        printf("V[%i] += V[%i]\n", x, y);

                        if ((int)c8->V[x] + (int)c8->V[y] < 256)
                                c8->V[0xF] &= 0;
                        else
                                c8->V[0xF] = 1;
                        
                        c8->V[x] += c8->V[y];
                        c8->pc += 2;
                        break;
                case 0x5:
                        printf("V[%i] -= V[%i]\n", x, y);
                        c8->V[x] -= c8->V[y];
                        c8->pc += 2;
                        break;
                case 0x6:
                        printf("V[%i] >>= V[%i]\n", x, y);
                        
                        c8->pc += 2;
                        break;
                case 0x7:
                        printf("V[%i] = V[%i] - V[%i]\n", x, y, x);
                        c8->pc += 2;
                        break;
                case 0xE:
                        printf("V[%i] <<= 1\n", x);
                        c8->pc += 2;
                        break;
                } break;
        case 0x9:
                printf("Skip next instruction if V[%i] (%i) != V[%i] (%i)\n", x, c8->V[x], y, c8->V[y]);
                if (c8->V[x] != c8->V[y]){
                        printf("Skipped instruction\n");
                        c8->pc += 4;
                } else {
                        printf("Did not skip instruction\n");
                        c8->pc += 2;
                }
                break;
        case 0xA:
                printf("I = %i\n", nnn);
                c8->I = nnn;
                c8->pc += 2;
                break;
        case 0xB:
                printf("PC = V[0] + %i\n", nnn);
                c8->pc = c8->V[0] + nnn;
                c8->pc += 2;
                break;
        case 0xC:
                printf("V[%i] = rand() & %i\n", x, nnn);
                c8->pc += 2;
                break;
        case 0xD:
                printf("Draw sprite at (%i, %i) with height %i\n", x, y, p);
                chip8_draw(c8, V[x], V[y], n);
                c8->pc += 2;
                c8->drawFlag = true;
                break;
        case 0xE:
                printf("Handle keypress\n");
                c8->pc += 2;
                break;
        case 0xF:
                switch (kk){
                case 0x07:
                        printf("V[%i] = get_delay()\n", x);
                        c8->pc += 2;
                        break;
                case 0x0A:
                        printf("V[%i] = get_key()\n", x);
                        c8->pc += 2;
                        break;
                case 0x15:
                        printf("Delay_timer(V[%i])\n", x);
                        c8->pc += 2;
                        break;
                case 0x18:
                        printf("Sound_timer(V[%i])\n", x);
                        c8->pc += 2;
                        break;
                case 0x1E:
                        printf("I += V[%i]\n", x);
                        c8->pc += 2;
                        break;
                case 0x20:
                        printf("Load sprite V[%i]\n", x);
                        c8->pc += 2;
                        break;
                        
                case 0x29:
                        printf("I = sprite_addr[V[%i]]\n", x);
                        c8->pc += 2;
                        break;
                case 0x33:
                        printf("BCD\n");
                        c8->pc += 2;
                        break;
                case 0x55:
                        printf("reg_dump\n");
                        c8->pc += 2;
                        break;
                case 0x65:
                        printf("reg_load\n");
                        c8->pc += 2;
                        break;
                default:
                        printf("--- UNKNOWN OPCODE: 0x%x ---\n", opcode);
                        c8->pc += 2;
                } break;

        default:
                printf("--- UNKNOWN OPCODE : 0x%x-----\n", opcode);
                c8->pc += 2;
                break;
        }
}

/* Just an easier way to dump the opcodes to the console */
void
chip8_dumpRom(Chip8* c8, uint16_t opcode){
        printf("===== OPCODE: 0x%x ===== \n", opcode);
        c8->pc += 2;
}

void
chip8_emulateCycle(Chip8* c8){
        // Fetch an opcode from memory
        uint16_t opcode = c8->memory[c8->pc] << 8 | c8->memory[c8->pc + 1];

 
        // Decode and execute opcode
        chip8_doOpcode(c8, opcode);

}

void chip8_processInput(SDL_Event* event){
        
}
        
