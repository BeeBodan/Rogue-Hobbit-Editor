#define SFML_NO_DEPRECATED_WARNINGS
#include<SFML/Graphics.hpp>
#include<iostream>
#include<sstream>
#include<conio.h>
#include <vector>
#include <list>
#include <Windows.h>
#include "QuestMenu.h"
#include "Camera.h"
#include "level.h"
#include "TinyXML/tinyxml.h"

#include "menu.h"

using namespace sf;
using namespace std;

//=============================Get the horizontal and vertical screen sizes in pixel============================
void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}


//===================================================MAIN CLASS==================================================
class MainClass
{
public:
	vector<Object> obj;
	float x, y, dx, dy, speed, TimerEnemy, TimerHobbit, TimerShoot, TimerDamage, TimerDamageGetOut;
	float frame;
	int w, h, health;
	bool life;
	Texture texture;
	Sprite sprite;
	String name;

	MainClass(Image &image, float X, float Y, int W, int H, String Name)
	{
		x = X, y = Y; w = W; h = H; name = Name;
		TimerEnemy = 0; TimerHobbit = 0; TimerShoot = 0; TimerDamage = 0; TimerDamageGetOut = 0; speed = 0; dx = 0; dy = 0;
		frame = 0;
		health = 100; life = true;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}

	FloatRect getRect()
	{
		return FloatRect(x, y, w, h);
	}
	virtual void update(float mainTime) = 0;
};



//===================================================CLASS PLAYER==================================================
class Class_Hero :public MainClass
{
public:
	enum { left, right, up, down, stay } state;
	int stonesPoint, elixir, enemyPoint, GetOutFrom;
	bool  cooldown = false, hitDamage = true, hitDamageGetOut1 = true, hitDamageGetOut2 = true, hitDamageGetOut3 = true, hitDamageGetOut4 = true, Press = false;

	Class_Hero(Image &image, Level &lvl, float X, float Y, int W, int H, String Name) :MainClass(image, X, Y, W, H, Name)     //lvl!!!!!
	{
		stonesPoint = 0; state = stay; elixir = 0, enemyPoint = 0;
		obj = lvl.GetAllObjects();
		sprite.setTextureRect(IntRect(0, 0, w, h));
		sprite.setPosition(x + w / 2, y + h / 2);
	}



	void move_hero()
	{
		if (life == true)
		{
			if (hitDamage == true)
			{
				Press = false;
				if (Keyboard::isKeyPressed)
				{
					if (Keyboard::isKeyPressed(Keyboard::A))
					{
						state = left; speed = 0.07; Press = true;
					}
					if (Keyboard::isKeyPressed(Keyboard::D))
					{
						state = right; speed = 0.07; Press = true;
					}
					if (Keyboard::isKeyPressed(Keyboard::S))
					{
						state = up; speed = 0.07; Press = true;
					}
					if (Keyboard::isKeyPressed(Keyboard::W))
					{
						state = down; speed = 0.07; Press = true;
					}
				}
			}
		}
	}

	void LogicMap(float Dx, float Dy)
	{
		for (int i = 0; i<obj.size(); i++)
		{
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "Solid")
				{
					if (Dy > 0) { y = obj[i].rect.top - h; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height; }
					if (Dx > 0) { x = obj[i].rect.left - w; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}
		}
	}

