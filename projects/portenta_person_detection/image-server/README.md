Image Server

Accepts raw grayscale image data via HTTP POST and save as a jpg into the static directory.

Create a virtual environment

    python3 -m venv venv

Activate

    . venv/bin/activate

Install requirements

    pip install -r requirements.txt

Run the server

    flask run --host=0.0.0.0

Post a file to test

    curl -vv --data-binary "@test.raw" http://localhost:5000/tf/96/96/0/255

Look in static for the resulting jpg file

