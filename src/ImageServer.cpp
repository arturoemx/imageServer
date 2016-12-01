#include "ImageServer.h"
#include "SockIO.h"
#include <cstring>


Camera *ImageServer::cam;

ImageServer::ImageServer() {
	ImageServer(8888, "127.0.0.1");
}

ImageServer::ImageServer(int port, const char* inetAddress) {
    // config Server
	this->port = port;

	this->inetAddress = (char*) malloc( strlen(inetAddress) );
	strcpy(this->inetAddress, inetAddress);

    // init camera and assign thread to it
    cam = new Camera(0);

    // init connection to server
    serverConnection = new ConnServer<connectionData>(port, inetAddress, (ImageServer::connectionHandler), MAX_CONNECTIONS);
}

void ImageServer::start() {
	serverConnection->acceptConnections();
}

void ImageServer::shutdown() {

}


void *ImageServer::connectionHandler(void *cd) {
	//Get the socket descriptor
    connectionData *conD = (connectionData *)cd;
    int sock = conD->client_socket;
    unsigned char msg[MSG_LENGTH];
     
    //Recibe mensaje del cliente
    do
    {   


        // Pedir al cliente que mande informacion
        memset(msg, 0, MSG_LENGTH);
        strncpy ((char *)msg, "SND", 3);
        if (!Write(sock, MSG_LENGTH, msg))
            break;
        cout << "Servidor mando SND" << endl;

        // Leer comando
        memset(msg, 0, MSG_LENGTH);
        if (!Read(sock, MSG_LENGTH, msg))
            break;

        cout << "Servidor recibio: " << msg << endl;

        // Check what to send
        if(strncmp((char*)msg, "IMG", 3) == 0) {
            // Send an [IMG]

            // Get frame from camera
            Mat img;
            img = cam->getLastFrame();

            // Get info
            struct ImageInfo imgInfo;
            imgInfo.rows = img.rows;
            imgInfo.cols = img.cols;
            imgInfo.type = img.type();
            imgInfo.size = img.total()*img.elemSize();

            // send info
            if(!Write(sock, sizeof(struct ImageInfo), (unsigned char*)&imgInfo))
                break;

            // send data
            uchar *data;
            data = img.data;
            Write(sock, imgInfo.size, data);

            cout << "Servidor mando IMG[" << imgInfo.rows <<  " x " << imgInfo.cols << "]: " << imgInfo.type << " (" << imgInfo.size << ")" << endl;

        } else if(strncmp((char*)msg, "POLL", 4) == 0) {
            // Send ACK
            memset(msg, 0, MSG_LENGTH);
            strncpy ((char *)msg, "ACK", 3);
            if (!Write(sock, MSG_LENGTH, msg))
                break;
        } else {
            // Send ERR
            memset(msg, 0, MSG_LENGTH);
            strncpy ((char *)msg, "ERR", 3);
            if (!Write(sock, MSG_LENGTH, msg))
                break;
        }
    }
    while(true);

    exit(0);
    return (void*)0;
}

