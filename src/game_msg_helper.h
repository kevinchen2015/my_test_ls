#ifndef _GAME_MSG_HELPER_H_
#define _GAME_MSG_HELPER_H_


class FrameCmdQueue;

void ParseMsg(char* buf, unsigned int size, void* user_data, FrameCmdQueue* queue);
void PackMsg(FrameCmdQueue* queue, char*& buf, unsigned int& size);


#endif