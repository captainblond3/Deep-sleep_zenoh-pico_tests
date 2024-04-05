# Firebeetle zenoh-pico

The esp32 firebeetle is an older (now outdated even) esp32 board that supports wifi and bluetooth 4.2. The firebeetle brand has a focus on deepsleep power preservance but has no other relevance to this project. It supports arduino and espdif frameworks, doesn't support micropython and generally blends in pretty well with other esp32 boards. Here's a [link](https://www.dfrobot.com/product-1590.html) if you want the specs

The purpose of this project and research is to try and create a stable conenction of zenoh-pico under deepsleep conditions as it is currently unstable at best, downright abysmal at worst. To explain DeepSleep without googling for 20 minutes: Arduino's have the ability to turn off all of it's compoonents except for the RTC components, this means the arduino will consume far less power when in this state and as such it's commonly implemented in IoT devices to preserve power and optimize it's lifespan.

## The problem with deepsleep and zenoh-pico;

When a device comes out of deepsleep it runs it's loop again and tries to reconnect to whatever it is coded to. More than often wifi or lora will be able to connect back to a network and repeat the cycle. Zenoh-pico however is not optimized to work under these conditions and will try and fail to connect to a router even if given a static address to connect to, this exact problem i want to resolve or at least diagnose enough to be able to mediate in my projects.