	void Animation(float mainTime)
	{
		if (life == true)
		{
			if (hitDamage == true)
			{
				if (state == left) { sprite.setTextureRect(IntRect(0, 35, 32, 35)); }
				if (state == right) { sprite.setTextureRect(IntRect(0, 0, 32, 35)); }
				if (state == up) { sprite.setTextureRect(IntRect(0, 105, 32, 35)); }
				if (state == down) { sprite.setTextureRect(IntRect(0, 70, 32, 35)); }

				if (Keyboard::isKeyPressed(Keyboard::A))
				{
					frame += 0.005 * mainTime;
					if (frame > 5) frame -= 5;
					sprite.setTextureRect(IntRect(32 * int(frame), 35, 32, 35));
				}
				if (Keyboard::isKeyPressed(Keyboard::D))
				{
					frame += 0.005 * mainTime;
					if (frame > 5) frame -= 5;
					sprite.setTextureRect(IntRect(32 * int(frame), 0, 32, 35));
				}
				if (Keyboard::isKeyPressed(Keyboard::W))
				{
					frame += 0.005 * mainTime;
					if (frame > 5) frame -= 5;
					sprite.setTextureRect(IntRect(32 * int(frame), 70, 32, 35));
				}
				if (Keyboard::isKeyPressed(Keyboard::S))
				{
					frame += 0.005 * mainTime;
					if (frame > 5) frame -= 5;
					sprite.setTextureRect(IntRect(32 * int(frame), 105, 32, 35));
				}
			}
			if (hitDamageGetOut1 == false)
			{
				frame += 0.009 * mainTime;
				if (frame > 5) frame -= 5;
				sprite.setTextureRect(IntRect(32 * int(frame), 175, 32, 35));
			}
			if (hitDamageGetOut2 == false)
			{
				frame += 0.009 * mainTime;
				if (frame > 5) frame -= 5;
				sprite.setTextureRect(IntRect(32 * int(frame), 140, 32, 35));
			}
			if (hitDamageGetOut3 == false)
			{
				frame += 0.009 * mainTime;
				if (frame > 5) frame -= 5;
				sprite.setTextureRect(IntRect(32 * int(frame), 210, 32, 35));
			}
			if (hitDamageGetOut4 == false)
			{
				frame += 0.009 * mainTime;
				if (frame > 5) frame -= 5;
				sprite.setTextureRect(IntRect(32 * int(frame), 245, 32, 35));
			}
		}
		else
		{
			sprite.setTextureRect(IntRect(0, 280, 38, 38));
		}
	}

	void update(float mainTime)
	{
		Animation(mainTime);
		move_hero();

		switch (state)
		{
		case right:dx = speed; dy = 0;
			if (hitDamageGetOut1 == false)
			{
				if (Press == false) { dx = 0.09; }
				dx = 0.09;
			}
			if (hitDamageGetOut2 == false)
			{
				if (Press == false) { dx = -0.09; }
				dx = -0.09;
			}
			if (hitDamageGetOut3 == false)
			{
				if (Press == false) { dy = 0.09; }
				dx = -0.09;
			}
			if (hitDamageGetOut4 == false)
			{
				if (Press == false) { dy = -0.09; }
				dx = -0.09;
			}
			break;
		case left:dx = -speed; dy = 0;
			if (hitDamageGetOut1 == false)
			{
				if (Press == false) { dx = 0.09; }
				dx = 0.09; 
			}
			if (hitDamageGetOut2 == false)
			{
				if (Press == false) { dx = -0.09; }
				dx = -0.09;
			}
			if (hitDamageGetOut3 == false)
			{
				if (Press == false) { dy = 0.09; }
				dx = 0.09;
			}
			if (hitDamageGetOut4 == false)
			{
				if (Press == false) { dy = -0.09; }
				dx = 0.09;
			}
			break;
		case up: dx = 0; dy = speed; //this down:)
			if (hitDamageGetOut1 == false)
			{
				if (Press == false) { dx = 0.09; }
				dy = -0.09; 
			}
			if (hitDamageGetOut2 == false)
			{
				if (Press == false) { dx = -0.09; }
				dy = -0.09;
			}
			if (hitDamageGetOut3 == false)
			{
				if (Press == false) { dy = 0.09; }
				dy = 0.09;
			}
			if (hitDamageGetOut4 == false)
			{
				if (Press == false) { dy = -0.09; }
				dy = -0.09;
			}
			break;
		case down: dx = 0; dy = -speed; //this up:)
			if (hitDamageGetOut1 == false)
			{
				if (Press == false) { dx = 0.09; }
				dy = 0.09; 
			} 
			if (hitDamageGetOut2 == false)
			{
				if (Press == false) { dx = -0.09; }
				dy = 0.09;
			}
			if (hitDamageGetOut3 == false)
			{
				if (Press == false) { dy = 0.09; }
				dy = 0.09;
			}
			if (hitDamageGetOut4 == false)
			{
				if (Press == false) { dy = -0.09; }
				dy = -0.09;
			}
			break;
		case stay:
			break;
		}

		x += dx*mainTime;
		LogicMap(dx, 0);

		y += dy*mainTime;
		LogicMap(0, dy);

		speed = 0;

		TimerDamageGetOut += mainTime;
		if (TimerDamageGetOut > 500)
		{
			hitDamageGetOut1 = true;
			hitDamageGetOut2 = true;
			hitDamageGetOut3 = true;
			hitDamageGetOut4 = true;
		}

		TimerDamage += mainTime;
		if (TimerDamage > 1000)
		{
			hitDamage = true;
		}

		TimerHobbit += mainTime;
		if (TimerHobbit > 3000)
		{
			cooldown = true;
		}

		sprite.setPosition(x + w / 2, y + h / 2);
		if (health <= 0) { life = false; }
		if (life) { getPlayerCoordXY(x + 80, y + 50); }    //80 and 50 for interface (center)
	}
};



