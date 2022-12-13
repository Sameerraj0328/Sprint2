#pragma once

#include <cstdio>
#include <string>
#include <bits/stdc++.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <array>
#include <map>
#include <utility>
using namespace std;

#define MAX_PACKET_SIZE 4096

namespace fd_wait {
    enum Result {
        FAILURE,
        TIMEOUT,
        SUCCESS
    };

    Result waitFor(int &fileDescriptor, uint32_t timeoutSeconds = 1);
};

typedef enum 
{
  USER_ID ,
  PASSWORD ,
  PRIVATE_CHAT ,
  GROUP_CHAT,
  SELECTED_USER ,
  SELECTED_GROUP ,
  SEND_FILE ,
  EXIT_CHAT,
  ACTIVE_USER_LIST,
  ACTIVE_GROUP_LIST ,
  AUTH_SUCCESS ,
  AUTH_FAIL ,
  REQ_PASSWD , 
  USER_NOT_FOUND,

} msg_types_e;

typedef struct messages
{
  msg_types_e msg_type;
  int         msg_len;
  string      msg_val;

}messages_t;

class Messaging
{
  map<msg_types_e, messages_t>  msg_map;

  public:
    Messaging()
    {
      initialize_messages();
    }
    string create_message(msg_types_e option, string u1);
    string create_message(msg_types_e option);
    void initialize_messages();
    string server_process_message(string );

};
class IpcMq
{
  private:
    string _mqName;
    mqd_t  _mqFd;
    int    _mqFlags;
    struct mq_attr _mqAttr;

  public:
    int createIpcMq(string name, int flags, int maxMsgs, int maxMsgSize);
    int sendIpcMessage(string msgToSend, int msgPrio);
    int recvIpcMessage(string *msgToRcv, unsigned int *msgPrio);
    void closeIpc();
    void deleteIpc();
};
class chatapp
{
  string user1,u1,user2, u2, user3,u3;
  map<string, string>  username_map = {{user1,u1},{user2,u2},{user3,u3}};

  public:
    int search_user(string);
    int auth_user(string);
    
};


