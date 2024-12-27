CC = gcc
CFLAGS = -Wall
BIN = graph_output.exe
SRC = main.c graph_algorithm_library.c

# Rule to build the executable
$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

# Rule to run the executable and clean it up afterwards
run: $(BIN)
	$(BIN)
	
# Rule for visualization (assuming output is piped to dot)
vis: $(BIN)
	$(BIN) | dot -Tpdf -o vis.pdf
	del /f $(BIN)

# Clean rule to remove the executable and generated files
clean:
	del /f $(BIN) vis.pdf