//===================================================CLASS ENEMIES==================================================
class Class_Enemy :public MainClass
{
public:
	Class_Enemy(Image &image, Level &lvl, float X, float Y, int W, int H, String Name) :MainClass(image, X, Y, W, H, Name)
	{
		obj = lvl.GetObjects("Solid");
		if (name == "easyEnemy" || name == "easyEnemy2")
		{
			dx = -0.05;
			dy = -0.05;
		}
	}

	void LogicMapEnemy(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)
		{
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "Solid")
				{
					if (Dy > 0) { y = obj[i].rect.top - h; dy = -0.05; sprite.scale(-1, 1); }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height; dy = 0.05; sprite.scale(-1, 1); }
					if (Dx > 0) { x = obj[i].rect.left - w; dx = -0.05; sprite.scale(-1, 1); }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.05; sprite.scale(-1, 1); }
				}
			}
		}
	}

	void Animation(float mainTime)
	{
		if (health > 0)
		{
			if (name == "easyEnemy")
			{
				frame += 0.005 * mainTime;
				if (frame > 6) frame -= 6;
				sprite.setTextureRect(IntRect(80 * int(frame), 0, w, h));

				TimerEnemy += mainTime;
				if (TimerEnemy > 3000)
				{
					dx *= -1; TimerEnemy = 0;
					sprite.scale(-1, 1);
				}

				sprite.setPosition(x + w / 2, y + h / 2);
			}
		}
		else
		{
			sprite.setTextureRect(IntRect(412, 420, 50, 23));   //Dead enemy
		}

		if (health > 0)
		{
			if (name == "easyEnemy2")
			{
				if (dy > 0)
				{
					frame += 0.005 * mainTime;
					if (frame > 4) frame -= 4;
					sprite.setTextureRect(IntRect(80 * int(frame), 227, w, h));
				}
				else
				{
					frame += 0.005 * mainTime;
					if (frame > 4) frame -= 4;
					sprite.setTextureRect(IntRect(80 * int(frame), 346, w, h));
				}

				TimerEnemy += mainTime;
				if (TimerEnemy > 4000)
				{
					dy *= -1; TimerEnemy = 0;
				}
				sprite.setPosition(x + w / 2, y + h / 2);
			}
		}
		else
		{
			sprite.setTextureRect(IntRect(412, 420, 50, 23));   //Dead enemy
		}
	}

	void update(float mainTime)
	{
		Animation(mainTime);

		if (health > 0)
		{
			if (name == "easyEnemy")
			{
				LogicMapEnemy(dx, 0);
				x += dx*mainTime;
			}
		}
		if (health > 0)
		{
			if (name == "easyEnemy2")
			{
				LogicMapEnemy(0, dy);
				y += dy*mainTime;
			}
		}
	}
};

//===================================================CLASS THINGS==================================================
class Class_Things :public MainClass
{
public:
	Class_Things(Image &image, Level &lvl, float X, float Y, int W, int H, String Name) :MainClass(image, X, Y, W, H, Name)
	{

	}

