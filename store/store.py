import serial
import json
import time
import sys

f = "data/{}.json".format(time.strftime("%Y%m%d-%H%M%S"))
input = '/dev/tty.usbmodem143101'

with serial.Serial(input, 9600) as ser:
    with open(f, "w") as w:
        print("writing to {}".format(f))
        while True:
            try:
                line = ser.readline().decode()
                d = line.split()
                w.write(json.dumps({
                    "t": time.time(),
                    "tmp": float(d[2][:-1]),
                }) + "\n")
                w.flush()
            except KeyboardInterrupt:
                print("written to {}".format(f))
                sys.exit(0)
