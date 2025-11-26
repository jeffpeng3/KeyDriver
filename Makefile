obj-m += gpioKeys.o
CROSS_COMPILE = /home/wsl/jp621/aarch64--glibc--stable-2022.08-1/bin/aarch64-buildroot-linux-gnu-
# UniversalIO-objs += ./src/main.o ./src/i2c_engine.o ./src/spi_engine.o ./src/uart_engine.o ./src/usb_setup.o

build:
	@make ARCH=arm64 -C /home/wsl/jp621/Linux_for_Tegra/source/kernel/kernel-jammy-src M=$(PWD) modules -j$(shell nproc)

clean:
	@make ARCH=arm64 -C /home/wsl/jp621/Linux_for_Tegra/source/kernel/kernel-jammy-src M=$(PWD) clean

upload:
	scp -J ceslab-onyx@192.168.17.76 ./gpioKeys.ko onyx@192.168.0.160:~/driver/