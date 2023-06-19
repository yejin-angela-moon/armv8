buffer: 
	.int 32
	.int 0
	.int 0x00038041
	.int 8
	.int 0x0
	.int 130
	.int 0x1
	.int 0x0

loop:
wait_not_full:
  
	ldr w0, [0x3f00b8b8] 
	and w0, w0, 0x80000000 
	cmp w0, #0
	b.ne, wait_not_full

	ldr w6, =buffer_address
	ldr w6, [w6]
	orr w6, w6, 0x8

	str w6, [0x3f00b8a0]

	mov x1, #50000000

wait_not_empty:
  ldr w2, [0x3f00b8b8]
  and w2, w2, 0x40000000
  cmp w0, #0
  b.ne, wait_not_empty

  ldr w6, [0x3f00b880]
  orr w6, w6, 0x8
  
  mov x1, #50000000
delay_loop:
	sub x1, x1, #1
	cmp x1, #0
	b delay_loop_not_done
delay_loop_done:
	b continue_after_delay
delay_loop_not_done:
	b delay_loop

continue_after_delay:
	ldr w7, [buffer, 20]
	eor w7, w7, 1
	str w7, [buffer, 20]

	b loop

buffer_address:
	.int buffer
