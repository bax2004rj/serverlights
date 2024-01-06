import serial
import psutil
import sys, getopt
import time
import struct

serialPoint = "/dev/ttyACM0"
delayTime = 0.1
fanDevation = 1000
temp1 = 70
temp2 = 100
tempChoice = "coretemp"

def main(argv):
    arduino = serial.Serial(serialPoint,9600)
    print("Serial port open!")
    while 1:
        # Get CPU usage 
        cpuUsage = psutil.cpu_percent(delayTime)
        # Covert to PWM value
        cpuUsage *= 2.55
        cpuUsage = int(cpuUsage)
        # print("c",cpuUsage)
        arduino.write(b'c')
        arduino.write(str(cpuUsage).encode())
        # Read system temp
        temps = psutil.sensors_temperatures()
        


if __name__ == "__main__":
    main(sys.argv[1:])