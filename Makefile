cc = gcc
cflags = -lm -Wall
rm := -rm -rf
deps = main.h i2c.h
objects = main.o i2c.o

# automatically input name of file in %
%.o : %.c $(deps)
	$(cc) -c -o $@ $< $(cflags)
	
# automatically input name of file in %
%.exe : %.o $(objects)
	$(cc) 

# declare that clean is never a file to be created
.PHONY: clean
clean:
	$(rm) $(objects)