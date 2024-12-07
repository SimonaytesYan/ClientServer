BIN = bin
OBJ = obj
COMPILER = g++
FLAGS = -O2 -fsanitize=address -g -lssl -lcrypto

SRC = src

run_tcp: prepare $(BIN)/server $(BIN)/client
#	./$(BIN)/server "tcp" &
#	./$(BIN)/client "tcp" & 

run_udp: prepare $(BIN)/server $(BIN)/client
	./$(BIN)/server "udp" &
	./$(BIN)/client "udp" & 

$(BIN)/server: $(SRC)/server/main.cpp $(SRC)/server/server.cpp $(OBJ)/tls.o
	$(COMPILER) $(FLAGS) $(SRC)/server/main.cpp $(SRC)/server/server.cpp $(OBJ)/tls.o -o $(BIN)/server

$(BIN)/client: $(SRC)/client/main.cpp $(SRC)/server/server.cpp $(OBJ)/tls.o
	$(COMPILER) $(FLAGS) $(SRC)/client/main.cpp $(SRC)/server/server.cpp $(OBJ)/tls.o -o $(BIN)/client

$(OBJ)/tls.o: $(SRC)/tls/tls.cpp $(SRC)/tls/tls.hpp
	$(COMPILER) -c $(FLAGS) $(SRC)/tls/tls.cpp  -o $(OBJ)/tls.o

clean:
	rm $(BIN)/*
	rm $(OBJ)/*

prepare:
	-mkdir $(BIN)
	-mkdir $(OBJ)
	