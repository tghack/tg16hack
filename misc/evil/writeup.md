### Writeup
Some of the packets have the evil bit set. See [here](https://tools.ietf.org/html/rfc3514).

We can filter these out using tshark:

```
$ tshark -r evil.pcapng -Y "ip[6:1]&0x80" -T fields -e data | tr -d '\n' > lol.raw
```

Then convert the hex values to ASCII:
```
$ python2
Python 2.7.11 (default, Dec  6 2015, 15:43:46) 
[GCC 5.2.0] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> data = open("lol.raw", "r").read()
>>> import binascii
>>> print binascii.unhexlify(data)
TG16{evil_Packets_everywhere_y0u_better_be_SCARED_y0}
>>>
```
