# Print the current LAN IP address and unique ID
import machine

# Get the ESP32 unique ID as a string
uid = ''.join(['{:02x}'.format(x) for x in machine.unique_id()])

# Print the IP address
print("IP address: {}".format(lan.ifconfig()[0]))
# Print the unique ID
print("Unique ID: {}".format(uid))
# Print dweet.io update URL
print("Dweet.io URL: https://dweet.io/dweet/for/wesp32-light-demo-{}?light=<value>".format(uid))
