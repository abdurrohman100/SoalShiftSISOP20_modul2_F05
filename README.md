----------
# SoalShiftSISOP20_modul2_F5
Repository Praktikum Sisop 1

5111840000100-ABDUR ROHMAN

5111840000166-FERALDY NATHANAEL

## Penjelasan Penyelesaian Soal nomor 1
Pada soal no 1 kita diminta untuk membuat program yang mensimulasikan kerja crontab dengan ketentuan 

 - program menerima 4 argumen berupa detik(0-59), menit(0-59),jam(0-23), path file.sh
 - program mengeluarkan pesan error jika argumen tidak sesuai
 - program hanya bisa menerima 1 config cron
 - program berjalan di background
 - tidak menggunakan system()

Untuk memecahkanya  kita menggunakan bantuan time_t dan struct tm, karena program harus berjalan dibackground maka program berupa daemon.
Alur kerja programnya adalah

 1. Menerima argumen berupa argv*[]
 2. Melakukan pengecekan terhadap argumen yang dimasukkan, disini juga dilakukan pengolahan string argumen kedalam array integer arrTime[].
 - Jika argumen detik,menit dan jam adalah * maka kita ubah nilai arrTimenya menjadi -1
 - Jika argumen berupa angka maka kita ubah menjadi integer menggunakan atoi()
 - Jika argumen bukan angka atau bintang maka terjadi error program langsung exit
 - Kemudian cek apakah waktu yang didapat pada arrTIme sesuai constrains
 3. Membuat Daemon
 4. Kemudian mennggunakan time_t untuk mendapatkan waktu pada system
 4. dari time_t keemudian kita ubah menjadi struct tm dengan mengubah raw time menjadi localtime menggunakan fungsi localtime()
 5. Pada struct tm sendiri terdapat tm_sec, tm_min, tm_hour. Jadi kita tinggal membandingkan arrTime dengan isi struct tm yang berkolerasi, dan juga kita mengeset kondisi always true jika arrTIme bernilai -1
 6. Jika kondisi nomor 5 terpenuhi maka kita tinggal melakukan fork() kemudian melakukan execv pada script bash yang sudah diberikan pada argv*[].
``` 
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
```
Program akan melakukan validasi input detik,menit,jam, menggunakan fungsi isBintang dan isNumber , jika argumen adalah number maka akan diubah menjadi integer dan dimasukan kedalam arrTIme
```
int main(int argc, char *argv[]){
    int arrTime[4];
    for(int i=1;i<argc-1;i++){
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
```
Program juga melakukan  pengecekan terhadap range waktu yang diinputkan.
```
    if(arrTime[0]<-1 || arrTime[0]>59 || arrTime[1]<-1 || arrTime[1]>59 || arrTime[2]<-1 || arrTime[2]>23){
        printf("Argumen Tidak Valid Waktu yang anda masukkan melebihi waktu dunia manusia\n");
        exit(EXIT_FAILURE);
    }
```
Karena harus berjalan di background maka program akan dibuat menjadi daemon
```
    pid_t pid,sid;
    pid =fork();
    if(pid<0){
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
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    int status;
```
    Untuk Program utamanya sendiri berada di dalam while. Pertama adalah mendapatkan waktu dari system menggunakan time_t dengan fungsi time(). Kemudian mengubahnya menjadi format localtime dengan fungsi localtime(), localtime() sendiri mengembalikan sebuah struct tm. Dari struct tm tersebut kita melakukan pengecekan dengan nilai di arrTIme yang berkolerasi
```
    while(1){
        time_t rawTime;
        struct tm *locTimeData;
        rawTime=time(&rawTime);
        locTimeData=localtime(&rawTime);
        if( (locTimeData->tm_sec == arrTime[0] || arrTime[0] == -1) &&
            (locTimeData->tm_min == arrTime[1] || arrTime[1] == -1) && 
            (locTimeData->tm_hour == arrTime[2] || arrTime[2] == -1))
            {
```
Kemudian jika kondisi if terpenuhi maka akan dilakukan fork(). kemudian child yang dihasilkan akan melakukan exec terhadap bash script yang menjadi argumen.
```
            pid_t myChild,child2;   
            myChild=fork();
            if(myChild==0){
                char *argex[4] = {"bash", argv[4], NULL};
                execv("/bin/bash",argex);
            }else{
                while ((wait(&status)) > 0);
                sleep(1);
            }
        }else
        {   
        }
    }
    
}
```
fungsi isBintang mengembalikan true jika argumenya berupa arsterisk, false jika bukan bintang
```
bool isBintang(char argku[]){
    for (int i = 0 ; i < strlen(argku) ; i++){
        if(argku[i]=='*'){
            return true;
        }
    }
    return false;
}
```
fungsi isNumber mengembalikan true jika argumenya berupa Bilangan, false jika terdapat char yang bukan bilangan
```
bool isNumber(char argku[]){
    for (int i = 0 ; i < strlen(argku) ; i++){
        if(!isdigit(argku[i])){
            return false;
        }
    }
    return true;
}
```
Untuk mengetesnya kami membuat hehe.sh yang isinya mencetak waktu eksekusi script dan menyimpanya ke log
```
echo $(date) >> /home/seijaku/Downloads/SoalShiftSISOP20_modul2_F05-master/soal1/log.txt
```


