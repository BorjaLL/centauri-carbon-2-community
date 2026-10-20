#define _GNU_SOURCE
#include "recovery_core.h"
#include "sha256_file.h"
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <errno.h>

#define QUALIFIED "ec5633b17de329d5b18c3f8d8de6ebf389391a04193536d976ba167022d07b99"
typedef struct { const char *data; uint32_t length; unsigned char storage[16]; } arm_string;
typedef struct { void *object,*control; } arm_shared;
static pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
static cc2_recovery state;
static void *server;
static void (*native_construct)(arm_shared *,const int *);
static int (*native_broadcast)(void *,const arm_string *);
static int (*native_send)(void *,const arm_string *);

static void write_best_effort(int fd,const char *p,size_t n) {
    while(n) {
        ssize_t written=write(fd,p,n);
        if(written>0){p+=written;n-=(size_t)written;}
        else if(written<0&&errno==EINTR)continue;
        else break;
    }
}
static void refuse(const char *why) {
    write_best_effort(2,"Camera recovery refused: ",25);
    write_best_effort(2,why,strlen(why));write_best_effort(2,"\n",1);_exit(126);
}
static void status_locked(void) {
    char line[400];
    int n=snprintf(line,sizeof(line),"trial=RECOVERY_R1 pid=%ld patched=1 ready=%d epoch=%u announced=%lu suppressed=%lu failed=%lu\n",
                   (long)getpid(),state.ready,state.epoch,state.sent,state.duplicate,state.failed);
    int fd=open("/tmp/cc2-camera-recovery.status",O_WRONLY|O_CREAT|O_TRUNC|O_NOFOLLOW,0600);
    if(fd>=0){if(n>0&&(size_t)n<sizeof(line))write_best_effort(fd,line,(size_t)n);(void)close(fd);}
}
static int socket_key(int fd,cc2_socket_key *key) {
    struct stat st;
    if(fstat(fd,&st)||!S_ISSOCK(st.st_mode))return 0;
    key->device=(uint64_t)st.st_dev;key->inode=(uint64_t)st.st_ino;return key->inode!=0;
}
static void take(void){if(pthread_mutex_lock(&lock))refuse("lock");}
static void release(void){if(pthread_mutex_unlock(&lock))refuse("unlock");}
static int presence(void *object,const arm_string *message,int present) {
    take();
    if(server&&server!=object)refuse("unexpected control server");
    server=object;cc2_recovery_presence(&state,present);status_locked();release();
    return native_broadcast(object,message);
}
static int inserted(void *object,const arm_string *message){return presence(object,message,1);}
static int removed(void *object,const arm_string *message){return presence(object,message,0);}

/* Called before the new shared_ptr is published to ServerSocket. This object
 * and fd are private to the accept thread here. No native server lock is held.
 * The native receive loop starts after this wrapper returns. */
