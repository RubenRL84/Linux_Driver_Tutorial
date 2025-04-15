### USAGE
```c

## 1 compile file
make

 ## 2 in a second terminal type 
dmesg -W

## 3 load the driver
insmod hello.ko

# 3.1 
    sudo mknod /dev/hello0 c 236 0

# 3.2
    sudo cat /dev/hello0

## 4 unload the driver

rmmod hello.ko
or just rmod hello
```