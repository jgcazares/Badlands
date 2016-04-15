#include "Playerbullet.h"

//bullet creation method
PlayerBullet::PlayerBullet(SDL_Renderer *renderer, string filePath, float x, float y, int dirX, int dirY)
{
	//set the bullet initial state
	active = false;

	//set the bullet speed
	speed = 800.0;

	//create the texture from the passed renderer
	texture = IMG_LoadTexture(renderer, filePath.c_str());

	//set the width and height
	int w,h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	//set the x and y position
	posRect.x = x - (posRect.w/2);
	posRect.y = y;

	//set the float position values of the texture for precision loss
	pos_X = x;
	pos_Y = y;

	//initialize the direction vars for the bullet
	//xDir = dirX;
	//yDir = dirY;

}

//reset the bullet method
void PlayerBullet::Reset()
{
	//reset the x position off the screen
	posRect.x = -1000;

	//update the pos_X for precision
	pos_X = posRect.x;

	//deactivate the bullet
	active = false;
}

//bullet draw method
void PlayerBullet::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

//bullet update method
void PlayerBullet::Update(float deltaTime)
{
	//adjust position floats based on speed
		//pos_X += (speed * xDir) * deltaTime;
		//pos_Y += (speed * yDir) * deltaTime;

		float radians = (tankangle *3.14)/180;

		float move_x = speed*cos(radians);
		float move_y = speed*sin(radians);

		pos_X += (move_x) *deltaTime;
		pos_Y += (move_y) *deltaTime;

		//update the bullet position with code to account for precision loss
		posRect.x = (int)(pos_X + 0.5f);
		posRect.y = (int)(pos_Y + 0.5f);

		//check to see if the bullet is off the top of the screen and deactivate and move offscreen
		if((posRect.y < (0 - posRect.h)) || (posRect.y > 768) || (posRect.x < (0 - posRect.w)) || (posRect.x > 1024)){
			Reset();
		}

}
