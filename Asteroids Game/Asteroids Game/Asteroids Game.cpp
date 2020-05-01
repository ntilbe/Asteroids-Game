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
		float angle;
	};
	//vectors to store the objects in the game
	vector<sSpaceObject>  vecAsteroids; //this vector will store the Asteroids
	sSpaceObject player;

protected:
	//called by olcConsoleGameEngine
	//called when the application launches
	virtual bool OnUserCreate()
	{
								// x,  	y		velocities
		vecAsteroids.push_back({ 20.0f, 20.0f, 8.0f, -6.0f, (int)16, 0.0f }); // populates vector with a single Asteroid, using the initializer list to do this

		//initialize player position
		player.x = ScreenWidth() / 2.0f;
		player.y = ScreenHeight() / 2.0f;
		player.dx = 0.0f;
		player.dy = 0.0f;
		player.angle = 0.0f;

		return true;
	}

	//called by olcConsoleGameEngine
	//this function updates the screen
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		// Clear Screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, 0);

		//Steers the ship
		if (m_keys[VK_LEFT].bHeld)
			player.angle -= 5.0f * fElapsedTime; //steering left
		if (m_keys[VK_RIGHT].bHeld)
			player.angle += 5.0f * fElapsedTime; //steering right

		//Thrust
		if (m_keys[VK_UP].bHeld) // thrusts with the up arrow
		{
			//since we don't store thrust as part of our space object, we need to update the velocity
			// ACCELERATION changes VELOCITY (with respect to time)
			player.dx += sin(player.angle) * 20.0f * fElapsedTime; //changing the x velocity by sin of the player angle multiplied by a random number (20), multiplied by the elapsed time
			player.dy += -cos(player.angle) * 20.0f * fElapsedTime;//altering stuff in the y axis uses the cos function
			//by doing this, we know that we get a vector related to the angle that the player is pointing in
			//we can use that vector as our velocity vector
		}

		//VELOCITY changes POSITION (with respect to time)
		player.x += player.dx * fElapsedTime; //takes the player's x-coordinate and update it by its velocity vector that we've just calculated right above, multiplied by the elapsed time
		player.y += player.dy * fElapsedTime;

		// keep ship within the game space
		WrapCoordinates(player.x, player.y, player.x, player.y);

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

		//Draw ship - define our ship as a set of 3 points
		float mx[3] = { 0.0f, -2.5f, +2.5f }; //ship model vertices
		float my[3] = { -5.5f, +2.5f, +2.5f };

		//created two more arrays to store the transformed points
		//the model above never changes, but what we draw to the screen is the transformation below (this is how all wire frame AND 3D graphics work)
		float sx[3], sy[3];

		//Rotate
		for (int i = 0; i < 3; i++) //this loop goes through all of the points in the model and multiplies them (like the matrix versus the vector)
		{
			//the angle we want to rotate by is the "player.angle", so we can rotate the model triangle in the direction the player is facing 
			sx[i] = mx[i] * cosf(player.angle) - my[i] * sinf(player.angle);
			sy[i] = mx[i] * sinf(player.angle) + my[i] * cosf(player.angle);
		}

		//Translate
		//once the model has been rotated, we then need to offset it to where the player currently is (set it to player's x and y coordinates)
		for (int i = 0; i < 3; i++)
		{
			sx[i] = sx[i] + player.x; //players x coordinate where the position vector of the spaceship is
			sy[i] = sy[i] + player.y; //players y coordinate
		}

		//Draw closed polygon
		for (int i = 0; i < 4; i++) //loops through all the points and draws lines between them
		{
			int j = (i + 1);
			DrawLine(sx[i % 3], sy[i % 3], sx[j % 3], sy[j % 3]); //4 points
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
		if (ix >= (float)ScreenWidth()) ox = ix - (float)ScreenWidth();
		
		// if the input y is less than 0, then I want to adjust my output to be the input y plus the current screen width, which is an int (why we used a float)
		if (iy < 0.0f) oy = iy + (float)ScreenHeight();
		// if the input y is greater than the screen width, then we update the output again
		if (iy >= (float)ScreenHeight()) oy = iy - (float)ScreenHeight();

	}
	//overrides the draw function
	virtual void Draw(int x, int y, short c = 0x2588, short col = 0x000F)
	{
		float fx, fy;
		WrapCoordinates(x, y, fx, fy);
		olcConsoleGameEngine::Draw(fx, fy, c, col);


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