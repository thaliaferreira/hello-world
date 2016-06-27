#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 500;

typedef struct
{
	int x, y;
	int life;
	char *nome;
} GIRL;

int processEvent(SDL_Window *, GIRL *);
void doRender(SDL_Renderer *, GIRL *);
void colision(SDL_Window *, GIRL *)

int main(int argc, char *args[])
{

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	int gameRunning=1;

	GIRL girl;
	girl.x = 350;
	girl.y = 250;

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Can't init SDL: %s", SDL_GetError());

	window = SDL_CreateWindow("Girl on Fire!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	while(gameRunning)
	{
		gameRunning = processEvent(window, &girl);

		doRender(renderer, &girl);

		colision(window, &girl);

		SDL_Delay(10);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);


	//Limpando a bagunça
	SDL_Quit();

	return 0;

}

int processEvent(SDL_Window *window, GIRL *girl)
{
	SDL_Event event;
	int gameRunning = 1; 
	
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				gameRunning = 0;
			break;
		}
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);
    	if(state[SDL_SCANCODE_LEFT])
    	{
       		girl->x -= 5;
    	}
    	if(state[SDL_SCANCODE_RIGHT])
    	{
       		girl->x += 5;
    	}
    	if(state[SDL_SCANCODE_UP])
    	{
        	girl->y -= 5;
   	}
	if(state[SDL_SCANCODE_DOWN])
    	{
       		girl->y += 5;
    	}

	return gameRunning;
}

void doRender(SDL_Renderer *renderer, GIRL *girl)
{
	//Desenhando o fundo
	SDL_SetRenderDrawColor(renderer, 0, 200, 0, 200);
	SDL_RenderClear(renderer);

	//Desenhando o "player"
	SDL_SetRenderDrawColor(renderer, 127, 0, 127, 255);
	SDL_Rect rect = {girl->x, girl->y, 100, 100};
	SDL_RenderFillRect(renderer, &rect);

	//Mostrando a obra de arte para o usuário :)
	SDL_RenderPresent(renderer);
}

void colision(SDL_Window *window, GIRL *girl)
{
	if (girl->x < 0 ) 
    	{
        	girl->x = 0;
    	}
    	else if (girl->x > SCREEN_WIDTH-100) 
    	{
		girl->x = SCREEN_WIDTH-100;
    	}
    	if (girl->y < 0) 
    	{
        	girl->y = 0;
    	}
    	else if (girl->y > SCREEN_HEIGHT-100) 
    	{
        	girl->y = SCREEN_HEIGHT-100;
    	}
}
