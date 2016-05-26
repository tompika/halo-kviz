/***************************************/
/*              TCP server             */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define BUFSIZE 1024                      // buffer size
#define PORT_NO 9400                      // port number
#define error(a,b) fprintf(stderr, a, b)  // error 'function'

char* recive_msg(int client_number);
int send_msg(int client_number, char* msg);
void printAllas();

   /* Declarations */
   int fd;                          // socket endpt
   int fdc1;                        // socket endpt
   int fdc2;
   int flags;                       // rcv flags
   struct sockaddr_in server;       // socket name (addr) of server
   struct sockaddr_in client01;     // socket name of client
   struct sockaddr_in client02;
   int server_size;                 // length of the socket addr. server
   unsigned int client01_size;      // length of the socket addr. client
   unsigned int client02_size;		// length of the socket addr. client
   int bytes;                       // length of buffer
   int rcvsize;                     // received bytes
   int trnmsize;                    // transmitted bytes
   int err;                         // error code
   char on;                         //
   char buffer[BUFSIZE+1];  

   char * user_buffer;
   char * user_buffer1;
   char * user_buffer2;

   int ertek1, ertek2;
   int pont1 = 0, pont2 = 0;
   int felad1 = 0, felad2 = 0;

int main(int argc, char *argv[] ){  // arg count, arg vector

   char * kerdes[4] = {"Mikor volt a mohacsi vesz?",
   					   "Mikor alakult meg a Szovjetunio?", 
   					   "Hany hetbol all egy ev?", 
   					   "Hany masodorperc 1 perc?", 
   					   "2 ^ 12 = ?",
   					   "10 + 10 = ?"};
   int  valasz[4] = {1526, 1922, 52, 60, 4096, 20};

   
   int vege01 = 0, vege02 = 0;
   int sikeres_veg = 0;
   int jatszma ;

   if(argc < 2){
        printf("Hibas bemenet!\n");
        exit(1);
   }
   else{
        jatszma = atoi(argv[1]);
        printf("Jatekok szama: %d\n",jatszma );
    }

            

   /* Initialization */
   on                     = 1;
   flags                  = 0;
   bytes                  = BUFSIZE;
   server_size            = sizeof server;
   client01_size          = sizeof client01;
   client02_size          = sizeof client02;
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port        = htons(PORT_NO);

   /* Creating socket */
   fd = socket(AF_INET, SOCK_STREAM, 0 );
   if (fd < 0) {
      error("%s: Socket creation error\n",argv[0]);
      exit(1);
      }

   /* Setting socket options */
   setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /* Binding socket */
   err = bind(fd, (struct sockaddr *) &server, server_size);
   if (err < 0) {
      error("%s: Cannot bind to the socket\n",argv[0]);
      exit(2);
      }

   /* Listening */
   err = listen(fd, 2);
   if (err < 0) {
      error("%s: Cannot listen to the socket\n",argv[0]);
      exit(3);
      }

   /* Accepting connection request client01 */
   fdc1 = accept(fd, (struct sockaddr *) &client01, &client01_size);
   if (fdc1 < 0) {
      error("%s: Cannot accept on socket\n",argv[0]);
      exit(4);
      }

   sprintf(buffer,"Várakozás a másik játékosra...");
   bytes = strlen(buffer) + 1;
   trnmsize = send(fdc1, buffer, bytes, flags);
   if (trnmsize < 0) {
      error("%s: Cannot send data to the client.\n",argv[0]);
      exit(5);
      }

   rcvsize = recv( fdc1, buffer, bytes, flags );
   if (rcvsize < 0) {
      error("%s: Cannot receive from the socket\n",argv[0]);
      exit(6);
      }

   /*****************************************************************/
   /* Accepting connection request client02 */  
   fdc2 = accept(fd, (struct sockaddr *) &client02, &client02_size);
   if (fdc2 < 0) {
      error("%s: Cannot accept on socket\n",argv[0]);
      exit(7);
      }

      
   sprintf(buffer,"Kezdődik a játék...");
   bytes = strlen(buffer) + 1;
   trnmsize = send(fdc2, buffer, bytes, flags);
   if (trnmsize < 0) {
      error("%s: Cannot send data to the client.\n",argv[0]);
      exit(5);
      }  
   rcvsize = recv( fdc2, buffer, bytes, flags );
   if (rcvsize < 0) {
      error("%s: Cannot receive from the socket\n",argv[0]);
      exit(6);
      }
  /* printf("%i bytes have been received from client02.\n Message: %s \n",
           rcvsize-1, buffer);*/
   

    puts("Jatekosok csatlakoztak");
    sprintf(buffer, "Jatszmak szama: %d", jatszma);
    send_msg(fdc1, buffer);
    send_msg(fdc2, buffer);
    
    int i = 0;
      while(i <= jatszma){
        

        if ( i == jatszma){ //#1
            printf("A jatek veget ert!\n");
            send_msg(fdc1, "vege");
            send_msg(fdc2, "vege");

            if( pont1 > pont2 ){ //#2
                send_msg(fdc1, "A jatek veget ert! Nyertel!");
                send_msg(fdc2, "A jatek veget ert! Vesztettel!");

                sprintf(buffer, "Vegeredmeny: ( %d ) - ( %d )", pont1, pont2);

                send_msg(fdc1, buffer);
                send_msg(fdc2, buffer);

            }
            else if( pont2 > pont1 ){
                send_msg(fdc2, "A jatek veget ert! Nyertel!");
                send_msg(fdc1, "A jatek veget ert! Vesztettel!");

                sprintf(buffer, "Vegeredmeny: ( %d ) - ( %d )", pont1, pont2);

                send_msg(fdc1, buffer);
                send_msg(fdc2, buffer);
            }
            else if( pont1 == pont2 ){
                send_msg(fdc1, "A jatek veget ert! Dontetlen!");
                send_msg(fdc2, "A jatek veget ert! Dontetlen!");

                sprintf(buffer, "Vegeredmeny: ( %d ) - ( %d )", pont1, pont2);

                send_msg(fdc1, buffer);
                send_msg(fdc2, buffer);

            }


            printf("\n\nVEGEREDMENY: \n");
            printAllas();
            sleep(2);
            break;
        }
        else if(felad1 == 1){ //#4
          
            send_msg(fdc1, "Feladtad a jatekot!");
            send_msg(fdc2, "Az ellenfel feladta!");

            

          /*  sprintf(buffer, "Vegeredmeny: ( %d ) - ( %d )", pont1, pont2);
            printf("BUFFER: \n", buffer);

            send_msg(fdc1, buffer);
            send_msg(fdc2, buffer);*/
            printf("\n\nVEGEREDMENY: \n");
            printAllas();
            break;

        }
        else if(felad2 == 1){ //#4
            
            send_msg(fdc1, "Az ellenfel feladta!");
            send_msg(fdc2, "Feladtad a jatekot!");

          

           /* sprintf(buffer, "Vegeredmeny: ( %d ) - ( %d )", pont1, pont2);
            printf("BUFFER: \n", buffer);

            send_msg(fdc1, buffer);
            send_msg(fdc2, buffer);*/
            printf("\n\nVEGEREDMENY: \n");
            printAllas();
            break;
        }


        else{ //#6

            send_msg(fdc1, kerdes[i]);
            send_msg(fdc2, kerdes[i]);

            recive_msg(fdc1);
            recive_msg(fdc2);

            
        }



        if( felad1 == 0 && felad2 == 0 && 
            abs(ertek1 - valasz[i]) < abs(ertek2 - valasz[i]) ) // 1. játékos nyert
        {
            pont1+=1;
            printf("1. jatekos pontot szerzett!\n");

            send_msg(fdc1, "Nyertel!");
            send_msg(fdc2, "Vesztettel!");

            printAllas();

        }
        else if( felad1 == 0 && felad2 == 0 && 
                 abs(ertek1 - valasz[i]) == abs(ertek2 - valasz[i]) ) // Döntetlen 
        {
            printf("Döntetlen!\n");

            send_msg(fdc1, "Dontetlen!");
            send_msg(fdc2, "Dontetlen!");
        }

        else if( felad1 == 0 && felad2 == 0 &&  
                 abs(ertek2 - valasz[i] < abs(ertek1 - valasz[i]) )) // 2. játékos nyert
        {
            pont2+=1;
            printf("2. jatekos pontot szerzett!\n");

            send_msg(fdc2, "Nyertel!");
            send_msg(fdc1, "Vesztettel!");

            printAllas();

        }


        
        
        i++;
      } //jatek while end


    
    close(fdc1);
    close(fdc2);
    close(fd);
    return 0;
}

