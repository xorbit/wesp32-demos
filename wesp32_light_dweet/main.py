import machine
import time
import urequests

# Initialize light PWM, flash for 1/4 sec on boot
led = machine.Pin(23, machine.Pin.OUT)
ledpwm = machine.PWM(led)
ledpwm.freq(500)
time.sleep(0.25)
ledpwm.duty(0)

# Get the ESP32 unique ID as a string
uid = ''.join(['{:02x}'.format(x) for x in machine.unique_id()])

# Check dweet.io every two seconds to get and update the light level
while True:
  try:
    light_level = int(urequests.get(
        'https://dweet.io/get/latest/dweet/for/wesp32-light-demo-{}'
        .format(uid))
        .json()['with'][0]['content']['light'])
    ledpwm.duty(light_level)
    print ("Light level {}".format(light_level))
  except:
    pass
  time.sleep(2)

