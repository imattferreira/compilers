CXX = g++
CXXFLAGS = -std=c++11 -Wall
INCLUDES = -ISources/Headers/include
SRCDIR = Sources
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/src/Lexer.cpp $(SRCDIR)/src/Parser.cpp $(SRCDIR)/src/AST.cpp
TARGET = lexer_program

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)
