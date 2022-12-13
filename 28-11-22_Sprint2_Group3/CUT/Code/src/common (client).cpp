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

  cout << "\nThe map msgs is : \n";
  cout << '\t' << "KEY" <<  '\t' << "TYPE" << '\t' << "LENGTH" << '\t' << "VALUE" << endl; 
  for (itr = msg_map.begin(); itr != msg_map.end(); ++itr) 
  {
    cout << '\t' << itr->first << '\t' << itr->second.msg_type << '\t' << itr->second.msg_len << '\t' << itr->second.msg_val
      << '\n';
  }
  cout << endl;
}

Messaging mg;
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


 return sendBuffer;
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
int chatapp::auth_user(string uid,string pswd)
{
     if(username_map[uid]==pswd)
     {
        return 1;}
     else
     {
       return 0;}
       }
chatapp ca;
string  Messaging::client_process_message(string s )
{
  char type, len;
  string rcvString = "";
  int ret;

  cout << "Input String = " << s << endl;

  type = int (s[0]); // 1
  len  = int (s[1]); // 5

  int t = type; 
  rcvString = s.substr(2,len); // ABCDE 

  cout << type  << endl;
  cout << len   << endl;
  cout << rcvString << endl;
 string  temp;

  // processing 
  switch (t)
  {
    case 13:
    
      cout << "Please Enter the password " <<  endl;
      string pswd;
      cin >> pswd;
     
      
     temp =   mg.create_message(PASSWORD,pswd);
      

      break;
   /*case 2:
      cout << "authenticating user" << endl;
      ret = ca.auth_user(u1,rcvString);
      if(ret==1){
          msg.create_message(AUTH_SUCCESS);}
      else{
        msg.create_message(AUTH_FAIL);}
        break;
   case 3:
           
       msg.create_message(ACTIVE_USER_LIST,);
       break;
  case 4:
      msg.create_mesage(ACTIVE_GROUP_LIST);
      break;
 case 5:
      //selected user */
      
      
  }
 // snprintf(sendBuffer,sizeof(sendBuffer),"%s\r\n",temp);
  return temp;

}

