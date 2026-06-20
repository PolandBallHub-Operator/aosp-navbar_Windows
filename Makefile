CXX = x86_64-w64-mingw32-g++
CXXFLAGS = -O2 -mwindows -static -I./include
LIBS = -luser32 -lgdi32 -lcomctl32 -ldwmapi

SRC = src/main.cpp src/navbar.cpp src/statusbar.cpp src/wm.cpp src/settings.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = android_navbar.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)
