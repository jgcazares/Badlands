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


	SDL_Texture *bkgd = IMG_LoadTexture(renderer,(images_dir + "desert.png").c_str());

	SDL_Rect bkgdRect;

	int BkgdSpeed = 100;

	SDL_Rect BkgdPos1, BkgdPos2;

	float Bkgd1Pos_X = 0, Bkgd1Pos_Y = 0;
	float Bkgd2Pos_X = -1024, Bkgd2Pos_Y = 0;

	BkgdPos1.x = 0;
	BkgdPos1.y = 0;
	BkgdPos1.w = 1024;
	BkgdPos1.h = 768;

	BkgdPos2.x = -1024;
	BkgdPos2.y = 0;
	BkgdPos2.w = 1024;
	BkgdPos2.h = 768;

	bkgdRect.x = 0;

	bkgdRect.y = 0;

	bkgdRect.w = 1024;

	bkgdRect.h = 768;


	// **************** Set up the gamestates *****************
	enum GameState {MENU, PLAY, INSTRUCTIONS, BACKSTORY, WIN, LOSE};

	//var enum to track where we are in game
	GameState gameState = MENU;

	//bool values to allow movement through the individual states
	bool menu, play, instructions, backstory, win , lose;



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
								menu = false;
								gameState = PLAY;
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

				// ********** UPDATE THE BACK GROUND **********************
				Bkgd1Pos_X += (BkgdSpeed * 1) * deltaTime;
				BkgdPos1.x = (int)(Bkgd1Pos_X + 0.5f);

				//reset when at the edge of the screen
				if(BkgdPos1.x >= 1024){
					BkgdPos1.x = -1024;
					Bkgd1Pos_X = BkgdPos1.x;
				}



				// ************************ START THE SDL DRAW PROCESS *****************
					//clear the SDL_Render target
					SDL_RenderClear(renderer);

					//draw the background
					SDL_RenderCopy(renderer, bkgd, NULL, &bkgdRect);

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
									SDL_RenderCopy(renderer, bkgd, NULL, &bkgdRect);

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
									SDL_RenderCopy(renderer, bkgd, NULL, &bkgdRect);

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
									SDL_RenderCopy(renderer, bkgd, NULL, &bkgdRect);

									//present new buffer to the screen
									SDL_RenderPresent(renderer);
			}// ends backstory screen
			break;



		case WIN:

			win = true;
			cout << "The Gamestate is Win Screen" << endl;
			cout << "Press the A Button for Main Menu" << endl;
			cout << "Press the B Button to Quit Game" << endl;

			cout << endl;

			while(win)
			{
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
								win = false;
								gameState = MENU;
							}
							//if A button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{
								win = false;
								quit = true;
							}

						}//end joystick check
						break;
					}// end button input check
				}//end win poll event

				// ************************ START THE SDL DRAW PROCESS *****************
									//clear the SDL_Render target
									SDL_RenderClear(renderer);

									//draw the background
									SDL_RenderCopy(renderer, bkgd, NULL, &bkgdRect);

									//present new buffer to the screen
									SDL_RenderPresent(renderer);
			}// ends win screen
			break;


		case LOSE:

			lose = true;
			cout << "The Gamestate is lose Screen" << endl;
			cout << "Press the A Button for Main Menu" << endl;
			cout << "Press the B Button to Quit Game" << endl;

			cout << endl;

			while(lose)
			{
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
								lose = false;
								gameState = MENU;
							}
							//if A button
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{
								lose = false;
								quit = true;
							}

						}//end joystick check
						break;
					}// end button input check
				}//end lose poll event

				// ************************ START THE SDL DRAW PROCESS *****************
									//clear the SDL_Render target
									SDL_RenderClear(renderer);

									//draw the background
									SDL_RenderCopy(renderer, bkgd, NULL, &bkgdRect);

									//present new buffer to the screen
									SDL_RenderPresent(renderer);
			}// ends lose screen
			break;



		}// ends the  switch


	}//end main loop


			//close and destroy the winodw
			SDL_DestroyWindow(window);

			//clean up
			SDL_Quit();

	return 0;
}
