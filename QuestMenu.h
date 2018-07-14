#include <string>

using namespace sf;
using namespace std;

int GetMission(int x)
{
	int mission = 0;
	if ((x > 0) && (x < 400))
	{
		mission = 0;
	}
	if (x > 400)
	{
		mission = 1;
	}
	if (x > 700)
	{
		mission = 2;
	}
	if (x > 900)
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
	case 0: MissionText = "              QUEST: \n        Hi, Hobbit! \n        Your firts quest \n        is to learn how \n        to collect and  \n        use the elixir, so \n        good luck!";
		break;
	case 1: MissionText = "\n     Mission check  \n       number #2";
		break;
	case 2: MissionText = "\n     Mission check  \n       number #3";
		break;
	case 3: MissionText = "\n     Mission check  \n       number #4";
		break;
	}
	return MissionText;
};