import machine
import network
import picoweb
import time
import gc

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

# Picoweb instance
app = picoweb.WebApp(__name__)

# Parse a query string into a dictionary with key/value pairs
def parse_query(qs):
  if len(qs) > 0:
    return dict([pair.split('=', 1) if '=' in pair else (pair, None)
                  for pair in qs.split('&')])
  else:
    return {}

# Home page
@app.route("/")
def index(req, resp):
  yield from app.sendfile(resp, 'static/index.html', 'text/html')
  yield from resp.aclose()

# Update light
@app.route("/light")
def light(req, resp):
  query = parse_query(req.qs)
  if 'level' in query:
    ledpwm.duty(int(query['level']))
  yield from picoweb.start_response(resp, content_type = "text/plain")
  yield from resp.awrite(str(ledpwm.duty()))
  yield from resp.aclose()

# Memory info
@app.route("/memory")
def light(req, resp):
  query = parse_query(req.qs)
  if 'gc' in query or 'collect' in query:
    gc.collect()
  yield from picoweb.start_response(resp, content_type = "text/plain")
  yield from resp.awrite(str(gc.mem_free()))
  yield from resp.aclose()

# Wait for network
while lan.ifconfig()[0] == '0.0.0.0':
  print("Waiting for network connection...")
  time.sleep(2)
print("Network connection established, starting Picoweb server.")

# Run Picoweb server
app.run(host=lan.ifconfig()[0], port=80)

