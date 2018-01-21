/*******************************************************-
 * CmdPongClasses.cpp
 *
 * This contains the code for all the classes of CmdPong.
 *
 *  Created on: Oct 18, 2016
 * 
 * ©2018 C. A. Acred all rights reserved.
 ------------------------------*
 */

#include "CmdPongClasses.h"


// cmdPongGame:
const float cmdPongGame::PADDLE_START_SPEED = .3;

cmdPongGame::cmdPongGame()
{
    screen = 0;
    needRender = true;
    display = new Display(40, 15, 10);
    input = new InputReader();
    string keyInput;
    loadTitleScreen();

    while (true)
    {  
        keyInput = input->getKeyState();
        update(keyInput);
        if (needRender)
        {
            system("cls");
            display->render();
            needRender = false;
        }
        Sleep(1000/60); // 60 Updates a second.
    }
}

cmdPongGame::update(string input)
{
    switch(screen)
    {
    case 0: // Menu
        if (input.find("SPACE") != string::npos)
        {
            screen = 1;
            playNewGame();
        }
        break;

    case 1: // Game screen
        if (gameOver)
        {
            screen = 2;
            loadEndScreen();
            break;
        }
        
        if (input.find("SPACE") != string::npos)
        {
            if (isPaused)
            {
                display->clearDisplayRow(2, 1, display->getXLength() - 1);
                display->changeDisplay("Press Space Bar to Pause", 10, 2);
            }
            else
                display->changeDisplay("Press Space Bar to Resume", 10, 2);
            isPaused = !isPaused;
            needRender = true;   
        }
        
        if (!isPaused)
        {
            updatePaddles(input);
            updateBall(input);
        }
        break;

    case 2: // End screen
        if (input.find("SPACE") != string::npos)
        {
            screen = 0;
            needRender = true;
        }
        if (input.find("ESC") != string::npos)
            exit(0);
        break;
    }
    
    updateDisplay(input);
    return 0;
}

cmdPongGame::loadTitleScreen()
{
    needRender = true;
    display->clearDisplay();
    display->changeDisplay("CMD Pong", 17, 1);
    display->changeDisplay("Press Space Bar to Begin", 10, 2);
    display->changeDisplay("---------------------------------------", 1, 3);
    display->changeDisplay("P1: Arrows; P2: w/s", 12, 8);
    return 0;
}

cmdPongGame::loadGameScreen()
{
    needRender = true;
    display->changeDisplay("CMD Pong", 17, 1);
    if (!isPaused)
        display->changeDisplay("Press Space Bar to Pause", 10, 2);
    else
        display->changeDisplay("Press Space Bar to Resume", 10, 2);
    display->changeDisplay("---------------------------------------", 1, 3);
    return 0;
}


cmdPongGame::loadEndScreen()
{
    needRender = true;
    display->changeDisplay("CMD Pong", 17, 1);
    display->clearDisplayRow(3, 1, display->getXLength());
    display->changeDisplay("Press Space restart; Q to exit", 7, 2);
    if (score1 == 7)
        display->changeDisplay("Player One Wins!", 12, 8);
    else
        display->changeDisplay("Player Two Wins!", 12, 8);
    display->changeDisplay("---------------------------------------", 1, 3);
    return 0;
}

cmdPongGame::playNewGame()
{
    score1 = 0;
    score2 = 0;
    isPaused = false;
    gameOver = false;
    this->ball = new Ball(display->getXLength()/2, display->getYLength()/2, Ball::BALL_START_SPEED, Ball::BALL_START_SPEED);
    this->playerOne = new Paddle(1, 4);
    this->playerTwo = new Paddle(display->getXLength()-1, 4);
    this->p1Count = new Counter(score1, 1, 2);
    this->p2Count = new Counter(score2, display->getXLength() - 1, 2);
    loadGameScreen();
    display->clearDisplayRow(8, 1, display->getXLength() - 1); // Get rid of control instructions
    ballCountDown();
    return 0;
}

