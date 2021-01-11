# Docker Setup 

The setup section covers how to install and use docker on various types of machine that may be on your desk, in the cloud, or at the edge. 
Before we dive into the setup details for different machines, it is helpful to have an overview of [Tool Requirements](#tool-requirements).

## Table of Contents

- [Tool Requirements](#tool-requirements)
- [Target: Workstation](#target-workstation)
   - [Arm Windows 10 laptop](#install-docker-on-a-windows-10-on-arm-laptop)
   - [Arm Linux laptop](#install-docker-on-arm-linux-laptop)
   - [Arm Chromebooks](#install-docker-on-arm-chromebooks)
   - [Windows 10 on x86_64](#install-docker-on-windows-10-on-x86_64)
   - [Linux on x86_64](#install-docker-on-linux-on-x86_64)
- [Target: Cloud](#target-in-the-cloud)
   - [AWS Graviton](#aws-graviton)
- [Target: Edge](#target-at-the-edge)
   - [Raspberry Pi 3 or 4](#raspberry-pi-3-or-4)
   - [Rock960](#rock960)

***

### Tool Requirements

Depending on the operating system you're using, there are different Docker products available to download.

***Docker products***

- [Docker Desktop for Windows and Mac](https://www.docker.com/products/docker-desktop)
- [Docker Engine for Linux](https://www.docker.com/products/container-runtime)

***Docker Engine versions***

- The Stable channel gives you latest releases for general availability.
- The Test channel gives pre-releases that are ready for testing before general availability (GA). Replace get.docker.com with test.docker.com below to get the test version.

### Install Docker
Let's proceed now with the installation of Docker. The Docker for Linux install procedure covers a majority of the target devices.  

### Docker for Linux install procedure

Almost universal, this procedure works for many Linux distributions. 

```shell
$ curl -fsSL get.docker.com -o get-docker.sh
$ sudo sh get-docker.sh
$ sudo usermod -aG docker your-user
$ newgrp docker (if you don’t want to log out and back in)
$ docker run hello-world
```

***Identify the architecture of any machine***
```shell
$ uname -m
```

If this Linux Install procedure does not work for you, or if you're using a Windows 10 x86_64 machine please select a different target device and proceed from there:
- [Target: Workstation](#target-workstation)
   - [Windows 10 on Arm laptop](#install-docker-on-a-windows-10-on-arm-laptop)
   - [Arm Linux laptop](#install-docker-on-arm-linux-laptop)
   - [Arm Chromebooks](#install-docker-on-arm-chromebooks)
   - [Windows 10 on x86_64](#install-docker-on-windows-10-on-x86_64)
   - [Linux on x86_64](#install-linux-on-x86_64)
- [Target: Cloud](#target-in-the-cloud)
   - [AWS Graviton](#aws-graviton)
- [Target: Edge](#target-at-the-edge)
   - [Raspberry Pi 3 or 4](#raspberry-pi-3-or-4)
   - [Rock960](#rock960)

***

## Target: Workstation

### Install Docker on a Windows 10 on Arm laptop

***Example systems are:***

- Microsoft Surface Pro X
- Samsung Galaxy Book S
- Lenovo Flex 5G
- Acer Spin 7

***Prerequisites***

- Install WSL 2 on the Windows 10 on Arm laptop. 
- Install the Ubuntu 20.04 Linux distribution in WSL 2 from the Microsoft Store.

***[More information available here](https://dev.to/aws-builders/windows-10-on-arm-with-wsl-2-2kbg)***

***Do it***

Use the general [Linux install procedure](#docker-for-linux-install-procedure)

```shell
$ sudo apt update 
$ curl -fsSL get.docker.com -o get-docker.sh
$ sudo sh get-docker.sh
$ sudo usermod -aG docker your-user
$ newgrp docker     (if you don’t want to log out and back in)
$ docker run hello-world
Docker daemon does not start automatically in WSL 2 but can be started manually:
$ sudo /etc/init.d/docker start
```

***How it works***

The Linux install will get the current docker release. [Review the script contents in a browser](https://get.docker.com/)

***More info***
- There are numerous Linux distribution options from the Microsoft Store.
- There is no Docker Desktop for Windows on Arm. Show your support for Docker Desktop on Arm [here](https://github.com/docker/roadmap/issues/91).
- For more information about [installing WSL 2](https://docs.microsoft.com/en-us/windows/wsl/install-win10)

***[Proceed to build and run instructions](/documentation/docker-guide/build.md/#windows-10-on-arm-laptop-or-arm-linux-laptop-or-arm-chromebook-with-linux)***

***

### Install Docker on Arm Linux laptop

***Example systems are:***

- Pinebook Pro

***Prerequisites***

- Pinebook Pro laptop running Manjaro Arm

The almost universal Linux install doesn’t recognize Manjaro on Arm, but pacman will install docker.

***Do it***

Use the general [Linux install procedure](#docker-for-linux-install-procedure)

```shell
$ sudo pacman -Syu 
$ sudo pacman -S docker
$ sudo systemctl enable docker
$ sudo systemctl start docker
$ sudo usermod -aG docker your-user
$ newgrp docker     (if you don’t want to log out and back in)
$ docker run hello-world
```

***How it works***

The pacman install will get the current docker release.

***More info***

Nothing right now. 

***[Proceed to build and run instructions](/documentation/docker-guide/build.md/#windows-10-on-arm-laptop-or-arm-linux-laptop-or-arm-chromebook-with-linux)***

***

### Install Docker on Arm Chromebooks

***Example systems are:***

- Acer Chromebook R13
- Lenovo Duet

***Prerequisites***

- Chromebooks can run Linux applications using the Linux feature. Go to the Chromebook Settings and enable Linux (Beta) feature. This will install the Linux subsystem, including a Debian 10 Buster distribution.
- Open the Linux terminal to use docker.

***Do it***

Use the general [Linux install procedure](#docker-for-linux-install-procedure)

```shell
$ sudo apt update 
$ curl -fsSL get.docker.com -o get-docker.sh
$ sudo sh get-docker.sh
$ sudo usermod -aG docker your-user
$ newgrp docker     (if you don’t want to log out and back in)
$ docker run hello-world
```

***[Proceed to build and run instructions](/documentation/docker-guide/build.md/#windows-10-on-arm-laptop-or-arm-linux-laptop-or-arm-chromebook-with-linux)***

***

### Install Docker on Windows 10 on x86_64

***Prerequisites***

- Install Docker Desktop with WSL2 backend

***Do it***

- Follow [this procedure](https://docs.docker.com/docker-for-windows/wsl/) to download and install Docker Desktop for Windows
- Docker Desktop includes the buildx plugin by default

***More Info***

More information on the Docker Desktop on Windows [User Manual](https://docs.docker.com/docker-for-windows/).

***[Proceed to build and run instructions](/documentation/docker-guide/build.md/#windows-10linux-on-x86_64-laptop)***

***

### Install Docker on Linux on x86_64

***Prerequisites***

- Docker on Linux 
- Docker buildx for multi-architecture image creation
- Instruction emulation(qemu) to register Arm executables to run on the x86 machine

***Do it***

- Use the general [Linux install procedure](#docker-for-linux-install-procedure) to install the latest verion 
- Install the instruction emulation to register Arm executables to run on the x86 machine 

```shell
$ docker run --rm --privileged docker/binfmt:820fdd95a9972a5308930a2bdfb8573dd4447ad3
```

***How it works***

- The docker/binfmt gets updated with the latest qemu and replaces any previous instructions that call for linuxkit/binfmt:v0.7

***More Info***

For more details [refer to the project](https://github.com/docker/binfmt)

***[Proceed to build and run instructions](/documentation/docker-guide/build.md/#windows-10linux-on-x86_64-laptop)***

***

## Target: In the cloud

### AWS Graviton

***Prerequisites***

- Amazon Web Services (AWS) Account

***Do it***

Create and start an AWS-Graviton based EC2 instance

On the running AWS-Graviton based EC2 instance [Use the Linux install procedure](#docker-for-linux-install-procedure)

***More Info***

[Getting started guide in GitHub](https://github.com/aws/aws-graviton-getting-started/blob/master/containers.md )
[Get started with AWS Graviton-based EC2 instances](https://aws.amazon.com/ec2/graviton/)

***[Proceed to build and run instructions](/documentation/docker-guide/build.md/#build-on-remote-machine-and-run-local)***

***

## Target: At the edge

### Raspberry Pi 3 or 4

***Do it***

[Use the Linux install procedure](#docker-for-linux-install-procedure)

***[Proceed to build and run instructions](/documentation/docker-guide/build.md/#run-on-raspberry-pi-3-or-4)***

***

### Rock960

[Use the Linux install procedure](#docker-for-linux-install-procedure)
