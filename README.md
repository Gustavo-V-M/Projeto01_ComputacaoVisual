# Project 01 of Visual Computing

## Development and Compilation

### On Windows

1. Create a `lib/` folder and clone the SDL3 repositories inside it&#x20;
   Example:

```cmd
mkdir lib && cd lib
git clone https://github.com/libsdl-org/SDL_image lib/SDL3_image
git clone https://github.com/libsdl-org/SDL lib/SDL3
git clone https://github.com/libsdl-org/SDL_ttf.git lib/SDL3_ttf
```

2. Run the following commands to build the program:
```
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

| Nome | RA |
| --- | --- |
| Enzo Guarnieri | 10410074 |
| Júlia Campolim Oste | 10408802 |
| Gustavo Villela Mitraud | 10400866 |
| Matheus Marçal Ramos de Oliveira | 10409001 |
| Sabrina Midori Futami Teixeira de Carvalho | 10410220 |
