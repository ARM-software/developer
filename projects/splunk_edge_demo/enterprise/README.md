# Splunk Enterprise Directory
The contents of this directory are used to deploy Splunk Enterprise in the cloud. The original demo uses adafruit sensor boards and scripts to do this (see ../python README.md). However, it is possible to write a script that produces mock data so that the sensor boards are not required. Refer to the ../python directory for more information on how to do this.

## Instructions 
The following instructions assume you have an AWS account and all depedencies needed to run terraform and ansible to deploy workloads onto AWS.

Initialize terraform in ./enterprise/terraform/tf-compute. To do this, run the following command:

```
terraform init
```

Run The terraform script (terraform apply) in ./enterprise/terraform/tf-compute. 

```
terraform apply -var "aws_key=<your_aws_key>" -var "ingress_ips=[\"0.0.0.0/0\"]"
```
**Note:** ```ingress_ips``` should be changed to the ip address you will access Splunk Web from (most likely the laptop you will run terraform on). The example above will open up your Splunk Enterprise install to everyone on the internet, which is generally not recommended. ```aws_key``` is the key you uploaded to AWS for SSH access.


The ```terraform apply``` command above will launch an AWS instance and configure it to receive forwarder data and for Splunk Web access (see security groups in terraform files). Terraform will create the VPCs/Subnets/etc. so you shouldn't have to create anything manually on AWS. Make sure to provide your AWS key (```aws_key```)  for connecting to the instance via SSH. If you don't do this, you will not be able to SSH into the instance or run the ansible playbook described below.

If you look at the ansible playbook (./enterprise/ansible) that installs Splunk Enterprise, you will notice there is a reference to a directory called "installers". This is where the .deb package will be stored. It is not included in this repo to reduce bloat and allow for newer versions of Splunk Enterprise to be experimented with. You will need to download the installer, create the installer directory, and copy in the download before running the ansible playbook. You will also need to update the ansible playbook to refer to the correct name of the .deb file (see the variable splunk_deb in the playbook) you downloaded.
You can download Splunk Enterprise from the Splunk Website. You will need to buy a license or signup for a free trial.

In addition to the installer, you will need to provide your own certificates in the ./enterprise/server_certs directory. These can be self-signed. The Splunk documentation explains how to create certificates. Alternatively, the Splunk documentation explains how to use Splunk default certificates (not recommended for production). Make sure to name the certs as follows:

mySplunkIndexCertificate.pem

mySplunkIndexPrivateKey.key

mySplunkWebCertificate.pem

mySplunkWebPrivateKey.key

The first two above are to secure Forwarder to Splunk indexer (running on Splunk Enterprise) traffic.
The last two above are to secure Splunk Web access.

After the ```terraform apply``` command is complete, run the ansible playbook in ./enterprise/ansible with the following command:

```ansible-playbook -i host splunk_ent_setup.yml```

The inventory file called "host" is placed there by the terrform script you ran above. This contains the IP address of the splunk enterprise instance created by the terraform script. When this playbook is run, it will ask you to select an admin password for the Splunk Enterprise instance. This command will startup Splunk Web. Instructions on how to connect to Splunk Web are below.

If you haven't already, deploy the Splunk Universal Forwarder on the edge device. See ../fwd/README.md

If you haven't already, connect to Splunk Web and sign in with user name 'admin' and the password you selected previously. To connect to Splunk web, go to https://splunk_ip:8000. Where "splunk_ip" is what's written into the host file. You can also sign into AWS to get the instance public IP. User name is "admin" and the password is whatever you selected when you ran the command.

Next we have to tell Splunk Enterprise what configurations we want it to push down to edge devices. To do this we have to setup server classes within the Splunk Enterprise UI. Go to Settings -> Forwarder Management. Select the "Server Classes" tab. Select "New Server Class". Select "Add Apps". Add in both "edge_array_input, and edge_array_output" and click save. Now edit both "edge_array_input" and "edge_array_output" and check the "Restart Splunkd" box. Make sure to save the change. Last, click on "Add Clients", place your Client Name (or Host Name) in the "Include" box at the top left and click save. In the main forwarder management screen, you should see that edge device has downloaded and applied its configuration.

Wait about a minute to give the edge device time to restart the splunk daemon and to start pushing data to Splunk Enterprise.

Last, we can get to the dashboard by going back to the home Splunk Enterprise screen (click on the top left image that say's "splunk>enterprise"). Click on "Edge Sensor Demo" in the apps column on the left, and click on "Edge Sensor Array".
