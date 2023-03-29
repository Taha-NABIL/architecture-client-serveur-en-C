#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "server2.h"
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

static void app(void)
{
   SOCKET sock = init_connection();
   char buffer[BUF_SIZE];
   /* the index for the array */
   int actual = 0;
   int max = sock;
   /* an array for all clients */
   Client clients[MAX_CLIENTS];
   char receiver[BUF_SIZE];
   char *buff;
   int message_number=0;
   char choice1[] = "1";
   int compare_choice1;
   char choice2[] = "2";
   int compare_choice2;
   char choice3[] = "3";
   int compare_choice3;
   char choice4[] = "4";
   int compare_choice4;
   char choice5[] = "5";
   int compare_choice5;
   char choice6[] = "6";
   int compare_choice6;
   char choice7[] = "7";
   int compare_choice7;
   char choice8[] = "8";
   int compare_choice8;
   char choice9[] = "9";
   int compare_choice9;
   




   int jeton_conexion[BUF_SIZE][BUF_SIZE];

   fd_set rdfs;

   while(1)
   {
      int i = 0;
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the connection socket */
      FD_SET(sock, &rdfs);

      /* add socket of each client */
      for(i = 0; i < actual; i++)
      {
         FD_SET(clients[i].sock, &rdfs);
      }

      if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }

      /* something from standard input : i.e keyboard */
      if(FD_ISSET(STDIN_FILENO, &rdfs))
      {
         /* stop process when type on keyboard */
         break;
      }
      else if(FD_ISSET(sock, &rdfs))
      {
         /* new client */
         SOCKADDR_IN csin = { 0 };
         size_t sinsize = sizeof csin;
         int csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
         if(csock == SOCKET_ERROR)
         {
            perror("accept()");
            continue;
         }



         /* after connecting the client sends its name */
         
	 if(read_client(csock, buffer) == -1)
         {
            /* disconnected */
            continue;
         }
         ///add a user if he doesn't exist else add him
	    FILE *check=fopen("clients_names","r");
	    
	    char line[20];
	    char user[BUF_SIZE];
	    snprintf(user,BUF_SIZE,"%s\n",buffer);
	    int w=1;
	    
	    
	    while(fgets(line,sizeof(line)-1,check)){
	    
	    if(strcmp(line,user)!=0){
	        w=1;
	        
	    }
	    else{
	        bzero(line,20);
	        w=0;
	        break;
	    }
	    }fclose(check);
	    if(w){
	        
	       FILE *type=fopen("clients_names","a");
	       fprintf(type,"%s\n",buffer);
           fclose(type);
	    }
	    

         /* what is the new maximum fd ? */
         max = csock > max ? csock : max;

         FD_SET(csock, &rdfs);
         Client c = { csock };
         strncpy(c.name, buffer, BUF_SIZE - 1);
         clients[actual] = c;
         actual++;

      }
      else

      {
         int i = 0;
         for(i = 0; i < actual; i++)
         {
            /* a client is talking */
            if(FD_ISSET(clients[i].sock, &rdfs))

            {
               Client client = clients[i];
               int c = read_client(clients[i].sock, buffer);
	       //hadi partie dyalo
	       
	           memcpy(receiver, buffer, BUF_SIZE - 1);
				

	       //hadi partie dyali
	     //  int jeton_conexion[BUF_SIZE][BUF_SIZE];
	       message_number=1;
       	       if(jeton_conexion[clients[i].sock][0]!=1){
		message_number = 0;
		  jeton_conexion[clients[i].sock][0]=1;
		}
	       

		
               /* client disconnected */
               if(c == 0)
               {
                  closesocket(clients[i].sock);
                  remove_client(clients, i, &actual);
                  strncpy(buffer, client.name, BUF_SIZE - 1);
                  strncat(buffer, " disconnected !", BUF_SIZE - strlen(buffer) - 1);
                  send_message_to_all_clients(clients, client, actual, buffer, 1);
               }
               else
               {
		if(message_number==0){    
		compare_choice1 = strcmp(buffer,choice1);
		compare_choice2 = strcmp(buffer,choice2);
		compare_choice3 = strcmp(buffer,choice3);
		compare_choice4 = strcmp(buffer,choice4);
		compare_choice5 = strcmp(buffer,choice5);
		compare_choice6 = strcmp(buffer,choice6);
		compare_choice7 = strcmp(buffer,choice7);
		compare_choice8 = strcmp(buffer,choice8);
		compare_choice9 = strcmp(buffer,choice9);
		
		
		
		}
		  if(message_number==1){
			  if(compare_choice3==0){
		  	send_message_to_all_clients(clients, client, actual, buffer, 0);
			compare_choice1 = 1;
		
		  	} if(compare_choice4 == 0){
		  
       
        read_client(client.sock, buffer);
        send_message_to_one_client_hist(client,receiver,buffer);
		send_message_to_one_client(clients, client, actual, receiver,buffer,0);		 
	//	send_message_to_group(clients, client, actual, receiver, buffer ,0);
		write_client(clients[i].sock, "\nPlease enter the option you want:\n\n\n1-Show a list of all members (on- and off- line.\n2-Show a list for online members.\n3-Send message to all members.\n4-Send message to a specific member.\n5-Show all groups.\n6-Create a group.\n7-Delete a group.\n8-add a member to a group.\n9-send message in a group.\n\nYour choice:\n");
		
		
		compare_choice2 = 1;
		

		  }if(compare_choice6==0){
		  // GROUPE ....
		create_group(client , buffer); 
write_client(clients[i].sock, "\nPlease enter the option you want:\n\n1-Show a list of all members (on- and off- line.\n2-Show a list for online members.\n3-Send message to all members.\n4-Send message to a specific member.\n5-Show all groups.\n6-Create a group.\n7-Delete a group.\n8-add a member to a group.\n9-send message in a group.\n\nYour choice:\n");
		  }if(compare_choice7==0) {
		  delete_group(client, buffer);
write_client(clients[i].sock, "\nPlease enter the option you want:\n\n1-Show a list of all members (on- and off- line.\n2-Show a list for online members.\n3-Send message to all members.\n4-Send message to a specific member.\n5-Show all groups.\n6-Create a group.\n7-Delete a group.\n8-add a member to a group.\n9-send message in a group.\n\nYour choice:\n");
		  }
		
		if(compare_choice2==0){
		show_online_members(clients, client, actual);
write_client(clients[i].sock, "\nPlease enter the option you want:\n\n1-Show a list of all members (on- and off- line.\n2-Show a list for online members.\n3-Send message to all members.\n4-Send message to a specific member.\n5-Show all groups.\n6-Create a group.\n7-Delete a group.\n8-add a member to a group.\n9-send message in a group.\n\nYour choice:\n");
		}
		
	       	if(compare_choice1==0){
		show_all_members(client);
write_client(clients[i].sock, "\nPlease enter the option you want:\n\n1-Show a list of all members (on- and off- line.\n2-Show a list for online members.\n3-Send message to all members.\n4-Send message to a specific member.\n5-Show all groups.\n6-Create a group.\n7-Delete a group.\n8-add a member to a group.\n9-send message in a group.\n\nYour choice:\n");
		}
		






		if(compare_choice9==0){
		   read_client(client.sock, buffer);
		   send_message_to_group(clients, client, actual, receiver, buffer ,0);
	   write_client(clients[i].sock, "\nPlease enter the option you want:\n\n1-Show a list of all members (on- and off- line.\n2-Show a list for online members.\n3-Send message to all members.\n4-Send message to a specific member.\n5-Show all groups.\n6-Create a group.\n7-Delete a group.\n8-add a member to a group.\n9-send message in a group.\n\nYour choice:\n");
						
		  }if(compare_choice8==0){
		 read_client(client.sock, buffer);

		  join_groupe(client,receiver,buffer);
		write_client(clients[i].sock, "\nPlease enter the option you want:\n\n1-Show a list of all members (on- and off- line.\n2-Show a list for online members.\n3-Send message to all members.\n4-Send message to a specific member.\n5-Show all groups.\n6-Create a group.\n7-Delete a group.\n8-add a member to a group.\n9-send message in a group.\n\nYour choice:\n");
						
		  
		  }if(compare_choice5==0){
		  
		  show_all_groups(client);
write_client(clients[i].sock, "\nPlease enter the option you want:\n\n1-Show a list of all members (on- and off- line.\n2-Show a list for online members.\n3-Send message to all members.\n4-Send message to a specific member.\n5-Show all groups.\n6-Create a group.\n7-Delete a group.\n8-add a member to a group.\n9-send message in a group.\n\nYour choice:\n");		  
}
			  else{
			  
/*		  
 closesocket(clients[i].sock);
 remove_client(clients, i, &actual);
 strncpy(buffer, client.name, BUF_SIZE - 1);
 strncat(buffer, " disconnected !", BUF_SIZE - strlen(buffer) - 1);
 send_message_to_all_clients(clients, client, actual, buffer, 1);
*/

		  }
jeton_conexion[clients[i].sock][0]=0;
}}
               break;
            }
         }
      }
   }

   clear_clients(clients, actual);
   end_connection(sock);
}

