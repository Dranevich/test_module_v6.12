# test_module
A simple example of Linux kernel module. Does nothing but writes some messages to it's own output file.

## Description
The goal of this project is to demonstrate the basic principes how Linux kernel modules work, including their specipic code structure, inserting and removing from the kernel as well as system requirements. The project had been developed as a part of internship Test Task in February, 2026.
Project includes:

- **test_module.c** - module source code

- **Makefile** - make instructions required for building the project

- **test_module_loader.c** - source code for user application used to setup modele parametres

- **README.md** - user documentation

- **test_module.zip** - an archive containing pre-compiled module files & source code.

## Features
- Writes standard messages to it's own output file every N seconds
- Output file name and N value are recieved from user

## Requirements
- Ubuntu Kernel version: 6.12 (6.12.0)
- Ubuntu Kernel headers (6.12)
- GCC version: 13.0
- CMake
- C language support

## Installation
### Linux Kernel v.6.12:
Just download the archieve from this repository and unzip them to your home directory:
```
git clone https://github.com/Dranevich/test_module_v6.12
unzip ~/test_module_v6.12/test_module.zip -d ~
```
### Another Linux Kernel version
0. If you're not sure which kernel version do you have, run
```
uname -r
```
Also you'll need to make sure you have Linux header files already installed:
```
ls /lib/modules/$(uname -r)/build
```
If the directory does not present, dounload required files:
```
sudo apt install linux-headers-$(uname -r)
```
1. Firstly, download the archive & unzip them:
```
git clone https://github.com/Dranevich/test_module_v6.12
unzip ~/test_module_v6.12/test_module.zip -d ~
```
2. Go to the kernel folder and clean the project. Then, build it again:
```
cd ~/test_module/kernel
make clean
make
```
3. You might also need to rebuild user program, too. To do that, run
```
cd ~/test_module/User
rm test_module_loader
gcc test_module_loader.c -o test_module_loader

## Usage
Go to the User folder and run test_module_loader. You'll be asked to enter output file name and timer period value in seconds:
```
cd ~/test_module/User
```
./test_module_loader [filename] [period]
For checking output file, run:
cat /var/tmp/test_module/[filename]
```
To remove the module from the kernel, run:
```
sudo rmmod test_module
```

# About the author
Author: Dranevich Anastasya

Developed as part of the internship Test Task.

Technologies: C, Linux, Make, gcc
