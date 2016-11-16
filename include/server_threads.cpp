// Server que atiende varios clientes mediante threads
// por cada cliente que se conecta se crea un hilo para atenderlo
//g++ server_threads.cpp -o server_threads -lpthread

 
#include <stdio.h>
#include <string.h>    
#include <stdlib.h>    
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h> //para los hilos , compilar con -lpthread
#include <iostream>
#include <cstdio>
#include <vector>
#include <serviceCall.h>
#include "SockIO.h"

#define BACKLOG 5

using namespace std;
 
//función que atiende a un cliente
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




