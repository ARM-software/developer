#!/usr/bin/env python
from time import sleep

# Register addresses
REG_PAR_G1 = 0xED
REG_PAR_G2_LSB = 0xEB
REG_PAR_G2_MSB = 0xEC
REG_PAR_G3 = 0xEE
REG_PAR_H1_LSB = 0xE2
REG_PAR_H1_MSB = 0xE3
REG_PAR_H2_LSB = 0xE2
REG_PAR_H2_MSB = 0xE1
REG_PAR_H3 = 0xE4
REG_PAR_H4 = 0xE5
REG_PAR_H5 = 0xE6
REG_PAR_H6 = 0xE7
REG_PAR_H7 = 0xE8
REG_PAR_P1_LSB = 0x8E
REG_PAR_P1_MSB = 0x8F
REG_PAR_P2_LSB = 0x90
REG_PAR_P2_MSB = 0x91
REG_PAR_P3 = 0x92
REG_PAR_P4_LSB = 0x94
REG_PAR_P4_MSB = 0x95
REG_PAR_P5_LSB = 0x96
REG_PAR_P5_MSB = 0x97
REG_PAR_P6 = 0x99
REG_PAR_P7 = 0x98
REG_PAR_P8_LSB = 0x9C
REG_PAR_P8_MSB = 0x9D
REG_PAR_P9_LSB = 0x9E
REG_PAR_P9_MSB = 0x9F
REG_PAR_P10 = 0xA0
REG_PAR_T1_LSB = 0xE9
REG_PAR_T1_MSB = 0xEA
REG_PAR_T2_LSB = 0x8A
REG_PAR_T2_MSB = 0x8B
REG_PAR_T3 = 0x8C
REG_RESET = 0xE0
REG_ID = 0xD0
REG_CTRL_MEAS = 0x74
REG_CTRL_HUM = 0x72
REG_CTRL_GAS_1 = 0x71
REG_GAS_WAIT_0 = 0x64
REG_RES_HEAT_0 = 0x5A
REG_MEAS_STATUS_0 = 0x1D
REG_GAS_LSB = 0x2B
REG_GAS_MSB = 0x2A
REG_HUM_LSB = 0x26
REG_HUM_MSB = 0x25
REG_PRESS_XLSB = 0x21
REG_PRESS_LSB = 0x20
REG_PRESS_MSB = 0x1F
REG_TEMP_XLSB = 0x24
REG_TEMP_LSB = 0x23
REG_TEMP_MSB = 0x22
REG_RANGE_SWITCHING_ERROR = 0x04
REG_RES_HEAT_RANGE = 0x02
REG_RES_HEAT_VAL = 0x00

# Constants array for gas resistance calculations
CONST_ARRAY1 = [1, 1, 1, 1, 1, 0.99, 1, 0.992, 1, 1, 0.998, 0.995, 1, 0.99, 1, 1]
CONST_ARRAY2 = [8000000, 4000000, 2000000, 1000000, 499500.4995, 248262.1648, 125000, 63004.03226, 31281.28128, 15625,
                7812.5, 3906.25, 1953.125, 976.5625, 488.28125, 244.140625]


