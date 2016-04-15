#include "turret.h"

Turret::Turret(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y)
{

	//activate
	active = true;

	fire = Mix_LoadWAV((audioPath + "fire.wav").c_str());

	string basePath = filePath + "tBase.png";

	tBase = IMG_LoadTexture(renderer, basePath.c_str());

	string barrelPath = filePath + "tHead.png";

	tBarrel = IMG_LoadTexture(renderer, barrelPath.c_str());

	baseRect.x = x;
	baseRect.y = y;

	int w, h;
	SDL_QueryTexture(tBase, NULL, NULL, &w, &h);
	baseRect.w = w;
	baseRect.h = h;

	barrelRect.x = x + 46;
	barrelRect.y = y + 46;

	SDL_QueryTexture(tBarrel, NULL, NULL, &w, &h);
	barrelRect.w = w;
	barrelRect.h = h;

	center.x = 3;
	center.y = -10;

	string bulletPath = filePath + "tbullet.png";

	for (int i = 0; i < 10; i++)
	{
		TurretBullet tmpBullet(renderer, bulletPath, 1500, 1500);

		bulletList.push_back(tmpBullet);
	}

	srand(time(NULL));

	//update the float
	posB_X = baseRect.x;
	posB_Y = baseRect.y;

	posT_X = baseRect.x;
	posT_Y = baseRect.x;


}

void Turret::TankMoveX(float tankSpeed, float deltaTime)
{

	posB_X += (tankSpeed) * deltaTime;
	posT_X += (tankSpeed) * deltaTime;

	baseRect.x = (int)(posB_X + 0.5f);
	barrelRect.x = (int)(posT_X + 0.5f);

}

void Turret::TankMoveY(float tankSpeed, float deltaTime)
{
	posB_Y += (tankSpeed) * deltaTime;
	posT_Y += (tankSpeed) * deltaTime;

	baseRect.y = (int)(posB_X + 0.5f);
	barrelRect.y = (int)(posT_X + 0.5f);
}

	//turret draw method
	void Turret::Draw(SDL_Renderer *renderer)
	{
		//draw the turrets bullets
		for (int i = 0; i < bulletList.size(); i++)
		{
			//check to see if the bullet is active
			if (bulletList[i].active) {
				//draw the bullet
				bulletList[i].Draw(renderer);
			}
		}

		//draw the base
		SDL_RenderCopy(renderer, tBase, NULL, &baseRect);

		//draw the barrels texture
		SDL_RenderCopyEx(renderer, tBarrel, NULL, &barrelRect, turretangle, &center, SDL_FLIP_NONE);
	}


	//turret update
	void Turret::Update(float deltaTime, SDL_Rect tankRect)
	{
		//get angle
		x = (tankRect.x + (tankRect.w / 2)) - (baseRect.x + (baseRect.w / 2));
		y = (tankRect.y + (tankRect.h / 2)) - (baseRect.y + (baseRect.h / 2));
		turretangle = atan2(y, x) * 180 / 3.14;

		if (SDL_GetTicks() > fireTime) {

			if(baseRect.x > 0 && baseRect.x < 1024 && baseRect.y > 0 && baseRect.y < 768){
				CreateBullet(tankRect);
			}


			fireTime = SDL_GetTicks() + (rand() % 3 + 1) * 1000;
		}

		//update the turrets bullets
		for (int i = 0; i < bulletList.size(); i++)
		{
			//check to see if the bullet is active
			if (bulletList[i].active) {
				//update the bullet
				bulletList[i].Update(deltaTime);
			}
		}
	}

	void Turret::CreateBullet(SDL_Rect target)
	{
		//see if there is a bullet active to fire
		for (int i = 0; i < bulletList.size(); i++)
		{
			//f the bullet is not active
			if (bulletList[i].active == false) {

				bulletList[i].Start(target, baseRect);

				//play the over sound
				Mix_PlayChannel(-1, fire, 0);

				//set the bullet to active
				bulletList[i].active = true;

				bulletList[i].posRect.x = ((baseRect.x + (baseRect.w / 2)) - (bulletList[i].posRect.w / 2));
				bulletList[i].posRect.y = ((baseRect.y + (baseRect.h / 2)) - (bulletList[i].posRect.h / 2));

				bulletList[i].pos_X = bulletList[i].posRect.x;
				bulletList[i].pos_Y = bulletList[i].posRect.y;

				//once bullet is found break out of the loop
				break;
			}
		}
	}
