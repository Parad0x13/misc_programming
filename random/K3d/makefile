# Make sure to use MinGW32-TDM 4.7.1 for this

CPP  = g++.exe
CC   = gcc.exe
WINDRES = windres.exe
RES  =
OBJ  = main.o models.o $(RES)
LINKOBJ  = main.o models.o $(RES)
LIBS =  -L"C:/MinGW/lib" -L"C:/freeglut/lib" -L"C:/binExtra/freeglut/lib" -mwindows -lfreeglut -lglu32 -lopengl32 -lwinmm -lgdi32
INCS =  -I"C:/MinGW/include" -I"C:/freeglut/include/GL" -I"C:/binExtra/freeglut/include/GL"
CXXINCS =  -I"C:/MinGW/lib/gcc/mingw32/4.7.1/include" -I"C:/MinGW/include"
BIN  = K3D.exe
CXXFLAGS = $(CXXINCS)
CFLAGS = $(INCS)
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before K3D.exe all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "K3D.exe" $(LIBS)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o $(CXXFLAGS)

models.o: models.cpp
	$(CPP) -c models.cpp -o models.o $(CXXFLAGS)
