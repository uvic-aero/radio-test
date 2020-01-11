import RPi.GPIO as GPIO
import time
import sys
from inputs import get_gamepad

class  Motor:
	def __init__(self, pwm_pin, a1_pin, a2_pin):
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

# BCM or BOARD refer to the different pin references
GPIO.setmode(GPIO.BCM)
driveR = Motor(21, 20, 16)
driveL = Motor(26, 19, 13)
try:
	while True:
		events = get_gamepad()
		for event in events:
			if event.code == "ABS_Y":
				driveL.set_power(-event.state*100/33000)
			elif event.code == "ABS_RY":
				driveR.set_power(-event.state*100/33000)
except KeyboardInterrupt:
	driveR.set_power(0)
	driveL.set_power(0)
	GPIO.cleanup()
	sys.exit(0)
	
# no longer using pins 
# make sure to call before program cancels
GPIO.cleanup()

