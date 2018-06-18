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

   printf("Reading Picture Size\n");
   int size;
   read(socket_desc, &size, sizeof(int));

   //Read Picture Byte Array
   printf("Reading Picture Byte Array\n");
   char p_array[size];
   read(socket_desc, p_array, size);
   
   //Convert it Back into Picture
   printf("Converting Byte Array to Picture\n");
   FILE *image;
   image = fopen("c1.bmp", "w");
   fwrite(p_array, 1, sizeof(p_array), image);
   fclose(image);


    return 0;
}
