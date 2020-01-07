from scapy.all import *

data = Ether(bytearray.fromhex("485d36f814c4d8cb8ac3232d0800450000287be7400080060000c0a801a3175e9f24e9a463dd716eb3174c56e6b7501007a878e80000"))
data.show()

for x in range(100):
    send(data)

#ip = IP(dst="158.69.30.150")
#tcp = ip / TCP(sport = 59573, dport = 25565)
#packet = tcp / Raw(data)
#packet.show()
#send(packet)
