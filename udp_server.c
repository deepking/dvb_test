#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5566
#define SERVER_IP "10.0.2.16"

int main(int argc, const char *argv[])
{
    int sockfd, len;
    struct sockaddr_in addr;
    struct sockaddr_in addr_send;
    int addr_len = sizeof(struct sockaddr_in);
    char buffer[256];
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    //addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_addr.s_addr = inet_addr("10.0.2.15");

    memset(&addr_send, 0, addr_len);
    addr_send.sin_family = AF_INET;
    addr_send.sin_port = htons(PORT);
    addr_send.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (bind(sockfd, &addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(1);
    }

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        memcpy(buffer + 40, "hello", 5);
        sendto(sockfd, buffer, sizeof(buffer), 0, &addr_send, &addr_len);
        len = recvfrom(sockfd, buffer, sizeof(buffer), 0, &addr, &addr_len);
        buffer[255] = '\0';
        printf("%s\n", buffer);
    }
    
    return 0;
}
