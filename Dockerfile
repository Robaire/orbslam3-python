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
    python3-pybind11 \
    libboost-all-dev \
    libssl-dev

RUN python3 -m pip install --upgrade pip

# Install ORBSLAM and python bindings
COPY . /orbslam3-python
RUN cd /orbslam3-python && pip install -e .