static void accepted(arm_shared *result,const int *fd) {
    native_construct(result,fd);
    if(!result->object||*fd<0)return;
    take();
    cc2_socket_key key;
    if(socket_key(*fd,&key)&&cc2_recovery_claim(&state,key)) {
        ssize_t n=send(*fd,cc2_insertion,cc2_insertion_length,MSG_NOSIGNAL|MSG_DONTWAIT);
        cc2_recovery_result(&state,n==(ssize_t)cc2_insertion_length);
        /* Never leave a partial JSON frame on a live control connection. No
         * retry: normal native disconnect/reconnect owns subsequent recovery. */
        if(n>0&&n!=(ssize_t)cc2_insertion_length)(void)shutdown(*fd,SHUT_RDWR);
    }
    status_locked();release();
}
static int broadcast_client(void *client,const arm_string *message) {
    if(!cc2_recovery_is_insertion(message->data,message->length))return native_send(client,message);
    take();
    int fd;memcpy(&fd,client,sizeof(fd));cc2_socket_key key;
    int result;
    if(!socket_key(fd,&key)) {
        result=native_send(client,message);
        cc2_recovery_result(&state,result);
    } else if(cc2_recovery_claim(&state,key)) {
        result=native_send(client,message);
        cc2_recovery_result(&state,result);
    } else result=1;
    status_locked();release();return result;
}
static uint32_t branch(uintptr_t site,uintptr_t destination) {
    intptr_t d=(intptr_t)destination-(intptr_t)(site+8);
    if((d&3)||d< -33554432||d>33554428)refuse("branch range");
    return 0xeb000000u|((uint32_t)(d/4)&0xffffffu);
}
__attribute__((constructor)) static void activate(void) {
    const char *enable=getenv("CC2_CAMERA_RECOVERY_TRIAL");
    if(!enable||strcmp(enable,"1"))return;
    if(sizeof(void *)!=4||sizeof(arm_string)!=24||sizeof(arm_shared)!=8)refuse("ARM C++ ABI");
    char path[256],hash[65];ssize_t n=readlink("/proc/self/exe",path,sizeof(path)-1);
    if(n<0||n>=(ssize_t)sizeof(path)-1)refuse("path");
    path[n]=0;
    if(strcmp(path,"/opt/bin/ai_camera"))refuse("executable path");
    if(cc2_sha256_file("/proc/self/exe",hash)||strcmp(hash,QUALIFIED))refuse("executable checksum");
    const uintptr_t sites[]={0xea488,0xea570,0x15289c,0x152a1c,0x152a50};
    const uint32_t expected[]={0xeb01a10d,0xeb01a0d3,0xeb0002f6,0xebfffdc7,0xebfffdba};
    uintptr_t a=0x15347c;memcpy(&native_construct,&a,4);
    a=0x1528c4;memcpy(&native_broadcast,&a,4);
    a=0x152140;memcpy(&native_send,&a,4);
    if(*(const uint32_t *)0x1297cc!=0xeb00e6f3u)refuse("community camera fingerprint");
    for(unsigned i=0;i<5;i++)if(*(const uint32_t *)sites[i]!=expected[i])refuse("call fingerprint");
    long size=sysconf(_SC_PAGESIZE);if(size!=4096)refuse("page size");
    unsigned char *tramp=NULL;
    for(uintptr_t hint=0x1000000;hint<=0x2000000;hint+=0x200000) {
        void *p=mmap((void *)hint,(size_t)size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
        if(p==MAP_FAILED)continue;
        intptr_t d=(intptr_t)p-(intptr_t)(sites[0]+8);
        if(!(d&3)&&d>=-33554432&&d<=33554380){tramp=p;break;}
        (void)munmap(p,(size_t)size);
    }
    if(!tramp)refuse("trampoline allocation");
    uint32_t words[10]={0xe51ff004,0,0xe51ff004,0,0xe51ff004,0,0xe51ff004,0,0xe51ff004,0};
    int (*i_fn)(void *,const arm_string *)=inserted;
    int (*r_fn)(void *,const arm_string *)=removed;
    int (*b_fn)(void *,const arm_string *)=broadcast_client;
    void (*a_fn)(arm_shared *,const int *)=accepted;
    memcpy(words+1,&i_fn,4);memcpy(words+3,&r_fn,4);memcpy(words+5,&a_fn,4);
    memcpy(words+7,&b_fn,4);memcpy(words+9,&b_fn,4);
    memcpy(tramp,words,sizeof(words));__builtin___clear_cache((char *)tramp,(char *)tramp+sizeof(words));
    if(mprotect(tramp,(size_t)size,PROT_READ|PROT_EXEC))refuse("trampoline protection");
    for(unsigned i=0;i<5;i++) {
        uintptr_t page=sites[i]&~(uintptr_t)4095;
        uint32_t instruction=branch(sites[i],(uintptr_t)tramp+8*i);
        if(mprotect((void *)page,(size_t)size,PROT_READ|PROT_WRITE|PROT_EXEC))refuse("text protection");
        __atomic_store_n((uint32_t *)sites[i],instruction,__ATOMIC_SEQ_CST);
        __builtin___clear_cache((char *)sites[i],(char *)sites[i]+4);
        if(mprotect((void *)page,(size_t)size,PROT_READ|PROT_EXEC))refuse("text restoration");
    }
    take();status_locked();release();unsetenv("CC2_CAMERA_RECOVERY_TRIAL");
}
