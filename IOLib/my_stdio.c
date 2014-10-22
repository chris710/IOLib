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
	MY_FILE* new_file = (MY_FILE*)malloc(sizeof(MY_FILE*));			//static MY_FILE new_file;
	//new_file = { new_desc, new_buffer, new_wbuffer, 0, 0, 0 };

	char* new_buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	new_file->buffer = new_buffer;
	new_file->file = new_desc;
	new_file->previous = NULL;
	new_file->pointer = 0;
	new_file->wpointer = 0;
	new_file->eof = 0;

	return new_file;
}

int my_fclose(MY_FILE *f) {
	if (f == NULL)
		return -1;
	/* free all the resources of MY_FILE */
	int res;
	if (res = close(f->file) < 0)
		return 0;
	free(f->buffer);
	return 0;
}

int my_fread(void *p, size_t size, size_t nbelem, MY_FILE *f) {

	/* check for errors */
	if (size < nbelem || f->buffer == NULL) {	//error
		return -1;
	}

	int eof = 0;	//# of bytes read
	void* destination = p;	// (void*)((char*)p + f->pointer);		//read from where
	/* flushing buffer if other destination */
	if (f->previous != NULL && p != f->previous) {
		memcpy(f->previous, &(f->buffer[0]), f->pointer +1);	//copy everything there is
		f->previous = p;
	}
	else if (f->previous == NULL) {
		f->previous = p;
	}
	
	if (nbelem > BUFFER_SIZE) {		//check if the size is bigger than the buffer
		int rest = (nbelem - (BUFFER_SIZE - f->pointer));	//how much more do need to read
		if (eof = read(f->file, f->buffer, (BUFFER_SIZE - f->pointer)) < 0)		//read what is left in the buffer
			return -1;
		/*if (!eof) {		//no bytes have been read (and no more will)
			f->eof = 0;
			return 0;
		}*/
		memcpy(destination, f->buffer[f->pointer], eof);
		while (rest>=0) {
			if( eof = read(f->file, f->buffer,(rest>BUFFER_SIZE ? BUFFER_SIZE : rest)) < 0)
				return -1;
			memcpy(destination, &(f->buffer[0]), eof);
			rest -= BUFFER_SIZE;
			/* check if eof */
			/*if (!eof) {
				f->eof = 1;
			}*/
			f->pointer == 0;
		}
	}
		// check if we need to read at all
	else if (f->pointer == 0 || (f->pointer + nbelem) > BUFFER_SIZE) {		//no data has been read yet or not all data is not in buffer
		if (eof = read(f->file, f->buffer, BUFFER_SIZE) < 0)	//read as much as you can
			return -1;
		/* check if eof */
		/*if (!eof) {
			f->eof = 1;
		}*/
		memcpy(destination, &(f->buffer[f->pointer]), nbelem);
	}
	else { //just copy the data from buffer to the destination 
		memcpy(destination, &(f->buffer[f->pointer]), nbelem);
	}
	/* return */
	if (!(f->pointer == (BUFFER_SIZE-1)) && (f->buffer[f->pointer + 1] > 128 || f->buffer[f->pointer+1] <0 || f->buffer[f->pointer+1] == NULL) && !eof) {	//reached the end of buffer	//TODO null comparison doesn't work	/side condition!!!
		f->eof = 1;
		return 0;
	}
	if (f->pointer + nbelem < BUFFER_SIZE)	//TODO change condition to include overflow
		f->pointer += nbelem;	//move the pointer
	return nbelem;
}

int my_fwrite(void *p, size_t taille, size_t nbelem, MY_FILE *f) {
	
	/* check for errors */
	if (taille < nbelem || f->buffer == NULL) {			//error
		return -1;
	}

	int eof = 1;	//# of bytes written  
	void* destination = p;	// (void*)((char*)p + f->wpointer);		//write to where
	/* flushing buffer if destination is different */
	if (f->previous != NULL && p != f->previous) {
		memcpy(f->previous, &(f->buffer[0]), f->pointer +1);	//copy everything there is
		if (eof = write(f->file, f->buffer, f->wpointer + 1) < 0)	//write what you already have
			return -1;	//error but it shouldn't be here
		f->previous = p;
	}
	else if (f->previous == NULL) {
		f->previous = p;
	}
	if(nbelem > BUFFER_SIZE) {	//more to write than we can handle in one step
		int rest = (nbelem - (BUFFER_SIZE - f->wpointer));	//how much more there is space left
		memcpy(&(f->buffer[0]), destination, (BUFFER_SIZE - f->wpointer));
		while (rest>=0) {		//write BUFFER_SIZE chunks till you make it
			if (eof = write(f->file, f->buffer, (rest > BUFFER_SIZE ? BUFFER_SIZE : rest)) < 0)
				return -1;
			rest -= BUFFER_SIZE;
		}
	}
		// check if we should write already
	else if (f->wpointer == BUFFER_SIZE-1 || (f->wpointer + nbelem) > (BUFFER_SIZE-1)) {		//buffer is full or buffer will overflow
		/* fill and write what we already got */
		memcpy(&(f->buffer[f->wpointer]),destination, (BUFFER_SIZE - f->wpointer));
		if (eof = write(f->file, f->buffer, f->wpointer + 1) < 0)	//write what you already have
			return -1;
		/* deal with the rest of data */
		destination = (void *)((char*)destination + (f->wpointer + nbelem - BUFFER_SIZE));
		memcpy(&(f->buffer[f->wpointer]),destination, nbelem);
	}
	else { //just add data to the buffer
		memcpy(&(f->buffer[f->wpointer]),destination, nbelem);
	}
	/* return */
	if (f->wpointer + nbelem < BUFFER_SIZE && eof)	//TODO change condition to include overflow
		f->wpointer += nbelem;	//move the pointer

	if (eof < 1)		//error
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
