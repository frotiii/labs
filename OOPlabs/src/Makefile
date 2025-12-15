CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET = game

SRCS = main.cpp \
       game.cpp \
       MoveManager.cpp \
       FightManager.cpp \
       player.cpp \
       enemy.cpp \
       field.cpp \
       cell.cpp \
       hand.cpp \
       areadamage.cpp \
       directdamage.cpp \
       StoreGame.cpp \
       gamestate.cpp \
       StartGame.cpp \
       ConsoleRenderer.cpp 

       

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q $(OBJS) $(TARGET).exe 2>nul || rm -f $(OBJS) $(TARGET)
