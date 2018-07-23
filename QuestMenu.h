#include <string>

using namespace sf;
using namespace std;

int GetMission(int x, int stonesPoint, int enemyPoint, int health)
{
	int mission = 0;
	if ((x > 0) && (x < 200))
	{
		mission = 0;
	}
	if (stonesPoint >= 1)
	{
		mission = 1;
	}
	if (enemyPoint == 2)
	{
		mission = 2;
	}
	if (health < 90)
	{
		mission = 3;
	}

	return mission;
}


string GetMissionText (int ChangeMission)
{
	string MissionText = "";

	switch (ChangeMission)
	{
	case 0: MissionText = "              QUEST: \n        Hi, Hobbit! \n        Try to pick up \n        the elixir and \n        stones. You will  \n        need them for \n        survival!";
		break;
	case 1: MissionText = "        Good!  \n        So try to kill\n        a few monsters, but \n        be careful!";
		break;
	case 2: MissionText = "        Great work!!! \n        Now you can go\n        to the river. \n        It is very \n        dangerous place!  \n        Good luck, \n        Hobbit!";
		break;
	case 3: MissionText = "        Oh... \n        You look bad...\n        Try to drink an \n        Elixir! \n        Just press 'T'!";
		break;
	}
	return MissionText;
};