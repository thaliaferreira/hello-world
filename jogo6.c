#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 500;

typedef struct
{
	int x, y, w, h;
	int life;
} GIRL;

typedef struct 
{
	int x, y, w, h;
} Enemy;

typedef struct 
{
	GIRL girl;

	Enemy enemies;

	//background
	SDL_Texture *background;

	//player
	SDL_Texture *girlImg;
	SDL_Rect srcRect;

	//enemy
	SDL_Texture *enemyImg;

	//renderer
	SDL_Renderer *renderer;

} GameState;


void LoadGame(GameState *);
int processEvent(SDL_Window *, GameState *);
void doRender(GameState *);
void colisionPlayer(SDL_Window *, GameState *);
void colisionEnemy(SDL_Window *, GameState *);

int main(int argc, char *args[])
{

	GameState gameState;

	SDL_Window *window = NULL;

	int gameRunning=1;

	gameState.girl.x = 350;
	gameState.girl.y = 250;
	gameState.girl.w = 60;
	gameState.girl.h = 90;

	gameState.enemies.x = 500;
	gameState.enemies.y = 250;
	gameState.enemies.w = 80;
	gameState.enemies.h = 80;
 
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Can't init SDL: %s", SDL_GetError());

	window = SDL_CreateWindow("Girl on Fire!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	gameState.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	LoadGame(&gameState);

	while(gameRunning)
	{

		gameRunning = processEvent(window, &gameState);
		
		doRender(&gameState);

		colisionPlayer(window, &gameState);

		colisionEnemy(window, &gameState);

		//SDL_Delay(10);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyTexture(gameState.girlImg);
	SDL_DestroyTexture(gameState.enemyImg);
	SDL_DestroyRenderer(gameState.renderer);

	//Limpando a bagunça
	SDL_Quit();

	return 0;

}

void LoadGame(GameState *game)
{
	SDL_Surface *surface = NULL;

	//fundo
	surface = IMG_Load("background.jpg");
	if(surface == NULL)
	{
		printf("Can't find background.jpg!\n");
		SDL_Quit();
		exit(1);
	}

	game->background = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	//player
	game->srcRect.x = 0;
	game->srcRect.y = 0;
	game->srcRect.w = game->girl.w;
	game->srcRect.h = game->girl.h;

	surface = IMG_Load("girl.png");
	if(surface == NULL)
	{
		printf("Can't find girl.png!\n");
		SDL_Quit();
		exit(1);
	}

	game->girlImg = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	//enemy
	surface = IMG_Load("enemy.gif");
	if(surface == NULL)
	{
		printf("Can't find enemy.gif!\n");
		SDL_Quit();
		exit(1);
	}

	game->enemyImg = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);
}

int processEvent(SDL_Window *window, GameState *game)
{
	SDL_Event event;
	int gameRunning = 1; 
	int speedP = 6, speedE = 3;
	
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				gameRunning = 0;
			break;
		}
	}

	//Movimento do player
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	    if(state[SDL_SCANCODE_LEFT])
	    {
	    	game->girl.x -= speedP;
	    
	    	if(game->srcRect.y == 0)
	    		game->srcRect.y += game->girl.h;

	    	if(game->srcRect.y != game->girl.h)
	    		game->srcRect.y = game->girl.h;

			game->srcRect.x += 60;

            if (game->srcRect.x == 360)
            {
	            game->srcRect.x = 0;
            }
	    }

	    if(state[SDL_SCANCODE_RIGHT])
	    {
	       	game->girl.x += speedP;
	       	
	       	if(game->srcRect.y == 0)
	    		game->srcRect.y += 2*game->girl.h;

	    	if(game->srcRect.y != 2*game->girl.h)
	    		game->srcRect.y = 2*game->girl.h;

	    	game->srcRect.x += 60;

            if (game->srcRect.x == 360)
            {
	            game->srcRect.x = 0;
            }
	    }
	   
	    if(state[SDL_SCANCODE_UP])
	    {
	        game->girl.y -= speedP;
	        
	        game->srcRect.x = 4*game->girl.w;
	        game->srcRect.y = 0;
	    	if(game->srcRect.x != 4*game->girl.w && game->srcRect.y != 0)
	    	{
	    		game->srcRect.x = 4*game->girl.w;
	    		game->srcRect.y = 0;
	    	}
	   	}
	    if(state[SDL_SCANCODE_DOWN])
	    {
	       	game->girl.y += speedP;

	       	game->srcRect.x = 0;
	    	game->srcRect.y = 0;
	    	if(game->srcRect.x != 0 && game->srcRect.y != 0)
	    		game->srcRect.x = 0;
	    }

	    //Movimento do inimigo

	    if(game->enemies.x < game->girl.x)
	    {
	    	game->enemies.x += speedE;
	    }
	    if(game->enemies.x > game->girl.x)
	    {
	    	game->enemies.x -= speedE;
	    }
	    if(game->enemies.y < game->girl.y)
	    {
	    	game->enemies.y += speedE;
	    }
	    if(game->enemies.y > game->girl.y)
	    {
	    	game->enemies.y -= speedE;
	    }
		if(game->enemies.x < game->girl.x && game->enemies.y < game->girl.y)
		{
			game->enemies.x += speedE-2;
			game->enemies.y += speedE-2;
		}
		if (game->enemies.x > game->girl.x && game->enemies.y > game->girl.y)
		{
			game->enemies.x -= speedE-2;
			game->enemies.y -= speedE-2;
		}
		if(game->enemies.y < game->girl.y && game->enemies.x > game->girl.x)
		{
			game->enemies.y += speedE-2;
	 		game->enemies.x -= speedE-2;
	 	}
	 	if(game->enemies.y > game->girl.y && game->enemies.x < game->girl.x)
		{
			game->enemies.y -= speedE-2;
			game->enemies.x += speedE-2;
		}
	   
	return gameRunning;
}

