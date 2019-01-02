import json
import sys
from statistics import mean
import matplotlib.pyplot as plt

f = sys.argv[1]
d = []
th = 40

print("reading {}".format(f))
with open(f) as fd:
    for l in fd:
        d.append(json.loads(l))

print("Loaded {} lines".format(len(d)))
t = [i["tmp"] for i in d]
r = t[:3]
m = mean(r)
st = []
for i in range(2, len(t)):
    if (m - th) < t[i] < (m + th):
        r.append(t[i])
        m = mean(r[-3:])
        print(m, t[i])
    else:
        print("Err val {} mean {:.2f}".format(t[i], m))
        r.append(m)
plt.plot(range(len(r)), r)
plt.savefig(f[:-5] + ".png")
# plt.show()


