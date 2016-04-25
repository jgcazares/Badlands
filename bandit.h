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
	#include "SDL2/SDL_image.h"
	#include "SDL2/SDL_mixer.h"
	#include "SDL2/SDL_ttf.h"
#endif


class Bandit{

public:
	//check to see if the tank is active
	bool active;

	//check to see if the drop has been dropped
	bool drop;

	//string to hold the p-ath to the tanks texture
	string banditPath;

	SDL_Texture *bandit;

	SDL_Rect banditRect;


	float x,y, banditAngle;

	Mix_Chunk *explode;

	float speed;

	int health;

	//point that will be used to rotate the tank
	SDL_Point center;

	float posT_X, posT_Y;

	//enemy tank creation method
	Bandit(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y);

	//update the enemy tank
	void Update(float deltaTime, SDL_Rect banditRect);

	//draw the enemy tank
	void Draw(SDL_Renderer *renderer);

	//reset the tank
	void Reset();
	void Restart();

	//remove health
	void RemoveHealth();

	void eTankMoveX(float tankSpeed, float deltaTime);

	void eTankMoveY(float tankSpeed, float deltaTime);

};
