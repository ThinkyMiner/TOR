#include "main.h"

Req *request(const char *dstip, const int *dstport)
{
    Req *req;
    req = malloc(req_size);
    req->vn = 4;
    req->cd = 1;
    req->dstport = htons(*dstport);
    req->dstip = inet_addr(dstip);
    strncpy((char *)req->userid, "anonymous", 8);
    return req;
}

int main(int argc, char *argv[])
{
    char *host;
    int port, s;
    struct sockaddr_in sock;
    Req *req;
    Res *res;
    int success;
    char buffer[res_size];
    char temp[512];

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
        return -1;
    }

    host = argv[1];
    port = atoi(argv[2]);

    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0)
    {
        perror("socket");
        return -1;
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(PORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    int var = connect(s, (struct sockaddr *)&sock, sizeof(sock));

    if (var != 0)
    {
        perror("connect");
        return -1;
    }

    printf("Connected to proxy");

    req = request(host, &port);
    write(s, req, req_size);
    memset(buffer, 0, res_size);
    if (read(s, buffer, res_size) < 1)
    {
        perror("read");
        free(req);
        close(s);
        return -1;
    }

    res = (Res *)buffer;
    success = (res->cd == 90);

    if (!success)
    {
        printf("Failed to traverse proxy error code %d\n", res->cd);
        fprintf(stderr, "[-] Failed to connect to %s:%d\n", host, port);
    }

    printf("Successfully connected to %s:%d\n", host, port);

    memset(temp, 0, 512);
    snprintf(temp, 511,
             "HEAD / HTTP/1.0\r\n"
             "HOST: www.iiitg.ac.in\r\n"
             "\r\n");

    write(s, temp, strlen(temp));

    memset(temp, 0, 512);
    read(s, temp, 511);
    printf("%s\n", temp);

    close(s);
    free(req);

    return 0;
}
