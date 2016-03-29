text = "QTLVZRFNCVAEÅDMBTNÆVXLNCVZABPZXDWWBSHBÅFQNTZRATNYGCBUWFCRJØGROÆÆVATÅWÅQNTCMØTXEVWSÅEFMNØTCAMJDÆÆNNØVADNTZZEÆEJQDNØBKKTSÅBOVÆRZAWERAEÆYÆQNÅPRAEJGRESNØUQRXUAÆTÅOXQRVÅHCATSFQNØECXKWWBSABSBWREAHÆTNYGCKJZÅQSMSTTIOTFQRRØXLFXCGQRNSHOHRTWQRBWÅBIIRVWREAHQRNYFHPÅÅHQKØEVZUÅSBLKSSBZEWMØWEUSBEIÅSBBKJAVZOVTCASNCZZGJGTTIOTVAKJZÅQSTCMØTXOBMLEDVQTYSFRETETTIOTVAEÆFADLRUQROÆDVAEAOBBEÅEYEOÆDHÆRNCVBSACGQRNØRZGÆWDQRÆNFÆVNCWXAPUVCEÆARXAFKCAULSÅXAR"
alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ"
key = "MAJOR"
# Æ 198
# Ø 216
# Å 197
def decrypt():
    clear = ""
    for i in range(len(text)):
        k = ord(key[i%len(key)])
        c = ord(text[i])

        if c == 198:
            c = c - 170
        elif c == 216:
            c = c - 188
        elif c == 197:
            c = c - 168
        else:
            c = c - 64

        if k == 198:
            k = k - 170
        elif k == 216:
            k = k - 188
        elif k == 197:
            k = k - 168
        else:
            k = k - 64

        c = (c - k) % 29
        clear += alphabet[c]
    return clear

print(decrypt())
