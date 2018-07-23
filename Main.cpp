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
	float x, y, dx, dy, speed, TimerEnemy, TimerHobbit, TimerShoot, TMPx;
	int w, h, health;
	bool life;
	Texture texture;
	Sprite sprite;
	String name;

	MainClass(Image &image, float X, float Y, int W, int H, String Name)
	{
		x = X, y = Y; w = W; h = H; name = Name;
		TimerEnemy = 0; TimerHobbit = 0; TimerShoot = 0; speed = 0; dx = 0; dy = 0, TMPx = 0;
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
	int stonesPoint, elixir, enemyPoint;
	bool  cooldown = false;

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
			if (Keyboard::isKeyPressed)
			{
				if (Keyboard::isKeyPressed(Keyboard::A))
				{
					state = left; speed = 0.07;
				}
				if (Keyboard::isKeyPressed(Keyboard::D))
				{
					state = right; speed = 0.07;
				}
				if (Keyboard::isKeyPressed(Keyboard::S))
				{
					state = up; speed = 0.07;
				}
				if (Keyboard::isKeyPressed(Keyboard::W))
				{
					state = down; speed = 0.07;
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

	void update(float mainTime)
	{
		move_hero();
		switch (state)
		{
		case right:dx = speed; dy = 0; break;
		case left:dx = -speed; dy = 0; break;
		case up: dx = 0; dy = speed; break;
		case down: dx = 0; dy = -speed; break;
		case stay: break;
		}
		x += dx*mainTime;
		LogicMap(dx, 0);

		y += dy*mainTime;
		LogicMap(0, dy);

		speed = 0;

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
				//if (obj[i].name == "Solid"){//если встретили препятствие (объект с именем solid)
				{
					if (Dy > 0) { y = obj[i].rect.top - h; dy = -0.05; sprite.scale(-1, 1); }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height; dy = 0.05; sprite.scale(-1, 1); }
					if (Dx > 0) { x = obj[i].rect.left - w; dx = -0.05; sprite.scale(-1, 1); }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.05; sprite.scale(-1, 1); }
				}
			}
		}
	}

	void update(float mainTime)
	{
		if (name == "easyEnemy")
		{
			sprite.setTextureRect(IntRect(800, 96, w, h));

			TimerEnemy += mainTime;
			if (TimerEnemy > 3000)
			{
				dx *= -1; TimerEnemy = 0;
				sprite.scale(-1, 1);
			}

			LogicMapEnemy(dx, 0);
			x += dx*mainTime;

			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
		if (name == "easyEnemy2")
		{
			sprite.setTextureRect(IntRect(768, 96, w, h));

			TimerEnemy += mainTime;
			if (TimerEnemy > 4000)
			{
				dy *= -1; TimerEnemy = 0;
				sprite.scale(-1, 1);
			}

			LogicMapEnemy(0, dy);
			y += dy*mainTime;

			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
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
					TMPx = x;
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
		sprite_bar.setPosition(viewCamera.getCenter().x + 245, viewCamera.getCenter().y - 160);
		bar.setPosition(viewCamera.getCenter().x + 248, viewCamera.getCenter().y - 157);

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
	Text text1("", font1, 12);
	//text.setStyle(Text::Bold);
	text1.setColor(Color::Black);

	//________text for 'Quest'__________
	Font font2;
	font2.loadFromFile("HobbitFont3.ttf");
	Text text2("", font2, 16);
	text2.setStyle(Text::Bold);
	text2.setColor(Color::Black);

	//________text for 'Dead'________
	Font font3;
	font3.loadFromFile("HobbitFont2.ttf");
	Text text3("", font3, 60);
	//text3.setStyle(Text::Bold);
	text3.setColor(Color::Red);

	//_________text for 'Interface'__________
	Image image_speak;
	image_speak.loadFromFile("images/ProtoTwoShort.png");
	Texture texture_speak;
	texture_speak.loadFromImage(image_speak);
	Sprite sprite_speak;
	sprite_speak.setTexture(texture_speak);
	sprite_speak.setTextureRect(IntRect(0, 0, 640, 64));

	//_________text for 'Interface'__________
	Image image_blok;
	image_blok.loadFromFile("images/ProtoBlok.png");
	Texture texture_blok;
	texture_blok.loadFromImage(image_blok);
	Sprite sprite_blok;
	sprite_blok.setTexture(texture_blok);
	sprite_blok.setTextureRect(IntRect(0, 0, 225, 480));

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

	Image heroImage, shooting, objects;
	heroImage.loadFromFile("images/CallMHero3.png");
	objects.loadFromFile("images/object.png");
	shooting.loadFromFile("images/shoot.png");

	Texture shooting_texture;
	shooting_texture.loadFromImage(shooting);
	Sprite shooting_sprite;
	shooting_sprite.setTexture(shooting_texture);


	//=================================================CLASS DECLARATION================================================
	list<MainClass*> bigList;
	list<MainClass*>::iterator Iterator;
	list<MainClass*>::iterator Iterator2;

	vector<Object> enemies, enemies2, stones, elixirHP;

	enemies = lvl.GetObjects("EasyEnemy");
	for (int i = 0; i < enemies.size(); i++)
	{
		bigList.push_back(new Class_Enemy(objects, lvl, enemies[i].rect.left, enemies[i].rect.top, 32, 32, "easyEnemy"));
	}
	enemies2 = lvl.GetObjects("EasyEnemy2");
	for (int i = 0; i < enemies2.size(); i++)
	{
		bigList.push_back(new Class_Enemy(objects, lvl, enemies2[i].rect.left, enemies2[i].rect.top, 32, 32, "easyEnemy2"));
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

		//===================================================FRAME SPRITE==================================================

		if (Hobbit.life == true)
		{
			if (Hobbit.state == Hobbit.left) { Hobbit.sprite.setTextureRect(IntRect(0, 33, 32, 32)); }
			if (Hobbit.state == Hobbit.right) { Hobbit.sprite.setTextureRect(IntRect(0, 0, 32, 32)); }
			if (Hobbit.state == Hobbit.up) { Hobbit.sprite.setTextureRect(IntRect(0, 99, 32, 32)); }
			if (Hobbit.state == Hobbit.down) { Hobbit.sprite.setTextureRect(IntRect(0, 66, 32, 32)); }

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				frame += 0.005 * mainTime;
				if (frame > 5) frame -= 5;
				Hobbit.sprite.setTextureRect(IntRect(32 * int(frame), 33, 32, 32));
			}
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				frame += 0.005 * mainTime;
				if (frame > 5) frame -= 5;
				Hobbit.sprite.setTextureRect(IntRect(32 * int(frame), 0, 32, 32));
			}
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				frame += 0.005 * mainTime;
				if (frame > 5) frame -= 5;
				Hobbit.sprite.setTextureRect(IntRect(32 * int(frame), 66, 32, 32));
			}
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				frame += 0.005 * mainTime;
				if (frame > 5) frame -= 5;
				Hobbit.sprite.setTextureRect(IntRect(32 * int(frame), 99, 32, 32));
			}
		}
		else
		{
			text3.setString("You Died");
			text3.setPosition(viewCamera.getCenter().x - 200, viewCamera.getCenter().y - 90);
			Hobbit.sprite.setTextureRect(IntRect(0, 98, 32, 32));
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
						(*Iterator)->dx = 0;
						Hobbit.health -= 35;
						(*Iterator)->health = 0;
					}
					if ((*Iterator)->name == "easyEnemy2")
					{
						(*Iterator)->dx = 0;
						Hobbit.health -= 50;
						(*Iterator)->health = 0;
					}
				}
				for (Iterator2 = bigList.begin(); Iterator2 != bigList.end(); Iterator2++)
				{
					if ((*Iterator)->getRect() != (*Iterator2)->getRect())
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

		ostringstream PlayerHealth, scoreString, Mission;
		PlayerHealth << Hobbit.health;
		scoreString << Hobbit.stonesPoint;
		Mission << GetMissionText(GetMission(Hobbit.x, Hobbit.stonesPoint, Hobbit.enemyPoint, Hobbit.health));
		text1.setString("      ''Name''" "\n\n" "Health: " "\n"
			"Elixir: "  "\n"  "Stones: " + scoreString.str() + "\n");
		text2.setString(Mission.str());

		sprite_speak.setPosition(viewCamera.getCenter().x - 320, viewCamera.getCenter().y + 176);
		window.draw(sprite_speak);
		sprite_blok.setPosition(viewCamera.getCenter().x + 95, viewCamera.getCenter().y - 240);
		window.draw(sprite_blok);

		HealthBarHobbit.update(Hobbit.health);
		text1.setPosition(viewCamera.getCenter().x + 178, viewCamera.getCenter().y - 210);
		text2.setPosition(viewCamera.getCenter().x + 138, viewCamera.getCenter().y - 85);
		HealthBarHobbit.draw(window);
		window.draw(text1);
		window.draw(text2);


		if (Hobbit.elixir == 0)
		{
			sprite_elixir0.setTextureRect(IntRect(0, 0, 88, 22));
			sprite_elixir0.setPosition(viewCamera.getCenter().x + 230, viewCamera.getCenter().y - 145);
			window.draw(sprite_elixir0);
		}
		if (Hobbit.elixir == 1)
		{
			sprite_elixir1.setTextureRect(IntRect(0, 0, 88, 22));
			sprite_elixir1.setPosition(viewCamera.getCenter().x + 230, viewCamera.getCenter().y - 145);
			window.draw(sprite_elixir1);
		}
		if (Hobbit.elixir == 2)
		{
			sprite_elixir2.setTextureRect(IntRect(0, 0, 88, 22));
			sprite_elixir2.setPosition(viewCamera.getCenter().x + 230, viewCamera.getCenter().y - 145);
			window.draw(sprite_elixir2);
		}
		if (Hobbit.elixir == 3)
		{
			sprite_elixir3.setTextureRect(IntRect(0, 0, 88, 22));
			sprite_elixir3.setPosition(viewCamera.getCenter().x + 230, viewCamera.getCenter().y - 145);
			window.draw(sprite_elixir3);
		}
		if (Hobbit.elixir == 4)
		{
			sprite_elixir4.setTextureRect(IntRect(0, 0, 88, 22));
			sprite_elixir4.setPosition(viewCamera.getCenter().x + 230, viewCamera.getCenter().y - 145);
			window.draw(sprite_elixir4);
		}
		if (Hobbit.elixir == 5)
		{
			sprite_elixir5.setTextureRect(IntRect(0, 0, 88, 22));
			sprite_elixir5.setPosition(viewCamera.getCenter().x + 230, viewCamera.getCenter().y - 145);
			window.draw(sprite_elixir5);
		}

		window.draw(text3);
		window.display();
	}
	return 0;
}