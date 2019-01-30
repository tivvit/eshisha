import serial
import sys
import json
import time
import requests
import struct

ser = serial.Serial(sys.argv[1], 9600)

def write_pid(pid_data):
    for i in pid_data:
        ser.write(struct.pack("f", i))


def read_pid():
    with open("pid.conf", "r") as fd:
        content = fd.read()
        return eval(content)

def main():
    pid_data = (0.0, 0.0, 0.0)
    last_checked = time.time()

    while True:
        data = ser.readline()
        requests.post('http://0.0.0.0:8888/', data={
            "message_type": "temperature",
            "message_text": data}
        )

        check_time = time.time()

        if check_time - last_checked >= 1.0:
            last_checked = check_time
            new_pid_data = read_pid()

            if new_pid_data != pid_data:
                pid_data = new_pid_data
                write_pid(pid_data)


if __name__ == "__main__":
    main()
