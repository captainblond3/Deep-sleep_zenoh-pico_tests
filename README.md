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

## Known issues/bugs

- Zenoh-pico fails to connect when scouting as a client
    - Scouting is currently no guarantee of the device finding a router, you're better off always setting an address and adding a repeat loop for the connecting process as the zenoh example code will cease action after one failed attempt

- Zenoh Router unable to find anything on a listen address
    - While listen should work in theory i've found more reliable results to set the client to the main address of the zenoh router when booting and not setting a listen addres on the router. I can't confirm or deny why this works but it works a lot more reliable than scouting.
    ![](https://github.com/captainblond3/Deep-sleep_zenoh-pico_tests/blob/main/images/zenohexample1.png)

- Zenoh-pico crashes when sending a message
    - the only condition under which zenoh-pico should crash a device (as far as I'm aware) is if zenoh-pico tries to publish a message without being connected to a router. So  check your code to see if a connection gets made before an attempt at publishing is made.

- Zenoh-pico generally has an unstable connection and barely manages to connect after deepsleep.
    - One of the known problems with deepsleep and zenoh-pico is that zenoh-pico isn't designed in it's current state to constantly disconnect and reconnect. The only workaround for this is to disconnect the session before the device goes into deepsleep by calling the `z_put` function to guarantee a proper disconnect occurs before a new connection attempt is made.

>[!TIP]
> If you're having trouble checking connections over a normal or crowded wifi network, consider connecting both router and client(s) to a hotspot network instead, this cuts out the middlemen. This is a generally useful debugging trick for when working with embedded devices. 