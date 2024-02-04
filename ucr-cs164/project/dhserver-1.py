from enum import Enum
import bisect
from time import time
from socket import *
import struct

h2b = bytes.fromhex

# print(gethostbyname(gethostname()))
LEASE_T = 60
LEASE_T_B = LEASE_T.to_bytes(4, "big")
SERVER_IP = 0xc0a80001 #"192.168.0.1"
SERVER_IP_B = SERVER_IP.to_bytes(4, "big")
SUBNET_MASK = 0xffffff00 #"255.255.255.0"
SUBNET_MASK_B = SUBNET_MASK.to_bytes(4, "big")
DHCP_SERVER = ('', 67)
DHCP_CLIENT = ('255.255.255.255', 68)

def niceip(data) -> str:
    return f'{data[0]}.{data[1]}.{data[2]}.{data[3]}'

class Option:
    def __init__(self, code, len, data):
        self.code = code
        self.len = len
        self.data = data

    def __repr__(self) -> str:
        return f'(code: {self.code}\t, len: {self.len}\t, data: {self.data})'
    
    def to_bytes(self):
        return self.code.to_bytes(1, "big") + self.len.to_bytes(1, "big") + self.data

    def fromhex(code, data):
        data = h2b(data)
        return Option.parse(h2b(code) + len(data).to_bytes(1, "big") + data)

    def parse(data):
        res = {}
        i = 0
        dl = len(data)
        # while i < data.len() and data[i] == 0:
        #     i += 1
        while i < dl and data[i] != 0xff:
            code = data[i]
            l = data[i + 1]
            i += 2 + l
            res.update({code: Option(code, l, data[i - l : min(i, dl)])})
        return res
    
    def dict_to_bytes(dict):
        res = b''
        for k in dict.keys():
            res += dict[k].to_bytes()
        return res
    
    def hexparse(code, data):
        return Option.parse(Option.fromhex(code, data))

class MsgE(Enum):
    DISCOVER = {'br': h2b('01'), 'opt': Option.fromhex('35', '01')}
    OFFER = {'br': h2b('02'), 'opt': Option.fromhex('35', '02')}
    REQ = {'br': h2b('01'), 'opt': Option.fromhex('35', '03')}
    ACK = {'br': h2b('02'), 'opt': Option.fromhex('35', '05')}

    def from_opt(opt):
        match opt.data[-1]:
            case 0x01:
                return MsgE.DISCOVER
            case 0x02:
                return MsgE.OFFER
            case 0x03:
                return MsgE.REQ
            case 0x05:
                return MsgE.ACK
            case _:
                raise Exception(f'Failed to create MsgE from {opt.data[-1]}')

