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
