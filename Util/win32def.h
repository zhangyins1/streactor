#ifndef    _WIN32DEF_H
#define    _WIN32DEF_H    1

#include "TypeDef.h"

#ifdef ST_WIN32

#pragma warning (disable:4786)
#pragma warning (disable:4503)
#pragma warning (disable:4819)    
#pragma warning (disable:4996)

#include <stdint.h>
#include <regex>
#include "in.h"

#define SIGPIPE 0
#define SIGUSR1 1
#define SIGUSR2 2
#define SIGALRM 0
#define SIG_BLOCK 0
#define SIG_UNBLOCK 0
#define SOL_SOCKET 0
#define SO_ERROR 0
#define F_SETFL 0
#define F_GETFL 0
#define O_NONBLOCK 1
#define PF_INET 0
#define SOCK_STREAM 0
#define SOCK_DGRAM 0
#define AF_INET 0
#define SO_REUSEADDR 0
#define SO_RCVBUF 0
#define SO_SNDBUF 0
#define CURLSHOPT_SHARE 0
#define CURL_LOCK_DATA_DNS 0
#define CURLOPT_SHARE 0
#define CURLOPT_DNS_CACHE_TIMEOUT 0
#define CURL_GLOBAL_ALL 0
#define CURLOPT_URL 0
#define CURLOPT_TCP_KEEPALIVE 0
#define CURLOPT_TCP_KEEPIDLE 0
#define CURLOPT_TCP_KEEPINTVL 0
#define CURLOPT_WRITEDATA 0
#define CURLOPT_WRITEFUNCTION 0
#define CURLOPT_SSL_VERIFYPEER 0
#define CURLOPT_SSL_VERIFYHOST 0
#define CURLOPT_NOSIGNAL 0
#define CURLOPT_DNS_USE_GLOBAL_CACHE 0
#define CURLOPT_POSTFIELDS 0
#define CURLE_OK 0
#define CLOCK_MONOTONIC 0
#define CURLFORM_COPYNAME 0
#define CURLFORM_FILE 0
#define CURLFORM_END 0
#define CURLFORM_COPYCONTENTS 0
#define CURLOPT_HTTPPOST 0

#define atoll _atoi64
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define strcasecmp(a,b) strcmp(a,b)
#define strncasecmp strnicmp
#define strcasestr strstr
#define close closesocket
#define shutdown(fd)

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef int pthread_mutex_t;
typedef int pthread_t;
typedef int sigset_t;
typedef int socklen_t;
typedef int CURL;
typedef int CURLSH;
typedef int CURLcode;
typedef int pthread_key_t;
typedef long ssize_t;

struct timeval {
    time_t tv_sec;
    int tv_usec;
};

struct timespec
{
    time_t tv_sec;
    long tv_nsec;
};

struct hostent{
    char * h_name;
    char ** h_aliases;
    short h_addrtype;
    short h_length;
    char ** h_addr_list;
};

extern void sleep(int sec);
extern void close(int fd);
extern int gettimeofday(struct timeval *, int);
extern bool pthread_mutex_init(pthread_mutex_t*, int);
extern void pthread_mutex_destroy(pthread_mutex_t*);
extern void pthread_mutex_lock(pthread_mutex_t*);
extern void pthread_mutex_unlock(pthread_mutex_t*);
extern void vasprintf(char **, const char*, char*);
extern void sigemptyset(sigset_t *);
extern void sigaddset(sigset_t *, int);
extern void sigprocmask(int, sigset_t *, int);
extern int getpid();
extern int accept(int, struct sockaddr *, socklen_t*);
extern char* inet_ntoa(struct in_addr &);
extern uint16_t ntohs(unsigned short);
extern int getsockopt(int, int, int, int*, int*);
extern int recv(int sockfd, void *buff, size_t nbytes, int flags);
extern int sendto(int s, const void*, int len, unsigned int flags, const struct sockaddr* to, int tolen);
extern int send(int sockfd, const void *buff, size_t nbytes, int flags);
extern int fcntl(int, int);
extern int fcntl(int, int, long);
extern int htons(int);
extern unsigned int inet_addr(const char * ip);
extern int socket(int, int, int);
extern void setsockopt(int, int, int, const int*, int);
extern int bind(int, struct sockaddr*, int);
extern int listen(int, int);
extern int connect(int, sockaddr*, int);
extern CURLSH* curl_share_init();
extern void curl_share_setopt(CURLSH*, int, int);
extern void curl_global_init(int);
extern CURL* curl_easy_init();
extern void curl_easy_setopt(CURL*, int, const void *);
extern void curl_easy_setopt(CURL*, int, int);
extern int curl_easy_perform(int *);
extern void curl_easy_cleanup(int *);
extern void curl_global_cleanup();
extern int curl_formadd(struct curl_httppost ** firstitem, struct curl_httppost ** lastitem, ...);
extern struct curl_slist *curl_slist_append(struct curl_slist * list, const char * string);
extern void curl_formfree(struct curl_httppost * form);
extern void curl_slist_free_all(struct curl_slist * list);
extern const char *curl_easy_strerror(CURLcode errornum);
extern void clock_gettime(int, timespec*);
extern int pthread_self();
extern void usleep(int); // 微秒
extern int pthread_create(pthread_t*, int, void *, void *);
extern int pthread_join(pthread_t, int);
extern int pthread_key_create(void*, void *);
extern int pthread_setspecific(int, void*);
extern void* pthread_getspecific(int);
extern hostent* gethostbyname(const char*);
extern int write(int, const void*, int);
extern int read(int, void*, int);

/* epoll declarations */
#define EPOLLIN        0x001
#define EPOLLPRI       0x002
#define EPOLLOUT       0x004
#define EPOLLERR       0x008
#define EPOLLHUP       0x010
#define EPOLLRDNORM    0x040
#define EPOLLRDBAND    0x080
#define EPOLLWRNORM    0x100
#define EPOLLWRBAND    0x200
#define EPOLLMSG       0x400

#define EPOLLRDHUP     0x2000

#define EPOLLEXCLUSIVE 0x10000000
#define EPOLLONESHOT   0x40000000
#define EPOLLET        0x80000000

#define EPOLL_CTL_ADD  1
#define EPOLL_CTL_DEL  2
#define EPOLL_CTL_MOD  3

typedef union epoll_data {
	void         *ptr;
	int           fd;
	uint32_t      u32;
	uint64_t      u64;
} epoll_data_t;

struct epoll_event {
	uint32_t      events;
	epoll_data_t  data;
};


int epoll_create(int size);

int epoll_create(int size)
{
	return -1;
}


int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
	return -1;
}


int epoll_wait(int epfd, struct epoll_event *events, int nevents, int timeout);

int epoll_wait(int epfd, struct epoll_event *events, int nevents, int timeout)
{
	return -1;
}

#define SYS_eventfd       323


#define SYS_io_setup      245
#define SYS_io_destroy    246
#define SYS_io_getevents  247

struct io_event {
	uint64_t  data;  /* the data field from the iocb */
	uint64_t  obj;   /* what iocb this event came from */
	int64_t   res;   /* result code for this event */
	int64_t   res2;  /* secondary result */
};

#endif /*_WIN32*/

#endif /* win32def.h  */
