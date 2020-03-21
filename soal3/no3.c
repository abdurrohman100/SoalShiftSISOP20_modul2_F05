#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include<dirent.h>
#include <string.h>
#include <sys/stat.h>



int main() {
  pid_t child_idA;
  pid_t child_idB;
  int status;
  child_idA = fork();
  child_idB = fork();
  //dihasilkan 4 individu
  //parent utama cd
  //p punya anak px
  //p punya anak lagi, py punya anak lagi pxy
  if(child_idA>0 && child_idB==0){
    char *argv[] = {"unzip", "/home/seijaku/Documents/Sisop/SoalShiftSISOP20_modul2_F05/soal3/jpg.zip", "-d", "/home/seijaku/Documents/Sisop/SoalShiftSISOP20_modul2_F05/soal3", NULL};
    execv("/usr/bin/unzip", argv);

  }
  else if(child_idA==0 && child_idB>0){
    // px punya anak namanya pxapwd
    pid_t childmkdir= fork();
    //pxa exec
    if (childmkdir == 0) {
      
      char *argv[] = {"mkdir", "-p", "/home/seijaku/Documents/Sisop/SoalShiftSISOP20_modul2_F05/soal3/indomie" , NULL};
      execv("/bin/mkdir", argv);

    }
    //px exec
    else {
      while ((wait(&status)) > 0);
      sleep(5);  
      char *argv[] = {"mkdir", "-p" , "/home/seijaku/Documents/Sisop/SoalShiftSISOP20_modul2_F05/soal3/sedaaap" , NULL};
      execv("/bin/mkdir", argv);
    }
  }
  sleep(30);

    DIR *ptrDir;
    struct dirent *dir;
    struct stat pathStat;
    char sourcePath[1000000];
    char pwd[10000]="/home/seijaku/Documents/Sisop/SoalShiftSISOP20_modul2_F05/soal3/jpg";
    chdir(pwd);
    ptrDir= opendir(".");
    while ((dir = readdir(ptrDir))!=NULL)
    {
        printf("-----%s\n",dir->d_name);
        /* code */
        if(strcmp(dir->d_name,".")==0 || strcmp(dir->d_name,"..")==0 || dir->d_name[0]=='.' ){
            continue;
        }
        else{
            stat(dir->d_name,&pathStat);
            
            sprintf(sourcePath,"%s/%s", pwd,dir->d_name);
            if(S_ISDIR(pathStat.st_mode)){
                pid_t apa;
                apa=fork();
                if(apa==0){
                  char* argmv[] = {"mv", sourcePath,"/home/seijaku/Documents/Sisop/SoalShiftSISOP20_modul2_F05/soal3/indomie/", NULL};
                execv("/bin/mv", argmv);
                }
                
            }
            printf("%s\n",sourcePath);

        }
    }
    while ((dir = readdir(ptrDir))!=NULL)
    {
        printf("-----%s\n",dir->d_name);
        /* code */
        if(strcmp(dir->d_name,".")==0 || strcmp(dir->d_name,"..")==0 || dir->d_name[0]=='.' ){
            continue;
        }
            stat(dir->d_name,&pathStat);
            
            sprintf(sourcePath,"%s/%s", pwd,dir->d_name);
                pid_t apa;
                apa=fork();
                if(apa==0){
                  char* argmv[] = {"mv", sourcePath,"/home/seijaku/Documents/Sisop/SoalShiftSISOP20_modul2_F05/soal3/indomie/", NULL};
                execv("/bin/mv", argmv);
            printf("%s\n",sourcePath);

        }
    }
    
}