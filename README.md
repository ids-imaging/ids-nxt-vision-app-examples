![IDS NXT Logo](utils/ids-nxt-logo.png#gh-light-mode-only)
![IDS NXT Logo White](utils/ids-nxt-logo-white.png#gh-dark-mode-only)
# Vision app examples
This repository provides several vision app samples to get you started quickly and easily with programming your own vision app and using the IDS NXT API. 

## Content
* Getting Started
    * Prerequisites
        * IDS NXT Vision App Creator
    * Documentation
        * NXT API
* Examples
    * How-To
        * Compiling a third party library
        * Listing tags
        * Specifying metadata for a vision app
* License
* Contact

## Getting Started
### Prerequisites
#### IDS NXT Vision App Creator
To customize and build the vision app samples, we provide the [IDS NXT Vision App Creator](https://en.ids-imaging.com/downloads.html). The IDS NXT Vision App Creator is an IDE, based on Qt Creator for programming vision apps for IDS NXT cameras. The IDE includes the toolchain and the IDS NXT SDK. You can directly upload the vision apps from the development environment to your IDS NXT camera.

### Documentation
#### IDS NXT API
The IDS NXT framework includes the [IDS NXT API](https://en.ids-imaging.com/manuals/ids-nxt/ids-nxt-os-sdk/en/index.html) which offers a wide range of functionality to implement a variety of applications building your own vision app.

## Examples
* [Basic vision app example](vision-apps/Basic)  
* [Using an ROI & result highlighting](vision-apps/GrayValue)  
* [AI based classification](vision-apps/Classification-DeepOceanCore)  
* [AI based detection](vision-apps/PersonDetection-DeepOceanCore)  
* [Face detection with OpenCV](vision-apps/FaceDetection-OpenCV)  
* [Handling multiple ROIs](vision-apps/MultiROI)  
* [Processing multiple CNNs](vision-apps/MultiCNN)  
* [Using FTP & vision app website](vision-apps/FileTransferAndWebsite)

### How-To
#### Compiling a third party library
* [OpenCV](misc/CompileOpenCV)  

#### Listing tags
You can find the tags in the list below as comments in the source code of the examples included in this repository.  
In order to find the corresponding source code, you can search for the hashtag in the files.  

* Connecting a Qt framework signal with a vision app method (**#SIGNAL_CONNECTION**)
* Handling of vision app results (**#RESULT_HANDLING**)
* Writing a message to the system log (**#SYSTEM_LOG**)
* Creating a custom result image (**#RESULT_IMAGE**)
* Setting camera parameters (**#CAMERA_PARAMS**)
* Using ROIs (**#ROI_PROCESSING**)
* Handling of CNNs for *classification* (**#CLASSIFICATION**)
* Handling of CNNs for *detection* (**#DETECTION**)
* Highlighting vision app results in the [IDS NXT Cockpit](https://en.ids-imaging.com/downloads.html) (**#RESULT_HIGHLIGHTING**)
* Add a website to your vision app (**#VAPP_WEBSITE**)

#### Specifying metadata
You can specify the metadata of a vision app and metadata of the GUI element in the following files:  
* Specifying vision app metadata (e.g. name, version, manufacturer, etc.) => **manifest.json**
* Specifying GUI element metadata (e.g. button names, descriptions, etc.) => **translation.json**


## Licenses
See the `license.txt` file of the respective vision app for more information.

## Contact
[IDS Imaging Development Systems GmbH](https://en.ids-imaging.com/)
