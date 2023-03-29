#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "client2.h"

static void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
   }
#endif
}

static void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}

static void app(const char *address, const char *name)
{
   SOCKET sock = init_connection(address);
   char buffer[BUF_SIZE];
   char receiver[BUF_SIZE];
   int s=0;
   fd_set rdfs;
   
   /* send our name */
   write_server(sock, name);
   
   printf("\nPlease enter the option you want:\n\nFor options, 1,2,5, you need to enter the options number and click on enter, then follow it by another check message by typing as an example -check- and then hit enter.\nFor the option 3, you need to enter the option number then click on okay, then type a message then click on enter.\nFor option 4, you need to enter the member name you want to send a message to, then click on enter, type your messaage and then click on enter.\nFor option 6 type 6 followed by enter, then enter the group name you want, and the same thing for option 7\n for option 8 you need to be the admin, enter the group name and then the client name you want to add.\nFor option 9 you need to specify the group, then type your message.\n\n Important info: If you try to type any other caracter, and confrim it you will be disconnected!!\n\nlist of options:\n1-Show a list of all members (on- and off- line.\n2-Show a list for online members.\n3-Send message to all members.\n4-Send message to a specific member.\n5-Show all groups.\n6-Create a group.\n7-Delete a group.\n8-add a member to a group.\n9-send message in a group.\n\nYour choice:\n");
		
   while(1)
   {
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the socket */
      FD_SET(sock, &rdfs);

      if(select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }
        
      /* something from standard input : i.e keyboard */
      if(FD_ISSET(STDIN_FILENO, &rdfs))
      {
          
            fgets(buffer, BUF_SIZE - 1, stdin);
         {
            char *p = NULL;
            p = strstr(buffer, "\n");
            if(p != NULL)
            {
               *p = 0;
            }
            else
            {
               /* fclean */
               buffer[BUF_SIZE - 1] = 0;
            }
         }
         write_server(sock, buffer);
      
      
      }
      else if(FD_ISSET(sock, &rdfs))
      {
         int n = read_server(sock, buffer);
         /* server down */
         if(n == 0)
         {
            printf("Server disconnected !\n");
            break;
         }
         puts(buffer);
      }
   }

   end_connection(sock);
}
//}
static int init_connection(const char *address)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };
   struct hostent *hostinfo;

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   hostinfo = gethostbyname(address);
   if (hostinfo == NULL)
   {
      fprintf (stderr, "Unknown host %s.\n", address);
      exit(EXIT_FAILURE);
   }

   sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
   {
      perror("connect()");
      exit(errno);
   }

   return sock;
}


static void end_connection(int sock)
{
   closesocket(sock);
}

static int read_server(SOCKET sock, char *buffer)
{
   int n = 0;

   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      exit(errno);
   }

   buffer[n] = 0;

   return n;
}

static void write_server(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

int main(int argc, char **argv)
{
   if(argc < 2)
   {
      printf("Usage : %s [address] [pseudo]\n", argv[0]);
      return EXIT_FAILURE;
   }

   init();

   app(argv[1], argv[2]);

   end();

   return EXIT_SUCCESS;
}
