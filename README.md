# 🧰 Simple Linux Driver Tutorial

Welcome to the **Simple Linux Driver Tutorial** repository! This project is a beginner-friendly introduction to writing a basic Linux kernel module (driver). It walks through the essentials of creating, compiling, and loading a simple character device driver in the Linux kernel space.

## 📚 What You'll Learn

- Basics of Linux kernel modules
- Creating a simple character device driver
- Compiling kernel modules with `Makefile`
- Using `insmod`, `rmmod`, and `dmesg`

## 🛠️ Requirements

- A Linux system (preferably Ubuntu or another Debian-based distro)
- Basic knowledge of C programming
- Kernel headers installed (`linux-headers-$(uname -r)`)

## 🚀 Getting Started

```bash
### 1. Clone the Repository
git clone https://github.com/RubenRL84/Linux_Driver_Tutorial.git
cd Linux_Driver_Tutorial

### 2. Build the Driver
make

### 3. Insert the Module
sudo insmod <name>.ko

### 4. Check Kernel logs
dmesg | tail

### 5. Remove the Module
sudo rmmod <name>.ko

### 6. Remove the Module
make clean
```

## 📄 License

This project is licensed under the MIT License.

## 🤝 Contributing

Pull requests are welcome! Feel free to fork this repo and improve the tutorial.

Happy hacking! 💻🐧
```vbnet
Let me know if you want a matching `Makefile` or example `simple_driver.c` too!
``
