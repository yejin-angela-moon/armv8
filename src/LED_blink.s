buffer_values_ON: 

.int 256 

.int 0 

.int 0x00038041 

.int 8 

.int 0 

.int 130 

.int 1 

.int 0 

buffer_values_OFF: 

.int 256 

.int 0 

.int 0x00038041 

.int 8 

.int 0 

.int 130 

.int 0 

.int 0 

  

ldr w0, status_address 

ldr w1, write_address 

ldr w2, read_address 

  

ldr w3, ON_address_shifted 

ldr w4, OFF_address_shifted 

  

ldr w29, bit_31_mask 

ldr w28, bit_30_mask 

  

  

main_loop: 

write_loop: 

ldr w5, [w0] 

ands w5, w5, w29 

b.ne write_loop 

	 

str w3 [w1] 

  

read_loop: 

ldr w5, [w0] 

ands w5, w5, w28 

b.ne read_loop 

  

ldr w6 [w2] 

  

ldr x30, big_num_for_waiting 

delay_loop: 

subs x30, x30, #1 

b.ne delay_loop 

  

write_loop: 

ldr w5, [w0] 

ands w5, w5, w29 

b.ne write_loop 

  

str w4 [w1] 

  

read_loop: 

ldr w5, [w0] 

ands w5, w5, w28 

b.ne read_loop 

  

ldr w6 [w2] 

  

ldr x30, big_num_for_waiting 

delay_loop: 

subs x30, x30, #1 

b.ne delay_loop 

  

b main_loop 

  

  

status_address: 

.int 0x3F00B898 

write_address: 

.int 0x3F00B8A0 

read_address: 

.int 0x3F00B880 

ON_address_shifted: 

.int 0x800008 

OFF_address_shifted: 

.int 0x800208 

bit_31_mask: 

.int 0x80000000 

bit_30_mask: 

.int 0x40000000 

big_num_for_waiting: 

.int 500000000 
