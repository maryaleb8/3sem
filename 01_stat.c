/*stat возвращает информацию о файле file_name и заполняет буфер buf.
lstat идентична stat, но в случае символьных сылок она возвращает информацию
о самой ссылке, а не о файле, на который она указывает.
fstat идентична stat, только возвращается информация об открытом файле,
на который указывает filedes (возвращаемый open(2)), а не о file_name.*/
// posix stat https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/stat.h.html
// gcc -Werror -Wall -Wextra -Wnarrowing -Wconversion -Wwrite-strings -Wcast-qual -Wundef -Wstrict-prototypes -Wbad-function-cast -Wlogical-op -Wreturn-type -g -O2 -fwhole-program 01_stat.c -o 01_stat.exe
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define S_BLKSIZE 512
/*#ifndef S_BLKSIZE
#include <sys/param.h>
#define S_BLKSIZE DEV_BSIZE
#else
#error "Unknown"
#endif*/

const char * file_type_name(mode_t mode) {
    switch (mode & S_IFMT) {
        case S_IFBLK:  return "block device";
        case S_IFCHR:  return "character device";
        case S_IFDIR:  return "directory";
        case S_IFIFO:  return "FIFO/pipe";
        case S_IFLNK:  return "symbolic link";
        case S_IFREG:  return "regular file";
        case S_IFSOCK: return "socket";
        default:       return "unknown";
    }
}

char file_type_let(mode_t mode) {
    switch (mode & S_IFMT) {
        case S_IFBLK:  return 'b';
        case S_IFCHR:  return 'c';
        case S_IFDIR:  return 'd';
        case S_IFIFO:  return 'p';
        case S_IFLNK:  return 'l';
        case S_IFREG:  return '-';
        case S_IFSOCK: return 's';
        default:       return '?';
    }
}

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

	char prava[sizeof("-rwxrwxrwx")];
	prava[0] = file_type_let(sb.st_mode);
	for(int i = 0; i < 9; i++)
	{
		prava[9 - i] = (((int)sb.st_mode & (1 << i))? "xwr"[i%3] : '-');
	}
	prava[10] = '\0';
	printf("File:                     %s\n", argv[1]);
	printf("Size:                     %ju bytes\n", (uintmax_t) sb.st_size);
	printf("Blocks:                   %ju blocks (%ju bytes)\n", (uintmax_t) sb.st_blocks, (uintmax_t) sb.st_blocks * S_BLKSIZE);
	printf("IO Block:                 %ju bytes\n", (uintmax_t) sb.st_blksize);
	printf("File type:                %s\n", file_type_name(sb.st_mode));
	printf("Device:               	  %lxh/%ldd\n", (long) sb.st_dev, (long) sb.st_dev);
	printf("Inode:                    %ju\n", (uintmax_t) sb.st_ino);
	printf("Links:                    %ju\n", (uintmax_t) sb.st_nlink);
	printf("Access:                   (%04lo/%s)  Uid:%ld  Gid:%ld  \n", (unsigned long) sb.st_mode & ALLPERMS, prava, (long) sb.st_uid, (long) sb.st_gid);
  char str[30];
  str[29] = '\0';
  strftime(str, 100, "%a %b %d %X %Z", localtime(&sb.st_atime));
  printf("Access:                   %s\n",  str);
  strftime(str, 100, "%a %b %d %X %Z", localtime(&sb.st_mtime));
	printf("Modified:                 %s\n",  str);
  strftime(str, 100, "%a %b %d %X %Z", localtime(&sb.st_ctime));
  printf("Change:                   %s\n",  str);

	return 0;

}
