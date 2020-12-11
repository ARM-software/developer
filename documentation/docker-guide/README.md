# Docker Guide for Arm developers

Docker containers are everywhere, primarily because they run the same everywhere. Containers are used on all major operating systems, on all major computing architectures to build, share, and run software. 
As more and more of the world’s computing happens on the Arm architecture, developers are looking for resources to make container development and deployment as easy as possible. 

We hope you find these ideas helpful.

[Get Started](/documentation/docker-guide/setup.md)

## Table of Contents

- [Setup](/documentation/docker-guide/setup.md)
   - [Linux users](/documentation/docker-guide/setup.md#setup-landscape)
   - [Workstation](/documentation/docker-guide/setup.md#target-workstation)
      - [Install Docker on a Windows 10 on Arm laptop]()
      - Arm Linux laptop]()
      - Arm Chromebooks]()
      - Windows 10 on x86_64]()
      - Linux on x86_64]()
   - [Cloud]()
      - AWS Graviton]()
      - Setup Graviton as remote builder]()
   - [Edge]()
      - Raspberry Pi 3 or 4]()
      - Rock960]()
- [Build]() 
   - [Windows 10 on Arm laptop OR Arm Linux laptop OR Arm Chromebook with Linux]()
   - [Windows 10/Linux on x86_64 laptop]()
      - [Build and run local]()
      - [Build on Remote machine and Run local]()
   - [Use GitHub actions to build and push Arm-based docker images]()
      - [Use buildx to build multi-architecture Arm images]()
      - [Use build with Arm self-hosted runner]()
