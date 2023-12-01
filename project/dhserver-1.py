from socket import *
from enum import Enum

class Option:
    def __init__(self, code, len, data) -> None:
        self.code = code
        self.len = len
        self.data = data
    
    def parse(data):
        res = {}
        i = 0
        dl = data.len()
        # while i < data.len() and data[i] == 0:
        #     i += 1
        while i < dl and data[i] != 0xff:
            code = data[i]
            l = data[i + 1]
            i += 2 + l
            res.update({code: Option(code, l, data[i - l, min(i, dl)])})
        return res

class MsgE(Enum):
    DISCOVER = {'op': bytes.fromhex('01')}
    OFFER = {'op': bytes.fromhex('02')}
    REQ = {'op': bytes.fromhex('01')}
    ACK = {'op': bytes.fromhex('02')}

common = {
     'htype' : bytes.fromhex('01'), # hrdw type : ethernet
     'hlen' : bytes.fromhex('06'), # mac len bytes
     'hops' : bytes.fromhex('00'), # idk
     'secs' : bytes.fromhex('0000'), # secs elasted not important
     'flags' : bytes.fromhex('0000'), # bootp flags not important
     'legacy': bytes.fromhex(f'{str([0] * 192)}'), # 192 zeroes for leagacy
     'magic_ck' : bytes.fromhex('63825363') # magic number
}

class Msg:
    def __init__(self, en, tr_id, c_ip, your_ip, s_ip, g_ip, c_h_addr, opt) -> None: # en: Msge, tr_id 4bytes, cl_ip_addr, your_ip ip to be given, s_ip server ip, g_ip gateway ip?, c_h_addr 16 bytes mac + 0, options
        self.en = en
        self.tr_id = tr_id
        self.c_ip = c_ip
        self.your_ip = your_ip
        self.s_ip = s_ip
        self.g_ip = g_ip
        self.c_h_addr = c_h_addr
        self.opt = opt
    
    def parse(data): # data bytes
        if common['hlen'] != data[2]:
            return None
        op = data[0]
        tr_id = data[4:8]
        c_ip = data[12:16]
        your_ip = data[16:20]
        s_ip = data[20:24]
        g_ip = data[24:28]
        c_h_addr = data[28:44]
        op = Option.parse(data[236:-1])
        return Msg()

DHCP_SERVER = ('', 67)
DHCP_CLIENT = ('255.255.255.255', 68)

# Create a UDP socket
s = socket(AF_INET, SOCK_DGRAM)

# Allow socket to broadcast messages
s.setsockopt(SOL_SOCKET, SO_BROADCAST, 1)

# Bind socket to the well-known port reserved for DHCP servers
s.bind(DHCP_SERVER)

# Recieve a UDP message
msg, addr = s.recvfrom(1024)

print(msg)

# Print the client's MAC Address from the DHCP header
print("Client's MAC Address is " + format(msg[28], 'x'), end = '')
for i in range(29, 34):
    print(":" + format(msg[i], 'x'), end = '')
print()

# Send a UDP message (Broadcast)
s.sendto(b'Hello World!', DHCP_CLIENT)


#options code length data
# op_msg_t = 0x350102
# op_sub_mask = 0x0104ffffff00 #255.255.255.0
# op_lease_t = 0x330400000708 # 30 min
# op_srvr_idnt = 0x3604 # 54 4, ip
# op_end = 0xff
