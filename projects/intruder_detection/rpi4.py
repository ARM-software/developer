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
 * Title:        rpi4.py
 * Description:  Basic communication between Pico and Raspberry Pi 4
 *
 * $Date:        10. Jan 2021
 *
 * Target Processor:  RP2040
 *
 """
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
