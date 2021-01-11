# Docker Guide for Arm developers

Docker containers are everywhere, primarily because they run the same everywhere. Containers are used on all major operating systems, on all major computing architectures to build, share, and run software. 
As more and more of the world’s computing happens on the Arm architecture, developers are looking for resources to make container development and deployment as easy as possible. 

We start by walking you through installing and deploying Docker on a variety of target machines. We then demonstrate how to use Docker to build, share and run containers using the simple examples provided. 
We hope you find these ideas helpful.

### [Get Started](/documentation/docker-guide/setup.md#docker-setup)

## Table of Contents

- [Setup](/documentation/docker-guide/setup.md)
   - [Tool Requirements](/documentation/docker-guide/setup.md#tool-requirements)
   - [Workstation](/documentation/docker-guide/setup.md#target-workstation)
      - [Arm Windows 10 laptop](/documentation/docker-guide/setup.md#install-docker-on-a-windows-10-on-arm-laptop)
      - [Arm Linux laptop](/documentation/docker-guide/setup.md#install-docker-on-arm-linux-laptop)
      - [Arm Chromebooks](/documentation/docker-guide/setup.md#install-docker-on-arm-chromebooks)
      - [Windows 10 on x86_64](/documentation/docker-guide/setup.md/#install-docker-on-windows-10-on-x86_64)
      - [Linux on x86_64](/documentation/docker-guide/setup.md#install-docker-on-linux-on-x86_64)
   - [Cloud](/documentation/docker-guide/setup.md#target-in-the-cloud)
      - [AWS Graviton](/documentation/docker-guide/setup.md#aws-graviton)
   - [Edge](/documentation/docker-guide/setup.md#target-at-the-edge)
      - [Raspberry Pi 3 or 4](/documentation/docker-guide/setup.md#raspberry-pi-3-or-4)
      - [Rock960](/documentation/docker-guide/setup.md#rock960)
- [Build and Run](/documentation/docker-guide/build.md#build-and-run) 
   - [Windows 10 on Arm laptop OR Arm Linux laptop OR Arm Chromebook with Linux](/documentation/docker-guide/build.md#windows-10-on-arm-laptop-or-arm-linux-laptop-or-arm-chromebook-with-linux)
   - [Windows 10/Linux on x86_64 laptop](/documentation/docker-guide/build.md#windows-10linux-on-x86_64-laptop)
      - [Build and Run local](/documentation/docker-guide/build.md#build-and-run-local)
      - [Build on Remote machine and Run local](/documentation/docker-guide/build.md#build-on-remote-machine-and-run-local)
   - [Use GitHub actions to build and push Arm-based docker images](/documentation/docker-guide/build.md#use-github-actions-to-build-and-push-arm-based-docker-images)
      - [Use buildx to build multi-architecture Arm images](/documentation/docker-guide/build.md#use-buildx-to-build-multi-architecture-arm-images)
      - [Use build with Arm self-hosted runner](/documentation/docker-guide/build.md#use-build-with-arm-self-hosted-runner)
