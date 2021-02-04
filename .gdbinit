target extended-remote localhost:3333
# set debug remote 1
file spl/u-boot-spl
#file u-boot
load
layout split
set $pc = 0x80000000
#b spl_init
#b ofnode_read_prop
#b spl_boot_device
#b spl_ll_find_loader
#b spl_mmc_load_image
#b spl_mmc_find_device
#b find_mmc_device
#b common/spl/spl_mmc.c:156
b drivers/mmc/mmc.c:3054
b drivers/core/device.c:408
#b drivers/core/device.c:459
continue
#restore spl/u-boot-spl.bin binary 0x80000000
