# Moataz Dabbour 
# 100863820

all: final

final: main.o init.o ops.o building.o
	@echo "Linking"
	@gcc main.o init.o ops.o building.o -o sim


main.o: main.c
	@gcc -c main.c

init.o: init.c
	@gcc -c init.c

ops.o: ops.c
	@gcc -c ops.c

building.o: building.c
	@gcc -c building.c

clean:
	@echo "Cleaning"
	@rm main.o init.o ops.o building.o