#include<iostream>
#include<chrono>
#include<thread>
#include "stdint.h"
#include"SDL2/SDL.h"
#include "VChip.hpp"
int keyBindings[16]=
{
	SDLK_x,
	SDLK_1,
	SDLK_2,
	SDLK_3,
	SDLK_q,
	SDLK_w,
	SDLK_e,
	SDLK_a,
	SDLK_s,
	SDLK_d,
	SDLK_z,
	SDLK_c,
	SDLK_4,
	SDLK_r,
	SDLK_f,
	SDLK_v,
};

int main(int argc, char **argv)
{
	int windowWidth = 640;
	int windowHeight = 480;
	
	VChip instance = VChip();
	
	SDL_Window* window = NULL;
	
	if(SDL_Init(SDL_INIT_EVERYTHING)<0)
	{
		printf("error initializing SDL:%s\n",SDL_GetError());
		exit(1);
	}
	
	
	window = SDL_CreateWindow("GAME",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,windowWidth,windowHeight,SDL_WINDOW_SHOWN);
	
	if(window == NULL)
	{
		printf("Window could not be created\n");
		exit(2);
	}
	
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,0);
	SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);

    // Create texture that stores frame buffer
    SDL_Texture* sdlTexture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            64, 32);

    // Temporary pixel buffer
    uint32_t pixels[2048];
    
    loadApp:
    	instance.loadApp();
    
	while(true)
	{
		instance.instructionCycle();
		SDL_Event e;
		while(SDL_PollEvent(&e))
			{
				if(e.type == SDL_QUIT)
				{
					exit(0);
				}
				
				if(e.type==SDL_KEYDOWN)
				{
					if(e.key.keysym.sym == SDLK_ESCAPE)
					{
						exit(0);
					}
					if(e.key.keysym.sym == SDLK_F1)
					{
						goto loadApp;
					}
					
					for(int i = 0;i < 16; i++ )
					{
						if(e.key.keysym.sym ==keyBindings[i])
						{
							instance.deviceKeypad[i]=1;
						}
					}
				}
				
				if(e.type==SDL_KEYUP)
				{
					for(int i = 0;i < 16; i++ )
					{
						if(e.key.keysym.sym ==keyBindings[i])
						{
							instance.deviceKeypad[i]=0;
						}
					}
				}
			}
			
	    	SDL_UpdateTexture(sdlTexture, NULL, pixels, 64 * sizeof(uint32_t));
            // Clear screen and render
            	SDL_RenderClear(renderer);
            	SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
            	SDL_RenderPresent(renderer);
			
	}
	std::this_thread::sleep_for(std::chrono::microseconds(1200));
	
	return 0;
}
