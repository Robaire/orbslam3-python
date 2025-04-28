
# Step 1: Clone the repository and its submodules
git submodule init && git submodule update

# Step 2: Install Pangolin
sudo apt update
sudo apt install build-essential cmake git libgl1-mesa-dev libglew-dev pkg-config libegl1-mesa-dev libwayland-dev libxkbcommon-dev wayland-protocols libepoxy-dev
git clone https://github.com/stevenlovegrove/Pangolin.git
cd Pangolin
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j3
sudo make install

# Step 3: Install system-level dependencies (for Ubuntu/Debian-based systems) for orb slam
cd ../..
sudo apt update
sudo apt install -y \
    libopencv-dev \
    libeigen3-dev \
    libglew-dev \
    cmake \
    pkg-config \
    python3-dev \
    python3-pip \
    libpython3-dev \
    libx11-dev \
    libgl1-mesa-dev \
    libqt5opengl5-dev \
    libpangolin-dev  # Pangolin is sometimes available directly

# Step 3: Install Python dependencies
pip install numpy setuptools

# Step 4: Build Python bindings
pip install .
