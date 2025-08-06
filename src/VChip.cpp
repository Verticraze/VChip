#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<random>
#include "time.h"

#include "VChip.hpp"

//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

unsigned char fontSet[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

VChip::VChip(){}
VChip::~VChip(){}

void VChip::Init()
{
	programCounter = 0x200;
	currentOpCode = 0;
	indexRegister = 0;
	stackPointer = 0;
	
	for(int i = 0; i < 2048;++i)
	{
		graphicsBuffer[i] = 0;
	}
	
	for(int i = 0 ;i < 4096 ; i++)
	{
		stack[i] = 0;
		deviceKeypad[i] = 0;
		deviceRegisters[i] = 0;
	}
	for(int i = 0;i < 80;++i)
	{
		deviceMemory[i] = fontSet[i];
	}
	
	delayTimer = 0;
	soundTimer = 0;

	srand(time(NULL));
}

bool VChip::loadApp()
{
	std::cout<<"LoadApp";
}

void VChip::instructionCycle()
{
	currentOpCode = deviceMemory[programCounter] << 8 | deviceMemory[programCounter+1];
	
	switch(currentOpCode & 0xF000)
	{
		case 0x0000:
			switch(currentOpCode&0x000F)
			{
				case 0x0000:
					for(int i = 0; i <2048;i++)
					{
						graphicsBuffer[i] = 0;
					}
					drawFlag = true;
					programCounter += 2 ;
					break;
				case 0x000E:
					--stackPointer;
					programCounter = stack[stackPointer];
					programCounter+=2;
					break;
					
				default:
					printf("Unknown op code : %.4X\n",currentOpCode);
					exit(3);
			}
		case 0x1000:
			programCounter = currentOpCode & 0x0FFF;
			break;
		case 0x2000:
			stack[stackPointer]=programCounter;
			++stackPointer;
			programCounter = currentOpCode & 0x0FFF;
			break;
		case 0x3000:
			if(deviceRegisters[(currentOpCode & 0x0F00) >> 8] == (currentOpCode & 0x00FF))
			{
				programCounter+=4;
			}
			else
			{
				programCounter+=2;
			}
			break;
		case 0x4000:
			if(deviceRegisters[(currentOpCode&0x0F00)>>8]!=(currentOpCode & 0x00FF))
			{
				programCounter+=4;
			}
			else
			{
				programCounter+=2;
			}
			break;
		case 0x5000:
			if(deviceRegisters[(currentOpCode&0x0F00) >>8] == deviceRegisters[(currentOpCode & 0x00F0)>>4])
			{
				programCounter+=4;
			}
			else
			{
				programCounter +=2;
			}
		case 0x6000:
			deviceRegisters[(currentOpCode & 0x0F00)>>8] = currentOpCode & 0x00FF;
			programCounter+=2;
			break;
		case 0x7000:
			deviceRegisters[currentOpCode & 0x0F00 >> 8] += currentOpCode & 0x00FF;
			programCounter+=2;
			break;
		case 0x8000:
				switch(currentOpCode & 0x000F):
				{
					case 0x0000:
						{		
							deviceRegisters[(currentOpCode & 0x0F00) >> 8] = deviceRegisters[(currentOpCode & 0x00F0) >> 4];
							programCounter += 2;
							break;
						}
					case 0x0001:
						{
							deviceRegisters[currentOpCode & 0x0F00) >> 8] |= deviceRegisters[(currentOpCode & 0x00F0) >> 4];
							programCounter += 2;
							break;
						}
					case 0x0002:
						{
							deviceRegisters[currentOpCode & 0x0F00) >> 8] &= deviceRegisters[(currentOpCode & 0x00F0) >> 4];
							programCounter += 2;
							break;
						}
					case 0x0003:
						{	
							deviceRegisters[currentOpCode & 0x0F00) >> 8] ^= deviceRegisters[(currentOpCode & 0x00F0) >> 4];
							programCounter += 2;
							break;
						}
					case 0x0004:
						{
							deviceRegisters[currentOpCode & 0x0F00) >> 8] += deviceRegisters[(currentOpCode & 0x00F0) >> 4];
							if(deviceRegisters [(currentOpCode & 0x00F0) >> 4] > [0xFF - deviceRegisters[(currentOpCode & 0x0F00) >> 8])
								{
									deviceRegisters[0xF] = 1;
								}
							else
								{
									deviceRegisters[0xF] = 0;
								}
							programCounter += 2;
							break;
						}	
					case 0x0005:
						{
							if(deviceRegisters[(currentOpCode & 0x00F0) >> 4] > [0xFF - deviceRegisters[(currentOpCode & 0x0F00) >> 8])
								{
									deviceRegisters[0xF] = 0;
								}
							else
								{
									deviceRegisters[0xF] = 1;
								}
							deviceRegisters[currentOpCode & 0x0F00) >> 8] -= deviceRegisters[(currentOpCode & 0x00F0) >> 4];
							programCounter+=2;
							break;
						}
					case 0x0006:
						{
							deviceRegisters[0xF] = deviceRegisters[(currentOpCode & 0x0F00) >> 8] & 0x1;
							deviceRegisters[(currentOpCode & 0x0F00) >> 8] >> = 1;
							programCounter += 2;
							break;
						}
					case 0x0007:
						{
							if(deviceRegisters[currentOpCode & 0x0F00) >> 8] > deviceRegisters[(currentOpCode & 0x00F0) >> 4])
								{
									deviceRegisters[0xF] = 0;
								}
							else
								{
									deviceRegisters[0xF] = 1;
								}							
							deviceRegisters[(currentOpCode & 0x0F00) >> 8] = deviceRegisters[(currentOpCode & 0x00F0 >> 4)] - deviceRegisters[(currentOpCode & 0x0F00) >> 8];
							programCounter += 2;
							break;
						}	
					case 0x000E:
						{
							deviceRegisters[0xF] = deviceRegisters[(currentOpCode & 0x0F00) >> 8] >> 7;
							deviceRegisters[(currentOpCode & 0x0F00) >> 8] << = 1;
							programCounter += 2;
							break;
							
						}
						
						default: printf("\nUnknown OpCode:%.4X\n",currentOpCode);
							exit(3);
							
					}
				break;
				
				case 0x9000:
					if(deviceRegisters[(currentOpCode & 0x0F00) >> 8] != deviceRegisters[(currentOpCode & 0x00F0) >> 4])
					{
						programCounter += 4;
					}
					else
					{
						programCounter += 2;
					}
				case 0xA000:
					{
						indexRegister = currentOpCode & 0x0FFF;
						programCounter += 2;
						break;
					}
				case  0xB000:
					{
						programCounter = (currentOpCode & 0x0FFF) + deviceRegisters[0];
						break;
					}
				case 0xC000:
					{
						deviceRegisters[(currentOpCode & 0x0F00)>> 8] = (rand() % (0xFF + 1))(currentOpCode & 0x00FF);
						programCounter += 2;
					}
				case 0xD000:
					{
						unsigned short x = deviceRegisters[(currentOpCode & 0x0F00) >> 8];
						unsigned short y = deviceRegisters[(currentOpCode & 0x00F0) >> 4];
						unsigned short height = currentOpCode & 0x000F;
						unsigned short pixel;
						
						for(int lineY = 0; lineY < height; lineY++)
						{
							pixel = deviceMemory[indexRegister +lineY];
							
							for(int lineX = 0; lineX < 8; lineX++)
							{
								if((pixel & (0x80 >> lineX)) != 0)
								{
									if(graphicsBuffer[(x + lineX + ( y + lineY ) * 64)] == 1)
									{
										deviceRegisters[0xF] = 1;
									}
									graphicsBuffer[x+lineX +((y+lineY)*64)] ^= 1;
								}
							}
						}
						drawFlag = true;
						programCounter += 2;
					}
					break;
				case 0xE000:
					{
						switch(currenOpCode & 0x00FF):
						{
							case 0x009E:
								{
									if(deviceKeypad[deviceRegisters[currentOpcode & 0x0F00 >> 8 ]] != 0)
									{
										programCounter += 4;
									}
									else
									{
										programCounter += 2;
									}
									break;
								}
								
							case 0x00A1:
								{
									if(deviceKeypad[deviceRegisters[currentOpcode & 0x0F00 >> 8 ]] != 0)
									{
										programCounter += 4;
									}
									else
									{
										programCounter += 2;
									}
									break;
								}
							default: printf("\n Unknown Opcode :%.4X \n",currentOpCode);
								exit(3);
						}
						break;
					}
				case 0xF000:
					{
						switch(currentOpCode & 0x00FF)
						{
							case 0x0007:
								{
									deviceRegisters[(currentOpCode & 0x0F00) >> 8] = delayTimer;
									programCounter += 2;
									break;
								}
							case 0x000A:
								{
									bool keyPressed = false;
									for(int i = 0 ; i < 16;i++)
									{
										if(deviceKeypad[i] != 0)
										{
											deviceRegisters[currentOpCode & 0x0F00) >> 8] = i;
											keyPressed = true;
										}
									}
									if(!keyPressed)
									{
										return;
									}
									programCounter += 2;
									break;
								}
							case 0x0015:
								{
									delayTimer = deviceRegisters[(currentOpCode & 0x0F00) >> 8 ];
									programCounter += 2 ;
									break;
								}
							case 0x0018:
								{
									soundTimer= deviceRegisters[(currentOpCode & 0x0F00) >> 8 ];
									programCounter += 2;
									break;
								}
							case 0x001E:
								{
									if(indexRegister + deviceRegisters[(currenOpCode & 0x0F00) >> 8])
									{
										deviceRegisters[0xF] = 1;
									}
									else
									{
										deviceRegisters[0xF] = 0;
									}
									indexRegisters += deviceRegisters[(currentOpCode & 0x0F00) >> 8];
									programCounter+=2;
									break;
								}
							case 0x0029:
							case 0x0033:
							case 0x0055:
							case 0x0065:							
							default:
						}
						break;
					}	
					default:printf("\n Unknown Opcode :%.4X \n",currentOpCode);
						exit(3);
		}
		
		if(delayTimer > 0)
		{
			delayTimer--;
		}								
		
		if(soundTimer > 0)
		{
			soundTimer--;
		}			
}
