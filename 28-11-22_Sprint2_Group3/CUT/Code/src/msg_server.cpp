#include <msg_server.h>
#include <common.h>
#include <map>


using namespace std;



map<int,Messaging> TcpServer::clientMessageMap;
TcpServer::TcpServer() {
    init_server();
    start_server();
}

TcpServer::~TcpServer() {

}
Messaging ag;

void TcpServer::init_server()
{
   server_fd = socket(AF_INET, SOCK_STREAM, 0);
  printf("%s():%d Server Socket FD = %d \n", __FUNCTION__, __LINE__, server_fd);
  memset(&server_addr, '0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(5000);
  bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  listen(server_fd, 10);
  printf("%s():%d Listening on Server Socket FD = %d \n", __FUNCTION__, __LINE__, server_fd);
}

void TcpServer::start_server()
{
  int i = 0;
  while(1)
  {
    printf("%s():%d Waiting on accept \n", __FUNCTION__, __LINE__);
    client_fd[i] = accept(server_fd, (struct sockaddr*)NULL, NULL);
    printf("%s():%d New Client %d Accepted: Client FD = %d\n", __FUNCTION__, __LINE__, i, client_fd[i]);
    pthread_create(&thread[i], NULL, client_handler, &client_fd[i]);
    i++;
    if (i == MAX_CLIENTS)
    {
      printf("%s():%d Max Clients %d Reached \n", __FUNCTION__, __LINE__, i);
      break;
    }
  }
}

void TcpServer::wait_for_client_termination()
{
  for(int i = 0; i < MAX_CLIENTS; i++)
  {
    pthread_join(thread[i], NULL);
  }
}

 void* TcpServer::client_handler (void *client_fd)
{
  int *c_fd = (int *) client_fd;
  char recvBuff[64];
  int recv_bytes; 
 // time_t ticks;
  int cid=*c_fd;
  cout<< "cid: "<< cid<< endl;
 /* if (TcpServer::clientMessageMap.find(cid) == TcpServer::clientMessageMap.end()){
    cout<< "create message obj for: "<< cid<< endl;
    Messaging m;
    TcpServer::clientMessageMap[cid] = m; 
  }*/
    
  memset(recvBuff, '0', sizeof(recvBuff));

  int x = 0;

  while(1)
  {
    recv_bytes = recv(cid, recvBuff, sizeof(recvBuff), 0);  
    printf("\n client_handler: %d \n",x);
    if(recv_bytes < 0)
    {
      printf("\n Read error \n");
      perror("Error: ");
      continue;
    }
    
    if(recv_bytes == 0)
    {
      printf("\n Client fd %d is closed \n", *c_fd);
      pthread_exit(NULL);
    }

   

    printf("%s():%d Client %d Msg = %s \n", __FUNCTION__, __LINE__, *c_fd, recvBuff);
   // string rB;
   // rB =recvBuff.c_str();
   // return rB;
    

   // string sendMsg = TcpServer::clientMessageMap[cid].server_process_message(rB,tmp);
  
   string caps;
  
  caps= ag.server_process_message(recvBuff);
  char sendBuffer[199];
  memset(sendBuffer,'0',sizeof(sendBuffer));
  snprintf(sendBuffer,sizeof(sendBuffer),caps.c_str());
    send(*c_fd,sendBuffer,sizeof(caps), 0);
    sleep(2);
    x++;
  }

  printf("%s():%d Exiting \n", __FUNCTION__, __LINE__);
  exit(0);
}

