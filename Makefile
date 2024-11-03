BIN = bin
OBJ = obj
COMPILER = g++
FLAGS = -O2

run_tcp: prepare $(BIN)/server $(BIN)/client
	./$(BIN)/server "tcp" &
	./$(BIN)/client "tcp" & 

kill:
	kill -9 %1

$(BIN)/server: server/main.cpp server/server.cpp
	$(COMPILER) $(FLAGS) server/main.cpp server/server.cpp -o $(BIN)/server

$(BIN)/client: client/main.cpp server/server.cpp
	$(COMPILER) $(FLAGS) client/main.cpp server/server.cpp -o $(BIN)/client

clean:
	rm $(BIN)/.*
	rm $(OBJ)/.*

prepare:
	-mkdir $(BIN)
	-mkdir $(OBJ)
	