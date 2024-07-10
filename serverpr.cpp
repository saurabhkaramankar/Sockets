#include <iostream>

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#endif
#include <thread>
#include <vector>

using namespace std;

#pragma comment(lib,"ws2_32.lib")
/**
initialize winsocket library
create the socket
get ip and the port
bind the ip/port to the socket
listen on the socket
accept
recv and send
close the socket
clean the socket
  **/

bool Initialize(){
  WSADATA data;
  return WSAStartuo(MAKEWORD(2,2), &data) == 0;
}

void InteractWithClient(SOCKET clientSocket ,vector<SOCKET> &clients) {
  //send/recv
  cout << "client has got connected to the server"<<endl;
  char buffer[4096];
  while(1){
    int bytesrecvd = recv(clientSocket, buffer, sizeof(buffer), 0);
    if(bytesrecvd <= 0){
      cout << "client is been disconnected " << endl;
      break;
    }
    string message(buffer, bytesrecvd);
    cout << " message from the client :"<< message << endl;

    for(auto client : clients){
      if(client != clientSocket){
      send(client, message.c_str(),message.length(),0);
      }
    }
  }
  auto it = find(clients.begin(), clients.end(), clientSocket);
  if(it != clients.end())
  {
    clients.erase(it);
  }
  
  closesocket(clientSocket);
}
int main() {
 if (!Initialize()){
   cout <<"winsock initialization failed"<<endl;
   return 1;
 }
  cout << "This is Server Program!\n";
  SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if(listensocket == INVALID_SOCKET){
    cout << "socket creation failed"<<endl;
    return 1;
  }
  //create address structure
  sockaddr_in serveraddr;
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(12345);

  //convert ip address (0.0.0.0) put it inside the sin_family in binary format
  if (InetPton(AF_INET,_T("0.0.0.0"), &serveraddr.sin_addr) != 1) {
    cout<<"setting address structure failed"<<endl;
    closesocket(listenSocket);
    WSACleanup();
    return 1;
  }

  //bind the ip and port to scket
 if( bind(listenSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR){
   cout << "bind failed"<<endl;
   closesocket(listenSocket);
   WSACleanup();
   return 1;
 }
  //listen
  if(listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
    cout << "Listen failed" << endl;
    closesocket(listenSocket);
    WSACleanup();
    return 1;
  }
  cout<<"server has started listening to the port :"<< port <<endl;
  vector<SOCKET> clients;
  whiile(1){
  //accept
  SOCKET clientSocket = accept(listenSocket,nullptr,nullptr);
  if(clientSocket == INVALID_SOCKET){
    cout<< "invalid client socket "<< endl;
  }

  clients.push_back(clientSocket);
  thread t1(InteractWithClient, clientSocket,std::ref(clients));
  t1.detach();
      
  }
  closesocket(listenSocket);
  WSACleanup();
  return 0;
}
