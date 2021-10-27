# Python Directory
**NOTE:** The sensor boards are not required to run this demo. See read_sensors.py description below.

This python directory will be uploaded into the edge device by the ansible playbook in ../fwd/ansible.

The original demo has adafruit sensor boards attached to the edge device via a USB<->i2c adaptor.
The USB<->i2c adaptor we used is called the I2CDriver or I2CMini (either will work):
https://www.adafruit.com/product/4267
https://www.adafruit.com/product/4970
Make sure to visit the I2CDriver site and install the python library.

We used this USB based approach because it's less messy than hooking into I2C pins on edge devices. It also makes it easier to work with these sensors on a laptop which usually do not have I2C pins exposed. When the USB adaptor is plugged in, double check that the device shows up at /dev/ttyUSB0. If not, this will have to be changed in the python scripts.

## Sensor boards
**bme680:**

Measures temperature, pressure, humidity, and VoC sensor:

https://www.adafruit.com/product/3660

https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme680-ds001.pdf

**tsl2591:**

Ambient Light Sensor:

https://www.adafruit.com/product/1980

https://ams.com/tsl25911

## Python Files

**bme680.py:**

This interfaces with the bme680 sensor board. You can run this script by itself to test the sensor board. It will configure and output all of the sensor readings to stdout.

**tsl2591.py:**

This interfaces with the tsl2591 sensor board. You can run this script by itself to test the sensor board. It will configure and output all of the sensor readings to stdout. There are two outputs to this sensor. The first is visible light energy and infrared energy integrated together, the other is just infrared energy integrated. For this reason, the visible + infrared readings will be higher. This is because more energy is being integrated.

**read_sensors.py:**

This file runs both of the above sensor boards. It will output readings to stdout, as well as write to /var/log/sensor_data. It's setup to record about 48 hours of history. The sensor_data file is what splunk will monitor for sending to the indexer running on the Splunk Enterprise in the cloud. Since we are using /var/log/sensor_data as the source for data. It is possible to write a mock data generator and have it write to this file. This would remove the need to attach the sensor boards noted above. If you are going to create your own mock data generator, take a look at how the data is formated in the read_sensors.py script. It's not a requirement to format it exactly like this, but it will make things easier to work with.