## Penjelasan Penyelesaian Soal nomor 2
Soal no 2 meminta kita untuk membuat sebuah program dalam bahasa c yang mampu melakukan beberapa hal, yaitu :
    1. Membuat sebuah folder dengan format nama tahun-bulan-tanggal_jam:menit:detik setiap 30 detik
    2. Mendownload gambar dengan format nama tahun-bulan-tanggal_jam:menit:detik dari url setiap 5 detik
    3. Jika sudah terdownload 20 gambar, melakukan zip terhadap folder
    4. Menghapus folder yang sudah di zip
    5. Mengenerate sebuah program killer yang fungsinya untuk melakukan kill terhadap program utama
    
Program utama ini sendiri dapat dijalankan dalam 2 mode yaitu mode 1 yaitu -a dan mode 2 yaitu -b. Perbedaan kedua mode ini adalah 
mode 1 jika di kill dengan program killer akan langsung menghentikan semua operasinya, sedangkan mode 2 akan terlebih dahulu menyelesaikan proses yang sedang berlangsung.

Pertama-tama karna program akan berjalan setiap 30 detik sekali (membuat folder) maka program akan dibuat dalam bentuk daemon
``` 
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
    //program daemon
    sleep(30)
  }
     
```

Selain itu program juga menerima argumen untuk menentukan mode, sehingga pada int main diterima argmode yang akan di cek, jika argmode
= a maka mode = 1, jika argmode = b maka mode = 2.


``` 
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
```

Pada bagian kode dalam daemon, dilakukan beberapa kali fork. Hal ini dikarenakan tidak hanya 1 process saja yang akan dilakukan program.
Process-process tersebut adalah process membuat folder, mendownload gambar, mengconvert folder menjadi zip dan terakhir meremove folder
yang sudah di zip. Dari sini maka harus dilakukan beberapa kali fork, dengan child paling akhir akan menjalankan pembuatan folder. Child
selanjutnya berfungsi menjalankan download. Child selanjutnya lagi akan menjalankan konversi folder menjadi zip. Dan terakhir, parrent
nya akan melakukan remove folder yang sudah di zip.



``` 

void killer (int mode){
    FILE* killer;
    int status;
    killer = fopen("killer.c","w");
if(mode==2){
    char pid[80];
    sprintf(pid,"\"%d\"",getpid());
    char command[100000] = "umask(0);\nprintf(\"killed\");\nexecl(\"/bin/kill\", \"kill\",";
    strcat(command,pid);
    strcat(command," ,NULL)");
    fprintf(killer,"#include <stdlib.h>\n#include <sys/types.h>\n#include <sys/stat.h>\n#include <sys/prctl.h>\n#include <unistd.h>\n#include <wait.h>\n#include <stdio.h>\n#include <string.h>\n#include <time.h>\nint main(){%s;}",command);
    fclose(killer);
    pid_t childkiller = fork();
  if(childkiller==0){
    char *argv[]={"gcc","killer.c","-o","killer.exe",NULL};
    execv("/usr/bin/gcc",argv);
  }
  while ((wait(&status)) > 0);
}
  else{
    char command[100000] = "umask(0);\nprintf(\"killed\");\nexecl(\"/usr/bin/killall\", \"killall\",\"testno2.exe\",NULL)";
    fprintf(killer,"#include <stdlib.h>\n#include <sys/types.h>\n#include <sys/stat.h>\n#include <sys/prctl.h>\n#include <unistd.h>\n#include <wait.h>\n#include <stdio.h>\n#include <string.h>\n#include <time.h>\nint main(){%s;}",command);
    fclose(killer);
    pid_t childkiller = fork();
  if(childkiller==0){
    char *argv[]={"gcc","killer.c","-o","killer.exe",NULL};
    execv("/usr/bin/gcc",argv);
  }
  }
}
     
```
Terakhir pada soal no 2, kita membuat fungsi dalam program yang gunanya untuk mengenerate sebuah program killer. Fungsi akan 
menerima mode apa yang dijalankan user. Jika mode 2, maka akan dibuat file .c yang isinya akan melakukan execv kill parrent dari
program utama. Sedangkan jika mode 1, maka akan dibuat file .c yang isinya akan melakukan killall terhadap program no2.
Setelah itu fungsi akan melakukan fork yang isinya menjalankan gcc untuk mengcompile file killer.c tersebut. Dalam program c killer yg dibuat program killer akan melakukan fork, child akan melakukan kill atau killall terhadap program utama sedangkan parentnnya akan
melakukan remove() terhadap program killer yang dibuat (remove killer.c dan killer.exe)


