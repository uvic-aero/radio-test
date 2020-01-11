import RPi.GPIO as GPIO
import time
import sys
"""
class  Motor:
    def __init__(self, pwm_pin, a1_pin, a2_pin):
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(pwm_pin, GPIO.OUT)
        GPIO.setup(a1_pin, GPIO.OUT)
        GPIO.setup(a2_pin, GPIO.OUT)
        self.pwm = GPIO.PWM(pwm_pin, 20000)
        self.pwm.start(0)
        self.a1 = a1_pin
        self.a2 = a2_pin
    def set_power (self, speed):
        print(speed)
        if speed < 0:
                self.reverse(-speed)
        else:
                self.forward(speed)

    def forward (self, speed):
        if speed > 100:
                print("Max speed is 100", file=sys.stderr)
                speed = 100
        GPIO.output(self.a1, GPIO.LOW)
        GPIO.output(self.a2, GPIO.HIGH)
        self.pwm.ChangeDutyCycle(int(speed))
    def reverse(self, speed):
        if speed > 100:
                print("Max speed is 100", file=sys.stderr)
                speed = 100
        GPIO.output(self.a1, GPIO.HIGH)
        GPIO.output(self.a2, GPIO.LOW)
        self.pwm.ChangeDutyCycle(int(speed))
"""
def clean_up(motors = []):
    for motor in motors:
        motor.set_power(0)
    GPIO.cleanup()
    print("cleaned gpio")

# BCM or BOARD refer to the different pin references
#driveR = Motor(21, 20, 16)
#driveL = Motor(26, 19, 13)
# no longer using pins 
# make sure to call before program cancels

