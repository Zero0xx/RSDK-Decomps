# ANDROID SETUP

* Clone the repo, then install the dependencies listed below
* Ensure the symbolic links in `[root]/android/app/jni` are correct: 
  * `RSDKv5` -> root of the RSDKv5 repository
  * `Game` -> root of the Mania repository (or any other game that you may be compiling)
  * Extra symbolic links can be added for things like mods, **as mods do not use the local files for logic.** Just ensure that there is an `Android.mk` file at the root of them. 
    * (to add symbolic links, use the following:)
      * Windows: `mklink /D "name-of-symlink" "[path]"`
      * Linux: `ln -s "name-of-symlink" "[path]"`
* Open `[root]/android/` in Android Studio, install the NDK and everything else that it asks for, and build.

The working directory will be at `[sdcard root]/RSDK/v5`. **Please note that GL shaders are required, or you will get a black screen.** See [the shaders README](../../RSDKv5/Shaders/README.md) for more details.

## Dependencies:
* libogg: https://xiph.org/downloads/ (libogg)
  
  Download libogg and unzip it in "./libogg/".

* libtheora: https://xiph.org/downloads/ (libtheora)
  
  Download libtheora and unzip it in "./libtheora/".

## Common build issues (Windows)
### `make: *** INTERNAL: readdir: No such file or directory`
Delete `android/app/build` and try again. This occurs when the path is short enough to build, but too long for a rebuild. 
### `make: Interrupt/exception caught (code = 0xc0000005)`
Your paths are too long. Try renaming the symbolic links to something shorter, or make a symbolic link to RSDKv5 closer to the root of the drive and open the project through there (e.x. C:/RSDKv5/android). *I haven't had either issue since I did this.*

## Known issues
These issues will not be fixed here as they are minor, but feel free to have a look at them for yourself in a fork!
### Audio device doesn't disconnect properly
The audio callback for the lost stream will continue to fire (AAudioDevice's AudioCallback,) so you only end up hearing half on the new stream. All examples I (RMG) looked at while porting didn't show any issues or workarounds for this. I tried to remedy by checking the stream's pointer, but that didn't fix anything.