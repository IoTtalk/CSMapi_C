CC = gcc
JSON_HEADER_DIR = /opt/local/include/json-c
CURL_HEADER_DIR = /opt/local/include/curl
LIB_DIR = /opt/local/lib

INCLUDE = -I$(JSON_HEADER_DIR) -I$(CURL_HEADER_DIR)
LDFLAGS = -L$(LIB_DIR) -ljson-c -lcurl

main.out: main.o CSMapi.o
	CC $(INCLUDE) $(LDFLAGS) -o main CSMapi.o main.o

clean:
	rm -f *.o
	rm -f *.out
