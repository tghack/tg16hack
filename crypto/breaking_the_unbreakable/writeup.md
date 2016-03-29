### Writeup

The task description mentions both OTP and LFSR. If you know anything about
OTPs, you should know that this is not truly an OTP as it would be
unbreakable. In this case, the key has been constructed by an LFSR which makes
it vulnerable to an attack. It is possible to reconstruct an LFSR of length n
if you know 2n-1 bits of the keystream. If you can reconstruct the LFSR used
for the encryption, you can generate the whole key and easily decrypt the flag.

This attack requires knowledge of some of the cleartext. Luckily, we know that
a flag is encrypted. This means the file starts with "TG", and this is all we
need to break the unbreakable.

OTPs only use XOR to encrypt. To get the start of the key, we need to XOR "TG"
with the first 16 encrypted bits. Our handy python-script does this for us:
```
$ python xor.py
bitarray('0010100100010001')
```

Now that we got the start of the key, we can use an algorithm called
Berlekamp-Massey to find the shortest LFSR that can construct the key we found.
There's an online calculator [here] (http://bma.bozhu.me/). This algorithm
expects the input reversed, so you have to flip the bit-order. Then the
algorithm spits out this polynomial:
```
x^8 + x^7 + x^5 + x^3 + 1
```

We construct the LFSR and create a keyfile:
```
$ ./lfsr
$ xxd -b keyfile
00000000: 00101001 00010001 11100011 00100001 00010100 01111100  )..!.|
00000006: 01111110 01000101 01100101 10011000 11111011 00101110  ~Ee...
0000000c: 10110101 10011100 10001101 01101110 01001111 01011011  ...nO[
00000012: 00100111 01011001 00011100 01111001 11100001 00011010  'Y.y..
00000018: 00110100 10000001 00011101 10010000 11111110 10110001  4.....
0000001e: 11101010 11001101
```

We can now XOR this keyfile with the encrypted flag and write the result to a
(hopefully) readable file:
```
$ ./decrypt
$ cat flag.clear
TG16{b23f5ee5d2a340f9}
```
