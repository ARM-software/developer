# Forwarder Directory

The contents of this direcotry are used to deploy the Splunk Universal Forwarder on an edge device. The forwarder will be setup as a deployment client. This means that its final configuration will be downloaded from the Splunk Enterprise instance that is hosted in the cloud. The ansible playbook provided here simply installs the forwarder and configures it just enough so that it knows how to connect to the Splunk Enterprise instance.

## Instructions
The following instructions assume you have all dependencies needed to run python scripts and ansible playbooks.

Create a file called 'hosts' in ./fwd/ansible/. In this file, place the IP address of the edge device. If the edge device has a public IP address, use that, if the device is on a private network along with the control node (usually the laptop you are working on), use the private IP address.

If you look at the ansible playbook that installs the Splunk Universal Forwarder, you will notice there is a reference to a directory called "installers". This is where the .tgz file will be stored. It is not included in this repo to reduce bloat. You will need to download the file, create the installer directory, and copy in the downloaded installer before running the ansible playbook. You will also need to update the ansible playbook to refer to the correct name of the .tgz file.
You will need to download the Splunk Universal Forwarder from the Splunk website. The forwarder is free and does not require a license.

In addition to the installer, you will need to provide your own certificates in the ./fwd/indexer_certs directory. The easiest thing to do is copy the below keys/certs from the ./enterprie/server_certs directory.

mySplunkIndexCertificate.pem

mySplunkIndexPrivateKey.key

Run the ansible playbook in ./fwd/ansible using the command below.

```ansible-playbook -i hosts splunk_fwd_setup.yml -K -k```

You will be prompted for the ssh password for the edge device, The edge device user to sign in with, and the admin password you want to use for the forwarder.

If you haven't started the read_sensors.py script on the edge device, now is the time to do it.

At this point, you shouldn't have to connect to the edge device, but if you wish to explore, you can connect and explore /opt/splunkforwarder. This is the location of the Splunk Forwarder installation. See the Splunk documentation for more information.
