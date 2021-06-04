.. SPDX-License-Identifier: GPL-2.0+

Openpiton RISC-V SoC
====================

OpenPiton RISC-V SoC
--------------------
OpenPiton is an open source, manycore processor and research platform. It is a tiled manycore framework scalable from one to 1/2 billion cores. It supports a number of ISAs including RISC-V with its P-Mesh cache coherence protocol and networks on chip. It is highly configurable in both core and uncore components. OpenPiton has been verified in both ASIC and multiple Xilinx FPGA prototypes running full-stack Debian linux.

RISCV-V Standard Bootflow
-------------------------
Currently, OpenPiton implements RISC-V standard bootflow in the following steps
mover.S -> u-boot-spl -> opensbi -> u-boot -> Linux
This board supports S-mode u-boot as well as M-mode SPL

Building OpenPition
---------------------
If you'd like to build OpenPiton, please go to OpenPiton github repo(at https://github.com/PrincetonUniversity/openpiton) to build from the latest changes

Building Images
---------------------------

SPL
---

1. Add the RISC-V toolchain to your PATH.
2. Setup ARCH & cross compilation environment variable:

.. code-block:: none

   export CROSS_COMPILE=<riscv64 toolchain prefix>
   export ARCH=riscv

3. make openpiton_riscv64_defconfig
4. make

U-Boot
------

1. Add the RISC-V toolchain to your PATH.
2. Setup ARCH & cross compilation environment variable:

.. code-block:: none

   export CROSS_COMPILE=<riscv64 toolchain prefix>
   export ARCH=riscv

3. make openpiton_riscv64_defconfig
4. make menuconfig, then change CONFIG_SYS_TEXT_BASE to 0x81020000
5. make


opensbi
-------

1. Add the RISC-V toolchain to your PATH.
2. Setup ARCH & cross compilation environment variable:

.. code-block:: none

   export CROSS_COMPILE=<riscv64 toolchain prefix>
   export ARCH=riscv

3. Go to OpenSBI directory
4. Edit platform/fpga/openpiton/config.mk, and change FW_TEXT_START to 0x81000000
5. make PLATFORM=fpga/openpiton FW_PAYLOAD_PATH=<path to u-boot-nodtb.bin>


Using fw_payload.bin with linux
-------------------------------
Put the generated fw_payload.bin into the /boot directory on the root filesystem, plug in the SD card, then flash the bitstream. Linux will boot automatically.

Booting
-------
Once you plugin the sdcard and power up, you should see the U-Boot prompt.

Sample Dual-core Debian boot log from OpenPiton
-----------------------------------------------

