#ifndef FRIENDREQUEST_H
#define FRIENDREQUEST_H
void requestToFriend(char *message, int socket_fd);
void processResponse(char *message, int socket_fd);
#endif
