# VChip - A CHIP-8 Emulator

**VChip** is a basic,portable CHIP-8 emulator written in C++. It aims to be simple, accurate, functional and serves as a great introduction for learning basic principles of the working of emulators.
## 🕹 What is CHIP-8?
CHIP-8 is an interpreted programming language which was developed in the 1970s by Joseph Weisbrecker on an 1802 microprocessor.It was used on COSMAC VIP and Telmac 1800, both of which were 8 bit microcomputers.
It consists of 16 8-bit registers, 4kB of memory, a hexadecimal keypad, a monochrome display spanning 64x32 pixels.
## Features
- *Accurate CHIP-8 instruction set support*
- *Keyboard input handling*
- *64x32 Monochromatic Display*
- *ROM loading support*
## Getting Started
## Prerequisites
*SDL 2* 
## Build & Run
Clone the repo

git clone https://github.com/Verticraze/VChip.git

cd VChip

# Build (if applicable)
make   

cd bin

./main <ROMFILE>

# Controls

CHIP-8 Keypad	Keyboard Mapping
1 2 3 C	  1 2 3 4
4 5 6 D	  Q W E R
7 8 9 E	  A S D F
A 0 B F	  Z X C V

# ROMs

Place your ROMs inside bin folder.

## TODO

Add Super CHIP-8 support (Hi-res mode)
Sound (beep) support
Save/load state

## Contributing

If you’d like to contribute to VChip:
1.Fork the repo
2.Create a new branch (git checkout -b feature/your-feature)
3.Commit your changes (git commit -m 'Add feature')
4.Push to the branch (git push origin feature/your-feature)
5.Open a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

# Contact

Verticraze
Email: verticraze@outlook.com
