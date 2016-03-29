# Write-up

This is the first stego task and therefore is probably something very simple.

If you know the format of the flag, just open a text editor and search for TG16.  
Alternatively, use your shell foo like this
```sh
$ strings harold1.jpg | grep TG16{
TG16{abe234bef8}
```

or like this, if you're in that camp
```sh
$ strings harold1.jpg | sed -n 's/TG16{/&/p'
TG16{abe234bef8}
```

Flag is: TG16{abe234bef8}
