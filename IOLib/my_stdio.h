#ifndef MY_STDIO_H
#define MY_STDIO_H

#define BUFFER_SIZE 512

#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct MY_FILE {
	int file;	//file descriptor given by open
	char* rbuffer;	//buffer for reading 
	char* wbuffer;	//buffer for writing 
	int pointer;	//last element read in rbuffer
	int wpointer;	//last element written in wbuffer
	int eof;	//eof occured during last read;
}MY_FILE;

//opens a file 'name' in a mode 'mode'
MY_FILE *my_fopen(char *name, char *mode);

//frees memory of a file 'f'
int my_fclose(MY_FILE *f);

//reads at most 'nbelem' bytes to array/file 'p' of size 'size' from file 'f'
int my_fread(void *p, size_t size, size_t nbelem, MY_FILE *f);

//writes at most 'nbelem' bytes from array/file 'p' of size 'size' to file 'f'
int my_fwrite(void *p, size_t taille, size_t nbelem, MY_FILE *f);

//returns 1 if eof occured during previous read
int my_feof(MY_FILE *f);



/**  FORMATED I/O  SECTION  **/

//writes text to 'f'
int my_fprintf(MY_FILE *f, char *format, ...);

//reads addresses after format from 'f'
int my_fscanf(MY_FILE *f, char *format, ...);


#endif