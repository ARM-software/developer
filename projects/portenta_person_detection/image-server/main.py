# Copyright 2021 Chariot Solutions Inc. (https://chariotsolutions.com)
# and ARM (https://arm.com) All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================*/

import os
from PIL import Image
from datetime import datetime

from flask import Flask, request, send_file, Response, abort, render_template, jsonify
app = Flask(__name__)

last_file = ''
last_file_timestamp = ''
person_score = ''
no_person_score = ''
is_person = False

@app.after_request
def set_response_headers(response):
    response.headers['Cache-Control'] = 'no-cache, no-store, must-revalidate'
    response.headers['Pragma'] = 'no-cache'
    response.headers['Expires'] = '0'
    return response

@app.route('/')
def show_image():
    return render_template('viewer.html');

@app.route('/robots.txt')
def no_robots():
    return send_file('robots.txt')

# The Arduino will post the image used for Tensor Flow as raw bytes
# The height and width should both be 96
# person_value and not_person_value are the results from tensor flow
@app.route('/tf/<image_width>/<image_height>/<person_value>/<not_person_value>', methods=['POST'])
def tf(image_width, image_height, person_value, not_person_value):
    width = int(image_width)
    height = int(image_height)
    image_dir = f"./static/captured-images"
    now = datetime.now() # current date and time
    timestamp = now.strftime("%Y%m%d-%H%M%S")
    file_name = f"img_{timestamp}_{person_value}_{not_person_value}.jpg"

    raw_image_data = request.get_data()
    if len(raw_image_data) != (width * height):
        abort(400, f"Expected {(width * height)} bytes but received {len(raw_image_data)}") 

    image = Image.frombytes('L', (width, height), raw_image_data, decoder_name='raw')

    if not os.path.isdir(image_dir):
        os.makedirs(image_dir, mode=0o777, exist_ok=False)

    image.save(os.path.join(image_dir, file_name))

    global last_file

    # if we're creating and showing the "big" file
    # big_file_name = f"big_{timestamp}_{person_value}_{not_person_value}.jpg"
    # last_file =  os.path.join(image_dir, big_file_name)

    # if we're using the received file as-is
    last_file = os.path.join(image_dir, file_name)


    global last_file_timestamp
    last_file_timestamp = timestamp

    global person_score
    person_score = person_value

    global no_person_score
    no_person_score = not_person_value

    global is_person
    is_person = int(person_value) > int(not_person_value)

    # scale the image up so it is easier to see
    # use this if we're getting 96x96 images
    # not necessary if we're getting 240x240
    # image4 = image.resize((width * 4, height * 4))
    # image4.save(os.path.join(image_dir, big_file_name))

    return Response(status=201)

@app.route('/image-info')
def image_info():
    global last_file
    global last_file_timestamp
    global person_score
    global no_person_score
    global is_person

    return jsonify(
        last_file=last_file,
        last_file_timestamp=last_file_timestamp,
        person_score=person_score,
        no_person_score=no_person_score,
        is_person=is_person
    )
