CC=gcc
CFLAGS= -g -Wall -I include/ 
EXEC=cassini

$(EXEC): src/cassini.o src/utils/requests.c
	$(CC) $(CFLAGS) -o $@ $^
%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<
distclean:
	@rm -rf *.o src/*.o cassini
