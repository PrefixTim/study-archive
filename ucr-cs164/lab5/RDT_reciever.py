from RDT3 import RDT3

s = RDT3(("", 3452))

print("\n\nScen 1")
print(s.recieve().decode())
print(s.recieve().decode())
print(s.recieve().decode())

print("\n\nScen 2")
print(s.recieve().decode())
print(s.recieve().decode())
print(s.recieve().decode())

print("\n\nScen 3")
print(s.recieve().decode())
print(s.recieve(delay=2).decode())
print(s.recieve().decode())

s.close()