import serial
import time
from keras.models import load_model
import cv2
import numpy as np

np.set_printoptions(suppress=True)

print("Connecting Bluetooth Device...")
port="COM8"
bluetooth=serial.Serial(port, 9600)
bluetooth.flushInput()
print("Bluetooth device connected")

model = load_model("converted_keras/keras_Model.h5", compile=False)
class_names = open("converted_keras/labels.txt", "r").readlines()

# CAMERA can be 0 or 1 based on default camera of your computer
camera = cv2.VideoCapture(0)

cv2.waitKey(0)
while True:
    # Grab the webcamera's image.
    ret, image = camera.read()

    # Resize the raw image into (224-height,224-width) pixels
    image = image[:, 80:560]
    image = cv2.resize(image, (224, 224), interpolation=cv2.INTER_AREA)
    image = cv2.flip(image, 1)

    # Show the image in a window
    cv2.imshow("Webcam Image", image)

    # Make the image a numpy array and reshape it to the models input shape.
    # Normalize the image array
    image = np.asarray(image, dtype=np.float32).reshape(1, 224, 224, 3)
    image = (image / 127.5) - 1

    # Predicts the model
    prediction = model.predict(image)
    index = np.argmax(prediction)
    class_name = class_names[index]
    confidence_score = prediction[0][index]

    # Print prediction and confidence score
    print("Class:", class_name[2:], end="")
    print("Confidence Score:", str(np.round(confidence_score * 100))[:-2], "%")

    # Listen to the keyboard for presses.
    keyboard_input = cv2.waitKey(1)

    # 27 is the ASCII for the esc key on your keyboard.
    if keyboard_input == 27:
        break

    bluetooth.write(str.encode(str(index)))
    input_data=bluetooth.readline()
    print(input_data.decode())

camera.release()
cv2.destroyAllWindows()

"""
def signal_handler(signal, frame):
	print("closing program")
	SerialPort.close()
	sys.exit(0)

COM=input("Enter the COM Port\n")
BAUD=input("Enter the Baudrate\n")
SerialPort = serial.Serial(COM,BAUD,timeout=1)
while (1):
	try:
		OutgoingData=input('> ')
		SerialPort.write(bytes(OutgoingData,'utf-8'))
	except KeyboardInterrupt:
		print("Closing and exiting the program")
		SerialPort.close()
		sys.exit(0)
	IncomingData=SerialPort.readline()
	if(IncomingData):
		print((IncomingData).decode('utf-8'))
	time.sleep(0.01)
"""

"""
serialcomm = serial.Serial('COM7', 38400, timeout=1)
if serialcomm.isOpen() == False:
    serialcomm.open
# serialcomm.port()
serialcomm.reset_input_buffer()

for x in range(50):
    print("a")
    serialcomm.write(bytes(str(x), 'utf-8'))
    inVar = serialcomm.read_until().decode()
    print("OUT: " + str(x) + " IN : " + str(inVar))
serialcomm.close()
"""