static void show_online_members(Client *clients  , Client sender, int actual){
	write_client(sender.sock, "Here is a list of online members:");
	for(int i = 0; i<actual; i++){
		write_client(sender.sock, "\n");
		write_client(sender.sock, clients[i].name);
		
	}write_client(sender.sock, "\n");

}
static void show_all_groups(Client sender){
	 write_client(sender.sock, "Here is a list of all groups:");
	FILE *check = fopen("groups","r");
	char line[20];
	char owner[20];
	 while(fgets(line,sizeof(line)-1,check)){
		 
	write_client(sender.sock,line);

       write_client(sender.sock,"\n");



	 }fclose(check);
}
static void show_all_members(Client sender){
	 write_client(sender.sock, "Here is a list of all members:");
	char names[BUF_SIZE*100];
FILE *check = fopen("clients_names","r");
loadfile("clients_names" , names);
write_client(sender.sock, names);
}

static void clear_clients(Client *clients, int actual)
{
   int i = 0;
   for(i = 0; i < actual; i++)
   {
      closesocket(clients[i].sock);
   }
}

static void remove_client(Client *clients, int to_remove, int *actual)
{
   /* we remove the client in the array */
   memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
   /* number client - 1 */
   (*actual)--;
}
static void join_groupe(Client sender,const char *receiver,const char *buffer){//receiver is group name and buffer is the client name to add
char line[20];
char own[BUF_SIZE];
if(access(receiver,F_OK)==0){
FILE *check=fopen(receiver,"r");
fgets(line,sizeof(line)-1,check);
snprintf(own,BUF_SIZE,"%s\n",sender.name);
fclose(check);
if(strcmp(line,own)==0){
FILE *type=fopen(receiver,"a");
fprintf(type,"%s\n",buffer);																	                    																				                    fclose(type);																							                    }else{write_client(sender.sock,"YOU R NOT GROUP OWNER\n");}
}else{write_client(sender.sock,"this group doesn't exist\n");}
}


