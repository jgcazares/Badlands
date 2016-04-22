//needed includes for cross platform developement
#if defined (_WIN32) || (WIN64)
#include "SDL.h"
#include  "SDL_image.h"
#endif

#if defined (__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#endif

#if defined (__linux__)
#include SDL2/SDL2.h
#include SDL2/SDL_image.h
#endif

#include <string>
#include <iostream>
using namespace std;


class Explode {

public:

	//boolean for the state of the explosion
	bool active;

	//explosion rectangle for positions
	SDL_Texture *texture;

	//explosion rectanglefor position
	SDL_Rect posRect;

	//explosion rectangle for position
	SDL_Rect drawRect;

	//width ad height of each frame plus width and height of entire image
	int frameWidth, frameHeight, textureWidth, textureHeight;

	//float values to track time until next frame of aniamtion
	float frameCounter;

	//explode creation method, requires the renderer a path to the needed image and x position and y position
	Explode(SDL_Renderer *renderer, string filePath, float x, float y);

	//explode update requires delta time
	void Update(float deltaTime);

	//explode draw requires renderer to be passed
	void Draw(SDL_Renderer *renderer);

	//explode reset
	void Reset();

	//explode destruction
	~Explode();


};
