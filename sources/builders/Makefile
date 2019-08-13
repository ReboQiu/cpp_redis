TARGET=libsentinel.so
TYPE=so
TOP_DIR= ./
debug=0

include ../../common.mk
SRC_DIR  += ./sources/builders ./sources/core ./sources/misc ./sources/network

C_INC += -I $(THRIDPART_DIR) -I $(COMMON_DIR) -I ./sources/builders -I ./sources/core -I ./sources/misc -I ./sources/network

CXXFLAGS +=-fno-strict-aliasing -O2 -export-dynamic \
	-D_GNU_SOURCE -std=c++11
EXTRA_CXXFLAGS += -std=c++11

all:$(TARGET)
	cp -f $(TARGET) $(LIB_DIR)
	