	void update(float mainTime)
	{
		if (name == "simpleStone")
		{
			sprite.setTextureRect(IntRect(608, 767, w, h));
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
		if (name == "elixirHP")
		{
			sprite.setTextureRect(IntRect(65, 800, w, h));
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
	}
};

//===================================================CLASS SHOOTING==================================================
class Class_Shooting :public MainClass
{
public:
	int line; //direction shoot

	Class_Shooting(Image &image, Level &lvl, float X, float Y, int W, int H, String Name, int dir) :MainClass(image, X, Y, W, H, Name)
	{
		obj = lvl.GetObjects("Solid");
		x = X;
		y = Y;
		w = 22, h = 14;
		line = dir;
		speed = 0.18;
		life = true;
	}

	void update(float mainTime)
	{
		switch (line)
		{
		case 0: dx = -speed; dy = 0; TimerShoot += mainTime; sprite.setRotation(sprite.getRotation() + 10); if (TimerShoot > 500) { y++; } break;
		case 1: dx = speed; dy = 0; TimerShoot += mainTime; sprite.setRotation(sprite.getRotation() + 10); if (TimerShoot > 500) { y++; } break;
		case 2: dx = 0; dy = speed; TimerShoot += mainTime; sprite.setRotation(sprite.getRotation() + 10); break;
		case 3: dx = 0; dy = -speed; TimerShoot += mainTime; sprite.setRotation(sprite.getRotation() + 10); break;
		default:
			break;
		}

		x += dx * mainTime;
		y += dy * mainTime;

		if (TimerShoot > 1000)
		{
			life = false;
		}

		for (int i = 0; i < obj.size(); i++)
		{
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "Solid")
				{
					life = false;
				}
			}
		}
		
		sprite.setTextureRect(IntRect(0, 0, w, h));
		sprite.setPosition(x + w / 2, y + h / 2);
	}
};


//===================================================CLASS HEALTHBAR==================================================
class Class_HealthBar
{
public:
	Image image_bar;
	Texture texture_bar;
	Sprite sprite_bar;
	int max;
	RectangleShape bar;

	Class_HealthBar()
	{
		image_bar.loadFromFile("images/Health_bar_little.png");
		image_bar.createMaskFromColor(Color(50, 96, 166));
		texture_bar.loadFromImage(image_bar);
		sprite_bar.setTexture(texture_bar);
		sprite_bar.setTextureRect(IntRect(22, 30, 66, 11));

		max = 100;
		bar.setFillColor(Color(0, 0, 0));
	}

	void update(int health)
	{
		if (health > 0)
			//if (health < max)
			bar.setSize(Vector2f((max - health) * 60 / max, 5));
	}
	void draw(RenderWindow &window)
	{
		sprite_bar.setPosition(viewCamera.getCenter().x + 185, viewCamera.getCenter().y - 155);
		bar.setPosition(viewCamera.getCenter().x + 188, viewCamera.getCenter().y - 152);

		window.draw(sprite_bar);
		window.draw(bar);
	}
};

