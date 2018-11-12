from flask import Flask, request, make_response
import json
app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def beacon_print():
  try:
    print (repr(request.json))
    return "OK"
  except:
    return make_response("Bad request", 400)
