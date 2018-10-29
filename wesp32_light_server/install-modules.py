# Install script for dependencies using upip
import machine
import network
import time
import upip

# Connect to LAN if not already done by boot.py
try:
  lan
except:
  print("Starting network connection...")
  lan = network.LAN(mdc = machine.Pin(16), mdio = machine.Pin(17),
                    power = None, phy_type = network.PHY_LAN8720, phy_addr=0)
  lan.active(1)

# Wait for network
while lan.ifconfig()[0] == '0.0.0.0':
  print("Waiting for network connection...")
  time.sleep(2)

# Install picoweb
print("Network connection established, installing picoweb...")
upip.install('picoweb')
# Install micropython-logging
upip.install('micropython-logging')
print("Installation finished.")
