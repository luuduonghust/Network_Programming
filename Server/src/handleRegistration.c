#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

char save(char* fullname, char* username, char* passsword);
void handleRegistration(char *message, int *gen_id, int socket_fd){
    int protocolCode;
    char* fullname;
    char* username;
    char* password;

    // sscanf(message, "%d/n%s/n%s/n%s/r/n", &protocolCode, fullname, username, password);
    if (sscanf(message, "%s/n%s/n%s/n%s/r/n", &protocolCode, fullname, username, password) == 4)
    {
       int registrationResult = save(fullname, username, password, gen_id);

       if (registrationResult == 0)
       {
        send(socket_fd, "201", sizeof("201"), 0 )
       }else
       {
        send(socket_fd, "401", sizeof("401"), 0 )
       }
    }
    
}

int save(char *fullname, char *username, char *password, int *gen_id){
    FILE *dbFile = fopen("db.txt", "a");
    if (dbFile == NULL) {
        perror("Error opening database file");
        return -1; 
    }
    //ham ghi
    gen_id++;
    fprintf(dbFile, "%s %s %s %d  1\n", fullname, username, password, gen_id);
    fclose(dbFile);
    return 0; 
}
int check(struct UserInfo user){
    FILE *file = fopen("../database/account.txt", "r"); 
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    struct UserInfo temp;
    while (fscanf(file, "%s %s %s ", temp.fullname, temp.username, temp.password, temp.gen_id) != EOF)
    {
      if (strcmp(user.username, temp.username) == 0)
      {
          fclose(file);
            return 1; 
      }
      
    }
     fclose(file);
      return 0; 

}