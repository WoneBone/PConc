CC=gcc
CFLAGS=-g -Wall 


picture: main.o imageRoutines.o image-lib.o
	$(CC) $(CFLAGS) -o picture main.o imageRoutines.o image-lib.o -lgd

serial-basic.o: main.c image-lib.h
	$(CC) -c $(CFLAGS) main.c image-lib.h imageRoutines.h -lgd -lm -lpthreads

drenasQueOProfFez.o: imageRoutines.c imageRoutines.h image-lib.h
	$(CC) -c $(CFLAGS) imageRoutines.c imageRoutines.h image-lib.h -lgd

image-lib.o:image-lib.c image-lib.h
	$(CC) -c $(CFLAGS) image-lib.c image-lib.h -lgd
	
clean:
	rm -f *.o core a.out picture *~
clean-data:
	rm -fry ./Resize-dir ./Thumbnail-dir ./Watermark-dir
