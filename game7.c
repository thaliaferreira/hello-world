#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MAX_BULLETS 1000

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 500;

typedef struct
{
	int x, y, w, h;
	int life;
	char *nome;
	int shooting;
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


typedef struct
{
	int x, y, vel;
} Bullet;

	Bullet *bullets[MAX_BULLETS] = {NULL};
	SDL_Texture *bulletTexture;
	SDL_Renderer *renderer;


void addBullet(int x, int y, int vel)
{
  int found = -1;
  for(int i = 0; i < MAX_BULLETS; i++)
  {
    if(bullets[i] == NULL)
    {
      found = i;
      break;
    }
  }
    
  if(found >= 0)
  {
    int i = found;
    bullets[i] = malloc(sizeof(Bullet));
    bullets[i]->x = x;
    bullets[i]->y = y;
    bullets[i]->vel = vel;    
  }
}

void removeBullet(int i)
{
  if(bullets[i])
  {
    free(bullets[i]);
    bullets[i] = NULL;
  }
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
	    if(state[SDL_SCANCODE_UP])
	    {
	        game->girl.y -= speedP;

	        game->srcRect.x = 4*game->girl.w;
		   	game->srcRect.y = 0;

	    	if(game->srcRect.x != 4*game->girl.w && game->srcRect.y != 0)
	    	{
	    		game->srcRect.x = 4*game->srcRect.x;
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

	    if(state[SDL_SCANCODE_LEFT])
	    {
	    	game->girl.x -= speedP;

	    	if(game->srcRect.y == 0)
	    		game->srcRect.y += game->girl.h;

	    	if(game->srcRect.y != game->girl.h)
	    		game->srcRect.y = game->girl.h;

            if (game->srcRect.x == 420)
            {
	            game->srcRect.x = 0;
            }

            else 
            {
            	game->srcRect.x += 60;
            }
	    }

	    if(state[SDL_SCANCODE_RIGHT])
	    {
	       	game->girl.x += speedP;

	       	if(game->srcRect.y == 0)
	    		game->srcRect.y += 2*game->girl.h;

	    	if(game->srcRect.y != 2*game->girl.h)
	    		game->srcRect.y = 2*game->girl.h;

            if (game->srcRect.x == 420)
            {
	            game->srcRect.x = 0;
            }

            else 
            {
            	game->srcRect.x += 60;
            }
	    }

	    if(state[SDL_SCANCODE_SPACE])
	   	{
	   		if (game->srcRect.x != 420 && game->srcRect.y != 270)
	   		{
	   			game->srcRect.x = 420;
	   			game->srcRect.y = 270;
	   		}

	   		addBullet(game->girl.x+10, game->girl.y+5, 6);

	   		game->girl.shooting = 1;

	   	}

	   	else
	   	{
	   		game->girl.shooting = 0;
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


void doRender(SDL_Renderer *renderer, GameState *game)
{
	//Desenhando o fundo
	SDL_RenderCopy(game->renderer, game->background, NULL, NULL);

	//Desenhando o "player"
	SDL_Rect rect = {game->girl.x, game->girl.y, game->girl.w, game->girl.h};
	SDL_RenderCopy(game->renderer, game->girlImg, &game->srcRect, &rect);

	//Desenhando o bullet
	////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	///////////////////////PROBLEMA/////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////
	/*for(int i = 0; i < MAX_BULLETS; i++) if(bullets[i])
	{
		SDL_Rect rect = {bullet[i]->x, bullet[i]->y, 8, 8};
		SDL_RenderCopy(renderer, bulletTexture, NULL, &rect);
	}*/

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

void Logic (GIRL *girl) 
{
	for(int i = 0; i < MAX_BULLETS; i++) if(bullets[i])
  	{
   		bullets[i]->x += bullets[i]->vel;
   
    	if(bullets[i]->x < -1000 || bullets[i]->x > 1000)
      		removeBullet(i);
  	}
}



int main(int argc, char *args[])
{

	GameState gameState;

	SDL_Window *window = NULL;
	SDL_Renderer *renderer;

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

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderSetLogicalSize(renderer, 320, 240);

	SDL_Surface *bullet = IMG_Load("bullet.png");

    if(!bullet)
    {
    	printf("Cannot find bullet\n");
    	return 1;
    }

    bulletTexture = SDL_CreateTextureFromSurface(renderer, bullet);
    SDL_FreeSurface(bullet);

	LoadGame(&gameState);

	while(gameRunning)
	{

		gameRunning = processEvent(window, &gameState);
		
		doRender(renderer, &gameState);

		colisionPlayer(window, &gameState);

		colisionEnemy(window, &gameState);

		//SDL_Delay(10);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyTexture(gameState.girlImg);
	SDL_DestroyRenderer(gameState.renderer);
	SDL_DestroyTexture(gameState.enemyImg);
	SDL_DestroyTexture(bulletTexture);

	for(int i = 0; i < MAX_BULLETS; i++)
	{
		removeBullet(i);
	}

	//Limpando a bagunça
	SDL_Quit();

	return 0;

}
