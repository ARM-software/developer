variable "instance_type" {
  type = string
}

variable "instance_size" {
  type = string
}

variable "vpc" {
  type = string
}

variable "ami_id" {
  type = string
}

variable "key_name" {
  type = string
}

provider "aws" {
  profile = "default"
  region  = "us-east-2"
}

locals {
  redis-node-userdata = <<USERDATA
#!/bin/bash
sudo apt update
## This is ito use the default redis server on the distribution
# sudo apt install -y redis-server
# sudo sed -i 's/^bind 127.*$/bind 0\.0\.0\.0 ::1/' /etc/redis/redis.conf
# sudo sed -i 's/^save .*$/save ""/g' /etc/redis/redis.conf
# sudo systemctl restart redis.service

## This is to build the redis server from repository
sudo apt install -y make build-essential
cd $HOME

## Install GCC 10.2
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt install -y gcc-10
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 10

git clone --recursive --depth 1 --branch 6.0.9 https://github.com/redis/redis.git
cd redis
make
sed -i 's/^bind 127.*$/bind 0\.0\.0\.0 ::1/' redis.conf
echo 'save ""' >> redis.conf
./src/redis-server redis.conf
USERDATA

  memtier-node-userdata = <<USERDATA
#!/bin/bash
cd /tmp
sudo apt update
sudo apt install -y build-essential autoconf automake libpcre3-dev libevent-dev pkg-config zlib1g-dev libssl-dev git

## This is to use a released version
# wget https://github.com/RedisLabs/memtier_benchmark/archive/refs/tags/1.3.0.tar.gz
# tar -xvzf 1.3.0.tar.gz
# cd memtier_benchmark-1.3.0

## This is to use master branch
git clone https://github.com/RedisLabs/memtier_benchmark.git
cd memtier_benchmark
autoreconf -ivf
./configure
make
sudo make install
USERDATA
  common_tags = {
    Name = "redis_benchmark"
  }
}

resource "aws_security_group" "allow_ssh" {
  name        = "redis_benchmark_allow_ssh"
  description = "Allow SSH inbound traffic"
  vpc_id      = var.vpc

  ingress {
    description = "SSH from ARM VPN address"
    from_port   = 0
    to_port     = 22
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"] # better to be specific, for instance, only allow VPN access
  }

  ingress {
    description = "Connect to Redis Server from cluster"
    from_port   = 0
    to_port     = 6379
    protocol    = "tcp"
    cidr_blocks = ["172.31.100.0/24"]
  }

  egress {
    from_port   = 0
    to_port     = 0
    protocol    = "-1"
    cidr_blocks = ["0.0.0.0/0"]
  }

  tags = local.common_tags
}

# cluster placement for lower latency
resource "aws_placement_group" "redis" {
  name     = "redis-placement"
  strategy = "cluster"

  tags = local.common_tags
}

resource "aws_eip" "redis_server_eip" {
  instance          = aws_instance.redis_server.id
  network_interface = aws_network_interface.redis_net.id
  vpc               = true
}

resource "aws_eip" "redis_memtier_eip" {
  instance          = aws_instance.redis_memtier.id
  network_interface = aws_network_interface.redis_memtier_net.id
  vpc               = true
}

# define a subnet in VPC for the cluster
resource "aws_subnet" "redis_subnet" {
  vpc_id            = var.vpc
  cidr_block        = "172.31.100.0/24"
  availability_zone = "us-east-2a"

  tags = local.common_tags
}

# instances network interface
resource "aws_network_interface" "redis_net" {
  subnet_id       = aws_subnet.redis_subnet.id
  security_groups = [aws_security_group.allow_ssh.id]

  tags = {
    Name = "redis_network_interface"
  }
}

resource "aws_network_interface" "redis_memtier_net" {
  subnet_id       = aws_subnet.redis_subnet.id
  security_groups = [aws_security_group.allow_ssh.id]

  tags = {
    Name = "redis_memtier_network_interface"
  }
}

resource "aws_instance" "redis_server" {
  ami           = var.ami_id // ubuntu 20.04 LTS 64-bit
  instance_type = "${var.instance_type}.${var.instance_size}"
  user_data     = base64encode(local.redis-node-userdata)
  key_name      = var.key_name

  placement_group = aws_placement_group.redis.id

  network_interface {
    network_interface_id = aws_network_interface.redis_net.id
    device_index         = 0
  }

  tags = {
    Name = "redis_server"
  }
}

resource "aws_instance" "redis_memtier" {
  ami           = var.ami_id // ubuntu 20.04 LTS 64-bit
  instance_type = "${var.instance_type}.xlarge"
  user_data     = base64encode(local.memtier-node-userdata)
  key_name      = var.key_name

  placement_group = aws_placement_group.redis.id

  network_interface {
    network_interface_id = aws_network_interface.redis_memtier_net.id
    device_index         = 0
  }

  tags = {
    Name = "redis_memtier"
  }
}

# output DNS names
resource "local_file" "redis_server_ip" {
  content  = "${aws_eip.redis_server_eip.public_dns}\n${aws_eip.redis_server_eip.private_dns}"
  filename = "REDIS_SERVER_ADDRESSES"
}

resource "local_file" "redis_memtier_ip" {
  content  = "${aws_eip.redis_memtier_eip.public_dns}\n${aws_eip.redis_memtier_eip.private_dns}"
  filename = "REDIS_MEMTIER_ADDRESSES"
}
