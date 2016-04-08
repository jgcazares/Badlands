#include <iostream>
using namespace std;

//screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

//delta time var inititalization
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

#if defined (__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>

///string variable to hold the current working directory on APPLE
string currentWorkingDirectory(getcwd(NULL,0));
//create a string to link to the images folder on apple
string images_dir = currentWorkingDirectory + "/src/Badlands/";
//create a string to link the audio
string audio_dir = currentWorkingDirectory + "/src/Badlands/";

#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include <direct.h>
#define getcwd _getcwd

string currentWorkingDirectory(getcwd(NULL, 0));
string images_dir = currentWorkingDirectory + "\\src\\Badlands\\";
string audio_dir = currentWorkingDirectory + "\\src\\Badlands\\";
#endif



#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_images.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#include <unistd.h>

string currentWorkingDirectory(getcwd(NULL, 0));
string images_dir = currentWorkingDirectory + "/src/Badlands/";
string audio_dir = currentWorkingDirectory + "/src/Badlands/";
#endif



//variables for the backgrounds speed
int bkgdSpeed = 100;

SDL_Rect bkgd1Pos, bkgd2Pos;

//background floats
float b1pos_x = 0, b1pos_y = 0;
float b2pos_x = -1024, b2pos_y = 0;

//MOVE THE BAKCGROUND
void UpdateBackground(float deltaTime) {

	//Update the bkgd 1
	b1pos_x += (bkgdSpeed * 1) * deltaTime;

	//set the new bkgd10 poition
	bkgd1Pos.x = (int)(b1pos_x + 0.5f);

	//reset when off the bottom of the screen
	if (bkgd1Pos.x >= 1024)
	{

		bkgd1Pos.x = -1024;
		b1pos_x = bkgd1Pos.x;


	}


	//Update the bkgd 2
	b2pos_x += (bkgdSpeed * 1) * deltaTime;

	//set the new bkgd10 poition
	bkgd2Pos.x = (int)(b2pos_x + 0.5f);

	//reset when off the bottom of the screen
	if (bkgd2Pos.x >= 1024)
	{

		bkgd1Pos.x = -1024;
		b2pos_x = bkgd2Pos.x;


	}

}

////////*****************joystick dead zone*********************
//analog stick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

//joystick direction vars
float xDir, yDir;

//create sedl rectangle for the two player graphic
SDL_Rect cursorPos, activePos;

//cursor float movement
float pos_X, pos_Y;

//cursor speed
int cursorSpeed = 400;

void moveCursor(const SDL_ControllerAxisEvent e){

	if(e.which == 0){
		if(e.axis == 0){

			if(e.value < -JOYSTICK_DEAD_ZONE){
				xDir = -1.0f;
			}else if(e.value > JOYSTICK_DEAD_ZONE){
				xDir = 1.0f;
			}else{
				xDir = 0.0f;
			}
		}

		if(e.axis == 1){

			if(e.value < -JOYSTICK_DEAD_ZONE){
				yDir = -1.0f;
			}else if(e.value > JOYSTICK_DEAD_ZONE){
				yDir = 1.0f;
			}else{
				yDir = 0.0f;
			}

		}



	}


}


void UpdateCursor(float deltaTime){
	//update the cursor
	pos_X += (cursorSpeed * xDir) * deltaTime;
	pos_Y += (cursorSpeed * yDir) * deltaTime;

	cursorPos.x = (int)(pos_X + 0.5f);
	cursorPos.y = (int)(pos_Y + 0.5f);

	activePos.x = cursorPos.x;
	activePos.y = cursorPos.y;

	if(cursorPos.x < 0){
		cursorPos.x = 0;
		pos_X = cursorPos.x;

	}

	if(cursorPos.x > 1024 - cursorPos.w){
		cursorPos.x = 1024 - cursorPos.w;
		pos_X = cursorPos.x;
	}

	if(cursorPos.y < 0){
		cursorPos.y = 0;
		pos_Y = cursorPos.y;
	}

	if(cursorPos.y > 768 - cursorPos.h){
		cursorPos.y = 768 - cursorPos.h;
		pos_Y = cursorPos.y;
	}

}


