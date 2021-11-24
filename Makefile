CC=gcc
CFLAGS= -g -Wall -I include/
EXEC=cassini

$(EXEC): src/cassini.o src/utils/requests.o src/utils/custom_string.o src/utils/command_line.o
	$(CC) $(CFLAGS) -o $@ $^
%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<
distclean:
	@rm -rf *.o src/*.o src/utils/*.o cassini
