# Arm Portenta H7 TinyML Demo

## Description
This is a demo of a TinyML application running on the Arm Cortex-M7 processor on the Arduino / Arm Portenta H7 Board.

The TinyML application is the Arduino "person_detection" sketch, modified to run on the Portenta H7.

The "person_detection" sketch captures images from the camera on the H7's Vision Shield, and uses TinyML (TensorFLow Lite) to determine whether or not there is a person in the image.

The application has been enhanced to send the captured images to an external server via HTTP for viewing, and to optionally save the images to an SD Card.

As currently configured, the feature of sending the images to a server is enabled, and the feature of saving the images to an SD card is disabled.

Both of these features, as well as additional logging to the Portenta's serial console, may be enabled or disabled by modifying boolean values in the person_detection.ino sketch.


## Portenta H7 Person Detection Setup
This demo requires a Portenta H7 and a Portenta Vision Shield.

The Person Detection project is contained in this repo in the person_detection folder.

To run the Person Detection, you must first install the Arduino IDE.

Then, install the "Arduino mbed-enabled Boards" package in the Arduino IDE by opening the Tools -> Board -> Boards Manager menu, and searching for "Portenta". Select the "Arduino mbed-enabled Boards" package, version 1.3.2  

If you already have this board package installed, but it is an earlier version, be sure to update it to version 1.3.2
 
Now, in the person\_detection folder, copy the config.h.template file to a new file named config.h, and change the values of the WIFI_SSID and WIFI_PASS entries
to the correct values for your WiFi router's SSID and Password, and change the value of the SERVER_IP_ADDRESS to the IP address of the computer on which you will be running the image-server.
 
Then, you can open the person\_detection/person_detection.ino file in the Arduino IDE, and compile and upload the sketch to the board. Be sure to select the "Arduino Portenta H7 (M7 Core)" board in the Tools - Board menu and the proper port in the Tools -> Port menu (the port name will vary depending on your computer's operating system).

At this step, the sketch should run, but of course, will not connect to the external "image viewer" server, as that server is not running yet.

* Note that we have seen problems with the Portenta / Vision Shield connecting to a WiFi access point. This problem appears to be related to either the Portenta WiFi module's sensitivity or the antenna that is shipped with the board. We found that moving the board to within a few feet of the access point solves the connectivity problem.

It may also be advisable to connect a higher-gain antenna in place of the one shipped with the board.

## Image Viewer Setup
The image viewer consists of a Python server to which the Portenta person_detection sketch will send images and person detection scores via HTTP POST. The server then makes the images and associated information available via a web browser.

To run the image viewer,

* Make sure that you have Python 3 installed.

* Go to the image-server folder in this repo, and at the command-line: 

Create a virtual environment

    python3 -m venv venv

Activate

    . venv/bin/activate

Install requirements

    pip install -r requirements.txt

Run the server

    flask run --host=0.0.0.0
    
    
Once the image-server is running, re-start the Portenta, and it should start sending images to the image-server.

You can see the images and the person detection information by opening a web browser to http://localhost:5000

## Enabling and Disabling Demo Features

### WiFi / Image Sending
By default, the Arduino sketch is configured to connect to the WiFi access point described in the config.h file.

If you do not wish to connect to a WiFi access point and send images to an instance of the image-server, change the value of the 'send\_scaled\_image' variable in the person\_detection.ino file to False. Change it back to True to re-enable it.


### Writing Images to an SD Card
By default, writing of images to an SD card is disabled.

If you wish to have the 96x96 images that are used by TensorFlow written to the SD Card, you must, of course, insert an SD card into the Vision Shield's SD card slot.

Then, you can enable this feature by changing the value of the 'write\_scaled\_image' variable in the person\_detection.ino file to True. Change it back to False to disable it again.

* Note that we have seen issues with writing to the SD card, where the Portenta will sometimes lock up after writing 10 or so images, sometimes more. If this happens, just restart the board by briefly pushing the reset button on the bottom of the H7 board.


### Console Logging
The 'person\_detection' sketch will always log some basic inforation to the serial console.
Verboseness of the information about the ongoing operation of the sketch is controlled by the 'write\_serial\_info' variable in the person_detection.ino file.

By default, the sketch logs verbosly.
For minimal logging (including all errors), change the 'write\_serial\_info' variable to False. To re-enable verbose logging, change it back to True.


## Other Considerations
### Throughput
Several steps in the person recognition process can take a noticeable amount of time.
In particular, the TensorFlow analyis and posting of the image to the image-viewer can take several seconds each. When the image-viewer is in use, the time between image changes in the viewer tends to be about 7 seconds. Due to the latency described above, images may appear in the viewer from 7 to 14 seconds after they are captured by the camera.

### Lighting
The person\_detection sketch works fairly well under moderate lighting, but works much better under well-lit conditions.

### Distance from the Camera
This demo appears to work best when people are from approximately 1 foot to about 10 feet from the camera. It fails to recognize people further away, and is unpredictable when they are closer.

### False-Positives
We have noticed that some images presented to the TensorFlow model will produce false positives. Among these were:

  * A white bed pillow propped up vertically on a sofa
  * Various body parts (hand, etc.) held very close to the camera.

 


