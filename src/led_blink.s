ldr w0, mailbox_base 

  
ldr w3, buffer_values_ON 

ldr w4, buffer_values_OFF 

add w3, w3, #0x80008 

add w4, w4, #0x80008 

  
ldr w29, bit_31_mask 

ldr w28, bit_30_mask 


main_loop: 

write_loop: 

ldr w5, [w0, #0x38] 

ands w5, w5, w29 

b.ne write_loop 
 

str w3 [w0, #0x20] 


read_loop: 

ldr w5, [w0, #0x18] 

ands w5, w5, w28 

b.ne read_loop 


ldr w6 [w0] 


ldr x30, big_num_for_waiting 

delay_loop: 

subs x30, x30, #1 

b.ne delay_loop 


write_loop: 

ldr w5, [w0, #0x38] 

ands w5, w5, w29 

b.ne write_loop 

	 
str w4 [w0, #0x20] 


read_loop: 

ldr w5, [w0, #0x18] 

ands w5, w5, w28 

b.ne read_loop 

ldr w6 [w0] 
  

ldr x30, big_num_for_waiting 

delay_loop: 

subs x30, x30, #1 

b.ne delay_loop 

b main_loop 
  

buffer_values_ON: 

.int 32 

.int 0 

.int 0x38041 

.int 8 

.int 0 

.int 130 

.int 1 

.int 0 

buffer_values_OFF: 

.int 32 

.int 0 

.int 0x38041 

.int 8 

.int 0 

.int 130 

.int 0 

.int 0 


mailbox_base: 

.int 0x3F00B880 

bit_31_mask: 

.int 0x80000000 

bit_30_mask: 

.int 0x40000000 

big_num_for_waiting: 

.int 500000000 
