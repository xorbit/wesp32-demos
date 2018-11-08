load('api_config.js');
load('api_gpio.js');
load('api_shadow.js');

print("Running init.js");

let pin = Cfg.get('wesp32_light.pin');
GPIO.set_mode(pin, GPIO.MODE_OUTPUT);

// Device state - LED on/off status
let state = { on: false };

// Set up Shadow handler to synchronise device state with the shadow state
Shadow.addHandler(function(event, obj) {
  if (event === 'CONNECTED') {
    // Connected to shadow - report our current state.
    Shadow.update(0, state);
  } else if (event === 'UPDATE_DELTA') {
    // Got delta. Iterate over the delta keys, handle those we know about.
    for (let key in obj) {
      if (key === 'on') {
        // Shadow wants us to change local state - do it.
        state.on = obj.on;
        GPIO.set_mode(pin, GPIO.MODE_OUTPUT);
        GPIO.write(pin, state.on ? 1 : 0);
        print('LED on: ', state.on);
      } else {
        print('Dont know how to handle key', key);
      }
    }
    // Once we've done synchronising with the shadow, report our state.
    Shadow.update(0, state);
  }
});

