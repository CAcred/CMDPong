/*******************************************************-
 * CmdPongClasses.h
 *
 * This holds all the classes used in CmdPong.
 *
 *  Created on: Oct 18, 2016
 *
 * ©2018 C. A. Acred all rights reserved.
 ------------------------------*
 */

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <windows.h>
#include <sstream>
#include <cstdlib>



//#include <"../Includes/C://Users//Night_Blader//Personal//Creations//CAA14's Programs/WeeklyWorkout/src/caa++-lib//util.cpp">
using namespace std;

class cmdPongGame;
class Display;
class Ball;
class Paddle;
class InputReader;
class Counter;


class cmdPongGame
{
	// Vars:
	Counter* p1Count;
	Counter* p2Count;
	int score1;
	int score2;
	bool isPaused;
	bool gameOver;
	int screen;
	bool needRender;
	Display* display;
	Paddle* playerOne;
	Paddle* playerTwo;
	Ball* ball;
	InputReader* input;
        
public:
	static const float PADDLE_START_SPEED;
	// Constructor:
	cmdPongGame();
	~cmdPongGame();

	// Methods:
	int playNewGame();
        
	int loadTitleScreen();
	
	int loadGameScreen();
	
	int loadEndScreen();

	int update(string input);

	int updatePaddles(string input);
        
	int updateBall(string input);
        
	int updateDisplay(string input);
	
	int updateScore(int player);
	
	int ballCountDown();
};

class Display
{	// Vars:
private:
	vector<string> display;
	int xLength;
	int yLength;
	int xStart;
	int bufferX;
	int bufferY;
	// Constructor:
public:
	Display(int width, int height, int xStart);
	~Display();

	// Methods:
	int render();

	int changeDisplay(string input, int x, int y);
	int changeDisplay(string input[], int x, int y, int size);
	int clearDisplay();
	int clearDisplayRow(int row, int xStart, int xEnd);
	
	int getXLength();
	int getYLength();
	
	int loadBorder();
};

class Ball
{
public:
    static const float BALL_START_SPEED;
	// Vars:
	string img;
	float xVel;
	float yVel;
	float xPos;
	float yPos;
	bool doesRender;
        
	// Constructor:
	Ball(float x, float y, float xv, float yv);
	~Ball();

	// Methods:
	int getXpos();
	
	int getYPos();
	
	string getImg();
};

class Paddle
{
public:
	// Vars:
	string img;
	float ySpd;
	int xPos;
	float yPos;
    bool doesRenders;
        
	// Constructor:
	Paddle(int x, int y);
	~Paddle();

	// Methods:
	int getXpos();
	
	int getYPos();
	
	string getImg();
};

class Counter
{
private:
	// Vars:
	int number;
	string img;
	int xPos;
	int yPos;
public:
	// Constructor:
	Counter(int startNum, int x, int y);
	~Counter();

	// Methods:
	int update();
        
	int getValue();
	
	string getImg();
	
	int getXpos();
	
	int getYpos();

	int add(int number);
};

class InputReader
{
    //Vars:
private:
    string keysDown;
    
public:
    InputReader();
    ~InputReader();
    
    string getKeyState();
};
