#ifndef MENU_H
#define MENU_H
void handleLogin(char *message, int socket_fd, const struct Session *sessionList);
void handleRegistration(char *message, int *gen_id, int socket_fd);
#endif
