# Splunk enterprise host IP(s)
output "splunk_host_ips" {
  value = aws_instance.splunk_instances.public_ip
}

resource "local_file" "splunk_ips" {
  content = aws_instance.splunk_instances.public_ip
  filename = "../../ansible/host"
}

resource "local_file" "splunk_ent_ip" {
  content = aws_instance.splunk_instances.public_ip
  filename = "../../../fwd/ansible/splunk_ent_ip"
}
