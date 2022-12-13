#include <iostream>
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
#include "../include/msg_client.h"
#include "../include/msg_client.h"

TcpClient client;
Messaging msg;


#define LOGIN 1

void printMenu() {
    std::cout << "Select one of the following options: \n" <<
                 "1. Login: \n" << 
                 "2. Private chat  \n"  <<
                 "3. Group chat   \n"  <<
                 "4. Select user for chat  \n"  <<
                 "5. Select Group for chat \n"  <<
                 "6. Select File to share \n" ;
}

int getMenuSelection() {
    int selection = 0;
    std::cin >> selection;
    if (!std::cin) {
        throw std::runtime_error("invalid menu input. expected a number, but got something else");
    }
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    return selection;
 

}

bool handleMenuSelection(int selection) {
    string tmpStr = "";
    static const int minSelection = USER_ID;
    static const int maxSelection = EXIT_CHAT+1;
    if (selection < minSelection || selection > maxSelection) {
        std::cout << "hehe invalid selection: " << selection <<
                     ". selection must be b/w " << minSelection << " and " << maxSelection << "\n";
        return false;
    }

    switch (selection) {
        case LOGIN: {
            string u1;              
            cout << "Enter User id for login\n";
            cin  >> u1; 
           tmpStr =  msg.create_message(USER_ID, u1);
            /* send this user id to ser2Dver via pthread 
	     * and wait for response */
	   /*  cout << "enter the password"<< endl;
	    
	    
	    

	    string passwd;
	    cin >> passwd;
	    
	    int n = passwd.length();
	    if(n<2 ){
		    cout <<" enter valid password"<< endl;
		    cin >> passwd;
	    }
	   
	  tmpStr =   msg.create_message(PASSWORD, passwd);*/
	   
	    break;
        }
     case  PRIVATE_CHAT: {
	   
	tmpStr =   msg.create_message(PRIVATE_CHAT);
	   break;}
     case GROUP_CHAT: {
        
     tmpStr = 	msg.create_message(GROUP_CHAT);
	break;}
    case SELECTED_USER: {
       string u1;
       cout << " enter the user" << endl;
       cin >> u1;
     tmpStr =  msg.create_message(SELECTED_USER,u1);
       break;}
     case SELECTED_GROUP: {
	string u1;
	cout << " enter the group" << endl;
	cin >> u1;
      tmpStr = 	msg.create_message(SELECTED_GROUP,u1);
	break;}
     case SEND_FILE: {
	string u1;
	cout << "enter the file name " << endl;
	cin >> u1;
     tmpStr =	msg.create_message(SEND_FILE,u1);
	break;}
    case EXIT_CHAT: {
     tmpStr =	msg.create_message(EXIT_CHAT);
	break;}
        default: {
          cout<< "tmpstr = " << tmpStr << endl;

            std::cout << " hhh invalid selection: " << selection <<
                      ". selection must be b/w " << minSelection << " and " << maxSelection << "\n";
	
        }
    }
    //sharing message to server
    client.sendMsg(tmpStr.c_str(),sizeof(tmpStr.c_str()));
   // int ret = mainMq.sendIpcMessage(tmpStr, 1);

 //   cout<<"return from IPC " << ret << endl;
    return false;
}

#define SCHEDULER_IP    "10.128.0.4"
#define SCHEDULER_PORT  "5000"
#define MQ_FLAGS O_RDWR | O_CREAT
#define MQ_MAXSIZE 1
#define MQ_MAXMSGSIZE 128

int main() 
{
 // mainMq.createIpcMq("/mq1", MQ_FLAGS, MQ_MAXSIZE, MQ_MAXMSGSIZE);
  string ip = "10.128.0.4";
  int port = 5000;

  bool connected = false;
  int x = 0;

  while (!connected) 
  {
    connected = client.connectTo(ip, port);

    if (connected) 
    {
      std::cout << "Client connected successfully\n";
    } else 
    {
      std::cout << "Client failed to connect: " << "\n"
        << "Make sure the server is open and listening\n\n";
      sleep(2);
      std::cout << "Retrying to connect...\n";
      x++;
      if (x == 2)
      {
        std::cout << "Exceeded max retries\n" << endl;
        exit(0);
      }
    }
  };

  bool shouldTerminate = false;
  while(!shouldTerminate)
  {
    printMenu();
    int selection = getMenuSelection();
    
    shouldTerminate = handleMenuSelection(selection);
   
  }

  return 0;
}
