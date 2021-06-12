.. SPDX-License-Identifier: GPL-2.0+

Openpiton RISC-V SoC
====================

OpenPiton RISC-V SoC
--------------------
OpenPiton is an open source, manycore processor and research platform. It is a
tiled manycore framework scalable from one to 1/2 billion cores. It supports a
number of ISAs including RISC-V with its P-Mesh cache coherence protocol and
networks on chip. It is highly configurable in both core and uncore components.
OpenPiton has been verified in both ASIC and multiple Xilinx FPGA prototypes
running full-stack Debian linux.

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

3. make openpiton_riscv64_spl_defconfig
4. make

U-Boot
------

1. Add the RISC-V toolchain to your PATH.
2. Setup ARCH & cross compilation environment variable:

.. code-block:: none

   export CROSS_COMPILE=<riscv64 toolchain prefix>
   export ARCH=riscv

3. make openpiton_riscv64_defconfig
4. make


opensbi
-------

1. Add the RISC-V toolchain to your PATH.
2. Setup ARCH & cross compilation environment variable:

.. code-block:: none

   export CROSS_COMPILE=<riscv64 toolchain prefix>
   export ARCH=riscv

3. Go to OpenSBI directory
4. make PLATFORM=fpga/openpiton FW_PAYLOAD_PATH=<path to u-boot-nodtb.bin>


Using fw_payload.bin with linux
-------------------------------
Put the generated fw_payload.bin into the /boot directory on the root filesystem,
plug in the SD card, then flash the bitstream. Linux will boot automatically.

Booting
-------
Once you plugin the sdcard and power up, you should see the U-Boot prompt.

Sample Dual-core Debian boot log from OpenPiton
-----------------------------------------------

.. code-block:: none

