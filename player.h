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

#include "Playerbullet.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdint.h>



class Player{

public:

	//textures for health bar
	SDL_Texture *back, *mid, *front;

	//rectangles for the health bar
	SDL_Rect backR, midR, frontR;

	//player health
	float playerHealth, maxHealth;

	//tank hit bu etank
	void enemyHit();

	//tank hit by turret bullet
	void eBulletHit();

	//is the player active - still alive
	bool active;

	//variable to hold the list of bullets
	vector<PlayerBullet> bulletList;

	//string to hold the path to the players texture
	string playerPath;

	//integer for the players numbers
	int playerNum;

	//players texture
	SDL_Texture *texture;

	//players sdl rect
	SDL_Rect posRect;


	//floats to hold the joystick axis
	float xDir, xDirOld;
	float yDir, yDirOld;

	//float for player speed
	float speed;

	//float for players position
	float pos_X, pos_Y;

	//vars used to get the angle between the tanks position and the joysticks position
	float x, y, tankangle;

	//point that will be used to rotate the turret
	SDL_Point center;

	//keep the last angle so the player can fire in the correct direction
	float oldAngle;

	//audio sound effect
	Mix_Chunk *fire;

	Sint16 Xvalue, Yvalue;

	///plyers creation method using passed in values for render, player number, path to texture, staring position x andy
	Player(SDL_Renderer *renderer, int pNum, string filePath, string audioPath, float x, float y);

	//method to allow the player to move via joystick axis
	void OnControllerAxis(Sint16 X, Sint16 Y);

	//method to allow the player interact with joystick button
	void OnControllerButton( const SDL_ControllerButtonEvent event);

	//update the player using the passed in deltatime
	void Update(float deltaTime);

	//draw the players main passed in renderer
	void Draw(SDL_Renderer *renderer);

	//reset the player
	void Reset();

private:
	void CreateBullet();

};
