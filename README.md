# Zenoh-pico Deepsleep

The purpose of this project and research is to try and create a stable conenction of zenoh-pico under deepsleep conditions as it is currently unstable at best, downright abysmal at worst. 
To explain DeepSleep without googling for 20 minutes: Arduino's have the ability to turn off all of it's compoonents except for the RTC components, this means the arduino will consume far less power 
when in this state and as such it's commonly implemented in IoT devices to preserve power and optimize it's lifespan.

## The problem with deepsleep and zenoh-pico

When a device comes out of deepsleep it runs it's loop again and tries to reconnect to whatever it is coded to. More than often wifi or lora will be able to connect back to a network and repeat the cycle.
Zenoh-pico however is not optimized to work under these conditions and will try and fail to connect to a router even if given a static address to connect to, this exact problem i want to resolve or at least 
diagnose enough to be able to mediate in my projects.

### Recreation

If you wish to try and recreate these setups be sure to check the platformio.ini files to ensure you get the correct libraries and frameworks. As for the zenoh-pico library for this version do **not** make the mistake of simply adding the library as a `lib_deps` option as it will by default import zenoh-pico 11 which is a dev module. I've been advised against using this from the zenoh-pico staff themselves. Instead i advise you to download/clone the 10.1.1 release and softlink it in your project. e.g. 

`ln -s path/to/zenoh/library path/to/your/project`