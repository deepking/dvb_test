#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5566
#define SERVER_IP "127.0.0.1"

int main(int argc, const char *argv[])
{
    int s;
    int len;
    struct sockaddr_in addr;
    int addr_len = sizeof(struct sockaddr_in);
    char buffer[256];

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        len = read(STDIN_FILENO, buffer, sizeof(buffer));
        sendto(s, buffer, len, 0, &addr, addr_len);
        len = recvfrom(s, buffer, sizeof(buffer), 0, &addr, &addr_len);
        buffer[255] = '\0';
        printf("recv: %s", buffer);
    }
    return 0;
}

