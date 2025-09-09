# Project 01 of Visual Computing

## Development and Compilation

### On Windows

1. Create a `lib/` folder and clone the SDL3 repositories inside it&#x20;
   Example:

```cmd
mkdir lib && cd lib
git clone https://github.com/libsdl-org/SDL_image lib/SDL3_image
git clone https://github.com/libsdl-org/SDL lib/SDL3
```

2. Run the following commands to build the program:

```cmd
cmake -S . -B build
cmake --build build
```

3. The executable will be available in the following folder:

```cmd
build/bin/main
or
build/bin/main.exe
```

### On Linux

1. Install the necessary dependencies, available at: [https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md#build-dependencies](https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md#build-dependencies)

2. Follow the same compilation steps as for Windows.

3. Run with:

```bash
./build/bin/main assets/image.jpg
```

## Group Members

* Enzo Guarnieri                               RA
* Júlia Campolim Oste                          RA
* Gustavo Villela Mitraud                      RA
* Matheus Marçal Ramos de Oliveira             RA
* Sabrina Midori Futami Teixeira de Carvalho   RA 10410220
