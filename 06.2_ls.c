/*упрощенный ls -la(в т ч скрытые файлы): вывести список записей в текущем
каталоге без рекурсии вывести только тип записи, имя записи*/
// удалить это или разобраться #define _DEFAULT_SOURCE
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>


char dtype_letter(unsigned d_type) {
    switch(d_type) {
        case DT_BLK :  return 'b';
        case DT_CHR :  return 'c';
        case DT_DIR :  return 'd';
        case DT_FIFO : return 'p';
        case DT_LNK :  return 'l';
        case DT_REG :  return '-';
        case DT_SOCK : return 's';
        default :      return '?';
    }
}

char stattype(unsigned mode) {
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

int main(int argc, char const *argv[]) {
    if(argc > 2) {
        fprintf(stderr, "Usage: %s [namedir]\n", argv[0]);
        return 1;
    }
    DIR * dir_fd;
    if(argc == 2 && chdir(argv[1]) == -1) {// у чидира проблемы убрать их нафиг
        perror("Failure in chdir");
        return 2;
    }
    dir_fd = opendir(".");
    if(!dir_fd) {
        perror("Failure in opendir");
        return 2;
    }

    struct dirent * entry;
  	while((entry = readdir(dir_fd)) != NULL) {
        char type = dtype_letter(entry->d_type);
        if(type == '?') {
        		struct stat sb;
        		if(lstat(entry->d_name, &sb) == 0) {
                type = stattype(sb.st_mode);
            }
            else if(lstat(entry->d_name, &sb) == -1) {
                perror ("Failure in lstat");
                return 3;
            }
    		}
    		printf("%c %s\n", type, entry->d_name);
  	}

    if(closedir(dir_fd) == -1) {
        perror("Failure in closedir");
        return 4;
    }
	  return 0;
}
