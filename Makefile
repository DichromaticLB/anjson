ARCH= X86_64
#ARCH= AARCH64

CXX_AARCH64 = aarch64-linux-gnu-g++
CXX_X86_64 = x86_64-linux-gnu-g++
CXX = $(CXX_$(ARCH))
CXXFLAGS =	-O2  -Wall -Wno-array-bounds -Wno-register\
	 -fmessage-length=0 -std=c++17 -I$(INCLUDE_DIR) -fPIC
LEX= flex
YACC= bison
BISONFLAGS= -d 
BISONSRC= syntax/anjson.y
BISONTGT= $(SRC_DIR)/anjson_parser.cpp 

BISONFLAGS2= -d 
BISONSRC2= syntax/xjson.y
BISONTGT2= $(SRC_DIR)/anjson_accessor.cpp

FLEXFLAGS= -Panj -+
FLEXSRC= syntax/anjson.l
FLEXTGT= $(SRC_DIR)/anjson_lexer.cpp

INSTALL_INCLUDE=/usr/include/
INSTALL_LIB=/usr/lib/

INCLUDE_DIR= include
SRC_DIR=src
OBJ_DIR=obj

SHARED= --shared -fPIC

MAINOBJ = main.o
_OBJ = anjson.o  anjson_lexer.o anjson_parser.o  anjson_streamworks.o anjson_accessor.o
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

TARGET =	libanjson.so
TARGETBIN = anjson
TARGETLIB = libanjson.a

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(FLEXTGT) $(BISONTGT) $(BISONTGT2)
	$(CXX) $(CXXFLAGS)  -c -o $@  $<

$(TARGET):	$(OBJ) 
	$(CXX) -o $(TARGET) $(SHARED) $(OBJ) 

$(TARGETBIN):	$(OBJ) $(OBJ_DIR)/$(MAINOBJ)
	$(CXX) -o $(TARGETBIN)  $(OBJ)  $(OBJ_DIR)/$(MAINOBJ)

$(TARGETLIB): $(OBJ) 
	$(AR) rcs $(TARGETLIB) $(OBJ)
	
$(FLEXTGT) : $(FLEXSRC) 
	$(LEX)   $(FLEXFLAGS) -o$(FLEXTGT)   $(FLEXSRC) 

$(BISONTGT): $(BISONSRC)
	$(YACC)  $(BISONFLAGS) -o $(BISONTGT) $(BISONSRC)	
	mv src/*hpp src/*hh include

$(BISONTGT2): $(BISONSRC2)
	$(YACC)  $(BISONFLAGS2) -o $(BISONTGT2) $(BISONSRC2)	

all:	$(TARGET)

exec: $(TARGETBIN)

static: $(TARGETLIB)

install: $(TARGET)
	mkdir -vp $(INSTALL_INCLUDE)anjson
	cp $(INCLUDE_DIR)/anjson.hpp $(INSTALL_INCLUDE)anjson
	cp $(TARGET) $(INSTALL_LIB)
	
clean:
	rm -f $(OBJ) $(OBJ_DIR)/$(MAINOBJ) $(TARGET) $(FLEXTGT) $(BISONTGT) $(TARGETBIN) \
	$(INCLUDE_DIR)/stack.hh $(INCLUDE_DIR)/anjson_parser.hpp $(INCLUDE_DIR)/anjson_accessor.hpp $(SRC_DIR)/anjson_accessor.cpp $(SRC_DIR)/stack.hh
