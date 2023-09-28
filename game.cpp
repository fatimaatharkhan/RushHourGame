//============================================================================
// Name        : .cpp
// Author      : FAST CS Department
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Rush Hour...
//============================================================================


/*
	Left To Do:
	Timing DONE
	Populating Passengers again DONE
	Random movement of car 
	Menu before game NOT NEEDED
	Scoring DONE
	Filing DONE
	No Collisions DONE
	Global to local variables
	Making proper shapes ALMOST DONE
*/

#ifndef RushHour_CPP_
#define RushHour_CPP_
#include "util.h"
#include <iostream>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include<cmath> // for basic math functions such as cos, sin, sqrt
using namespace std;

// seed the random numbers generator by current time (see the documentation of srand for further help)...

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}


int xI = 10, yI = 760;
int arr[400][2];
int Walls[120][2];
int Passengers[5][2];
int Boxes[3][2];
int Trees[4][2];
int NoOfCars = 4;
int Cars[20][2];
int CollisionX = -1;
int CollisionY = -1;
int carcolor;
int PassengerNo;
int DestinationX = -1;
int DestinationY = -1;
bool dropped = false, check = false;
string name;
int PickStatus = -1;
int score = 0;
string Scores[11][2] = {"-10000"};


void ColorOfCar() //Chooses red if user enters 1, yellow if user enters 2 and this function is called again if the user enters 3 by assigning a random value from 1 and 2 to choose between red and yellow
{
	if(carcolor == 1)//If carcolor = 1, red car
	{
		DrawSquare(xI+10, yI+20, 20, colors[RED]);
		DrawSquare(xI+1, yI+12, 20, colors[RED]);
		DrawSquare(xI+19, yI+12, 20, colors[RED]);
		DrawSquare(xI+14, yI+26, 12, colors[ORANGE]);
		DrawCircle((xI+11) , (yI+7) , 8 , colors[ORANGE]);
		DrawCircle((xI+29) , (yI+7) , 8 , colors[ORANGE]);
	}
	else if (carcolor == 2) // If carcolor = 2, yellow car
	{
		DrawSquare(xI+10, yI+20, 20, colors[YELLOW]);
		DrawSquare(xI+1, yI+12, 20, colors[YELLOW]);
		DrawSquare(xI+19, yI+12, 20, colors[YELLOW]);
		DrawSquare(xI+14, yI+26, 12, colors[LIGHT_BLUE]);
		DrawCircle((xI+11) , (yI+7) , 8 , colors[LIGHT_BLUE]);
		DrawCircle((xI+29) , (yI+7) , 8 , colors[LIGHT_BLUE]);
	}
	else
	{
		carcolor = GetRandInRange(0,3); // If carcolor = 3, a random number is generated from 1 and 2 and the function is called again.
		ColorOfCar();
	}
}

void drawCar()
{
	ColorOfCar();
	glutPostRedisplay();
}

/*
 * Main Canvas drawing function.
 */

int Menu() // Menu is printed using this function
{
	int choice;
	cout << "MENU:" << endl;
	cout << "Press 1 to Start the Game: " << endl;
	cout << "Press 2 to Display the Leaderboard: " << endl;
	cout << "Press any other key to Exit: " << endl;
	cout << "Please enter your choice now: " << endl;
	cin >> choice;
	return choice;
}

void MenuFirstOption() // If the user presses 1 in the menu, this function is called to choose a car color.
{
	do
	{
		cout << "Please choose a color for your taxi:" << endl;
		cout << "Press 1 for Red:" << endl;
		cout << "Press 2 for Yellow:" << endl;
		cout << "Press 3 for a Random color:" << endl;
		cout << "Please enter your choice now: " << endl;
		cin >> carcolor;
	}
	while (carcolor != 1 and carcolor != 2 and carcolor != 3);
	cout << "Please enter your name: " << endl;
	cin >> name;
}

