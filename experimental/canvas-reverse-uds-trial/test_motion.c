#include "guards.h"
#include <stdio.h>
#include <assert.h>

static const char *valid="{\"id\":8100,\"result\":{\"status\":{\"canvas_dev\":{\"active_cid\":-1,\"auto_plug_in_enable\":0,\"canvas_lite\":{\"connected\":1,\"channels1\":{\"cid\":1,\"status\":1}}},\"print_stats\":{\"state\":\"standby\"},\"idle_timeout\":{\"state\":\"Ready\"},\"filament_switch_sensor filament_sensor\":{\"filament_detected\":false},\"extruder\":{\"temperature\":25,\"target\":0},\"heater_bed\":{\"temperature\":24,\"target\":0}}}}";
static void reject_replacement(const char *from,const char *to) {
    char changed[2048];const char *p=strstr(valid,from);assert(p);
    size_t n=(size_t)(p-valid);memcpy(changed,valid,n);
    snprintf(changed+n,sizeof(changed)-n,"%s%s",to,p+strlen(from));
    assert(!status_allowed(changed));
}
int main(void) {
    assert(status_allowed(valid));
    reject_replacement("\"active_cid\":-1","\"active_cid\":1");
    reject_replacement("\"auto_plug_in_enable\":0","\"auto_plug_in_enable\":1");
    reject_replacement("\"connected\":1","\"connected\":0");
    reject_replacement("\"cid\":1","\"cid\":2");
    reject_replacement("\"status\":1","\"status\":0");
    reject_replacement("\"status\":1","\"status\":2");
    reject_replacement("\"standby\"","\"printing\"");
    reject_replacement("\"standby\"","\"paused\"");
    reject_replacement("\"Ready\"","\"Printing\"");
    reject_replacement("\"filament_detected\":false","\"filament_detected\":true");
    reject_replacement("\"filament_detected\":false","\"filament_detected\":null");
    reject_replacement("\"temperature\":25","\"temperature\":51");
    reject_replacement("\"temperature\":25","\"temperature\":-1");
    reject_replacement("\"target\":0","\"target\":250");
    assert(!status_allowed("{}"));
    assert(!status_allowed("{\"result\":{\"status\":{}}}"));
    puts("PASS: idle/head/slot/connection/temperature guards");
    return 0;
}
