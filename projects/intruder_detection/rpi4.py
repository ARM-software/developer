#RPi 4 code
import serial
import timer
#Setting up the serial port
PORT = "/dev/ttyACM0"
BAUD = 115200
s = serial.Serial(PORT)
s.baudrate = BAUD
s.parity   = serial.PARITY_NONE
s.databits = serial.EIGHTBITS
s.stopbits = serial.STOPBITS_ONE
s.timeout  = 1
s.reset_input_buffer()
#function for triggering the face recognition
def find_face():
    #face recognition code
    #open camera
    #face.find()
    #if face_found.name() != "carlo":
    #    send cloud message to balena

while True:
    # Read serial uart data from Pico
    try:
        data = s.readline().decode('UTF-8')
        data = int(data)
        print("Receive data: {}".format(data))
    except:
        pass
    
    # Send serial uart data to Pico board
    '''
    if ledStatus != prevLedStatus:
        s.write(str(ledStatus).encode('UTF-8'))
        prevLedStatus = ledStatus
        '''
    
s.close()
