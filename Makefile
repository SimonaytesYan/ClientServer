BIN = bin
OBJ = obj
COMPILER = g++
FLAGS = -O2 -fsanitize=address -g  

SRC = src

run_tcp: prepare $(BIN)/server $(BIN)/client
#	./$(BIN)/server "tcp" &
#	./$(BIN)/client "tcp" & 

run_udp: prepare $(BIN)/server $(BIN)/client
	./$(BIN)/server "udp" &
	./$(BIN)/client "udp" & 

$(BIN)/server: $(SRC)/server/main.cpp server/server.cpp
	$(COMPILER) $(FLAGS) $(SRC)/server/main.cpp $(SRC)/server/server.cpp -o $(BIN)/server

$(BIN)/client: $(SRC)/client/main.cpp $(SRC)/client/client.cpp
	$(COMPILER) $(FLAGS) client/main.cpp $(SRC)/client/client.cpp -o $(BIN)/client

clean:
	rm $(BIN)/*
	rm $(OBJ)/*

prepare:
	-mkdir $(BIN)
	-mkdir $(OBJ)
	