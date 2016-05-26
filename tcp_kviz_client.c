/***************************************/
/*             TCP client              */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024
#define PORT_NO 9400
#define error(a,b) fprintf(stderr, a, b)


int send_msg(char* message);
char* recive_msg();


   /* Declarations */
   int fd;                         // socket endpt
   int flags;                      // rcv flags
   struct sockaddr_in server;        // socket name (addr) of server
   struct sockaddr_in client;      // socket name of client
   int server_size;                // length of the socket addr. server
   int client_size;                // length of the socket addr. client
   int bytes;                      // length of buffer
   int rcvsize;                    // received bytes
   int trnmsize;                   // transmitted bytes
   int err;                        // error code
   int ip;                         // ip address
   char on;                        //
   char buffer[BUFSIZE+1];         // datagram dat buffer area
   char server_addr[16];           // server address
   char * user_buffer;
   int vege = 0;
   int felad1 = 0, felad2 = 0;

int main(int argc, char *argv[] ) {// arg count, arg vector

   /* Initialization */
   on    = 1;
   flags = 0;
   server_size = sizeof server;
   client_size = sizeof client;
   sprintf(server_addr, "%s", argv[1]);
   ip = inet_addr(server_addr);
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = ip;
   server.sin_port        = htons(PORT_NO);

   

   /* Creating socket */
   fd = socket(AF_INET, SOCK_STREAM, 0);
   if (fd < 0) {
      error("%s: Socket creation error.\n",argv[0]);
      exit(1);
   }

   /* Setting socket options */
   setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /* Connecting to the server */
   err = connect(fd, (struct sockaddr *) &server, server_size);
   if (err < 0) {
      error("%s: Cannot connect to the server.\n", argv[0]);
      exit(2);
   }  
   
   rcvsize = recv( fd, buffer, BUFSIZE, flags );
   if (rcvsize < 0) {
      error("%s: Cannot receive data from the socket.\n", argv[0]);
      exit(5);
   }
   
   if(strcmp(buffer,"Várakozás a másik játékosra...") == 0 || strcmp(buffer,"Te vagy az első játékos...") == 0){
       if(strcmp(buffer,"Várakozás a másik játékosra...") == 0){
           printf("%s\n", buffer); //várakozás a másik játékosra...
       }
   }
       
       sprintf(buffer, "OK client");
       bytes = strlen(buffer) + 1;
       trnmsize = send(fd, buffer, bytes, flags);
       if (trnmsize < 0) {
          error("%s: Cannot send data to server.\n", argv[0]);
          exit(3);
          }
          
       rcvsize = recv( fd, buffer, BUFSIZE, flags );
       if (rcvsize < 0) {
          error("%s: Cannot receive data from the socket.\n", argv[0]);
          exit(5);
          }
       printf("%s\n", buffer); //kezdődik a játék...

       recive_msg();
        
        while(vege != -1){
   

            if( vege == 1 ){

                    recive_msg();
                    recive_msg();

                    vege = -1;
                    close(fd);
                    break;
            }

            else if( felad1 == 1 || felad2 == 1){           
                vege = -1;

                close(fd);
                
                break;

            }


            else{ //#6

                printf("Valasz: ");
                scanf("%s", buffer);
                send_msg(buffer);

                recive_msg();
          
                recive_msg();
            }

               


                

          }
      
    
    close(fd);

    return 0;
}

int send_msg(char* message){
  strcpy(buffer, message);
  bytes=strlen(message);
  err=send(fd, buffer, 1024, flags);
  if(err<0){
    printf("Az adat kuldese a szerverre sikertelen\n");
  }
  printf("\n\n");
  return 1;
}

char* recive_msg(){  // Üzenetek fogadása a szervertől
    user_buffer=(char*)calloc(1024,sizeof(char));

    err=recv(fd, buffer, 1024, flags);
    if(err<0){
        printf("No message arrived from server\n");
        exit(4);
    }
    strcpy(user_buffer, buffer);

    if( strstr(user_buffer, "Vegeredmeny") != NULL){ 
          printf("->>>>  %s  <<<<-\n", user_buffer);
    }    


    else if( strcmp(user_buffer, "Nyertel!") == 0 || 
        strcmp(user_buffer, "Vesztettel!") == 0 ||
        strcmp(user_buffer, "Dontetlen!") == 0 ){

        printf("Jatszma eredmenye: %s\n", user_buffer);

    }
    else if( strcmp(user_buffer, "vege") == 0 ){ //#1
        vege = 1;
    }
    else if(  strcmp( user_buffer, "A jatek veget ert! Nyertel!") == 0 || 
              strcmp( user_buffer, "A jatek veget ert! Vesztettel!") == 0 ||
              strcmp( user_buffer, "A jatek veget ert! Dontetlen!") == 0 ){
     // printf("Vegeredmeny: \n");
     printf("  %s\n", user_buffer);

    }
    else if( strcmp( user_buffer, "Feladtad a jatekot!") == 0 &&
        felad2 == 0){ //#4
      printf("Vesztettel!\n      Feladtad a jatekot!\n");
      printf("Jatek vege!\n");
      felad1 = 1;
    }
    else if( strcmp( user_buffer, "Az ellenfel feladta!") == 0 &&
        felad1 == 0){ //#4
      printf("Nyertel!\n      Az ellenfel feladta a jatekot!\n");

      printf("Jatek vege!\n");
      felad2 = 1;
    }


    else{

        printf("Kerdes: %s\n", user_buffer);

    }
    return user_buffer;
}