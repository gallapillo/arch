#include "myBigChars.h"
#include <unistd.h>

int bc_bigcharread(int fd, int *big, int need_count, int *count) {
    if (big == NULL || count == NULL || need_count <= 0) {
        return -1;
    }
    
    ssize_t read_bytes = read(fd, big, need_count * sizeof(int) * 2);
    if (read_bytes <= 0) {
        *count = 0;
        return -1;
    }
    
    *count = read_bytes / (sizeof(int) * 2);
    return 0;
}