void doRender(GameState *game)
{
	//Desenhando o fundo
	SDL_RenderCopy(game->renderer, game->background, NULL, NULL);

	//Desenhando o player
	SDL_Rect playerRect = {game->girl.x, game->girl.y, game->girl.w, game->girl.h};
	SDL_RenderCopy(game->renderer, game->girlImg, &game->srcRect, &playerRect);

	//Desenhando o inimigo
	SDL_Rect enemyRect = {game->enemies.x, game->enemies.y, game->enemies.w, game->enemies.h};
	SDL_RenderCopy(game->renderer, game->enemyImg, NULL, &enemyRect);

	//Mostrando a obra de arte para o usuário :)
	SDL_RenderPresent(game->renderer);
}

void colisionPlayer(SDL_Window *window, GameState *game)
{
    int wallWidth = 62;

    if (game->girl.x < wallWidth ) 
    {
        game->girl.x = wallWidth;
    }
    else if (game->girl.x > (SCREEN_WIDTH-wallWidth)-game->girl.w) 
    {
        game->girl.x = (SCREEN_WIDTH-wallWidth)-game->girl.w;
    }
    if (game->girl.y < wallWidth) 
    {
        game->girl.y = wallWidth;
    }
    else if (game->girl.y > (SCREEN_HEIGHT-wallWidth)-game->girl.h) 
    {
        game->girl.y = (SCREEN_HEIGHT-wallWidth)-game->girl.h;
    }
}

void colisionEnemy(SDL_Window *window, GameState *game)
{
    int wallWidth = 62;

    if (game->enemies.x < wallWidth ) 
    {
        game->enemies.x = wallWidth;
    }
    else if (game->enemies.x > (SCREEN_WIDTH-wallWidth)-game->enemies.w) 
    {
        game->enemies.x = (SCREEN_WIDTH-wallWidth)-game->enemies.w;
    }
    if (game->enemies.y < wallWidth) 
    {
        game->enemies.y = wallWidth;
    }
    else if (game->enemies.y > (SCREEN_HEIGHT-wallWidth)-game->enemies.h) 
    {
        game->enemies.y = (SCREEN_HEIGHT-wallWidth)-game->enemies.h;
    }
}