#include <cstdint>
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
#include "../include/common.h"
#include <bits/stdc++.h>
#include "../include/msg_server.h"

using namespace std;

#include <sys/select.h>

#define SELECT_FAILED -1
#define SELECT_TIMEOUT 0

namespace fd_wait {
    /**
     * monitor file descriptor and wait for I/O operation
     */
    Result waitFor(int &fileDescriptor, uint32_t timeoutSeconds) {
        struct timeval tv;
        tv.tv_sec = timeoutSeconds;
        tv.tv_usec = 0;
        fd_set fds;

        FD_ZERO(&fds);
        FD_SET(fileDescriptor, &fds);
        const int selectRet = select(fileDescriptor + 1, &fds, nullptr, nullptr, &tv);

        if (selectRet == SELECT_FAILED) {
            return Result::FAILURE;
        } else if (selectRet == SELECT_TIMEOUT) {
            return Result::TIMEOUT;
        }
        return Result::SUCCESS;
    }
}

void Messaging::initialize_messages ()
{
  messages_t m;

  msg_map.insert(pair<msg_types_e, messages_t> (USER_ID, {USER_ID,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (PASSWORD,  {PASSWORD,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (PRIVATE_CHAT,    {PRIVATE_CHAT,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (GROUP_CHAT,    {GROUP_CHAT,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (SELECTED_USER,       {SELECTED_USER,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t>  (SELECTED_GROUP,{SELECTED_GROUP,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t>  (SEND_FILE,{SEND_FILE,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (ACTIVE_USER_LIST,{ACTIVE_USER_LIST,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (ACTIVE_GROUP_LIST,{ACTIVE_GROUP_LIST,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (AUTH_SUCCESS,{AUTH_SUCCESS,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (AUTH_FAIL,{AUTH_FAIL,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (REQ_PASSWD,{REQ_PASSWD,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (USER_NOT_FOUND,{USER_NOT_FOUND,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (EXIT_CHAT,{EXIT_CHAT,0,""}));
  

  map<msg_types_e, messages_t>::iterator itr;

 /* cout << "\nThe map msgs is : \n";
  cout << '\t' << "KEY" <<  '\t' << "TYPE" << '\t' << "LENGTH" << '\t' << "VALUE" << endl; 
  for (itr = msg_map.begin(); itr != msg_map.end(); ++itr) 
  {
    cout << '\t' << itr->first << '\t' << itr->second.msg_type << '\t' << itr->second.msg_len << '\t' << itr->second.msg_val
      << '\n';
  }
  cout << endl;*/
}

//Messaging mg;
string  Messaging::create_message (msg_types_e option,string u1)
{
  char sendBuffer[64];

  memset(sendBuffer, '0', sizeof(sendBuffer));

  if(msg_map.find(option)!= msg_map.end())
  {
   
      
      msg_map[option].msg_val = u1;
      msg_map[option].msg_len = u1.length();

      snprintf(sendBuffer, sizeof(sendBuffer), "%d%d%s\r\n", msg_map[option].msg_type, 
        msg_map[option].msg_len, msg_map[option].msg_val.c_str());
  }

  cout << "message created is " << sendBuffer << endl;
      return sendBuffer;

}

string Messaging::create_message(msg_types_e option)
{
  char sendBuffer[64];
  //string s ="";

  memset(sendBuffer, '0', sizeof(sendBuffer));

  if(msg_map.find(option)!= msg_map.end())
  {
    snprintf(sendBuffer, sizeof(sendBuffer), "%d\r\n", msg_map[option].msg_type); 
  }

  cout << "message created is " << sendBuffer << endl;
  
  //s = sendBuffer;

  return sendBuffer;
}




int IpcMq::createIpcMq(string name, int flags, int maxMsgs, int maxMsgSize)
{
 // int ret = -1;

  _mqName  = name;
  _mqFlags = flags;

  _mqAttr.mq_maxmsg   = maxMsgs;  // max msgs in queue is 1
  _mqAttr.mq_msgsize  = maxMsgSize;  // max size of msg in queue is 4


  _mqFd = mq_open(name.c_str(), _mqFlags, 0777, &_mqAttr);

  cout << "error = " << errno << endl;

  cout << "mq_open return value: " << _mqFd << endl;



  return 0;
}
void IpcMq::closeIpc()
{
  mq_close(_mqFd);
}

void IpcMq::deleteIpc()
{
  mq_unlink(_mqName.c_str());
}

int IpcMq::sendIpcMessage(string msgToSend, int msgPrio)
{
  int ret = -1;

  cout << "sendIPC = " << msgToSend << endl;
  cout << "sendIPC.c_str() = " << msgToSend.c_str() << endl;
  ret = mq_send(_mqFd,
               (const char *) msgToSend.c_str(), strlen(msgToSend.c_str()), msgPrio);

  cout << "MQ Sent " << ret << endl;
  return ret;
}
int IpcMq::recvIpcMessage(string *msgToRcv, unsigned int *msgPrio)
{
  int ret = -1;
  char recvBuff[128];

  memset(recvBuff, 0, sizeof(recvBuff));

  ret = mq_receive(_mqFd, recvBuff, sizeof(recvBuff), msgPrio);

  cout << "MQ RCV " << ret << endl;

  cout << "recvIPC = " << recvBuff << endl;

  *msgToRcv = recvBuff;
  cout << "msgToRcv = " << *msgToRcv << endl;

  return ret;
}
int chatapp::search_user(string uid)
{
  if(username_map.find(uid)!=username_map.end())
  {
    cout << "User Present" << endl;
    return 1;
  }
  else
    return 0;

}
int chatapp::auth_user(string pswd)
{    
    map<string ,string>::iterator it;
 for  ( it=username_map.begin();it!=username_map.end();++it)
    {
         int a;
     if(it->second==pswd)
     {
        a=1;}
     else
     {
        a=0;
       }
     return a;
}}
chatapp ca;
TcpServer Ts;
string  Messaging::server_process_message(string s)
{
  char type, len;
  string rcvString = "";
  int ret;
  char sendBuff[64];
  memset(sendBuff,'0',sizeof(sendBuff));

  cout << "Input String = " << s << endl;

  type = int (s[0]); // 1
  len  = int (s[1]); // 5

  int t = type; 
  rcvString = s.substr(2,len); // ABCDE 

  cout << type  << endl;
  cout << len   << endl;
  cout << rcvString << endl;
 string  tmp ;
 string tmps;
 string a = create_message(REQ_PASSWD);
 cout << a << endl;
  // processing 
  switch (t)
  {
    case 0:
    
      cout << "Checking for user name " << rcvString << endl;
      
      ret =ca.search_user(rcvString);
      if (ret == 1)
      {
        // server should create 
      tmp =  create_message(REQ_PASSWD);
      }
      else{
    tmp =   create_message(USER_NOT_FOUND);}
      break;
  case 2:
      cout << "authenticating user" << endl;
      ret = ca.auth_user(rcvString);
      if(ret==1){
       tmp =  create_message(AUTH_SUCCESS);}
      else{
      tmp =  create_message(AUTH_FAIL);}
        break;
  /* case 3:
           
       msg.create_message(ACTIVE_USER_LIST,);
       break;
  case 4:
      msg.create_mesage(ACTIVE_GROUP_LIST);
      break;
 case 5:
      //selected user */
      
      
  }
  cout << tmp << endl;
//  snprintf(sendBuff,sizeof(sendBuff),"%s\r\n",tmp.c_str());
 // tmp=sendBuff;
//  cout <<"processed message"<< s<< endl;
 // cout << "message"<<tmp.c_str << endl;
  
  return tmp ;
}

