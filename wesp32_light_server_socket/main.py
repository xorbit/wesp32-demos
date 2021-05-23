import machine
import network
import socket
import time

# Connect to LAN if not already done by boot.py
try:
  lan
except:
  lan = network.LAN(mdc = machine.Pin(16), mdio = machine.Pin(17),
                    power = None, phy_type = network.PHY_LAN8720, phy_addr=0)
  lan.active(1)

# Initialize light PWM, flash for 1/4 sec on boot
led = machine.Pin(23, machine.Pin.OUT)

# Wait for network
while lan.ifconfig()[0] == '0.0.0.0':
  print("Waiting for network connection...")
  time.sleep(2)
print("Network connection established, starting web server.")

# Web page HTML
def web_page():
  if led.value() == 1:
    gpio_state="ON"
  else:
    gpio_state="OFF"
  
  html = """<html><head> <title>ESP Web Server</title> <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,"> <style>html{font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;}
  h1{color: #0F3376; padding: 2vh;}p{font-size: 1.5rem;}.button{display: inline-block; background-color: #e7bd3b; border: none; 
  border-radius: 4px; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
  .button2{background-color: #4286f4;}</style></head><body> <h1>wESP32 Light Server</h1> 
  <p>GPIO state: <strong>""" + gpio_state + """</strong></p><p><a href="/?led=on"><button class="button">ON</button></a></p>
  <p><a href="/?led=off"><button class="button button2">OFF</button></a></p></body></html>"""
  return html

# Open a socket to listen to port 80
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 80))
s.listen(5)

# Main loop
while True:
  conn, addr = s.accept()
  print('Got a connection from %s' % str(addr))
  request = conn.recv(1024)
  request = str(request)
  print('Content = %s' % request)
  led_on = request.find('/?led=on')
  led_off = request.find('/?led=off')
  if led_on == 6:
    print('LED ON')
    led.value(1)
  if led_off == 6:
    print('LED OFF')
    led.value(0)
  response = web_page()
  conn.send('HTTP/1.1 200 OK\r\n')
  conn.send('Content-Type: text/html\r\n')
  conn.send('Connection: close\r\n\r\n')
  conn.sendall(response)
  conn.close()

