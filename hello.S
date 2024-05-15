  .text
  .global _start
_start:
  li    t0, 11
  or    t1, t0, x0
  snez  t1, t1
  mv    a0, t1
  ret
