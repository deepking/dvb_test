#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    struct ifreq interface;
    char* strIfName = "dvb0";
    int sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sd < 0) {
        perror("socket");
    }

    // bind specific interface
    memset(&interface, 0, sizeof(interface));
    strncpy(interface.ifr_name, strIfName, strlen(strIfName));
    strncpy(interface.ifr_ifrn.ifrn_name, strIfName, strlen(strIfName)); 
    if (setsockopt(sd, SOL_SOCKET, SO_BINDTODEVICE, &interface, sizeof(interface)) < 0) {
        perror("setsockopt_bind");
        close(sd);
    }

    // broadcast
    int broadcastEnable = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0) {
        perror("setsockopt_bcast");
        close(sd);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5566);
    addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    //addr.sin_addr.s_addr = inet_addr("10.0.2.16");

    /*
    if (bind(sd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("bind");
    }
    */

    char* strMsg = "hello";
    int i = 0;
    for (i = 0; i < 1; i++)
        sendto(sd, strMsg, strlen(strMsg), 0, (struct sockaddr*) &addr, sizeof(addr));

    return 0;
}

