#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "chip8.h"
#include "memory.h"
#include "cpu.h"
#include "display.h"

using namespace std;

void press_enter_to_continue() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

Chip8::Chip8()
    : width(64),
      height(32),
      scale(30),
      memory(0x1000),
      cpu(width, height, memory),
      display(width, height, scale)
{
    SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "Chip8::Chip8() called");
}

Chip8::~Chip8() {
    delete[] memory.elem;
    delete[] cpu.stack;
    delete[] cpu.V;
}

void Chip8::read_program(string filename) {
    ifstream file(filename, ios::binary);
    if (file) {
        int i = 0x200;
        uint8_t data;
        while (file.read(reinterpret_cast<char*>(&data), sizeof(data))) {
            memory[i] = data;
            i++;
        }
        file.close();
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file %s", filename.c_str());
    }
}

int Chip8::cpu_step() {
    int opcode = memory.read_16(cpu.PC);
    int rc = cpu.execute(opcode);
    cpu.PC += 2;
    return rc;
}

void Chip8::init() {
    display.init();
}

void Chip8::loop(bool& running) {
    press_enter_to_continue();
    if (running) {
        print_cpu();
        running = cpu_step();
    }
    display.loop(running, cpu.frame_buffer);
}

void Chip8::shutdown() {
    display.shutdown();
}

void Chip8::print_cpu() {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "-------------------------------");
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "CPU State");
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "-------------------------------");
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "PC: %03X  Opcode: %04X", cpu.PC, memory.read_16(cpu.PC));
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "I: %03X", cpu.I);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "delay: %02X", cpu.delay);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "sound: %02X", cpu.sound);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "SP: %02X", cpu.SP);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, " ");
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Stack:");
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%03X %03X %03X %03X %03X %03X %03X %03X", 
        cpu.stack[0x0],
        cpu.stack[0x1],
        cpu.stack[0x2],
        cpu.stack[0x3],
        cpu.stack[0x4],
        cpu.stack[0x5],
        cpu.stack[0x6],
        cpu.stack[0x7]

    );
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%03X %03X %03X %03X %03X %03X %03X %03X", 
        cpu.stack[0x8],
        cpu.stack[0x9],
        cpu.stack[0xA],
        cpu.stack[0xB],
        cpu.stack[0xC],
        cpu.stack[0xD],
        cpu.stack[0xE],
        cpu.stack[0xF]

    );
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, " ");
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "GP Registers:");
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "V0: %02X V1: %02X", cpu.V[0x0], cpu.V[0x1]);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "V2: %02X V3: %02X", cpu.V[0x2], cpu.V[0x3]);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "V4: %02X V5: %02X", cpu.V[0x4], cpu.V[0x5]);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "V6: %02X V7: %02X", cpu.V[0x6], cpu.V[0x7]);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "V8: %02X V9: %02X", cpu.V[0x8], cpu.V[0x9]);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "VA: %02X VB: %02X", cpu.V[0xA], cpu.V[0xB]);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "VC: %02X VD: %02X", cpu.V[0xC], cpu.V[0xD]);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "VE: %02X VF: %02X", cpu.V[0xE], cpu.V[0xF]);
}
