CC = g++
CFLAGS = -Wall -Wextra -g -std=c++14

SERVER_SRC = AtomStorageServer
SERVER = server

CLIENT_SRC = AtomClient
CLIENT = client

UDS_CLIENT_SRC = UDSClient
UDS_CLIENT = uds_client

MOLECULE = Molecule
MOLECULE_FACTORY = MoleculeFactory
CLIENT_API = ClientAPI

run: $(SERVER)
	./$(SERVER) -s uds_stream_socket -d uds_dgram_sock

all: $(SERVER) $(CLIENT) $(UDS_CLIENT)

$(SERVER): $(SERVER_SRC).o $(MOLECULE).o $(MOLECULE_FACTORY).o
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER_SRC).o --coverage

$(CLIENT): $(CLIENT_SRC).o $(CLIENT_API).o
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT_SRC).o $(CLIENT_API).o --coverage

$(UDS_CLIENT): $(UDS_CLIENT_SRC).o $(CLIENT_API).o
	$(CC) $(CFLAGS) -o $(UDS_CLIENT) $(UDS_CLIENT_SRC).o $(CLIENT_API).o --coverage

$(SERVER_SRC).o: $(SERVER_SRC).cpp
	$(CC) $(CFLAGS) -c $(SERVER_SRC).cpp

$(MOLECULE).o: $(MOLECULE).cpp
	$(CC) $(CFLAGS) -c $(MOLECULE).cpp

$(MOLECULE_FACTORY).o: $(MOLECULE_FACTORY).cpp
	$(CC) $(CFLAGS) -c $(MOLECULE_FACTORY).cpp

$(CLIENT_SRC).o: $(CLIENT_SRC).cpp
	$(CC) $(CFLAGS) -c $(CLIENT_SRC).cpp

$(UDS_CLIENT_SRC).o: $(UDS_CLIENT_SRC).cpp
	$(CC) $(CFLAGS) -c $(UDS_CLIENT_SRC).cpp

clean:
	rm -f $(SERVER) $(CLIENT) $(UDS_CLIENT) *.o *.gcno *.gcda