cmdPongGame::updateBall(string input)
{
    if (ball->xPos == 1)
    {
        updateScore(2);
        return 0;
    }
    if (ball->xPos == display->getXLength() - 1)
    {
        updateScore(1);
        return 0;
    }
    
    if (int(ball->xPos + ball->xVel) != int(ball->xPos) || int(ball->yPos + ball->yVel) != int(ball->yPos))
        needRender = true; // There has been movement.
        
        
    ball->xPos += ball->xVel;
    ball->yPos += ball->yVel;
    
    // Check yPos for out of bounds:
    if (ball->yPos >= display->getYLength() - 1)
    {
        ball->yPos = display->getYLength() - 2;
        ball->yVel *= -1.001;
    }
    if (ball->yPos <= 4)
    {
        ball->yPos = 4;
        ball->yVel *= -1.001;
    }
    
    
    // Check xPos for scoring/contact with paddle: 
    // NOTE: The values are floats to allow for fractional speeds, but when compared they must become ints
    if (ball->xPos <= 2)
    {
        if (int(playerOne->yPos) == int(ball->yPos))
        {
            ball->xPos = 2;
            ball->xVel *= -1.01;
        }
        else if (ball->xPos <= 1)
            ball->xPos = 1;
    }
    
    if (int(display->getXLength() - 1) - int(ball->xPos) <= 2)
    {
        if (int(playerTwo->yPos) == int(ball->yPos))
        {
            ball->xPos =  display->getXLength() - 2;
            ball->xVel *= -1.01;
        }
        else if (int(display->getXLength() - ball->xPos) <= 1)
            ball->xPos =  display->getXLength() - 1;
    }
    
    return 0;
}

cmdPongGame::updatePaddles(string input)
{    
    if (input.find("w") != string::npos)
    {
        if (int(playerOne->yPos - playerOne->ySpd) != int(playerOne->yPos))
            needRender = true;
        playerOne->yPos -= playerOne->ySpd;
    }
    if (input.find("s") != string::npos)
    {
        if (int(playerOne->yPos + playerOne->ySpd) != int(playerOne->yPos))
            needRender = true;
        playerOne->yPos += playerOne->ySpd;
    }
    
    // Insure no out of bounds:
    if (int(playerOne->yPos) > display->getYLength() - 1)
        playerOne->yPos = display->getYLength() - 1;
    
    if (int(playerOne->yPos) <  4)
        playerOne->yPos = 4;
    
    if (input.find("UP") != string::npos)
    {
        if (int(playerOne->yPos - playerOne->ySpd) != int(playerOne->yPos))
            needRender = true;
        playerTwo->yPos -= playerTwo->ySpd;
    }
    if (input.find("DOWN") != string::npos)
    {
        if (int(playerOne->yPos + playerOne->ySpd) != int(playerOne->yPos))
            needRender = true;
        playerTwo->yPos += playerTwo->ySpd;
    }
    
    // Insure no out of bounds:
    if (int(playerTwo->yPos) > display->getYLength() - 1)
        playerTwo->yPos = display->getYLength() - 1;
    
    if (int(playerTwo->yPos) <  4)
        playerTwo->yPos = 4;

    return 0;
}

cmdPongGame::updateDisplay(string input)
{
    if (needRender)
    {
        display->clearDisplay();
        switch (screen)
        {
            case 0:
                loadTitleScreen();
                break;
            case 1:
                display->clearDisplay();
                loadGameScreen();
                display->changeDisplay(ball->getImg(), ball->getXpos(), ball->getYPos());
                display->changeDisplay(playerOne->getImg(), playerOne->getXpos(), playerOne->getYPos());
                display->changeDisplay(playerTwo->getImg(), playerTwo->getXpos(), playerTwo->getYPos());
                
                display->changeDisplay(p1Count->getImg(), p1Count->getXpos(), p1Count->getYpos());
                display->changeDisplay(p2Count->getImg(), p2Count->getXpos(), p2Count->getYpos());
                break;
            case 2:
                loadEndScreen();
                display->changeDisplay(p1Count->getImg(), p1Count->getXpos(), p1Count->getYpos());
                display->changeDisplay(p2Count->getImg(), p2Count->getXpos(), p2Count->getYpos());
                break;
        }
    }
    return 0;
}

cmdPongGame::updateScore(int player)
{
    if (player == 1)
    {
        score1 += 1;
        p1Count->add(1);
    }
    else
    {
        score2 += 1;
        p2Count->add(1);
    }
    if (score1 == 7 || score2 == 7)
        gameOver = true;
    else
        ballCountDown();
    return 0;
}

cmdPongGame::ballCountDown() // Count down to three on the screen and then launch ball anew.
{
    for (int i = 0; i < 3; i++)
    {
        if (i == 0)
            display->changeDisplay("3", 20, 7);
        else if (i == 1)
            display->changeDisplay("2", 20, 7);
        else if (i == 2)
            display->changeDisplay("1", 20, 7);
        system("cls");
        display->render();
        Sleep(1000);
    }
    ball->xPos = display->getXLength()/2;
    ball->yPos = display->getYLength()/2;
    return 0;
}


