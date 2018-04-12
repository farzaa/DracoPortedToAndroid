# DracoPortedToAndroid

Hi there! So, the very popular compression library [Draco](https://github.com/google/draco) is pretty cool. Draco is able reduce a 3D model from 6MB to 100kb. Thats amazing. But, it's actually a bit difficult to get this library working on an Android device. I wanted to get this working because being able to "decode" large 3d-models on the chip of a mobile device would wildly reduce the size of the downloads. Users would download tiny 100kb "drc" files and quickly decode them to full quality right in their hand. As augmented reality becomes bigger, I think this tech is very important. 

This repo serves as an example for people who wish to use Draco in their apps. The example is based heavily off of [hello-libs](https://github.com/googlesamples/android-ndk/tree/master/hello-libs). You'll need to download the [static libraries](https://drive.google.com/file/d/1nimttIIacp_8UdPttJ8-jhJ44vCrI9a7/view?usp=sharing) which have been compiled for a couple of architectures. Put this folder in the root directory.

The core of the project is in DracoPortedToAndroid/app/src/main/cpp/hello-libs.cpp.
