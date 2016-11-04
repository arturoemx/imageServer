// Server que atiende varios clientes mediante threads
// por cada cliente que se conecta se crea un hilo para atenderlo
//g++ server_threads.cpp -o server_threads -lpthread

 
#include<stdio.h>
#include<string.h>    
#include<stdlib.h>    
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //para los hilos , compilar con -lpthread
#include<iostream>
#include<cstdio>
#include<vector>
#include <serviceCall.h>
#include <SockIO.h>

#define BACKLOG 5

using namespace std;
 
//función que atiende a un cliente

struct connectionData
{
    int client_socket;
    connectionData()
    {
        client_socket = -1;
    }
    connectionData(int cs)
    {
        client_socket = cs;
    }
};




template <typename X>
class connServer
{
        int maxConnections, nConnections;
        int socket_desc, port;
        struct sockaddr_in server, client;
        string ipAddress;
        bool accCon;
        void *(*connection_handler)(void *);
        X *connData;
        pthread_t *thrIds;

    public:
        connServer(int pt, const char *ipa, void *(*connHndlr)(void *),int  mxConn)
        {
            accCon = false;
            connection_handler = connHndlr;
            socket_desc = socket(AF_INET , SOCK_STREAM , 0);
            maxConnections = mxConn;;
            nConnections = 0;
            if (socket_desc == -1)
            {
                perror ("connServer Constructor: Creating socket endoint\n");
                return;
            }
            port = pt;
            server.sin_family = AF_INET;
            if (ipa[0]=='\0')
            {
                server.sin_addr.s_addr = INADDR_ANY;
                ipAddress = string("Anny");
            }
            else
            {
                ipAddress = string(ipa);

                if (!inet_aton (ipAddress.c_str(), &server.sin_addr))
                {
                    cerr << "conServer Constructor: invalid IP address: " << ipAddress << endl;
                    return;
                }
            }
            server.sin_port = htons( port );

            if (bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
            {
                perror ("connServer Constructor: binding socket endpoint\n");
                return;
            }
        
            if (listen(socket_desc, BACKLOG) == -1)
            {
                perror("connServer Constructor: error on listen.");
                return;
            }
            accCon = true;
            connData = new X[maxConnections];
            thrIds = new pthread_t[maxConnections];
        }
        void acceptConnections()
        {
            int clntSocket, sockAddInSize = sizeof(struct sockaddr_in);

            while (accCon)
            {
                if (nConnections < maxConnections)
                {
                    clntSocket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&sockAddInSize);
                    if (clntSocket == -1)
                    {
                        perror("connServer:acceptConnections");
                        accCon = false;
                    }
                    else
                    {
                        connData[nConnections].client_socket = clntSocket;
                        if( pthread_create( &(thrIds[nConnections]), NULL, connection_handler, (void*) &(connData[nConnections])))
                        {
                            perror("No se pudo crear el hilo");
                            return;
 
                        }
                        else
                            nConnections++;
                    }
                }
            }
        }
};

typedef union
{
    int val;
    unsigned char cmd[4];
} Content;


//**************
//Función que atiende a cada cliente
//**************
void *connection_handler(void *cd)
{
    //Get the socket descriptor
    connectionData *conD = (connectionData *)cd;
    int sock = conD->client_socket;
    Content msg;
    msg.val = 0;
    
    string message; 
     
    //Recibe mensaje del cliente
    do
    {
        strncpy ((char *)msg.cmd, "SND", 3);
        if (!Write(sock, 4, msg.cmd))
         break;
        if (!Read(sock, 4, msg.cmd))
         break;
        cout << "SRV: Se leyo el valor " << msg.val << endl;
        msg.val >>= 1;
        if (!Write(sock, 4, msg.cmd))
         break;
    }
    while(true);

    return 0;
}
 
int main(int argc , char *argv[])
{
    connServer<connectionData> cnS(8888, "", connection_handler, 1000); 

    cnS.acceptConnections();

    return 0;
}
 