// Display:
Display::Display(int width, int height, int xStart)
{
    // Initialize variables:
    for (int y = 0; y < height; y++)
        display.push_back(" ");
    
    this->xStart = xStart;
    this->xLength = width;
    this->yLength = display.size();
    this->bufferX = 1;
    this->bufferY = 1;
    
    loadBorder();
    return;
}

Display::render()
{
    // Display display:
    for (int j = 0; j < this->display.size(); j++)
    {
        for (int b = 0; b < this->xStart; b++) // Use the xStart buffer.
        {
            cout << ' ';
        }
            cout << this->display[j];
            cout << '\n';
    }
}

Display::getXLength()
{
    return xLength;
}


Display::getYLength()
{
    return yLength;
}

Display::changeDisplay(string input, int x, int y)
{
    for (int i = 0; i < input.length(); i++)
    {
        display.at(y)[i+x] = input[i];
    }
    return 0;
}

Display::changeDisplay(string input[], int x, int y, int size)
{
    for (int j = 0; j < size; j++)
    {
        for (int i = 0; i < input[j].length(); i++)
        {
            display.at(y+j)[i+x] = input[j][i];
        }
    }
    return 0;
}

Display::clearDisplay()
{
    for (int y = 0; y < this->yLength; y++)
    {
        for (int x = 0; x < this->xLength + 1; x++)
            display.at(y)[x] = ' ';
    }
    loadBorder();
    return 0;
}

Display::clearDisplayRow(int row, int xStart, int xEnd)
{
    for (int x = xStart; x < xEnd; x++)
    {
        if (x > display.at(row).length())
            break;
        display.at(row)[x] = ' ';
    }
    return 0;
}

Display::loadBorder()
{
    string temp;
    // Build top:
    temp = " ";
    for (int i = 0; i < xLength - 1; i++)
    {
        temp += '-';
    } 

    display.at(0) = temp;
    
    // Build body:
    for (int y = 1; y < yLength - 1; y++)
    {
        temp = "|";

        for (int x = 1; x < (xLength); x++)
        {
            temp += ' ';
        }
        temp += '|';
        display.at(y) = temp;
    }
    // Build bottom:
    display.at(this->yLength - 1) = display.at(0);
    return 0;
}

// Ball:
const float Ball::BALL_START_SPEED = .3;

Ball::Ball(float x, float y, float xv, float yv)
{
    this->doesRender = true;
    this->img = "o";
    this->xPos = x;
    this->yPos = y;
    this->xVel = xv;
    this->yVel = yv;
    return;
}

Ball::getXpos()
{
    return this->xPos;
}
        
Ball::getYPos()
{
    return this->yPos;
}

string Ball::getImg()
{
    return this->img;
}

// Paddle: 
Paddle::Paddle(int x, int y)
{
    this->img = "|";
    this->doesRenders = true;
    this->xPos = x;
    this->yPos = y;
    this->ySpd = cmdPongGame::PADDLE_START_SPEED;
    return;
}

Paddle::getXpos()
{
    return this->xPos;
}
        
Paddle::getYPos()
{
    return int(this->yPos);
}

string Paddle::getImg()
{
    return this->img;
}

// InputReader:
InputReader::InputReader(){keysDown = "";}
InputReader::~InputReader(){}

string InputReader::getKeyState()
{
    string result = "";
    
    if (GetAsyncKeyState(VK_UP)) 
        result += "UP";
    
    if (GetAsyncKeyState(VK_DOWN))
        result += "DOWN";
    
    if (GetAsyncKeyState(0x57))
        result += "w";

    if (GetAsyncKeyState(0x53))
        result += "s";
    
    if (GetAsyncKeyState(0x51))
        result += "ESC";
    
    if (GetAsyncKeyState(VK_SPACE)) // The if/if else struc does not return a key being pressed until it has been released
    {
        if (keysDown.find("SPACE") == string::npos)
        {
            keysDown += "SPACE";
        }
    }
    else if (keysDown.find("SPACE") != string::npos)
    {
        result += "SPACE";
        keysDown.erase(keysDown.find("SPACE"), 5);
    }
    
    return result;
}

// Counter:
Counter::Counter(int startNum, int x, int y)
{
    xPos = x;
    yPos = y;
    number = startNum;
    update();
    return;
}

Counter::update()
{
    std::ostringstream stm;
    stm << number;
    img = stm.str();
}

string Counter::getImg()
{
    return img;
}

Counter::getValue()
{
    return number;
}

Counter::getXpos()
{
    return xPos;
}

Counter::getYpos()
{
    return yPos;
}

Counter::add(int number)
{
    this->number += number;
    update();
}