#define _POSIX_C_SOURCE 200809L
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#define SITE ((uintptr_t)0x5f2190)
static const unsigned char before[24]={0xc7,0x6b,0xb4,0xee,0x10,0xfa,0xf1,0xee,0x01,0x30,0xa0,0xc3,0x00,0x30,0xa0,0xd3,0x73,0x30,0xef,0xe6,0x00,0x00,0x53,0xe3};
static const unsigned char after[24]={0xc7,0x7b,0xb0,0xee,0xc7,0x6b,0xb4,0xee,0x10,0xfa,0xf1,0xee,0x01,0x30,0xa0,0xc3,0x00,0x30,0xa0,0xd3,0x00,0x00,0x53,0xe3};
static void refuse(const char *reason) {
    static const char prefix[]="Canvas reverse trial refused: ";
    if(write(STDERR_FILENO,prefix,sizeof(prefix)-1)<0)_exit(126);
    if(write(STDERR_FILENO,reason,strlen(reason))<0)_exit(126);
    if(write(STDERR_FILENO,"\n",1)<0)_exit(126);
    _exit(126);
}
__attribute__((constructor)) static void initialize(void) {
    const char *enabled=getenv("CC2_CANVAS_REVERSE_TRIAL");
    if(!enabled||strcmp(enabled,"1"))return;
    char path[256];ssize_t n=readlink("/proc/self/exe",path,sizeof(path)-1);
    if(n<0)refuse("executable path");
    path[n]=0;
    if(strcmp(path,"/opt/usr/cc2-reactor-runtime-v2/elegoo_printer"))refuse("unqualified executable path");
    if(sizeof(void*)!=4)refuse("ARM32 only");
    if(memcmp((const void*)SITE,before,sizeof(before)))refuse("instruction fingerprint");
    long size=sysconf(_SC_PAGESIZE);
    if(size<=0||((unsigned long)size&((unsigned long)size-1)))refuse("page size");
    uintptr_t page=SITE&~((uintptr_t)size-1);
    if(SITE+sizeof(after)>page+(uintptr_t)size)refuse("page bounds");
    if(mprotect((void*)page,(size_t)size,PROT_READ|PROT_WRITE|PROT_EXEC))refuse("write protection");
    memcpy((void*)SITE,after,sizeof(after));
    __builtin___clear_cache((char*)SITE,(char*)SITE+sizeof(after));
    if(mprotect((void*)page,(size_t)size,PROT_READ|PROT_EXEC))refuse("restore protection");
    if(memcmp((const void*)SITE,after,sizeof(after)))refuse("patch readback");
    int fd=open("/opt/usr/cc2-reactor-runtime-v2/canvas-reverse.marker",O_WRONLY|O_CREAT|O_TRUNC,0600);
    if(fd<0)refuse("marker open");
    char marker[128];int length=snprintf(marker,sizeof(marker),"pid=%ld site=0x5f2190 patched=1\n",(long)getpid());
    if(length<=0||write(fd,marker,(size_t)length)!=length||close(fd))refuse("marker write");
    unsetenv("CC2_CANVAS_REVERSE_TRIAL");
}
