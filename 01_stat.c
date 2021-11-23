/*stat возвращает информацию о файле file_name и заполняет буфер buf.
lstat идентична stat, но в случае символьных сылок она возвращает информацию
о самой ссылке, а не о файле, на который она указывает.
fstat идентична stat, только возвращается информация об открытом файле,
на который указывает filedes (возвращаемый open(2)), а не о file_name.

*/

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

char * file_type_name(int mode) {
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

char file_type_let(int mode) {
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

	char prava[10];
	prava[0] = file_type_let(sb.st_mode);
	for(int i = 0; i < 9; i++)
	{
		prava[9 - i] = ((sb.st_mode & (1 << i))? "xwr"[i%3] : '-');
	}

	//printf("(%lo/%.10s)\n", (unsigned long) sb.st_mode, prava);

	printf("File:                     %s\n", argv[1]);
	printf("Size:                     %lld bytes\n", (long long) sb.st_size);
	printf("Blocks:                   %lld\n", (long long) sb.st_blocks);
	printf("IO Block:                 %ld bytes\n", (long) sb.st_blksize);
	printf("File type:                %s\n", file_type_name(sb.st_mode));
	printf("Device:                %lxh/%ldd\n", (long) sb.st_dev, (long) sb.st_dev);
	printf("Inode:                    %ld\n", (long) sb.st_ino);
	printf("Links:                    %ld\n", (long) sb.st_nlink);
	printf("Access:                   (%04lo/%.10s)  Uid:%ld  Gid:%ld  \n", (unsigned long) sb.st_mode & ALLPERMS, prava, (long) sb.st_uid, (long) sb.st_gid);
	printf("Access:                   %s", ctime(&sb.st_atime));
	printf("Modified:                 %s", ctime(&sb.st_mtime));
	printf("Change:                   %s", ctime(&sb.st_ctime));

	return 0;

}
