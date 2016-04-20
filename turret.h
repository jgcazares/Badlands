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
#include "SDL2_ttf.h"
#endif

#include "tbullet.h"

class Turret {

public:
	//is the turret active still alive
	bool active;

	bool powderDropped;

	//variable to hold the list of bullets
	vector<TurretBullet>bulletList;

	//string to hold the path to the turrets texture
	string turretPath;

	//turret texture to hold the base and barel images
	SDL_Texture *tBase;
	SDL_Texture *tBarrel;

	SDL_Texture *powder;

	SDL_Rect powderRect;

	//turrets sdl rect
	SDL_Rect baseRect;

	SDL_Rect barrelRect;

	float x, y, turretangle;

	int tHealth;

	SDL_Point center;

	float fireTime = 3.0f;
	float fireRate = 100.0f;

	Mix_Chunk *fire;

	//turret creation
	Turret(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y);

	void Update(float deltaTime, SDL_Rect tankRect);

	void Draw(SDL_Renderer *renderer);

	//reset
	void Reset();

	void CreateBullet(SDL_Rect target);

	void RemoveHealth();

	void Drop();

	void TankMoveX(float tankSpeed, float deltaTime);

	void TankMoveY(float tankSpeed, float deltaTime);

	float posB_X, posB_Y, posT_X, posT_Y;
};
