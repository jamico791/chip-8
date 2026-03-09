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
      scale(20),
      memory(),
      cpu(memory),
      display(width, height, scale)
{
    should_step = false;
    SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "Chip8::Chip8() called");
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
    cpu.fetch();
    int rc = cpu.execute();
    return rc;
}

void Chip8::init() {
    load_sprites();
    display.init();
    SDL_SetRenderDrawColor(display.renderer, 0x61, 0x86, 0xA9, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(display.renderer);

    // Set Pixel Color
    SDL_SetRenderDrawColor(display.renderer, 0x21, 0x29, 0x46, SDL_ALPHA_OPAQUE);
    float x = 63;
    float y = 31;

    display.render_frame(cpu.frame_buffer);
}

void Chip8::loop(bool& running, bool should_execute) {
    SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "Enter Chip8::loop");
    if (running) {
        int key_pressed = -1;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    running = false;
                    break;
                default:
                    key_pressed = event.key.scancode;
                    break;
                }
            }

            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        if (running && should_execute) {
            print_cpu();
            running = cpu_step();
            display.loop(running, cpu.frame_buffer);
        }
    }
    if (should_step) {
        bool enter_pressed = false;
        while (!enter_pressed && running) {
            SDL_Event step_event;
            while (SDL_PollEvent(&step_event)) {
                if (step_event.type == SDL_EVENT_KEY_DOWN) {
                    if (step_event.key.scancode == SDL_SCANCODE_RETURN)
                        enter_pressed = true;
                    else if (step_event.key.scancode == SDL_SCANCODE_ESCAPE)
                        running = false;
                }
            }
        }
    }
}

void Chip8::shutdown() {
    display.shutdown();
}

void Chip8::step() {
    should_step = !should_step;
}

// Write sprites into memory starting at index 0
void Chip8::load_sprites() {
    vector<int> vec {
        0xF0, 0x90, 0x90, 0x90, 0xF0,    //0
        0x20, 0x60, 0x20, 0x20, 0x70,    //1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,    //2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,    //3
        0x90, 0x90, 0xF0, 0x10, 0x10,    //4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,    //5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,    //6
        0xF0, 0x10, 0x20, 0x40, 0x40,    //7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,    //8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,    //9
        0xF0, 0x90, 0xF0, 0x90, 0x90,    //A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,    //B
        0xF0, 0x80, 0x80, 0x80, 0xF0,    //C
        0xE0, 0x90, 0x90, 0x90, 0xE0,    //D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,    //E
        0xF0, 0x80, 0xF0, 0x80, 0x80     //F
    };
    memory.write_n(vec, 0);
}

void Chip8::print_cpu() {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "-------------------------------");
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "CPU State");
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "-------------------------------");
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "PC: %03X  Opcode: %04X", cpu.PC, cpu.instruction);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "I: %03X", cpu.I);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "DT: %02X", cpu.DT);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "ST: %02X", cpu.ST);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "SP: %02X", cpu.SP);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, " ");
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Stack:");
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%03X %03X %03X %03X %03X %03X %03X %03X", 
        cpu.sub_stack[0x0],
        cpu.sub_stack[0x1],
        cpu.sub_stack[0x2],
        cpu.sub_stack[0x3],
        cpu.sub_stack[0x4],
        cpu.sub_stack[0x5],
        cpu.sub_stack[0x6],
        cpu.sub_stack[0x7]

    );
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%03X %03X %03X %03X %03X %03X %03X %03X", 
        cpu.sub_stack[0x8],
        cpu.sub_stack[0x9],
        cpu.sub_stack[0xA],
        cpu.sub_stack[0xB],
        cpu.sub_stack[0xC],
        cpu.sub_stack[0xD],
        cpu.sub_stack[0xE],
        cpu.sub_stack[0xF]

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