class BME680:
    def __init__(self, i2cdev, addr=0x77):
        self.i2cdev = i2cdev
        self.addr = addr

        # Reset the BME680
        self.i2cdev.regwr(self.addr, REG_RESET, 0xB6)  # 0xB6 is the magic reset value (see datasheet)

        # Report chip ID to make sure we can read registers properly
        chip_id = hex(self.i2cdev.regrd(self.addr, REG_ID, "<B"))
        print("Expected BME680 chip id: 0x61")
        print("Chip id found: {}".format(chip_id))

        # Set oversamples
        osrs_h = 1
        osrs_p = 5  # 5 = 16x, see datasheeet
        osrs_t = 4
        ctrl_meas = (osrs_t << 5) | (osrs_p << 2)
        self.i2cdev.regwr(self.addr, REG_CTRL_HUM, osrs_h)
        self.i2cdev.regwr(self.addr, REG_CTRL_MEAS, ctrl_meas)

        # Set Gas sensor parameters
        self.i2cdev.regwr(self.addr, REG_GAS_WAIT_0, 0x59)  # 100ms heat up duration
        # Calculate header set point
        target_temp = 300  # Targer gas heater temp in C
        amb_temp = 21  # Room temp in C.
        par_g1 = self.i2cdev.regrd(self.addr, REG_PAR_G1, "<B")
        par_g2_lsb = self.i2cdev.regrd(self.addr, REG_PAR_G2_LSB, "<B")
        par_g2_msb = self.i2cdev.regrd(self.addr, REG_PAR_G2_MSB, "<B")
        par_g2 = (par_g2_msb << 8) | par_g2_lsb
        par_g3 = self.i2cdev.regrd(self.addr, REG_PAR_G3, "<B")
        res_heat_range = self.i2cdev.regrd(self.addr, REG_RES_HEAT_RANGE, "<B")
        res_heat_range = res_heat_range >> 4
        res_heat_val = self.i2cdev.regrd(self.addr, REG_RES_HEAT_VAL, "<B")

        var1 = (par_g1 / 16.0) + 49.0
        var2 = ((par_g2 / 32768.0) * 0.0005) + 0.00235
        var3 = par_g3 / 1024.0
        var4 = var1 * (1.0 + (var2 * target_temp))
        var5 = var4 + (var3 * amb_temp)
        res_heat_0 = (3.4 * ((var5 * (4.0 / (4.0 + res_heat_range)) * (1.0 / (1.0 +
                      (res_heat_val * 0.002)))) - 25))
        res_heat_0 = int(res_heat_0)

        self.i2cdev.regwr(self.addr, REG_RES_HEAT_0, res_heat_0)
        self.i2cdev.regwr(self.addr, REG_CTRL_GAS_1, 0x10)  # Select setpoint 0 and turn on gas sensor

    def _get_temp(self):
        # Read temp registers
        temp_xlsb = self.i2cdev.regrd(self.addr, REG_TEMP_XLSB, "<B")
        temp_lsb = self.i2cdev.regrd(self.addr, REG_TEMP_LSB, "<B")
        temp_msb = self.i2cdev.regrd(self.addr, REG_TEMP_MSB, "<B")
        temp_adc = (temp_msb << 12) | (temp_lsb << 4) | (temp_xlsb >> 4)

        # Get calibration values
        par_t1_lsb = self.i2cdev.regrd(self.addr, REG_PAR_T1_LSB, "<B")
        par_t1_msb = self.i2cdev.regrd(self.addr, REG_PAR_T1_MSB, "<B")
        par_t1 = (par_t1_msb << 8) | par_t1_lsb

        par_t2_lsb = self.i2cdev.regrd(self.addr, REG_PAR_T2_LSB, "<B")
        par_t2_msb = self.i2cdev.regrd(self.addr, REG_PAR_T2_MSB, "<B")
        par_t2 = (par_t2_msb << 8) | par_t2_lsb

        par_t3 = self.i2cdev.regrd(self.addr, REG_PAR_T3, "<B")

        # The data sheet lists these formulas
        var1 = ((temp_adc / 16384.0) - (par_t1 / 1024.0)) * par_t2
        var2 = (((temp_adc / 131072.0) - (par_t1 / 8192.0)) * ((temp_adc / 131072.0) -
                (par_t1 / 8192.0))) * (par_t3 * 16.0)
        t_fine = var1 + var2
        temp_comp = t_fine / 5120.0  # This is the temp in C
        temp_f = (temp_comp * 1.8) + 32.0   # convert to F

        return temp_f, t_fine, temp_comp

    def _get_press(self, t_fine):
        # Read Pressure register
        press_xlsb = self.i2cdev.regrd(self.addr, REG_PRESS_XLSB, "<B")
        press_lsb = self.i2cdev.regrd(self.addr, REG_PRESS_LSB, "<B")
        press_msb = self.i2cdev.regrd(self.addr, REG_PRESS_MSB, "<B")
        press_adc = (press_msb << 12) | (press_lsb << 4) | (press_xlsb >> 4)

        # Get calibration values
        par_p1_lsb = self.i2cdev.regrd(self.addr, REG_PAR_P1_LSB, "<B")
        par_p1_msb = self.i2cdev.regrd(self.addr, REG_PAR_P1_MSB, "<B")
        par_p1 = (par_p1_msb << 8) | par_p1_lsb

        par_p2_lsb = self.i2cdev.regrd(self.addr, REG_PAR_P2_LSB, "<B")
        par_p2_msb = self.i2cdev.regrd(self.addr, REG_PAR_P2_MSB, "<B")
        par_p2 = (par_p2_msb << 8) | par_p2_lsb

        par_p3 = self.i2cdev.regrd(self.addr, REG_PAR_P3, "<B")

        par_p4_lsb = self.i2cdev.regrd(self.addr, REG_PAR_P4_LSB, "<B")
        par_p4_msb = self.i2cdev.regrd(self.addr, REG_PAR_P4_MSB, "<B")
        par_p4 = (par_p4_msb << 8) | par_p4_lsb

        par_p5_lsb = self.i2cdev.regrd(self.addr, REG_PAR_P5_LSB, "<B")
        par_p5_msb = self.i2cdev.regrd(self.addr, REG_PAR_P5_MSB, "<B")
        par_p5 = (par_p5_msb << 8) | par_p5_lsb

        par_p6 = self.i2cdev.regrd(self.addr, REG_PAR_P6, "<B")
        par_p7 = self.i2cdev.regrd(self.addr, REG_PAR_P7, "<B")

        par_p8_lsb = self.i2cdev.regrd(self.addr, REG_PAR_P8_LSB, "<B")
        par_p8_msb = self.i2cdev.regrd(self.addr, REG_PAR_P8_MSB, "<B")
        par_p8 = (par_p8_msb << 8) | par_p8_lsb

        par_p9_lsb = self.i2cdev.regrd(self.addr, REG_PAR_P9_LSB, "<B")
        par_p9_msb = self.i2cdev.regrd(self.addr, REG_PAR_P9_MSB, "<B")
        par_p9 = (par_p9_msb << 8) | par_p9_lsb

        par_p10 = self.i2cdev.regrd(self.addr, REG_PAR_P10, "<B")

        # The data sheet lists these formulas
        var1 = (t_fine / 2.0) - 64000.0
        var2 = var1 * var1 * (par_p6 / 131072.0)
        var2 = var2 + (var1 * par_p5 * 2.0)
        var2 = (var2 / 4.0) + (par_p4 * 65536.0)
        var1 = (((par_p3 * var1 * var1) / 16384.0) + (par_p2 * var1)) / 524288.0
        var1 = (1.0 + (var1 / 32768.0)) * par_p1
        press_comp = 1048576.0 - press_adc
        press_comp = ((press_comp - (var2 / 4096.0)) * 6250.0) / var1
        var1 = (par_p9 * press_comp * press_comp) / 2147483648.0
        var2 = press_comp * (par_p8 / 32768.0)
        var3 = (press_comp / 256.0) * (press_comp / 256.0) * (press_comp / 256.0) * (par_p10 / 131072.0)
        press_comp = press_comp + (var1 + var2 + var3 + (par_p7 * 128.0)) / 16.0

        return press_comp

    def _get_hum(self, temp_comp):
        # Read humidity registers
        hum_lsb = self.i2cdev.regrd(self.addr, REG_HUM_LSB, "<B")
        hum_msb = self.i2cdev.regrd(self.addr, REG_HUM_MSB, "<B")
        hum_adc = (hum_msb << 8) | hum_lsb

        # Get calibration values
        par_h1_lsb = self.i2cdev.regrd(self.addr, REG_PAR_H1_LSB, "<B")
        par_h1_msb = self.i2cdev.regrd(self.addr, REG_PAR_H1_MSB, "<B")
        par_h1 = (par_h1_msb << 4) | (par_h1_lsb & 0xF)

        par_h2_lsb = self.i2cdev.regrd(self.addr, REG_PAR_H2_LSB, "<B")
        par_h2_msb = self.i2cdev.regrd(self.addr, REG_PAR_H2_MSB, "<B")
        par_h2 = (par_h2_msb << 4) | ((par_h2_lsb & 0xF0) >> 4)

        par_h3 = self.i2cdev.regrd(self.addr, REG_PAR_H3, "<B")
        par_h4 = self.i2cdev.regrd(self.addr, REG_PAR_H4, "<B")
        par_h5 = self.i2cdev.regrd(self.addr, REG_PAR_H5, "<B")
        par_h6 = self.i2cdev.regrd(self.addr, REG_PAR_H6, "<B")
        par_h7 = self.i2cdev.regrd(self.addr, REG_PAR_H7, "<B")

        # The data sheet lists these formulas
        var1 = hum_adc - ((par_h1 * 16.0) + ((par_h3 / 2.0) * temp_comp))
        var2 = var1 * ((par_h2 / 262144.0) * (1.0 + ((par_h4 / 16384.0) *
                       temp_comp) + ((par_h5 / 1048576.0) * temp_comp * temp_comp)))
        var3 = par_h6 / 16384.0
        var4 = par_h7 / 2097152.0
        hum_comp = var2 + ((var3 + (var4 * temp_comp)) * var2 * var2)

        return hum_comp

    def _get_gasres(self):
        # Get Gas sensor reading
        gas_lsb = self.i2cdev.regrd(self.addr, REG_GAS_LSB, "<B")
        gas_msb = self.i2cdev.regrd(self.addr, REG_GAS_MSB, "<B")
        gas_adc = (gas_msb << 2) | (gas_lsb >> 6)
        gas_range = gas_lsb & 0x0F

        # The data sheet lists these formulas
        range_switching_error = self.i2cdev.regrd(self.addr, REG_RANGE_SWITCHING_ERROR, "<B")
        var1 = (1340.0 + 5.0 * range_switching_error) * CONST_ARRAY1[gas_range]
        gas_res = var1 * CONST_ARRAY2[gas_range] / (gas_adc - 512.0 + var1)

        return gas_res

    def get_measurement(self):
        '''
        Returns the follwoing list.
        [Temp,Pressure,Humidity]
        Temp is in F
        Pressure is in Pascals
        Humidity is %
        Gas resistance (lower means worse)
            There is a calculation to get air quality from the GAS resistance. It is not listed in the datasheet.
            We are going to skip that calculation for this demo.
        Each measurement will wait 1s. This can be changed, but this is good for demo purposes.
        '''
        # Start a measurement
        ctrl_meas = self.i2cdev.regrd(self.addr, REG_CTRL_MEAS, "<B")
        ctrl_meas = ctrl_meas | 1
        self.i2cdev.regwr(self.addr, REG_CTRL_MEAS, ctrl_meas)

        # Poll for completion
        new_data = 0
        timeout = 0
        while new_data == 0:
            timeout += 1
            sleep(1)
            new_data = self.i2cdev.regrd(self.addr, REG_MEAS_STATUS_0, "<B") & 128
            if new_data:  # measurement complete
                # Get temp, pressure
                temp_f, t_fine, temp_comp = self._get_temp()
                press_pa = self._get_press(t_fine)
                hum = self._get_hum(temp_comp)
                gas_res = self._get_gasres()

                return [round(temp_f, 2), round(press_pa, 2), round(hum, 2), round(gas_res, 2)]
            if timeout >= 10:
                print("Measurement timeout")
                return [0, 0, 0, 0]


def main():
    import i2cdriver
    print("BME680 Sensor Test")
    i2c = i2cdriver.I2CDriver("/dev/ttyUSB0")
    i2c.scan()   # Reset all devices on the i2c bus
    bme680 = BME680(i2c)

    while 1:
        print(bme680.get_measurement())


if __name__ == "__main__":
    main()
