#include "poweder.h"

Powder::Powder(SDL_Renderer *renderer, string filePath, int color, float x, float y)
{
	active = true;

	if (color == 0) {
		//create the jewel base file path
		string basePath = filePath + "Bag.png";

		//load the image into the texture
		powder = IMG_LoadTexture(renderer, basePath.c_str());

	}
	else if (color == 1) {
		//create the jewel base file path
		string basePath = filePath + "Bag.png";

		//load the image into the texture
		powder = IMG_LoadTexture(renderer, basePath.c_str());
	}
	else if (color == 2) {
		//create the jewel base file path
		string basePath = filePath + "Bag.png";

		//load the image into the texture
		powder = IMG_LoadTexture(renderer, basePath.c_str());
	}
	else if (color == 3) {

		//create the jewel file path
		string basePath = filePath + "Bag.png";

		//load the image into the texture
		powder = IMG_LoadTexture(renderer, basePath.c_str());
	}
	else if (color == 4) {

		//create the jewel file path
		string basePath = filePath + "fuelDrop.png";

		//load the image into the texture
		powder = IMG_LoadTexture(renderer, basePath.c_str());
	}

	//set the SDL_Rect X and Y for the base image
	powderRect.x = x;
	powderRect.y = y;

	//use the SDL_Query texture to get the W and H  of the base's texture
	int w, h;
	SDL_QueryTexture(powder, NULL, NULL, &w, &h);
	powderRect.w = w;
	powderRect.h = h;

	posJ_X = powderRect.x;
	posJ_Y = powderRect.y;
}

//jewel draw method
void Powder::Draw(SDL_Renderer *renderer)
{
	//Draw the base
	SDL_RenderCopy(renderer, powder, NULL, &powderRect);
}

void Powder::TankMoveX(float tankSpeed, float deltaTime)
{
	posJ_X += (tankSpeed)* deltaTime;

	powderRect.x = (int)(posJ_X + 0.5f);
}

void Powder::TankMoveY(float tankSpeed, float deltaTime)
{
	posJ_Y += (tankSpeed)* deltaTime;

	powderRect.y = (int)(posJ_Y + 0.5f);
}