int send_msg(int client_number, char * msg){

int flags = 0;
        if(client_number == fdc1){
            bytes=strlen(msg)+1;
            strcpy(buffer,msg);
            err = send( fdc1, buffer, 1024, flags );
            if ( err < 0 )
            {
                printf("Adat kuldese a client1-re sikertelen\n");
                exit(3);
            }
            //printf("Server -> Client_1: %s\n", msg);


        }

        else{
            bytes=strlen(msg)+1;
            strcpy(buffer,msg);
            err = send( fdc2, buffer, 1024, flags );
            if ( err < 0 )
            {
                printf("Adat kuldese a client2-re sikertelen\n");
                exit(3);
            }
            //printf("Server -> Client_2: %s\n", msg);
        }
        return 1;
    }

char* recive_msg(int client_number){

    int flags = 0;
    user_buffer=(char*)calloc(1024,sizeof(char));
    if(client_number == fdc1){
        err=recv(fdc1, buffer, 1024, flags);
        if(err<0){
            printf("No message arrived from client1\n");
        exit(4);
    }
        //strcpy(user_buffer1, buffer);
        strcpy(user_buffer, buffer);

        if ( strcmp(user_buffer, "feladom") == 0){
            felad1 = 1;
            printf("Az elso jatekos feladta!");
        }

        else{
            ertek1 = atoi(user_buffer);
            //printf("Client1 erteke: %d\n", ertek1);

            //printf("Client1: %s\n", user_buffer);
            //puts(user_buffer);
        }

    }else{
            err=recv(fdc2, buffer, 1024, flags);
        if(err<0){
             printf("No message arrived from client2\n");
            exit(4);
        }
        //strcpy(user_buffer2, buffer);
        strcpy(user_buffer, buffer);


        if ( strcmp(user_buffer, "feladom") == 0){
            felad2 = 1;
            printf("A masodik jatekos feladta!");
        }
        else{

            ertek2 = atoi(user_buffer);
            //printf("Client2 erteke: %d\n", ertek2);
            //printf("Client2: %s\n", user_buffer);
            //puts(user_buffer);
        }
    }


    return user_buffer;
}

void printAllas(){

    printf("	Jatek allasa: \n");
    printf("    	1. jatekos: (%d)\n", pont1);
    printf("    	2. jatekos: (%d)\n", pont2);
    printf("\n\n");


}