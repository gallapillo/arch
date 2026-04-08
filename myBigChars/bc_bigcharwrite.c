#include "myBigChars.h"
#include <unistd.h>

int bc_bigcharwrite(int fd, int *big, int count) {
    if (big == NULL || count <= 0) {
        return -1;
    }
    
    ssize_t written = write(fd, big, count * sizeof(int) * 2);
    if (written != count * sizeof(int) * 2) {
        return -1;
    }
    
    return 0;
}