class Msg():
    common = {
            'htype' : h2b('01'), # hrdw type : ethernet
            'hlen' : h2b('06'), # mac len bytes
            'hops' : h2b('00'), # idk
            'secs' : h2b('0000'), # secs elasted not important
            'flags' : h2b('0000'), # bootp flags not important
            'legacy': h2b('00' * 192), # 192 zeroes for leagacy
            'magic_ck' : h2b('63825363'), # magic number
            'padding' : h2b('00' * 29),
            'c_h_padding' : h2b('00' * 10),
            'opt_end': h2b('ff')
        }

    def __init__(self, tr_id, c_ip, your_ip, s_ip, g_ip, c_h_addr, opt): # en: Msge, tr_id 4bytes, cl_ip_addr, your_ip ip to be given, s_ip server ip, g_ip gateway ip?, c_h_addr 16 bytes mac + 0, options
        self.en = MsgE.from_opt(opt[0x35])
        self.tr_id = tr_id
        self.c_ip = c_ip
        self.your_ip = your_ip
        self.s_ip = s_ip
        self.g_ip = g_ip
        self.c_h_addr = c_h_addr
        self.opt = opt
    
    def __repr__(self) -> str:
        return (f'id: {self.tr_id}\n' +
          f'c ip: {niceip(self.c_ip)}\nyour ip: {niceip(self.your_ip)}\ns ip: {niceip(self.s_ip)}\ng ip: {niceip(self.g_ip)}\nmac?: {self.c_h_addr}\n' +
           f'{str(self.opt)}')

    def parse_req(data): # data bytes
        if MsgE.DISCOVER.value['br'] != data[0:1] and Msg.common['hlen'] != data[2:3] and Msg.common['magic_ck'] != data[236:240]:
            return None
        tr_id = data[4:8]
        c_ip = data[12:16]
        your_ip = data[16:20]
        s_ip = data[20:24]
        g_ip = data[24:28]
        c_h_addr = data[28:34]
        
        opt = Option.parse(data[240:-1])
        return Msg(tr_id, c_ip, your_ip, s_ip, g_ip, c_h_addr, opt)

    def msg2resp(self):
        return self.en.value['br'] + Msg.common['htype'] + Msg.common['hlen'] + Msg.common['hops'] + self.tr_id +\
            Msg.common['secs'] + Msg.common['flags'] +\
            self.c_ip + self.your_ip +\
            self.s_ip + self.g_ip +\
            self.c_h_addr + Msg.common['c_h_padding'] +\
            Msg.common['legacy'] +\
            Msg.common['magic_ck'] +\
            Option.dict_to_bytes(self.opt) + Msg.common['opt_end'] +\
            Msg.common['padding']

class DHCP:
    def __init__(self) -> None:
        self.ledger = {}
        self.ips = [0x0, 0x1] # ip ending in 0 and 1 are taken by default
        self.s = socket(AF_INET, SOCK_DGRAM)
        self.s.setsockopt(SOL_SOCKET, SO_BROADCAST, 1)
        self.s.bind(DHCP_SERVER)

    def get_ip_mac(self, m):
        mac = int.from_bytes(m, 'big')
        now = time()
        for m, (ip, t) in self.ledger.items():
            if t + LEASE_T < now:
                self.ledger.pop(m)
                self.ips.remove(ip)

        res = None
        if mac in self.ledger:
            ip, t = self.ledger[mac]
            self.ledger.update({mac: (ip, time())})
            res = ip
        else:
            newip = 0x2
            for ip in self.ips: # dont care if we run out
                if newip == ip:
                    newip += 1
            self.ledger.update({mac: (newip, time())})
            bisect.insort(self.ips, newip)
            res = newip
        return (SUBNET_MASK & SERVER_IP) | (~SUBNET_MASK & res)

    def proccess(self):
        msg, addr = self.s.recvfrom(1024)
        msg = Msg.parse_req(msg)

        resp = None
        ip = self.get_ip_mac(msg.c_h_addr)
        client, port = DHCP_CLIENT

        match msg.en:
            case MsgE.DISCOVER:
                msg.en = MsgE.OFFER
                msg.opt = msg.en.value['opt']
            case MsgE.REQ:
                if (0x32 in msg.opt.keys() and msg.opt[0x32].data == ip.to_bytes(4, 'big')) or (msg.c_ip == ip.to_bytes(4, 'big')):
                    msg.en = MsgE.ACK
                    msg.opt = msg.en.value['opt']
                else:
                    print(f'Cant assignn {ip},')
                    return
            case _:
                pass
        msg.your_ip = ip.to_bytes(4, 'big')        
        msg.s_ip = SERVER_IP_B
        msg.opt.update({0x01: Option(0x01, 0x04, SUBNET_MASK_B)})
        msg.opt.update({0x33: Option(0x33, 0x04, LEASE_T_B)})
        msg.opt.update({0x36: Option(0x36, 0x04, SERVER_IP_B)})
        if msg.c_ip == msg.your_ip:
            client = niceip(msg.your_ip)
        self.s.sendto(msg.msg2resp(), (client, port))
        

if __name__ == '__main__':
    dhcp = DHCP()
    while True:
        dhcp.proccess()