## DESCRIPTION
Simple driver to control a led (GPIO 21) and a button (GPIO 20) on a raspbeerry pi

## USAGE
```c
//compile file
make

//in a second terminal type 
dmesg -W

// load the driver
insmod hello.ko
pressing the button for example
// unload the driver

rmmod hello.ko
or just rmod hello
```