PROJNAME=defraw

CC=g++
CFLAGS+=
LDFLAGS+=-lSDL -lSDL_gfx -lm
EXECUTABLE=$(PROJNAME)

$(EXECUTABLE): locations.o main.o objects.o score.o
	$(CC) locations.o main.o objects.o score.o $(LDFLAGS) -o $(EXECUTABLE)

main.o: main.cpp score.h objects.h locations.h main.h defines.h
	$(CC) -c main.cpp

locations.o: locations.cpp score.h objects.h locations.h main.h
	$(CC) -c locations.cpp
	
objects.o: objects.cpp score.h objects.h defines.h
	$(CC) -c objects.cpp
	
score.o: score.cpp score.h
	$(CC) -c score.cpp
	
clean:
	rm -f *.o $(EXECUTABLE)
