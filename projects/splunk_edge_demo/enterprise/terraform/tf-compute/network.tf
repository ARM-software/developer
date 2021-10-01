# Create our VPC
resource "aws_vpc" "splunk_demo_vpc" {
  cidr_block = "10.0.0.0/16"
  tags = {
    Name = "splunk-demo-vpc"
  }
}

# Create a subnet in the VPC
resource "aws_subnet" "splunk_demo_subnet" {
  vpc_id            = aws_vpc.splunk_demo_vpc.id
  cidr_block        = "10.0.0.0/16"
  availability_zone = var.instance_az
  tags = {
    Name = "splunk-demo-subnet"
  }
}

# Create and connect internet gateway to the VPC
resource "aws_internet_gateway" "splunk_int_gw" {
  vpc_id = aws_vpc.splunk_demo_vpc.id
  tags = {
    Name = "splunk-demo-intgw"
  }
}

# Create a routing table that routes towards the internet gateway
resource "aws_route_table" "splunk_demo_route_table" {
  vpc_id = aws_vpc.splunk_demo_vpc.id
  route {
    cidr_block = "0.0.0.0/0"
    gateway_id = aws_internet_gateway.splunk_int_gw.id
  }

  tags = {
    Name = "splunk-demo-route-table"
  }
}

# Associate the internet gateway with the subnet created above via the route table created above
resource "aws_route_table_association" "splunk_gateway_association" {
  subnet_id      = aws_subnet.splunk_demo_subnet.id
  route_table_id = aws_route_table.splunk_demo_route_table.id
}
