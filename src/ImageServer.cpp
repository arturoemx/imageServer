#include "ImageServer.h"
#include <cstring>

ImageServer::ImageServer() {
	ImageServer(8888, "127.0.0.1");
}

ImageServer::ImageServer(int port, const char* inetAddress) {
	this->port = port;
	
	this->inetAddress = (char*) malloc( strlen(inetAddress) );
	strcpy(this->inetAddress, inetAddress);
}

void ImageServer::start() {
	serverConnection->acceptConnections();
}

void ImageServer::init() {
	serverConnection = new connServer<connectionData>(port, inetAddress, this->connectionHandler, MAX_CONNECTIONS);
}

void ImageServer::shutdown() {

}

void* ImageServer::connectionHandler(void *cd) {
	//Get the socket descriptor
    connectionData *conD = (connectionData *)cd;
    int sock = conD->client_socket;
    Content msg;
    
    //Recibe mensaje del cliente
    do
    {
        // Pedir al cliente que mande informacion
        strncpy ((char *)msg.cmd, "SND", 3);
        if (!Write(sock, 4, msg.cmd))
            break;
        cout << "Servidor mando SND" << endl;

        // Leer comando
        if (!Read(sock, 4, msg.cmd))
            break;

        cout << "Servidor recibio: " << msg.cmd << endl;

    
        // Responder con:
        // ACK si igual
        // ERR si diferente
        memset(msg.cmd, 0, 4);
        if(1)
            strncpy((char*) msg.cmd, "ACK", 3);
        else
            strncpy((char*) msg.cmd, "ERR", 3);

        if (!Write(sock, 4, msg.cmd))
            break;

        cout << "Servidor mando: " << msg.cmd << endl;

    }
    while(true);
}