BIN = bin
OBJ = obj
COMPILER = g++
FLAGS = -O2 -fsanitize=address -g  

run_tcp: prepare $(BIN)/server $(BIN)/client
#	./$(BIN)/server "tcp" &
#	./$(BIN)/client "tcp" & 

run_udp: prepare $(BIN)/server $(BIN)/client
	./$(BIN)/server "udp" &
	./$(BIN)/client "udp" & 

$(BIN)/server: server/main.cpp server/server.cpp
	$(COMPILER) $(FLAGS) server/main.cpp server/server.cpp -o $(BIN)/server

$(BIN)/client: client/main.cpp server/server.cpp
	$(COMPILER) $(FLAGS) client/main.cpp server/server.cpp -o $(BIN)/client

clean:
	rm $(BIN)/*
	rm $(OBJ)/*

prepare:
	-mkdir $(BIN)
	-mkdir $(OBJ)
	