#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>
#include <errno.h>

bool isBintang(char argku[]){
    for (int i = 0 ; i < strlen(argku) ; i++){
        if(argku[i]=='*'){
            return true;
        }
    }
    return false;
}
bool isNumber(char argku[]){
    for (int i = 0 ; i < strlen(argku) ; i++){
        if(argku[i]<48 && argku[i]>57){
            return false;
        }
    }
    return true;
}


int main(int argc, char *argv[]){
    int arrTime[4];
    printf("arg count %d\n",argc);
     for(int i=0;i<argc;i++){
        printf("i = %d -> %s\n",i,argv[i]);
    }
    
    for(int i=1;i<argc-1;i++){
        //printf("i = %d -> %s\n",i,argv[i]);
        if(isNumber(argv[i])){
            arrTime[i-1]=atoi(argv[i]);
        }
        else if(isBintang(argv[i]))
        {
            arrTime[i-1]=-1;
        }
        else{
            printf("Argumen TIdak Valid ada penyusup\n");
            exit(EXIT_FAILURE);
        }
    }
    if(arrTime[0]<0 || arrTime[0]>59 || arrTime[1]<0 || arrTime[1]>59 || arrTime[2]<0 || arrTime[2]>23){
        printf("Argumen TIdak Valid Gak ada Jam segitu anjir\n");
        exit(EXIT_FAILURE);
    }
    
    /*<-----DEBUG ARGUMEN----->
    for (int j=0;j<3;j++){
        printf("inin %d\n",time[j]);
    }
    */
    /*<----Time Debug-->
    int whileCount=10;
    while(whileCount--){
        time_t rawTime;
        char output[10000];
        struct tm *timeData;
        //Get time from system store it into rawTime
        rawTime=time(&rawTime);
        //CONVERT RAW TIME INTO struct tm in localtime
        timeData=localtime(&rawTime);
        sprintf(output, "[%d %d %d %d:%d:%d]",timeData->tm_mday, timeData->tm_mon + 1, timeData->tm_year + 1900, timeData->tm_hour, timeData->tm_min, timeData->tm_sec);
        printf("%s\n",output);
        sleep(1);
    }
    */
    

    //<----DAEMON---->
    
    // pid_t pid,sid;
    // pid =fork();
    // if(pid<0){
    //    exit(EXIT_FAILURE);
    // } 
    // umask(0);
    // sid=setsid();
    // if(sid<0){
    //     exit(EXIT_FAILURE);
    // }
    // if(chdir("/")<0){
    //     exit(EXIT_FAILURE);
    // }
    // umask(0);
    // close(STDIN_FILENO);
    //close(STDOUT_FILENO);
    //close(STDERR_FILENO);
    // while(1){
    //     time_t rawTime;
    //     struct tm *timeData;
    //     rawTime=time(&rawTime);
    //     timeData=localtime(&rawTime);
    //     printf("%s",argv[4]);
    //     if((timeData->tm_sec == arrTime[0] || arrTime[0] == -1) && (timeData->tm_min == arrTime[1] || arrTime[1] == -1) || (timeData->tm_hour == arrTime[2] || arrTime[2] == -1)){
    //         pid_t child1;
    //         child1=fork();
    //         if(child1==0){
    //             //printf("%s",argv[4]);
    //             char *arg[4] = {"bashs", argv[4], NULL};
    //             execv("/bin/bash",arg);
    //         }
    //     }
    //     sleep(1);

    // }
    
}