void FillingScoresArray() // Fills the array of scores from the text file
{
	string line;
	int a = 0, b = 0;
	fstream myfile;
	myfile.open ("Score.txt");
	if (!myfile)
	{
		ofstream myfile;
		myfile.open ("Score.txt");
	}
	else
	{
		while(getline(myfile, line))
		{
			Scores[b][0] = "";
			Scores[b][1] = "";
			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] == ',')
				{
					a = 1;
					i++;
				}
				Scores[b][a] += (char)line[i];
			}
			b++;
			a = 0;
        	}
        }
	myfile.close();
}

void MenuSecondOption() // This function is called if the user presses 2 in the menu
{
	FillingScoresArray();
	cout << endl << setw(20) << left << "NAME" << "SCORE" << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << setw(20) << left << Scores[i][0] << Scores[i][1] << endl;
	}
}

void SortingScores() // Once the array of scores if filled from the text file, this function sorts it into descending order
{
	string tempName,tempScore;
	for (int i = 0; i < 11; i++)
	{
        	for (int x = i+1; x < 11; x++)
        	{
        		if (stoi(Scores[i][1]) < stoi(Scores[x][1]))
        		{
         			tempName = Scores[i][0];
        			tempScore = Scores[i][1];
        			Scores[i][0] = Scores[x][0];
                		Scores[i][1] = Scores[x][1];
                		Scores[x][0] = tempName;
                		Scores[x][1] = tempScore;
        		}
        	}
      	}
}

void Finish(int m) // When 3 minutes are over, this function is called where we save the latest score
{
	FillingScoresArray();
	Scores[10][0] = name;
	Scores[10][1] = to_string(score);
	SortingScores();
	remove("Score.txt");
	ofstream myfile;
	myfile.open ("Score.txt");
	for (int i = 0; i < 10; i++)
	{
		if (Scores[i][1] != "-10000")
		{
			myfile << Scores[i][0] << "," << Scores[i][1] << "\n";
		}
	}
	myfile.close();
	exit(0);
}

void PopulateCars()// Populates the array of cars
{
	int x, y;
	for (int i = 0; i < 4; i++)
	{
		x = GetRandInRange(0,19);
		y = GetRandInRange(0,19);
		Cars[i][0] = ((x*40)+10);
		Cars[i][1] = (y*40);
	}
}

bool CheckCars( int x, int y)//Checks if the x and y values is already filled with a car
{
	bool available = true;
	for (int i = 0; i < 100; i++)
	{
		if (x == Cars[i][0])
		{
			if (y == Cars[i][1])
			{
				available = false;
				break;
			}
		}
	}
	return available; // If that x and y coordinate is not filled it returns true, else false
}

void PopulateWalls()// Populates the array of walls
{
	int a = 0, x, y;
	for (int i = 0; i < 35; i++)
	{
		do
		{
		x = GetRandInRange(1,18);
		y = GetRandInRange(1,18);
		}
		while (!CheckCars((x*40) + 10,(y*40)));
		Walls[a][0] = (x*40) + 10;
		Walls[a][1] = (y*40);
		if (i % 2 == 0)
		{
			int b = 0;
			while (b < 3)
			{
				if (((x+b)*40) + 10 < 760 and ((x+b)*40) + 10 > 10)
				{
					Walls[a][0] = (((x+b)*40) + 10);
					Walls[a][1] = (y*40);
					b++;
					a++;
				}
				else
				{
					break;
				}
			}
		}
		
		else
		{
			int b = 0;
			while (b < 4)
			{
				if ((y+b)*40 < 760 and (y+b)*40 > 10)
				{
					Walls[a][0] = ((x*40) + 10);
					Walls[a][1] = ((y+b)*40);
					b++;
					a++;
				}
				else
				{
					break;
				}
			}
		}
	}
}

