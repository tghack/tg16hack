# Write-up

I'm still expecting this to be very easy as it's one of the early ones, and since
Harold doesn't seem to know squat about stego.

A simple

```sh
$ strings harold2.jpg | grep TG16{
```

does not work this time, but we can view it in a pager and look for clues:

```sh
$ less harold2.jpg
```

Here we see something interesting. Especially this part here:

```
XP-<B1><F3>O<EE>^Sq^B<FC>F^T&<D2>`<82><C1><BD>-<AF>Z^Y<93><EF>^A^@<92>j^@<FC><D3><FD>^?<87><F9><9B>
 ֤=7<91>5<F5>"d^N[<B7><EE><E3>xr<B3><9B><BD>3<AE><C1>^WXs^U^HS1p<ED><BA><E9><FA>O^B<B9><A5>w<D5>TYmX
^R%<91><C6><E2>1uL_<9B><B9>v<E7><82>Ю<B9><93><EB>{NUy<EF>`^M<F3>ث<A0><8B>\JS<95><FB>F<BF>^XŠ<88><8
^B<9D><B0><C1>os<E5>ΔO<F3><9B><C5>9"<F5><D2><C3>^A<80><U+05F6>1_^T<AC><C9>^F<E3>^M/ߎv'<8E>l<F9><AD
<AD>cj^Fka^F^^V<BA><97><E2>C<FB>V^BCFZ<87>Y<95>T?<9C><F4><CD>o˘>^X<B6>[<E2>PG$E<EF><FB>a<87><8A>6T
<E
```

jk, lol


Okay, there was nothing in the top of the file and I'm not sifting through that wall of text.
Let's try strings again:

```sh
$ strings harold2.jpg
.               # shortened output for brevity
.
W6ZGTX'
pNfw
'rgW%x
flag.pngUT      # End of output here, lucky us, we didn't need to scroll through the text
```

Here we are lucky and can with some certainty say that there probably is a whole file stuffed
in here.
If I hadn't found this so quick, I would've started `grep`'ing for common words like "flag",
"secret", "code", "password" and such.

The question now is, what can we know from what we have learned?
If we trust that `flag.png` is a filename, then that file is probably wrapped up in something
else, or we wouldn't have had the filename. The simplest form of packaging I can think of is `zip`.

Let's try to unzip it:

```sh
$ unzip harold2.jpg
Archive:  harold2.jpg
warning [harold2.jpg]:  31849 extra bytes at beginning or within zipfile
  (attempting to process anyway)
    inflating: flag.png
```

Hooray! Looking at the `flag.png` gives us the secret message from Harold.


*How can this work? `harold2.jpg` is clearly not a zip-file!?*  
The program that does the unzip'ing just looks for the zip-headers,
some predefined sequence of characters that indicates the start of a
zip file, and then works from where it finds those, as we see in
the output.


Another, quicker way to see what a blob/file consists of is to run `binwalk` on it:

```sh
$  binwalk harold2.jpg

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
31849         0x7C69          Zip archive data, at least v2.0 to extract, compressed size: 13313,  uncompressed size: 14017, name: "flag.png"
45306         0xB0FA          End of Zip archive

```

From here we would know to `unzip` it.

Consult your package-manager (i.e. apt-get, dnf, yum, brew for osx) to get a hold of it.
Binwalk can do some crazy stuff. It is worth learning to use if you're planning to do
a lot of stego.


Flag is: TG16{i\_can\_has\_foods?}
