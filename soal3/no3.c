#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>


int main() {
  pid_t child_idA;
  pid_t child_idB;
  int status;
  child_idA = fork();
  child_idB = fork();
  
  if (child_idA < 0 || child_idA < 0) {
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
  }

  if (child_idA == 0) {
    // this is child
    char *argv[] = {"mkdir", "-p", "/home/seijaku/modul2/indomie" , NULL};
    execv("/bin/mkdir", argv);

  } else if(child_idB==0){
    // this is parent
    // no need to wait
    // while ((wait(&status)) > 0);
    //sleep(5);
    char *argv[] = {"mkdir", "-p" , "/home/seijaku/modul2/sedaaap" , NULL};
    execv("/bin/mkdir", argv);
  }
    
    child_idA = fork();
    //IBU
    if (child_idA < 0) {
        exit(EXIT_FAILURE);
    }
    //Anak
    if (child_idA == 0) {
    // this is child
        char *argv[] = {"unzip", "/home/seijaku/jpg.zip", "-d", "/home/seijaku/modul2", NULL};
        execv("/usr/bin/unzip", argv);
    } 
    //IBU nya nunggu
    else {
        while ((wait(&status)) > 0);
    }
    
    child_idA=fork();
    if (child_idA < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_idA == 0) {
    // this is child
        
        char *argv[] = {"mv", "/home/seijaku/modul2/jpg//*/","/home/seijaku/modul2/sedaaap" , NULL};
        execv("/bin/mv", argv);

    } else if(child_idB==0){
    
    }



}