CC = g++
CFLAGS = -Wall -Wextra -pedantic -march=native -std=c++20 -g
TARGET = passgen

all: $(TARGET)

$(TARGET): passgen.cpp
	$(CC) $(CFLAGS) passgen.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
