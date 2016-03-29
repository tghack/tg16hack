
# Write-up
We are stepping up the difficulty with a Vigenere-cipher. It is still a shift
cipher, but in addition uses a keyword to determine the shift for each letter
in the text. To break this cipher we need to find the length of the keyword.
For this we use [Kasiski examination] (https://en.wikipedia.org/wiki/Kasiski_examination):

```
$ python kasiski.py
TSF 1
TSF 456
SFZ 2
FZZ 3
ZZZ 4
ZZM 5
ZMN 6
MNP 7
NPS 8
PSI 9
PSI 499
SIT 10
ITB 11
ITB 96
TBX 12
TBX 75
BXH 13
BXH 76
XHP 14
XHP 77
HPS 15
HPS 78
PSM 16
PSM 184
PSM 387
PSM
```
We see we get three occurrences of "PSM" at positions 16, 184, 387. We then find
the [greatest common divisor] (https://en.wikipedia.org/wiki/Greatest_common_divisor)
of (184-16) and (387-184):

```
$ python gcd.py 168 203
7
```

We now have a good indication that the length of the keyword is 7. This means
we can split the ciphertext into 7 subtexts where every letter has been
shifted the same. Using frequency analysis and a little guesswork, we now
find the keyword:

```
ALBERTI
```

Knowing the keyword, we can easily decrypt the ciphertext:

```
THEVIGENERECIPHERISSIMPLEENOUGHTOBEAFIELDCIPHERIFITISUSEDINCONJUNCTIONWITHCIPHERDISKSTHECONFEDERATESTATESOFAMERICAFOREXAMPLEUSEDABRASSCIPHERDISKTOIMPLEMENTTHEVIGENERECIPHERDURINGTHEAMERICANCIVILWARTHECONFEDERACYSMESSAGESWEREFARFROMSECRETANDTHEUNIONREGULARLYCRACKEDTHEIRMESSAGESTHEVIGENERECIPHERISSIMPLEENOUGHTOBEAFIELDCIPHERIFITISUSEDINCONJUNCTIONWITHCIPHERDISKSTHECONFEDERATESTATESOFAMERICAFOREXAMPLEUSEDABRASSCIPHERDISKTOIMPLEMENTTHEVIGENERECIPHERDURINGTHEAMERICANCIVILWARTHECONFEDERACYSMESSAGESWEREFARFROMSECRETANDTHEUNIONREGULARLYCRACKEDTHEIRMESSAGESTHEFLAGISTRITHEMIUS
```

Flag is: TRITHEMIUS
