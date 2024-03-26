# CubeTimer

CubeTimer is a simple timer application made in C++ using SFML library to time solves and save them in times.txt

## Installation

### Prerequisities
- GCC (GNU Compiler Collection)
- SFML library

### Steps

1. Clone this reposiory:
```sh
git clone https://github.com/vh8t/cubetimer_app
```

2. Install SFML library

#### Windows
- Download SFML from https://www.sfml-dev.org/download.php
- Extract the downloaded file and place it in a convenient location.
- Add the SFML bin directory to your system's PATH environment variable.

#### Linux
- **Ubuntu/Debian**:
```sh
sudo apt-get install libsfml-dev
```

- **Fedora**:
```sh
sudo dnf install sfml-devel
```

- **Arch Linux**:
```sh
sudo pacman -S sfml
```

#### macOS
- **Homebrew**:
```sh
brew install sfml
```

- **Without Homebrew**:
  - Download SFML from https://www.sfml-dev.org/download.php
  - Extract he downloaded file.
  - Move the extracted folder to location like `/usr/local/` or `/opt/`
  - Update your `LD_LIBRARY_PATH` environment variable to include the SFML library directory.

3. Compile the main.cpp file using GCC:
```sh
g++ main.cpp -o CubeTimer -lsfml-graphics -lsfml-window -lsfml-system
```

## Usage

After compilation run the application:
```sh
./CubeTimer
```

The solves will be saved in `./times.txt`
