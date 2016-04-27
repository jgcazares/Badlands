#include "explosion.h"

//explode creation method
Explode::Explode(SDL_Renderer *renderer, string filePath, float x, float y)
{
	//set the explosion initial state
	active = false;

	//get the directory path the explosion graphic
	string explodePath = filePath + "explosion.png";

	//init the frame counter
	frameCounter = 0.0f;

	//load th etexture into a surface
	SDL_Surface *surface = IMG_Load(explodePath.c_str());

	//create the texture from the [assed renderer and created surface
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	//free the surface for later use
	SDL_FreeSurface(surface);

	//set the width and height of the bullets rectangle
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	//getting the width and height of the entire image
	textureWidth = w;
	textureHeight = h;

	//setting the individual image size
	posRect.w = 196;
	posRect.h = 190;

	//set the x and y positions of the explosion rectangle
	posRect.x = x;
	posRect.y = y;

	//frame width and height of each animation image
	frameWidth = 196;
	frameHeight = 190;

	//original draw rectangle used to draw one frame of the animation strip
	drawRect.x = 196;
	drawRect.y = 0;
	drawRect.w = frameWidth;
	drawRect.h = frameHeight;
}


void Explode::Reset() {
	//deactivate the bullet
	active = false;
}


//bullet update
void Explode::Update(float deltaTime) {
	//check to see if the bullet is active
	if (active) {

		//add to the framecounter
		frameCounter += deltaTime;

		//check to see if any time has passed
		if (frameCounter >= .035f) {

			//reset the frame counter
			frameCounter = 0.0f;

			//add to the draw rect the width of an individual frame to move the drawing area one over
			drawRect.x += frameWidth;

			//see if we have gone past the image width
			if (drawRect.x >= textureWidth)
			{

				//if so draw back to the first
				drawRect.x = 0;

				//set the active to false
				active = false;

				//move off screen till needed again
				posRect.x = -1000;
				posRect.y = -1000;
			}
		}
	}
}



//bullet draw method
void Explode::Draw(SDL_Renderer *renderer) {
	SDL_RenderCopy(renderer, texture, &drawRect, &posRect);
}

//bullet destruction
Explode::~Explode()
{
	//SDL_DestroyTexture(texture);
}