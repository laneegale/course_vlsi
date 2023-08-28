# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = build

# File names
MAIN = main
SRCS = main.cpp $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
HDRS = $(wildcard $(INCDIR)/*.h)

# Targets
all: $(MAIN)

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(MAIN) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HDRS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INCDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(MAIN)

.PHONY: all clean
