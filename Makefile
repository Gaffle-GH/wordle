CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I include

BIN_DIR = bin
TARGET = $(BIN_DIR)/wordle

SRC_DIR = src
SRCS = $(SRC_DIR)/wordle.cc

$(TARGET): $(SRCS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

restart: 
	rm -f $(TARGET)
	$(MAKE) run

.PHONY: clean