//man statvfs - function returns a "generic superblock" describing a file system

#include <stdio.h>
#include <sys/statvfs.h>
#include <sys/types.h>

void info_sys(struct statvfs * sb){
    printf("Total:                  %lu\n", sb->f_blocks * sb->f_bsize);
    printf("Free:                   %lu\n", sb->f_bfree * sb->f_bsize);
    printf("Free for unpriv users:  %lu\n", sb->f_bavail * sb->f_bsize);
    printf("Used:                   %lu\n", sb->f_blocks * sb->f_bsize - sb->f_bavail * sb->f_bsize);
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s pathname\n", argv[0]);
        return 1;
    }
    struct statvfs buf;
    if(statvfs(argv[1], &buf) == -1) {
        perror("Failure in statvfs");
        return 2;
    }
    info_sys(&buf);
    return 0;
}
