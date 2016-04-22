#include "bandit.h"

Bandit::Bandit(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y)
{
	active = true;



	explode = Mix_LoadWAV((audioPath + "fire.wav").c_str());

	string basePath = filePath + "bandit.png";

	bandit = IMG_LoadTexture(renderer, basePath.c_str());

	banditRect.x = x;
	banditRect.y = y;

	int w, h;
	SDL_QueryTexture(bandit,NULL,NULL,&w,&h);
	banditRect.w = w;
	banditRect.h = h;

	posT_X = banditRect.x;
	posT_Y = banditRect.y;

	speed = 100;

	center.x = 36;
	center.y = 57;

	health = 10;
}


void Bandit::RemoveHealth()
{
	health -= 2;

	if(health <= 0)
	{
		Reset();
	}
}

void Bandit::Reset()
{
	banditRect.x = -1000;

	posT_X = banditRect.x;

	health = 10;

	active = false;
}

void Bandit::eTankMoveX(float tankSpeed, float deltaTime)
{
	posT_X +=(tankSpeed)*deltaTime;

	banditRect.x =(int)(posT_X + 0.5f);
}

void Bandit::eTankMoveY(float tankSpeed, float deltaTime)
{
	posT_Y +=(tankSpeed)*deltaTime;

	banditRect.y =(int)(posT_Y + 0.5f);
}

void Bandit::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopyEx(renderer, bandit, NULL, &banditRect, banditAngle, &center, SDL_FLIP_NONE);

}

void Bandit::Update(float deltaTime, SDL_Rect tankRect)
{
	double distancex = (banditRect.x - tankRect.x) * (banditRect.x - tankRect.x);
	double distancey = (banditRect.y - tankRect.y) * (banditRect.y - tankRect.y);

	double calcdistance = sqrt(distancex + distancey);

	if(calcdistance <= 250){
		active = true;
	}else{
		active = false;
	}

	if(active){
		x = (tankRect.x + (tankRect.w/2)) - (banditRect.x + (banditRect.w/2));
		y = (tankRect.y + (tankRect.h/2)) - (banditRect.y + (banditRect.h/2));
		banditAngle = atan2(y,x)*180/3.14;

		float radians = (banditAngle *3.14)/180;

		float move_x = speed *cos(radians);
		float move_y = speed *sin(radians);

		posT_X +=(move_x)*deltaTime;
		posT_Y +=(move_y)*deltaTime;

		banditRect.x = (int)(posT_X + 0.5f);
		banditRect.y = (int)(posT_Y + 0.5f);
	}
}

