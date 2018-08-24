#include<SFML/Graphics.hpp>

using namespace sf;

View viewCamera;

View getPlayerCoordXY(float x, float y)
{
	float cordX = x, cordY = y;
	
	if (x < 320) cordX = 320;
	if (y < 240) cordY = 240;
	if (y > 640) cordY = 640;
	if (x > 1120) cordX = 1120;

	viewCamera.setCenter(cordX, cordY);
	return viewCamera;
}