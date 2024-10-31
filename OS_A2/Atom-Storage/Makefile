CC = g++
CFLAGS = -Wall -Wextra -g -std=c++14 --coverage

SERVER_SRC = AtomStorageServer
SERVER = server

CLIENT_SRC = AtomClient
CLIENT = client

MOLECULE = Molecule
MOLECULE_FACTORY = MoleculeFactory

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_SRC).o $(MOLECULE).o $(MOLECULE_FACTORY).o
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER_SRC).o --coverage

$(CLIENT): $(CLIENT_SRC).o
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT_SRC).o --coverage

$(SERVER_SRC).o: $(SERVER_SRC).cpp
	$(CC) $(CFLAGS) -c $(SERVER_SRC).cpp

$(MOLECULE).o: $(MOLECULE).cpp
	$(CC) $(CFLAGS) -c $(MOLECULE).cpp

$(MOLECULE_FACTORY).o: $(MOLECULE_FACTORY).cpp
	$(CC) $(CFLAGS) -c $(MOLECULE_FACTORY).cpp

$(CLIENT_SRC).o: $(CLIENT_SRC).cpp
	$(CC) $(CFLAGS) -c $(CLIENT_SRC).cpp

clean:
	rm -f $(SERVER) $(CLIENT) *.o *.gcno *.gcda
