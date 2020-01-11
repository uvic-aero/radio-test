import RPi.GPIO as GPIO
import time

# BCM or BOARD refer to the different pin references
GPIO.setmode(GPIO.BCM)
GPIO.setup(21, GPIO.OUT)
GPIO.setup(20, GPIO.OUT)
GPIO.setup(16, GPIO.OUT)
GPIO.setup(26, GPIO.OUT)
GPIO.setup(19, GPIO.OUT)
GPIO.setup(13, GPIO.OUT)

#Set the two PWM lines
m1pwm = GPIO.PWM(21,20000)
m2pwm = GPIO.PWM(26,20000)

m1pwm.start(10) #10% duty cycle
m2pwm.start(10)

m1a1 = GPIO.output(19, GPIO.LOW)
m1a2 = GPIO.output(13, GPIO.HIGH)

m2a1 = GPIO.output(20, GPIO.LOW)
m2a2 = GPIO.output(16, GPIO.HIGH)

time.sleep(1)

# Turn motors off
m1pwm.ChangeDutyCycle(0)
m2pwm.ChangeDutyCycle(0)

# no longer using pins 
# make sure to call before program cancels
GPIO.cleanup()