static void create_group(Client sender,const char* buffer){
char filename[20];
char groups_h[BUF_SIZE*100];
strcpy(filename,"groups");
 FILE *check=fopen(filename,"r");

	    char line[20];
	    char user[BUF_SIZE];
	    snprintf(user,BUF_SIZE,"%s\n",buffer);
	    int w=0;
	    while(fgets(line,sizeof(line)-1,check)){ 
	    if(strcmp(line,user)==0){
	         write_client(sender.sock, "\nGroup already exists\n");
		    break;
	    }
	    else{
	        bzero(line,20);
	     w=1;
	        
	    }
	    }
	    fclose(check);
	    if(w){
		     write_client(sender.sock, "\nyour group was created:\n");
		FILE *list_n = fopen(buffer,"w");
		fprintf(list_n,"%s",sender.name);
		fprintf(list_n,"%s","\n");
		fclose(list_n);

	     FILE *check_h=fopen(filename,"a");
	    fprintf(check_h,"%s", user);
	   fclose(check_h);
	  w=0; 
	    }



}





static void delete_group(Client sender,const char* buffer){
char filename[20];
char groups_h[BUF_SIZE*100];
strcpy(filename,"groups");
 FILE *check=fopen(filename,"r");
	    char line[20];
	    char user[BUF_SIZE];
	    snprintf(user,BUF_SIZE,"%s\n",buffer);
	    int w=0;
	    while(fgets(line,sizeof(line)-1,check)){
	    
	    if(strcmp(line,user)==0){
	        remove(buffer);
		    w=1;
	    }
	    else{
		strcpy(groups_h,(strcat(groups_h,line)));
	        bzero(line,20);
	    // 
	     // write_client(sender.sock,groups_h);  
	    }
	    }
	   // fclose(check);
	    if(w){ write_client(sender.sock, "\nGroup deleted!\n");
		    fclose(check);
		  
	     
	FILE *check_h=fopen(filename,"w");
	    fprintf(check_h,"%s\n", groups_h);
	   fclose(check_h); 
	   
memset(groups_h,0,sizeof(groups_h));
	    }
	    if(w==0) write_client(sender.sock, "\nThe group name doesn't ewist\n");

}


