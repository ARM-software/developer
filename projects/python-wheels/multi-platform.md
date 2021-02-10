# Build Python software to target multiple-platforms

## Why do I need to care about the platform when I use Python?

Python programs and libraries are written as human readable text. This text is compiled by the Python interpreter to produce 'pyc' files. These pyc files are typically executed by Python's virtual machine. This design means that programs written in Python will run anywhere the Python virtual machine is present. In some cases, the behavior of the Python virtual machine is unsuitable for the application being written. Most commonly, library designers choose to bypass the Python virtual machine to improve performance.

A popular library that has a Python virtual machine bypass is [lxml](https://lxml.de/). This library provides XML tooling to a Python application by exposing the popular C libraries libxml2 and libxslt. Because there is a dependency on C libraries, if you build an application in Python using lxml, your Python application will also need these C libraries. C libraries must be compiled for the platform (AArch64, x86, etc) that they are run on.

One of the challenges software engineers face today is maintaining platform neutrality. Platform neutral applications are cheaper to port to a new target. Moving to a new target can save money or improve one or more performance characteristics. As the network edge grows in importance as a venue to host low-latency, high-bandwidth applications the ability to run code at the edge becomes a commercial advantage. Platform neutral code enables a developer to gain advantage as they can be first to run on the edge regardless of the underlying architecture (AArch64, x86, or other).

## Python libraries and wheels

Installing [pandas](https://pandas.pydata.org/) on Aarch64 is as simple as `pip install pandas`. When you issue this command, the Python package installer (pip) visits the [Python Package Index](https://pypi.org/) to get the artifacts to provide the pandas library. You can see these directly at [https://pypi.org/project/pandas/#files](https://pypi.org/project/pandas/#files). These artifacts are know as [wheels](https://pythonwheels.com/). If pip finds a wheel that matches the platform pip is running on, it downloads and installs that artifact. If pip does not find a matching artifact it downloads the source and builds the library locally. The library that is installed on the system is always the same irrespective if it is downloaded or built from source, but building from source is significantly slower.

If the library does not include any native code wheels are still helpful as they include package metadata. The library [requests](https://pypi.org/project/requests/) is an example where there is no native code. In this case the wheel is marked as 'any', and takes the same time to install on any platform. You can see an 'any' wheel here: [https://pypi.org/project/requests/#files](https://pypi.org/project/requests/#files)

## Python libraries and Conda

TBD

## Building wheels for AArch64 on AArch64

### Native compilation

You proceed through the wheel building process on an AArch64 machine. This is straightforward and requires the wheel builder to have access to an AArch64 build machine. 

```
pip install wheel
python setup.py sdist bdist_wheel
```

### Building a wheel from a CI/CD system that supports AArch64

Travis-CI includes support for AArch64 builders. You can generate a wheel during the deploy phase of the Travis-CI task. A complete example for of this is here:

[https://github.com/mpmkp2020/numpy_demo/blob/master/.travis.yml](https://github.com/mpmkp2020/numpy_demo/blob/master/.travis.yml)

You can use multibuild to build linux wheel on AArch64 architecture same as on x86 machine. A complete example for using multibuild is here:
[https://github.com/python-cffi/cffi-travis-wheel](https://github.com/python-cffi/cffi-travis-wheel). It's [README](https://github.com/python-cffi/cffi-travis-wheel/blob/master/README.md) explains using travis-ci to upload wheel on pypi as well.

## Building AArch64 wheels on x86

### Run a AArch64 native container on x86 with emulation

There is a containerized environment available to run on AArch64 to build wheels to the current specification. The container is available here: [https://quay.io/repository/pypa/manylinux2014_aarch64](https://quay.io/repository/pypa/manylinux2014_aarch64). To run an AArch64 container on x86 requires an emulator. Instructions for using the QEMU emulator to run a container are here: [https://github.com/multiarch/qemu-user-static](https://github.com/multiarch/qemu-user-static).

```
docker run --rm --privileged multiarch/qemu-user-static --reset -p yes
```
Now we clone the project we care about. In this case, I’m using the popular Kiwi implementation of the Cassowary constraint solving algorithm: kiwisolver. 
```
git clone https://github.com/nucleic/kiwi 
cd kiwi 
```
Now we can call the AArch64 manylinux2014 environment:
```
$ docker run --rm -v `pwd`:/io quay.io/pypa/manylinux2014_aarch64 bash -c '/opt/python/cp38-cp38/bin/python ./setup.py bdist_wheel'
running bdist_wheel
running build
running build_ext
building 'kiwisolver' extension
creating build
creating build/temp.linux-aarch64-3.8
creating build/temp.linux-aarch64-3.8/py
gcc -pthread -Wno-unused-result -Wsign-compare -DNDEBUG -g -fwrapv -O3 -Wall -fPIC -I/io/.eggs/cppy-1.1.0-py3.8.egg/cppy/include -I. -I/opt/python/cp38-cp38/include/python3.8 -c py/kiwisolver.cpp -o build/temp.linux-aarch64-3.8/py/kiwisolver.o -std=c++11
gcc -pthread -Wno-unused-result -Wsign-compare -DNDEBUG -g -fwrapv -O3 -Wall -fPIC -I/io/.eggs/cppy-1.1.0-py3.8.egg/cppy/include -I. -I/opt/python/cp38-cp38/include/python3.8 -c py/constraint.cpp -o build/temp.linux-aarch64-3.8/py/constraint.o -std=c++11
...
creating build/bdist.linux-aarch64/wheel/kiwisolver-1.2.0.dist-info/WHEEL
creating 'dist/kiwisolver-1.2.0-cp38-cp38-linux_aarch64.whl' and adding 'build/bdist.linux-aarch64/wheel' to it
adding 'kiwisolver.cpython-38-aarch64-linux-gnu.so'
adding 'kiwisolver-1.2.0.dist-info/LICENSE'
adding 'kiwisolver-1.2.0.dist-info/METADATA'
adding 'kiwisolver-1.2.0.dist-info/WHEEL'
adding 'kiwisolver-1.2.0.dist-info/top_level.txt'
adding 'kiwisolver-1.2.0.dist-info/RECORD'
removing build/bdist.linux-aarch64/wheel
```

The wheel is present here: ```./dist/kiwisolver-1.2.0-cp38-cp38-linux_aarch64.whl ```


### Modify the x86 wheel building environment to include a cross-compiler

To follow.


### Using dockcross

[Dockcross](https://github.com/dockcross/dockcross/) provide cross compilers as Docker images. The work to make these usable for building wheels is on going and includes:

* fix-up the linux-arm64 base image with the library version necessary for PEP-0599.
* inherit linux-arm64 into the manylinux2014-aarch64 image.

It is possible to use the existing dockcross/manylinux2014-aarch64 image. First, prepare the dockcross cross compiler image so the it can be convieniently used locally:
```
docker run --rm dockcross/manylinux2014-aarch64 > ./buildaarch64wheel 
chmod u+x ./buildaarch64wheel 
```
Now we clone the project we care about. In this case, I’m using the popular Kiwi implementation of the Cassowary constraint solving algorithm: kiwisolver. 
```
git clone https://github.com/nucleic/kiwi 
cd kiwi 
```
Finally, we can use the dockcross cross compiler to build the wheel with Python 3.8.5 as of time of writing:
```
$ ../buildaarch64wheel bash -c '/opt/_internal/cpython-3.8.5/bin/python ./setup.py bdist_wheel --plat-name linux_aarch64' 
running bdist_wheel 
running build 
running build_ext 
building 'kiwisolver' extension 
... 
creating 'dist/kiwisolver-1.2.0-cp38-cp38-linux_aarch64.whl' and adding 'build/bdist.linux-x86_64/wheel' to it 
adding 'kiwisolver.cpython-38-x86_64-linux-gnu.so' 
adding 'kiwisolver-1.2.0.dist-info/LICENSE' 
adding 'kiwisolver-1.2.0.dist-info/METADATA' 
adding 'kiwisolver-1.2.0.dist-info/WHEEL' 
adding 'kiwisolver-1.2.0.dist-info/top_level.txt' 
adding 'kiwisolver-1.2.0.dist-info/RECORD' 
removing build/bdist.linux-x86_64/wheel 
```

The wheel is present here ```./dist/kiwisolver-1.2.0-cp38-cp38-linux_aarch64.whl ```

