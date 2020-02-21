"""
PressyPress is designed to fully explore the compression capabilities
of 7za. It should go through every possible configuration and find what
command line options compress the best.

It's also to help me understand how the compression algorithms work.
"""

import os
import subprocess

def getSize(filename):
	retVal = 0
	try:
		retVal = size = os.path.getsize(filename)
		return retVal
	except:
		return retVal

compressedData = "data.7z"
def clean():
	try:
		os.remove(compressedData)
	except:
		pass

clean()
additionals = ["-mx=0", "-mx=1", "-mx=3", "-mx=5", "-mx=7", "-mx=9"]
args = ["7za.exe", a", compressedData, "./data/*"]
result = subprocess.check_output(args).decode("utf-8")
print(getSize("./data.7z"))

# https://sevenzip.osdn.jp/chm/cmdline/switches/method.htm
"""
-m{Parameters} : set compression Method
-mmt[N] : set number of CPU threads
This count? -stm{HexMask} : set CPU thread affinity mask (hexadecimal number)
This count? -v{Size}[b|k|m|g] : Create volumes

Can play around with:
	Archive format
		7z
		tar
		wim
		zip
	Compression level
		Store
		Fastest
		Fast
		Normal
		Maximum
		Ultra
	Compression method
		LZMA2
		LZMA
		PPMd
		BZip2
	Dictionary size
		64 KB
		1, 2, 3, 4, 68, 12, 16, 24, 32, 48, 64, 96,
		128, 192, 256, 384, 512, 768, 1024, 1536 MB
	Word size
		8, 12, 16, 24, 32, 48, 64, 96, 128, 192, 256, 273
	Solid Block size
		Non-solid
		1, 2, 4, 8, 16, 32, 64, 128, 256, 512 MB
		1, 2, 4, 8, 16, 32, 64 GB
		Solid
	Number of CPU threads
		1 - 16

	Can measure:
		Total output size
		Total time spent compressing
		Memory usage for Compressing
		Memory usage for Decompressing
"""
