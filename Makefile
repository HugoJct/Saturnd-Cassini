CC=gcc
CFLAGS= -g -Wall -I include/
BUILD_DIR=build

all: create_dirs saturnd cassini

saturnd: $(BUILD_DIR)/saturnd.o  $(BUILD_DIR)/daemon.o
	$(CC) $(CFLAGS) -o $@ $^

cassini: $(BUILD_DIR)/cassini.o $(BUILD_DIR)/utils/send_requests.o $(BUILD_DIR)/utils/custom_string.o $(BUILD_DIR)/utils/command_line.o $(BUILD_DIR)/timing-text-io.o $(BUILD_DIR)/utils/read_responses.o
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o : src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

create_dirs:
	@mkdir -p build/utils/

distclean:
	@rm -rf cassini saturnd saturnd.pid build/