## Penjelasan Penyelesaian Soal nomor 3

Pada nomor 3 kita diminta membuat program untuk multiproccesing ketentuanya sebagai berikut

 1. Program buatan jaya harus bisa membuat dua direktori di
“/home/[USER]/modul2/”. Direktori yang pertama diberi nama “indomie”, lalu
lima detik kemudian membuat direktori yang kedua bernama “sedaap”.
 2. Kemudian program tersebut harus meng-ekstrak file jpg.zip di direktori
“/home/[USER]/modul2/”. Setelah tugas sebelumnya selesai, ternyata tidak
hanya itu tugasnya.
 3. Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (di
dalam direktori “home/[USER]/modul2/jpg/”) harus dipindahkan sesuai dengan
pengelompokan, semua file harus dipindahkan ke
“/home/[USER]/modul2/sedaap/” dan semua direktori harus dipindahkan ke
“/home/[USER]/modul2/indomie/”.
 4. Untuk setiap direktori yang dipindahkan ke “/home/[USER]/modul2/indomie/”
harus membuat dua file kosong. File yang pertama diberi nama “coba1.txt”, lalu
3 detik kemudian membuat file bernama “coba2.txt”.
(contoh : “/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”).

untuk code nomer 3 bisa dilihat [disini](https://raw.githubusercontent.com/abdurrohman100/SoalShiftSISOP20_modul2_F05/master/soal3/no3rev.c) 
Pertama kita lakukan fork(). kemudian pada child1 kita melakukan pembuatan directory. Karena ada 2 directory maka kita fork() lagi pada proses child1 sebut saja anaknya di_child1 pada proses di_child1 kita lakukan mkdir untuk indomie, kemudian pada parrentnya (child1) akan melakukan wait kemudain melakukan delay menggunakan sleep selaa 5 detik. Kemudian child1 akan membuat directory sedaaap

```
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

```
Kemudian pada parrent utama akan menungu hingga child1 selesai.
Setelah itu parrent utama melakukan fork() lagi sebut saja anaknya child2
child2 kemudian akan melakukan execv unutk melakukan unzipping terhadap jpeg.zip

```
  if(child_id2==0){
            printf("Unziping-----%s to %s\n",jpgku,jpgd);
            char *argv[] = {"unzip","-qo", jpgku, "-d", jpgd, NULL};
            execv("/usr/bin/unzip", argv);
        }
```
Parrent utama akan menunggu child2 selesai kemudian parrent utama akan melakukan opendir() terhadap direktori hasil unzip tadi	` .
```
chdir(jpgf);
printf("Changging Directory----- to %s\n",jpgf);
curDir=opendir(".");
```
Pembacaan dilakukan menggunakan struct dirrent  dan fungsi readdir()
dan melakukan pengecekan terhadap semua isi direktori tersebut yang bukan merupakan direktori . ataupun .. apakah isi direktori tersebut adalah sebuah folder atau file. Untuk membedakan file atau folder kami menggunakan struct stat dan fungsi S_ISDIR().
Jika ditemukan folder maka lakukan fork() sebut saja nama anaknya aFolder. pada aFolder ini pertama akan membuat 2 buah txt yaitu coba1.txt dan coba2.txt menggunakan fopen() dengan metode "w"  dimana terdapat delay 3 seccond dalam pembuatanya. Kemudian aFolder akan melakukan pengecekan apakah di direktori indomie terdapat folder yang namanya sama dengan folder sekarang, jika ada maka aFolder akan melakukan fork() sebut saja anaknya cpchild. cpchild akan melakukan execv dan meremove folder yang sudah ada pada indomie. aFolder akan menunggu cpchild selesai kemudian baru melakukan execv mv untuk memindahkan folder sekarang ke indomie. 
Namun jika yang ditemukan adalah file maka langsung saja parrent utama akan melakukan fork(), sebut saja anaknya aFile pada aFile ini dia akan langsung melakukan mv terhadap file tersebut ke folder sedaaap.

```
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
}
```
