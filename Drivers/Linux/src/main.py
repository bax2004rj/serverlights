from __future__ import print_function
import serial
import psutil
import sys, getopt

serialPoint = "/dev/ttyACM0"
delayTime = 0.1
fanDevation = 1000
temp1 = 70
temp2 = 100
tempChoiceTopLevel = "coretemp" # These two settings relate to which sensor to read from. It is given in a category and subcategory format.
tempChoiceLowerLevel = "Physical id 0"
fanChoiceTopLevel = "" # Similar to temperature sensor system, but acpi is required, so the code isn't butthurt if empty
fanChoiceLowerLevel = "" 

def readFanSpeed():
    fans = psutil.sensors_temperatures()
    if not fans or fans == "{}":
        print("Error: Fan reading unsupported!")
    try: # Masterclass in nested statements!
        for name, entries in fans.items(): #Search for toplevel
            if name == fanChoiceTopLevel: #Toplevel found
                for entry in entries: #Search for lower level
                    if entry == fanChoiceTopLevel: #Lower level found!
                        return entry.current
    except Exception:
        print("Error: Fan reading unsupported!")

def main(argv):
    arduino = serial.Serial(serialPoint,9600)
    print("Serial port open!")
    print("Daemon started!")
    arduino.write(b'b')
    while 1:
        # Read fan speed once and save for comparison later
        previousFanSpeed = readFanSpeed()
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
        temp = 0
        if not temps:
            print("Error: Temperature reading unsupported!")
        else:
            for name, entries in temps.items(): #Search for toplevel
                if name == tempChoiceTopLevel: #Toplevel found
                    for entry in entries: #Search for lower level
                        if entry == tempChoiceTopLevel: #Lower level found!
                            temp = entry.current
        if temp > temp1: #Temp too high
            arduino.write(b't')
        if temp > temp2:
            arduino.write(b'y')
        # Read fan speed
        fanNow = readFanSpeed()
        try:
            if(fanNow-previousFanSpeed>1000):
                arduino.write(b'f')
        except TypeError:
            arduino.write(b'd') ## Write fan warning flag to make it flash fan out light.

if __name__ == "__main__":
    main(sys.argv[1:])