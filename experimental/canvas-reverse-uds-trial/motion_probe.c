#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>
#include "guards.h"

#define BASE "/opt/usr/cc2-reactor-runtime-v2"
#ifndef RPC_SOCKET
#define RPC_SOCKET "/tmp/elegoo_uds"
#endif
static volatile sig_atomic_t interrupted;
static void interrupt_requested(int number) { (void)number;interrupted=1; }
static double monotonic(void) {
    struct timespec t;if(clock_gettime(CLOCK_MONOTONIC,&t))return -1;
    return t.tv_sec+t.tv_nsec/1e9;
}
/* One bounded request per connection. Unsolicited frames never count as ACKs. */
static int rpc(const char *request,int id,double seconds,char *out,size_t cap,int stopping) {
    int fd=socket(AF_UNIX,SOCK_STREAM,0);if(fd<0)return 0;
    struct sockaddr_un a;memset(&a,0,sizeof(a));a.sun_family=AF_UNIX;
    if(strlen(RPC_SOCKET)>=sizeof(a.sun_path)){close(fd);return 0;}
    strcpy(a.sun_path,RPC_SOCKET);
    if(fcntl(fd,F_SETFL,O_NONBLOCK)<0){close(fd);return 0;}
    double now=monotonic();if(now<0){close(fd);return 0;}double deadline=now+seconds;
    if(connect(fd,(struct sockaddr*)&a,sizeof(a))<0&&errno!=EINPROGRESS){close(fd);return 0;}
    size_t sent=0,used=0,total=0;char frame[16384];int success=0;
    while((now=monotonic())>=0&&now<deadline&&(stopping||!interrupted)) {
        struct pollfd p={fd,(short)(sent<strlen(request)?POLLOUT:POLLIN),0};
        int ready=poll(&p,1,100);if(ready<0){if(errno==EINTR)continue;break;}if(!ready)continue;
        if(sent<strlen(request)) {
            int error=0;socklen_t length=sizeof(error);
            if(getsockopt(fd,SOL_SOCKET,SO_ERROR,&error,&length)||error)break;
            ssize_t n=write(fd,request+sent,strlen(request)-sent);
            if(n<0){if(errno==EINTR||errno==EAGAIN)continue;break;}if(!n)break;
            sent+=(size_t)n;continue;
        }
        char block[2048];ssize_t n=read(fd,block,sizeof(block));
        if(n<0){if(errno==EINTR||errno==EAGAIN)continue;break;}if(!n)break;
        total+=(size_t)n;if(total>1024U*1024U)break;
        for(ssize_t i=0;i<n;i++) {
            if(block[i]!=3){if(used+1>=sizeof(frame))goto done;frame[used++]=block[i];continue;}
            frame[used]=0;const char *end=frame+used;double got;
            if(used&&frame[0]=='{'&&json_container_end(frame,end)==end&&numeric(frame,end,"id",&got)&&got==id) {
                if(used>=cap)goto done;
                memcpy(out,frame,used+1);
                success=json_member(frame,end,"result")&&!json_member(frame,end,"error");goto done;
            }
            used=0;
        }
    }
done:
    close(fd);return success;
}
static int activation(void) {
    long pid=0,callback=0;char line[256],path[64],exe[256];FILE *f=fopen(BASE "/canvas-reverse.marker","r");
    if(!f)return 0;
    unsigned long site=0;int patched=0;char extra;
    int ok=fgets(line,sizeof(line),f)&&sscanf(line,"pid=%ld site=%lx patched=%d %c",&pid,&site,&patched,&extra)==3;fclose(f);
    if(!ok||site!=0x5f2190||patched!=1)return 0;
    if(!ok||pid<=1)return 0;
    f=fopen(BASE "/activation.marker","r");if(!f)return 0;
    ok=fgets(line,sizeof(line),f)&&sscanf(line,"pid=%ld",&callback)==1;fclose(f);if(!ok||pid!=callback)return 0;
    snprintf(path,sizeof(path),"/proc/%ld/exe",pid);ssize_t n=readlink(path,exe,sizeof(exe)-1);
    if(n<0)return 0;
    exe[n]=0;if(strcmp(exe,BASE "/elegoo_printer"))return 0;
    snprintf(path,sizeof(path),"/proc/%ld/maps",pid);f=fopen(path,"r");if(!f)return 0;
    int reactor=0,reverse=0;
    while(fgets(line,sizeof(line),f)){
        if(strstr(line,BASE "/libcc2-reactor-v2.so"))reactor=1;
        if(strstr(line,BASE "/libcc2-canvas-reverse-trial.so"))reverse=1;
    }
    fclose(f);return reactor&&reverse;
}
static int preflight(void) {
    if(access("/opt/usr/cc2-canvas-reverse-disabled",F_OK)==0||access("/opt/usr/cc2-reactor-disabled",F_OK)==0||!activation()){fprintf(stderr,"STOP: matching live callback/reverse activation not verified\n");return 0;}
    char out[16384];
    const char *q="{\"id\":8100,\"method\":\"objects/query\",\"params\":{\"objects\":{\"canvas_dev\":null,\"print_stats\":[\"state\"],\"idle_timeout\":[\"state\"],\"filament_switch_sensor filament_sensor\":[\"filament_detected\"],\"extruder\":[\"temperature\",\"target\"],\"heater_bed\":[\"temperature\",\"target\"]}}}\003";
    if(!rpc(q,8100,3,out,sizeof(out),0)||!status_allowed(out)){
        fprintf(stderr,"STOP: need cold/idle printer, targets zero, auto-insertion off, head unloaded and slot 2 present\n");return 0;
    }
    const char *info="{\"id\":8101,\"method\":\"info\",\"params\":{}}\003";
    if(!rpc(info,8101,3,out,sizeof(out),0))return 0;
    object root={out,out+strlen(out)};
    return text(child(root,"result"),"state","ready");
}
#ifndef PROBE_TEST
int main(int argc,char **argv) {
    if(argc!=2||(strcmp(argv[1],"check")&&strcmp(argv[1],"pulse-slot2"))){fprintf(stderr,"Usage: %s check|pulse-slot2\n",argv[0]);return 2;}
    signal(SIGPIPE,SIG_IGN);struct sigaction sa;memset(&sa,0,sizeof(sa));sa.sa_handler=interrupt_requested;
    sigaction(SIGINT,&sa,NULL);sigaction(SIGTERM,&sa,NULL);
    if(!preflight())return 3;
    if(!strcmp(argv[1],"check")){puts("CHECK PASS: no movement sent. Direction and physical stop are not qualified yet.");return 0;}
    if(!isatty(STDIN_FILENO)){fprintf(stderr,"Interactive SSH terminal required\n");return 3;}
    puts("Slot 2 only: 10 mm reverse target, SPEED=5, ACCEL=100, TIMEOUT=3. Observe the feeder.");
    puts("Type SLOT2 to send ONE pulse, or anything else to cancel:");
    char confirm[32];if(!fgets(confirm,sizeof(confirm),stdin)||strcmp(confirm,"SLOT2\n")){puts("Cancelled; no movement sent.");return 0;}
    if(interrupted||!preflight())return 3;
    char out[16384];
    const char *move="{\"id\":8102,\"method\":\"gcode/script\",\"params\":{\"script\":\"CANVAS_MOTOR_CONTROL CHANNEL=1 DISTANCE=-10 SPEED=5 ACCEL=100 TIMEOUT=3\"}}\003";
    const char *stop="{\"id\":8103,\"method\":\"gcode/script\",\"params\":{\"script\":\"CANVAS_MOTOR_CONTROL CHANNEL=1 DISTANCE=0 SPEED=0 ACCEL=100 TIMEOUT=1\"}}\003";
    int moved=rpc(move,8102,5,out,sizeof(out),0);
    /* Stop is attempted even after an error, partial send or Ctrl+C. Never retry movement. */
    int stopped=rpc(stop,8103,3,out,sizeof(out),1);
    printf("Movement reply: %s; stop reply: %s.\n",moved?"acknowledged":"FAILED/CANCELLED",stopped?"acknowledged":"FAILED");
    puts("ACK does not prove physical stop. Report actual direction, measured travel and whether the feeder stopped.");
    return moved&&stopped?0:4;
}
#endif
