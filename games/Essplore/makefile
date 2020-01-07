CC=g++
SOURCES=main.cpp

all:
	$(CC) -c -o a.o $(SOURCES) -I".\freeglut\include"
	$(CC) -o a.exe a.o -L".\freeglut\lib" -lfreeglut -lopengl32

noCMD:
	$(CC) -c -o a.o $(SOURCES) -I".\freeglut\include"
	$(CC) -o a.exe out.o -L".\freeglut\lib" -lfreeglut -lopengl32 -Wl,--subsystem,windows

clean:
	rm a.o
	rm a.exe