bool CheckWall( int x, int y)//Checks if the x and y values is already filled with a wall
{
	bool available = true;
	for (int i = 0; i < 100; i++)
	{
		if (x == Walls[i][0])
		{
			if (y == Walls[i][1])
			{
				available = false;
				break;
			}
		}
	}
	return available; // If that x and y coordinate is not filled it returns true, else false
}

void PopulatePassengers()// Populates the array of passengers
{
	PassengerNo = GetRandInRange(3,5);
	int x, y;
	for (int i = 0; i < PassengerNo; i++)
	{
		do
		{
			x = GetRandInRange(0,19);
			y = GetRandInRange(0,19);
		}
		while (!CheckWall(((x*40)+10),(y*40)) or !CheckCars(((x*40)+10),(y*40)));
		Passengers[i][0] = ((x*40)+10);
		Passengers[i][1] = (y*40);
	}
}

bool CheckPassengers( int x, int y) //Checks if the x and y values is already filled with a passenger
{
	bool available = true;
	for (int i = 0; i < PassengerNo; i++)
	{
		if (x == Passengers[i][0])
		{
			if (y == Passengers[i][1])
			{
				available = false;
				break;
			}
		}
	}
	return available; // If that x and y coordinate is not filled it returns true, else false
}

void PopulateBoxes()// Populates the array of boxes
{
	int x, y;
	for (int i = 0; i < 3; i++)
	{
		do
		{
			x = GetRandInRange(0,19);
			y = GetRandInRange(0,19);
		}
		while (!CheckWall(((x*40)+10),(y*40)) or !CheckPassengers(((x*40)+10),(y*40)) or !CheckCars(((x*40)+10),(y*40)));
		Boxes[i][0] = ((x*40)+10);
		Boxes[i][1] = (y*40);
	}
}

bool CheckBoxes( int x, int y) //Checks if the x and y values is already filled with a box
{
	bool available = true;
	for (int i = 0; i < 3; i++)
	{
		if (x == Boxes[i][0])
		{
			if (y == Boxes[i][1])
			{
				available = false;
				break;
			}
		}
	}
	return available; // If that x and y coordinate is not filled it returns true, else false
}

void PopulateTrees() // Populates the array of trees
{
	int x, y;
	for (int i = 0; i < 4; i++)
	{
		do
		{
			x = GetRandInRange(0,19);
			y = GetRandInRange(0,19);
		}
		while (!CheckWall(((x*40)+10),(y*40)) or !CheckPassengers(((x*40)+10),(y*40)) or !CheckCars(((x*40)+10),(y*40)) or !CheckBoxes(((x*40)+10),(y*40)));
		Trees[i][0] = ((x*40)+10);
		Trees[i][1] = (y*40);
	}
}

bool CheckTrees( int x, int y) //Checks if the x and y values is already filled with a tree
{
	bool available = true;
	for (int i = 0; i < 4; i++)
	{
		if (x == Trees[i][0])
		{
			if (y == Trees[i][1])
			{
				available = false;
				break;
			}
		}
	}
	return available; // If that x and y coordinate is not filled it returns true, else false
}

void DrawWalls() // Draws the walls on the coordinates saved in the wall array
{
	int a = 0;
	for (int i = 0; i < 100; i++)
	{
		DrawSquare((Walls[i][0]) , (Walls[i][1]) , 40 , colors[DARK_ORANGE]);
	}
}

void ScoreCheck()
{
	if (score >= 100)
	{
		Finish(0);
	}
}

