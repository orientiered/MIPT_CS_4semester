#pragma once

#include <map>
#include <string>
#include <ctype.h>

struct NyObj;

struct NyType {
    char *name;
    int size;
    NyObj* (*alloc)(int size);
    void (*dealloc)(NyObj* obj);
};

struct NyObj {
    int refcount;
    NyType type;
};

struct NyObjVar {
    NyObj o;
    int varsize;
};


struct NyInt {
    NyObjVar obj;
    int value;
};


struct NyRuntime {
    std::map<std::string, NyObj*> obj_storage;
};


const char delims[] = {'=', '+', '-', '*', '/', ';'};
const size_t delims_size = 6;

inline bool is_delim(char c) {
    for (int i = 0; i < delims_size; i++) {
        if (c == delims[i]) return true;
    }
    return false;
}

void runtime_exec(NyRuntime *runtime, const char *prog) {
    char name_buf[256];
    char *name_ptr = name_buf;

    const char *prog_ptr = prog;
    while (*prog_ptr) {
        if (isspace(*prog_ptr)) {
            prog_ptr++;
        } else if (is_delim(*prog_ptr)) {

        } else {
            *name_ptr = *prog_ptr;
            *(++name_ptr) = 0;
        }
    }
}
/*
#pragma once

#include <stddef.h>
struct class_t;

// typedef void (*method_impl_t)();
typedef void *method_impl_t;
typedef struct method {
    char *sel;
    method_impl_t impl;
} method_t;


const size_t CLASS_MAX_METHODS = 10;
const char * const OBJ_DESTROY_METHOD = "destroy";
const char * const OBJ_INIT_METHOD = "init";

typedef struct class_t {
    struct class_t *super;
    const char *name;
    method_t *methods;
    size_t method_cnt;
    size_t sz;
} class_t;

typedef struct obj {
    class_t *isa;
} obj_t;

typedef void (*dtor_t)(obj_t *);
typedef void (*initor_t)(obj_t *);

class_t *class_create(class_t *super, const char *name, size_t sz);
int class_addMethod(class_t *cls, const char *selector, method_impl_t impl);

#define CLASS_ADD_METHOD(cls, selector, impl) class_addMethod(cls, selector, (method_impl_t) impl )
#define CLASS_ADD_DTOR(cls, impl) CLASS_ADD_METHOD(cls, OBJ_DESTROY_METHOD, impl)
#define CLASS_ADD_INITOR(cls, impl) CLASS_ADD_METHOD(cls, OBJ_INIT_METHOD, impl)

void class_destroy(class_t *cls);

obj_t *obj_create(class_t *cls);
method_impl_t obj_lookup(obj_t *obj, const char *selector);

void obj_invoke_0ptr(obj_t *obj, const char *selector);
void obj_invoke_1ptr(obj_t *obj, const char *selector, void *arg);

void obj_destroy(obj_t *obj);


*/
