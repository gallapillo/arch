#include "sc.h"
#include <stdlib.h>

int sc_icounterGet(int *value) {
    if (value == NULL) {
        return -1;
    }
    *value = icounter;
    return 0;
}