void Destination() // Destination of the picked up passenger is determined here
{
	int x, y;
	if (PickStatus != -1) // Pickstatus = -1 when no passenger is to be dropped
	{
		if (PickStatus == 0)
		{
			if (DestinationX == -1)
			{
				do
				{
					x = GetRandInRange(0,19);
					y = GetRandInRange(0,19);
				}
				while (!CheckWall(((x*40)+10),(y*40)) or !CheckPassengers(((x*40)+10),(y*40)) or !CheckCars(((x*40)+10),(y*40)) or !CheckBoxes(((x*40)+10),(y*40)) or !CheckTrees(((x*40)+10),(y*40)));
				DestinationX = ((x*40)+10);
				DestinationY = (y*40);
			}
			DrawSquare(DestinationX , DestinationY , 40 , colors[GREEN]);
		}
		else if (PickStatus == 1)
		{
			DrawSquare(DestinationX , DestinationY , 40 , colors[WHITE]);
			DestinationX = -1;
			DestinationY = -1;
			PickStatus = -1;
			score += 10;
			ScoreCheck();
		}	
	}
}

void DrawPassengers() // Draws the walls on the coordinates saved in the passenger array
{
	int a = 0, x, y;
	for (int i = 0; i < 5; i++)
	{
		if ((Passengers[i][0] == -2) and (Passengers[i][1] == -2))
		{
			continue;
		}
		else if ((Passengers[i][0] == -1) and (Passengers[i][1] == -1))
		{
			DrawSquare((Passengers[i][0]) , (Passengers[i][1]) , 40 , colors[WHITE]);
			do
			{
				x = GetRandInRange(0,19);
				y = GetRandInRange(0,19);
			}
			while (!CheckWall(((x*40)+10),(y*40)) or !CheckPassengers(((x*40)+10),(y*40)) or !CheckCars(((x*40)+10),(y*40)) or !CheckBoxes(((x*40)+10),(y*40)) or !CheckTrees(((x*40)+10),(y*40)));
				Passengers[i][0] = ((x*40)+10);
				Passengers[i][1] = (y*40);
		}
		else
		{
			DrawCircle((Passengers[i][0]+20) , (Passengers[i][1]+30) , 7 , colors[BLACK]);
			DrawLine((Passengers[i][0]+20), (Passengers[i][1]+30), (Passengers[i][0]+20), (Passengers[i][1]+10), 2 , colors[BLACK]);
			DrawLine((Passengers[i][0]+20), (Passengers[i][1]+25), (Passengers[i][0]+10), (Passengers[i][1]+20), 2 , colors[BLACK]);
			DrawLine((Passengers[i][0]+20), (Passengers[i][1]+25), (Passengers[i][0]+30), (Passengers[i][1]+20), 2 , colors[BLACK]);
			DrawLine((Passengers[i][0]+20), (Passengers[i][1]+10), (Passengers[i][0]+10), (Passengers[i][1]), 2 , colors[BLACK]);
			DrawLine((Passengers[i][0]+20), (Passengers[i][1]+10), (Passengers[i][0]+30), (Passengers[i][1]), 2 , colors[BLACK]);
		}
	}
}

void DrawCars() // Draws the walls on the coordinates saved in the cars array
{
	for (int i = 0; i < 5; i++)
	{
		DrawSquare(Cars[i][0]+10, Cars[i][1]+20, 20, colors[MAROON]);
		DrawSquare(Cars[i][0]+1, Cars[i][1]+12, 20, colors[MAROON]);
		DrawSquare(Cars[i][0]+19, Cars[i][1]+12, 20, colors[MAROON]);
		DrawSquare(Cars[i][0]+14, Cars[i][1]+26, 12, colors[BLUE]);
		DrawCircle((Cars[i][0]+11) , (Cars[i][1]+7) , 8 , colors[BLUE]);
		DrawCircle((Cars[i][0]+29) , (Cars[i][1]+7) , 8 , colors[BLUE]);
	}
}

void DrawBoxes() // Draws the walls on the coordinates saved in the boxes array
{
	for (int i = 0; i < 3; i++)
	{
		DrawSquare((Boxes[i][0]+5) , (Boxes[i][1]+5) , 30 , colors[SANDY_BROWN]);
		DrawLine((Boxes[i][0]+5) , (Boxes[i][1]+10) , (Boxes[i][0]+35) , (Boxes[i][1]+25) , 3 , colors[GHOST_WHITE]);
	}
}

