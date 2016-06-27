#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 500;

typedef struct
{
	int x, y, w, h;
	int life;
	char *nome;
} GIRL;

typedef struct 
{
	GIRL girl;

	//background
	SDL_Texture *background;

	//player
	SDL_Texture *girlImg;
	SDL_Rect srcRect;
	//SDL_Rect destRect;

	//renderer
	SDL_Renderer *renderer;

} GameState;


void LoadGame(GameState *);
int processEvent(SDL_Window *, GameState *);
void doRender(GameState *);
void colision(SDL_Window *, GameState *);



int main(int argc, char *args[])
{

	GameState gameState;

	SDL_Window *window = NULL;
	//SDL_Renderer *renderer = NULL;

	int gameRunning=1;

	gameState.girl.x = 350;
	gameState.girl.y = 250;
	gameState.girl.w = 60;
	gameState.girl.h = 90;
 
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Can't init SDL: %s", SDL_GetError());

	window = SDL_CreateWindow("Girl on Fire!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	gameState.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	while(gameRunning)
	{
		LoadGame(&gameState);

		gameRunning = processEvent(window, &gameState);
		
		doRender(&gameState);

		colision(window, &gameState);

		//SDL_Delay(10);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyTexture(gameState.girlImg);
	SDL_DestroyRenderer(gameState.renderer);

	//Limpando a bagunça
	SDL_Quit();

	return 0;

}

void LoadGame(GameState *game)
{
	SDL_Surface *surface = NULL;

	surface = IMG_Load("background.jpg");
	if(surface == NULL)
	{
		printf("Can't find background.jpg!\n");
		SDL_Quit();
		exit(1);
	}

	game->background = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	game->srcRect.x = 0;
	game->srcRect.y = 0;
	game->srcRect.w = game->girl.w;
	game->srcRect.h = game->girl.h;

	// game->destRect.x = SCREEN_WIDTH/2 - game->girl.w/2;
	// game->destRect.y = SCREEN_HEIGHT/2 - game->girl.h/2;
	// game->destRect.w = game->girl.w;
	// game->destRect.h = game->girl.h;

	surface = IMG_Load("girl.png");
	if(surface == NULL)
	{
		printf("Can't find girl.png!\n");
		SDL_Quit();
		exit(1);
	}

	game->girlImg = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);
}

int processEvent(SDL_Window *window, GameState *game)
{
	SDL_Event event;
	int gameRunning = 1; 
	int speed = 10;
	
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
    if(state[SDL_SCANCODE_UP])
    {
        game->girl.y -= speed;
        if(game->srcRect.x == 0)
    		game->srcRect.x += 4*game->girl.w;
    	if(game->srcRect.x != 4*game->girl.w)
    		game->srcRect.x = 4*game->girl.w;
   	}
    if(state[SDL_SCANCODE_DOWN])
    {
       	game->girl.y += speed;

       	game->srcRect.x = 0;
    	game->srcRect.y = 0;
    	if(game->srcRect.x != 0 && game->srcRect.y != 0)
    		game->srcRect.x = 0;
    }
    if(state[SDL_SCANCODE_LEFT])
    {
    	game->girl.x -= speed;
    	if(game->srcRect.y == 0)
    		game->srcRect.y += game->girl.h;
    	if(game->srcRect.y != game->girl.h)
    		game->srcRect.y = game->girl.h;
    }
    if(state[SDL_SCANCODE_RIGHT])
    {
       	game->girl.x += speed;
       	if(game->srcRect.y == 0)
    		game->srcRect.y += 2*game->girl.h;
    	if(game->srcRect.y != 2*game->girl.h)
    		game->srcRect.y = 2*game->girl.h;
    }
   
	return gameRunning;
}

void doRender(GameState *game)
{
	//Desenhando o fundo
	SDL_RenderCopy(game->renderer, game->background, NULL, NULL);

	//Desenhando o "player"
	SDL_Rect rect = {game->girl.x, game->girl.y, game->girl.w, game->girl.h};
	SDL_RenderCopy(game->renderer, game->girlImg, &game->srcRect, &rect);

	//Mostrando a obra de arte para o usuário :)
	SDL_RenderPresent(game->renderer);
}

void colision(SDL_Window *window, GameState *game)
{
	if (game->girl.x < 62 ) 
    {
        game->girl.x = 62;
    }
    else if (game->girl.x > SCREEN_WIDTH-(2*game->girl.w+2)) 
    {
        game->girl.x = SCREEN_WIDTH-(2*game->girl.w+2);
    }
    if (game->girl.y < 62) 
    {
       	game->girl.y = 62;
    }
    else if (game->girl.y > SCREEN_HEIGHT-(2*game->girl.h-10)) 
    {
        game->girl.y = SCREEN_HEIGHT-(2*game->girl.h-20);
    }
}
