#ifndef __NET_UTIL_H_H__
#define __NET_UTIL_H_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

extern int StSocket(int type);
extern int StBind(int fd, const char *pszAddr, unsigned short unPort);
extern int StListen(int fd, int backlog);
extern bool StSetNonBlock(int fd);
extern void StSetReuseAddr(int sockfd, bool on);
extern void StSetReusePort(int sockfd, bool on);
extern void StSetSocketBuffSize(int fd, int nRcvBuffSize, int nSndBuffSize);
extern int StConnect(int fd, const char *pszAddr, unsigned short port);
extern int StAccept(int fd);
extern void StClose(int fd);

#endif // !__NET_UTIL_H_H__
