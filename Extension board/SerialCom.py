import RPi.GPIO as GPIO
import serial

# Set the GPIO mode
GPIO.setmode(GPIO.BCM)
lock=25
release=22
heatbed=6

flag=1

arduino = serial.Serial("/dev/ttyUSB0", 9600)

# Set the pin as an output
GPIO.setup(lock, GPIO.IN)
GPIO.setup(release, GPIO.IN)
GPIO.setup(heatbed, GPIO.IN)
try:    
    while True:
        if GPIO.input(lock) and flag==1:
            print("lock tool")
            cmd="1,100,"
            arduino.write(cmd.encode())
            flag=0
        if GPIO.input(release) and flag==1:
            print("unlock tool")
            cmd="1,180,"
            arduino.write(cmd.encode())
            flag=0
        if GPIO.input(heatbed) and flag==1:
            print("heatbed")   
            flag=0  

        if(GPIO.input(lock) ==False and GPIO.input(release) ==False and GPIO.input(heatbed) ==False):
            flag=1

        


finally:
    # Clean up the GPIO settings on program exit
    GPIO.cleanup()