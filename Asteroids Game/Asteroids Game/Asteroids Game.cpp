// Asteroids Game.cpp 
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

#include "olcConsoleGameEngine.h"

class OneLoneCoder_Asteroids : public olcConsoleGameEngine
{

public:
	OneLoneCoder_Asteroids()
	{
		m_sAppName = L"Asteroids";
	}


private:
	struct sSpaceObject //struct to represent object
	{
		// tells us where the object is in "space"
		float x; //x coordinate
		float y; //y coordinate
		////velocity coordinates (the speed and direction that the object is traveling in)
		float dx; //velocity x coordinate
		float dy; //velocity y coordinate
		int nSize; //most of the objects will be asteroids and they will change in size
	};
	//vectors to store the objects in the game
	vector<sSpaceObject>  vecAsteroids; //this vector will store the Asteroids


protected:
	//called by olcConsoleGameEngine
	//called when the application launches
	virtual bool OnUserCreate()
	{
								// x,  	y		velocities
		vecAsteroids.push_back({ 20.0f, 20.0f, 8.0f, -6.0f, (int)16 }); // populates vector with a single Asteroid, using the initializer list to do this

		return true;
	}

	//called by olcConsoleGameEngine
	//this function updates the screen
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		// Clear Screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, 0);

		// update and draw asteroids
		for (auto& a : vecAsteroids) // to iterate through the vector
		{
			// fElapseTime is the update between successive frames, the time taken between one frame and the next
			//this allows us to run the program at a known speed, across a variety of different computing systems
			a.x += a.dx * fElapsedTime; //the x position of the asteroid needs to be updated by the velocity vector, multiplied by the elapsed time
			a.y += a.dy * fElapsedTime; //same as above, but the y coordinates now
			WrapCoordinates(a.x, a.y, a.x, a.y); //passes in the current x and y position into the function and then get them back

			// this loop is to draw the asteroids
			for (int x = 0; x < a.nSize; x++)
				for (int y = 0; y < a.nSize; y++)
					Draw(a.x + x, a.y + y, PIXEL_QUARTER, FG_RED);
		}
		return true;
	}

	void WrapCoordinates(float ix, float iy, float &ox, float &oy) // ix and iy are the inputs, outputs ox and oy
	{
		// sets the output to the input
		ox = ix;
		oy = iy;
		// then check if the input is beyond the boundaries of our array
		// if the input x is less than 0, then I want to adjust my output to be the input x plus the current screen width, which is an int (why we used a float)
		if (ix < 0.0f) ox = ix + (float)ScreenWidth();
		// if the input x is greater than the screen width, then we update the output again
		if (ix >= ScreenWidth()) ox = ix - (float)ScreenWidth();
		
		// if the input y is less than 0, then I want to adjust my output to be the input y plus the current screen width, which is an int (why we used a float)
		if (iy < 0.0f) ox = ix + (float)ScreenHeight();
		// if the input y is greater than the screen width, then we update the output again
		if (iy >= ScreenHeight()) ox = ix - (float)ScreenHeight();

	}
};

int main()
{
	// Use olcConsoleGameEngine derived app
	//creates an instance of the game engine
	OneLoneCoder_Asteroids game;
	// set it to 160 characters wide, by 100 characters tall, and each character is 8 by 8 pixels
	game.ConstructConsole(160, 100, 8, 8);
	game.Start();
	return 0;
}