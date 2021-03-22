# modify these 3 lines depending of what you want
CFLAGS 		:= -Wno-unused-command-line-argument# -Werror -Wall -Wextra
LDFLAGS		:=
LIBS 		:= -lsfml-graphics -lsfml-window -lsfml-system
# type of source files
# c or cpp (make sure to not have space after)
ifndef SRCEXT
	SRCEXT 		:= cpp
endif

# detect if compiler is gcc instead of clang. Not viewing for other compiler
# C
ifeq ($(SRCEXT), c)
	ifeq ($(CC), gcc)
		CC := gcc
	else
		CC := clang
	endif # C : clang or gcc
# C++
else
	ifeq ($(CXX), g++)
		CC := g++
	else
		CC := clang++
	endif # C++ : clang++ or g++
	CFLAGS += -std=c++17
endif

# executable name
ifdef PGNAME
	EXECUTABLE = $(PGNAME)
else
	EXECUTABLE 	:= program
endif # pgname

# program name location
ifndef OUT
	OUT := ./bin
else
	
endif # out

# compilation mode
ifdef DEBUG
	TARGETDIR = $(OUT)/debug
else
	TARGETDIR = $(OUT)/release
endif # debug

# final full executable location
TARGET 		:= $(TARGETDIR)/$(EXECUTABLE)

# .o location
ifndef BUILDDIR
	BUILDDIR 	:= ./build
endif

# source files location
ifndef SRCDIR
	SRCDIR 		:= ./src
endif
# header files location
ifndef INCDIR
	INCDIR 		:= ./include
endif

SOURCES 	:= $(shell find $(SRCDIR)/** -type f -name *.$(SRCEXT))

INCDIRS		:=
INCLIST		:=
BUILDLIST	:=
INC			:= -I $(INCDIR)

ifneq (, $(firstword $(wildcard $(INCDIR)/*)))
	INCDIRS 	:= $(shell find $(INCDIR)/** -name '*.h' -exec dirname {} \; | sort | uniq)
	INCLIST 	:= $(patsubst $(INCDIR)/%, -I $(INCDIR)/%, $(INCDIRS))
	BUILDLIST 	:= $(patsubst $(INCDIR)/%, $(BUILDDIR)/%, $(INCDIRS))
	INC 		:= -I $(INCLIST)
endif # incdir

ifneq ($(SRCDIR), $(INCDIR))
	INC += -I $(INCDIR)
endif


ifdef DEBUG
OBJECTS 	:= $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))

$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGETDIR)
	@echo "Linking..."
	@echo "  Linking $(TARGET)"
	$(CC) -g -o $(TARGET) $^ $(LIBS) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
ifdef BUILDLIST
	@mkdir -p $(BUILDLIST)
endif
	@echo "Compiling $<...";
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

else # RELEASE

$(TARGET):
	@mkdir -p $(TARGETDIR)
	@echo "Linking..."
	$(CC) $(INC) -o $(TARGET) $(SOURCES) $(LIBS) $(LDFLAGS)

endif #debug / release targets


clean:
	rm -f -r $(BUILDDIR)/** $(OUT)/**
	@echo "All objects removed"

.PHONY: clean