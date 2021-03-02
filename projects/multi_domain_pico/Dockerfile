FROM ubuntu:18.04
ENV TERM linux
ENV DEBIAN_FRONTEND noninteractive
# Basic apt update
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends locales ca-certificates &&  rm -rf /var/lib/apt/lists/*
RUN apt-get update && apt-get -y install sudo

# make sure cmake is v12.0
RUN sudo apt -y install g++ wget make
ADD https://cmake.org/files/v3.14/cmake-3.14.1-Linux-x86_64.sh /cmake-3.14.1-Linux-x86_64.sh
RUN mkdir /opt/cmake
RUN sh /cmake-3.14.1-Linux-x86_64.sh --prefix=/opt/cmake --skip-license
RUN ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake
RUN cmake --version

# Install dependencies
RUN sudo apt update && sudo apt -y install gcc-arm-none-eabi \
      gcc \
      g++ \
      git \
      gdb-multiarch \
      automake \
      autoconf \
      build-essential \
      texinfo \
      libtool \
      libftdi-dev \
      wget \
      libusb-1.0-0-dev \
      python3 && \
      mkdir pico

# copy required files into docker image
COPY pico-sdk pico/pico-sdk
RUN mkdir pico/mult_domain_poc
COPY mult_domain pico/mult_domain_poc/mult_domain
COPY single_core pico/mult_domain_poc/single_core
COPY CMakeLists.txt pico/mult_domain_poc/CMakeLists.txt
COPY pico_sdk_import.cmake pico/mult_domain_poc/pico_sdk_import.cmake

#setup pico using pico_setup.sh
RUN OUTDIR="$(pwd)pico" && \
      export PICO_SDK_PATH="$OUTDIR/pico-sdk" && \
      cd $PICO_SDK_PATH && \
      echo "Adding PICO_SDK_PATH to ~/.bashrc" && \
      echo "export PICO_SDK_PATH=$PICO_SDK_PATH" >> ~/.bashrc

RUN OUTDIR="$(pwd)pico" && \
      export PICO_SDK_PATH="$OUTDIR/pico-sdk" && \
      mkdir -p pico/mult_domain_poc/build && \
      cd pico/mult_domain_poc/build && \
      cmake .. && \
      make -j8

# now build the model
#RUN OUTDIR="$(pwd)pico" && \
#      export PICO_SDK_PATH="$OUTDIR/pico-sdk" && \
#      cd pico/mult_domain_poc && mkdir build && \
#      cd build && cmake .. && \
#      cd mult_domain && make

#docker create -ti --name dummy pico_hello_world:1 bash
#docker cp dummy:/pico/pico-examples/build/hello_xor/hello_xor.uf2 gen/hello_xor.uf2
#docker rm -fv dummy
