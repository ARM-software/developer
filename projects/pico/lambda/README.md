
# Preparing AWS Lambda Deployment Package in Python & Testing Kafka Connect AWS Lambda Connector

A deployment package is a ZIP archive that contains your function code and dependencies. You need to create a deployment package if you use the Lambda API to manage functions, or if you need to include libraries and dependencies other than the AWS SDK. You can upload the package directly to Lambda, or you can use an Amazon S3 bucket, and then upload it to Lambda. If the deployment package is larger than 50 MB, you must use Amazon S3.

Let us try out a simple example to build Kafka Module and package it in the form of zip which can be loaded onto AWS Lambda.


## Pre-requisite:

- Docker Desktop for Mac
- Python 3.6 
- Kafka Cluster running on http://35.189.130.4:9000/

## Using a Virtual Environment

You may need to use a virtual environment to install dependencies for your function. This can occur if your function or its dependencies have dependencies on native libraries, or if you used Homebrew to install Python.

To update a Python function with a virtual environment

1. Create a virtual environment.

```
[Captains-Bay]🚩 >  pwd
/Users/ajeetraina/pico
[Captains-Bay]🚩 >  virtualenv v-env
Using base prefix '/Library/Frameworks/Python.framework/Versions/3.6'
New python executable in /Users/ajeetraina/pico/v-env/bin/python3.6
Also creating executable in /Users/ajeetraina/pico/v-env/bin/python
Installing setuptools, pip, wheel...
done.
[Captains-Bay]🚩 
```

For Python 3.3 and newer, you need to use the built-in venv module to create a virtual environment, instead of installing virtualenv.

```
[Captains-Bay]🚩 >  python3 -m venv v-env
[Captains-Bay]🚩 > 
```

## Activate the environment

```
source v-env/bin/activate
```

## Install libraries with pip

```
(v-env) [Captains-Bay]🚩 >  pip install kafka
Collecting kafka
  Downloading https://files.pythonhosted.org/packages/21/71/73286e748ac5045b6a669c2fe44b03ac4c5d3d2af9291c4c6fc76438a9a9/kafka-1.3.5-py2.py3-none-any.whl (207kB)
     |████████████████████████████████| 215kB 428kB/s 
Installing collected packages: kafka
Successfully installed kafka-1.3.5
(v-env) [Captains-Bay]🚩 > 
```

## Deactivate the virtual environment

```
deactivate
```

## Create a ZIP archive with the contents of the library

```
cd v-env/lib/python3.7/site-packages
```



```
zip -r9 ${OLDPWD}/function.zip .
```

```
cd $OLDPWD
```

## Add your function code to the archive

Add [function.py](https://github.com/collabnix/pico/blob/master/lambda/function.py) here under the same directory

```
zip -g function.zip function.py
```

# Testing Kafka Connect AWS Lambda Connector

## Pre-requisite:

- aws.amazon.com
- Click on Services > Lambda

## Steps:

- Open AWS Lambda Page
- Click on "Create Funtion"
- Select "Author from Scratch"
- Enter Function name of your Choice
- Choose "Python 3.6" as Runtime
- Click on "Create Function"

You should see the message "Congratulations! Your Lambda function "kafka-pico-lambda" has been successfully created. You can now change its code and configuration. Choose Test to input a test event when you want to test your function."

Under the function code, select "Upload as zip file" and upload function.zip. Select Python 3.6 as Runtime and handler as function.lambda_handler.


![alt text](https://github.com/collabnix/pico/blob/master/lambda/Screen%20Shot%202019-07-01%20at%203.32.15%20PM.png)

Click on Save.

## Triggering the Consumer Code

Go to any one of your Kafka container(say, kafka-1) and run the below command:

```
docker exec -it kafka-1 bash
```

```
kafka-console-producer --broker-list kafka-1:9092 --topic aws-lambda-topic 
```
 and enter some text randomly.
 Say, I typed dharwad for example.
 
 Go back to Lambda and Click on Test. You should be able to see dharwad as output as shown below:


![alt text](https://github.com/collabnix/pico/blob/master/lambda/Screen%20Shot%202019-07-01%20at%203.31.58%20PM.png)

## Troubleshooting: 
- If it displays "Unable to import Kafka module" then possibly you have missed out the above steps. Go back and start from the beginning in building the zip file.
- If in case timedout error appear, please increase the timed-out value to 3-4 minutes.

