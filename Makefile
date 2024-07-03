CXX = g++
CXXFLAGS = -std=c++20 -Ithirdparty

SRC_DIR = .
INCLUDE_DIR = thirdparty
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.cpp) thirdparty/tinyxml2.cpp
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)