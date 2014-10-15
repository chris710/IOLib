#include "my_stdio.h"

MY_FILE *my_fopen(char *name, char *mode) {
	/* mode parsing */
	int flags;	//file access mode
	if (*mode == 'r')
		flags = O_RDONLY;
	else if (*mode == 'w')
		flags = O_WRONLY;

	/* opening file */
	int new_desc = open(name,flags);
	if (new_desc == -1)
		return NULL;

	/* MY_FILE structure initialization */
	char* new_buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	MY_FILE new_file = { new_desc, new_buffer, 0, 0 };

	return &new_file;
}

int my_fclose(MY_FILE *f) {
	/* free all the resources of MY_FILE */
	free(f->buffer);
	if (f->buffer == NULL)
		return 1;
	return 0;
}

int my_fread(void *p, size_t size, size_t nbelem, MY_FILE *f) {
	/* check for errors */
	if (size < nbelem) {
		//error
		return -1;
	}
	/* check if we need to read */
	if (f->pointer == 0 || (f->pointer + nbelem) > BUFFER_SIZE) {		//no data has been read yet or no all data is not in buffer
		read(f, f->buffer,BUFFER_SIZE);	//read as much as you can
		/* check if eof */
		f->pointer == 0;
	}
	/* copy the mem to the destination */
	memcpy(p, f->buffer[f->pointer]);
	/* if the size is bigger than the buffer*/
	if (nbelem > BUFFER_SIZE) {
		int i = 0, times = nbelem / BUFFER_SIZE;	//how many times we need to fill the buffer
		memcpy(p, f->buffer[f->pointer]);
		for (i = 0; i < times; ++i) {

		}
	}
	/* return */
	f->pointer += nbelem;	//move the pointer
	return nbelem;
}

int my_fwrite(void *p, size_t taille, size_t nbelem, MY_FILE *f) {

}

int my_feof(MY_FILE *f) {
	if (f->eof)
		return 1;
	return 0;
}



/**  FORMATED I/O  SECTION  **/
int my_fprintf(MY_FILE *f, char *format, ...) {

}

int my_fscanf(MY_FILE *f, char *format, ...) {

}
