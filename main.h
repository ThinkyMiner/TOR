#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PROXY "127.0.0.1"
#define PORT 9050
#define req_size sizeof(Req)
#define res_size sizeof(Res)

typedef unsigned char int8;
typedef unsigned short int16;
typedef unsigned int int32;

/*
        +----+----+----+----+----+----+----+----+----+----+....+----+
        | VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
        +----+----+----+----+----+----+----+----+----+----+....+----+
           1    1      2              4           variable       1
*/

struct proxy_request
{
    int8 vn;
    int8 cd;
    int16 dstport;
    int32 dstip;
    unsigned char userid[8];
    int8 null;
};

typedef struct proxy_request Req;

/*
        +----+----+----+----+----+----+----+----+
        | VN | CD | DSTPORT |      DSTIP        |
        +----+----+----+----+----+----+----+----+
           1    1      2              4
*/

struct proxy_response
{
    int8 vn;
    int8 cd;
    int16 _dstport;
    int32 _dstip;
};
typedef struct proxy_response Res;

Req *request(const char *, const int *);
int main(int, char **);