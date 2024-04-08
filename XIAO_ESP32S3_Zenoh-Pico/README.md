# XIAO ESP32S3 Zenoh-Pico

The XIAO ESP32S3 is as the name implies a esp32 board from seeedstudio's XIAO series and is equipped with a newer more powerfull cpu compared to many predecessors. It has bluetooth 5.0 and wifi connectivity and can have even more features with the 'sense' upgraded version. This series type of board works well due to it's powerful components and low power potential but does in turn suffer from it's own succes due to poor thermal management.

## Zenoh-Pico

As far as i've tested the board is an excellent choice for connectivity due to it's wifi system and added antenna, in the future the board also makes a viable candidate for zenoh-pico over bluetooth but as it currently stands zenoh-pico does not support bluetooth 4.0 or above, should this change in the forseeable future than this board would also be a viable client/peer in a larger network as it can both be connected via wifi and bluetooth.

# Findings/issues

### deepsleep connectivity

If deepsleep is added to the loop the zenoh connection has to be remade every loop, this however can be problematic if the zenoh connection to the router is not terminated before going into deepsleep. Should these precautions not be taken a common problem will appear where the board can connect to the wifi but not open a zenoh session. 

![Image of the esp32s3 zenoh-pico failed to open session error and reboot](/main/images/zenohsessionfailure.png)

The obvious first thing to try when you have a shaky/failing connection to a session is to set a listen address on the zenoh router: Using `zenohd -l tcp/192.168.x.xxx:xxxx` is a clear way to try and set a static address for client to connect to in theory. It is advisable to have the router running *before* setting up a client as it might miss the router otherwise.

While trying to debug this issue i discovered a strange connectivity gimmick/fix. I had set my board to a static address but ran zenohd without a listening address to which it generated a new address, this address was the exact same as the previously set listening address but had a substantially increased rate of connectivity. The previous static address could fail 8-40 times with rebooting before connecting to a router whereas this 'lazy' method had near instant connectivity and reduced the fail rate by about half or less as the previous connection. 

![Image of the automatic 'static' address](/main/images/zenohfind1.png)

This 'trick' is by no means the magic trick to make zenoh-pico work with a 100% reliabilit but for debugging purposes it certainly has it's uses. The fact it works might have to do with the disconnectivity issue embedded currently has but i'd have to check with zettascale engineers for this as i simply lack the knowledge and experience to prove this assumption.