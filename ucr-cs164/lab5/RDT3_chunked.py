import socket
import sys
import time
from check import ip_checksum
import random


def chksum(data):
    return bytes([ord(val) for val in ip_checksum(data)])


def newsock(addr=(None, None)) -> None:
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        if addr[0] is None or addr[1] is None:
            return s
        s.bind((addr[0], addr[1]))
        return s
    except socket.error as err:
        print("Error creating socket\n", err)
        sys.exit()


def sendto(s, data, addr, timeout=0.1, error=None, bufsize=1024, seq=None):
    if not type(data) is bytes:
        raise "Data is not bytes"

    s.settimeout(timeout)
    if seq is None:
        pkt_seq = False
        chnk_size = bufsize - 3
        chunks = [data[i : i + chnk_size] for i in range(0, len(data), chnk_size)] + [b""]
    else:
        pkt_seq = seq
        chunks = [data]
    res_seq = not pkt_seq

    for chunk in chunks:
        while pkt_seq ^ res_seq:
            checksum = chksum(chunk.decode())
            if (not error is None) and random.random() > error:
                print("Transfer error\n")
                checksum = chksum("error")

            pkt = bytes([pkt_seq]) + chunk + checksum
            print(
                "Sent{ Seq: ",
                bytes([pkt_seq])[0],
                "\t, Data: ",
                chunk.decode(),
                "\t, Chksum:",
                checksum,
                "\t}\n",
            )
            s.sendto(pkt, addr)

            try:
                pkt_ack, _ = s.recvfrom(bufsize)

                if pkt_ack[-2:] != chksum("ACK") and pkt_ack[1:-2] != b"ACK":
                    continue

                res_seq = bool(pkt_ack[0])
                print("Recieved ack:\t", pkt_ack[0])

            except socket.timeout:
                print("Timeout\n")
                continue

        pkt_seq = not pkt_seq
    return pkt_seq


def recieve(s, bufsize=1024, error=None, chunked=False, seq=None):
    data, addr = (b"", None)
    chunk = b" "
    if seq is None:
        pkt_seq = False
    else:
        pkt_seq = seq
    res_seq = not pkt_seq
    crrpt = False
    while chunk != b"":
        while pkt_seq ^ res_seq or crrpt:
            crrpt = False
            data_rec, addr = s.recvfrom(bufsize)

            res_seq = bool(data_rec[0])
            chunk = data_rec[1:-2]
            print(chunk)
            crrpt = data_rec[-2:] != chksum(chunk.decode())
            print(
                "Recv from:\t",
                addr,
                "\n{ Seq: ",
                data_rec[0],
                "\t, Data:",
                chunk.decode(),
                "\t, Chksum Rec:",
                data_rec[-2:],
                "\t, Chksum Data:",
                chksum(chunk.decode()),
                "\t, Iscrrpt:",
                crrpt,
                "\t, Expect seq=",
                bytes([pkt_seq])[0],
                "\t}\n",
            )

            if (not error is None) and random.random() > error:
                time.sleep(0.2)
                print("Delay error\n")

            if not crrpt:
                pkt_ack = bytes([res_seq]) + b"ACK" + chksum("ACK")
                s.sendto(pkt_ack, addr)
                print("Sent ack:", bytes([res_seq]))

        data += chunk
        pkt_seq = not res_seq
        if not seq is None:
            break

    return data, pkt_seq
