# ffmpegpp

**C++ wrapper** for the **FFmpeg C API**
Supports **Linux** and **Windows**
Tested with **FFmpeg 7.1.2**

---

## ✨ Overview

`ffmpegpp` is a lightweight and clean C++ wrapper built on top of the FFmpeg C API.

* Simplifies FFmpeg usage with a safe and clear API
* Current encoding support:

  * **x264**
  * **x265**
* Built and tested on:

  * **Linux**
  * **Windows**

---

## 📦 Install vcpkg

Install vcpkg by following the official documentation:

```
https://vcpkg.io/en/
```

---

## 📥 Install Dependencies via vcpkg

Install FFmpeg (with common codecs) and GoogleTest (optional for testing):

```bash
vcpkg install \
  ffmpeg[fdk-aac,fontconfig,freetype,fribidi,gpl,mp3lame,nvcodec,vorbis,vpx,webp,x264,x265] \
  gtest
```

---

## 🛠️ Build Instructions

Make sure to pass the **vcpkg toolchain file** to CMake.

### Linux

```bash
cmake -DCMAKE_TOOLCHAIN_FILE=<YOUR_VCPKG_PATH>/scripts/buildsystems/vcpkg.cmake \
      -DFFMPEGPP_TEST=ON .
make -j10
make install
```

### Windows (MSVC)

On Windows, you can build `ffmpegpp` directly using **Visual Studio**:

1. Open **Visual Studio**
2. Go to **File → Open → Folder...**
3. Select the root folder of `ffmpegpp`
4. Visual Studio will automatically detect the CMake project
5. Go to **CMake Settings** and add the following CMake configuration:

```cmake
-DCMAKE_TOOLCHAIN_FILE=<YOUR_VCPKG_PATH>/scripts/buildsystems/vcpkg.cmake -DFFMPEGPP_TEST=ON
```

6. Build using **Build → Build All** or press **Ctrl+Shift+B**

```bash
cmake -DCMAKE_TOOLCHAIN_FILE=<YOUR_VCPKG_PATH>/scripts/buildsystems/vcpkg.cmake ^
      -DFFMPEGPP_TEST=ON .
cmake --build . --config Release
cmake --install . --config Release
```

---

## ✔️ Tested Environment

`ffmpegpp` has been tested with:

* **FFmpeg 7.1.2**
* Linux (Ubuntu)
* Windows 10/11 (MSVC)
