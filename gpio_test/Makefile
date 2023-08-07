DEPS = io_utils.h
SRC_FILES = io_utils.c gpio_test.c
OBJ_FILES = $(SRC_FILES:.c=.o)
EXECUTABLE = gpio_test

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	${CC} $(OBJ_FILES) -o $(EXECUTABLE)

%.o: %.c $(DEPS)
	${CC} $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(EXECUTABLE)
