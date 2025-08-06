#ifndef VChip_H
#define VChip_H

#include<stdint.h>

class VChip
{
	private:
		uint16_t stack[16]; //stack
		uint16_t stackPointer;//sp
		uint16_t programCounter;//pc
		uint16_t currentOpCode;//opcode
		uint16_t indexRegister;//I
		uint8_t deviceMemory[4096];//memory
		uint8_t deviceRegisters[16];//V
		uint8_t delayTimer;//delay_timer
		uint8_t soundTimer;//sound_timer
		
		void Init();
		
	public:
	
		uint8_t graphicsBuffer[64*32];//gfx
		uint8_t deviceKeypad[16];//key
		bool drawFlag;//drawFlag
		
		VChip();
		~VChip();
		
		void instructionCycle();//emulate_cycle
		bool loadApp();//load and file_path
						
};
#endif
