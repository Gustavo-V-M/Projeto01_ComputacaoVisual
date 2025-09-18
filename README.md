# Project 01 of Visual Computing

---

#### Class 07N - Project n. 01: Image Processing

#### Professor: Andre Kishimoto

#### Group Members

|                    Nome                    |    RA    |
| :----------------------------------------: | :------: |
|               Enzo Guarnieri               | 10410074 |
|            Júlia Campolim Oste             | 10408802 |
|          Gustavo Villela Mitraud           | 10400866 |
|      Matheus Marçal Ramos de Oliveira      | 10409001 |
| Sabrina Midori Futami Teixeira de Carvalho | 10410220 |

---

### Summary

1. [About the project](#1-about-the-project)<br>
   1.1. [General aspects and technical requirements](#11-general-aspects-and-technical-requirements)<br>
   1.2. [Loading the image](#12-loading-the-image)<br>
   1.3. [Analyzing and converting to grayscale](#13-analyzing-and-converting-to-grayscale)<br>
   1.4. [Graphical User Interface with two windows](#14-graphical-user-interface-with-two-windows)<br>
   1.5. [Analyzing and displaying the histogram](#15-analyzing-and-displaying-the-histogram)<br>
   1.6. [Histogram equalization](#16-histogram-equalization)<br>
   1.7. [Saving the image](#17-saving-the-image)<br>
2. [Compiling and running](#2-compiling-and-running)<br>
   2.1. [On Windows](#21-on-windows)<br>
   2.2. [On Linux](#22-on-linux)<br>

---

## 1. About the project

---

### 1.1. General aspects and technical requirements

This application consists of opening two windows: one displaying a grayscale image (the program automatically converts the image to grayscale if it is not already), and another displaying the corresponding histogram that represents the image’s intensity values.

The following requirements were defined:

- Language: C++
- Mandatory libraries: SDL3 and SDL_image
- Compiler: g++
- Code quality: Code organized into structures and functions with clear responsibilities; proper memory management.
- Documentation: A complete and detailed README.
- Repository: Hosted on GitHub.

The group decided to divide the project tasks as follows:

- Image loading and saving: Enzo
- Image analysis and grayscale conversion: Marçal
- Graphical User Interface with two windows: Sabrina
- Histogram analysis and display: Júlia
- Histogram equalization: Gustavo

---

### 1.2. Loading the image

---

### 1.3. Analyzing and converting to grayscale

---

### 1.4. Graphical User Interface with two windows

---

### 1.5. Analyzing and displaying the histogram

---

### 1.6. Histogram equalization

---

### 1.7. Saving the image

---

## 2. Compiling and running

---

### 2.1. On Windows

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
./main <image-path>
```

- Replace `image-path` for `assets/<image-type>/<image-name>`

- `<image-type>` can be:

  - `bmp` or
  - `png` or
  - `jpg`

- `<image-name>` can be:
  - if `JPG`: `image.jpg`
  - if `BMP` or `PNG`:
    - `image-high-contrast.<bmp, png>`
    - `image-low-contrast.<bmp, png>`
    - `image-light.<bmp, png>`
    - `image-dark.<bmp, png>`

---

### 2.2. On Linux

1. Install the necessary dependencies, available at: [https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md#build-dependencies](https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md#build-dependencies)

2. Follow the same compilation and executing steps as for Windows.
