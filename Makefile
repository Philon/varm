CONFIG = ./config.mk
-include $(CONFIG)

COMPONENTS := u-boot linux busybox
CLEANS = $(patsubst %,clean_%,$(COMPONENTS))
MENUCONFIGS = $(patsubst %,%_menuconfig,$(COMPONENTS))

u-boot_defconfig = vexpress_ca9x4_defconfig
linux_defconfig = vexpress_defconfig
busybox_defconfig = defconfig

.PHONY: all clean $(COMPONENTS) $(CLEANS) $(MENUCONFIGS)

all: $(COMPONENTS)
	@echo $(CROSS_COMPILE)

$(COMPONENTS): %:$(ODIR)/%/.config
	$(MAKE) -C $* O=$(ODIR)/$* HOSTCFLAGS=$(HOSTCFLAGS) HOSTCXXFLAGS=$(HOSTCXXFLAGS) HOSTLDFLAGS=$(HOSTLDFLAGS) -j$(JOBS)

$(MENUCONFIGS): %_menuconfig:$(ODIR)/%/.config
	$(MAKE) -C $* O=$(ODIR)/$* menuconfig

$(ODIR)/%/.config:
	@mkdir -p $(dir $@)
	$(MAKE) -C $* O=$(ODIR)/$* $($*_defconfig)

clean: $(CLEANS)
	@echo $^
$(CLEANS): clean_%:
	-rm -rf $(ODIR)/$*