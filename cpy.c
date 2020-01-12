#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> // Open - Creat functions
#include <errno.h>
#include <unistd.h> // Read - Write functions

/* Prototypes */

int main(int, char**);
void help();
void exit_errors(int);
void check_errors();

/* Macros */

#define BUFFER_SIZE 		4096
#define TRUE 				1
#define USER_PERMISSION_WRE 00700 //Código referente a permissão do usuario para ler, executar e escrever no arquivo
#define HELP_COMMAND 		"--help"


int main(int argc, char** argv){
	if (argc == 2 && !strcmp(argv[1], HELP_COMMAND)){
		help();
		exit_errors(0);
	}
		
	if (argc != 3) exit_errors(1); //Faltam argumentos

	char buffer[BUFFER_SIZE];
	int file_in, file_out, read_count, write_count;

	file_in = open(argv[1], O_RDONLY);
	check_errors();

	file_out = creat(argv[2], USER_PERMISSION_WRE);
	check_errors();
	
	while(TRUE){
		read_count = read(file_in, buffer, BUFFER_SIZE);
		/*
		* read function:
		* The value returned is the number of bytes read (zero indicates end of file)
		* and the file position is advanced by this number. 
		*/
		if (read_count <= 0) break;
		write_count = write(file_out, buffer, read_count);
		if (write_count <= 0) check_errors();
	}
	
	close(file_in);
	close(file_out);
	exit_errors(read_count);
	/*
	* if read_count was 0, mean it's finished with sucess
	* and whatever different value means it's finished with some error
	*/

}

void help(){
	printf("Execute this aplication with this arguments:\n");
	printf("file1 file2, where file1 copy will be created on file2\n");
	return;
}

void check_errors(){
	if (errno != 0){
		printf("%s\n", strerror(errno));
		exit_errors(errno);
	}
}

void exit_errors(int code){
	if (code != 0)
		printf("Something wrong happened, aborting...\n");
	exit(code);
}