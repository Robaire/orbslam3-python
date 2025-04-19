# About
Python bindings for ORBSLAM3.
These bindings use a modified version of ORBSLAM with a gravity value of 1.625 m/s² for IMU measurements, additional functions to extract camera trajectories, and examples removed.
Based on this [repo](https://github.com/xingruiyang/ORB-SLAM3-python).

# Instructions
1. Clone submodules: `git submodule init && git submodule update`
2. Install C++ Dependencies: [OpenCV](https://opencv.org/) >= 4.4, [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) >= 3.1, and [Pangolin](https://github.com/stevenlovegrove/Pangolin)
3. Install Python Dependencies: [numpy]() and [setuptools]()
4. Build bindings: `pip install .`

# ToDo
- [X] Update CMake configuration and version to build
- [X] Update orbslam version
- [X] Patch orbslam for trajectory extraction
- [X] Patch gravity value in orbslam
- [X] Update pybind11
- [X] Add Stereo tracking binding with IMU data
- [ ] Build on MacOS
