CXX = g++
CXXFLAGS = -g -Wall -std=c++11
LDFLAGS = -pthread

SRCS = main.cpp taskmanager.cpp task.cpp account.cpp addtask.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

TARGET = myprogram

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ -g -lcrypto




.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)
