# File transfer and website vision app

In this vision app you will use the FTP client which is provided by the IDS NXT framework. The FTP client allows to send data to an FTP server.  

## Getting Started
### Prerequisites
![IDS NXT API version](https://img.shields.io/badge/NXT_API-v2.3.0-008A96.svg)  ![IDS NXT OS version](https://img.shields.io/badge/NXT_OS-v2.6.x-008A96.svg)  

* Hint: The FTP server configuration can be set using the `IDS NXT cockpit` or via REST at **<Camera_IP>/network/ftpclient**  
    Configuration example:  
    ```
    "Enabled": true  
    "ServerPassword": yourFtpServerPassword  
    "ServerURL": ftp(s)://yourFtpServerIP  
    "Timeout": 1000  
    "UserName": yourUserName
    ```

## Vision app website
IDS NXT Ocean 2.2 introduces vision app websites which can be accessed at **<Camera_IP>/vapps/<VAPP_NAME>/website**.
This example demonstrates how to add a website to a vision app.  
In order to add a website to a vision app you need to:
* Create a new directory "website" in the root directory of the vision app.
* Paste your website (index.html file + assets if necessary) to /website.
* Add the /website directory to the deployed files in your *.pro file e.g.
``` bash
DEPLOYFILES += $$PWD/qtlogging.ini $$PWD/website
```
A custom title for the website node can be set in the manifest.json file under the "Website" section. This title is used as the text for the Website button in the IDS NXT cockpit.
## Licenses
[![License for avatar](https://img.shields.io/badge/License_for_avatar-Flaticon-lightgrey.svg)](https://www.flaticon.com/free-icons/website)
