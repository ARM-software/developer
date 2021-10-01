#!/usr/bin/env python
from i2cdriver import I2CDriver
from bme680 import BME680
from tsl2591 import TSL2591
from time import sleep
from datetime import datetime


def main():
    print("Polling All Sensors")
    i2c = I2CDriver("/dev/ttyUSB0")
    i2c.scan()   # Reset all devices on the i2c bus
    tphg_sensor = BME680(i2c)
    als_sensor = TSL2591(i2c)

    # Clear the sensor data file
    with open('/var/log/sensor_data', 'w') as sensor_data_file:
        sensor_data_file.write('')

    while True:
        sleep(4)
        tphg = tphg_sensor.get_measurement()
        als = als_sensor.get_measurement()
        print("Temp (F), Press (pa), Hum (%), VOC (ohm): {}".format(tphg))
        print("Light (integral): {}\n".format(als))

        # Get event count count
        with open('/var/log/sensor_data') as f:
            count = sum(1 for _ in f)

        read_write = 'w' if count > 36000 else 'a'  # Overwrite file is event count exceeds 36000 (about 48hrs)
        with open('/var/log/sensor_data', read_write) as sensor_data_file:
            sensor_data_file.write(
                "time={},temperature={},pressure={},humidity={},\
                        voc={},als-inf-vis={},als-inf={}\n".format(datetime.now(), tphg[0], tphg[1], tphg[2],
                                                                   tphg[3], als[0], als[1]))


if __name__ == "__main__":
    main()
