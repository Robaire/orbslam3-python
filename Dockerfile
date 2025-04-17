FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Update apt
RUN apt-get update && apt-get install -y --no-install-recommends apt-utils

# Install build dependencies
RUN apt-get install -y \
    build-essential \
    git \
    cmake \
    libopencv-dev \
    libeigen3-dev \
    libgl1-mesa-dev \ 
    libegl1-mesa-dev \
    libepoxy-dev \
    libglew-dev \ 
    libjpeg-dev \ 
    libpng-dev \
    catch2 \
    libavcodec-dev \ 
    libavutil-dev \ 
    libavformat-dev \ 
    libswscale-dev \
    libavdevice-dev \
    python3-dev \
    python3-pip \
    libboost-all-dev \
    libssl-dev

RUN python3 -m pip install --upgrade pip && pip install setuptools numpy

# Install Pangolin
RUN git clone --recursive https://github.com/stevenlovegrove/Pangolin.git && \
    cd Pangolin && \
    cmake -B build && cmake --build build && cmake --install build

# Install ORBSLAM and python bindings
COPY . /orbslam3-python
RUN cd /orbslam3-python && pip install .
# RUN cd /orbslam3-python && python3 setup.py install

# Install ORBSLAM and python bindings
# RUN git clone --recursive https://github.com/robaire/orbslam3-python.git && \
#     cd orbslam3-python && pip install .