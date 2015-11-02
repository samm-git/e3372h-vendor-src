# Created by Jeffery.zhai/199916 2012.03.21

# What are targets ?
$(warning The targets are $(MAKECMDGOALS).)

# include the define at the top
include $(BALONG_TOPDIR)/build/scripts/make_base.mk

# process
.PHONY:all ccore vxworks
all:  ccore
	@echo do [$@]
	
ccore : 
	$(MAKE)  -f makefile_ccore.mk
vxworks : 
	$(MAKE)  -f makefile_vxworks.mk
	
# clean
.PHONY: clean
clean:
ifeq ($(OBB_CORE_NAME),all)
	$(MAKE)  $(OBB_JOBS)  -f makefile_ccore.mk $@
else
#	$(MAKE)  $(OBB_JOBS)  -f makefile_$(OBB_CORE_NAME).mk $@
endif
	$(Q)echo "COMM_CODE clean complete."

