CXX = g++
EXE = bfk

LOPTS = #-lsfml-graphics -lsfml-window -lsfml-system 

all: $(EXE)

$(EXE): main.o
	$(CXX) main.o -o $(EXE) $(LOPTS) 

main.o: main.cpp
	$(CXX) -c main.cpp -o main.o

clean:
	rm $(EXE) *.o