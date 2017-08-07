// lockstep.cpp : 定义控制台应用程序的入口点。
//


#include<Windows.h>
#include<iostream>
#include <conio.h>

#include "../src/game_client.h"
#include "../src/game_server.h"
#include "../src/frame_cmd.h"

//for input test
#include "../src/game_world_client.h"
#include "../src/game_predict.h"
#include "../src/game_input.h"

using namespace std;

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"kcp_udp.lib")

GameClient client;
GameServer server;

bool stoped = true;
void on_input_char(char ch)
{
	GameInput* input = client.game_world_->game_predict_->GetPredictInput();
	GameInput::InputCmd cmd = input->GetInputCmd(GAME_MOVE_CMD);
	if (cmd.IsWritten()) return;

	MoveCmd* move_cmd = (MoveCmd*)cmd.cmd;
	switch (ch)
	{
		case 'w':
		{
			move_cmd->id = GAME_MOVE_CMD;
			move_cmd->move_type = MOVE_FOWARD;
			stoped = false;
		}
		break;

		case 's':
		{
			move_cmd->id = GAME_MOVE_CMD;
			move_cmd->move_type = MOVE_BACK;
			stoped = false;
		}
		break;

		case 'a':
		{
			move_cmd->id = GAME_MOVE_CMD;
			move_cmd->move_type = MOVE_LEFT;
			stoped = false;
		}
		break;

		case 'd':
		{
			move_cmd->id = GAME_MOVE_CMD;
			move_cmd->move_type = MOVE_RIGHT;
			stoped = false;
		}
		break;
		
	}
}


void no_input()
{
	if (stoped) return;

	GameInput* input = client.game_world_->game_predict_->GetPredictInput();
	GameInput::InputCmd cmd = input->GetInputCmd(GAME_MOVE_CMD);
	if (cmd.IsWritten()) return;

	stoped = true;

	MoveCmd* move_cmd = (MoveCmd*)cmd.cmd;
	move_cmd->id = GAME_MOVE_CMD;
	move_cmd->move_type = MOVE_STOP;
}


int main()
{

	unsigned int lastTime = timeGetTime();
	unsigned int stat_time = lastTime;

	client.Start();

	while (1)
	{
		unsigned int timeNow = timeGetTime();
		int dt = timeNow - lastTime;

		server.OnFixedUpdate(timeNow, dt);

		client.OnFixedUpdate(timeNow, dt);
		client.OnUpdate(timeNow, dt);

		lastTime = timeNow;
		
		int sleep_ms = 33 - dt;
		if (sleep_ms <= 0)
			sleep_ms = 1;

		if (kbhit())
		{
			on_input_char(getch());
		}
		else
		{
			no_input();
		}
		Sleep(sleep_ms);
	}

	return 0;
}
