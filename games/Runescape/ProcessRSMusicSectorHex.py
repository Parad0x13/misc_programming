import zlib
import binascii

index = 0
with open("data", "r") as fh:
	data = fh.read()
	data = data.split("X")    # DB Browser for SQL uses prefix of X to indicate hex values, might be and SQL thing
	for clip in data:
		if len(clip) == 0: continue    # Just in case, also first will always be empty by default

		clip = ''.join(filter(str.isalnum, clip))    # Cleaner way to ensure non standard quotes get erased
		clip = binascii.unhexlify(clip)
		clip = clip[8:]
		bin = zlib.decompress(clip)

		with open("{}.ogg".format(index), "wb") as file: file.write(bin)
		index += 1

# It seems like the last output isn't really part of the song? Or it's metadata that won't play not sure

# Some experimenting has shown that this seems to be only 1 song, and the issue is I can't just
# concatenate each clip as it's it's own ogg file altogether. Looks like I have to properly figure
# out how to concatenate ogg files otherwise it won't work properly
# Actually, that doesn't seem to really matter all too much? It just skipps as though they are
# concurrent songs in a playlist... very odd
