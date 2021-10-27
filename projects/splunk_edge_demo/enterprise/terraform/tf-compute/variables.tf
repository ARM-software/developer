######################################################################
# Variables related to general setup of the infrastructure
######################################################################
variable "aws_region" {
  description = "The AWS region we want to deploy on."
  type = string
  default = "us-east-1"
}

###################################
# Variable related to instance deployment
###################################
variable "enterprise_instance_ami" {
  description = "The AMI to use for enterprise_instance_type"
  type = string
  default = "ami-09e67e426f25ce0d7"
}

variable "enterprise_instance_type" {
  description = "The EC2 instance type to use for splunk enterprise"
  type = string
  default = "m6i.large"
}

variable "instance_az" {
  description = "Availability Zone we want for splunk"
  type = string
  default = "us-east-1a"
}

variable "ingress_ips" {
  description = "A list of IP address we will allow to connect to the splunk instance(s), 0.0.0.0/0 is all ips. all ips is ok for debug, but don't use it for demos"
  type = list(string)
  default =  [""]
}

variable "aws_key" {
  description = "The AWS public key to push into the instances for SSH/SCP."
  type = string
  default = ""
}
