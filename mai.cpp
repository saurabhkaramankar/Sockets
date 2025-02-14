#include <iostream>

#ifdef _WIN32
#include <Winsock2.h>
#endif

#include <WS2tcpip.h>
#include <tchar.h>
#include <thread>
using namespace std;

#pragma comment(lib,"ws2_32.lib")
/**
initialize winsocket library
create the socket
connect to the server 
recv and send
close the socket
clean the socket
  **/

bool Initialize(){
  WSADATA data;
  return WSAStartuo(MAKEWORD(2,2), &data) == 0;
}

void SendMsg(Socket s){
  cout<<"Enter your child name : "<<endl;
  string name;
  getline(cin , name);
  string message;
  while(1){
    getline(cin, message);
    string msg = name + " : " + message;
    int bytesent = send(s, msg.c_str(), msg.length(), 0);
    if(bytesent == SOCKET_ERROR){
      cout << "error sending message " << endl;
      break;
    }

  if(message == "quit"){
    cout << "stopping the application" <<endl;
    break;
  }
    
  }
  closesocket(s);
  WSACleanup();
}

void ReceiveMsg(Socket s){
  char buffer[4096];
  int recvlength;
  string mseg = "";
  while(1){
    recvlength = recv(s, buffer, sizeof(buffer), 0);
    if(recvlength <=0 ){
      cout<<"disconnected from the server"<<endl;
      break;
    }
    else{
      msg = string(buffer, recvlength);
      cout<< "server : " << msg << endl;
    }
  }
  closesocket(s);
  WSACleanup();
}
int main() {
 if (!Initialize()){
   cout <<"initialization of wisock failed"<<endl;
   return 1;
 }
  SOCKET s;

 
  s = socket(AF_INET, SOCK_STREAM, 0);
  if(s == INVALID_SOCKET){
    cout << "Invalid socket created"<<endl;
    return 1;
  }
  int port = 12345;
  string serveraddress = "127.0.0.1";
  //create address structure
  sockaddr_in serveraddr;
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(port);
  inet_pton(AF_INET, serveraddress.c_str(), &(serveraddr.sin_addr));
  
  

  //connect
 if( connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR){
   cout << "server not able to connect"<<endl;
   cout<<":"<< WSAGetLastError()<<endl;
   closesocket(listenSocket);
   WSACleanup();
   return 1;
 }

 cout << "connected to the server "<<endl;

  thread senderthread(SendMsg ,s);
  thread receiverthread(ReceiveMsg ,s);

  senderthread.join();
  receiver.join();
  return 0;
}