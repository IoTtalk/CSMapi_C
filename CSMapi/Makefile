CC = clang
JSON_HEADER_DIR = /usr/local/include/json-c
CURL_HEADER_DIR = /usr/local/include/curl
LIB_DIR = /usr/local/lib

INCLUDE = -I$(JSON_HEADER_DIR) -I$(CURL_HEADER_DIR)
LDFLAGS = -L$(LIB_DIR) -ljson-c -lcurl 

main.out: main.o CSMapi.o
	${CC} -o main CSMapi.o main.o $(INCLUDE) $(LDFLAGS) 

clean:
	rm -f *.o
	rm -f *.out
