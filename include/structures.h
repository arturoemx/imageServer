#ifndef STRUCTURES_H
#define STRUCTURES_H

#define WIDTH 512
#define HEIGHT 512

#define WRITE_ERR 0
#define READ_ERR 1

typedef union
{   
    unsigned char image[WIDTH*HEIGHT];
    unsigned char cmd[4];
} Content;


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

#endif 