#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void killer(int);

int main(int arg,char *argmode[]) {
  
  int mode;
  if(argmode[1][1]=='a')mode=1;
  else mode=2;
  pid_t child_id;
  int status;
  pid_t pid,sid;
  pid = fork();
  if(pid<0) exit(EXIT_FAILURE);
  if(pid>0) exit(EXIT_SUCCESS);
  umask(0);
  sid = setsid();
  if(sid<0) exit(EXIT_FAILURE);
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  killer(mode);
while(1){
  time_t rawtime;
  struct tm *info;
  time(&rawtime);
  info = localtime(&rawtime);
  char folder[80];
  char z[1000000]="/home/feraldy/";
  strftime(folder,80,"%Y-%m-%d_%X",info);
	
  child_id = fork();
  if (child_id < 0) {
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
  }
  if (child_id == 0) {
      pid_t child2_id;
      child2_id = fork();
      if(child2_id<0){
         exit(EXIT_FAILURE);
      }
      if(child2_id == 0) {
    	 pid_t child3_id;
      	 child3_id = fork();
      	 if(child3_id<0){
            exit(EXIT_FAILURE);
       	 }
      	 if(child3_id == 0) {
	          char *argv[] = {"mkdir", "-p", folder , NULL};
	          execv("/bin/mkdir", argv);
         }//make folder
         else{
            while ((wait(&status)) > 0);
                
	            int i;
	            for(i=0;i<20;i++){
		              chdir(folder);
		              time_t rawtime2;
  		            struct tm *info2;
  		            time(&rawtime2);
  		            info2 = localtime(&rawtime2);
		              char img[80];
		              strftime(img,80,"%Y-%m-%d_%X",info2);
		              strcat(img,".jpg");
		              char url2[80];
		              char url[100000] = "https://picsum.photos/";
		              sprintf(url2,"%ld",(rawtime2 % 1000)+100);
		              strcat(url,url2);
	                pid_t childtemp = fork();
		              if(childtemp == 0){
	  	                char *argv[] = {"wget",url,"-O",img, NULL};
		                  execv("/usr/bin/wget",argv);
		                  exit(EXIT_SUCCESS);
	              	}
		              sleep(5);
	              }
                chdir("..");       
         }//download
        printf("convert\n");
         char zip[100000];
         strcpy(zip,folder);
         strcat(zip,".zip");
         char *argv[] = {"zip", "-r", zip, folder, NULL};
    	execv("/usr/bin/zip",argv);
      }
      else{
         while ((wait(&status)) > 0);
	     printf("remove\n");
        char *argv[] = {"rm", "-r",folder, NULL};
         execv("/bin/rm", argv);
      }
  } 
  sleep(30);
}
}

void killer (int mode){
    FILE* killer;
    int status;
    killer = fopen("killer.c","w");
if(mode==2){
    char pid[80];
    sprintf(pid,"\"%d\"",getpid());
    char command[100000] = "umask(0);\nprintf(\"killed\");\nint status;\npid_t child_id;\nchild_id = fork();\nif(child_id == 0){\nexecl(\"/bin/kill\", \"kill\",";
    strcat(command,pid);
    strcat(command," ,NULL);}\nelse{while ((wait(&status)) > 0);\nremove(\"killer.c\");\nremove(\"killer.exe\");}");
    fprintf(killer,"#include <stdlib.h>\n#include <sys/types.h>\n#include <sys/stat.h>\n#include <sys/prctl.h>\n#include <unistd.h>\n#include <wait.h>\n#include <stdio.h>\n#include <string.h>\n#include <time.h>\nint main(){%s}",command);
    fclose(killer);
    pid_t childkiller = fork();
  if(childkiller==0){
    char *argv[]={"gcc","killer.c","-o","killer.exe",NULL};
    execv("/usr/bin/gcc",argv);
  }
  while ((wait(&status)) > 0);
}
  else{
    char command[100000] = "umask(0);\nprintf(\"killed\");\nint status;\npid_t child_id;\nchild_id = fork();\nif(child_id == 0){\nexecl(\"/usr/bin/killall\", \"killall\",\"testno2.exe\",NULL);}\nelse{while ((wait(&status)) > 0);\nremove(\"killer.c\");\nremove(\"killer.exe\");}";
    fprintf(killer,"#include <stdlib.h>\n#include <sys/types.h>\n#include <sys/stat.h>\n#include <sys/prctl.h>\n#include <unistd.h>\n#include <wait.h>\n#include <stdio.h>\n#include <string.h>\n#include <time.h>\nint main(){%s}",command);
    fclose(killer);
    pid_t childkiller = fork();
  if(childkiller==0){
    char *argv[]={"gcc","killer.c","-o","killer.exe",NULL};
    execv("/usr/bin/gcc",argv);
  }
  }
}


