FFMedia
C++ wrapper for the FFmpeg C API
Supports Linux and Windows
Tested with FFmpeg 7.1.2
✨ Overview
FFMedia is a lightweight and clean C++ wrapper built on top of the FFmpeg C API. It aims to simplify FFmpeg usage with a safe and clear C++ interface.
Key Features
Simple & Safe API: Wraps complex FFmpeg C structures into easy-to-use C++ classes.
Encoding & Decoding:
Software: x264, x265, AAC, etc.
Hardware Acceleration: NVIDIA CUDA (NVENC/NVDEC) support.
Network & Streaming:
Basic streaming capabilities.
SRT Server Framework (🚧 Planned / Coming Soon)
Cross-Platform: Built and tested on Linux and Windows.
📦 Install vcpkg
Install vcpkg by following the official documentation:
[https://vcpkg.io/en/](https://vcpkg.io/en/)


📥 Install Dependencies via vcpkg
Install FFmpeg (with common codecs and hardware acceleration support) and other dependencies:
vcpkg install \
  ffmpeg[fdk-aac,fontconfig,freetype,fribidi,gpl,mp3lame,nvcodec,vorbis,vpx,webp,x264,x265] \
  gtest libsrt libdatachannel


[!NOTE]
Supported FFmpeg Version: > The specific version of FFmpeg supported by FFMedia depends on the latest version provided by the vcpkg registry at the time of your installation. If vcpkg updates its FFmpeg port, the supported version may change accordingly.
🛠️ Build Instructions
Make sure to pass the vcpkg toolchain file to CMake.
Linux
cmake -DCMAKE_TOOLCHAIN_FILE=<YOUR_VCPKG_PATH>/scripts/buildsystems/vcpkg.cmake \
      -DFFMPEGPP_TEST=ON .
make -j10
make install


Windows (MSVC)
On Windows, you can build FFMedia directly using Visual Studio:
Open Visual Studio.
Go to File → Open → Folder....
Select the root folder of FFMedia.
Visual Studio should automatically detect the CMake project.
Go to CMake Settings and add the following CMake configuration options:
-DCMAKE_TOOLCHAIN_FILE=<YOUR_VCPKG_PATH>/scripts/buildsystems/vcpkg.cmake -DFFMPEGPP_TEST=ON


Build using Build → Build All or press Ctrl+Shift+B.
Or using Command Line:
cmake -DCMAKE_TOOLCHAIN_FILE=<YOUR_VCPKG_PATH>/scripts/buildsystems/vcpkg.cmake ^
      -DFFMPEGPP_TEST=ON .
cmake --build . --config Release
cmake --install . --config Release


✔️ Tested Environment
FFMedia has been tested with:
FFmpeg 7.1.2 (via vcpkg)
Linux (Ubuntu)
Windows 10/11 (MSVC)
