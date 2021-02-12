  .text
  .globl spl_reloc
  .type spl_reloc, @function
spl_reloc:
  // clear some global variables to avoid deadlock
  la t0, hart_lottery
  li t1, 0
  sw t1, 0(t0)
  la t0, available_harts_lock
  li t1, 1
  sw t1, 0(t0)
  // first argument is the current text base
  // second argument is the target text base
  // third argument is the length of the copy
  // assume address is aligned by 8
  // t0 holds the current size
  // t1 hold temporary data
  mv s0, a1
  li t0, 0
memcpy:
  bge t0, a2, jmp_to_reloc
  ld t1, 0(a0)
  sd t1, 0(a1)
  addi a0, a0, 0x8
  addi a1, a1, 0x8
  addi t0, t0, 0x8
  j memcpy
jmp_to_reloc:
  jalr x0, s0, 0x0
