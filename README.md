# SoalShiftSISOP20_modul2_F5
Repository Praktikum Sisop 1

5111840000100-ABDUR ROHMAN

5111840000166-FERALDY NATHANAEL



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
