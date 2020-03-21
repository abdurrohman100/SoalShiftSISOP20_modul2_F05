#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <string.h>

int main() {
  pid_t child_id1;
  int status;
  child_id1=fork();
  char pwd[100]="/home/seijaku/Documents/Sisop/SoalShiftSISOP20_modul2_F05/soal3";
  char sedaaap[100]="/home/seijaku/Documents/Sisop/SoalShiftSISOP20_modul2_F05/soal3/sedaaap";
  char indomie[100]="/home/seijaku/Documents/Sisop/SoalShiftSISOP20_modul2_F05/soal3/indomie";

  char jpgku[100]= "/home/seijaku/Documents/Sisop/SoalShiftSISOP20_modul2_F05/soal3/jpg.zip";
  char jpgd[100]= "/home/seijaku/Documents/Sisop/SoalShiftSISOP20_modul2_F05/soal3";
  char jpgf[100];
  sprintf(jpgf,"%s/jpg",jpgd);

  DIR *curDir;
  struct dirent *aDir;
  struct stat pathStat;
  
  if(child_id1<0){
      exit(EXIT_FAILURE);
  }

  if(child_id1==0){
    pid_t di_child1;
       di_child1=fork();
    if(di_child1<0){
        exit(EXIT_FAILURE);
    }
    if(di_child1==0){
        //makedir
        printf("Make-----Indomie Directory at %s\n",indomie);
        char *argv[] = {"mkdir", "-p", indomie, NULL};
        execv("/bin/mkdir",argv);
    }
    else
    {
        while ((wait(&status)) > 0);
        printf("Dealy 5 Second\n");
        sleep(5);
        while ((wait(&status)) > 0);
        printf("Make-----Sedaaap Directory at %s\n",sedaaap);
        char *argv[] = {"mkdir", "-p", sedaaap, NULL};
        execv("/bin/mkdir",argv);
        
    }
    
      
  }else{
      wait(&status);
      pid_t child_id2;
      pid_t aFolder;
      pid_t aFile;
      //anak kedua parrent
      child_id2=fork();
      if(child_id2<0){
          exit(EXIT_FAILURE);
      }

      if(child_id2==0){
            printf("Unziping-----%s to %s\n",jpgku,jpgd);
            char *argv[] = {"unzip","-qo", jpgku, "-d", jpgd, NULL};
            execv("/usr/bin/unzip", argv);
      }
      else{
            wait(&status);
            chdir(jpgf);
            printf("Changging Directory----- to %s\n",jpgf);
            curDir=opendir(".");
            printf("Reading Directory\n");
            if(curDir){

                while ( ( aDir=readdir(curDir) ) != NULL)
                {
                    
                    if(strcmp(aDir->d_name,".")==0 || strcmp(aDir->d_name,"..")==0 || aDir->d_name[0]=='.' ){
                        printf("FOUND . .. named %s\n",aDir->d_name);
                        continue;
                    }
                    else{
                        stat(aDir->d_name,&pathStat);
                        char filename[1000];
                        if(S_ISDIR(pathStat.st_mode)){
                            printf("FOUND DIR named %s\n",aDir->d_name);
                            aFolder=fork();

                            if(aFolder<0){
                                exit(EXIT_FAILURE);
                            }
                            
                            if(aFolder==0){
                                printf("FORK SUCCESS.....\n");
                                FILE *fileku;
                                sprintf(filename,"%s/%s/coba1.txt",jpgf,aDir->d_name);
                                printf("Making----- txt1 at %s\n",filename);
                                fileku=fopen(filename,"w");
                                fclose(fileku);
                                sleep(3);
                                sprintf(filename,"%s/%s/coba2.txt",jpgf,aDir->d_name);
                                printf("Making----- txt2 at %s\n",filename);
                                fileku=fopen(filename,"w");
                                fclose(fileku);
                                
                                struct stat isExist;
                                pid_t cpchild;
                                sprintf(filename,"%s/%s",indomie,aDir->d_name);
                                if( stat(filename,&isExist) == 0 && S_ISDIR(isExist.st_mode)){
                                    printf("FOUND----- %s at %s\n",aDir->d_name,indomie);
                                    cpchild=fork();
                                    if(cpchild<0){
                                        exit(EXIT_FAILURE);
                                    }
                                    if(cpchild==0){
                                        printf("Removing----- %s\n",filename);
                                        char *argv[] = {"rm","-rf", filename, NULL};
                                        execv("/bin/rm", argv);
                                    }else{
                                        while ((wait(&status)) > 0);
                                    }
                                }
                                sprintf(filename,"%s/%s",jpgf,aDir->d_name);
                                printf("Moving Folder----- %s to %s\n",filename,indomie);
                                char *argv[] = {"mv","-f", filename, indomie, NULL};
                                execv("/bin/mv", argv);
                            }
                        }
                        else{
                            printf("FOUND FILE named %s\n",aDir->d_name);
                            aFile=fork();
                            if(aFile<0){
                                exit(EXIT_FAILURE);
                            }
                            if(aFile==0){
                                sprintf(filename,"%s/%s",jpgf,aDir->d_name);
                                printf("FORK SUCCESS.....\n");
                                printf("Moving FILE----- %s to %s\n",filename,indomie);
                                char *argv[] = {"mv","-f", aDir->d_name, sedaaap, NULL};
                                execv("/bin/mv", argv);
                            }else{
                                wait(&status);
                            }
                            
                        }
                    }
                    ;
                }
                curDir=NULL;
                
            }


      }
      //wait(&status);
  }
    
}
