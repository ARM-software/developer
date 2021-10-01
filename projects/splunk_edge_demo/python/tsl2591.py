#!/usr/bin/env python
from time import sleep

# Register addresses
REG_ENABLE = 0x00
REG_CONFIG = 0x01
REG_ID = 0x12
REG_STATUS = 0x13
REG_C0DATAL = 0x14
REG_C0DATAH = 0x15
REG_C1DATAL = 0x16
REG_C1DATAH = 0x17

# Command bits
COMMAND_BITS = 0xA0


class TSL2591:
    def __init__(self, i2cdev, addr=0x29):
        self.i2cdev = i2cdev
        self.addr = addr

        # Reset the TLS2591
        self.i2cdev.regwr(self.addr, COMMAND_BITS | REG_CONFIG, 0x40)

        # Report chip ID to make sure we can read registers properly
        dev_id = hex(self.i2cdev.regrd(self.addr, COMMAND_BITS | REG_ID, "<B"))
        print("Expected TSL2591 device id: 0x50")
        print("Device id found: {}".format(dev_id))

        # Setup integrator gain and integrator time (see datasheet)
        self.i2cdev.regwr(self.addr, COMMAND_BITS | REG_CONFIG, 0x12)

        # Enable the sensor and integrator, measurements run continuously from this point on.
        self.i2cdev.regwr(self.addr, COMMAND_BITS | REG_ENABLE, 0x03)

    def get_measurement(self):
        '''
        Returns broad spectrum and infrared integration readings in a list.
        To calculate lux from these numbers is a long process that we're not going to do here.
        Each measurement will wait 1s. This can be changed, but this is good for demo purposes.
        '''
        # Read inteation ADC.
        # CH0 is broad spectrum (visible_infra)
        # CH1 is infrared
        new_data = 0
        timeout = 0
        while new_data == 0:
            timeout += 1
            sleep(1)
            status = self.i2cdev.regrd(self.addr, COMMAND_BITS | REG_STATUS, "<B")
            if (status & 1) == 1:
                ch0_l = self.i2cdev.regrd(self.addr, COMMAND_BITS | REG_C0DATAL, "<B")
                ch0_h = self.i2cdev.regrd(self.addr, COMMAND_BITS | REG_C0DATAH, "<B")
                ch1_l = self.i2cdev.regrd(self.addr, COMMAND_BITS | REG_C1DATAL, "<B")
                ch1_h = self.i2cdev.regrd(self.addr, COMMAND_BITS | REG_C1DATAH, "<B")
                ch0 = (ch0_h << 8) | ch0_l
                ch1 = (ch1_h << 8) | ch1_l

                return [ch0, ch1]

            if timeout >= 10:
                print("Measurement timeout")
                return [0, 0]


def main():
    import i2cdriver
    print("TSL2591 Sensor Test")
    i2c = i2cdriver.I2CDriver("/dev/ttyUSB0")
    i2c.scan()   # Reset all devices on the i2c bus
    tsl2591 = TSL2591(i2c)

    while 1:
        print(tsl2591.get_measurement())


if __name__ == "__main__":
    main()
