from machine import Pin, Timer

def check_sensor(timer):
    global sensor
    if sensor.value() == 1:
        gp1.value(1)
    else:
        gp1.value(0)

#GP4 - 5v output
gp4 = Pin(4,Pin.OUT)
gp4.value(1)
#GP1 - output for LED
gp1= Pin(1,Pin.OUT)
#GP5 - input from sensor
sensor = Pin(5,Pin.IN)
tim = Timer()
tim.init(freq=1.5, mode=Timer.PERIODIC, callback=check_sensor)
