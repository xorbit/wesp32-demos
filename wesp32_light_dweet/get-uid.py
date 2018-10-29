# Print the unique ID
import machine

# Get the ESP32 unique ID as a string
uid = ''.join(['{:02x}'.format(x) for x in machine.unique_id()])

# Print unique ID
print(uid, end='')
