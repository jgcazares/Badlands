#include <string>
#include <iostream>

using namespace std;
#include <vector>

#if defined (__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#endif


#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_images.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#endif

class Powder {

public:
	bool active;

	SDL_Texture *powder;

	SDL_Rect powderRect;

	float posJ_X, posJ_Y;

	Powder(SDL_Renderer *renderer, string filePath, int color, float x, float y);

	void Draw(SDL_Renderer *renderer);

	void Update(float deltaTime);

	void TankMoveX(float tankSpeed, float deltaTime);

	void TankMoveY(float tankSpeed, float deltaTime);

};
