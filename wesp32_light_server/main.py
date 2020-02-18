import machine
import network
import time
import gc
from microdot import Microdot, send_file

# Connect to LAN if not already done by boot.py
try:
  lan
except:
  lan = network.LAN(mdc = machine.Pin(16), mdio = machine.Pin(17),
                    power = None, phy_type = network.PHY_LAN8720, phy_addr=0)
  lan.active(1)

# Initialize light PWM, flash for 1/4 sec on boot
led = machine.Pin(23, machine.Pin.OUT)
ledpwm = machine.PWM(led)
ledpwm.freq(500)
time.sleep(0.25)
ledpwm.duty(0)

# Microdot instance
app = Microdot()

# Home page
@app.route("/")
def index(request):
  return send_file('static/index.html')

# Logo image
@app.route("/static/wesp32-logo.jpg")
def logo(request):
  return send_file('static/wesp32-logo.jpg')

# Update light
@app.route("/light")
def light(request):
  if 'level' in request.args:
    ledpwm.duty(int(request.args['level']))
  return str(ledpwm.duty())

# Memory info
@app.route("/memory")
def light(request):
  if 'gc' in request.args or 'collect' in request.args:
    print ("Garbage collect")
    gc.collect()
  return str(gc.mem_free())

# Wait for network
while lan.ifconfig()[0] == '0.0.0.0':
  print("Waiting for network connection...")
  time.sleep(2)
print("Network connection established, starting Microdot server.")

# Run Microdot server
app.run(host=lan.ifconfig()[0], port=80)

