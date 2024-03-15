import requests
import serial
import datetime

def read_serial_line(ser):
    data = ser.readline().decode('utf-8').strip()
    success = len(data) >= 9
    return success, data, (datetime.datetime.now() - datetime.datetime(1970, 1, 1)).total_seconds()

if __name__ == '__main__':
    idMap = dict()
    lastIds = []
    ser = serial.Serial('COM5', 9600, timeout=1)
    while True:
        success, id, secs = read_serial_line(ser)
        if success:
            idMap[id] = secs
        activeIds = [x for x in idMap.keys() if (secs - idMap[x]) < 2]
        print(activeIds)
        if not set(lastIds) == set(activeIds):
            print(requests.post('http://localhost:8080/items', json=activeIds))
            lastIds = activeIds
