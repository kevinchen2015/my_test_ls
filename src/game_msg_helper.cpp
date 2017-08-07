#include "game_msg_helper.h"
#include <assert.h>
#include "game_cmd.h"
#include "frame_cmd.h"


void ParseMsg(char* buf, unsigned int size, void* user_data, FrameCmdQueue* queue)
{
	unsigned int used_size = 0;
	while (size - used_size > 0)
	{
		NetMsg* net_msg = (NetMsg*)(buf + used_size);
		if (net_msg->size > size - used_size)
		{
			break;
		}

		unsigned int used_cmd_size = 0;
		used_cmd_size += (sizeof(NetMsg));

		while (used_cmd_size < net_msg->size)
		{
			FrameCmd* new_cmd = new FrameCmd();
			new_cmd->frame_id = net_msg->frame_id;
			new_cmd->time = net_msg->time;
			new_cmd->user_data = user_data;
			Cmd* cmd = (Cmd*)(buf + used_size + used_cmd_size);
			if (cmd->id == FRAME_START_CMD
				|| cmd->id == FRAME_END_CMD)
			{
				new_cmd->cmd_len = sizeof(Cmd);
				Cmd* c = (Cmd*)malloc(new_cmd->cmd_len);
				*c = *cmd;
				used_cmd_size += sizeof(Cmd);
				new_cmd->cmd = c;
			}
			else if (cmd->id == GAME_MOVE_CMD)
			{
				new_cmd->cmd_len = sizeof(MoveCmd);
				MoveCmd* c = (MoveCmd*)malloc(new_cmd->cmd_len);
				memcpy(c, cmd, sizeof(MoveCmd));
				used_cmd_size += sizeof(MoveCmd);
				
				new_cmd->cmd = c;
			}
			else if (cmd->id == GAME_CREATE_ENTITY_CMD)
			{
				new_cmd->cmd_len = sizeof(CreateEntityCmd);
				CreateEntityCmd* c = (CreateEntityCmd*)malloc(new_cmd->cmd_len);
				memcpy(c, cmd, sizeof(CreateEntityCmd));
				used_cmd_size += sizeof(CreateEntityCmd);

				new_cmd->cmd = c;
			}
			else
			{
				delete new_cmd;
				assert(0);
				break;
			}
			queue->PushFrameCmd(new_cmd);
		}
		used_size += used_cmd_size;
	}
}

void PackMsg(FrameCmdQueue* queue, char*& buf, unsigned int& size)
{
	static char s_pack_buff[1024 * 6];
	buf = s_pack_buff;
	size = 0;
	if (queue->GetCmdList().size() == 0)
	{
		return;
	}

	NetMsg* net_msg = (NetMsg*)s_pack_buff;
	net_msg->frame_id = 0;
	size += sizeof(NetMsg);
	for (auto cmd : queue->GetCmdList())
	{
		if (net_msg->frame_id == 0)
		{
			net_msg->frame_id = cmd->frame_id;
			net_msg->time = cmd->time;
		}
		else
		{
			assert(net_msg->frame_id == cmd->frame_id);
		}
		memcpy(buf + size, cmd->cmd, cmd->cmd_len);
		size += cmd->cmd_len;
	}
	net_msg->size = size;
}