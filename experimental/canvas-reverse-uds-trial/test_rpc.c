static char test_socket_path[108];
#define RPC_SOCKET test_socket_path
#define PROBE_TEST
#include "motion_probe.c"
#include <assert.h>
#include <pthread.h>

struct peer {int fd;const char *reply;};
static void *respond(void *value) {
    struct peer *p=value;int fd=accept(p->fd,NULL,NULL);assert(fd>=0);
    char request[1024];size_t n=0;
    do {ssize_t got=read(fd,request+n,sizeof(request)-n-1);assert(got>0);n+=(size_t)got;assert(n<sizeof(request)-1);}while(!memchr(request,3,n));
    request[n]=0;assert(strstr(request,"\"id\":8100"));assert(!strstr(request,"gcode/script"));
    size_t length=strlen(p->reply),sent=0;
    while(sent<length){size_t chunk=length-sent>7?7:length-sent;ssize_t written=write(fd,p->reply+sent,chunk);if(written<=0)break;sent+=(size_t)written;}
    close(fd);return NULL;
}
static void test(const char *reply,int expected) {
    int fd=socket(AF_UNIX,SOCK_STREAM,0);assert(fd>=0);
    struct sockaddr_un a;memset(&a,0,sizeof(a));a.sun_family=AF_UNIX;strcpy(a.sun_path,test_socket_path);
    assert(bind(fd,(struct sockaddr*)&a,sizeof(a))==0);assert(listen(fd,1)==0);
    struct peer p={fd,reply};pthread_t thread;assert(pthread_create(&thread,NULL,respond,&p)==0);
    char output[16384];int result=rpc("{\"id\":8100,\"method\":\"info\",\"params\":{}}\003",8100,1,output,sizeof(output),0);
    assert(result==expected);assert(pthread_join(thread,NULL)==0);close(fd);assert(unlink(test_socket_path)==0);
}
int main(void) {
    signal(SIGPIPE,SIG_IGN);
    char folder[]="/tmp/cc2-reverse-rpc-XXXXXX";assert(mkdtemp(folder));
    assert(snprintf(test_socket_path,sizeof(test_socket_path),"%s/uds",folder)<(int)sizeof(test_socket_path));
    test("{\"id\":0,\"result\":{}}\003{\"id\":8100,\"result\":{\"state\":\"ready\"}}\003",1);
    test("{\"id\":8100,\"error\":{\"message\":\"refused\"}}\003",0);
    test("{\"id\":0,\"result\":{}}\003",0);
    test("{\"id\":8100,\"result\":{}",0);
    test("{\"id\":8100,\"result\":{},\"error\":{}}\003",0);
    assert(!preflight());interrupt_requested(SIGINT);assert(interrupted);
    assert(rmdir(folder)==0);
    puts("PASS: real socket RPC fragmentation, unsolicited frames, failure and incomplete replies");return 0;
}
