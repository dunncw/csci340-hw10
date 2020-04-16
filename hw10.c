#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct bd{

    char file_sig[16];
    int arr_cnt;
    
    int reader; // simply used to read bytes and store them in the the buffer     
    char* line;
} bd;



int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "usage: hw10 <file_name>\n");
        exit(1);
    }
  
    char* file_name;
    file_name = argv[1];

    int fd;
    //char fbuf[10];
    //char fbuf_w[10];

    bd record;

    fd = open(file_name, O_RDONLY);
    if(fd != -1){
        read(fd, &record.file_sig, 4);
        lseek(fd, 4, SEEK_SET);
        read(fd, &record.arr_cnt, 4);
        //close(fd);
    }

    int offset_arry[record.arr_cnt];

    int end_of_offset_arr = record.arr_cnt * 4 + 8; //4 every int is 1 byte and there are x amount and the 8 is for the file_sig and arr_cnt 
    int count = 0;

    lseek(fd, 4, SEEK_SET);
    //read thru offset array
    for(int i = 8; i <  end_of_offset_arr; i = i + 4){
        lseek(fd, i, SEEK_SET);
        read(fd, &record.reader, 4);
        offset_arry[count] = record.reader;
        record.reader = 0;
        //lseek(fd, 4, SEEK_CUR);
        count++;
    }

    printf("%s\n", record.file_sig);
    printf("%d\n", record.arr_cnt);

    int len_of_line = 0;
    
    for(int i = 0; i < record.arr_cnt ; i++){
        lseek(fd, offset_arry[i], SEEK_SET);
        read(fd, &record.reader, 4);
        
        len_of_line = (record.reader - 1) * 4;
        char raw_line[len_of_line];        

        lseek(fd, offset_arry[i] + 4, SEEK_SET);
        read(fd, raw_line, record.reader);

        record.line = raw_line;
        fprintf(stdout, "%s\n", record.line);
        record.reader = 0; 
    }


    return 0;

}
