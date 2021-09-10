#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <errno.h>

#include <fcntl.h>

#include <unistd.h>

int main(int argc, char *argv[]){
	
	if (argc != 2) {
		perror("input");
		printf("Error with input");
		return 1;
        }

	struct stat sb;
	
	if (lstat(argv[1], &sb) == -1){
		perror("lstat");
		printf("Error with file name");
		return 2;
	}

	printf("File: %s\n", argv[1]);
	printf("I-node number: %ld\n", (long) sb.st_ino);
	printf("Mode: %lo (octal)\n", (unsigned long) sb.st_mode);
	printf("Link count: %ld\n", (long) sb.st_nlink);
        printf("Ownership: UID=%ld   GID=%ld\n", (long) sb.st_uid, (long) sb.st_gid);
	printf("Preferred I/O block size: %ld bytes\n", (long) sb.st_blksize);
        printf("File size %lld bytes\n", (long long) sb.st_size);
        printf("Blocks allocated: %lld\n", (long long) sb.st_blocks);
	printf("Last status change: %s", ctime(&sb.st_ctime));
        printf("Last file access: %s", ctime(&sb.st_atime));
        printf("Last file modification: %s", ctime(&sb.st_mtime));
	
	return 0;

}



