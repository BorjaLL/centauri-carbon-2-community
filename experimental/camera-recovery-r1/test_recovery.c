#include "camera_recovery.c"
#include <assert.h>
#include <sys/wait.h>
#ifdef NDEBUG
#error Assertions are required
#endif
static int forwarded;
static int fake_client;
static arm_string event;
static void construct(arm_shared *out,const int *fd) {
    assert(*fd==fake_client);out->object=&fake_client;out->control=out;
}
static int transmit(void *object,const arm_string *message) {
    int fd;memcpy(&fd,object,sizeof(fd));++forwarded;
    return send(fd,message->data,message->length,MSG_NOSIGNAL)==(ssize_t)message->length;
}
static int broadcast(void *object,const arm_string *message) {
    (void)object;return broadcast_client(&fake_client,message);
}
static void expect_frame(int fd) {
    char buf[512];ssize_t n=recv(fd,buf,sizeof(buf),MSG_DONTWAIT);
    assert(n==(ssize_t)cc2_insertion_length&&!memcmp(buf,cc2_insertion,(size_t)n));
}
static void expect_empty(int fd){char c;assert(recv(fd,&c,1,MSG_DONTWAIT)==-1&&(errno==EAGAIN||errno==EWOULDBLOCK));}
static void setup(int pair[2]){
    assert(!socketpair(AF_UNIX,SOCK_STREAM,0,pair));fake_client=pair[0];
    memset(&state,0,sizeof(state));server=NULL;forwarded=0;
    native_construct=construct;native_send=transmit;native_broadcast=broadcast;
    event=(arm_string){cc2_insertion,(uint32_t)cc2_insertion_length,{0}};
}
static void *announce_thread(void *unused){(void)unused;inserted((void *)1,&event);return NULL;}
static void *accept_thread(void *unused){(void)unused;arm_shared out;accepted(&out,&fake_client);return NULL;}
int main(void){
    cc2_recovery s={0};cc2_socket_key a={1,42},b={1,43};
    assert(!cc2_recovery_claim(&s,a));
    cc2_recovery_presence(&s,1);assert(cc2_recovery_claim(&s,a));assert(!cc2_recovery_claim(&s,a));
    assert(cc2_recovery_claim(&s,b));cc2_recovery_presence(&s,0);assert(!cc2_recovery_claim(&s,b));
    cc2_recovery_presence(&s,1);assert(cc2_recovery_claim(&s,a));
    s.epoch=~0u;cc2_recovery_presence(&s,1);assert(s.epoch==1&&cc2_recovery_claim(&s,a));
    assert(cc2_recovery_is_insertion(cc2_insertion,cc2_insertion_length));
    assert(!cc2_recovery_is_insertion(cc2_insertion,cc2_insertion_length-1));
    int p[2];arm_shared out;
    /* Connect before readiness: no fabricated early event. Native event wins. */
    setup(p);accepted(&out,&fake_client);expect_empty(p[1]);inserted((void *)1,&event);expect_frame(p[1]);
    accepted(&out,&fake_client);expect_empty(p[1]);assert(state.sent==1&&forwarded==1);close(p[0]);close(p[1]);
    /* Readiness before connection: one replay; later broadcast is suppressed. */
    setup(p);take();cc2_recovery_presence(&state,1);release();
    accepted(&out,&fake_client);expect_frame(p[1]);broadcast_client(&fake_client,&event);expect_empty(p[1]);
    assert(state.sent==1&&forwarded==0);
    /* Ordinary messages remain unchanged. */
    arm_string ordinary={"ordinary",8,{0}};assert(broadcast_client(&fake_client,&ordinary));
    char buf[8];assert(recv(p[1],buf,8,0)==8&&!memcmp(buf,"ordinary",8));
    /* Same object and reused fd, different socket inode must receive again. */
    int old=p[0];close(p[0]);close(p[1]);assert(!socketpair(AF_UNIX,SOCK_STREAM,0,p));assert(p[0]==old);
    fake_client=p[0];accepted(&out,&fake_client);expect_frame(p[1]);assert(state.sent==2);close(p[0]);close(p[1]);
    /* No retry after failed replay. */
    setup(p);close(p[1]);take();cc2_recovery_presence(&state,1);release();
    accepted(&out,&fake_client);assert(state.failed==1);accepted(&out,&fake_client);assert(state.failed==1);close(p[0]);
    /* Concurrent native insertion and accepted client: exactly one frame. */
    for(unsigned i=0;i<150;i++) {
        setup(p);pthread_t t1,t2;assert(!pthread_create(&t1,NULL,announce_thread,NULL));
        assert(!pthread_create(&t2,NULL,accept_thread,NULL));assert(!pthread_join(t1,NULL));assert(!pthread_join(t2,NULL));
        expect_frame(p[1]);expect_empty(p[1]);assert(state.sent==1);close(p[0]);close(p[1]);
    }
    /* Process and branch guards. */
    for(int mode=0;mode<3;mode++){
        pid_t child=fork();assert(child>=0);
        if(!child){if(mode==0){setenv("CC2_CAMERA_RECOVERY_TRIAL","1",1);activate();}
            else if(mode==1)(void)branch(0x15289c,0x1000001);
            else (void)branch(0x15289c,0x70000000);
            _exit(99);}
        int status;assert(waitpid(child,&status,0)==child);assert(WIFEXITED(status)&&WEXITSTATUS(status)==126);
    }
    puts("PASS: native-event ordering, 150 races, inode/FD reuse, framing, missing readiness, failed-send no retry, ordinary replies and activation guards");
    return 0;
}
