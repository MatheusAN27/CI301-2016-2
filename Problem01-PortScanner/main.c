#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include "data.c"
void scan(char* ip, int port){
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        int sock;
        struct sockaddr_in sa;
        int connection;
        memset(&sa, 0, sizeof(sa));
        sa.sin_family = AF_INET;
        sa.sin_addr.s_addr = inet_addr(ip);
        sa.sin_port = htons(port);
        sock = socket(AF_INET, SOCK_STREAM, 6);
        if(sock < 0) {
                fprintf(stderr, "Cannot create socket");
                return;
        }
        if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
        {
                fprintf(stderr, "setsockopt failed\n");
                return;
        }
        connection = connect(sock, (struct sockaddr*)&sa, sizeof(sa));
        if (connection != -1) {
                char buffer[4096];
                write(sock, "garbage\r\n", strlen("garbage\r\n"));
                memset(buffer, 0, sizeof(buffer));
                read(sock, buffer, sizeof(buffer));
                if (strlen(buffer) == 0) {
                  char host[1024];
                  char service[1024];
                  getnameinfo((struct sockaddr*)&sa, sizeof(sa),
                              host, sizeof(host),
                              service, sizeof(service), 0);
                  printf("%s\t%d\t%s\n", ip, port, service);
                }
                else{
                  printf("%s\t%d\t%s\n", ip, port, buffer);
                }
        }
        close(sock);
}

char* timestamp(){
        time_t ltime;
        ltime=time(NULL);
        return asctime( localtime(&ltime) );
}

int main(int argc, char *argv[]) {
        Data* data = get_data(argc, argv);
        char* ip;
        printf("Varredura iniciada em %s\n", timestamp());
        printf("IP:%s\n", argv[1]);
        if (argc > 2) {
                printf("Portas:%s\n", argv[2]);
        }else{
                printf("Portas:%d-%d\n", 1,65535);
        }
        puts("---");
        for (uint current_ip = data->ip_start; current_ip <= data->ip_end; ++current_ip) {
                ip = String_concat(data->base_ip,
                                   String_int_to_string(current_ip)
                                   );
                for (uint port = data->port_start; port <= data->port_end; ++port)
                        scan(ip, port);
        }
        return 0;
}
