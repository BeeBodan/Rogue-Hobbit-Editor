#include<SFML/Graphics.hpp>

using namespace sf;

View viewCamera;

View getPlayerCoordXY(float x, float y)
{
	float cordX = x, cordY = y;
	
	if (x < 320) cordX = 320;
	if (y < 239) cordY = 239;
	if (y > 560) cordY = 560;
	if (x > 958) cordX = 958;

	viewCamera.setCenter(cordX, cordY);
	return viewCamera;
}