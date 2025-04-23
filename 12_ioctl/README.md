## USAGE

# 1 compile file
```c
make
```
# 2 in a second terminal type 
```c
   dmesg -W
```
# 3 load the driver
```c
  insmod hello.ko
```
- 3.1
```bash
 sudo mknod /dev/hello0 c 236 0

```
- 3.2
```bash
 sudo ./cdev_test /dev/hello0
```
# 4 unload the driver
```bash
rmmod hello.ko
```
or just
```bash
rmod hello
```
