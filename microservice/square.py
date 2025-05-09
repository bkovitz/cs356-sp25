from flask import Flask, jsonify

app = Flask(__name__)

@app.route('/square/<int:number>', methods=['GET'])
def calculate_Square(number):
  result = number * number
  return jsonify({
    'input':number,
    'square':result
  })

if __name__ == '__main__':
  print('square calculator micro service')
  app.run(host='0.0.0.0', port=8080)