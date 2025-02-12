CC = g++
CFLAGS = -I sqlite3 -std=c++17
SRC = main.cpp sqlite3/sqlite3.c
TARGET = url_shortener.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
