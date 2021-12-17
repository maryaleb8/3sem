/*упрощенный ls -la(в т ч скрытые файлы): вывести список записей в текущем
каталоге без рекурсии вывести только тип записи, имя записи*/
//разобраться с отступами
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
//редактировать библиотеки, могли удалить файл но мы прочитали название


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
    if(argc > 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

    DIR * dir_fd = opendir(".");
    if(!dir_fd) {
        perror("Failure in opendir");
        return 2;
    }
    int result = 0;

    while(1) {
        int errno = 0;
        struct dirent * entry = readdir(dir_fd);
		    if(entry == NULL) {
            if(errno) {
                perror("readdir");
                result = 3;
            }
            break;
        }
        char type = dtype_letter(entry->d_type);
        if(type == '?') {
        		struct stat sb;
        		if(lstat(entry->d_name, &sb) == 0) {
                type = stattype(sb.st_mode);
            }
            else {
                fprintf(stderr, "Failed to determine %s type: %s", entry->d_name, strerror(errno));
            }
    		}
    		printf("%c %s\n", type, entry->d_name);
  	}

    if(closedir(dir_fd) == -1) {
        perror("Failure in closedir");
        result = 4;
      }
	 return result;
}
