# Heltec Wireless stick lite V3

The Wireless stick lite v3 or HTIT for short is a heltec board with a modded esp32s3 (SP32-S3FN8), intergrated wifi antenna, lora, bluetooth le, mesh and 5. When looking at the specs alone this board would be a juggernaut of flexibility with a strong processor to boot. The documentation and support for this board however is abysmal to get through and requires some fidlding to get to work in platformio. In the worst case scenario you'll have to flash it first to get it updated to a workable state. So it's in theory a great board that could possibly even funcion as a zenoh node in the future to connect wifi, bluetooth and lora. In practice i haven't even been able to get the board to work with zenoh-pico yet. The specs if you're interested [link](https://heltec.org/project/wireless-stick-lite-v2/)

## installation

There is a flash program but this shouldn't be necessary as it functions as a bootloader update and uploads a general test script that tries out lora, wifi and bluetooth. Should you need it you can find it on the website under 'Downloadable Resources" [link](https://resource.heltec.cn/download/Wireless_Stick_Lite_V3) Mind you this program only runs on windows and you'll still have to check the usb port in the script (i've tried to run it on linux but it's just not worth the effort)

For whatever reason platformio does have the HTIT listed but this board doesn't  support the right chipset and will give a chipset error. So for starters use the heltec wifi lora v3 board as it seemingly works exactly the same but doesn't give the chipset error, the pinout should still be the same.
![](https://github.com/captainblond3/Deep-sleep_zenoh-pico_tests/blob/main/images/heltec_example)

There is a official heltec framework and while it does work it only works under arduino IDE and hasn't been supported fully so i'll leave it out as i don't use IDE often and prefer vscode + platformio

