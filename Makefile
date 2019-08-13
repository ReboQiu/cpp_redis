TARGET=libcpp_redis.a
TYPE=lib
TOP_DIR=$(abspath ../../../)
debug=0
CPP_REDIS=$(abspath ./)
PARENT_REDIS=$(abspath ../)
TACOPIE=$(abspath $(CPP_REDIS)/tacopie/)

SRC_DIR  += $(CPP_REDIS)/sources/builders $(CPP_REDIS)/sources/core $(CPP_REDIS)/sources/misc $(CPP_REDIS)/sources/network $(TACOPIE)/sources/network $(TACOPIE)/sources/utils $(TACOPIE)/sources/network/common $(TACOPIE)/sources/network/unix
include ${TOP_DIR}/common/common.mk

C_INC += -I $(THRIDPART_DIR) -I $(COMMON_DIR) -I $(CPP_REDIS)/tacopie/includes/ -I $(CPP_REDIS)/includes/

CXXFLAGS +=-fno-strict-aliasing -O2 -export-dynamic \
	-D_GNU_SOURCE -std=c++11 
#-D__CPP_REDIS_LOGGING_ENABLED
EXTRA_CXXFLAGS += -std=c++11

all:$(TARGET)
	cp -f $(TARGET) $(TOP_DIR)/common/lib/
	

