#include <stdio.h>
#include <stdlib.h>
#include "c_array.h"

int main() {
    struct vector v;
    v_init(&v, 4);
    for (int i = 0; i < 100; ++i)
        v_push(&v, &i);

    for (int i = 0; i < v.count; ++i)
        printf("%d\n", *(int*)v_get(&v, i));

    return 0;
}
