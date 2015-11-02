# Created by Jeffery.zhai/199916 2012.03.21

# include the define at the top
include $(BALONG_TOPDIR)/build/scripts/make_base.mk

# os_ccore define
OBC_LOCAL_CORE_NAME		?=
OBC_LOCAL_MOUDLE_NAME	?=

#CC_USER_FLAGS   ?= cc_flags_test   := -W:c:,-Xmismatch-warning=2 -Xclib-optim-off -Xansi  -Xcode-absolute-far -Xlocal-data-area-static-only -Xforce-declarations   -Xchar-signed -Xfp-fast -Xdollar-in-ident -ew1554,1551,1552,1086,1047,1547 -ei1516,1643,1604 -Xlint=0x08

OBC_LOCAL_SRC_DIR       := 
OBC_LOCAL_SRC_FILE      := $(BALONG_TOPDIR)/build/libs/os/$(OBC_LOCAL_CORE_NAME)/version.c \
                       $(BALONG_TOPDIR)/build/libs/os/$(OBC_LOCAL_CORE_NAME)/usrEntry.c
					   

OBC_LOCAL_OBJ_DIR  := 
OBC_LOCAL_OBJ_FILE :=

OBC_LOCAL_INC_DIR += $(OBC_LOCAL_PATH)/ \
	$(WIND_BASE)/target/h \
	$(WIND_BASE)/target/h/wrn/coreip \
	$(WIND_BASE)/target/config/comps/src \
	$(WIND_BASE)/target/src/drv \
	$(WIND_HOME)/components/windml-5.3/h \
	$(WIND_HOME)/components/webservices-1.7/h \
	$(WIND_HOME)/components/webcli-4.8/target/h \
	$(WIND_HOME)/components/opc-3.2/h \
	$(WIND_HOME)/components/dcom-2.3/h \
	$(WIND_HOME)/components/windml-5.3/osconfig/vxworks/src \
	$(WIND_HOME)/components/ip_net2-6.8/osconfig/vxworks/src \
	$(WIND_HOME)/components/windml-5.3/osconfig/vxworks/h \
	$(WIND_HOME)/components/ip_net2-6.8/osconfig/vxworks/h \
	$(WIND_BASE)/target/src/hwif \
	$(WIND_BASE)/target/src/hwif/h
	

# user target define for vx_works_config
PRJ_OS_PATH := $(BALONG_TOPDIR)/$(CFG_OS_PATH)/$(OBC_LOCAL_CORE_NAME)
PRJ_OS_DELIVERY_PATH := $(OBB_PRODUCT_DELIVERY_DIR)/os/$(OBC_LOCAL_CORE_NAME)

#PRJ_OS_VX_WPJ_FILE_NAME := BALONGV7R2_$(OBB_BUILD_TYPE).IMG.wpj
PRJ_OS_VX_WPJ_FILE_NAME := $(CFG_OS_IMG_FILE)
PRJ_OS_FILES := $(filter-out  %.wpj %.WPJ,$(shell ls $(PRJ_OS_PATH)))

PRJ_OS_DELIVERY_FILES := $(addprefix $(PRJ_OS_DELIVERY_PATH)/, $(PRJ_OS_FILES))
PRJ_OS_TARGET_FILES  := $(addprefix $(PRJ_OS_DELIVERY_PATH)/, linkSyms.c prjComps.h prjConfig.c prjParams.h)

PRJ_OS_VX_WPJ_FILE := $(PRJ_OS_PATH)/$(PRJ_OS_VX_WPJ_FILE_NAME)
PRJ_OS_DELIVERY_VX_WPJ_FILE   :=$(PRJ_OS_DELIVERY_PATH)/$(PRJ_OS_VX_WPJ_FILE_NAME)

PRJ_GEN_TCL_FILE              := $(WIND_BASE)/host/resource/hutils/tcl/configGen.tcl
PRJ_GEN_TCL_TOOL              := $(WIND_HOME)/workbench-3.2/foundation/x86-linux2/bin/tclsh

OBC_LOCAL_CLEAN_DIRS := $(PRJ_OS_DELIVERY_PATH) 

#include rules. must be droped at the bottom
include $(BALONG_TOPDIR)/build/scripts/rules/cc_vxworks6.8_rules.mk

#do user target . must be droped at the lower.
do_pre_build_before : $(PRJ_OS_TARGET_FILES) $(PRJ_OS_DELIVERY_FILES) prjfile
#do_pre_build_before : 
	@echo do[$@]
	
# TODO：这里可能存在多线程问题
$(PRJ_OS_TARGET_FILES) : prjfile
	@echo [prjfile] $@
prjfile : $(PRJ_OS_DELIVERY_VX_WPJ_FILE) $(PRJ_OS_DELIVERY_FILES) | $(PRJ_OS_DELIVERY_PATH)
	@echo [TCL] $@
	$(Q)$(PRJ_GEN_TCL_TOOL) "$(PRJ_GEN_TCL_FILE)" "$(PRJ_OS_DELIVERY_VX_WPJ_FILE)"

$(PRJ_OS_DELIVERY_VX_WPJ_FILE) : $(PRJ_OS_VX_WPJ_FILE) | $(PRJ_OS_DELIVERY_PATH)
	@echo [sed] $@
	$(Q)sed -e 's,$(CFG_OS_LIB),$(OS_LIB_PATH),g' "$(PRJ_OS_VX_WPJ_FILE)" > "$(PRJ_OS_DELIVERY_VX_WPJ_FILE)" 

$(PRJ_OS_DELIVERY_FILES) :$(PRJ_OS_DELIVERY_PATH)%:$(PRJ_OS_PATH)%  | $(PRJ_OS_DELIVERY_PATH)
	@echo [cp] $@
	$(Q)cp -f $(V) $? $@
	$(Q)chmod  777 $@


$(PRJ_OS_DELIVERY_PATH) :
	$(Q)mkdir -p $(PRJ_OS_DELIVERY_PATH)
	
	


