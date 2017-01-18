#include <ConnServer.h>


ConnServer::ConnServer (int pt, const char *ipa, void *(*connHndlr) (void *),
                        int mxConn)
{
       accCon = false;
       connection_handler = connHndlr;
       socketDesc = socket (AF_INET, SOCK_STREAM, 0);
       maxConnections = mxConn;
       nConnections = 0;
       if (socketDesc == -1)
       {
            perror ("ConnServer Constructor: Creating socket endpoint\n");
            return;
       }
       port = pt;
       server.sin_family = AF_INET;
       if (ipa[0] == '\0')
       {
            server.sin_addr.s_addr = INADDR_ANY;
            ipAddress = string ("Anny");
       }
       else
       {
            ipAddress = string (ipa);

            if (!inet_aton (ipAddress.c_str (), &server.sin_addr))
            {
                   cerr << "conServer Constructor: invalid IP address: " << ipAddress
                        << endl;
                   return;
            }
       }
       server.sin_port = htons (port);

       if (bind (socketDesc, (struct sockaddr *) &server, sizeof (server)) <
               0)
       {
            perror ("ConnServer Constructor: binding socket endpoint\n");
            return;
       }

       if (listen (socketDesc, BACKLOG) == -1)
       {
            perror ("ConnServer Constructor: error on listen.");
            return;
       }
       accCon = true;
       connData = new int[maxConnections];
       thrIds = new pthread_t[maxConnections];
}

ConnServer::~ConnServer()
{
   if (connData)
       delete[] connData;
   if (thrIds)
       delete[] thrIds;
}


void ConnServer::acceptConnections ()
{
       int clntSocket, sockAddInSize = sizeof (struct sockaddr_in);
       struct sockaddr_in client;

       while (accCon)
       {
            if (nConnections < maxConnections)
            {
                   clntSocket =
                        accept (socketDesc, (struct sockaddr *) &client,
                                        (socklen_t *) & sockAddInSize);
                   if (clntSocket == -1)
                   {
                        perror ("ConnServer:acceptConnections");
                        accCon = false;
                   }
                   else
                   {
                        connData[nConnections] = clntSocket;
                        if (pthread_create
                                (&(thrIds[nConnections]), NULL, connection_handler,
                                   (void *) &(connData[nConnections])))
                        {
                               perror ("No se pudo crear el hilo");
                               return;

                        }
                        else
                        {
                               nConnections++;
                               cout << "Se acaba de hacer una nueva conexión: " 
                                    << nConnections << endl;
                               cout.flush();
                        }
                   }
            }
       }
}
