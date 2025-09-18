# Project 01 of Visual Computing

## Group Members

| Nome                                       | RA       |
| ------------------------------------------ | -------- |
| Enzo Guarnieri                             | 10410074 |
| Júlia Campolim Oste                        | 10408802 |
| Gustavo Villela Mitraud                    | 10400866 |
| Matheus Marçal Ramos de Oliveira           | 10409001 |
| Sabrina Midori Futami Teixeira de Carvalho | 10410220 |

## Development and Compilation

### On Windows

1. Clone the repository using the following command:

```cmd
git clone --recurse-submodules <repository-url>
```

2. Run the following commands to build the program:

```
cmake -S . -B build
cmake --build build
```

3. The executable will be available in `build/bin/main`, but we also copy it to the root project (`/`).

4. Run the program using

```bash
./main assets/<image-name>
```

### On Linux

1. Install the necessary dependencies, available at: [https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md#build-dependencies](https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md#build-dependencies)

2. Follow the same compilation and executing steps as for Windows.
