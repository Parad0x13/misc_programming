CC=g++
SOURCES=ProteinEvolve.cpp

all:
	$(CC) -c -o out.o $(SOURCES) -I".\freeglut\include"
	$(CC) -o out.exe out.o -L".\freeglut\lib" -lfreeglut -lopengl32

noCMD:
	$(CC) -c -o out.o $(SOURCES) -I".\freeglut\include"
	$(CC) -o out.exe out.o -L".\freeglut\lib" -lfreeglut -lopengl32 -Wl,--subsystem,windows

clean:
	rm out.o
	rm out.exe