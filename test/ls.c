#include <stdio.h>
#include <dirent.h>
  #include <time.h>
#include <sys/stat.h>  // stat permite ver el tipo de archivo
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#define RESET "\033[0m"
#define BLUE  "\033[34m"//blue //solo para cambiar color
#define GREEN  "\033[32m"//green

int ordenaAscendente(const struct dirent **a, const struct dirent **b) {
    return - alphasort(a, b);
}

int main (int argc, char *v[]) {
    struct dirent *pDirent;
    struct stat fileStat;
struct passwd  *pwd;
struct group   *grp;
	struct timespec st_atim;  /* Time of last access */
               struct timespec st_mtim;  /* Time of last modification */
               struct timespec st_ctim;  /* Time of last status change */
    DIR *pDir;

    struct dirent **namelist;
	int n;

    if (argc < 2) {
        v[1] = ".";
		n = scandir(".", &namelist, NULL, ordenaAscendente);

    if (n == -1) {
        printf ("Cannot open directory '%s'\n", v[1]);
        return 1;
    }

    while (n--) {
	if(strcmp(namelist[n]->d_name,".")==0//para que no acepte ni puntos ni dos puntos
	||strcmp(namelist[n]->d_name,"..")==0
	)continue;

	

	



    stat(namelist[n]->d_name, &fileStat);//Obtiene informacion del archivo
            printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
	    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
	    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
	    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
	    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
	    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
	    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
	    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
	    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");

 printf( " %d",fileStat.st_nlink);/*PARA SACAR EL NUMERO */

/* Print out owner's name if it is found using getpwuid(). PARA SACAR EL USUARIO */
    if ((pwd = getpwuid(fileStat.st_uid)) != NULL)
        printf(" %-8.8s", pwd->pw_name);
    else
        printf(" %-8d", fileStat.st_uid);


    /* Print out group name if it is found using getgrgid(). PARA SACAR EL GRUPO*/
    if ((grp = getgrgid(fileStat.st_gid)) != NULL)
        printf(" %-8.8s", grp->gr_name);
    else
        printf(" %-8d", fileStat.st_gid);



	   

           printf( " %5ld",fileStat.st_size);/*PARA SACAR EL TAMAÑO*/

		 	char mtime[80];/*PARA SACAR LA FECHA*/
	time_t t = fileStat.st_mtime; /*st_mtime is type time_t */
	struct tm lt;
	localtime_r(&t, &lt); /* convert to struct tm */
	strftime(mtime, sizeof mtime, " %b %d %Y %H:%M", &lt);
	printf(" %s ", mtime);

   




         if(S_ISDIR(fileStat.st_mode))
        printf(BLUE "%s dir" RESET, namelist[n]->d_name);
    else
        if(fileStat.st_mode & S_IXUSR)
            printf(GREEN "%s " RESET, namelist[n]->d_name);
        else
                printf ("%s  ", namelist[n]->d_name);
  
 printf("\n");
    }

}
    closedir (pDir);
    return 0;
}
