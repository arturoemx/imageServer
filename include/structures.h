#ifndef STRUCTURES_H
#define STRUCTURES_H

#define SEND_FAILURE 0
#define SEND_SUCCESS 1

#define FORMAT_ERROR 3


#define MSG_LENGTH 10

struct ImageInfo {
        int rows;
        int cols;
        int type;
        int size;
};

#endif 