void DrawTrees() // Draws the walls on the coordinates saved in the trees array
{
	for (int i = 0; i < 4; i++)
	{
		DrawTriangle(Trees[i][0] , (Trees[i][1]+20) , (Trees[i][0]+40) , (Trees[i][1]+20) , (Trees[i][0]+20) , (Trees[i][1]+40) , colors[GREEN]);
		DrawLine((Trees[i][0]+20), (Trees[i][1]+20), (Trees[i][0]+20), (Trees[i][1]), 10 , colors[BROWN]);
	}
}

void PopulateGrid() // Draws the 20 by 20 grid
{
	int y = 0, x = 10;
	for (int i = 0; i < 20; i++)
	{
		for (int a = 0; a < 20; a++)
		{
			DrawSquare( x , y , 40 ,colors[LIGHT_GREEN]);
			x += 40;
			arr[i*a][0] = x;
			arr[i*a][1]= y;
		}
		y += 40;
		x = 10;
	}
}

void GameDisplay()
{
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

	glClearColor(0/*Red Component*/, 0,	//148.0/255/*Green Component*/,
			0.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); //Update the colors
	
	DrawString( 50, 800, "Score=0", colors[MISTY_ROSE]);
	
	PopulateGrid();
	drawCar();
	DrawWalls();
	DrawPassengers();
	DrawCars();
	DrawBoxes();
	DrawTrees();
	Destination();
	glutSwapBuffers(); // do not modify this line..

}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */


void PickPassenger() // Called when spacebar is pressed to check whether there is a passenger on any of the four sides of the taxi
{
	int prev = 40;
	if (PickStatus == -1)
	{
		for (int a = -1; a < 2; a++)
		{
			if (a == 0)
			{
				continue;
			}
			
			for (int b = 0; b < PassengerNo; b++)
			{
				if (xI + (prev*a) == Passengers[b][0])
				{
					if (yI == Passengers[b][1])
					{
						Passengers[b][0] = -1;
						Passengers[b][1] = -1;
						dropped = false;
						PickStatus = 0;
					}
				}
				if (yI + (prev*a) == Passengers[b][1])
				{
					if (xI == Passengers[b][0])
					{
						Passengers[b][0] = -1;
						Passengers[b][1] = -1;
						PickStatus = 0;
					}
				}
			}
		}
	}
}

void DropPassenger() // Called when spacebar is pressed to check whether there is a destination of the picked up passenger on any of the four sides of the taxi
{
	int prev = 40;
	for (int a = -1; a < 2; a++)
	{
		if (a == 0)
		{
			continue;
		}
		if (xI + (prev*a) == DestinationX)
		{
			if (yI == DestinationY)
			{
				PickStatus = 1;
			}
		}
		if (yI + (prev*a) == DestinationY)
		{
			if (xI == DestinationX)
			{
				PickStatus = 1;
			}
		}
	}
}

void CollisionScoring(int x, int y) // The score is evaluated each time there is a collision
{
	if (!CheckPassengers(x,y))
	{
		score -= 5;
	}
	else
	{
		if (carcolor == 1)
		{
			if (!CheckBoxes(x,y) or !CheckTrees(x,y))
			{
				score -= 2;
			}
			else if (CheckCars(x,y))
			{
				score -= 3;
			}
		}
		else
		{
			if (!CheckBoxes(x,y) or !CheckTrees(x,y))
			{
				score -= 4;
			}
			else if (CheckCars(x,y))
			{
				score -= 2;
			}
		}
	}
	ScoreCheck();
}

void NonPrintableKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/)
	{
		// what to do when left key is pressed...
		if (xI > 40)
		{
			if (CheckWall((xI-40),yI) and CheckPassengers((xI-40),yI) and CheckCars((xI-40),yI) and CheckBoxes((xI-40),yI) and CheckTrees((xI-40),yI)) //If none of the obstacles are on the left of the taxi, the taxi will move to the left
			{
				xI -= 40;
			}
			else
			{
				CollisionScoring(xI-40,yI);
			}
		}
	} 
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) 
	{
		if (xI < 760)
		{
			if (CheckWall((xI+40),yI) and CheckPassengers((xI+40),yI) and CheckCars((xI+40),yI) and CheckBoxes((xI+40),yI) and CheckTrees((xI+40),yI)) //If none of the obstacles are on the right of the taxi, the taxi will move to the right
			{
				xI += 40;
			}
			else
			{
				CollisionScoring(xI+40,yI);
			}
		}
	}
	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) 
	{
		if (yI < 760)
		{
			if (CheckWall(xI,(yI+40)) and CheckPassengers(xI,(yI+40)) and CheckCars(xI,(yI+40)) and CheckBoxes(xI,(yI+40)) and CheckTrees(xI,(yI+40))) //If none of the obstacles are above the taxi, the taxi will move up
			{
				yI += 40;
			}
			else
			{
				CollisionScoring(xI,yI+40);
			}
		}
	}

	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/)
	{
		if (yI > 0)
		{
			if (CheckWall(xI,(yI-40)) and CheckPassengers(xI,(yI-40)) and CheckCars(xI,(yI-40)) and CheckBoxes(xI,(yI-40)) and CheckTrees(xI,(yI-40))) //If none of the obstacles are below the taxi, the taxi will move down
			{
				yI -= 40;
			}
			else
			{
				CollisionScoring(xI,yI-40);
			}
		}
	}
	
	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/

	glutPostRedisplay();

}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y)
{
	if (key == 27/* Escape key ASCII*/)
	{
		exit(1); // exit the program when escape key is pressed.
	}
	if (key == 32)
	{
		PickPassenger();
		DropPassenger();
	}
	glutPostRedisplay();
}

bool flag1 = true;
void MoveCars1()
{
	if (flag1 == true)
	{
		Cars[0][0] -= 40;
		int posX = Cars[0][0]-40;
		int posY =  Cars[0][1];
		if (posX <= 40 or !CheckWall(posX,posY) or !CheckPassengers(posX,posY) or !CheckCars(posX,posY) or !CheckBoxes(posX,posY) or !CheckTrees(posX,posY))
		{
			flag1 = false;
		}
	}
	else if (flag1 == false)
	{
		Cars[0][0] += 40;
		int posX = Cars[0][0]+40;
		int posY =  Cars[0][1];
		if (posX >= 810 or !CheckWall(posX,posY) or !CheckPassengers(posX,posY) or !CheckCars(posX,posY) or !CheckBoxes(posX,posY) or !CheckTrees(posX,posY))
		{
			flag1 = true;
		}
	}
}


bool flag2 = true;
void MoveCars2()
{
	if (flag2 == true)
	{
	
		Cars[1][1] -= 40;
		int posX = Cars[1][0];
		int posY =  Cars[1][1]-40;
		if (posY <= 0 or !CheckWall(posX,posY) or !CheckPassengers(posX,posY) or !CheckCars(posX,posY) or !CheckBoxes(posX,posY) or !CheckTrees(posX,posY))
		{
			flag2 = false;
		}
	}
	else if (flag2 == false)
	{
		Cars[1][1] += 40;
		int posX = Cars[1][0];
		int posY =  Cars[1][1]+40;
		if (posY >= 800 or !CheckWall(posX,posY) or !CheckPassengers(posX,posY) or !CheckCars(posX,posY) or !CheckBoxes(posX,posY) or !CheckTrees(posX,posY))
		{
			flag2 = true;
		}
	}
}

