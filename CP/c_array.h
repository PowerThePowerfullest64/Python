#ifndef C_ARRAY
#define C_ARRAY

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct vector {
    void* data;
    size_t element_size;
    size_t count;
    size_t capacity;
};

static inline void v_init(struct vector* v, size_t element_size) {
    v->data = NULL;
    v->element_size = element_size;
    v->count = 0;
    v->capacity = 0;
}

static inline bool v_grow(struct vector* v) {
    if (v->capacity > v->count)
        return true;

    size_t new_cap = v->capacity > 0 ? v->capacity<<1 : 1;

    void* tmp = realloc(v->data, v->element_size * new_cap);
    if (tmp == NULL) {
        return false;
    }

    v->data = tmp;
    v->capacity = new_cap;
    printf("Grew array\n");
    return true;
}

static inline bool v_push(struct vector* v, void* val) {
    if (!v_grow(v))
        return false;
    
    memcpy((char*)(v->data) + v->count * v->element_size, val, v->element_size);
    v->count++;
    return true;
}

static inline void v_pop(struct vector* v) {
    if (v->count > 0)
        v->count--;
}

static inline void v_delete(struct vector* v) {
    free(v->data);
    v->data = NULL;
    v->count = v->capacity = 0; // reset count and cap.
}

static inline void* v_get(struct vector* v, size_t index) {
    return (char*)v->data + index * v->element_size;
}

static inline void* v_at(struct vector* v, size_t index) {
    if (index >= v->count) {
        printf("WARNING: Index out of range.\n");
        return NULL;
    }

    return (char*)v->data + index * v->element_size;
}

static inline bool v_insert(struct vector* v, size_t index, void* val) {
    if (index > v->count)
        return false;
    if (!v_push(v, val))
        return false;

    size_t size_to_move = (v->count - index - 1) * v->element_size;
    memmove((char*)v->data + (index+1) * v->element_size, (char*)v->data + index * v->element_size, size_to_move);
    memcpy((char*)v->data + index * v->element_size, val, v->element_size);

    return true;
}

static inline bool v_remove(struct vector* v, size_t index) {
    if (index >= v->count)
        return false;

    if (index == v->count - 1) {
        v_pop(v);
        return true;
    }

    size_t size_to_move = (v->count - index - 1) * v->element_size;
    memmove((char*)v->data + index * v->element_size, (char*)v->data + (index+1) * v->element_size, size_to_move);
    v_pop(v);

    return true;
}

static inline bool v_reserve(struct vector* v, size_t count) {
    if (count <= v->capacity)
        return false;
    
    void* tmp = realloc(v->data, count * v->element_size);
    if (tmp == NULL)
        return false;

    v->data = tmp;
    v->capacity = count;
    return true;
}

#endif