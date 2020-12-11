## Table of Contents
- [Windows 10 on Arm laptop OR Arm Linux laptop OR Arm Chromebook with Linux](#windows-10-on-arm-laptop-or-arm-linux-laptop-or-arm-chromebook-with-linux)
- [Windows 10/Linux on x86_64 laptop]()
   - [Build and run local]()
   - [Build on Remote machine and Run local]()
- [Use GitHub actions to build and push Arm-based docker images]()
   - [Use buildx to build multi-architecture Arm images]()
   - [Use build with Arm self-hosted runner]()

***

## Build and Run

### Windows 10 on Arm laptop OR Arm Linux laptop OR Arm Chromebook with Linux

Build a simple hello world example which prints the architecture of the machine. This is exactly the same on Windows 10 on Arm using WSL2, an Arm Linux laptop, or a Raspberry Pi 3 or 4 a running 64-bit operating system.

***Prerequisites***

- Get an example from git and select a language of interest. Choices are C, flask, golang, and PHP.

```shell
$ git clone https://github.com/jasonrandrews/hello-arm.git
$ cd hello-arm
```

***Do it***

***For the C language:***

```shell
$ cd c-hello-world
$ ./build.sh
$ ./run.sh
```

***How it works***

This is a local docker image build and run on a Windows 10 on Arm laptop using WSL. It looks just like Linux and will create an Arm image and run a container to print the architecture of the machine from the uname command. 

### Windows 10/Linux on x86_64 laptop

***

### Build and Run local

***Prerequisites***

Get the C language hello-world example

```shell
$ git clone https://github.com/pareenaverma/docker-projects
```

***Do it***

```shell
$ cd c-hello-world
```

Make sure to edit the scripts to change the Docker ID and make sure to login to Docker Hub so the images will get pushed

```shell
$ ./build.sh
$ ./run.sh 
```

***

### Build on Remote machine and Run local

***Prerequisites***

- Docker installed on remote machine
- SSH-Key Authorization: SSH access to a remote docker host with a key-based authentication

***For Linux machines:***
```shell
$ eval $(ssh-agent)
$ ssh-add <ssh-private-key>
```

***For Windows:***
```shell
$ start-ssh-agent.cmd
$ ssh-add <ssh-private-key>
```

***Do it***

```shell
$ cd c-hello-world
```

Edit remote-build.sh to point to the username and IP address of your AWS Graviton1/Graviton2 instance

```shell
$ ./remote-build.sh
$ docker context use default
$./local-run.sh
```

***

### Use GitHub actions to build and push Arm-based docker images

#### Use buildx to build multi-architecture Arm images

***Prerequisites***

- A GitHub account
- A Docker Hub Account – a registry to push your image to
- A sample workflow and C language [hello world example](https://github.com/pareenaverma/docker-projects)

***Do It***

- Fork the https://github.com/pareenaverma/docker-projects repository
- The GitHub actions are in a specific directory .github/workflows
- Inspect the dockerbuildimg.yml file – this file contains the GitHub actions needed to build and push the Arm docker image for the c-hello-world to your Docker Hub account
- Add the DOCKER_USERNAME and DOCKER_PASSWORD secrets to your GitHub repository. These point to your username and password of your DockerHub account.
   - Follow the steps under [“Creating encrypting secrets for a repository”](https://docs.github.com/en/free-pro-team@latest/actions/reference/encrypted-secrets) 
- Commit any simple change to your forked repository.
   - The example workflow is setup to trigger when you push code to your repository. 
- Go to the ‘Actions” menu on your repository to view the live actions when you commit a change.
- The Arm-docker image “c-hello-world-github" is built and pushed to the DockerHub registry. You can also log into Docker Hub and see your image pushed there.

***How it works***

The first step in the workflow is to check out the code from the repo, this will bring our codebase into our build environment for GitHub Actions. Then we pull in an Action which gives us the ability to run docker buildx. We then set some environment variables that are used by the docker commands. The next Action is to login into DockerHub with your encrypted secrets. Final step is running docker buildx to build/push our c-hello-world-github image into the DockerHub registry.

***

#### Use build with Arm self-hosted runner

The previous approach uses the GitHub runner to run the GitHub Actions. You can host your own runners and customize the environment used to run jobs in your GitHub Actions workflows. In this example we’ll detail how to build Arm-based Docker images with an Arm self-hosted runner in your GitHub Actions workflows.

***Prerequisites:***

- Linux Arm machine or AWS Graviton1/Graviton2 instance to be used as the self-hosted runner
- A GitHub account
- A Docker Hub Account – a registry to push your image to
- A sample workflow and C language [hello world example](https://github.com/pareenaverma/docker-projects)

***Do It***

- Fork [this repository](https://github.com/pareenaverma/docker-projects)
- Follow the steps under Adding a self-hosted runner to a repository. On step 5 select Operating System: Linux and Architecture: ARM64
- Follow the rest of the instructions to download, configure and run your self-hosted Arm runner
- Now inspect the ```.github/workflows/selfhostedrunner.yml``` file. This file contains the GitHub actions needed to build and push the Arm docker image for the c-hello-world to your Docker Hub account using the self-hosted runner.
- Add the ```DOCKER_USERNAME and DOCKER_PASSWORD``` secrets to your GitHub repository. These point to your username and password of your DockerHub account. Follow the steps under [“Creating encrypting secrets for a repository”](https://docs.github.com/en/free-pro-team@latest/actions/reference/encrypted-secrets) 
- Commit any simple change to your forked repository. The example workflow is setup to trigger when you push code to your repository.  Make sure your self-hosted runner is running.
- Go to the ‘Actions” menu on your repository to view the live actions when you commit a change.
- The Arm-docker image “c-hello-world-github" is built and pushed to the DockerHub registry. You can also log into Docker Hub and see your image pushed there.
