// Server que atiende varios clientes mediante threads
// por cada cliente que se conecta se crea un hilo para atenderlo
//g++ server_threads.cpp -o server_threads -lpthread

#if __OCV_VER__ >= 3
#include <opencv2/opencv.hpp>
#else
#include <cv.h>
#endif
#include <highgui.h>
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
#include <imageBuffer.h>
#include <infoFrame.h>

#define BACKLOG 5
#define IMGBUFFERSIZE 10

using namespace std;
using namespace cv;
 
//función que atiende a un cliente

struct connectionData
{
    int client_socket;
    imgBuffer<infoFrame> *imBuff;
     
    connectionData(imgBuffer<infoFrame> *ib = 0)
    {
        client_socket = -1;
        imBuff = ib;
    }
    connectionData(int cs, imgBuffer<infoFrame> *ib=0)
    {
        client_socket = cs;
        imBuff = ib;
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
        connServer(int pt, const char *ipa, void *(*connHndlr)(void *),int  mxConn, X *connDataBase = 0)
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
            if (connDataBase)
                for (int i=0;i<maxConnections;++i)
                    connData[i] = *connDataBase;
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

struct camInfo
{
    string fileName;
    int camId;
    bool Run;
    int period;
    unsigned int counter;
    imgBuffer<infoFrame> imBuff;
    camInfo(int N=10)
    {
        fileName = string("None");
        camId = 0;
        Run = true;
        period = 33000;
        counter = 0;
        imBuff.setBufferSize(N);
    }
    camInfo (char *fname, int N=10)
    {
        initObject(fname);
        imBuff.setBufferSize(N);
    }
    void initObject(char *fname)
    {
        if (isdigit(fname[0]))
        {
            camId = atoi(fname);
            fileName = string("None");
        }
        else
        {
            fileName = string(fname);
            camId = -1;
        }
        Run = true;
        period = 33000;
        counter = 0;
    }
};

void *saveImages(void *cd)
{
    int i;
    camInfo *cI = (camInfo *)cd;
    char fName[256];
    infoFrame iF;
    for (i=0;i<40;++i)
    {
        cI->imBuff.getLast(iF);
        cout << "Se obtuvo el frame: " << i << " con ts = " << iF.t.tv_usec << endl;
        cout.flush();
        snprintf(fName, 255, "Img_%03d.png", i);
        imwrite(fName, iF.frame);
        usleep(500000);
    }
    return 0;
}

void *captureThread(void *cd)
{
    infoFrame iFrm;
    camInfo *cI = (camInfo *)cd;
    VideoCapture cap;
    int fill = cI->imBuff.getSize();
    
    if (cI->fileName == string("None") && cI->camId != -1)
        cap.open(cI->camId);
    else
        if (cI->fileName != string("None"))
            cap.open(cI->fileName.c_str());

    if (!cap.isOpened())
    {
        cerr << "Error in captureThread: No se pudo abrir la cámara/archivo" << endl;
        cerr.flush();
        pthread_exit(0);
    }

    cI->Run = true;
    while (cI->Run)
    {
       cout.flush();
       cap >> iFrm.frame;
       iFrm.setTime();
       if (iFrm.frame.empty())
       {
        cerr << "Warning in captureThread: frame.empty" << endl;
       }
       else
       {
            if (fill) //Llenamos el buffer primero.
            {
                cI->imBuff.Queue(iFrm);
                fill--;
            }
            else
            {
                cI->imBuff.advHead();
                cI->imBuff.Queue(iFrm);
            }
       }
       usleep(cI->period);
       cI->counter++;
    }
    return 0;
}
 
int main(int argc , char *argv[])
{
    camInfo cI(15);
    connectionData cD(-1, &cI.imBuff);
    pthread_t camThread,testThread;
    connServer<connectionData> cnS(8888, "", connection_handler, 1000, &cD); 

    if (argc > 1)
        cI.initObject(argv[1]);
        

    if( pthread_create( &camThread, NULL, captureThread, (void *)&cI))
    {
        perror("No se pudo crear el hilo de captura de imágenes");
        return 0;
    } 

/*    if( pthread_create( &testThread, NULL, saveImages, (void *)&cI))
    {
        perror("No se pudo crear el hilo de captura de imágenes");
        return 0;
    } 
*/
    cnS.acceptConnections();

    return 0;
}
 

