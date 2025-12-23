CXX = g++
CXXFLAGS = -std=c++11 -Wall -I../allegro/include
LDFLAGS = -L../allegro/lib -lallegro_monolith

# Source files
SRCS = Attack.cpp \
       BandMember.cpp \
       Bomb.cpp \
       Circle.cpp \
       GameWindow.cpp \
       global.cpp \
       Level.cpp \
       Main.cpp \
       Menu.cpp \
       Slider.cpp \
       Stone.cpp \
       algif5/src/algif.cpp \
       algif5/src/bitmap.cpp \
       algif5/src/gif.cpp \
       algif5/src/lzw.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = ../Bomberman.exe

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug target
debug: CXXFLAGS += -g
debug: LDFLAGS = -L../allegro/lib -lallegro_monolith-debug
debug: all

clean:
	del /f $(subst /,\,$(OBJS)) $(TARGET)

run: all
	$(TARGET)

.PHONY: all clean run debug