.. code-block:: none

  <debug_uart>
  spl_early_init
  Bound device uart@fff0c2c000 to root_driver
  Bound device sdhci@0xf000000000.blk to sdhci@0xf000000000
  Bound device sdhci@0xf000000000 to root_driver
  Bound device clint@fff1020000 to root_driver
  ofnode_read_u32_array: ranges: fdtdec_get_int_array: ranges
  get_prop_check_min_len: ranges
  fdtdec_get_addr_size_auto_parent: na=1, ns=0, fdtdec_get_addr_size_fixed: reg: addr=00000000x
  ofnode_read_u32_index: timebase-frequency: (not found)
  ofnode_read_u32_index: timebase-frequency: x (520835)
  Bound device riscv_timer to cpu@0
  Bound device cpu@0 to cpus
  fdtdec_get_addr_size_auto_parent: na=1, ns=0, fdtdec_get_addr_size_fixed: reg: addr=00000000x
  ofnode_read_u32_index: timebase-frequency: (not found)
  ofnode_read_u32_index: timebase-frequency: x (520835)
  Bound device cpu@1 to cpus
  Bound device cpus to root_driver
  ofnode_read_prop: riscv,isa: rv64imafdc
  ofnode_read_prop: riscv,isa: rv64imafdc
  ofnode_read_u32_index: clock-frequency: (not found)
  fdtdec_get_addr_size_auto_parent: na=2, ns=2, fdtdec_get_addr_size_fixed: reg: addr=00000000x
  sifive_clint clint@fff1020000: missing clocks or clock-frequency property; falling back on timebase-frequency
  fdtdec_get_addr_size_auto_parent: na=2, ns=2, fdtdec_get_addr_size_fixed: reg: addr=00000000x
  ofnode_read_u32_index: reg: x (0)
  ofnode_read_u32_index: reg: x (1)
  fdtdec_get_addr_size_auto_parent: na=2, ns=2, fdtdec_get_addr_size_fixed: reg: addr=00000000x
  ofnode_read_u32_index: reg-offset: (not found)
  ofnode_read_u32_index: reg-shift: x (0)
  ofnode_read_u32_index: reg-io-width: (not found)
  ofnode_read_u32_index: clock-frequency: x (66667000)
  hello world from uboot spl
   Class     Index  Probed  Driver                Name
  -----------------------------------------------------------
             10s  -2147434144  [  ]                       20s  root_driver
             10s  -2147433712  [  ]                       20s  |-- uart@fff0c2c000
             10s  -2147433776  [  ]                       20s  |-- sdhci@0xf000000000
             10s  -2147433528  [  ]                       20s  |   `-- sdhci@0xf000000000.blk
             10s  -2147433608  [  ]                       20s  |-- clint@fff1020000
             10s  -2147434112  [  ]                       20s  `-- cpus
             10s  -2147433552  [  ]                       20s      |-- cpu@0
             10s  -2147433608  [  ]                       20s      |   `-- riscv_timer
             10s  -2147433552  [  ]                       20s      `-- cpu@1
  >>SPL: board_init_r()
  using memory lx-lx for malloc()
  spl_init
  Trying to boot from MMC1
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  spl: mmc boot mode: fs
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  GPT: first_usable_lba: X last_usable_lba: X last lba: 22
  alloc_read_gpt_entries: count = 128 * 128 = 16384
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  part_get_info_efi: start 0x800, size 0x3b71800, name
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  EXT4 features COMPAT: 0000003c INCOMPAT: 00000242 RO_COMPAT: 0000007b
  EXT2 rev 1, inode_size 256, descriptor size 32
  ext4fs read 0 group descriptor (blkno 1 blkoff 0)
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  Iterate dir fw_payload.bin
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  iterate >.<
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  iterate >..<
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  iterate >lost+found<
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  iterate >fw_payload.bin<
  ext4fs read 1 group descriptor (blkno 1 blkoff 32)
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  mkimage signature not found - ih_magic = 50433
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  Jumping to U-Boot
  loaded - jumping to U-Boot...
  image entry point: 0x
  ofnode_read_u32_index: reg: x (0)
  ofnode_read_u32_index: reg: x (1)

  OpenSBI v0.9-5-gd06cb61
     ____                    _____ ____ _____
    / __ \                  / ____|  _ \_   _|
   | |  | |_ __   ___ _ __ | (___ | |_) || |
   | |  | | '_ \ / _ \ '_ \ \___ \|  _ < | |
   | |__| | |_) |  __/ | | |____) | |_) || |_
    \____/| .__/ \___|_| |_|_____/|____/_____|
          | |
          |_|

  Platform Name             : OPENPITON RISC-V
  Platform Features         : timer,mfdeleg
  Platform HART Count       : 3
  Firmware Base             : 0x81000000
  Firmware Size             : 104 KB
  Runtime SBI Version       : 0.2

  Domain0 Name              : root
  Domain0 Boot HART         : 0
  Domain0 HARTs             : 0*,1*,2*
  Domain0 Region00          : 0x0000000081000000-0x000000008101ffff ()
  Domain0 Region01          : 0x0000000000000000-0xffffffffffffffff (R,W,X)
  Domain0 Next Address      : 0x0000000081200000
  Domain0 Next Arg1         : 0x0000000082200000
  Domain0 Next Mode         : S-mode
  Domain0 SysReset          : yes

  Boot HART ID              : 0
  Boot HART Domain          : root
  Boot HART ISA             : rv64imafdcsu
  Boot HART Features        : scounteren,mcounteren
  Boot HART PMP Count       : 0
  Boot HART PMP Granularity : 0
  Boot HART PMP Address Bits: 0
  Boot HART MHPM Count      : 0
  Boot HART MHPM Count      : 0
  Boot HART MIDELEG         : 0x0000000000000222
  Boot HART MEDELEG         : 0x000000000000b109
  <debug_uart>
  initcall: 0000000081205cd0
  initcall: 000000008121f82c
  initcall: 000000008120992a
  initcall: 000000008120b2d4
  initcall: 0000000081205e92
  initcall: 0000000081205cea
  initcall: 0000000081205eda
  initcall: 0000000081205eaa
  initcall: 0000000081205eae
  initcall: 0000000081205e8c
  ofnode_read_bool: u-boot,dm-pre-reloc: true
  Looking for 'serial' at 1040, name uart@fff0c2c000
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  Found seq 0
  Bound device uart@fff0c2c000 to root_driver
  ofnode_read_bool: u-boot,dm-pre-reloc: true
  Looking for 'mmc' at 1872, name sdhci@0xf000000000
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  Not found
  Looking for 'mmc' at 1872, name sdhci@0xf000000000
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  Not found
  mmc_bind: alias ret=-2, devnum=-1
  Bound device sdhci@0xf000000000.blk to sdhci@0xf000000000
  Bound device sdhci@0xf000000000 to root_driver
  ofnode_read_bool: u-boot,dm-pre-reloc: true
  Looking for 'timer' at 2008, name clint@fff1020000
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  Not found
  Bound device clint@fff1020000 to root_driver
  initcall: 000000008120028a
  ofnode_read_u32_array: ranges: fdtdec_get_int_array: ranges
  get_prop_check_min_len: ranges
  ofnode_read_bool: u-boot,dm-pre-reloc: true
  Looking for 'cpu' at 336, name cpu@0
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  Not found
  ofnode_read_u32_index: timebase-frequency: (not found)
  ofnode_read_u32_index: timebase-frequency: 0x7f283 (520835)
  Bound device riscv_timer to cpu@0
  Bound device cpu@0 to cpus
  ofnode_read_bool: u-boot,dm-pre-reloc: true
  Looking for 'cpu' at 644, name cpu@1
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  Not found
  ofnode_read_u32_index: timebase-frequency: (not found)
  ofnode_read_u32_index: timebase-frequency: 0x7f283 (520835)
  Bound device cpu@1 to cpus
  Bound device cpus to root_driver
  Looking for highest alias id for 'cpu'
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  clk_get_by_index_tail: Node 'cpu@0', property 'clocks', failed to request CLK index 0: -2
  Looking for highest alias id for 'cpu'
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  clk_get_by_index_tail: Node 'cpu@1', property 'clocks', failed to request CLK index 0: -2
  ofnode_read_prop: riscv,isa: rv64imafdc
  initcall: 0000000081221336
  initcall: 0000000081213d8c
  env_init: Environment <NULL> init done (ret=0)
  initcall: 0000000081205e68
  initcall: 00000000812132fe
  OF: ** translation for device uart@fff0c2c000 **
  OF: bus is default (na=2, ns=2) on
  OF: translating address: ff000000 00c0c2f0
  OF: reached root node
  ofnode_read_u32_index: reg-offset: (not found)
  ofnode_read_u32_index: reg-shift: 0x0 (0)
  ofnode_read_u32_index: reg-io-width: (not found)
  clk_get_by_index_tail: Node 'uart@fff0c2c000', property 'clocks', failed to request CLK index 0: -2
  ofnode_read_u32_index: clock-frequency: 0x3f941f8 (66667000)
  initcall: 0000000081220592


  U-Boot 2021.01-g17d45f5d-dirty (Mar 14 2021 - 20:31:58 +0800)

  initcall: 0000000081205db8
  U-Boot code: 81200000 -> 8123F0C0  BSS: -> 81241D10
  initcall: 0000000081205cfe
  initcall: 0000000081205e50
  DRAM:  initcall: 0000000081200318
  fdtdec_setup_mem_size_base: Initial DRAM size 40000000
  initcall: 0000000081205fca
  Monitor len: 00041D10
  Ram size: 40000000
  Ram top: C0000000
  initcall: 0000000081205cee
  initcall: 0000000081205eb2
  initcall: 0000000081205eb6
  initcall: 0000000081205eba
  initcall: 0000000081205d6e
  Reserving 263k for U-Boot at: bffbe000
  initcall: 0000000081205f2a
  Reserving 8316k for malloc() at: bf79f000
  initcall: 0000000081205ee2
  Reserving 112 Bytes for Board Info at: bf79ef90
  initcall: 0000000081205ebe
  initcall: 0000000081205d40
  Reserving 384 Bytes for Global Data at: bf79ee10
  initcall: 0000000081205ec2
  initcall: 0000000081205ec6
  initcall: 0000000081205eca
  initcall: 0000000081205ede
  initcall: 000000008120603a
  initcall: 000000008120031c
  fdtdec_setup_memory_banksize: DRAM Bank #0: start = 0x80000000, size = 0x40000000
  initcall: 0000000081205f5a

  RAM Configuration:
  Bank #0: 80000000 1 GiB

  DRAM:  1 GiB
  initcall: 0000000081206054
  initcall: 0000000081205d24
  New Stack Pointer is: bf79ee00
  initcall: 0000000081205ece
  initcall: 0000000081205ed2
  initcall: 0000000081205ed6
  initcall: 0000000081205de6
  Relocation Offset is: 3edbe000
  Relocating to bffbe000, new gd at bf79ee10, sp at bf79ee00
  initcall: 0000000081205ea6
  initcall: 0000000081205d02
  initcall: 00000000bffc40da
  initcall: 00000000bffc40de
  initcall: 00000000812060ee (relocated to 00000000bffc40ee)
  initcall: 0000000081206228 (relocated to 00000000bffc4228)
  initcall: 00000000812061f6 (relocated to 00000000bffc41f6)
  Pre-reloc malloc() used 0x990 bytes (2 KB)
  using memory 0xbf79f000-0xbffbe000 for malloc()
  initcall: 000000008120b2d4 (relocated to 00000000bffc92d4)
  initcall: 00000000812061e2 (relocated to 00000000bffc41e2)
  initcall: 000000008120622c (relocated to 00000000bffc422c)
  initcall: 0000000081206230 (relocated to 00000000bffc4230)
  initcall: 00000000812061cc (relocated to 00000000bffc41cc)
  clk_set_defaults()
  clk_set_default_parents: could not read assigned-clock-parents for 00000000bf79f090
  ofnode_read_prop: assigned-clock-rates: <not found>
  Looking for 'serial' at 1040, name uart@fff0c2c000
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  Found seq 0
  Bound device uart@fff0c2c000 to root_driver
  Looking for 'mmc' at 1872, name sdhci@0xf000000000
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  Not found
  Looking for 'mmc' at 1872, name sdhci@0xf000000000
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  Not found
  mmc_bind: alias ret=-2, devnum=-1
  Bound device sdhci@0xf000000000.blk to sdhci@0xf000000000
  Bound device sdhci@0xf000000000 to root_driver
  Looking for 'timer' at 2008, name clint@fff1020000
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  Not found
  Bound device clint@fff1020000 to root_driver
  initcall: 00000000812008b4 (relocated to 00000000bffbe8b4)
  initcall: 000000008120623c (relocated to 00000000bffc423c)
  initcall: 0000000081206234 (relocated to 00000000bffc4234)
  initcall: 000000008120a64e (relocated to 00000000bffc864e)
  initcall: 0000000081213400 (relocated to 00000000bffd1400)
  OF: ** translation for device uart@fff0c2c000 **
  OF: bus is default (na=2, ns=2) on
  OF: translating address: ff000000 00c0c2f0
  OF: reached root node
  ofnode_read_u32_index: reg-offset: (not found)
  ofnode_read_u32_index: reg-shift: 0x0 (0)
  ofnode_read_u32_index: reg-io-width: (not found)
  clk_get_by_index_tail: Node 'uart@fff0c2c000', property 'clocks', failed to request CLK index 0: -2
  ofnode_read_u32_index: clock-frequency: 0x3f941f8 (66667000)
  clk_set_defaults(uart@fff0c2c000)
  clk_set_default_parents: could not read assigned-clock-parents for 00000000bf79f180
  ofnode_read_prop: assigned-clock-rates: <not found>
  initcall: 00000000812061b0 (relocated to 00000000bffc41b0)
  Now running in RAM - U-Boot at: bffbe000
  initcall: 0000000081200316 (relocated to 00000000bffbe316)
  ofnode_read_u32_array: ranges: fdtdec_get_int_array: ranges
  get_prop_check_min_len: ranges
  Looking for 'cpu' at 336, name cpu@0
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  Not found
  ofnode_read_u32_index: timebase-frequency: (not found)
  ofnode_read_u32_index: timebase-frequency: 0x7f283 (520835)
  Bound device riscv_timer to cpu@0
  Bound device cpu@0 to cpus
  Looking for 'cpu' at 644, name cpu@1
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  Not found
  ofnode_read_u32_index: timebase-frequency: (not found)
  ofnode_read_u32_index: timebase-frequency: 0x7f283 (520835)
  Bound device cpu@1 to cpus
  Bound device cpus to root_driver
  clk_set_defaults(cpus)
  clk_set_default_parents: could not read assigned-clock-parents for 00000000bf79f860
  ofnode_read_prop: assigned-clock-rates: <not found>
  Looking for highest alias id for 'cpu'
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  clk_set_defaults(cpu@0)
  clk_set_default_parents: could not read assigned-clock-parents for 00000000bf79f930
  ofnode_read_prop: assigned-clock-rates: <not found>
  clk_get_by_index_tail: Node 'cpu@0', property 'clocks', failed to request CLK index 0: -2
  Looking for highest alias id for 'cpu'
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  clk_set_defaults(cpu@1)
  clk_set_default_parents: could not read assigned-clock-parents for 00000000bf79fac0
  ofnode_read_prop: assigned-clock-rates: <not found>
  clk_get_by_index_tail: Node 'cpu@1', property 'clocks', failed to request CLK index 0: -2
  initcall: 0000000081206238 (relocated to 00000000bffc4238)
  initcall: 0000000081206190 (relocated to 00000000bffc4190)
  MMC:   Looking for highest alias id for 'mmc'
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  clk_set_defaults(sdhci@0xf000000000)
  clk_set_default_parents: could not read assigned-clock-parents for 00000000bf79f2a0
  ofnode_read_prop: assigned-clock-rates: <not found>
  sdhci@0xf000000000: 0 (eMMC)
  initcall: 0000000081206136 (relocated to 00000000bffc4136)
  fdtdec_get_config_int: load-environment
  Using default environment
  Initial value for argc=3
  Final value for argc=3
  initcall: 0000000081206242 (relocated to 00000000bffc4242)
  initcall: 000000008120a666 (relocated to 00000000bffc8666)
  initcall: 0000000081206126 (relocated to 00000000bffc4126)
  initcall: 0000000081208990 (relocated to 00000000bffc6990)
  In:    uart@fff0c2c000
  Out:   uart@fff0c2c000
  Err:   uart@fff0c2c000
  Initial value for argc=3
  Final value for argc=3
  Initial value for argc=3
  Final value for argc=3
  Initial value for argc=3
  Final value for argc=3
  initcall: 0000000081200640 (relocated to 00000000bffbe640)
  initcall: 000000008120611c (relocated to 00000000bffc411c)
  fdtdec_get_config_int: bootdelay
  ### main_loop entered: bootdelay=-2

  fdtdec_get_config_int: kernel-offset
  fdtdec_get_config_int: rootdisk-offset
  fdtdec_get_config_string: bootcmd
  fdtdec_get_config_int: bootsecure
  ### main_loop: bootcmd="fdt addr ${fdtcontroladdr}; fdt move ${fdtcontroladdr} 0x86000000; ext4load mmc 0:1 0x80200000 Image; booti 0x80200000 - 0x86000000; "
  Initial value for argc=3
  Final value for argc=3
  Initial value for argc=3
  Final value for argc=3
  blk_get_devnum_by_typename: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  miss: start 0, count 1
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  fill: start 0, count 1
  part_init: try 'EFI': ret=0
  blk_get_devnum_by_typename: Device desc 00000000bf79f590
  miss: start 0, count 1
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  fill: start 0, count 1
  part_init: try 'EFI': ret=0
  hit: start 0, count 1
  miss: start 1, count 1
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  fill: start 1, count 1
  GPT: first_usable_lba: 22 last_usable_lba: 3B723DE last lba: 10000000
  alloc_read_gpt_entries: count = 128 * 128 = 16384
  miss: start 2, count 32
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  part_get_info_efi: start 0x800, size 0x3b71800, name
  miss: start 802, count 2
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  fill: start 802, count 2
  EXT4 features COMPAT: 0000003c INCOMPAT: 00000242 RO_COMPAT: 0000007b
  EXT2 rev 1, inode_size 256, descriptor size 32
  ext4fs read 0 group descriptor (blkno 1 blkoff 0)
  miss: start 808, count 1
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  fill: start 808, count 1
  miss: start bc8, count 1
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  fill: start bc8, count 1
  ofnode_read_prop: tick-timer: <not found>
  Looking for highest alias id for 'timer'
     - u-boot,dm-pre-reloc,
     - console, /uart@fff0c2c000
     - serial0, /uart@fff0c2c000
  clk_get_by_index_tail: Node 'clint@fff1020000', property 'clocks', failed to request CLK index 0: -2
  ofnode_read_u32_index: clock-frequency: (not found)
  clk_set_defaults(clint@fff1020000)
  clk_set_default_parents: could not read assigned-clock-parents for 00000000bf79f660
  ofnode_read_prop: assigned-clock-rates: <not found>
  OF: ** translation for device clint@fff1020000 **
  OF: bus is default (na=2, ns=2) on
  OF: translating address: ff000000 000002f1
  OF: reached root node
  sifive_clint clint@fff1020000: missing clocks or clock-frequency property; falling back on timebase-frequency
  Iterate dir Image
  miss: start cbc8, count 1
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  fill: start cbc8, count 1
  hit: start cbc8, count 1
  iterate >.<
  hit: start cbc8, count 1
  hit: start cbc8, count 1
  iterate >..<
  hit: start cbc8, count 1
  hit: start cbc8, count 1
  iterate >lost+found<
  hit: start cbc8, count 1
  hit: start cbc8, count 1
  iterate >fw_payload.bin<
  hit: start cbc8, count 1
  hit: start cbc8, count 1
  iterate >Image<
  ext4fs read 1 group descriptor (blkno 1 blkoff 32)
  hit: start 808, count 1
  miss: start 2081, count 1
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  fill: start 2081, count 1
  lmb_dump_all:
      memory.cnt             = 0x1
      memory.size            = 0x0
      memory.reg[0x0].base   = 0x80000000
                     .size   = 0x40000000

      reserved.cnt           = 0x0
      reserved.size          = 0x0
  Iterate dir Image
  hit: start cbc8, count 1
  hit: start cbc8, count 1
  iterate >.<
  hit: start cbc8, count 1
  hit: start cbc8, count 1
  iterate >..<
  hit: start cbc8, count 1
  hit: start cbc8, count 1
  iterate >lost+found<
  hit: start cbc8, count 1
  hit: start cbc8, count 1
  iterate >fw_payload.bin<
  hit: start cbc8, count 1
  hit: start cbc8, count 1
  iterate >Image<
  ext4fs read 1 group descriptor (blkno 1 blkoff 32)
  hit: start 808, count 1
  hit: start 2081, count 1
  miss: start bc800, count 12048
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  miss: start bf978, count 17
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  miss: start bf989, count 1
  blk_find_device: if_type=6, devnum=0: sdhci@0xf000000000.blk, 6, 0
  fill: start bf989, count 1
  6492992 bytes read in 5329 ms (1.2 MiB/s)
  Initial value for argc=3
  Final value for argc=3
  Initial value for argc=3
  Final value for argc=3
  *  kernel: cmdline image address = 0x80200000
  ## Skipping init Ramdisk
  ## No init Ramdisk
     ramdisk start = 0x00000000, ramdisk end = 0x00000000
  *  fdt: cmdline image address = 0x86000000
  ## Checking for 'FDT'/'FDT Image' at 86000000
  *  fdt: raw FDT blob
  ## Flattened Device Tree blob at 86000000
     Booting using the fdt blob at 0x86000000
     of_flat_tree at 0x86000000 size 0x00001dbb
  Initial value for argc=3
  Final value for argc=3
  ## initrd_high = 0xc0000000, copy_to_ram = 1
     ramdisk load start = 0x00000000, ramdisk load end = 0x00000000
  Initial value for argc=3
  Final value for argc=3
  Initial value for argc=3
  Final value for argc=3
  using: FDT
  ## device tree at 0000000086000000 ... 0000000086001dba (len=19899 [0x4DBB])
     Loading Device Tree to 00000000bfffb000, end 00000000bffffdba ... OK
  Initial value for argc=3
  Final value for argc=3
  ## Transferring control to kernel (at address 80200000) ...

  Starting kernel ...

  [    0.000000] OF: fdt: Ignoring memory range 0x80000000 - 0x80200000
  [    0.000000] Linux version 5.6.0-rc4-gb9d34f7e294d-dirty (eva@eva-virtual-machine) (gcc version 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)) #27 SMP Mon Mar 1 23:47:04 CST 2021
  [    0.000000] earlycon: sbi0 at I/O port 0x0 (options '')
  [    0.000000] printk: bootconsole [sbi0] enabled
  [    0.000000] Zone ranges:
  [    0.000000]   DMA32    [mem 0x0000000080200000-0x00000000bfffffff]
  [    0.000000]   Normal   empty
  [    0.000000] Movable zone start for each node
  [    0.000000] Early memory node ranges
  [    0.000000]   node   0: [mem 0x0000000080200000-0x00000000bfffffff]
  [    0.000000] Initmem setup node 0 [mem 0x0000000080200000-0x00000000bfffffff]
  [    0.000000] On node 0 totalpages: 261632
  [    0.000000]   DMA32 zone: 4088 pages used for memmap
  [    0.000000]   DMA32 zone: 0 pages reserved
  [    0.000000]   DMA32 zone: 261632 pages, LIFO batch:63
  [    0.000000] software IO TLB: mapped [mem 0xbaff7000-0xbeff7000] (64MB)
  [    0.000000] SBI specification v0.2 detected
  [    0.000000] SBI implementation ID=0x1 Version=0x9
  [    0.000000] SBI v0.2 TIME extension detected
  [    0.000000] SBI v0.2 IPI extension detected
  [    0.000000] SBI v0.2 RFENCE extension detected
  [    0.000000] SBI v0.2 HSM extension detected
  [    0.000000] elf_hwcap is 0x112d
  [    0.000000] percpu: Embedded 16 pages/cpu s25368 r8192 d31976 u65536
  [    0.000000] pcpu-alloc: s25368 r8192 d31976 u65536 alloc=16*4096
  [    0.000000] pcpu-alloc: [0] 0 [0] 1
  [    0.000000] Built 1 zonelists, mobility grouping on.  Total pages: 257544
  [    0.000000] Kernel command line: earlycon=sbi root=/dev/piton_sd1
  [    0.000000] Dentry cache hash table entries: 131072 (order: 8, 1048576 bytes, linear)
  [    0.000000] Inode-cache hash table entries: 65536 (order: 7, 524288 bytes, linear)
  [    0.000000] Sorting __ex_table...
  [    0.000000] mem auto-init: stack:off, heap alloc:off, heap free:off
  [    0.000000] Memory: 956188K/1046528K available (4357K kernel code, 286K rwdata, 1200K rodata, 168K init, 311K bss, 90340K reserved, 0K cma-reserved)
  [    0.000000] SLUB: HWalign=64, Order=0-3, MinObjects=0, CPUs=2, Nodes=1
  [    0.000000] rcu: Hierarchical RCU implementation.
  [    0.000000] rcu:     RCU restricting CPUs from NR_CPUS=8 to nr_cpu_ids=2.
  [    0.000000] rcu: RCU calculated value of scheduler-enlistment delay is 10 jiffies.
  [    0.000000] rcu: Adjusting geometry for rcu_fanout_leaf=16, nr_cpu_ids=2
  [    0.000000] NR_IRQS: 0, nr_irqs: 0, preallocated irqs: 0
  [    0.000000] plic: mapped 2 interrupts with 2 handlers for 4 contexts.
  [    0.000000] riscv_timer_init_dt: Registering clocksource cpuid [0] hartid [0]
  [    0.000000] clocksource: riscv_clocksource: mask: 0xffffffffffffffff max_cycles: 0x1ec037a6a, max_idle_ns: 7052723236599 ns
  [    0.000147] sched_clock: 64 bits at 520kHz, resolution 1919ns, wraps every 4398046510738ns
  [    0.009642] printk: console [hvc0] enabled
  [    0.009642] printk: console [hvc0] enabled
  [    0.018055] printk: bootconsole [sbi0] disabled
  [    0.018055] printk: bootconsole [sbi0] disabled
  [    0.028266] Calibrating delay loop (skipped), value calculated using timer frequency.. 1.04 BogoMIPS (lpj=5208)
  [    0.038993] pid_max: default: 32768 minimum: 301
  [    0.049869] Mount-cache hash table entries: 2048 (order: 2, 16384 bytes, linear)
  [    0.058262] Mountpoint-cache hash table entries: 2048 (order: 2, 16384 bytes, linear)
  [    0.069225] *** VALIDATE tmpfs ***
  [    0.089356] *** VALIDATE proc ***
  [    0.101548] *** VALIDATE cgroup ***
  [    0.105423] *** VALIDATE cgroup2 ***
  [    0.144623] rcu: Hierarchical SRCU implementation.
  [    0.164975] smp: Bringing up secondary CPUs ...
  [    0.194931] smp: Brought up 1 node, 2 CPUs
  [    0.216819] devtmpfs: initialized
  [    0.247264] random: get_random_u32 called from bucket_table_alloc.isra.25+0x4e/0x15c with crng_init=0
  [    0.267593] clocksource: jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 19112604462750000 ns
  [    0.278485] futex hash table entries: 512 (order: 3, 32768 bytes, linear)
  [    0.300550] NET: Registered protocol family 16
  [    0.834379] clocksource: Switched to clocksource riscv_clocksource
  [    0.842324] *** VALIDATE bpf ***
  [    0.856050] *** VALIDATE ramfs ***
  [    0.910981] NET: Registered protocol family 2
  [    0.937371] tcp_listen_portaddr_hash hash table entries: 512 (order: 1, 8192 bytes, linear)
  [    0.947904] TCP established hash table entries: 8192 (order: 4, 65536 bytes, linear)
  [    0.959472] TCP bind hash table entries: 8192 (order: 5, 131072 bytes, linear)
  [    0.972158] TCP: Hash tables configured (established 8192 bind 8192)
  [    0.986974] UDP hash table entries: 512 (order: 2, 16384 bytes, linear)
  [    0.995658] UDP-Lite hash table entries: 512 (order: 2, 16384 bytes, linear)
  [    1.010319] NET: Registered protocol family 1
  [    1.038492] RPC: Registered named UNIX socket transport module.
  [    1.045494] RPC: Registered udp transport module.
  [    1.050417] RPC: Registered tcp transport module.
  [    1.055833] RPC: Registered tcp NFSv4.1 backchannel transport module.
  [    1.079432] Initialise system trusted keyrings
  [    1.090919] workingset: timestamp_bits=46 max_order=18 bucket_order=0
  [    1.442287] *** VALIDATE nfs ***
  [    1.447483] *** VALIDATE nfs4 ***
  [    1.452686] NFS: Registering the id_resolver key type
  [    1.458989] Key type id_resolver registered
  [    1.464122] Key type id_legacy registered
  [    1.468760] nfs4filelayout_init: NFSv4 File Layout Driver Registering...
  [    1.476542] Installing knfsd (copyright (C) 1996 okir@monad.swb.de).
  [    1.979131] Key type asymmetric registered
  [    1.984371] Asymmetric key parser 'x509' registered
  [    1.990770] Block layer SCSI generic (bsg) driver version 0.4 loaded (major 254)
  [    1.999112] io scheduler mq-deadline registered
  [    2.004492] io scheduler kyber registered
  [    4.260627] Serial: 8250/16550 driver, 4 ports, IRQ sharing disabled
  [    4.312162] fff0c2c000.uart: ttyS0 at MMIO 0xfff0c2c000 (irq = 1, base_baud = 4166687) is a 16550
  [    4.659782] loop: module loaded
  [    4.663995] piton_sd:v1.0 Apr 26, 2019
  [    4.663995]
  [    4.670302] gpt partition table header:
  [    4.670398] signature: 5452415020494645
  [    4.674979] revision: 10000
  [    4.678967] size: 5c
  [    4.681839] crc_header: 680c3ba9
  [    4.684662] reserved: 0
  [    4.688037] current lba: 1
  [    4.690560] backup lda: 3b723ff
  [    4.693880] partition entries lba: 2
  [    4.697190] number partition entries: 80
  [    4.700843] size partition entries: 80
  [   10.126082]  piton_sd: piton_sd1
  [   10.786268] libphy: Fixed MDIO Bus: probed
  [   10.837802] NET: Registered protocol family 10
  [   10.874758] Segment Routing with IPv6
  [   10.880578] sit: IPv6, IPv4 and MPLS over IPv4 tunneling driver
  [   10.905701] NET: Registered protocol family 17
  [   10.918150] Key type dns_resolver registered
  [   10.932005] Loading compiled-in X.509 certificates
  [   11.147948] EXT4-fs (piton_sd1): mounted filesystem with ordered data mode. Opts: (null)
  [   11.158370] VFS: Mounted root (ext4 filesystem) readonly on device 254:1.
  [   11.233225] devtmpfs: mounted
  [   11.245265] Freeing unused kernel memory: 168K
  [   11.249963] This architecture does not have kernel memory protection.
  [   11.257188] Run /sbin/init as init process
  [   11.261430]   with arguments:
  [   11.265087]     /sbin/init
  [   11.267950]   with environment:
  [   11.271162]     HOME=/
  [   11.274453]     TERM=linux
  [   18.961333] systemd[1]: System time before build time, advancing clock.
  [   19.340844] systemd[1]: systemd 238 running in system mode. (+PAM +AUDIT +SELINUX +IMA +APPARMOR +SMACK +SYSVINIT +UTMP +LIBCRYPTSETUP +GCRYPT +GNUTLS +ACL +XZ +LZ4 -SECCOMP +BLKID +ELFUTILS +KMOD -IDN2 +IDN -PCRE2 defaul
  t-hierarchy=hybrid)
  [   19.370409] systemd[1]: Detected architecture riscv64.

  Welcome to Debian GNU/Linux buster/sid!

  [   19.587454] systemd[1]: Set hostname to <openpiton>.
  [   29.215526] random: systemd: uninitialized urandom read (16 bytes read)
  [   29.234429] systemd[1]: Listening on /dev/initctl Compatibility Named Pipe.
  [  OK  ] Listening on /dev/initctl Compatibility Named Pipe.
  [   29.272633] random: systemd: uninitialized urandom read (16 bytes read)
  [   29.392391] systemd[1]: Created slice system-serial\x2dgetty.slice.
  [  OK  ] Created slice system-serial\x2dgetty.slice.
  [   29.423151] random: systemd: uninitialized urandom read (16 bytes read)
  [   29.441241] systemd[1]: Started Dispatch Password Requests to Console Directory Watch.
  [  OK  ] Started Dispatch Password Requests to Console Directory Watch.
  [   29.526093] systemd[1]: Created slice system-getty.slice.
  [  OK  ] Created slice system-getty.slice.
  [   29.555742] systemd[1]: Reached target Swap.
  [  OK  ] Reached target Swap.
  [   29.593093] systemd[1]: Started Forward Password Requests to Wall Directory Watch.
  [  OK  ] Started Forward Password Requests to Wall Directory Watch.
  [   29.628869] systemd[1]: Reached target Local Encrypted Volumes.
  [  OK  ] Reached target Local Encrypted Volumes.
  [   29.677206] systemd[1]: Listening on Journal Socket.
  [  OK  ] Listening on Journal Socket.
  [   29.861203] systemd[1]: Starting Remount Root and Kernel File Systems...
           Starting Remount Root and Kernel File Systems...
  [   29.987658] systemd[1]: Listening on Journal Socket (/dev/log).
  [  OK  ] Listening on Journal Socket (/dev/log).
           Starting Load Kernel Modules...
           Mounting Kernel Debug File System...
  [  OK  ] Listening on udev Kernel Socket.
  [  OK  ] Reached target Paths.
           Starting Journal Service...
           Starting Create Static Device Nodes in /dev...
  [  OK  ] Reached target Remote File Systems.
  [  OK  ] Reached target Slices.
  [  OK  ] Listening on udev Control Socket.
           Starting udev Coldplug all Devices...
  [  OK  ] Reached target Sockets.
  [   34.481282] systemd[1]: Started Remount Root and Kernel File Systems.
  [  OK  ] Started Remount Root and Kernel File Systems.
  [   35.320761] systemd[1]: Started Load Kernel Modules.
  [  OK  ] Started Load Kernel Modules.
  [   35.438044] systemd[1]: Mounted Kernel Debug File System.
  [  OK  ] Mounted Kernel Debug File System.
  [   35.771088] systemd[1]: Started Create Static Device Nodes in /dev.
  [  OK  ] Started Create Static Device Nodes in /dev.
  [   36.367663] systemd[1]: Starting Apply Kernel Variables...
           Starting Apply Kernel Variables...
  [   39.671147] systemd[1]: Starting Load/Save Random Seed...
           Starting Load/Save Random Seed...
  [   39.737905] systemd[1]: Reached target Local File Systems (Pre).
  [  OK  ] Reached target Local File Systems (Pre).
  [   39.830756] systemd[1]: Reached target Local File Systems.
  [  OK  ] Reached target Local File Systems.
  [   40.432728] systemd[1]: Starting udev Kernel Device Manager...
           Starting udev Kernel Device Manager...
  [   40.551781] systemd[1]: Started Journal Service.
  [  OK  ] Started Journal Service.
  [  OK  ] Started Apply Kernel Variables.
           Starting Raise network interfaces...
           Starting Flush Journal to Persistent Storage...
  [  OK  ] Started Load/Save Random Seed.
  [  OK  ] Started udev Kernel Device Manager.
  [*     ] (1 of 4) A start job is running for&ersistent Storage (23s / 1min 44s)[   53.102639] systemd-journald[93]: Received request to flush runtime journal from PID 1
  [  OK  ] Started Flush Journal to Persistent Storage.
           Starting Create Volatile Files and Directories...
  [  OK  ] Started Raise network interfaces.
  [  OK  ] Reached target Network.
  [FAILED] Failed to start Create Volatile Files and Directories.
  See 'systemctl status systemd-tmpfiles-setup.service' for details.
           Starting Update UTMP about System Boot/Shutdown...
  [FAILED] Failed to start Network Time Synchronization.
  See 'systemctl status systemd-timesyncd.service' for details.
  [  OK  ] Reached target System Time Synchronized.
  [  OK  ] Stopped Network Time Synchronization.
  [FAILED] Failed to start Network Time Synchronization.
  See 'systemctl status systemd-timesyncd.service' for details.
  [  OK  ] Stopped Network Time Synchronization.
  [FAILED] Failed to start Network Time Synchronization.
  See 'systemctl status systemd-timesyncd.service' for details.
  [  OK  ] Stopped Network Time Synchronization.
  [FAILED] Failed to start Network Time Synchronization.
  See 'systemctl status systemd-timesyncd.service' for details.
  [  OK  ] Stopped Network Time Synchronization.
  [FAILED] Failed to start Network Time Synchronization.
  See 'systemctl status systemd-timesyncd.service' for details.
  [FAILED] Failed to start Update UTMP about System Boot/Shutdown.
  See 'systemctl status systemd-update-utmp.service' for details.
  [DEPEND] Dependency failed for Update UTMP about System Runlevel Changes.
  [  OK  ] Stopped Network Time Synchronization.
  [FAILED] Failed to start Network Time Synchronization.
  See 'systemctl status systemd-timesyncd.service' for details.
  [*     ] (2 of 2) A start job is running for&v-hvc0.device (2min 6s / 4min 33s)
  [  OK  ] Found device /dev/hvc0.
  [  OK  ] Started udev Coldplug all Devices.
  [  OK  ] Reached target System Initialization.
  [  OK  ] Started Daily apt download activities.
  [  OK  ] Reached target Basic System.
           Starting Permit User Sessions...
  [  OK  ] Started Daily apt upgrade and clean activities.
  [  OK  ] Started Daily Cleanup of Temporary Directories.
  [  OK  ] Reached target Timers.
  [  OK  ] Started Regular background program processing daemon.
  [  OK  ] Started Permit User Sessions.
  [  OK  ] Started Serial Getty on hvc0.
  [  OK  ] Reached target Login Prompts.
  [  OK  ] Reached target Multi-User System.
  [  OK  ] Reached target Graphical Interface.

  Debian GNU/Linux buster/sid openpiton hvc0

  openpiton login: openpiton
  Password:
  Linux openpiton 5.6.0-rc4-gb9d34f7e294d-dirty #27 SMP Mon Mar 1 23:47:04 CST 2021 riscv64

  The programs included with the Debian GNU/Linux system are free software;
  the exact distribution terms for each program are described in the
  individual files in /usr/share/doc/*/copyright.

  Debian GNU/Linux comes with ABSOLUTELY NO WARRANTY, to the extent
  permitted by applicable law.
  openpiton@openpiton:~$ lscpu
  Architecture:        riscv64
  Byte Order:          Little Endian
  CPU(s):              2
  On-line CPU(s) list: 0,1
  Thread(s) per core:  2
  Core(s) per socket:  1
  Socket(s):           1
  openpiton@openpiton:~$
  openpiton@openpiton:~$
