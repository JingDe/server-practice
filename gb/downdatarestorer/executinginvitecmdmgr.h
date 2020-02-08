#ifndef DOWN_DATA_RESTORER_EXECUTING_CMD_H
#define DOWN_DATA_RESTORER_EXECUTING_CMD_H

#include"downdatarestorerdef.h"
#include"executinginvitecmd.h"
#include"rwmutex.h"

class RedisClient;
class MutexLock;

class ExecutingInviteCmdMgr{
public:
	ExecutingInviteCmdMgr(RedisClient*, int worker_thread_num_);
	~ExecutingInviteCmdMgr();
	
	int Load(vector<ExecutingInviteCmdList>& cmd_lists);
	int Load(int worker_thread_num, ExecutingInviteCmdList& cmd_list);
//	int Search(const string& executing_invite_cmd_id, ExecutingInviteCmd& cmd);
	int Search(const string& executing_invite_cmd_id, ExecutingInviteCmd& cmd, int worker_thread_no);
	int Insert(const ExecutingInviteCmd& cmd, int worker_thread_no);
	int Delete(const ExecutingInviteCmd& cmd, int worker_thread_no);
	int Clear();
	int Clear(int worker_thread_no);
	int ClearWithLockHeld(int worker_thread_no);
	int Update(const vector<ExecutingInviteCmdList>& cmd_lists);
	int Update(const ExecutingInviteCmdList& cmd_list, int worker_thread_no);
	int Update(const list<void*>& cmd_list, int worker_thread_no);

	int GetExecutingInviteCmdListSize(int worker_thread_no);
	
private:
	RedisClient* redis_client_;
	int worker_thread_num_;
//	MutexLock* modify_mutex_list_;
	RWMutex* rwmutexes_;
	
	const static string s_set_key_prefix;
	const static string s_key_prefix;
};
#endif
