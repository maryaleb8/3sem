#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

char mode_letter(mode_t st_mode) {
    switch (st_mode & S_IFMT) {
        case S_IFBLK: return 'b';
        case S_IFCHR: return 'c';
        case S_IFDIR: return 'd';
        case S_IFIFO: return 'p';
        case S_IFLNK: return 'l';
        case S_IFREG: return '-';
        case S_IFSOCK: return 's';
    }
    return '?';
}

char dtype_letter(unsigned d_type) {
    switch(d_type) {
        case DT_BLK: return 'b';
        case DT_CHR: return 'c';
        case DT_DIR: return 'd';
        case DT_FIFO: return 'p';
        case DT_LNK: return 'l';
        case DT_REG: return '-';
        case DT_SOCK: return 's';
    }
    return '?';
}

int main(int argc, char* argv[])
{
		if(argc != 2) {
				printf("Usage: %s [namedir]\n", argv[0]);
				return 1;
		}
		DIR *dir_fd = opendir(".");
		if (dir_fd == NULL)
		{
			return 1;
		}
		struct dirent *entry;
		while ((entry = readdir(dir_fd)) != NULL){
				char type = dtype_letter(entry->d_type);
				if(type == '?') {//для случая когда выпал ? узнаем тщательнее с лстат
					struct stat sb;
					if(lstat(entry->d_name, &sb) == 0){
						type = mode_letter(sb.st_mode);
					}
			}
				printf("%s\n", entry->d_name);
		}
    if(closedir(dir_fd) == -1) {
        perror("Failure in closedir");
        return 2;
    }
		return 0;
}
