# Compile OpenCV for IDS NXT cameras

If you want to use an external library such as openCV on the IDS NXT platform you need to compile the sources for aarch64.
For this purpose you can use the virtual build engine which comes with the [IDS NXT VApp Creator](https://en.ids-imaging.com/download-details/AS00029.html).  

## Getting Started
### Prerequisites
* Download and install the [IDS NXT VApp Creator](https://en.ids-imaging.com/download-details/AS00029.html).

### Build the library
To build openCV we will run [compileOpenCV.sh](compileOpenCV.sh) in the Vsion App build engine. The script will download the sources and perform the compilation process.  
*Hint: If you are working behind a proxy you need to set the environment variables `http_proxy` and `https_proxy` in the build engine.*

1. Copy the bash script [compileOpenCV.sh](compileOpenCV.sh) to the shared folder of the Vision App build engine (default is C:\Users\<YOUR_USER>\Documents\NXT)
2. Open IDS NXT Vision App Build Engine in VirtualBox
    1. (Optional) Change your keyboard layout e.g.  
    ```bash
    sudo loadkeys de
    ```
3. Change to the shared folder:  
```bash
cd ~/share/
```
4. Run the install script  
```bash
bash ./compileOpenCV.sh
```
5. After the build process you can find openCV in the shared folder.

For more detailed information see the comments in [compileOpenCV.sh](compileOpenCV.sh)

## Usage
Copy the binaries to your Vision App like in this [example](../../vapps/FaceDetection-OpenCV/README.md)