#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;

int gameRunning = 1;


int main (int argc, char* args[])
{

	SDL_Window* window = NULL;

	SDL_Surface* gScreen = NULL;

	SDL_Surface* gBackground = NULL;

	SDL_Surface* gPlayer = NULL;

	//jogador

	float velx= 3.5;

	int player_fullwidth = 480;

	int player_frame_width = 60;
    int player_frame_height = 90;

    SDL_Rect player_frame, player_position;

    player_frame.x = 0;
    player_frame.y = 0;
    player_frame.w = player_frame_width;
    player_frame.h = player_frame_height;

    player_position.x = SCREEN_WIDTH/2;
    player_position.y = SCREEN_HEIGHT/2;
    player_position.w = player_frame_width;
    player_position.h = player_frame_height;

    gPlayer = IMG_Load("personagem.png");
    gBackground = IMG_Load("fundo.jpg");


	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("Jogo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	gScreen = SDL_GetWindowSurface( window );

    SDL_Event event;
                              
            while(gameRunning)
            {
                if(SDL_PollEvent(&event))
                {
                    
                    if(event.type == SDL_QUIT)
                    {
                        gameRunning = 0;
                    }

                    if(event.type == SDL_KEYDOWN)
                    {
                           
                           switch(event.key.keysym.sym)
                           {
                                   
                               case SDLK_RIGHT: player_position.x += velx;

                                    if (player_frame.y == 0)
                                    {
                                        player_frame.y = player_frame.y + 2*player_frame_height;
                                    }

                                    if(player_frame.y != 2*player_frame_height)
                                    {
                                    	player_frame.y = 2*player_frame_height;
                                    }

                                   break;

                               case SDLK_LEFT: player_position.x -= velx;

                               		if (player_frame.y == 0)
                                    {
                                        player_frame.y = player_frame.y + player_frame_height;
                                    }

                                    if(player_frame.y != player_frame_height)
                                    {
                                    	player_frame.y = player_frame_height;
                                    }
                                 
                                   break;

                               case SDLK_UP: player_position.y -= velx;

                               		player_frame.x = 4*player_frame_width;
                               		player_frame.y = 0;

                                    if(player_frame.x != 4*player_frame_width && player_frame.y != 0)
                                    {
                                    	player_frame.x = 4*player_frame_width;
                                    	player_frame.y = 0;
                                    }

                                   break;

                               case SDLK_DOWN: player_position.y += velx;

                               		player_frame.x = 0;
                               		player_frame.y = 0;

                                    if(player_frame.x != 0 && player_frame.y != 0)
                                    {
                                    	player_frame.x = 0;
                                    	player_frame.y = 0;
                                    }

                                   break;

                                   //tentativa de diagonal

                               case SDLK_UP && SDLK_LEFT:

                               		player_position.y -= velx;
                               		player_position.x -= velx;

                               		break;
                           }
                    }

                    //colisao

				    if (player_position.x < 0 ) 
				    {
				      	player_position.x = 0;
				    }
				    else if ( player_position.x > SCREEN_WIDTH-player_frame_width ) 
				    {
				      	player_position.x = SCREEN_WIDTH-player_frame_width;
				    }
				    if ( player_position.y < 0 ) 
				    {
				      	player_position.y = 0;
				    }
				    else if ( player_position.y > SCREEN_HEIGHT-player_frame_height ) 
				    {
				      	player_position.y = SCREEN_HEIGHT-player_frame_height;
				    }
				                    

                    
                }

            SDL_BlitSurface(gBackground, NULL, gScreen, NULL);
            SDL_BlitSurface(gPlayer, &player_frame, gScreen, &player_position);
            
            SDL_UpdateWindowSurface( window );
            
            SDL_Delay(10);
                
            }
    
                
    

    SDL_FreeSurface(gBackground);
    SDL_FreeSurface(gPlayer);
    
    SDL_DestroyWindow( window );
    
    SDL_Quit();
    
    return 0;
}
