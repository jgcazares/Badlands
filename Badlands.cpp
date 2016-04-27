#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdint.h>
#include <sstream>
using namespace std;

//screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

//delta time var inititalization
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

int bulletNum;

//bool e1K = false, e2K = false, e3K = false, e4K = false;


#include "time.h"
#include "player.h"
#include "turret.h"
#include "bandit.h"
#include "poweder.h"
#include "explosion.h"

#if defined (__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <unistd.h>

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
string images_dir = currentWorkingDirectory + "\\Badlands\\";
string audio_dir = currentWorkingDirectory + "\\Badlands\\";
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
SDL_Rect lose1Pos, lose2Pos;

//background floats
float b1pos_x = 0, b1pos_y = 0;
float b2pos_x = -1024, b2pos_y = 0;

//background floats
float l1pos_x = 0, l1pos_y = 0;
float l2pos_x = 0, l2pos_y = -768;


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

//MOVE THE BAKCGROUND
void UpdateLose(float deltaTime) {

	//Update the bkgd 1
	l1pos_y += (bkgdSpeed * 1) * deltaTime;

	//set the new bkgd10 poition
	lose1Pos.y = (int)(l1pos_y + 0.5f);

	//reset when off the bottom of the screen
	if (lose1Pos.y >= 768)
	{

		lose1Pos.y = -768;
		l1pos_y = lose1Pos.y;


	}


	//Update the bkgd 2
	l2pos_y += (bkgdSpeed * 1) * deltaTime;

	//set the new bkgd10 poition
	lose2Pos.y = (int)(l2pos_y + 0.5f);

	//reset when off the bottom of the screen
	if (lose2Pos.y >= 768)
	{

		lose1Pos.y = -768;
		l2pos_y = lose2Pos.y;


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
		quitOver = false, menuOver = false, playOver = false, startOver = false;

vector<Explode> explodeList;








int main(int argc, char* argv[]) {

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

	//set up a sound efect chunk for the button over state
	Mix_Chunk *overSound = Mix_LoadWAV((audio_dir + "over.wav").c_str());

	//set up a sound effect of the button pressed state
	Mix_Chunk *pressedSound =Mix_LoadWAV((audio_dir + "pressed.wav").c_str());

	//set up a sound effect of the button pressed state
	Mix_Chunk *winSound =Mix_LoadWAV((audio_dir + "winM.wav").c_str());

	//set up a sound effect of the button pressed state
	Mix_Chunk *pickUpSound =Mix_LoadWAV((audio_dir + "pickUp.wav").c_str());

	//load music file
	Mix_Music *menuM = Mix_LoadMUS((audio_dir + "battle.wav").c_str());

	//if the music file is not playing play it
	if(!Mix_PlayingMusic())
		Mix_PlayMusic(menuM, -1);

	//bool value to control the over sound effect
	bool alreadyOver = false;


	//***********CREATE BACKGROUND************
	// create a SDL surface to hold the background image
	SDL_Surface *surface = IMG_Load((images_dir + "desert2.png").c_str());

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

	//////***********************************CREATE lose END *******************

	// create a SDL surface to hold the background image
	SDL_Surface *loseS = IMG_Load((images_dir + "loseScreen.png").c_str());

	//create an sdl texture
	SDL_Texture *ls1;

	//place the surface into the texture bkgd1
	ls1 = SDL_CreateTextureFromSurface(renderer, loseS);

	//create an sdl texture
	SDL_Texture *ls2;

	//place the surface into the texture bkgd1
	ls2 = SDL_CreateTextureFromSurface(renderer, loseS);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//set the X Y W H for the rectangle
	lose1Pos.x = 0;
	lose1Pos.y = 0;
	lose1Pos.w = 1024;
	lose1Pos.h = 768;



	//set the X Y W H for the rectangle
	lose2Pos.x = 0;
	lose2Pos.y = -768;
	lose2Pos.w = 1024;
	lose2Pos.h = 768;

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
	SDL_Rect instructNPos2;


	//set the X Y W H for the rectangle
	instructNPos.x = 320;
	instructNPos.y = 424;
	instructNPos.w = 320;
	instructNPos.h = 68;

	//set the X Y W H for the rectangle
	instructNPos2.x = 700;
	instructNPos2.y = 20;
	instructNPos2.w = 320;
	instructNPos2.h = 68;

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

	//create sdl rect for title
	SDL_Rect backNPos2;

	//set the X Y W H for the rectangle
	backNPos2.x = 700;
	backNPos2.y = 20;
	backNPos2.w = 252;
	backNPos2.h = 71;

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
	playNPos.y = 20;
	playNPos.w = 297;
	playNPos.h = 75;
	////*********************** win menu end***********************************

	/////*************Start Gmae graphics***********************
	// create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "start gameO.png").c_str());

	//create an sdl texture
	SDL_Texture *stN;

	//place the surface into the texture bkgd1
	stN = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	// create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "start game.png").c_str());

	//create an sdl texture
	SDL_Texture *stO;

	//place the surface into the texture bkgd1
	stO = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect stNPos;

	//set the X Y W H for the rectangle
	stNPos.x = 350;
	stNPos.y = 20;
	stNPos.w = 297;
	stNPos.h = 75;
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
	menuPos.y = 20;
	menuPos.w = 272;
	menuPos.h = 65;

	// ******************** LEVEL 1 BACKGROUND *********************
	// create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "level1.png").c_str());

	//create an sdl texture
	SDL_Texture *level1;

	//place the surface into the texture bkgd1
	level1 = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect level1Pos;

	//set the X Y W H for the rectangle
	level1Pos.x = 0;
	level1Pos.y = 0;
	level1Pos.w = 1024;
	level1Pos.h = 768;
	// ******************** level 1 background *********************


	// ******************** LEVEL 2 BACKGROUND *********************
	// create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "level2.png").c_str());

	//create an sdl texture
	SDL_Texture *level2;

	//place the surface into the texture bkgd1
	level2 = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect level2Pos;

	//set the X Y W H for the rectangle
	level2Pos.x = 0;
	level2Pos.y = 0;
	level2Pos.w = 2048;
	level2Pos.h = 768;
	// ******************** level 2 background *********************

	// ******************** Backstory BACKGROUND *********************
	// create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "backBkgd.png").c_str());

	//create an sdl texture
	SDL_Texture *backStory;

	//place the surface into the texture bkgd1
	backStory = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect backSPos;

	//set the X Y W H for the rectangle
	backSPos.x = 50;
	backSPos.y = 100;
	backSPos.w = 944;
	backSPos.h = 568;
	// ******************** level 2 background *********************



	// ******************** instructions BACKGROUND *********************
	// create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "inBkgd.png").c_str());

	//create an sdl texture
	SDL_Texture *insB;

	//place the surface into the texture bkgd1
	insB = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect insBPos;

	//set the X Y W H for the rectangle
	insBPos.x = 100;
	insBPos.y = 300;
	insBPos.w = 869;
	insBPos.h = 315;
	// ******************** level 2 background *********************


	// ******************** Win BACKGROUND *********************
	// create a SDL surface to hold the background image
	surface = IMG_Load((images_dir + "winBkgd.png").c_str());

	//create an sdl texture
	SDL_Texture *winB;

	//place the surface into the texture bkgd1
	winB = SDL_CreateTextureFromSurface(renderer, surface);

	//free the sdl surface
	SDL_FreeSurface(surface);

	//create sdl rect for title
	SDL_Rect winBPos;

	//set the X Y W H for the rectangle
	winBPos.x = 0;
	winBPos.y = 0;
	winBPos.w = 1024;
	winBPos.h = 768;
	// ******************** level 2 background *********************




	// **************** Set up the gamestates *****************
	enum GameState {MENU, PLAY, INSTRUCTIONS, BACKSTORY, WIN, LOSE, LEVEL2};

	//var enum to track where we are in game
	GameState gameState = MENU;

	//bool values to allow movement through the individual states
	bool menu, play, instructions, backstory, win , lose, Level2;


	


	Player player1 = Player(renderer, 0, images_dir.c_str(), audio_dir.c_str(), 100.0, 350.0);
	Player player2 = Player(renderer, 0, images_dir.c_str(), audio_dir.c_str(), 100.0, 350.0);


	Turret turret1 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 900.0f, 100.0f);
	Turret turret2 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 900.0f, 250.0f);
	Turret turret3 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 900.0f, 500.0f);
	Turret turret4 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 900.0f, 650.0f);

	Turret turret5 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 1800.0f, 100.0f);
	Turret turret6 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 1800.0f, 250.0f);
	Turret turret7 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 1800.0f, 500.0f);
	Turret turret8 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 1800.0f, 650.0f);

	Bandit bandit1 = Bandit(renderer, images_dir.c_str(), audio_dir.c_str(), 500.0f, 200.0f);
	Bandit bandit2 = Bandit(renderer, images_dir.c_str(), audio_dir.c_str(), 500.0f, 250.0f);
	Bandit bandit3 = Bandit(renderer, images_dir.c_str(), audio_dir.c_str(), 500.0f, 400.0f);
	Bandit bandit4 = Bandit(renderer, images_dir.c_str(), audio_dir.c_str(), 500.0f, 550.0f);

	Bandit bandit5 = Bandit(renderer, images_dir.c_str(), audio_dir.c_str(), 1600.0f, 550.0f);

	
	Explode ex1(renderer, images_dir, 810, 70);
	Explode ex2(renderer, images_dir, 810, 220);
	Explode ex3(renderer, images_dir, 810, 470);
	Explode ex4(renderer, images_dir, 810, 620);

	
	Explode ex5(renderer, images_dir, 1800, 70);
	Explode ex6(renderer, images_dir, 1800, 220);
	Explode ex7(renderer, images_dir, 1800, 470);
	Explode ex8(renderer, images_dir, 1800, 620);
	


	float X_pos = 0.0f;

	float Y_pos = 0.0f;


	// jewel HUD
	SDL_Texture *bagbkgd = IMG_LoadTexture(renderer, (images_dir + "bagHud.png").c_str());
	SDL_Rect bagbkgdRect;
	bagbkgdRect.x = 370;
	bagbkgdRect.y = 10;
	bagbkgdRect.w = 284;
	bagbkgdRect.h = 91;


	SDL_Texture *bag1 = IMG_LoadTexture(renderer, (images_dir + "bag1.png").c_str());
	SDL_Rect bag1Pos;
	bag1Pos.x = 370;
	bag1Pos.y = 10;
	bag1Pos.w = 284;
	bag1Pos.h = 91;

	SDL_Texture *bag2 = IMG_LoadTexture(renderer, (images_dir + "bag2.png").c_str());
	SDL_Rect bag2Pos;
	bag2Pos.x = 370;
	bag2Pos.y = 10;
	bag2Pos.w = 284;
	bag2Pos.h = 91;

	SDL_Texture *bag3 = IMG_LoadTexture(renderer, (images_dir + "bag3.png").c_str());
	SDL_Rect bag3Pos;
	bag3Pos.x = 370;
	bag3Pos.y = 10;
	bag3Pos.w = 284;
	bag3Pos.h = 91;

	SDL_Texture *bag4 = IMG_LoadTexture(renderer, (images_dir + "bag4.png").c_str());
	SDL_Rect bag4Pos;
	bag4Pos.x = 370;
	bag4Pos.y = 10;
	bag4Pos.w = 284;
	bag4Pos.h = 91;

	bool haveBag1 = false;
	bool haveBag2 = false;
	bool haveBag3 = false;
	bool haveBag4 = false;

	Powder powder1 = Powder(renderer, images_dir.c_str(), 0, 900.0f, 100.0f);
	Powder powder2 = Powder(renderer, images_dir.c_str(), 1, 900.0f, 250.0f);
	Powder powder3 = Powder(renderer, images_dir.c_str(), 2, 900.0f, 500.0f);
	Powder powder4 = Powder(renderer, images_dir.c_str(), 3, 900.0f, 650.0f);





	//Fuel Stuff
	Powder fuel1 = Powder(renderer, images_dir.c_str(), 4, 1800.0f, 100.0f);
	Powder fuel2 = Powder(renderer, images_dir.c_str(), 4, 1800.0f, 250.0f);
	Powder fuel3 = Powder(renderer, images_dir.c_str(), 4, 1800.0f, 500.0f);
	Powder fuel4 = Powder(renderer, images_dir.c_str(), 4, 1800.0f, 650.0f);

	SDL_Texture *fuelB = IMG_LoadTexture(renderer, (images_dir + "fuelHUD.png").c_str());
	SDL_Texture *fuelM = IMG_LoadTexture(renderer, (images_dir + "movingBar.png").c_str());
	

	SDL_Rect fuelRect;
	fuelRect.x = 10;
	fuelRect.y = 600;
	fuelRect.w = 238;
	fuelRect.h = 79;

	SDL_Rect movingRect;
	movingRect.x = 71;
	movingRect.y = 643;
	movingRect.w = 173;
	movingRect.h = 29;

	//Fuel INFO
	float currentFuel = 0.0f;
	float maxFuel = 100.0f;

	int fuelCount = 0;


	// jewel HUD
	SDL_Texture *bHud = IMG_LoadTexture(renderer, (images_dir + "bulletHud.png").c_str());
	SDL_Rect bHudRect;
	bHudRect.x = 10;
	bHudRect.y = 650;
	bHudRect.w = 194;
	bHudRect.h = 90;

	

	SDL_Texture *b1 = IMG_LoadTexture(renderer, (images_dir + "bullet1.png").c_str());
	SDL_Rect b1Rect;
	b1Rect.x = 10;
	b1Rect.y = 650;
	b1Rect.w = 194;
	b1Rect.h = 90;

	SDL_Texture *b2 = IMG_LoadTexture(renderer, (images_dir + "bullet2.png").c_str());
	SDL_Rect b2Rect;
	b2Rect.x = 10;
	b2Rect.y = 650;
	b2Rect.w = 194;
	b2Rect.h = 90;

	SDL_Texture *b3 = IMG_LoadTexture(renderer, (images_dir + "bullet3.png").c_str());
	SDL_Rect b3Rect;
	b3Rect.x = 10;
	b3Rect.y = 650;
	b3Rect.w = 194;
	b3Rect.h = 90;

	SDL_Texture *b4 = IMG_LoadTexture(renderer, (images_dir + "bullet4.png").c_str());
	SDL_Rect b4Rect;
	b4Rect.x = 10;
	b4Rect.y = 650;
	b4Rect.w = 194;
	b4Rect.h = 90;


	SDL_Texture *b5 = IMG_LoadTexture(renderer, (images_dir + "bullet5.png").c_str());
	SDL_Rect b5Rect;
	b5Rect.x = 10;
	b5Rect.y = 650;
	b5Rect.w = 194;
	b5Rect.h = 90;

	SDL_Texture *b6 = IMG_LoadTexture(renderer, (images_dir + "bullet6.png").c_str());
	SDL_Rect b6Rect;
	b6Rect.x = 10;
	b6Rect.y = 650;
	b6Rect.w = 194;
	b6Rect.h = 90;

	SDL_Texture *b7 = IMG_LoadTexture(renderer, (images_dir + "bullet7.png").c_str());
	SDL_Rect b7Rect;
	b7Rect.x = 10;
	b7Rect.y = 650;
	b7Rect.w = 194;
	b7Rect.h = 90;

	SDL_Texture *b8 = IMG_LoadTexture(renderer, (images_dir + "bullet8.png").c_str());
	SDL_Rect b8Rect;
	b8Rect.x = 10;
	b8Rect.y = 650;
	b8Rect.w = 194;
	b8Rect.h = 90;

	SDL_Texture *b9 = IMG_LoadTexture(renderer, (images_dir + "bullet9.png").c_str());
	SDL_Rect b9Rect;
	b9Rect.x = 10;
	b9Rect.y = 650;
	b9Rect.w = 194;
	b9Rect.h = 90;

	SDL_Texture *b10 = IMG_LoadTexture(renderer, (images_dir + "bullet10.png").c_str());
	SDL_Rect b10Rect;
	b10Rect.x = 10;
	b10Rect.y = 650;
	b10Rect.w = 194;
	b10Rect.h = 90;

	SDL_Texture *b11 = IMG_LoadTexture(renderer, (images_dir + "bullet11.png").c_str());
	SDL_Rect b11Rect;
	b11Rect.x = 10;
	b11Rect.y = 650;
	b11Rect.w = 194;
	b11Rect.h = 90;

	SDL_Texture *b12 = IMG_LoadTexture(renderer, (images_dir + "bullet12.png").c_str());
	SDL_Rect b12Rect;
	b12Rect.x = 10;
	b12Rect.y = 650;
	b12Rect.w = 194;
	b12Rect.h = 90;

	/*SDL_Texture *b[13] = {bHud, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12};*/


	// ******************* ENTER THE PROGRAM LOOP ***************************
	while(!quit)
	{

		switch(gameState)
		{
		case MENU:

			menu = true;


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
									gameState = BACKSTORY;
									players2Over = false;
								}

								//if player chooses instructions
								if(instructionsOver){
									Mix_PlayChannel(-1, pressedSound, 0);
									menu = false;
									gameState = INSTRUCTIONS;
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

						}//end joystick check
						break;

					case SDL_CONTROLLERAXISMOTION:

						moveCursor(e.caxis);
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

		case PLAY:

			play = true;

			currentFuel = 0.0f;

			bulletNum = 12;

			player1.Reset();

			ex1.active = false;
			ex2.active = false;
			ex3.active = false;
			ex4.active = false;

			while(play)
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
								player1.OnControllerButton(e.cbutton);
								break;
							}


						}//end joystick check
						break;
					}// end button input check
				}//end play poll event

				const Sint16 Xvalue = SDL_GameControllerGetAxis(gGameController0, SDL_CONTROLLER_AXIS_LEFTX);
				const Sint16 Yvalue = SDL_GameControllerGetAxis(gGameController0, SDL_CONTROLLER_AXIS_LEFTY);

				//pass to player 1
				player1.OnControllerAxis(Xvalue,Yvalue);

				// ************************** UPDATES *******************************
				UpdateBackground(deltaTime);
				if (player1.active = true) {
					player1.Update(deltaTime);
				}


				turret1.Update(deltaTime, player1.posRect);
				turret2.Update(deltaTime, player1.posRect);
				turret3.Update(deltaTime, player1.posRect);
				turret4.Update(deltaTime, player1.posRect);

				//update the enemy
				bandit1.Update(deltaTime, player1.posRect);
				bandit2.Update(deltaTime, player1.posRect);
				bandit3.Update(deltaTime, player1.posRect);
				bandit4.Update(deltaTime, player1.posRect);


				//check for hit from turre1
				for(int i = 0; i < turret1.bulletList.size(); i++)
				{
					if(SDL_HasIntersection(&player1.posRect, &turret1.bulletList[i].posRect)){
						turret1.bulletList[i].Reset();
						player1.eBulletHit();
						break;
					}
				}

				//check for hit from turre2
				for(int i = 0; i < turret2.bulletList.size(); i++)
				{
					if(SDL_HasIntersection(&player1.posRect, &turret2.bulletList[i].posRect)){
						turret2.bulletList[i].Reset();
						player1.eBulletHit();
						break;
					}
				}

				//check for hit from turre3
				for(int i = 0; i < turret3.bulletList.size(); i++)
				{
					if(SDL_HasIntersection(&player1.posRect, &turret3.bulletList[i].posRect)){
						turret3.bulletList[i].Reset();
						player1.eBulletHit();
						break;
					}
				}

				//check for hit from turre4
				for(int i = 0; i < turret4.bulletList.size(); i++)
				{
					if(SDL_HasIntersection(&player1.posRect, &turret4.bulletList[i].posRect)){
						turret4.bulletList[i].Reset();
						player1.eBulletHit();
						break;
					}
				}



				//check if the player damages an enemy
				for(int i = 0; i < player1.bulletList.size(); i++)
				{
					//turret 1
					if(SDL_HasIntersection(&turret1.baseRect, &player1.bulletList[i].posRect)){
						player1.bulletList[i].Reset();
						if(turret1.active == true){
							turret1.RemoveHealth();
							ex1.active = true;
							
						}
						
						break;
					}

					//bandit 1
					if(SDL_HasIntersection(&bandit1.banditRect, &player1.bulletList[i].posRect)){
						player1.bulletList[i].Reset();
						if(bandit1.active == true){
							bandit1.RemoveHealth();
						}
						break;
					}

					//turret 2
					if(SDL_HasIntersection(&turret2.baseRect, &player1.bulletList[i].posRect)){
						player1.bulletList[i].Reset();
						if(turret2.active == true){
							turret2.RemoveHealth();
							ex2.active = true;
						}
						break;
					}

					//bandit 2
					if(SDL_HasIntersection(&bandit2.banditRect, &player1.bulletList[i].posRect)){
						player1.bulletList[i].Reset();
						if(bandit2.active == true){
							bandit2.RemoveHealth();
						}
						break;
					}

					//turret 3
					if(SDL_HasIntersection(&turret3.baseRect, &player1.bulletList[i].posRect)){
						player1.bulletList[i].Reset();
						if(turret3.active == true){
							turret3.RemoveHealth();
							ex3.active = true;
						}
						break;
					}

					//bandit 3
					if(SDL_HasIntersection(&bandit3.banditRect, &player1.bulletList[i].posRect)){
						player1.bulletList[i].Reset();
						if(bandit3.active == true){
							bandit3.RemoveHealth();
						}
						break;
					}


					//turret 4
					if(SDL_HasIntersection(&turret4.baseRect, &player1.bulletList[i].posRect)){
						player1.bulletList[i].Reset();
						if(turret4.active == true){
							turret4.RemoveHealth();
							ex4.active = true;
						}
						break;
					}

					//bandit 4
					if(SDL_HasIntersection(&bandit4.banditRect, &player1.bulletList[i].posRect)){
						player1.bulletList[i].Reset();
						if(bandit4.active == true){
							bandit4.RemoveHealth();
						}
						break;
					}


				}





				//check to see if the player has been hit by the enemy tank
				if(SDL_HasIntersection(&player1.posRect, &bandit1.banditRect)){
					player1.enemyHit();
				}

				//check to see if the player has been hit by the enemy tank
				if(SDL_HasIntersection(&player1.posRect, &bandit2.banditRect)){
					player1.enemyHit();
				}

				//check to see if the player has been hit by the enemy tank
				if(SDL_HasIntersection(&player1.posRect, &bandit3.banditRect)){
					player1.enemyHit();
				}

				//check to see if the player has been hit by the enemy tank
				if(SDL_HasIntersection(&player1.posRect, &bandit4.banditRect)){
					player1.enemyHit();
				}


				//check to see if the player has hit the purple jewel
				if (SDL_HasIntersection(&player1.posRect, &powder1.powderRect)) {
					haveBag1 = true;
					powder1.active = false;
					powder1.powderRect.x = -5000;
					powder1.powderRect.y = -5000;
					Mix_PlayChannel(-1, pickUpSound, 0);
				}

				//check to see if the player has hit the red jewel
				if (SDL_HasIntersection(&player1.posRect, &powder2.powderRect)) {
					haveBag2 = true;
					powder2.active = false;
					powder2.powderRect.x = -5000;
					powder2.powderRect.y = -5000;
					Mix_PlayChannel(-1, pickUpSound, 0);
				}

				//check to see if the player has hit the red jewel
				if (SDL_HasIntersection(&player1.posRect, &powder3.powderRect)) {
					haveBag3 = true;
					powder3.active = false;
					powder3.powderRect.x = -5000;
					powder3.powderRect.y = -5000;
					Mix_PlayChannel(-1, pickUpSound, 0);
				}

				//check to see if the player has hit the red jewel
				if (SDL_HasIntersection(&player1.posRect, &powder4.powderRect)) {
					haveBag4 = true;
					powder4.active = false;
					powder4.powderRect.x = -5000;
					powder4.powderRect.y = -5000;
					Mix_PlayChannel(-1, pickUpSound, 0);
				}




				if(player1.playerHealth <= 0){
					play = false;
					gameState = LOSE;
				}

				if (haveBag1 == true && haveBag2 == true && haveBag3 == true && haveBag4 == true) {
					play = false;
					gameState = LEVEL2;
				}

				
				if (ex1.active = true && turret1.e1K == true) {
					ex1.Update(deltaTime);
				}
				if (ex2.active = true && turret2.e1K == true) {
					ex2.Update(deltaTime);
				}
				if (ex3.active = true && turret3.e1K == true) {
					ex3.Update(deltaTime);
				}
				if (ex4.active = true && turret4.e1K == true) {
					ex4.Update(deltaTime);
				}


				// ************************ START THE SDL DRAW PROCESS *****************
				//clear the SDL_Render target
				SDL_RenderClear(renderer);

				//draw the bkgd image
				SDL_RenderCopy(renderer, level1, NULL, &level1Pos);

				SDL_RenderCopy(renderer, bagbkgd, NULL, &bagbkgdRect);

				///////  BULLET HUD GRAPHICS //////////////////////////////////////////////
				//if (bulletNum = 12) {
				//	SDL_RenderCopy(renderer, b12, NULL, &b12Rect);
				//}
				//else if (bulletNum = 11) {
				//	SDL_RenderCopy(renderer, b11, NULL, &b11Rect);
				//}
				//else if(bulletNum = 10) {
				//	SDL_RenderCopy(renderer, b10, NULL, &b10Rect);
				//}
				//else if(bulletNum = 9) {
				//	SDL_RenderCopy(renderer, b9, NULL, &b9Rect);
				//}
				//else if(bulletNum = 8) {
				//	SDL_RenderCopy(renderer, b8, NULL, &b8Rect);
				//}
				//else if(bulletNum = 7) {
				//	SDL_RenderCopy(renderer, b7, NULL, &b7Rect);
				//}
				//else if(bulletNum = 6) {
				//	SDL_RenderCopy(renderer, b6, NULL, &b6Rect);
				//}
				//else if(bulletNum = 5) {
				//	SDL_RenderCopy(renderer, b5, NULL, &b5Rect);
				//}
				//else if(bulletNum = 4) {
				//	SDL_RenderCopy(renderer, b4, NULL, &b4Rect);
				//}
				//else if(bulletNum = 3) {
				//	SDL_RenderCopy(renderer, b3, NULL, &b3Rect);
				//}
				//else if(bulletNum = 2) {
				//	SDL_RenderCopy(renderer, b2, NULL, &b2Rect);
				//}
				//else if(bulletNum = 1) {
				//	SDL_RenderCopy(renderer, b1, NULL, &b1Rect);
				//}
				//else if(bulletNum = 0) {
				//	SDL_RenderCopy(renderer, bHud, NULL, &bHudRect);
				//}

				//draw the pruple UI
				if (haveBag1)
					SDL_RenderCopy(renderer, bag1, NULL, &bag1Pos);

				//draw the red UI
				if (haveBag2)
					SDL_RenderCopy(renderer, bag2, NULL, &bag2Pos);

				//draw the blue UI
				if (haveBag3)
					SDL_RenderCopy(renderer, bag3, NULL, &bag3Pos);

				//draw the blue UI
				if (haveBag4)
					SDL_RenderCopy(renderer, bag4, NULL, &bag4Pos);

				player1.Draw(renderer);


				if (powder1.active)
					powder1.Draw(renderer);
				if (powder2.active)
					powder2.Draw(renderer);
				if (powder3.active)
					powder3.Draw(renderer);
				if (powder4.active)
					powder4.Draw(renderer);



				//draw the turret 1
				turret1.Draw(renderer);
				turret2.Draw(renderer);
				turret3.Draw(renderer);
				turret4.Draw(renderer);

				bandit1.Draw(renderer);
				bandit2.Draw(renderer);
				bandit3.Draw(renderer);
				bandit4.Draw(renderer);


			
				if (ex1.active = true && turret1.e1K == true) {
					ex1.Draw(renderer);
				}
				if (ex2.active = true && turret2.e1K == true) {
					ex2.Draw(renderer);
				}
				if (ex3.active = true && turret3.e1K == true) {
					ex3.Draw(renderer);
				}
				if (ex4.active = true && turret4.e1K == true) {
					ex4.Draw(renderer);
				}
				//present new buffer to the screen
				SDL_RenderPresent(renderer);
			}// ends play screen
			break;



			case LEVEL2:

			Level2 = true;

			player2.Reset();

			ex5.active = false;
			ex6.active = false;
			ex7.active = false;
			ex8.active = false;

			while(Level2)
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
						Level2 = false;
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
								player2.OnControllerButton(e.cbutton);
								break;
							}


						}//end joystick check
						break;
					}// end button input check
				}//end play poll event

				const Sint16 Xvalue = SDL_GameControllerGetAxis(gGameController0, SDL_CONTROLLER_AXIS_LEFTX);
				const Sint16 Yvalue = SDL_GameControllerGetAxis(gGameController0, SDL_CONTROLLER_AXIS_LEFTY);

				//pass to player 1
				player2.OnControllerAxis(Xvalue,Yvalue);

				// ************************** UPDATES *******************************
				UpdateBackground(deltaTime);
				player2.Update(deltaTime);

				//move the background left and right
				if ((player2.posRect.x >= 1024 - player2.posRect.w) && (player2.Xvalue > 8000)) {

					//adjust the position floats based on speed direction and deltatime
					X_pos -= (player2.speed) * deltaTime;
					if ((level2Pos.x > -1024)) {
						level2Pos.x = (int)(X_pos + 0.5f);
						
						//move the turret
						if (turret5.active = true) {
							turret5.TankMoveX(-player2.speed, deltaTime);
						}
						if (turret6.active = true) {
							turret6.TankMoveX(-player2.speed, deltaTime);
						}
						if (turret7.active = true) {
							turret7.TankMoveX(-player2.speed, deltaTime);
						}
						if (turret8.active = true) {
							turret8.TankMoveX(-player2.speed, deltaTime);
						}

						//boss tanks
						bandit5.eTankMoveX(-player2.speed, deltaTime);
						


						fuel1.TankMoveX(-player2.speed, deltaTime);
						fuel2.TankMoveX(-player2.speed, deltaTime);
						fuel3.TankMoveX(-player2.speed, deltaTime);
						fuel4.TankMoveX(-player2.speed, deltaTime);
					}
					else {
						X_pos = level2Pos.x;
					}
				}

				if ((player2.posRect.x <= 0) && (player2.Xvalue < 8000)) {
					X_pos += (player2.speed) * deltaTime;
					if ((level2Pos.x < 0)) {
						level2Pos.x = (int)(X_pos + 0.5f);
						//move the turret
						turret5.TankMoveX(player2.speed, deltaTime);
						turret6.TankMoveX(player2.speed, deltaTime);
						turret7.TankMoveX(player2.speed, deltaTime);
						turret8.TankMoveX(player2.speed, deltaTime);

						//enemy tanks
						bandit5.eTankMoveX(player2.speed, deltaTime);
					

						fuel1.TankMoveX(player2.speed, deltaTime);
						fuel2.TankMoveX(player2.speed, deltaTime);
						fuel3.TankMoveX(player2.speed, deltaTime);
						fuel4.TankMoveX(player2.speed, deltaTime);
					}
					else {
						X_pos = level2Pos.x;
					}
				}



				turret5.Update(deltaTime, player2.posRect);
				turret6.Update(deltaTime, player2.posRect);
				turret7.Update(deltaTime, player2.posRect);
				turret8.Update(deltaTime, player2.posRect);

				////update the enemy
				bandit5.Update(deltaTime, player2.posRect);


				//check for hit from turret 5
				for(int i = 0; i < turret5.bulletList.size(); i++)
				{
					if(SDL_HasIntersection(&player2.posRect, &turret5.bulletList[i].posRect)){
						turret5.bulletList[i].Reset();
						player2.eBulletHit();
						break;
					}
				}

				//check for hit from turret 6
				for(int i = 0; i < turret6.bulletList.size(); i++)
				{
					if(SDL_HasIntersection(&player2.posRect, &turret6.bulletList[i].posRect)){
						turret6.bulletList[i].Reset();
						player2.eBulletHit();
						break;
					}
				}

				//check for hit from turret 7
				for(int i = 0; i < turret7.bulletList.size(); i++)
				{
					if(SDL_HasIntersection(&player2.posRect, &turret7.bulletList[i].posRect)){
						turret7.bulletList[i].Reset();
						player2.eBulletHit();
						break;
					}
				}

				//check for hit from turret 8
				for(int i = 0; i < turret8.bulletList.size(); i++)
				{
					if(SDL_HasIntersection(&player2.posRect, &turret8.bulletList[i].posRect)){
						turret8.bulletList[i].Reset();
						player2.eBulletHit();
						break;
					}
				}



				//check if the player damages an enemy
				for(int i = 0; i < player2.bulletList.size(); i++)
				{
					//turret 5
					if(SDL_HasIntersection(&turret5.baseRect, &player2.bulletList[i].posRect)){
						player2.bulletList[i].Reset();
						if(turret5.active == true){
							turret5.RemoveHealth();
							ex5.active = true;
						}
						break;
					}


					//turret 6
					if(SDL_HasIntersection(&turret6.baseRect, &player2.bulletList[i].posRect)){
						player2.bulletList[i].Reset();
						if(turret6.active == true){
							turret6.RemoveHealth();
							ex6.active = true;
						}
						break;
					}

		

					//turret 7
					if(SDL_HasIntersection(&turret7.baseRect, &player2.bulletList[i].posRect)){
						player2.bulletList[i].Reset();
						if(turret7.active == true){
							turret7.RemoveHealth();
							ex7.active = true;
						}
						break;
					}

	

					//turret 8
					if(SDL_HasIntersection(&turret8.baseRect, &player2.bulletList[i].posRect)){
						player2.bulletList[i].Reset();
						if(turret8.active == true){
							turret8.RemoveHealth();
							ex8.active = true;
						}
						break;
					}



					//bandit 5
					if(SDL_HasIntersection(&bandit5.banditRect, &player2.bulletList[i].posRect)){
						player2.bulletList[i].Reset();
						if(bandit5.active == true){
							bandit5.RemoveHealth();
						}
						break;
					}


				}


				//check to see if the player has been hit by the enemy tank
				if(SDL_HasIntersection(&player2.posRect, &bandit5.banditRect)){
					player2.enemyHit();
				}

				//FOLDER CODE
				if (SDL_HasIntersection(&player2.posRect, &fuel1.powderRect) && fuel1.active == true) {
					fuel1.active = false;
					fuel1.powderRect.x = -5000;
					fuel1.powderRect.y = -5000;
					currentFuel += 100 / 4;
					fuelCount++;
					Mix_PlayChannel(-1, pickUpSound, 0);
				}
				if (SDL_HasIntersection(&player2.posRect, &fuel2.powderRect) && fuel2.active == true) {
					fuel2.active = false;
					fuel2.powderRect.x = -5000;
					fuel2.powderRect.y = -5000;
					currentFuel += 100 / 4;
					fuelCount++;
					Mix_PlayChannel(-1, pickUpSound, 0);
				}
				if (SDL_HasIntersection(&player2.posRect, &fuel3.powderRect) && fuel3.active == true) {
					fuel3.active = false;
					fuel3.powderRect.x = -5000;
					fuel3.powderRect.y = -5000;
					currentFuel += 100 / 4;
					fuelCount++;
					Mix_PlayChannel(-1, pickUpSound, 0);
				}
				if (SDL_HasIntersection(&player2.posRect, &fuel4.powderRect) && fuel4.active == true) {
					fuel4.active = false;
					fuel4.powderRect.x = -5000;
					fuel4.powderRect.y = -5000;
					currentFuel += 100 / 4;
					fuelCount++;
					Mix_PlayChannel(-1, pickUpSound, 0);
				}

				movingRect.w = currentFuel / maxFuel * 173;


				if(player2.playerHealth <= 0){
					Level2 = false;
					gameState = LOSE;
				}

				if (fuelCount == 4) {
					Level2 = false;
					gameState = WIN;
				}

				if (ex5.active = true && turret5.e1K == true) {
					ex5.Update(deltaTime);
				}
				if (ex6.active = true && turret6.e1K == true) {
					ex6.Update(deltaTime);
				}
				if (ex7.active = true && turret7.e1K == true) {
					ex7.Update(deltaTime);
				}
				if (ex8.active = true && turret8.e1K == true) {
					ex8.Update(deltaTime);
				}

				// ************************ START THE SDL DRAW PROCESS *****************
				//clear the SDL_Render target
				SDL_RenderClear(renderer);

				//draw the bkgd image
				SDL_RenderCopy(renderer, level2, NULL, &level2Pos);

				//fuel middle UI
				SDL_RenderCopy(renderer, fuelM, NULL, &movingRect);
				//fuel back UI
				SDL_RenderCopy(renderer, fuelB, NULL, &fuelRect);
			

				player2.Draw(renderer);

				if (fuel1.active) {
					fuel1.Draw(renderer);
				}
				if (fuel2.active) {
					fuel2.Draw(renderer);
				}
				if (fuel3.active) {
					fuel3.Draw(renderer);
				}
				if (fuel4.active) {
					fuel4.Draw(renderer);
				}

				//draw the turret 1
				turret5.Draw(renderer);
				turret6.Draw(renderer);
				turret7.Draw(renderer);
				turret8.Draw(renderer);


				bandit5.Draw(renderer);

				if (ex5.active = true && turret5.e1K == true) {
					ex5.Draw(renderer);
				}
				if (ex6.active = true && turret6.e1K == true) {
					ex6.Draw(renderer);
				}
				if (ex7.active = true && turret7.e1K == true) {
					ex7.Draw(renderer);
				}
				if (ex8.active = true && turret8.e1K == true) {
					ex8.Draw(renderer);
				}

				//present new buffer to the screen
				SDL_RenderPresent(renderer);
			}// ends play screen
			break;




		case BACKSTORY:

			backstory = true;

			cout << endl;

			while(backstory)
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
								//if player chooses main menu
								if (menuOver) {
									Mix_PlayChannel(-1, pressedSound, 0);
									backstory = false;
									gameState = MENU;
									menuOver = false;
								}
								if (instructionsOver) {
									Mix_PlayChannel(-1, pressedSound, 0);
									backstory = false;
									gameState = INSTRUCTIONS;
									instructionsOver = false;
								}
								if (startOver) {
									Mix_PlayChannel(-1, pressedSound, 0);
									backstory = false;
									gameState = PLAY;
									startOver = false;
								}
							}

						}//end joystick check
						break;

					case SDL_CONTROLLERAXISMOTION:

						moveCursor(e.caxis);

						break;
					}// end button input check
				}//end backstory poll event



				 //****************************UPDATE SECTION*******************************
				UpdateBackground(deltaTime);

				//curosr
				UpdateCursor(deltaTime);

				menuOver = SDL_HasIntersection(&activePos, &menuPos);
				instructionsOver = SDL_HasIntersection(&activePos, &instructNPos2);
				startOver = SDL_HasIntersection(&activePos, &stNPos);


				//if the cursor is over a button, play the over sound
				if (menuOver) {
					if (alreadyOver == false) {
						//Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				//if the cursor is not over any button reset the alreadyOver var
				if (!menuOver) {
					alreadyOver = false;
				}

				//if the cursor is over a button, play the over sound
				if (instructionsOver) {
					if (alreadyOver == false) {
						//Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}
				//if the cursor is not over any button reset the alreadyOver var
				if (!instructionsOver) {
					alreadyOver = false;
				}

				//if the cursor is over a button, play the over sound
				if (startOver) {
					if (alreadyOver == false) {
						//Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				//if the cursor is not over any button reset the alreadyOver var
				if (!startOver) {
					alreadyOver = false;
				}



				// ************************ START THE SDL DRAW PROCESS *****************
				//clear the SDL_Render target
				SDL_RenderClear(renderer);


				//draw the bkgd image
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				//draw the bkgd2 image
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				SDL_RenderCopy(renderer, backStory, NULL, &backSPos);

				//draw the background
				if (menuOver) {
					SDL_RenderCopy(renderer, menuO, NULL, &menuPos);
				}
				else {
					SDL_RenderCopy(renderer, menuN, NULL, &menuPos);
				}
				//draw the background
				if (instructionsOver) {
					SDL_RenderCopy(renderer, instructO, NULL, &instructNPos2);
				}
				else {
					SDL_RenderCopy(renderer, instructN, NULL, &instructNPos2);
				}
				//draw the background
				if (startOver) {
					SDL_RenderCopy(renderer, stO, NULL, &stNPos);
				}
				else {
					SDL_RenderCopy(renderer, stN, NULL, &stNPos);
				}

				//draw the cursor  cursor
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				//present new buffer to the screen
				SDL_RenderPresent(renderer);
			}// ends backstory screen
			break;

case INSTRUCTIONS:

	instructions = true;


	while (instructions)
	{
		//set up the frame rate for the section or case
		thisTime = SDL_GetTicks();
		deltaTime = (float)(thisTime - lastTime) / 1000;
		lastTime = thisTime;

		//check for input
		if (SDL_PollEvent(&e))
		{
			//see if the player closes the window's X button
			if (e.type == SDL_QUIT)
			{
				quit = true;
				instructions = false;
				break;
			}

			//test for controller input
			switch (e.type)
			{

			case SDL_CONTROLLERBUTTONDOWN:

				//checks to see if this is the controller 0
				if (e.cdevice.which == 0) {
					//if A button
					if (e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
					{
						//if player chooses main menu
						if (menuOver) {
							Mix_PlayChannel(-1, pressedSound, 0);
							instructions = false;
							gameState = MENU;
							menuOver = false;
	
						}
						if (players2Over) {
							Mix_PlayChannel(-1, pressedSound, 0);
							instructions = false;
							gameState = BACKSTORY;
							players2Over = false;
						}
						if (startOver) {
							Mix_PlayChannel(-1, pressedSound, 0);
							instructions = false;
							gameState = PLAY;
							startOver = false;
						}
					}
					

				}//end joystick check
				break;

			case SDL_CONTROLLERAXISMOTION:

				moveCursor(e.caxis);

				break;

			}// end button input check
		}//end instructions poll event

		 //****************************UPDATE SECTION*******************************
		UpdateBackground(deltaTime);

		//curosr
		UpdateCursor(deltaTime);

		menuOver = SDL_HasIntersection(&activePos, &menuPos);
		players2Over = SDL_HasIntersection(&activePos, &backNPos2);
		startOver = SDL_HasIntersection(&activePos, &stNPos);


		//if the cursor is over a button, play the over sound
		if (menuOver) {
			if (alreadyOver == false) {
				//Mix_PlayChannel(-1, overSound, 0);
				alreadyOver = true;
			}
		}

		//if the cursor is not over any button reset the alreadyOver var
		if (!menuOver) {
			alreadyOver = false;
		}

		//if the cursor is over a button, play the over sound
		if (players2Over) {
			if (alreadyOver == false) {
				//Mix_PlayChannel(-1, overSound, 0);
				alreadyOver = true;
			}
		}
		//if the cursor is not over any button reset the alreadyOver var
		if (!players2Over) {
			alreadyOver = false;
		}

		//if the cursor is over a button, play the over sound
		if (startOver) {
			if (alreadyOver == false) {
				//Mix_PlayChannel(-1, overSound, 0);
				alreadyOver = true;
			}
		}

		//if the cursor is not over any button reset the alreadyOver var
		if (!startOver) {
			alreadyOver = false;
		}

		 // ************************ START THE SDL DRAW PROCESS *****************
		 //clear the SDL_Render target
		SDL_RenderClear(renderer);

		//draw the bkgd image
		SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

		//draw the bkgd2 image
		SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

		SDL_RenderCopy(renderer, insB, NULL, &insBPos);

		//draw the background
		if (menuOver) {
			SDL_RenderCopy(renderer, menuO, NULL, &menuPos);
		}
		else {
			SDL_RenderCopy(renderer, menuN, NULL, &menuPos);
		}
		//draw the background
		if (players2Over) {
			SDL_RenderCopy(renderer, backO, NULL, &backNPos2);
		}
		else {
			SDL_RenderCopy(renderer, backN, NULL, &backNPos2);
		}
		//draw the background
		if (startOver) {
			SDL_RenderCopy(renderer, stO, NULL, &stNPos);
		}
		else {
			SDL_RenderCopy(renderer, stN, NULL, &stNPos);
		}

		//draw the cursor  cursor
		SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);


		//present new buffer to the screen
		SDL_RenderPresent(renderer);

	}// ends instructions screen
	break;

		case WIN:
			alreadyOver = false;
			win = true;

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

				//draw the win Background image
				SDL_RenderCopy(renderer, winB, NULL, &winBPos);

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
									lose = false;
									gameState = MENU;
									menuOver = false;
								}

								//if player chooses to play again
								if(playOver){
									Mix_PlayChannel(-1, pressedSound, 0);
									lose = false;
									gameState = PLAY;
									playOver = false;
																}
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
				UpdateLose(deltaTime);

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

				//draw the bkgd image
				SDL_RenderCopy(renderer, ls1, NULL, &lose1Pos);

				//draw the bkgd2 image
				SDL_RenderCopy(renderer, ls2, NULL, &lose2Pos);

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

		}// ends the  switch


	}//end main loop


	//close and destroy the winodw
	SDL_DestroyWindow(window);

	//clean up
	SDL_Quit();

	return 0;
}
