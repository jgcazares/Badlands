#include "player.h"

//analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

//tank creation
Player::Player(SDL_Renderer *renderer, int pNum, string filePath, string audioPath, float x, float y)
{

	back = IMG_LoadTexture(renderer, (filePath + "health1.png").c_str());
	mid = IMG_LoadTexture(renderer, (filePath + "health2.png").c_str());
	front = IMG_LoadTexture(renderer, (filePath + "health3.png").c_str());

	backR.x = midR.x = frontR.x = 50;
	backR.y = midR.y = frontR.y = 40;
	backR.w = midR.w = frontR.w = 239;
	backR.h = midR.h = frontR.h = 32;

	playerHealth = 100.0f;
	maxHealth = 100.0f;


	//activate the player
	active = true;

	//set the player number to 0 or 1;
	playerNum = pNum;

	//set the float for player speed
	speed = 200.0f;

	fire = Mix_LoadWAV((audioPath + "fire.wav").c_str());

	//see if this is player 1 or player 2, and create the correct file path
	if(playerNum == 0){
		//create the redtak
		playerPath = filePath + "player.png";
	}

	//load the surfacen into the texture
	texture = IMG_LoadTexture(renderer, playerPath.c_str());


	//set the sdl rect
	posRect.x = x;
	posRect.y = y;

	//use the SDL query texture to get the w and h of the players texture
	int w,h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	//set the movemet speed
	pos_X = x;
	pos_Y = y;

	//set the xdir and ydir
	xDir = 0;
	yDir = 0;

	//set initial values so the tank can shoot
	xDirOld = 1;
	yDirOld = 0;

	//find center of the image texture
	center.x = posRect.w/2;
	center.y = posRect.h/2;

	//string to create the path to the players bullet image
	string bulletPath;

	//see if this is player 1 or player 2
	if(playerNum == 0){
		//creatge bullet 1 texture
		bulletPath = filePath + "bullet.png";
	}
	//create the players bullet pool
	for(int i = 0; i < 10; i++)
	{
		//create the bullet and move offscreen out of the play area
		PlayerBullet tmpBullet(renderer, bulletPath, -1000, -1000, 0,0);

		//add to the bullet list
		bulletList.push_back(tmpBullet);
	}

}


void Player::Update(float deltaTime)
{
	//check for gamepad input
	if(Xvalue != 0 || Yvalue != 0){
		//get the angle between the tank and the turret
		//x = posRect.x - xDir;
		//y = posRect.y - yDir;
		tankangle = atan2(Yvalue,Xvalue) * 180/3.14;

		//set this as the old angle and dir some the players/tank can shoot when stopped
		oldAngle = tankangle;
		//xDirOld = xDir;
		//yDirOld = yDir;

		//gives us radians
		float radians = (tankangle*3.14)/180;

		float move_x = speed *cos(radians);
		float move_y = speed *sin(radians);

		//update floats for precision loss
		pos_X += (move_x) *deltaTime;
		pos_Y += (move_y) *deltaTime;

		//update the player position
		posRect.x = (int)(pos_X + 0.5f);
		posRect.y = (int)(pos_Y + 0.5f);

	}else{
		tankangle = oldAngle;
	}


	//adjust the position based on speed direction of the joystick and deltatime
	//pos_X += (speed * xDir) * deltaTime;
	//pos_Y += (speed * yDir) * deltaTime;

	//update the player pos
	//posRect.x = (int)(pos_X +0.5f);
	//posRect.y = (int)(pos_Y +0.5f);


	//check if the tank is off screen and set it back
	if(posRect.x < 0){
		posRect.x = 0;
		pos_X = posRect.x;
	}

	if(posRect.x > 1024 - posRect.w){
		posRect.x = 1024 - posRect.w;
		pos_X = posRect.x;
	}

	if(posRect.y < 0){
		posRect.y = 0;
		pos_Y = posRect.y;
	}

	if(posRect.y > 768 - posRect.h){
		posRect.y = 768 - posRect.h;
		pos_Y = posRect.y;
	}

	//update the tanks bullets
	for(int i = 0; i < bulletList.size(); i++)
	{
		//check to see if the bullet is active
		if(bulletList[i].active){

			//update the bullet
			bulletList[i].Update(deltaTime);
		}
	}


}

void Player::enemyHit(){

	playerHealth -= .025f;

	midR.w = playerHealth/maxHealth * 239;
}

void Player::eBulletHit(){

	playerHealth -= 5;

	midR.w = playerHealth/maxHealth * 239;
}

void Player::Draw(SDL_Renderer *renderer)
{
	//draw the players bullets
	for(int i = 0; i < bulletList.size(); i++)
	{
		//check to see if the bullet is active
		if(bulletList[i].active){
			//draw the bullet
			bulletList[i].Draw(renderer);
		}
	}

	//draw the player texture
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, tankangle, &center, SDL_FLIP_NONE);

	SDL_RenderCopy(renderer, back, NULL, &backR);
	SDL_RenderCopy(renderer, mid, NULL, &midR);
	SDL_RenderCopy(renderer, front, NULL, &frontR);
}

void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	//if the players number is 0 and the joystick button is from joystick 0
	if(event.which == 0 && playerNum == 0)
	{
		//if A button
		if(event.button == 0)
		{
			//create a bullet
			CreateBullet();
		}
	}

	//if the players number is 0 and the joystick button is from joystick 0
	if(event.which == 1 && playerNum == 1)
	{
		//if A button
		if(event.button == 0)
		{
			//create a bullet
			CreateBullet();
		}
	}
}

//create a bullet
void Player::CreateBullet(){

	//see if there is a bullet active to fire
	for(int i = 0; i < bulletList.size(); i++)
	{

		//see if the bullet is not active
		if(bulletList[i].active == false){

			//play the over sound
			Mix_PlayChannel(-1, fire,0);

			//set the bullet to active
			bulletList[i].active = true;

			//use some math in the x positionto get the bullet close to the center
			bulletList[i].posRect.x = (posRect.x + (posRect.w/2));
			bulletList[i].posRect.y = (posRect.y + (posRect.h/2));

			//finish aligning to the player center using the texture width
			bulletList[i].posRect.x = bulletList[i].posRect.x - (bulletList[i].posRect.w/2);
			bulletList[i].posRect.y = bulletList[i].posRect.y - (bulletList[i].posRect.h/2);

			//set the x and y position fo the bullets float positions
			bulletList[i].pos_X = bulletList[i].posRect.x;
			bulletList[i].pos_Y = bulletList[i].posRect.y;

			//if the tank is moving fire in that direction
			if(Xvalue != 0 || Yvalue != 0){
				//set the x and y positions of the bullets float position
				bulletList[i].tankangle = tankangle;
			}else{
				//if the tank is not moving fire in the direction currently facing
				bulletList[i].tankangle = oldAngle;
			}

			//once bullet is found break out of loop
			break;
		}
	}
}


void Player::OnControllerAxis(Sint16 X, Sint16 Y)
{
	Xvalue = X;
	Yvalue = Y;

	if(!(Xvalue < -JOYSTICK_DEAD_ZONE)&&!(Xvalue > JOYSTICK_DEAD_ZONE))
	{
		Xvalue = 0.0f;
	}

	if(!(Yvalue < -JOYSTICK_DEAD_ZONE)&&!(Yvalue > JOYSTICK_DEAD_ZONE))
		{
			Yvalue = 0.0f;
		}
}