//==============================================================================================================
//===================================================MAIN PART==================================================
//==============================================================================================================
int main()
{
	int horizontal = 0;
	int vertical = 0;
	GetDesktopResolution(horizontal, vertical);
	RenderWindow window(VideoMode(horizontal, vertical), "HobbitRogueEdition", Style::Fullscreen);
	//RenderWindow window(VideoMode(1280, 800), "HobbiRogueEdition");
	viewCamera.reset(FloatRect(0, 0, 640, 480));

	//________text for 'Health and stones'_______
	Font font1;
	font1.loadFromFile("HobbitFont2.ttf");
	Text text1("", font1, 11);
	//text.setStyle(Text::Bold);
	text1.setColor(Color::Black);

	//________text for 'Quest'__________
	Font font2;
	font2.loadFromFile("HobbitFont3.ttf");
	Text text2("", font2, 11);
	text2.setStyle(Text::Bold);
	text2.setColor(Color::Black);

	//________text for 'Dead'________
	Font font3;
	font3.loadFromFile("HobbitFont2.ttf");
	Text text3("", font3, 60);
	//text3.setStyle(Text::Bold);
	text3.setColor(Color::Red);

	//_________Interface__________
	Image image_iterface;
	image_iterface.loadFromFile("images/MIDDLE.png");
	Texture texture_interface;
	texture_interface.loadFromImage(image_iterface);
	Sprite sprite_interface;
	sprite_interface.setTexture(texture_interface);
	sprite_interface.setTextureRect(IntRect(0, 0, 640, 480));

	//_________Auch____________
	Image auch_image;
	auch_image.loadFromFile("images/Auch.png");
	Texture auch_texture;
	auch_texture.loadFromImage(auch_image);
	Sprite auch_sprite;
	auch_sprite.setTexture(auch_texture);
	auch_sprite.setTextureRect(IntRect(0, 0, 32, 16));

	//_________Load Map________
	Level lvl;
	lvl.LoadFromFile("MapFour.tmx");
	

	//_________image for 'elixir 1-5'__________
	Image image_elixir0, image_elixir1, image_elixir2, image_elixir3, image_elixir4, image_elixir5;
	image_elixir0.loadFromFile("images/Elixir0little.png");
	image_elixir1.loadFromFile("images/Elixir1little.png");
	image_elixir2.loadFromFile("images/Elixir2little.png");
	image_elixir3.loadFromFile("images/Elixir3little.png");
	image_elixir4.loadFromFile("images/Elixir4little.png");
	image_elixir5.loadFromFile("images/Elixir5little.png");
	Texture texture_elixir0, texture_elixir1, texture_elixir2, texture_elixir3, texture_elixir4, texture_elixir5;
	texture_elixir0.loadFromImage(image_elixir0);
	texture_elixir1.loadFromImage(image_elixir1);
	texture_elixir2.loadFromImage(image_elixir2);
	texture_elixir3.loadFromImage(image_elixir3);
	texture_elixir4.loadFromImage(image_elixir4);
	texture_elixir5.loadFromImage(image_elixir5);
	Sprite sprite_elixir0, sprite_elixir1, sprite_elixir2, sprite_elixir3, sprite_elixir4, sprite_elixir5;
	sprite_elixir0.setTexture(texture_elixir0);
	sprite_elixir1.setTexture(texture_elixir1);
	sprite_elixir2.setTexture(texture_elixir2);
	sprite_elixir3.setTexture(texture_elixir3);
	sprite_elixir4.setTexture(texture_elixir4);
	sprite_elixir5.setTexture(texture_elixir5);

	Image heroImage, shooting, objects, enemy;
	heroImage.loadFromFile("images/HeroDmg.png");
	objects.loadFromFile("images/object.png");
	shooting.loadFromFile("images/shoot.png");
	enemy.loadFromFile("images/dino.png");

	//=================================================CLASS DECLARATION================================================
	list<MainClass*> bigList;
	list<MainClass*>::iterator Iterator;
	list<MainClass*>::iterator Iterator2;
	list<MainClass*>::iterator Iterator3;

	vector<Object> enemies, enemies2, stones, elixirHP;

	enemies = lvl.GetObjects("EasyEnemy");
	for (int i = 0; i < enemies.size(); i++)
	{
		bigList.push_back(new Class_Enemy(enemy, lvl, enemies[i].rect.left, enemies[i].rect.top, 80, 50, "easyEnemy"));
	}
	enemies2 = lvl.GetObjects("EasyEnemy2");
	for (int i = 0; i < enemies2.size(); i++)
	{
		bigList.push_back(new Class_Enemy(enemy, lvl, enemies2[i].rect.left, enemies2[i].rect.top, 80, 50, "easyEnemy2"));
	}
	stones = lvl.GetObjects("Stone");
	for (int i = 0; i < stones.size(); i++)
	{
		bigList.push_back(new Class_Things(objects, lvl, stones[i].rect.left, stones[i].rect.top, 32, 32, "simpleStone"));
	}
	elixirHP = lvl.GetObjects("ElixirHP");
	for (int i = 0; i < elixirHP.size(); i++)
	{
		bigList.push_back(new Class_Things(objects, lvl, elixirHP[i].rect.left, elixirHP[i].rect.top, 32, 32, "elixirHP"));
	}

	Object player = lvl.GetObject("Player");
	Class_Hero Hobbit(heroImage, lvl, player.rect.left, player.rect.top, 32, 32, "player1");

	//_____Some variables_____
	bool showMissionText = true;
	Clock clock;
	float frame = 0;
	Class_HealthBar HealthBarHobbit;

	while (window.isOpen())
	{

		float mainTime = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		mainTime = mainTime / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
				if ((event.key.code == Keyboard::Escape))
					window.close();

			//_____________T(elixir)____________
			if (Hobbit.life == true)
			{
				if (event.type == Event::KeyPressed)
					if (event.key.code == Keyboard::T)
					{
						for (int i = Hobbit.elixir; i > 0; i++)
						{
							if (Hobbit.health > 80)
							{
								Hobbit.health = 100;
								Hobbit.elixir--;
								cout << "Bottles: " << Hobbit.elixir << endl;
								cout << "+20 health!" << endl;
							}
							else
							{
								Hobbit.health += 20;
								Hobbit.elixir--;
								cout << "Bottles: " << Hobbit.elixir << endl;
								cout << "+20 health!" << endl;
							}
							break;
						}
					}
			}

			//_____________Space(FIRE)____________
			if (Hobbit.life == true)
			{
				if (event.type == Event::KeyPressed)
					if (event.key.code == Keyboard::Space)
					{
						if (Hobbit.stonesPoint >= 1)
						{
							if (Hobbit.cooldown == true)
							{
								bigList.push_back(new Class_Shooting(shooting, lvl, Hobbit.x, Hobbit.y, 22, 14, "ShootStone", Hobbit.state));
								Hobbit.stonesPoint--;
								Hobbit.TimerHobbit = 0; Hobbit.cooldown = false;
							}

						}
					}
			}
		}


		//================================================MAIN LOGIC FOR ALL (UPDATE)===============================================
		Hobbit.update(mainTime);

		//________________Main Logic_________________
		if (Hobbit.life == true)
		{
			for (Iterator = bigList.begin(); Iterator != bigList.end();)
			{
				MainClass *all = *Iterator; //(*Iterator)->
				all->update(mainTime);
				if (all->life == false)
				{
					Iterator = bigList.erase(Iterator); delete all;
				}
				else Iterator++;
			}
			//________________Logic for Enemies_____________
			for (Iterator = bigList.begin(); Iterator != bigList.end(); Iterator++)
			{
				if ((*Iterator)->getRect().intersects(Hobbit.getRect()))
				{
					if ((*Iterator)->name == "easyEnemy")
					{
						if ((*Iterator)->health > 0)
						{
							(*Iterator)->dx *= -1;
							(*Iterator)->sprite.scale(-1, 1);

							if (Hobbit.dx < 0 && Hobbit.hitDamage == true) 
							{ 
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 5;
								Hobbit.hitDamageGetOut1 = false; Hobbit.TimerDamageGetOut = 0;
								Hobbit.x = (*Iterator)->x + (*Iterator)->w; cout << "I go on mob 111" << endl;
							}
							if (Hobbit.dx > 0 && Hobbit.hitDamage == true) 
							{
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 5;
								Hobbit.hitDamageGetOut2 = false; Hobbit.TimerDamageGetOut = 0;
								Hobbit.x = (*Iterator)->x - Hobbit.w; cout << "I go on mob 222" << endl;
							}
							if (Hobbit.dy < 0 && Hobbit.hitDamage == true)
							{
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 10;
								Hobbit.hitDamageGetOut3 = false; Hobbit.TimerDamageGetOut = 0;
								Hobbit.y = (*Iterator)->y + (*Iterator)->h;
							}
							if (Hobbit.dy > 0 && Hobbit.hitDamage == true)
							{ 
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 10;
								Hobbit.hitDamageGetOut4 = false; Hobbit.TimerDamageGetOut = 0;
								Hobbit.y = (*Iterator)->y - Hobbit.h;
							}


							if ((*Iterator)->dx < 0 && Hobbit.hitDamage == true) //move right!
							{ 
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 5;
								Hobbit.hitDamageGetOut1 = false; Hobbit.TimerDamageGetOut = 0; cout << "Mob go on me 111" << endl;
							}
							if ((*Iterator)->dx > 0 && Hobbit.hitDamage == true) //move left!
							{
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 5;
								Hobbit.hitDamageGetOut2 = false; Hobbit.TimerDamageGetOut = 0; cout << "Mob go on me 222" << endl;
							}
							/*if ((*Iterator)->dy < 0 && Hobbit.hitDamage == true) 
							{
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 10; cout << "f3" << endl;
								Hobbit.hitDamageGetOut1 = false; Hobbit.TimerDamageGetOut = 0;
							}
							if ((*Iterator)->dy > 0 && Hobbit.hitDamage == true) 
							{
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 10; cout << "f4" << endl;
								Hobbit.hitDamageGetOut1 = false; Hobbit.TimerDamageGetOut = 0;
							}*/
						}
					}
					if ((*Iterator)->name == "easyEnemy2")
					{
						if ((*Iterator)->health > 0)
						{
							(*Iterator)->dy *= -1;
							(*Iterator)->sprite.scale(-1, 1);

							if (Hobbit.dx < 0 && Hobbit.hitDamage == true)
							{
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 10;
								Hobbit.hitDamageGetOut1 = false; Hobbit.TimerDamageGetOut = 0;
								Hobbit.x = (*Iterator)->x + (*Iterator)->w;
							}
							if (Hobbit.dx > 0 && Hobbit.hitDamage == true)
							{
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 10;
								Hobbit.hitDamageGetOut2 = false; Hobbit.TimerDamageGetOut = 0;
								Hobbit.x = (*Iterator)->x - Hobbit.w;
							}
							if (Hobbit.dy < 0 && Hobbit.hitDamage == true)
							{
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 10;
								Hobbit.hitDamageGetOut3 = false; Hobbit.TimerDamageGetOut = 0;
								Hobbit.y = (*Iterator)->y + (*Iterator)->h; cout << "HOB go on mob 111" << endl;
							}
							if (Hobbit.dy > 0 && Hobbit.hitDamage == true)
							{
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 10;
								Hobbit.hitDamageGetOut4 = false; Hobbit.TimerDamageGetOut = 0;
								Hobbit.y = (*Iterator)->y - Hobbit.h; cout << "HOB go on mob 222" << endl;
							}


							/*if ((*Iterator)->dx < 0 && Hobbit.hitDamage == true) 
							{
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 10; cout << "11" << endl;
								Hobbit.hitDamageGetOut1 = false; Hobbit.TimerDamageGetOut = 0;
							}
							if ((*Iterator)->dx > 0 && Hobbit.hitDamage == true) 
							{
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 10; cout << "22" << endl;
								Hobbit.hitDamageGetOut1 = false; Hobbit.TimerDamageGetOut = 0;
							}*/
							if ((*Iterator)->dy < 0 && Hobbit.hitDamage == true) 
							{
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 10;
								Hobbit.hitDamageGetOut3 = false; Hobbit.TimerDamageGetOut = 0; cout << "DRAGON go on HOB 111" << endl;
							}
							if ((*Iterator)->dy > 0 && Hobbit.hitDamage == true) 
							{
								Hobbit.hitDamage = false; Hobbit.TimerDamage = 0; Hobbit.health -= 10;
								Hobbit.hitDamageGetOut4 = false; Hobbit.TimerDamageGetOut = 0; cout << "DRAGON go on HOB 222" << endl;
							}
						}
					}
				}
				for (Iterator3 = bigList.begin(); Iterator3 != bigList.end(); Iterator3++)
				{
					if ((*Iterator)->getRect() != (*Iterator3)->getRect())
					{
						if (((*Iterator)->getRect().intersects((*Iterator3)->getRect())) && ((*Iterator)->name == "easyEnemy") && ((*Iterator3)->name == "easyEnemy"))
						{
							(*Iterator)->dx *= -1;
							(*Iterator)->sprite.scale(-1, 1);
							(*Iterator3)->dx *= -1;
							(*Iterator3)->sprite.scale(-1, 1);
						}
					}
				}

				for (Iterator2 = bigList.begin(); Iterator2 != bigList.end(); Iterator2++)
				{
					if ((*Iterator)->getRect() != (*Iterator2)->getRect())
					{
						if ((*Iterator)->health > 0)
						{
							if (((*Iterator)->getRect().intersects((*Iterator2)->getRect())) && ((*Iterator)->name == "easyEnemy") && ((*Iterator2)->name == "ShootStone"))
							{
								(*Iterator)->health = 0;
								(*Iterator2)->life = false;
								Hobbit.enemyPoint += 1;
							}
							if (((*Iterator)->getRect().intersects((*Iterator2)->getRect())) && ((*Iterator)->name == "easyEnemy2") && ((*Iterator2)->name == "ShootStone"))
							{
								(*Iterator)->health = 0;
								(*Iterator2)->life = false;
								Hobbit.enemyPoint += 1;
							}
						}
					}
				}
			}
			//_______________Logic for Objects________________
			for (Iterator = bigList.begin(); Iterator != bigList.end(); Iterator++)
			{
				if ((*Iterator)->getRect().intersects(Hobbit.getRect()))
				{
					if ((*Iterator)->name == "simpleStone")
					{
						Hobbit.stonesPoint += 1;
						(*Iterator)->health = 0;
					}
					if ((*Iterator)->name == "elixirHP")
					{
						if (Hobbit.elixir < 5)
						{
							Hobbit.elixir += 1;
							(*Iterator)->health = 0;
						}
					}
				}
			}
		}
		else
		{
			text3.setString("You Died");
			text3.setPosition(viewCamera.getCenter().x - 200, viewCamera.getCenter().y - 90);
		}


		//===================================================MAIN DRAW==================================================
		window.setView(viewCamera);
		window.clear(Color(77, 83, 140));
		lvl.Draw(window);
		
		
		//__________Draw all sprites____________
		for (Iterator = bigList.begin(); Iterator != bigList.end(); Iterator++)
		{
			window.draw((*Iterator)->sprite);
		}
		window.draw(Hobbit.sprite);

		//===================================================INTERFACE==================================================
		ostringstream PlayerHealth, scoreString, Mission;
		PlayerHealth << Hobbit.health;
		scoreString << Hobbit.stonesPoint;
		Mission << GetMissionText(GetMission(Hobbit.x, Hobbit.stonesPoint, Hobbit.enemyPoint, Hobbit.health));
		text1.setString(" ''Name''" "\n" "\n"  "\n"  "Stones: " + scoreString.str() + "\n");
		text2.setString(Mission.str());

		sprite_interface.setPosition(viewCamera.getCenter().x - 320, viewCamera.getCenter().y - 240);
		window.draw(sprite_interface);


		HealthBarHobbit.update(Hobbit.health);
		text1.setPosition(viewCamera.getCenter().x + 180, viewCamera.getCenter().y - 180);
		text2.setPosition(viewCamera.getCenter().x + 138, viewCamera.getCenter().y - 70);
		HealthBarHobbit.draw(window);
		window.draw(text1);
		window.draw(text2);


		if (Hobbit.elixir == 0)
		{
			sprite_elixir0.setTextureRect(IntRect(0, 0, 83, 22));
			sprite_elixir0.setPosition(viewCamera.getCenter().x + 180, viewCamera.getCenter().y - 138);
			window.draw(sprite_elixir0);
		}
		if (Hobbit.elixir == 1)
		{
			sprite_elixir1.setTextureRect(IntRect(0, 0, 83, 22));
			sprite_elixir1.setPosition(viewCamera.getCenter().x + 180, viewCamera.getCenter().y - 138);
			window.draw(sprite_elixir1);
		}
		if (Hobbit.elixir == 2)
		{
			sprite_elixir2.setTextureRect(IntRect(0, 0, 83, 22));
			sprite_elixir2.setPosition(viewCamera.getCenter().x + 180, viewCamera.getCenter().y - 138);
			window.draw(sprite_elixir2);
		}
		if (Hobbit.elixir == 3)
		{
			sprite_elixir3.setTextureRect(IntRect(0, 0, 83, 22));
			sprite_elixir3.setPosition(viewCamera.getCenter().x + 180, viewCamera.getCenter().y - 138);
			window.draw(sprite_elixir3);
		}
		if (Hobbit.elixir == 4)
		{
			sprite_elixir4.setTextureRect(IntRect(0, 0, 83, 22));
			sprite_elixir4.setPosition(viewCamera.getCenter().x + 180, viewCamera.getCenter().y - 138);
			window.draw(sprite_elixir4);
		}
		if (Hobbit.elixir == 5)
		{
			sprite_elixir5.setTextureRect(IntRect(0, 0, 83, 22));
			sprite_elixir5.setPosition(viewCamera.getCenter().x + 180, viewCamera.getCenter().y - 138);
			window.draw(sprite_elixir5);
		}

		if (Hobbit.hitDamage == false)
		{
			auch_sprite.setPosition(Hobbit.x + 10, Hobbit.y - 10);
			window.draw(auch_sprite);
		}

		window.draw(text3);
		window.display();
	}
	return 0;
}
