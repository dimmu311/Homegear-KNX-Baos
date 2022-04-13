# Homegear-KNX-Baos
KNX Baos module for Homegear

with this Family you can use KNX-Baos with Homegear. For example you can use the Weinzirl kBerry with your Raspberry Pi and Homegear. This module uses the KNX-Baos SDK. See the Installation section.
I find this solution more handy than the Homegear-KNX module.
- with this module you don't need to add JSON descriptions to your KNX Project and 
- you don't need to export your KNX Project it to Homegear,
- you don't need KNX ip Interface (you need KNX-Baos interface but this is less expensive)


but
- with this module you have a limitation of 1000 Groupadresses.
- you need to connect all Groupadresses you want to use in Homegear in your KNX Project with the KNX Baos device.


**_NOTE:_** This module should work with all KNX-Baos devices Weinzierl offers but until now i only testet with kBerry.


## Installation
Unitl now there is no precompiled version, so you have to compile your own.
This is done in 5 Steps.

1. Download this Repository
2. Download the KNX-Baos SDK
3. Compile the KNX-Baos SDK
4. Copy the SDK to your lib directory
5. Compile this module

**_NOTE:_** The KNX-Baos SDK is not part of this repository. So you need to get it from Weinzirl.


DO NOT follow the build instructions from Weinzirl. You need to build a shared lib.


If you have any dependey problems while building the sdk, follow the build instructions of Weinzirl until you reach the point where you should build the static lib. Then stop and follow this instructions.


You need the download the sdk ```/src/PhysicalInterfaces``` otherwise the paths during building the module are wrong.

```console
## download this repository
git clone https://github.com/dimmu311/Homegear-KNX-Baos.git
cd Homegear-KNX-Baos/src/PhysicalInterfaces
## download the sdk
git clone https://github.com/weinzierl-engineering/baos.git

## build the sdk
mkdir build_unix
cd build_unix
cmake -DCMAKE_BUILD_TYPE=Release -DWZSDK_STATIC=0 -DWZSDK_SAMPLES=0 -DWZSDK_SAMPLES_QT=0 -G "Unix Makefiles" ../
## on older rpi like pri2b use make without setting more build threads
make -j2

## copy the sdk
cp lib/* /usr/lib

## build the module
cd ../../../../
./makeRelese.sh

## copy the conf to the config dir
cp misc/Config\ Directory/knxbaos.conf /etc/homegear/families/

```

on some debian based systems you need to call ```/sbin/ldconfig -v``` to fix a linker cach problem. I think step 4 and this is just a workaorund for some linker problems. But i did not find a better way until now.

## Configuration
just edit the knxbaos.conf. This example should work with kBerry on all rpi models with homegear image. If you use original raspbian image you maybe need to free the uart port.

## Woking with the module
After you made changes to your KNX project and have uploaded to the KNX-Baos, you need to call ```sp``` in homegear-cli on family 67. This will update the Homegear device.

For each Datapoint id in KNX you will find a channel on the homegear peer. So this means you onyl have one singel peer with multiple channels.

During a limitation in the sdk it is not possible to show the Datapoint name in homegear. I'm not sure if this is a bug in the sdk or if this is a limitation of the kBerry. So this means that in the sdk it looks like ther is a methode that receives the name, but this always returns an error in my setup.





