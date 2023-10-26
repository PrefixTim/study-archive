import matplotlib.pyplot as plt
import subprocess

labels = ["simple", "simple-threaded", "mutex", "mutex-threaded", "atomic", "atomic-threaded"]
data = []
x = []
for i in range(1, 101):
    print(i, "%")
    data.append([int(x) for x in (subprocess.run(["./atomic", str(5), str(i*100)], capture_output=True).stdout.split())])
    x.append(i*100)

plt.plot(x, data)
plt.legend(labels)

plt.show()
plt.savefig('plot.png')