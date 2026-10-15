#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
static const char *json_string_end(const char *p,const char *end){
    for(++p;p<end;++p){
        if(*p=='\\'){if(++p>=end)return NULL;}
        else if(*p=='"')return p+1;
    }
    return NULL;
}

static const char *json_container_end(const char *p,const char *end){
    int depth=0;
    while(p<end){
        if(*p=='"'){p=json_string_end(p,end);if(!p)return NULL;continue;}
        if(*p=='{'||*p=='[')depth++;
        else if((*p=='}'||*p==']')&&--depth==0)return p+1;
        p++;
    }
    return NULL;
}

static const char *json_skip_space(const char *p,const char *end){
    while(p<end&&isspace((unsigned char)*p))p++;
    return p;
}

/* Returns the start of the value of a top-level member of the object [obj,end). */
static const char *json_member(const char *obj,const char *end,const char *key){
    size_t key_len=strlen(key);
    const char *p=obj+1;
    while(1){
        p=json_skip_space(p,end);
        if(p<end&&*p==',')p=json_skip_space(p+1,end);
        if(p>=end||*p!='"')return NULL;
        const char *key_end=json_string_end(p,end);if(!key_end)return NULL;
        int match=(size_t)(key_end-p-2)==key_len&&memcmp(p+1,key,key_len)==0;
        p=json_skip_space(key_end,end);
        if(p>=end||*p!=':')return NULL;
        p=json_skip_space(p+1,end);
        if(p>=end)return NULL;
        if(match)return p;
        if(*p=='{'||*p=='[')p=json_container_end(p,end);
        else if(*p=='"')p=json_string_end(p,end);
        else while(p<end&&*p!=','&&*p!='}')p++;
        if(!p)return NULL;
    }
}

static const char *json_member_object(const char *obj,const char *end,const char *key,char open,const char **value_end){
    const char *value=json_member(obj,end,key);
    if(!value||*value!=open)return NULL;
    *value_end=json_container_end(value,end);
    return *value_end?value:NULL;
}

static int numeric(const char *o,const char *e,const char *key,double *out){
 const char *p=json_member(o,e,key);if(!p||(!isdigit((unsigned char)*p)&&*p!='-'))return 0;
 char *q;errno=0;double v=strtod(p,&q);
 if(q==p||q>e||errno||!isfinite(v))return 0;
 q=(char *)json_skip_space(q,e);if(q<e&&*q!=','&&*q!='}')return 0;
 *out=v;return 1;
}
