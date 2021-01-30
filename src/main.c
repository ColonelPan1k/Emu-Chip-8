/* gcc -o X.c Y.c Z.c -lSDL to compile */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <SDL/SDL.h>
#include "Chip-8.h"

int main(int argc, char** argv){

        Chip8* C8 = chip8_init();
        chip8_loadRom(C8, argv[1]);

        for(int i = 0; i < 64; ++i){
                chip8_emulateCycle(C8);
        }
        
        
        return 0;
}
