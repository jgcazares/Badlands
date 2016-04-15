#include <string>
#include <iostream>
using namespace std;

#if defined(_WIN32) || (_WIN64)
	#include "SDL.h"
	#include "SDL_image.h"
#endif

#if defined (__APPLE__)
	#include <SDL2/SDL.h>
	#include <SDL2_image/SDL_image.h>
#endif

#if defined(__linux__)
	#include "SDL2/SDL.h"
	#include "SDL2/SDL_images.h"
#endif

class PlayerBullet {

public:
	//boolean for the state of the bullet
	bool active;

	//bullet texture
	SDL_Texture *texture;

	//bullet rectangle for position ax and y and size w,h
	SDL_Rect posRect;

	//movement direction values
	float xDir, yDir;

	//bullet speed
	float speed;

	//bullet float positions to prevent precision loss
	float pos_X, pos_Y;

	//tank angle pass
	float tankangle;

	//bullet creation method requires renderer a path to the image and x and y position
	PlayerBullet(SDL_Renderer *renderer, string filePath, float x, float y, int dirX, int dirY);

	//bullet update
	void Update(float deltaTime);

	//bullet draw
	void Draw(SDL_Renderer *renderer);

	//bullet resert
	void Reset();
};
