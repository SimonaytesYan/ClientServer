BIN = bin
OBJ = obj
COMPILER = g++
FLAGS = -O2

run: prepare $(BIN)/server $(BIN)/client
	./$(BIN)/server &
	./$(BIN)/client &

kill:
	kill -9 %1 %2

$(BIN)/server: server/main.cpp
	$(COMPILER) $(FLAGS) server/main.cpp -o $(BIN)/server

$(BIN)/client: client/main.cpp
	$(COMPILER) $(FLAGS) client/main.cpp -o $(BIN)/client

clean:
	rm $(BIN)/.*
	rm $(OBJ)/.*

prepare:
	-mkdir $(BIN)
	-mkdir $(OBJ)
	