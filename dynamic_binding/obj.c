#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obj.h"

class_t *class_create(class_t *super, const char *name, size_t sz) {
    class_t *res = (class_t *) calloc(1, sizeof(class_t));
    res->super = super;
    res->name = name;
    res->sz = sz;
    res->methods = (method_t*) calloc(CLASS_MAX_METHODS, sizeof(method_t));

    return res;
}

void class_destroy(class_t *cls) {
    for (int i = 0; i < CLASS_MAX_METHODS; i++) {
        free(cls->methods[i].sel);
    }

    free(cls->methods);
    free(cls);
}

int class_addMethod(class_t *cls, const char *selector, method_impl_t impl) {
    if (cls->method_cnt == CLASS_MAX_METHODS) {
        printf("Max methods count reached\n");
        return 1;
    }

    cls->methods[cls->method_cnt].impl = impl;
    cls->methods[cls->method_cnt].sel = strdup(selector);
    cls->method_cnt++;

    return 0;
}


static method_impl_t class_method_lookup(class_t *cls, const char *selector) {
    for (int i = 0; i < cls->method_cnt; i++) {
        if (strcmp(selector, cls->methods[i].sel) == 0)
            return cls->methods[i].impl;
    }

    return NULL;
}

static void obj_recursive_init(obj_t *obj, class_t *cls) {
    if (!cls) return;

    obj_recursive_init(obj, cls->super);
    method_impl_t m = class_method_lookup(cls, OBJ_INIT_METHOD);
    if (m) ((initor_t)m)(obj);

}

obj_t *obj_create(class_t *cls) {
    obj_t *res = (obj_t*) calloc(1, cls->sz);
    res->isa = cls;

    obj_recursive_init(res, cls);

    return res;
}

method_impl_t obj_lookup(obj_t *obj, const char *selector) {
    class_t *cur_class = obj->isa;

    while (cur_class) {
        method_impl_t m = class_method_lookup(cur_class, selector);
        if (m) return m;

        cur_class = cur_class->super;
    }

    printf("Method %s for class %s not found\n", selector, obj->isa->name);
    return NULL;
}

void obj_destroy(obj_t *obj) {
    class_t *cur_class = obj->isa;
    while (cur_class) {
        method_impl_t m = class_method_lookup(cur_class, OBJ_DESTROY_METHOD);
        if (m) {
            ((dtor_t)m)(obj);
        }

        cur_class = cur_class->super;
    }

    free(obj);
}

typedef void (*func_0ptr)(obj_t *obj);
typedef void (*func_1ptr)(obj_t *obj, void *arg);
void obj_invoke_0ptr(obj_t *obj, const char *selector) {
    ((func_0ptr)obj_lookup(obj, selector))(obj);
}

void obj_invoke_1ptr(obj_t *obj, const char *selector, void *arg) {
    ((func_1ptr)obj_lookup(obj, selector))(obj, arg);
}