static void send_message_to_all_clients(Client *clients, Client sender, int actual, const char *buffer, char from_server)
{
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   for(i = 0; i < actual; i++)
   {
      /* we don't send message to the sender */
      if(sender.sock != clients[i].sock)
      
	   {
         if(from_server == 0)
         {
            strncpy(message, sender.name, BUF_SIZE - 1);
            strncat(message, " : ", sizeof message - strlen(message) - 1);
         }
         strncat(message, buffer, sizeof message - strlen(message) - 1);
         write_client(clients[i].sock, message);
	 
      }else{write_client(clients[i].sock, "\nPlease enter the option you want:\n\n1-Show a list of all members (on- and off- line.\n2-Show a list for online members.\n3-Send message to all members.\n4-Send message to a specific member.\n5-Show all groups.\n6-Create a group.\n7-Delete a group.\n8-add a member to a group.\n9-send message in a group.\n\nYour choice:\n");}
   }
}

static int init_connection(void)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
   {
      perror("bind()");
      exit(errno);
   }

   if(listen(sock, MAX_CLIENTS) == SOCKET_ERROR)
   {
      perror("listen()");
      exit(errno);
   }

   return sock;
}

static void end_connection(int sock)
{
   closesocket(sock);
}

static int read_client(SOCKET sock, char *buffer)
{
   int n = 0;

   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      /* if recv error we disonnect the client */
      n = 0;
   }

   buffer[n] = 0;

   return n;
}

static void write_client(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}
static void send_message_to_one_client(Client *clients, Client sender, int actual, const char *receiver,const char *buffer, char from_server)
{
   
   int i = 0;
   char message[BUF_SIZE];
   //char buffer[BUF_SIZE];
   char filename[BUF_SIZE];
   char env[BUF_SIZE];
   char rec[BUF_SIZE];
   char tmp[BUF_SIZE];
   char hist[BUF_SIZE*100];
   message[0] = 0;
   char choice2[] = "2"; 

  

   for(i = 0; i < actual; i++)
   {
      if(strcmp(receiver,clients[i].name)==0){
        if(from_server == 0)
         {
          
            strncpy(message, sender.name, BUF_SIZE - 1);
            strncat(message, " : ", sizeof message - strlen(message) - 1);
            
         }
               
        
         //permutation de env et rec
         strncpy(env, sender.name, BUF_SIZE - 1);
         strncpy(rec, clients[i].name, BUF_SIZE - 1);
         if(strcmp(env,rec)>0){
            strncpy(tmp, rec, BUF_SIZE - 1);
            strncpy(rec, env, BUF_SIZE - 1);
            strncpy(env, tmp, BUF_SIZE - 1);
         }
         /////////////////////////////////////////////////////////
            /////creating file name
         strncpy(filename,env, BUF_SIZE - 1);
         strncat(filename, " <--->", sizeof filename - strlen(filename) - 1);
         strncat(filename, rec, sizeof filename - strlen(filename) - 1);
          ///creqtion du fichier 
         // FILE *out_file=fopen(filename,"a");
         /// affichage historique
         loadfile(filename,hist);
         write_client(sender.sock, hist);
         ///////////////////////////////
        // read_client(sender.sock, buffer);
        //////////////stockage dans l'historique
	 strncat(message, buffer, sizeof message - strlen(message) - 1);
	 write_client(clients[i].sock, message);
         
        // fprintf(out_file," %s : %s \n", sender.name,buffer);
         

      }
   }
}
static void send_message_to_one_client_hist(Client sender, const char *receiver,const char *buffer)
{
   
   
   //char buffer[BUF_SIZE];
   char filename[BUF_SIZE];
   char env[BUF_SIZE];
   char rec[BUF_SIZE];
   char tmp[BUF_SIZE];
   char hist[BUF_SIZE*100];
  
   

  
//fgets
        FILE *check=fopen("clients_names","r");
	    char line[20];
	    char user[BUF_SIZE];
	    snprintf(user,BUF_SIZE,"%s\n",receiver);
	    int w=1;
	    while(fgets(line,sizeof(line)-1,check)){
	    
	    if(strcmp(line,user)==0){
	        w=1;
	        break;
	    }
	    else{
	        bzero(line,20);
	        w=0;
	        
	    }
	    }
	    if(w){
	       fclose(check); 
	      //permutation de env et rec
          strncpy(env, sender.name, BUF_SIZE - 1);
          strncpy(rec, receiver, BUF_SIZE - 1);
         if(strcmp(env,rec)>0){
            strncpy(tmp, rec, BUF_SIZE - 1);
            strncpy(rec, env, BUF_SIZE - 1);
            strncpy(env, tmp, BUF_SIZE - 1);
         }
         /////////////////////////////////////////////////////////
            /////creating file name
         strncpy(filename,env, BUF_SIZE - 1);
         strncat(filename, " <--->", sizeof filename - strlen(filename) - 1);
         strncat(filename, rec, sizeof filename - strlen(filename) - 1);
          
          ///creqtion du fichier 
          FILE *out_file=fopen(filename,"a");
         //stockage dans l'historique
         //read_client(sender.sock, buffer);
       
         fprintf(out_file," %s : %s \n", sender.name,buffer);
         fclose(out_file);

	    }

}


