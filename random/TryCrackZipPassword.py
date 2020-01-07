from zipfile import ZipFile

def extractZip(zip, pwd):
    pwd = bytes(pwd, encoding = "utf8")
    try: zip.extractall(pwd = pwd)
    except: pass
    else: print("Password is {}".format(pwd))

print("Cracking...")
zip = ZipFile("6969.zip")
pwds = open("rockyou.txt", "rb").read()
for pwd in pwds: extractZip(zip, str(pwd))

print("Done")
