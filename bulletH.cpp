#include "bulletH.h"

Bhud::Bhud(SDL_Renderer *renderer, string filePath, int bullet, float x, float y)
{
	active = true;

	if (bullet == 0) {
		//create the jewel base file path
		string basePath = filePath + "bulletHud.png";

		//load the image into the texture
		hud = IMG_LoadTexture(renderer, basePath.c_str());

	}
	else if (bullet == 1) {
		//create the jewel base file path
		string basePath = filePath + "bullet1.png";

		//load the image into the texture
		hud = IMG_LoadTexture(renderer, basePath.c_str());
	}
	else if (bullet == 2) {
		//create the jewel base file path
		string basePath = filePath + "bullet2.png";

		//load the image into the texture
		hud = IMG_LoadTexture(renderer, basePath.c_str());
	}
	else if (bullet == 3) {

		//create the jewel file path
		string basePath = filePath + "bullet3.png";

		//load the image into the texture
		hud = IMG_LoadTexture(renderer, basePath.c_str());
	}
	else if (bullet == 4) {

		//create the jewel file path
		string basePath = filePath + "bullet4.png";

		//load the image into the texture
		hud = IMG_LoadTexture(renderer, basePath.c_str());
	}

	else if (bullet == 5) {

		//create the jewel file path
		string basePath = filePath + "bullet5.png";

		//load the image into the texture
		hud = IMG_LoadTexture(renderer, basePath.c_str());
	}
	else if (bullet == 6) {

		//create the jewel file path
		string basePath = filePath + "bullet6.png";

		//load the image into the texture
		hud = IMG_LoadTexture(renderer, basePath.c_str());
	}
	else if (bullet == 7) {

		//create the jewel file path
		string basePath = filePath + "bullet7.png";

		//load the image into the texture
		hud = IMG_LoadTexture(renderer, basePath.c_str());
	}
	else if (bullet == 8) {

		//create the jewel file path
		string basePath = filePath + "bullet8.png";

		//load the image into the texture
		hud = IMG_LoadTexture(renderer, basePath.c_str());
	}
	else if (bullet == 9) {

		//create the jewel file path
		string basePath = filePath + "bullet9.png";

		//load the image into the texture
		hud = IMG_LoadTexture(renderer, basePath.c_str());
	}
	else if (bullet == 10) {

		//create the jewel file path
		string basePath = filePath + "bullet10.png";

		//load the image into the texture
		hud = IMG_LoadTexture(renderer, basePath.c_str());
	}
	else if (bullet == 11) {

		//create the jewel file path
		string basePath = filePath + "bullet11.png";

		//load the image into the texture
		hud = IMG_LoadTexture(renderer, basePath.c_str());
	}
	else if (bullet == 12) {

		//create the jewel file path
		string basePath = filePath + "bullet12.png";

		//load the image into the texture
		hud = IMG_LoadTexture(renderer, basePath.c_str());
	}

	//set the SDL_Rect X and Y for the base image
	hudRect.x = x;
	hudRect.y = y;

	//use the SDL_Query texture to get the W and H  of the base's texture
	int w, h;
	SDL_QueryTexture(hud, NULL, NULL, &w, &h);
	hudRect.w = w;
	hudRect.h = h;

	posJ_X = hudRect.x;
	posJ_Y = hudRect.y;
}

//jewel draw method
void Bhud::Draw(SDL_Renderer *renderer)
{
	//Draw the base
	SDL_RenderCopy(renderer, hud, NULL, &hudRect);
}

void Bhud::TankMoveX(float tankSpeed, float deltaTime)
{
	posJ_X += (tankSpeed)* deltaTime;

	hudRect.x = (int)(posJ_X + 0.5f);
}

void Bhud::TankMoveY(float tankSpeed, float deltaTime)
{
	posJ_Y += (tankSpeed)* deltaTime;

	hudRect.y = (int)(posJ_Y + 0.5f);
}

