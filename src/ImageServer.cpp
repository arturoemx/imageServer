#include "ImageServer.h"
#include "SockIO.h"
#include <cstring>

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
	// serverConnection->acceptConnections();
}

void ImageServer::shutdown() {
}

void *ImageServer::connectionHandler(void *cd) {
	//Get the socket descriptor
    connectionData *conD = (connectionData *)cd;
    int sock = conD->client_socket;
    unsigned char msg[4];
    
    namedWindow("serv", 1);
    Mat img;    
    //Recibe mensaje del cliente
    do
    {
        // Pedir al cliente que mande informacion
        strncpy ((char *)msg, "SND", 3);
        if (!Write(sock, 4, msg))
            break;
        cout << "Servidor mando SND" << endl;

        // Leer comando
        memset(msg, 0, 4);
        if (!Read(sock, 4, msg))
            break;

        cout << "Servidor recibio: " << msg << endl;

        memset(msg, 0, 4);

        // Check what to send
        if(strncmp((char*)rcmd, "IMG", 3)) {
            // Get image
            // cout << cap.read(img) << endl;
            
            // Resize image
            Mat dst(512,512, img.type());
            resize(img, dst, dst.size());

            imshow("hg", dst);

            // Send data
            uchar *data;
            data = (uchar*)malloc(512*512);
            memcpy(data, dst.data, 512*512);
            Write(sock, dst.cols * dst.rows, data);

            cout << "Servidor mando IMG" << dst.size() << ": " << dst.type() << endl;

        } else if(strncmp((char*)rcmd, "PLL", 3)) {
            // Send ACK
            strncpy ((char *)msg, "ACK", 3);
            if (!Write(sock, 4, msg))
                break;
        } else {
            // Send ERR
            strncpy ((char *)msg, "ERR", 3);
            if (!Write(sock, 4, msg))
                break;
        }
    }
    while(true);

    return (void*)0;
}

