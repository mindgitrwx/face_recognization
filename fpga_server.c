/*
    C socket server example
*/
 
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <errno.h>

 
#define MAXBUF 1024

int main(int argc , char *argv[])
{
    int    socket_desc        , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char   client_message[2000];

    //Todo
    char frame_flag_name[100];
    char str_end[20] = "end";
    int  frame_flag = 0;
     
    int server_sockfd, client_sockfd;
    int des_fd;
    int client_len, read_len, file_read_len;
    char buf[MAXBUF];


    client_len = sizeof(client);

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port        = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    server.sin_family = AF_INET;                     // 주소체계 
    server.sin_port   = htons( 8888 );        c = sizeof(struct sockaddr_in);
     
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
 
    //Receive a message from client---------------------------------------------------
    while(1) { 
        char file_name[MAXBUF]; // local val
        memset(buf, 0x00, MAXBUF);

        /* accept() */
      //  client_sockfd = accept(server_sockfd, (struct sockaddr *)&clientaddr, &client_len);
        printf("New Client Connect : %s\n", inet_ntoa(client.sin_addr));
        
        /* file name */
        read_len = read(client_sock, buf, MAXBUF);
        if(read_len > 0)
        {
            strcpy(file_name, buf);
            printf("%s > %s\n", inet_ntoa(client.sin_addr), file_name);
        }
        else
        {
            close(client_sock);
            break;
        }
        
        /* create file */
        FILE_OPEN: des_fd = open(file_name, O_WRONLY | O_CREAT | O_EXCL, 0700);
        if(!des_fd)
        {
            perror("file open error : ");
            break;
        }
        if(errno == EEXIST)
        {
            close(des_fd); size_t len = strlen(file_name);
            file_name[len++] = '_';
            file_name[len++] = 'n';
            file_name[len] = '\0';
            goto FILE_OPEN;
        }
       
        /* file save */
        while(1)
        {
            memset(buf, 0x00, MAXBUF);
            file_read_len = read(client_sock, buf, MAXBUF);
            write(des_fd, buf, file_read_len);
            if(file_read_len == EOF | file_read_len == 0)
            {
                printf("finish file\n");
                break;
            }
        }

        close(client_sock);
        close(des_fd);
    }
    close(server_sockfd);
    //Read Size 부분을 수정해야 한다 
    //while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    //{
        //Send the message back to client
        //Client message를 Circle, Lectangle 등을 통해 넣는 것을 만든다

        

    //    write(client_sock , client_message , strlen(client_message));
    //}

    //Todo: changed location of code
   
    return 0;
}
