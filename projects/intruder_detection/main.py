"""
 * Copyright (C) 2010-2021 Arm Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Project:      Intruder Detection
 * Title:        main.py
 * Description:  Basic Pico script to turn on an LED if motion sensor is activated
 *
 * $Date:        09. Feb 2021
 *
 * Target Processor:  RP2040
 *
 """

from machine import Pin, Timer

def check_sensor(timer):
    global sensor
    if sensor.value() == 1:
        gp1.value(1)
    else:
        gp1.value(0)

#GP4 - 5v output
gp4 = Pin(4,Pin.OUT)
gp4.value(1)
#GP1 - output for LED
gp1= Pin(1,Pin.OUT)
#GP5 - input from sensor
sensor = Pin(5,Pin.IN)
tim = Timer()
tim.init(freq=1.5, mode=Timer.PERIODIC, callback=check_sensor)
