# Visual Computing - Project 01: Image Processing

#### Professor: Andre Kishimoto

#### Class 07N - Group Members

|                    Name                    |    RA    |
| :----------------------------------------: | :------: |
|               Enzo Guarnieri               | 10410074 |
|            Júlia Campolim Oste             | 10408802 |
|          Gustavo Villela Mitraud           | 10400866 |
|      Matheus Marçal Ramos de Oliveira      | 10409001 |
| Sabrina Midori Futami Teixeira de Carvalho | 10410220 |

---

### Summary

1. [About the project](#1-about-the-project)<br>
   1.1. [General aspects, technical requirements and task division](#11-general-aspects-technical-requirements-and-task-division)<br>
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

### 1.1. General aspects, technical requirements and task division

This application consists basically of opening two windows: one displaying a grayscale image (the program automatically converts the image to grayscale if it is not already), and another displaying the corresponding histogram that represents the image’s intensity values (more details ahead).

The following requirements were defined:

- Language: C++
- Mandatory libraries: SDL3, SDL3_image and SDL3_ttf
- Compiler: g++
- Code quality: code organized into structures and functions with clear responsibilities; proper memory management.
- Documentation: a complete and detailed README.
- Repository: hosted on GitHub.

The group decided to divide the project tasks as follows:

- Image loading and saving: Enzo
- Image analysis and grayscale conversion: Marçal
- Graphical User Interface with two windows: Sabrina
- Histogram analysis and display: Júlia
- Histogram equalization: Gustavo

---

### 1.2. Loading the image

The program loads the input image using the SDL\_image library (`IMG_Load()`). The steps involved are:

1. **File existence check**: Before attempting to load the image, the program verifies that the file exists using C++17’s `std::filesystem::exists()`. If the file is missing, an error message is logged, and execution is stopped to prevent crashes.

2. **SDL\_image initialization**: The library is initialized with support for common formats such as PNG, JPG, BMP, TIFF, and WEBP using `IMG_Init()`. This ensures that the program can handle a wide range of input images.

3. **Loading and validation**: `IMG_Load()` reads the image from disk and returns an `SDL_Surface*`. If the function fails (due to unsupported format or corrupted file), an error is logged using `SDL_GetError()` and the program exits gracefully.

---

### 1.3. Analyzing and converting to grayscale

Two in-place functions, `is_grey_scale(SDL_Surface*)` and `to_grey_scale(SDL_Surface*)`, are used to build the grayscale pipeline. They both work directly over the image's pixel buffer by utilizing SDL's surface API.

1. **Verification** - `is_grey_scale` - Verification: after receiving a valid `SDL_Surface*`, the function locks the surface to obtain safe access to its pixel data and retrieves the pixel format description via `SDL_GetPixelFormatDetails(surface->format)`. It then scans every pixel (w * h), decoding each one with `SDL_GetRGBA(pixels[i], format, NULL, &r, &g, &b, &a)`. A pixel must have three equal color components R == G == B in order to be considered grayscale. The function unlocks the surface and returns false on the first mismatch, it unlocks and returns true if the entire scan is finished without any mismatches. To ensure consistent channel extraction, this is carried out in the main flow once the input surface has been converted to `SDL_PIXELFORMAT_RGBA32`.

2. **Conversion** - `to_grey_scale`: when the image is classified as colored, the converter locks the surface, iterates over all pixels, and again uses `SDL_GetRGBA` to read each pixel’s channels. It computes the luminance using the specified perceptual weights, _Y = 0.2125*R + 0.7154*G + 0.0721*B_, casts the result to Uint8, and writes it back to the same pixel with `SDL_MapRGBA(format, NULL, Y, Y, Y, a)`, preserving the original alpha. The surface is unlocked once the buffer has been processed in its entirety and is prepared as the canonical grayscale input for the next processes (histogram, equalization, rendering), and the operation is in-place and does not allocate a second picture.

---

### 1.4. Graphical User Interface with two windows

The graphical interface is composed of two coordinated windows: a main window that displays the image being processed, and a secondary window that shows the image histogram together with an operation button. These components are managed by the **Renderer** class, which encapsulates SDL initialization, window management, rendering, event handling, and resource cleanup.

- **Main window:** displays the image. Its initial size matches the size of the loaded image, and it starts centered on the primary monitor. Since it is resizable, the image automatically adapts to window resizing.

- **Secondary window (child of the main):** created with a fixed size defined in the code, initially centered, and then repositioned to the right of the main window and vertically aligned with its center. Unlike a regular non-resizable window, it is created as a borderless window. This window displays the histogram of the image and includes an operation button (details in item 1.6).

The interface operates as follows:

1. The main window is created first, using the image dimensions obtained by `get_image_size()`. It is set as resizable, and since it serves as the parent, its `parent` parameter is set to `NULL`.

2. The secondary window is then created with fixed dimensions, passing the main window as its parent. The code ensures that the main window exists before attempting to create the secondary one.

3. Once both windows exist, `position_secondary()` repositions the secondary window to the right of the main window and vertically centers it relative to the parent.

4. The image is loaded and converted to grayscale. If loading fails, an error message is logged using `SDL_GetError()`.

5. In the secondary window, `center_button()` defines the geometry of a button that is horizontally centered and placed near the bottom, ensuring it does not overlap the histogram. This button is drawn with `draw_button()`, and it toggles between the states _“Equalized”_ and _“Not Equalized”_ when pressed.

6. The program continues running inside `event_loop()`, which polls SDL events. It closes when the main window is destroyed (by clicking the X or pressing **ESC**), at which point the secondary window is also destroyed, since it depends on the main one.

7. At termination, `destroy_window()` ensures that all windows, renderers, surfaces, and textures are properly released, avoiding memory leaks.

**Implementation overview:**

- **Initialization and cleanup:**

  - `sdl_init()` starts SDL, while `sdl_exit()` shuts it down.
  - `get_image_size()` uses `IMG_Load()` to obtain the image dimensions before creating the main window.

- **Window creation and positioning:**

  - `create_window()` builds the main window (resizable, using the image size) and the secondary window (fixed size, borderless, parented to the main).
  - `position_secondary()` adjusts the secondary window’s placement beside the main one.

- **Rendering:**

  - `render()` draws the grayscale or equalized image in the main window, automatically adapting to resizing.
  - `render_secondary()` manages the secondary window: it displays the histogram using `render_texture_fit()`, adds optional descriptive text with `renderText()`, and handles the action button via `draw_button()`.
  - The button’s state is stored in `is_button_pressed`, which toggles between _“Equalized”_ and _“Not Equalized.”_

- **Event handling:**

  - `event_loop()` processes user inputs and system events. It closes the application when ESC is pressed or when the main window is closed.
  - During execution, it alternates between rendering the original grayscale image and histogram, or their equalized versions, depending on the button state.

- **Resource management:**

  - `destroy_window()` ensures that textures, renderers, and windows are released in the correct order.

Through this structure, the Renderer class guarantees a fluid interaction between the two windows: the main one presenting the image with resizing support, and the secondary one displaying the histogram, explanatory text, and an interactive button to switch between equalization modes.

---

### 1.5. Analyzing and displaying the histogram

---

### 1.6. Histogram equalization

---

### 1.7. Saving the image

The program allows saving the processed image directly from the main window.

1. **Triggering the save action**: when the user presses the **S** key during execution, the application captures the current surface being displayed in the main window. Depending on the state of the toggle button, this may be either the original grayscale image or the equalized version.

2. **File format and location**: regardless of the original format, the output is always saved as a **PNG** file. The resulting file is written to the same folder from which the program was launched, ensuring portability and avoiding the need for absolute paths.

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
