#include <stdio.h>
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
#include<ctype.h>
bool isBintang(char argku[]);
bool isNumber(char argku[]);

int main(int argc, char *argv[]){
    int arrTime[4];
    // printf("arg count %d\n",argc);
    //  for(int i=0;i<argc;i++){
    //     printf("i = %d -> %s\n",i,argv[i]);
    // }
    
    for(int i=1;i<argc-1;i++){
        //printf("i = %d -> %s\n",i,argv[i]);

        if(isBintang(argv[i]))
        {
            arrTime[i-1]=-1;
        }
        else if(isNumber(argv[i])){
            arrTime[i-1]=atoi(argv[i]);
        }
        else{
            printf("Argumen Tidak Valid ada char penyusup\n");
            exit(EXIT_FAILURE);
        }
    }
    if(arrTime[0]<-1 || arrTime[0]>59 || arrTime[1]<-1 || arrTime[1]>59 || arrTime[2]<-1 || arrTime[2]>23){
        printf("Argumen Tidak Valid Waktu yang anda masukkan melebihi waktu dunia manusia\n");
        exit(EXIT_FAILURE);
    }
    /*<----DEBUG INPUT TIME---->
        //printf("%d %d %d\n",arrTime[0],arrTime[1],arrTime[2]); 
    */
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
    //printf("Hello Demon\n");
    pid_t pid,sid;
    pid =fork();
    if(pid<0){
        //printf("Gagal buat anak di daemon");
        exit(EXIT_FAILURE);
    } 
    umask(0);
    sid=setsid();
    if(sid<0){
        exit(EXIT_FAILURE);
    }
    if(chdir("/")<0){
        exit(EXIT_FAILURE);
    }
    //printf("%s\n",argv[4]);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    int status;
    while(1){
        time_t rawTime;
        struct tm *locTimeData;
        rawTime=time(&rawTime);
        locTimeData=localtime(&rawTime);
        //printf("%s",argv[4]);
       
        if((locTimeData->tm_sec == arrTime[0] || arrTime[0] == -1) && (locTimeData->tm_min == arrTime[1] || arrTime[1] == -1) || (locTimeData->tm_hour == arrTime[2] || arrTime[2] == -1)){
            pid_t myChild,child2;
            
            myChild=fork();
            child2=fork();
            // if(child2==0){
            //     char *argch[4] = {"chmod","+x" ,argv[4], NULL};
            //     execv("/bin/chmod",argch);
            // }
            // while ((wait(&status)) > 0);
            
            if(myChild==0){
                //printf("harusnya di exec");
                char *argex[4] = {"bash", argv[4], NULL};
                execv("/bin/bash",argex);
            }else{
                while ((wait(&status)) > 0);
            }
        }
        sleep(1);

    }
    
}

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
        if(isdigit(argku[i])){
            return false;
        }
    }
    return true;
}