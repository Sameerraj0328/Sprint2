
#include "../include/msg_client.h"
#include "../include/common.h"

using namespace std;

TcpClient::TcpClient() {
    _isConnected = false;
    _isClosed = true;
}

TcpClient::~TcpClient() {
    close();
}

/*
 * Receive server packets, and notify user
 */
//extern class IpcMq mainMq;
 Messaging mes;
void* receiveTask(void *arg) 
{
    class TcpClient *cl = (class TcpClient *) arg;
    int ret = -1;
    
    cout<<"test printf in thread" << endl;
    //cl -> setIsConnected(true);
     while(cl->isConnected()) 
    {
      /* string rxMsgFromMain;
       unsigned int prio;

        ret = mainMq.recvIpcMessage(&rxMsgFromMain, &prio);  
	cout<< "Recived: " << ret << endl;
	cout << "MSG: " << rxMsgFromMain << endl;
	cout<< "Prio: " << prio << endl;*/

        char msg[1024];

        const size_t numOfBytesReceived = recv(cl->getSockFd(), msg, MAX_PACKET_SIZE, 0);

        if(numOfBytesReceived < 1) 
        {
            std::string errorMsg;
            if (numOfBytesReceived == 0) 
            { //server closed connection
                errorMsg = "Server closed connection";
            } else {
                errorMsg = strerror(errno);
            }
            cl->setIsConnected(false);
            return NULL;
        } else {
            cout << "recevied message:" << msg << endl;
	    string out;
	    char sendB[199];
	    memset(sendB,'0',sizeof(sendB));
	    //char  sendMsg[100];
	 out  =  mes.client_process_message(msg);
	 snprintf(sendB,sizeof(sendB),out.c_str());
	send(cl->getSockFd(),sendB,sizeof(sendB),0);
	
        }

    }
    return NULL;
}

bool TcpClient::connectTo(const std::string & address, int port) {
    try {
        initializeSocket();
        setAddress(address, port);
    } catch (const std::runtime_error& error) {
        return -1;
    }

    const int connectResult = connect(_sockfd, (struct sockaddr *)&_server , sizeof(_server));
    const bool connectionFailed = (connectResult == -1);
    if (connectionFailed) {
        return -1;
    }
   

    pthread_create(&_receiveThread, NULL, receiveTask, this); 
    _isConnected = true;
    _isClosed = false;

    return true;
}

void TcpClient::initializeSocket() {

    _sockfd = socket(AF_INET , SOCK_STREAM , 0);
    
    if (_sockfd == -1) 
    {
        throw std::runtime_error(strerror(errno));
    }
}

void TcpClient::setAddress(const std::string& address, int port) {
    const int inetSuccess = inet_aton(address.c_str(), &_server.sin_addr);

    if(!inetSuccess) 
    { // inet_addr failed to parse address
        // if hostname is not in IP strings and dots format, try resolve it
        struct hostent *host;
        struct in_addr **addrList;
        if ( (host = gethostbyname( address.c_str() ) ) == nullptr){
            throw std::runtime_error("Failed to resolve hostname");
        }
        addrList = (struct in_addr **) host->h_addr_list;
        _server.sin_addr = *addrList[0];
    }
    _server.sin_family = AF_INET;
    _server.sin_port = htons(port);
}


int TcpClient::sendMsg(const char * msg, size_t size) 
{
    const size_t numBytesSent = send(_sockfd, msg, size, 0);

    if (numBytesSent < 0 ) 
    { // send failed
        return -1;
    }
    
    if (numBytesSent < size) 
    { // not all bytes were sent
        return -1;
    }
    return 0;
}



void TcpClient::waitForReceiveThread() 
{
  pthread_join(_receiveThread, NULL);
}

int TcpClient::close(){
    if (_isClosed) {
        return -1;
    }

    _isClosed = true;
    return 0;
}


