# RPG World Simulator

Experiment on simulating RPG world NPC behavioural emergence.

## Usage

Fetch submodules:
```
git submodule update --init --recursive
```

Install dependencies (on Ubuntu linux):
```
sudo apt install libsdl2-dev libeigen3-dev
```

Build:
```
sudo apt install ninja-build
mkdir build
cd build/
cmake .. -GNinja
ninja -j0
./rpg_world_simulator
```
