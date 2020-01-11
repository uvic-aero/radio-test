from inputs import  get_gamepad

while True:
	events = get_gamepad()
	for event in events:
		if event.code == 'ABS_Y':
			print(event.state)
		elif event.code == 'ABS_RY':
			print(event.state)
