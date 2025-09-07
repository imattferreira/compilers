CXX = g++
CXXFLAGS = -std=c++11 -Wall
INCLUDES = -ILexer/Headers/include
SRCDIR = Lexer/Sources
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/src/Lexer.cpp
TARGET = lexer_program

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)
