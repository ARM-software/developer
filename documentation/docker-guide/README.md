# Docker Guide for Arm developers

Docker containers are everywhere, primarily because they run the same everywhere. Containers are used on all major operating systems, on all major computing architectures to build, share, and run software. 
As more and more of the world’s computing happens on the Arm architecture, developers are looking for resources to make container development and deployment as easy as possible. 
We hope you find these ideas helpful.

## Table of Contents
1) Setup (setup.md)
   1.1) Workstation
      1.1.1) Install Docker on a Windows 10 on Arm laptop (--> 2.1)
      1.1.2) Arm Linux laptop (--> 2.1)
      1.1.3) Arm Chromebooks (--> 2.1)
      1.1.4) Windows 10 on x86_64 (--> 2.2)
      1.1.5) Linux on x86_64 (--> 2.2)
   1.2) Cloud
      1.2.1) AWS Graviton
      1.2.2) Setup Graviton as remote builder
   1.3) Edge
      1.3.1) Raspberry Pi 3 or 4
      1.3.2) Rock960
   1.4) (Optional) Docker Experimental features
2) Build (build.md) 
   2.1) Windows 10 on Arm laptop OR Arm Linux laptop OR Arm Chromebook with Linux
   2.2) Windows 10/Linux on x86_64 laptop
      2.2.1) Build and run local
      2.2.2) Build on Remote machine and Run local
   2.3) Use GitHub actions to build and push Arm-based docker images
      2.3.1) Use buildx to build multi-architecture Arm images
      2.3.2) Use build with Arm self-hosted runner

## Docker Setup 

The setup section covers how to install and use docker on various types of machine that may be on your desk, in the cloud, or at the edge. 
Before the setup details for different machines, it is helpful to have an overview of the setup landscape.

### Setup landscape

Docker products

- Docker Desktop for Windows and Mac  https://www.docker.com/products/docker-desktop
- Docker engine for Linux      https://www.docker.com/products/container-runtime

Docker Desktop versions:

- Stable
- Edge

Docker Engine versions

- The Stable channel gives you latest releases for general availability.
- The Test channel gives pre-releases that are ready for testing before general availability (GA). Replace get.docker.com with test.docker.com below to get the test version.

### Docker for Linux install procedure

Almost universal, this works for many Linux distributions. 

``
$ curl -fsSL get.docker.com -o get-docker.sh
$ sudo sh get-docker.sh
$ sudo usermod -aG docker your-user
$ newgrp docker (if you don’t want to log out and back in)
$ docker run hello-world
``


Identify the architecture of any machine
``
$ uname -m
``

Arm machines will be:
``aarch64 or armv7a``
