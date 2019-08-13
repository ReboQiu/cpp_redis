TARGET=libtacopie.a
TYPE=lib
TOP_DIR=$(abspath ../../../)
debug=0
TACOPIE=$(abspath ./)

SRC_DIR  += $(TACOPIE)/sources/network $(TACOPIE)/sources/utils $(TACOPIE)/sources/network/common $(TACOPIE)/sources/network/unix
include ${TOP_DIR}/common/common.mk

C_INC += -I $(THRIDPART_DIR) -I $(COMMON_DIR) -I $(TACOPIE)/includes/

CXXFLAGS +=-fno-strict-aliasing -O2 -export-dynamic \
	-D_GNU_SOURCE -std=c++11
EXTRA_CXXFLAGS += -std=c++11

all:$(TARGET)
	cp -f $(TARGET) $(TOP_DIR)/common/lib/
	

