# Neo FFT3D (forked from fft3dfilter)

Neo FFT3D Copyright(C) 2020 Xinyue Lu, and previous developers

FFT3DFilter is a 3D Frequency Domain filter - strong denoiser and moderate sharpener. It was originally written by Alexander G. Balakhnin aka Fizick, and later modified by martin53 for AviSynth 2.6 and later modified by Ferenc Pintér aka printerf for further improvement, high bit depth, and more. Kudos to them for creating and improving this fantastic tool.

In this project, legacy format like YUY2 has been removed, legacy parameters like multiplane has been removed, and SIMD code has been completely re-written for all core parts of the code. Due to API change, the project has been renamed from FFT3DFilter to Neo_FFT3D to avoid confusion. SSE is required to run optimized routine.


## Usage

```python
# AviSynth+
LoadPlugin("neo-fft3d.dll")
neo_fft3d(clip, sigma=2.0, bt=3, y=3, u=3, v=3, ...)
```

Parameters:

[Check original usage documents.](https://avisynth.org.ru/fft3dfilter/fft3dfilter.html)

- *bt*

    * -1: Sharpen and dehalo only
    * 0: Temporal Kalman filter
    * 1: 2D (spatial) Wiener filter
    * 2: 3D Wiener filter for 2 frames
    * 3: 3D Wiener filter for 3 frames
    * 4: 3D Wiener filter for 4 frames
    * 5: 3D Wiener filter for 5 frames
    * As shown below:

        |bt | prev2 | prev1 |current| next1 | next2 |
        |---|-------|-------|-------|-------|-------|
        | 1 |       |       |   o   |       |       |
        | 2 |       |   o   |   o   |       |       |
        | 3 |       |   o   |   o   |   o   |       |
        | 4 |   o   |   o   |   o   |   o   |       |
        | 5 |   o   |   o   |   o   |   o   |   o   |

    Default: 3.

- *sigma*

    Denoise strongness.

    Default: 2.0.

- *y*, *u*, *v*

    Whether a plane is to be filtered.

        1 - Do not touch, leaving garbage data
        2 - Copy from origin
        3 - Process

    Default: 3.

## License

* GPLv2.
