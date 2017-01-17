/*!
\file Client.h
\brief 
*/

#ifndef __SOCKIO__
#define __SOCKIO__

#define MAXCHUNK 4096


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

int Read (int sock, u_long tam, unsigned char *buffer);
int Write (int sock, u_long tam, unsigned char *buffer);

#endif
