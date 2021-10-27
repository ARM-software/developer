resource "aws_security_group" "splunk_allow_ssh" {
  name        = "splunk_allow_ssh"
  description = "Allows SSH sessions"
  vpc_id = aws_vpc.splunk_demo_vpc.id

  ingress {
      description      = "SSH from VPC"
      from_port        = 22
      to_port          = 22
      protocol         = "tcp"
      cidr_blocks      = var.ingress_ips
  }

  tags = {
    Name = "splunk_allow_ssh"
  }
}

resource "aws_security_group" "splunk_allow_ui" {
  name        = "splunk_allow_ui"
  description = "Allows connection to Splunk admin GUI"
  vpc_id = aws_vpc.splunk_demo_vpc.id

  ingress {
      description      = "Ingress to splunk admin GUI"
      from_port        = 8000
      to_port          = 8000
      protocol         = "tcp"
      cidr_blocks      = var.ingress_ips
  }

  tags = {
    Name = "splunk_allow_ui"
  }
}

resource "aws_security_group" "splunk_receiver" {
  name        = "splunk_receiver"
  description = "Port(s) to receive forwarder data on"
  vpc_id = aws_vpc.splunk_demo_vpc.id

  ingress {
      description      = "Port range for receiving forwarder data"
      from_port        = 9997
      to_port          = 9997
      protocol         = "tcp"
      cidr_blocks      = var.ingress_ips
  }

  tags = {
    Name = "splunk_allow_ui"
  }
}

resource "aws_security_group" "splunk_allow_deployment_clients" {
  name        = "splunk_allow_deployment_clients"
  description = "Port to allow deployment clients to connect to the deployment server we are hosting."
  vpc_id = aws_vpc.splunk_demo_vpc.id

  ingress {
      description      = "Port range for deployment clients"
      from_port        = 8089
      to_port          = 8089
      protocol         = "tcp"
      cidr_blocks      = var.ingress_ips
  }

  tags = {
    Name = "splunk_allow_ui"
  }
}

resource "aws_security_group" "splunk_allow_egress" {
  name        = "splunk_egress"
  description = "Allows egress"
  vpc_id = aws_vpc.splunk_demo_vpc.id

  egress {
      from_port        = 0
      to_port          = 0
      protocol         = "-1"
      cidr_blocks      = ["0.0.0.0/0"]
      ipv6_cidr_blocks = ["::/0"]
  }

  tags = {
    Name = "splunk_allow_egress"
  }
}
