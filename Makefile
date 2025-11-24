IDIR = include
SDIR = src
ODIR = obj
TDIR = test

# Source files in src/
SRCS = $(wildcard $(SDIR)/*.cpp)
OBJS = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))
TESTS = $(wildcard $(TDIR)/*.cpp)
TOBJS = $(patsubst $(TDIR)/%.cpp,$(ODIR)/%.o,$(TESTS))

CC = g++
FLAGS=-I$(IDIR) -std=c++23 -Wall

all: btc tmain


# template rules to handle obj/ and obj compilation
$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.cpp | $(ODIR)
	$(CC) -c -o $@ $< $(FLAGS) $(LIBS)

$(TOBJS): $(TESTS) | $(ODIR)
	$(CC) -c -o $@ $< $(FLAGS) $(LIBS)

test_main.o: $(TDIR)/main.cpp | $(ODIR)
	$(CC) -c -o $@ $< $(FLAGS)  $(LIBS)

# rules for compiling final obj and exe's
btc: $(ODIR)/bt_compress.o $(OBJS) | $(ODIR)
	$(CC) -o $@ $^ $(FLAGS) $(LIBS)

# TODO: not quite sure how to handle compilation of test file,
# since it's kind of a competing main from the btc main
tmain: $(ODIR)/test_main.o $(TOBJS) | $(ODIR)
	$(CC) -o $@ $^ $(FLAGS) $(LIBS)

clean:
	rm -rf $(ODIR) btc tmain *~ core $(IDIR)/*~


.PHONY: all clean btc tmain
