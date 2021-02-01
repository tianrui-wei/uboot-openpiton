target extended-remote localhost:3333
# set debug remote 1
file spl/u-boot-spl
load
layout split
set $pc = 0x80000000
#restore spl/u-boot-spl.bin binary 0x80000000