bool flag3 = true;
void MoveCars3()
{
	if (flag3 == true)
	{
		Cars[2][1] -= 40;
		int posX = Cars[2][0];
		int posY =  Cars[2][1]-40;
		if (posY <= 0 or !CheckWall(posX,posY) or !CheckPassengers(posX,posY) or !CheckCars(posX,posY) or !CheckBoxes(posX,posY) or !CheckTrees(posX,posY))
		{
			flag3 = false;
		}
	}
	else if (flag3 == false)
	{
		Cars[2][1] += 40;
		int posX = Cars[2][0];
		int posY =  Cars[2][1]+40;
		if (posY >= 800 or !CheckWall(posX,posY) or !CheckPassengers(posX,posY) or !CheckCars(posX,posY) or !CheckBoxes(posX,posY) or !CheckTrees(posX,posY))
		{
			flag3 = true;
		}
	}
}

bool flag4 = true;
void MoveCars4()
{
	if (flag4 == true)
	{
		Cars[3][1] -= 40;
		int posX = Cars[3][0];
		int posY =  Cars[3][1]-40;
		if (posY <= 0 or !CheckWall(posX,posY) or !CheckPassengers(posX,posY) or !CheckCars(posX,posY) or !CheckBoxes(posX,posY) or !CheckTrees(posX,posY))
		{
			flag4 = false;
		}
	}
	else if (flag4 == false)
	{
		Cars[3][1] += 40;
		int posX = Cars[3][0];
		int posY =  Cars[3][1]+40;
		if (posY >= 800 or !CheckWall(posX,posY) or !CheckPassengers(posX,posY) or !CheckCars(posX,posY) or !CheckBoxes(posX,posY) or !CheckTrees(posX,posY))
		{
			flag4 = true;
		}
	}
}

/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m) {

	// implement your functionality here
	MoveCars1();
	MoveCars2();
	MoveCars3();
	MoveCars4();

	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(100, Timer, 0);
}


/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y) {
	cout << x << " " << y << endl;
	glutPostRedisplay();
}
void MouseMoved(int x, int y) {
	//cout << x << " " << y << endl;
	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
			{
		cout << GLUT_DOWN << " " << GLUT_UP << endl;

	} else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
			{
			cout<<"Right Button Pressed"<<endl;

	}
	glutPostRedisplay();
}


/*
 * our gateway main function
 * */
 

void InitializeArray() // The scores array is initlialized to -10000 in case there are less than 10 records in the start
{
	for (int i = 0; i < 11; i++)
	{
		for (int x = 0; x < 2; x++)
		{
			Scores[i][x] = "-10000";
		}
	}
}

int main(int argc, char*argv[])
{
	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.
	int option = Menu();
	if (option == 1)
	{
		InitializeArray();
		MenuFirstOption();
		int width = 840, height = 840; // i have set my window size to be 800 x 600
		//srand(time(0));
		InitRandomizer(); // seed the random number generator...
		glutInit(&argc, argv); // initialize the graphics library...
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
		glutInitWindowPosition(50, 50); // set the initial position of our window
		glutInitWindowSize(width, height); // set the size of our window
		glutCreateWindow("Rush Hour by Fatima Athar Khan 21I-0385"); // set the title of our game window
		SetCanvasSize(width, height); // set the number of pixels...
		
		PopulateCars();
		PopulateWalls();
		PopulatePassengers();
		PopulateBoxes();
		PopulateTrees();
		glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
		glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
		glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
		// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
		glutTimerFunc(100.0, Timer, 0);
		
		glutTimerFunc(180000.0, Finish, 0);
	
		glutMouseFunc(MouseClicked);
		glutPassiveMotionFunc(MouseMoved); // Mouse
		glutMotionFunc(MousePressedAndMoved); // Mouse
	
		// now handle the control to library and it will call our registered functions when
		// it deems necessary...
		glutMainLoop();
	}
	
	else if (option == 2)
	{
		MenuSecondOption();
	}
	return 1;
}
#endif /* RushHour_CPP_ */
