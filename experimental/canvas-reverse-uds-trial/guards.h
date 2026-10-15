#include "json_read.h"
typedef struct {const char *p,*e;} object;
static object child(object parent,const char *key) {
    object r={0};if(!parent.p)return r;
    r.p=json_member_object(parent.p,parent.e,key,'{',&r.e);return r;
}
static int number(object o,const char *key,double expected) {
    double v;return o.p&&numeric(o.p,o.e,key,&v)&&v==expected;
}
static int text(object o,const char *key,const char *expected) {
    if(!o.p)return 0;
    const char *p=json_member(o.p,o.e,key);if(!p||*p!='"')return 0;
    const char *e=json_string_end(p,o.e);return e&&(size_t)(e-p-2)==strlen(expected)&&!memcmp(p+1,expected,strlen(expected));
}
static int false_value(object o,const char *key) {
    if(!o.p)return 0;
    const char *p=json_member(o.p,o.e,key);
    return p&&o.e-p>=5&&!memcmp(p,"false",5)&&(p+5==o.e||p[5]==','||p[5]=='}'||isspace((unsigned char)p[5]));
}
static int cold(object o) {
    double v;return o.p&&numeric(o.p,o.e,"temperature",&v)&&v>=0&&v<=50&&number(o,"target",0);
}
static int status_allowed(const char *json) {
    object root={json,json+strlen(json)};object status=child(child(root,"result"),"status");
    object canvas=child(status,"canvas_dev"),slots=child(canvas,"canvas_lite");
    object print=child(status,"print_stats"),idle=child(status,"idle_timeout");
    return number(canvas,"active_cid",-1)&&number(canvas,"auto_plug_in_enable",0)&&number(slots,"connected",1)
        &&number(child(slots,"channels1"),"cid",1)&&number(child(slots,"channels1"),"status",1)
        &&(text(print,"state","standby")||text(print,"state","complete")||text(print,"state","cancelled"))
        &&(text(idle,"state","Idle")||text(idle,"state","Ready"))
        &&false_value(child(status,"filament_switch_sensor filament_sensor"),"filament_detected")
        &&cold(child(status,"extruder"))&&cold(child(status,"heater_bed"));
}
