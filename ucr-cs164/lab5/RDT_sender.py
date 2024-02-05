from RDT3 import RDT3
from check import ip_checksum

s = RDT3()

print("\n\nScen 1")
print("1st\n")
s.sendto("pkg 1".encode(), ("", 3452))
print("2nd\n")
s.sendto("pkg 2".encode(), ("", 3452))
print("3rd\n")
s.sendto("pkg 3".encode(), ("", 3452))

print("\n\nScen 2")
print("1st\n")
s.sendto("pkg 1".encode(), ("", 3452))
print("2nd\n")
s.sendto("pkg 2".encode(), ("", 3452), checksum=ip_checksum("Wrong"))
print("3rd\n")
s.sendto("pkg 3".encode(), ("", 3452))

print("\n\nScen 3")
print("1st\n")
s.sendto("pkg 1".encode(), ("", 3452))
print("2nd\n")
s.sendto("pkg 2".encode(), ("", 3452))
print("3rd\n")
s.sendto("pkg 3".encode(), ("", 3452))
s.close()