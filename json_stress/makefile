CC=g++
FLAGS= -g  -std=c++11
INC= -I../depends/include/ -I/usr/local/openssl/include/  -I/usr/local/include/
#LIBS= -L/usr/local/lib/ -L/lib64 -L/usr/local/openssl/lib -lboost_system -lboost_random -lboost_regex  -lssl -lcrypto -lboost_thread -lboost_chrono -ljson ../common/libcommon.a -lpthread -lz -lcurl -lssl
LIBS= -L/usr/local/lib/ -L/lib64 -L/usr/local/openssl/lib -lboost_system -lboost_random -lboost_regex  -lssl -lcrypto -lboost_thread -lboost_chrono -ljson  -lpthread -lz -lcurl -lssl
SRC=$(wildcard *.cpp)
BIN=$(patsubst %.cpp, %, $(SRC))

all:$(BIN)
	
%:%.cpp
	$(CC) $< -o $@ $(INC) $(LIBS) $(FLAGS) 
	

clean:
	rm -f $(BIN)
	

