from flask import Flask, request
from flask_cors import CORS
import json

app = Flask(__name__)
CORS(app)
is_light = False

@app.route('/light/', methods=['POST'])
def light_switch():
    f = open('light', 'r')
    status = f.read(1)
    f.close()

    f = open('light', 'w')
    if (status == '1'):
        f.write('0')
    else:
        f.write('1')
    f.close()
    return 'ok'

@app.route('/light/', methods=['GET'])
def light_status():
    f = open('light', 'r')
    status = f.read(1)
    f.close()
    return status

@app.route('/data/', methods=['POST'])
def update():
    data = {}
    for item in request.form:
        data[item] = request.form[item]
    f = open('temp', 'w')
    f.write(json.dumps(data))
    f.close()
    return data

@app.route('/data/', methods=['GET'])
def retrive():
    f = open('temp', 'r')
    data = f.readline()
    f.close()
    return data

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port='9321')
