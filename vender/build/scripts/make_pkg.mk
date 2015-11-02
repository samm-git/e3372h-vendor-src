# Created by Michael Yao/y00184236/2012-03-30
include $(BALONG_TOPDIR)/build/scripts/make_base.mk

# Packet Tool Configuration
PACK_HEAD_TOOL          :=$(BALONG_TOPDIR)/build/tools/utility/postlink/packHead/packHead.py
USBLOADER_TOOL          :=$(BALONG_TOPDIR)/build/tools/utility/postlink/merge/merge.py
K3V3_VRL_TOOL_PATH      :=$(BALONG_TOPDIR)/../thirdparty/tools/vrl_creater_for_k3v3
SEC_VRL_TOOL_PATH       :=$(OBB_PRODUCT_DELIVERY_DIR)/obj/vrl/vrl_creater_for_k3v3
VRL_XLOADER_TBL         :=$(BALONG_TOPDIR)/build/tools/vrl_creater_for_k3v3_xloader/xloader.tbl
VRL_RAW_XLOADER_FILE    :=$(OBB_ANDROID_DIR)/out/target/product/$(CFG_OS_ANDROID_PRODUCT_NAME)/xloader.img
VRL_ENC_XLOADER_FILE    :=$(OBB_PRODUCT_DELIVERY_DIR)/image/sec_xloader.img
OBJCPY = $(OBB_ANDROID_DIR)/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-objcopy
ifneq ($(strip $(OBB_CORE_NAME)),all)
PKG_DEPENDS             := $(OBB_CORE_NAME)_pkgs
else
PKG_DEPENDS             := acore_pkgs ccore_pkgs mcore_pkgs
endif
# rules
#*******************************************************************************
.PHONY:all $(PKG_DEPENDS)
all: $(PKG_DEPENDS)
	@echo do [$@]
sec_tool:
ifeq ($(strip $(CFG_OS_ANDROID_USE_K3V3_KERNEL)),YES)
	@echo rebuiling the secure signature tool
	$(Q)rm -rf $(SEC_VRL_TOOL_PATH)
	$(Q)mkdir -p $(SEC_VRL_TOOL_PATH)
	$(Q)-cp -rfp $(K3V3_VRL_TOOL_PATH)/* $(SEC_VRL_TOOL_PATH)/
	$(Q) cd $(SEC_VRL_TOOL_PATH)/utils/src/secure_boot_utils  && make clean && make
else
	@echo do nothing
endif

acore_pkgs:sec_tool
	@echo Do [$@] complete

ccore_pkgs:sec_tool
	@echo Do [$@] complete

mcore_pkgs:sec_tool
	@echo Do [$@] complete

# clean
.PHONY: clean
clean:
	$(Q)-rm -rf $(SEC_VRL_TOOL_PATH)
	$(Q)-rm -f $(VRL_ENC_XLOADER_FILE)
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/dt.img
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/lpm3.img
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/sensorhub.img
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/trustedcore.img
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/xloader.img
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/boot.img
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/bsp_mcore.bin
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/balong_modem.bin
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/lib/usbloader.bin
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/recovery.img
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/fastboot.img
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/system.img
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/userdata.img
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/ptable.img
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/lib/onchip.img
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/ptable.bin
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/lib/fastboot.img.elf
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/lib/vmlinux
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/lphy.bin
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/nv.bin
	$(Q)-rm -f $(OBB_PRODUCT_DELIVERY_DIR)/image/ucos.bin
