CC = gcc
BIN = bin

compile:
	$(CC) $(FILE) -o $(BIN)/$(basename $(notdir $(FILE)))

run:
	./bin/$(FILE)

clean:
	rm -rf $(BIN)/*
