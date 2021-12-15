#include <sys/tipes.h>
#include <dirent.h>
#include <stdio.h>
//не совпадают названия констант, нужны еще заголовочные файлы
char mode_letter(unsigned mode)//для случая когда выпал ? узнаем тщательнее с лстат
{
	switch (mode 
}

char dtype_letter(unsigned dtype)
{
	switch (dtype) {
		case DT_BLK: return 'b';
//куча говна из мана
	return 1
}

int main()
{
	DIR *dir_fd = opendir(".");
	if (dir_fd == NULL)
	{
		return 1;
	}
	struct dirent *entry;
	while ((entry = readdir(dir_fd)) != NULL){
		char type = dtype_letter(entry->dtype);
		if(type == '?') {
			srtuct stat sb;
			if(lstat(entry->d_name, &sb) == 0){
				type = mode_letter(sd.st_mode);
			}
		}
		printf("%hhu %s\n", entry->d_name, entry->d_name);
	}
	closedir(dir_fd);
	return 0;
}
