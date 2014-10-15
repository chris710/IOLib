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
	else if (f->pointer == 0 || (f->pointer + nbelem) > BUFFER_SIZE) {		//no data has been read yet or no all data is not in buffer
		eof = read(f->file, f->rbuffer,BUFFER_SIZE);	//read as much as you can
		/* check if eof */
		if (!eof) {
			f->eof = 1;
		}
		f->pointer == 0;
	}
	else { //just copy the data from buffer to the destination 
		memcpy(p, f->rbuffer[f->pointer], eof);
		/* check if eof */
		if (!eof) {
			f->eof = 1;
		}
		f->pointer == 0;
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
	/* check if buffer is full */
	if (f->wpointer == BUFFER_SIZE - 1) {	//if so write it
		eof = write(f->file, f->wbuffer,BUFFER_SIZE);		//read what is left in the buffer
	}
	else if(nbelem > BUFFER_SIZE) {	//more to write than we can handle in one step
		memcpy(f->rbuffer[0], p, eof);
	}


	if (!eof)		//error
		return -1;





	if (nbelem > BUFFER_SIZE) {		//check if the size is bigger than the buffer
		int rest = (nbelem - (BUFFER_SIZE - f->pointer));	//how much more do need to read
		eof = write(f->file, f->buffer,(BUFFER_SIZE - f->pointer));		//read what is left in the buffer
		if (eof == -1) {			//error
			return -1;
		}
		//for (i = 0; i < times; ++i) {
		while (rest>=0) {
			eof = write(f->file, f->buffer,(rest>BUFFER_SIZE ? BUFFER_SIZE : rest));
			rest -= BUFFER_SIZE;
		}
	}
		// check if we need to read at all
	else if (f->pointer == 0 || (f->pointer + nbelem) > BUFFER_SIZE) {		//no data has been read yet or no all data is not in buffer
		eof = write(f->file, f->buffer,BUFFER_SIZE);	//read as much as you can
		if (eof == -1) {			//error
			return -1;
		}
	}
	else { //just copy the data from buffer to the destination 
		memcpy(p, f->buffer[f->pointer], eof);
	}
	/* return */
//	if (f->pointer + nbelem < BUFFER_SIZE && !eof)
//		f->pointer += nbelem;	//move the pointer
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
