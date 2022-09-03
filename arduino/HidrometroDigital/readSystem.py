import serial

arduino = serial.Serial('COM5', 115200)

while True:
    linha = str(arduino.readline())
    print(linha)

arduino.close()
