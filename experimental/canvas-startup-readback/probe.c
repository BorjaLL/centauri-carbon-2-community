#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "../../cc2-control/src/canvas_json.h"

/* One fixed, read-only query. No subscriptions, scripts or control endpoints. */
static const char request[] = "{\"id\":7471,\"method\":\"objects/query\",\"params\":{\"objects\":{\"canvas_dev\":null,\"filament_switch_sensor filament_sensor\":null,\"extruder\":null,\"print_stats\":[\"state\"]}}}\003";
static char frame[65536];
int main(int argc,char **argv) {
    if(argc>2){fputs("Usage: cc2-canvas-startup-readback [socket-path]\n",stderr);return 2;}
    const char *path=argc==2?argv[1]:"/tmp/elegoo_uds";
    struct sockaddr_un address;memset(&address,0,sizeof(address));
    if(strlen(path)>=sizeof(address.sun_path))return 2;
    address.sun_family=AF_UNIX;strcpy(address.sun_path,path);
    signal(SIGPIPE,SIG_IGN);alarm(8);
    int fd=socket(AF_UNIX,SOCK_STREAM,0);
    if(fd<0){perror("socket");return 2;}
    if(connect(fd,(struct sockaddr *)&address,offsetof(struct sockaddr_un,sun_path)+strlen(path)+1)<0){perror("connect");close(fd);return 2;}
    size_t sent=0;
    while(sent<sizeof(request)-1){
        ssize_t n=write(fd,request+sent,sizeof(request)-1-sent);
        if(n<0&&errno==EINTR)continue;
        if(n<=0){close(fd);return 3;}sent+=(size_t)n;
    }
    size_t used=0;unsigned frames=0;
    for(;;){
        struct pollfd p={fd,POLLIN,0};int ready=poll(&p,1,3000);
        if(ready<0&&errno==EINTR)continue;
        if(ready<=0){fputs("No complete matching reply; no action sent.\n",stderr);close(fd);return 4;}
        char buffer[4096];ssize_t count=read(fd,buffer,sizeof(buffer));
        if(count<0&&errno==EINTR)continue;
        if(count<=0){close(fd);return 4;}
        for(ssize_t i=0;i<count;i++){
            if((unsigned char)buffer[i]!=3){if(used+1>=sizeof(frame)){close(fd);return 5;}frame[used++]=buffer[i];continue;}
            frame[used]=0;double id=0;
            const char *end=frame+used;
            if(used&&frame[0]=='{'&&ccv_json_container_end(frame,end)==end&&ccv_numeric(frame,end,"id",&id)&&id==7471){
                const char *object_end=NULL;
                const char *error=ccv_json_member_object(frame,end,"error",'{',&object_end);
                const char *result=ccv_json_member_object(frame,end,"result",'{',&object_end);
                fwrite(frame,1,used,stdout);fputc('\n',stdout);close(fd);return error||!result?6:0;
            }
            used=0;if(++frames>32){close(fd);return 5;}
        }
    }
}
