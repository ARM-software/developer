import face_recognition
import picamera
import numpy as np
import time
import requests
import subprocess

def ping_cloud():
    requests.post('https://maker.ifttt.com/trigger/unknown_person_detected/with/key/YOUR_API_KEY')

#function for triggering the face recognition
def find_face():
    # Get a reference to the Raspberry Pi camera.
    # If this fails, make sure you have a camera connected to the RPi and that you
    # enabled your camera in raspi-config and rebooted first.
    camera = picamera.PiCamera()
    camera.resolution = (320, 240)
    camera.rotation = 180
    output = np.empty((240, 320, 3), dtype=np.uint8)
    # Load a sample picture and learn how to recognize it.
    person_image = face_recognition.load_image_file("carlo_pic.jpg")
    person_face_encoding = face_recognition.face_encodings(person_image)[0]
    # Initialize some variables
    face_locations = []
    face_encodings = []
    while True:
        # Grab a single frame of video from the RPi camera as a numpy array
        camera.capture(output, format="rgb")

        # Find all the faces and face encodings in the current frame of video
        face_locations = face_recognition.face_locations(output)
        #print("Found {} faces in image.".format(len(face_locations)))
        face_encodings = face_recognition.face_encodings(output, face_locations)

        # Loop over each face found in the frame to see if it's someone we know.
        for face_encoding in face_encodings:
            # See if the face is a match for the known face(s)
            match = face_recognition.compare_faces([person_face_encoding], face_encoding)
            name = "<Unknown Person>"

            if match[0]:
                name = "Carlo"

           
            if name == "<Unknown Person>":
                ping_cloud()
                subprocess.call("sudo shutdown -h now",shell=True)#after warning the user shutdown
                time.sleep(60*5) #sleep for 5 minutes before sending another message to cloud
                break

while True:
    try:
        find_face()
    except:
        pass