static void send_message_to_group(Client *clients, Client sender, int actual, const char *receiver,const char *buffer, char from_server)
{
    
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   
   char hist[BUF_SIZE*100];
  if(access(receiver,F_OK)==0){///verifier si le groupe exist ///le decalage peut etre enlever en ajouter la fonction read client(buffer)to the code and reemove it from before
        FILE *check=fopen(receiver,"r");
	    char line[20];
	    char user[BUF_SIZE];
	    char resu[BUF_SIZE];
	    char filename[BUF_SIZE];
	    int w=0;
	    int z=0;
	    if(from_server == 0)
         {
            strncpy(message, sender.name, BUF_SIZE - 1);
            strncat(message, " : ", sizeof message - strlen(message) - 1);
         }
         strncat(message, buffer, sizeof message - strlen(message) - 1);
         ///nom d'historique
	     strncpy(filename,receiver, BUF_SIZE - 1);
         strncat(filename, " : ", sizeof filename - strlen(filename) - 1);
         strncat(filename,"chat", sizeof filename - strlen(filename) - 1);
         //verifie si le sender est dans le groupe
	    while(fgets(line,sizeof(line)-1,check)){
	        snprintf(resu,BUF_SIZE,"%s\n",sender.name);
	        if(strcmp(line,resu) ==0){
	            printf("exist\n");
	            w=1;
	            //ecriture historique
	            FILE *in_file=fopen(filename,"a");//ecriture
	            fprintf(in_file," %s : %s \n", sender.name,buffer);
                fclose(in_file);
               fclose(check);
	            break;
	            }
	            }
	            if(w==0){ write_client(sender.sock,"u dont belong here!!!!!\n");}

	    ////////////////////////
	    check=fopen(receiver,"r");
	    while(fgets(line,sizeof(line)-1,check)){
	    for(i = 0; i < actual; i++){
	        snprintf(user,BUF_SIZE,"%s\n",clients[i].name);
	        if(strcmp(line,user)==0 && sender.name != clients[i].name && w==1){
	           z=1;
	           write_client(clients[i].sock, message);
	      
	    }
	    else{
	       ///i dont think it's a bzero because we still need the line to stay full with content
	       printf("group members disconnected\n");
	        
	    }
	    
	    }
	}    
if(z==1){
	            ///creqtion du fichier 
                FILE *out_file=fopen(filename,"r");//lecture
                /// affichage historique
                bzero(hist,BUF_SIZE);
                loadfile(filename,hist);
                write_client(sender.sock, hist);
	          z=0;  
                
                }	        
   

   fclose(check); 
   
   }else{write_client(sender.sock,"this groupe doesn't exist\n");
   }
}





//fonction pour convertir le contenu d'un fichier texte en string 
char *loadfile(char *name,char *file){
	FILE *x = NULL; 
	int charactereActuel = 0;	
	int i = 0;
	x = fopen(name,"r");
	if(x!=NULL){
	do{
	charactereActuel = fgetc(x);
	file[i]=charactereActuel;
	i++;	
	}while(charactereActuel != EOF);

fclose(x);
	}

	return file;
}

int main(int argc, char **argv)
{
   
   init();

   app();

   end();

   return EXIT_SUCCESS;
}
