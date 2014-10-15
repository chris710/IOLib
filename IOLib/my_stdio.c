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
	MY_FILE new_file = { new_desc, new_buffer, new_buffer, 0, 0, 0 };

	return &new_file;
}

int my_fclose(MY_FILE *f) {
	/* free all the resources of MY_FILE */
	free(f->wbuffer);
	free(f->rbuffer);
	if (f->wbuffer == NULL && f->rbuffer == NULL)
		return 1;
	return 0;
}

int my_fread(void *p, size_t size, size_t nbelem, MY_FILE *f) {
	int eof;	//# of bytes read
	/* check for errors */
	if (size < nbelem) {
		//error
		return -1;
	}
	
	if (nbelem > BUFFER_SIZE) {		//check if the size is bigger than the buffer
		int rest = (nbelem - (BUFFER_SIZE - f->pointer));	//how much more do need to read
		eof = read(f->file, f->rbuffer,(BUFFER_SIZE - f->pointer));		//read what is left in the buffer
		if (!eof) {		//no bytes have been read
			f->eof == 0;
			return 0;
		}
		memcpy(p, f->rbuffer[f->pointer], eof);
		//for (i = 0; i < times; ++i) {
		while (rest>=0) {
			eof = read(f->file, f->rbuffer,(rest>BUFFER_SIZE ? BUFFER_SIZE : rest));
			memcpy(p, f->rbuffer[0], eof);
			rest -= BUFFER_SIZE;
			/* check if eof */
			if (!eof) {
				f->eof = 1;
			}
			f->pointer == 0;
		}
	}
		// check if we need to read at all
	else if (f->pointer == 0 || (f->pointer + nbelem) > BUFFER_SIZE) {		//no data has been read yet or not all data is not in buffer
		eof = read(f->file, f->rbuffer,BUFFER_SIZE);	//read as much as you can
		/* check if eof */
		if (!eof) {
			f->eof = 1;
		}
		f->pointer == 0;
	}
	else { //just copy the data from buffer to the destination 
		memcpy(p, f->rbuffer[f->pointer], nbelem);
	}
	/* return */
	if (f->pointer + nbelem < BUFFER_SIZE && !eof)
		f->pointer += nbelem;	//move the pointer
	return nbelem;
}

int my_fwrite(void *p, size_t taille, size_t nbelem, MY_FILE *f) {
	int eof;	//# of bytes written  
	/* check for errors */
	if (taille < nbelem) {			//error
		return -1;
	}
	if(nbelem > BUFFER_SIZE) {	//more to write than we can handle in one step
		int rest = (nbelem - (BUFFER_SIZE - f->wpointer));	//how much more do need to read
		memcpy(f->wbuffer[0], p, (BUFFER_SIZE - f->wpointer));
		while (rest>=0) {		//write BUFFER_SIZE chunks till you make it
			eof = write(f->file, f->wbuffer,(rest>BUFFER_SIZE ? BUFFER_SIZE : rest));
			rest -= BUFFER_SIZE;
		}
	}
		// check if we should write already
	else if (f->wpointer == BUFFER_SIZE-1 || (f->wpointer + nbelem) > BUFFER_SIZE) {		//buffer is full or buffer will overflow
		eof = write(f->file, f->wbuffer, f->wpointer+1);	//write what you already have
	}
	else { //just add data to the buffer
		memcpy(f->wbuffer[f->wpointer],p, nbelem);
	}
	/* return */
	if (f->wpointer + nbelem < BUFFER_SIZE && !eof)
		f->wpointer += nbelem;	//move the pointer

	if (!eof)		//error
		return -1;
	
	return nbelem;
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
