#include<SFML/Graphics.hpp>

using namespace sf;

View viewCamera;

View getPlayerCoordXY(float x, float y)
{
	float cordX = x, cordY = y;
	
	if (x < 320) cordX = 320;
	if (y < 239) cordY = 239;
	if (y > 615) cordY = 615;
	if (x > 1100) cordX = 1100;

	viewCamera.setCenter(cordX, cordY);
	return viewCamera;
}