//variables for all the menus button over
bool players1Over = false, players2Over = false, instructionsOver = false,
		quitOver = false, menuOver = false, playOver = false;



int main() {

	// ************************ CREATE THE SDL WINDOW START ***************************
	//start sdl2
	SDL_Init(SDL_INIT_EVERYTHING);


	//CREATE a sdl window reference
	SDL_Window *window = NULL;

	//create a SDL render target
	SDL_Renderer *renderer = NULL;

	//create a SDL Window in the middle of the screen
	window = SDL_CreateWindow("Badlands",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_SHOWN);

	//create an SDL render target
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//************** CREATE THE SDL WINDOW END  ******************






	// ************* VARIABLE CREATION ***************************
	//MAIN LOOP FLAG
	bool quit = false;
	//event handler
	SDL_Event e;
	// ************** variable creation end **********************

	// ************************* turn on the game controller events ***********************
	SDL_GameControllerEventState(SDL_ENABLE);

	//set up a game controller variable for player 1
	SDL_GameController* gGameController0 = NULL;

	//open game controller
	gGameController0 = SDL_GameControllerOpen(0);

	//OPEN AUDIO CHANNEL
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//load music file
	Mix_Music *bgm = Mix_LoadMUS((audio_dir + "battle.wav").c_str());

	//if the music file is not playing play it
	if(!Mix_PlayingMusic())
		Mix_PlayMusic(bgm, -1);

	//set up a sound efect chunk for the button over state
	Mix_Chunk *overSound = Mix_LoadWAV((audio_dir + "over.wav").c_str());

	//set up a sound effect of the button pressed state
	Mix_Chunk *pressedSound =Mix_LoadWAV((audio_dir + "pressed.wav").c_str());

	//bool value to control the over sound effect
	bool alreadyOver = false;


	//***********CREATE BACKGROUND************
	// create a SDL surface to hold the background image
	SDL_Surface *surface = IMG_Load((images_dir + "desert.png").c_str());

	//create an sdl texture
	SDL_Texture *bkgd1;

	//place the surface into the texture bkgd1
	bkgd1 = SDL_CreateTextureFromSurface(renderer, surface);

	//create an sdl texture
	SDL_Texture *bkgd2;

	//place the surface into the texture bkgd1
	bkgd2 = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//set the X Y W H for the rectangle
	bkgd1Pos.x = 0;
	bkgd1Pos.y = 0;
	bkgd1Pos.w = 1024;
	bkgd1Pos.h = 768;



	//set the X Y W H for the rectangle
	bkgd2Pos.x = -1024;
	bkgd2Pos.y = 0;
	bkgd2Pos.w = 1024;
	bkgd2Pos.h = 768;

	//////***********************************CREATE BAKCGROUND END *******************


	////////************************cursor START****************************
	surface = IMG_Load((images_dir + "sword.png").c_str());

	//create an sdl texture
	SDL_Texture *cursor;

	//place the surface into the texture bkgd1
	cursor = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);


	//create sdl rect for title
	//SDL_Rect cursorPos, activePos;

	//set the X Y W H for the cursor graphic
	cursorPos.x = 0;
	cursorPos.y = 0;
	cursorPos.w = 58;
	cursorPos.h = 52;

	//set the X Y W H for the rectangle
	activePos.x = 10;
	activePos.y = 10;
	activePos.w = 10;
	activePos.h = 10;

	//cursor speed
	//int cursorSpeed = 400;

	////////***********************CURSOR END

	//////*******************CREATE MENU START***************************************


	/////*****************************TITLE START**************************
	// create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "Title.png").c_str());

	//create an sdl texture
	SDL_Texture *title;

	//place the surface into the texture bkgd1
	title = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect TitlePos;

	//set the X Y W H for the rectangle
	TitlePos.x = 264;
	TitlePos.y = 30;
	TitlePos.w = 384;
	TitlePos.h = 163;

	////*********************** TITLE END

	///************************PLAYER ONE****************************
	surface = IMG_Load((images_dir + "play.png").c_str());

	//create an sdl texture
	SDL_Texture *oneN;

	//place the surface into the texture bkgd1
	oneN = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//over state
	surface = IMG_Load((images_dir + "playRED.png").c_str());

	//create an sdl texture
	SDL_Texture *oneO;

	//place the surface into the texture bkgd1
	oneO = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect oneNPos;

	//set the X Y W H for the rectangle
	oneNPos.x = 420;
	oneNPos.y = 261;
	oneNPos.w = 104;
	oneNPos.h = 80;

	////////**********************PLAYER ONE END



	////////************************INSTRUCTIONS****************************
	surface = IMG_Load((images_dir + "instructions.png").c_str());

	//create an sdl texture
	SDL_Texture *instructN;

	//place the surface into the texture bkgd1
	instructN = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//over state
	surface = IMG_Load((images_dir + "instructionsRED.png").c_str());

	//create an sdl texture
	SDL_Texture *instructO;

	//place the surface into the texture bkgd1
	instructO = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect instructNPos;

	//set the X Y W H for the rectangle
	instructNPos.x = 320;
	instructNPos.y = 424;
	instructNPos.w = 320;
	instructNPos.h = 68;

	//////////************************ISTRUCTIONS END************************

	////////************************BACK STORY TWO****************************
	surface = IMG_Load((images_dir + "backstory.png").c_str());

	//create an sdl texture
	SDL_Texture *backN;

	//place the surface into the texture bkgd1
	backN = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//over state
	surface = IMG_Load((images_dir + "backstoryRED.png").c_str());

	//create an sdl texture
	SDL_Texture *backO;

	//place the surface into the texture bkgd1
	backO = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect backNPos;

	//set the X Y W H for the rectangle
	backNPos.x = 350;
	backNPos.y = 343;
	backNPos.w = 252;
	backNPos.h = 71;

	//////////************************BACK STORY END************************


	////////************************QUIT START****************************
	surface = IMG_Load((images_dir + "quit.png").c_str());

	//create an sdl texture
	SDL_Texture *quitN;

	//place the surface into the texture bkgd1
	quitN = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//over state
	surface = IMG_Load((images_dir + "quitRED.png").c_str());

	//create an sdl texture
	SDL_Texture *quitO;

	//place the surface into the texture bkgd1
	quitO = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect quitNPos;

	//set the X Y W H for the rectangle
	quitNPos.x = 360;
	quitNPos.y = 503;
	quitNPos.w = 232;
	quitNPos.h = 71;

	//////////************************QUIT END************************



	////****************** MAIN MENU END*************************************



	/////*****************************win menu START**************************

	/////*************win graphics***********************
	// create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "win.png").c_str());

	//create an sdl texture
	SDL_Texture *winText;

	//place the surface into the texture bkgd1
	winText = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect winPos;

	//set the X Y W H for the rectangle
	winPos.x = 240;
	winPos.y = 288;
	winPos.w = 589;
	winPos.h = 159;
	/////////////*****************WIN END****************************


	/////*************PLAY AGAIN graphics***********************
	// create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "play again.png").c_str());

	//create an sdl texture
	SDL_Texture *playN;

	//place the surface into the texture bkgd1
	playN = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	// create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "play again RED.png").c_str());

	//create an sdl texture
	SDL_Texture *playO;

	//place the surface into the texture bkgd1
	playO = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect playNPos;

	//set the X Y W H for the rectangle
	playNPos.x = 700;
	playNPos.y = 680;
	playNPos.w = 297;
	playNPos.h = 75;
	////*********************** win menu end***********************************


	////////////***********LOSE MENU************************************

	/////*************LOSE graphics***********************
	// create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "Lose.png").c_str());

	//create an sdl texture
	SDL_Texture *loseText;

	//place the surface into the texture bkgd1
	loseText = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect losePos;

	//set the X Y W H for the rectangle
	losePos.x = 320;
	losePos.y = 288;
	losePos.w = 438;
	losePos.h = 75;
	/////////////*****************LOSE END****************************






	/////*****************************instructions menu START**************************

	/////*************instuctions graphics***********************
	// create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "Instructions.png").c_str());

	//create an sdl texture
	SDL_Texture *instuctionsText;

	//place the surface into the texture bkgd1
	instuctionsText = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect instuctionsTextPos;

	//set the X Y W H for the rectangle
	instuctionsTextPos.x = 120;
	instuctionsTextPos.y = 271;
	instuctionsTextPos.w = 781;
	instuctionsTextPos.h = 258;

	/////*************small menu graphics***********************
	// create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "menu.png").c_str());

	//create an sdl texture
	SDL_Texture *menuN;

	//place the surface into the texture bkgd1
	menuN = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	// create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "menuRED.png").c_str());

	//create an sdl texture
	SDL_Texture *menuO;

	//place the surface into the texture bkgd1
	menuO = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect menuPos;

	//set the X Y W H for the rectangle
	menuPos.x = 30;
	menuPos.y = 680;
	menuPos.w = 272;
	menuPos.h = 65;


	// **************** Set up the gamestates *****************
	enum GameState {MENU, PLAY, INSTRUCTIONS, BACKSTORY, WIN, LOSE, LEVEL2};

	//var enum to track where we are in game
	GameState gameState = WIN;

	//bool values to allow movement through the individual states
	bool menu, play, instructions, backstory, win , lose, level2;



	// ******************* ENTER THE PROGRAM LOOP ***************************
	while(!quit)
	{

		switch(gameState)
		{
		case MENU:

			menu = true;
			cout << "The Gamestate is Menu" << endl;
			cout << "Press the A Button for Play" << endl;
			cout << "Press the B Button for Instructions" << endl;
			cout << "Press the X Button for Backstory" << endl;
			cout << "Press the Y Button for quit" << endl;
			cout << endl;

			while(menu)
			{
				//create delta time for framerate independence
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime)/ 1000;
				lastTime = thisTime;

				//check for input
				if(SDL_PollEvent(&e))
				{
					//see if the player closes the window's X button
					if(e.type == SDL_QUIT)
					{
						quit = true;
						menu = false;
						break;
					}

					//test for controller input
					switch(e.type)
					{

					case SDL_CONTROLLERBUTTONDOWN:

						//checks to see if this is the controller 0
						if(e.cdevice.which == 0){
							//if A button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								//if the player choses 1 player game
								if(players1Over){
									//play the over sound plays fine though the levels, must pause/delay for quit
									Mix_PlayChannel(-1, pressedSound, 0);
									menu = false;
									gameState = PLAY;
									players1Over = false;
								}

								//if the player chooses 2 player game
								if(players2Over){
									Mix_PlayChannel(-1, pressedSound, 0);
									menu = false;
									gameState = INSTRUCTIONS;
									players2Over = false;
								}

								//if player chooses instructions
								if(instructionsOver){
									Mix_PlayChannel(-1, pressedSound, 0);
									menu = false;
									gameState = BACKSTORY;
									instructionsOver = false;
								}

								//if the player chooses to quuit the game
								if(quitOver){
									Mix_PlayChannel(-1, pressedSound, 0);
									//add slight delay
									SDL_Delay(200);
									menu = false;
									quit = true;
									quitOver = false;
								}
							}

							//if b button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{
								menu = false;
								gameState = INSTRUCTIONS;
							}

							//if x button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_X)
							{
								menu = false;
								gameState = BACKSTORY;
							}

							//if y button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
							{
								menu = false;
								quit = true;
							}
						}//end joystick check
						break;

					}// end button input check

				}//end menu poll event

				// ************* start the update process ********************
				UpdateBackground(deltaTime);
				UpdateCursor(deltaTime);

				//check for collision between cursor active state and buttons
				players1Over = SDL_HasIntersection(&activePos, &oneNPos);
				players2Over = SDL_HasIntersection(&activePos, &backNPos);
				instructionsOver = SDL_HasIntersection(&activePos, &instructNPos);
				quitOver = SDL_HasIntersection(&activePos, &quitNPos);


				//if the cursor is over a button, play the over sound
				if(players1Over || players2Over || instructionsOver || quitOver){
					if(alreadyOver == false){
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				//if the cursor is not over any button reset the alreadyOver var
				if(!players1Over && !players2Over && !instructionsOver && !quitOver){
					alreadyOver = false;
				}


				// ************************ START THE SDL DRAW PROCESS *****************
				//clear the SDL_Render target
				SDL_RenderClear(renderer);

				//draw the bkgd image
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				//draw the bkgd2 image
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				//draw the title image
				SDL_RenderCopy(renderer, title, NULL, &TitlePos);

				/////////*******************NEW******************************************************
				//draw the player 1 button
				if(players1Over){
					SDL_RenderCopy(renderer, oneN, NULL, &oneNPos);
				}else{
					SDL_RenderCopy(renderer, oneO, NULL, &oneNPos);
				}

				//draw the instructions button
				if(instructionsOver){
					SDL_RenderCopy(renderer, instructN, NULL, &instructNPos);
				}else{
					SDL_RenderCopy(renderer, instructO, NULL, &instructNPos);
				}

				//draw the back story 2 button
				if(players2Over){
					SDL_RenderCopy(renderer, backN, NULL, &backNPos);
				}else{
					SDL_RenderCopy(renderer, backO, NULL, &backNPos);
				}

				//draw the quit game button
				if(quitOver){
					SDL_RenderCopy(renderer, quitN, NULL, &quitNPos);
				}else{
					SDL_RenderCopy(renderer, quitO, NULL, &quitNPos);
				}


				//draw the cursor  image
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				//present new buffer to the screen
				SDL_RenderPresent(renderer);

			}// ends menu screen
			break;

		case INSTRUCTIONS:

			instructions = true;
			cout << "The Gamestate is Instructions" << endl;
			cout << "Press the A Button for Main Menu" << endl;
			cout << endl;

			while(instructions)
			{
				//check for input
				if(SDL_PollEvent(&e))
				{
					//see if the player closes the window's X button
					if(e.type == SDL_QUIT)
					{
						quit = true;
						instructions = false;
						break;
					}

					//test for controller input
					switch(e.type)
					{

					case SDL_CONTROLLERBUTTONDOWN:

						//checks to see if this is the controller 0
						if(e.cdevice.which == 0){
							//if A button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								instructions = false;
								gameState = MENU;
							}

						}//end joystick check
						break;

					}// end button input check
				}//end instructions poll event

				// ************************ START THE SDL DRAW PROCESS *****************
				//clear the SDL_Render target
				SDL_RenderClear(renderer);

				//draw the background


				//present new buffer to the screen
				SDL_RenderPresent(renderer);

			}// ends instructions screen
			break;


		case PLAY:

			play = true;
			cout << "The Gamestate is Play" << endl;
			cout << "Press the A Button for Win Screen" << endl;
			cout << "Press the B Button for Lose Screen" << endl;
			cout << endl;

			while(play)
			{
				//check for input
				if(SDL_PollEvent(&e))
				{
					//see if the player closes the window's X button
					if(e.type == SDL_QUIT)
					{
						quit = true;
						play = false;
						break;
					}

					//test for controller input
					switch(e.type)
					{

					case SDL_CONTROLLERBUTTONDOWN:

						//checks to see if this is the controller 0
						if(e.cdevice.which == 0){
							//if A button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								play = false;
								gameState = WIN;
							}

							//if b button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{
								play = false;
								gameState = LOSE;
							}
						}//end joystick check
						break;
					}// end button input check
				}//end play poll event

				// ************************ START THE SDL DRAW PROCESS *****************
				//clear the SDL_Render target
				SDL_RenderClear(renderer);

				//draw the background


				//present new buffer to the screen
				SDL_RenderPresent(renderer);
			}// ends play screen
			break;


		case BACKSTORY:

			backstory = true;
			cout << "The Gamestate is Backstory" << endl;
			cout << "Press the A Button for Main Menu" << endl;

			cout << endl;

			while(backstory)
			{
				//check for input
				if(SDL_PollEvent(&e))
				{
					//see if the player closes the window's X button
					if(e.type == SDL_QUIT)
					{
						quit = true;
						backstory = false;
						break;
					}

					//test for controller input
					switch(e.type)
					{

					case SDL_CONTROLLERBUTTONDOWN:

						//checks to see if this is the controller 0
						if(e.cdevice.which == 0){
							//if A button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								backstory = false;
								gameState = MENU;
							}

						}//end joystick check
						break;
					}// end button input check
				}//end backstory poll event

				// ************************ START THE SDL DRAW PROCESS *****************
				//clear the SDL_Render target
				SDL_RenderClear(renderer);

				//draw the background


				//present new buffer to the screen
				SDL_RenderPresent(renderer);
			}// ends backstory screen
			break;



		case WIN:
			alreadyOver = false;
			win = true;

			win = true;
			cout << "The Gamestate is Win Screen" << endl;
			cout << "Press the A Button for Main Menu" << endl;
			cout << "Press the B Button to Quit Game" << endl;

			cout << endl;

			while(win)
			{
				//set up the frame rate for the section or case
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				//check for input
				if(SDL_PollEvent(&e))
				{
					//see if the player closes the window's X button
					if(e.type == SDL_QUIT)
					{
						quit = true;
						win = false;
						break;
					}

					//test for controller input
					switch(e.type)
					{

					case SDL_CONTROLLERBUTTONDOWN:

						//checks to see if this is the controller 0
						if(e.cdevice.which == 0){
							//if A button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								//if player chooses main menu
								if(menuOver){
									Mix_PlayChannel(-1, pressedSound, 0);
									win = false;
									gameState = MENU;
									menuOver = false;
								}

								//if player chooses to play again
								if(playOver){
									Mix_PlayChannel(-1, pressedSound, 0);
									win = false;
									gameState = PLAY;
									playOver = false;
								}
							}
							//if A button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{
								win = false;
								quit = true;
							}

						}//end joystick check
						break;
					case SDL_CONTROLLERAXISMOTION:

						moveCursor(e.caxis);

						break;

					}// end button input check
				}//end win poll event


				//****************************UPDATE SECTION*******************************
				UpdateBackground(deltaTime);

				//curosr
				UpdateCursor(deltaTime);

				menuOver = SDL_HasIntersection(&activePos, &menuPos);
				playOver = SDL_HasIntersection(&activePos, &playNPos);



				//if the cursor is over a button, play the over sound
				if(menuOver || playOver){
					if(alreadyOver == false){
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				//if the cursor is not over any button reset the alreadyOver var
				if(!menuOver && !playOver){
					alreadyOver = false;
				}

				// ************************ START THE SDL DRAW PROCESS *****************
				//clear the SDL_Render target
				SDL_RenderClear(renderer);

				//draw the win text image
				SDL_RenderCopy(renderer, winText, NULL, &winPos);


				if(menuOver){
					SDL_RenderCopy(renderer, menuO, NULL, &menuPos);
				}else{
					SDL_RenderCopy(renderer, menuN, NULL, &menuPos);
				}


				if(playOver){
					SDL_RenderCopy(renderer, playO, NULL, &playNPos);
				}else{
					SDL_RenderCopy(renderer, playN, NULL, &playNPos);
				}

				//draw the cursor  cursor
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);


				//present new buffer to the screen
				SDL_RenderPresent(renderer);
			}// ends win screen
			break;


		case LOSE:

			alreadyOver = false;
			lose = true;

			cout << "The Gamestate is lose Screen" << endl;
			cout << "Press the A Button for Main Menu" << endl;
			cout << "Press the B Button to Quit Game" << endl;

			cout << endl;

			while(lose)
			{

				//set up the frame rate for the section or case
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				//check for input
				if(SDL_PollEvent(&e))
				{
					//see if the player closes the window's X button
					if(e.type == SDL_QUIT)
					{
						quit = true;
						lose = false;
						break;
					}

					//test for controller input
					switch(e.type)
					{

					case SDL_CONTROLLERBUTTONDOWN:

						//checks to see if this is the controller 0
						if(e.cdevice.which == 0){
							//if A button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								//if player chooses main menu
								if(menuOver){
									Mix_PlayChannel(-1, pressedSound, 0);
									win = false;
									gameState = MENU;
									menuOver = false;
								}

								//if player chooses to play again
								if(playOver){
									Mix_PlayChannel(-1, pressedSound, 0);
									win = false;
									gameState = PLAY;
									playOver = false;
								}
							}
							//if A button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{
								lose = false;
								quit = true;
							}

						}//end joystick check
						break;

					case SDL_CONTROLLERAXISMOTION:

						moveCursor(e.caxis);
						break;
					}// end button input check
				}//end lose poll event


				//****************************UPDATE SECTION*******************************
				UpdateBackground(deltaTime);

				//curosr
				UpdateCursor(deltaTime);

				menuOver = SDL_HasIntersection(&activePos, &menuPos);
				playOver = SDL_HasIntersection(&activePos, &playNPos);


				//if the cursor is over a button, play the over sound
				if(menuOver || playOver){
					if(alreadyOver == false){
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				//if the cursor is not over any button reset the alreadyOver var
				if(!menuOver && !playOver){
					alreadyOver = false;
				}

				// ************************ START THE SDL DRAW PROCESS *****************
				//clear the SDL_Render target
				SDL_RenderClear(renderer);

				//draw the win text image
				SDL_RenderCopy(renderer, loseText, NULL, &losePos);

				// ******************************* NEW ***********************************************
				if(menuOver){
					SDL_RenderCopy(renderer, menuO, NULL, &menuPos);
				}else{
					SDL_RenderCopy(renderer, menuN, NULL, &menuPos);
				}

				if(playOver){
					SDL_RenderCopy(renderer, playO, NULL, &playNPos);
				}else{
					SDL_RenderCopy(renderer, playN, NULL, &playNPos);
				}
				// ******************************* NEW ***********************************************

				//draw the cursor  image
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);


				//present new buffer to the screen
				SDL_RenderPresent(renderer);
			}// ends lose screen
			break;

		case LEVEL2:

			level2 = true;
			cout << "The Gamestate is Play" << endl;
			cout << "Press the A Button for Win Screen" << endl;
			cout << "Press the B Button for Lose Screen" << endl;
			cout << endl;

			while(level2)
			{
				//check for input
				if(SDL_PollEvent(&e))
				{
					//see if the player closes the window's X button
					if(e.type == SDL_QUIT)
					{
						quit = true;
						level2 = false;
						break;
					}

					//test for controller input
					switch(e.type)
					{

					case SDL_CONTROLLERBUTTONDOWN:

						//checks to see if this is the controller 0
						if(e.cdevice.which == 0){
							//if A button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								level2 = false;
								gameState = WIN;
							}

							//if b button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{
								level2 = false;
								gameState = LOSE;
							}
						}//end joystick check
						break;
					}// end button input check
				}//end play poll event

				// ************************ START THE SDL DRAW PROCESS *****************
				//clear the SDL_Render target
				SDL_RenderClear(renderer);

				//draw the background


				//present new buffer to the screen
				SDL_RenderPresent(renderer);
			}// ends play screen
			break;

		}// ends the  switch


	}//end main loop


	//close and destroy the winodw
	SDL_DestroyWindow(window);

	//clean up
	SDL_Quit();

	return 0;
}
