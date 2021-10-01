# Splunk enterprise instance(s)
resource "aws_instance" "splunk_instances" {
  # Assign the type, ami and name of each instance.
  # migrated tags are for credits with AWS
  instance_type = var.enterprise_instance_type
  ami = var.enterprise_instance_ami
  availability_zone = var.instance_az
  tags = {
    Name = "arm-splunk-demo",
  }

  # Network
  subnet_id = aws_subnet.splunk_demo_subnet.id
  associate_public_ip_address = true

  # Security & SSH
  vpc_security_group_ids = [
    aws_security_group.splunk_allow_ssh.id,
    aws_security_group.splunk_allow_egress.id,
    aws_security_group.splunk_allow_ui.id,
    aws_security_group.splunk_allow_deployment_clients.id,
    aws_security_group.splunk_receiver.id
  ]
  key_name = var.aws_key

  # Storage
  root_block_device {
    volume_size = 100
  }
}
