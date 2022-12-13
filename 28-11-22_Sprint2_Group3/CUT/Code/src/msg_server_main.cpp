#include <iostream>
#include "../include/msg_server.h"
#include "../include/common.h"

  int main()
{
  class TcpServer s1; 

  printf("%s():%d Init Server \n", __FUNCTION__, __LINE__);
  s1.init_server();
  /* Starting the server and waiting for MAX_CLIENTS 
   * to connect to server */
  printf("%s():%d Start Server \n", __FUNCTION__, __LINE__);
  s1.start_server();
  s1.wait_for_client_termination();
  
  //hotelusers.init("../../data/users.csv");
  
  return 0;
}
