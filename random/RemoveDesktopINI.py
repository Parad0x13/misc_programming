import os
rootdir = "G:/Google Drive/"

for subdir, dirs, files in os.walk(rootdir):
	for file in files:
		proper = os.path.join(subdir, file)
		if file == "desktop.ini":
			print(proper)
			os.remove(proper)
