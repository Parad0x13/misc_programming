from zipfile import ZipFile

def extractZip(zip):
    zf = ZipFile(zip)
    names = zf.namelist()
    pwd = bytes(names[0], encoding = "utf8")[:-4]
    print("Password is {}".format(pwd))
    data = zf.read(names[0], pwd = pwd)
    with open(names[0], "wb") as new: new.write(data)
    return names[0]

index = 0
currentName = "37366.zip"
while True:
    print("Zip number {}".format(index))
    index += 1
    currentName = extractZip(currentName)
