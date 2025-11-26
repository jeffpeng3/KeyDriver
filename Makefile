obj-m += gpioKeys.o
CROSS_COMPILE = /home/wsl/jp621/aarch64--glibc--stable-2022.08-1/bin/aarch64-buildroot-linux-gnu-
ccflags-y := -std=gnu99
# UniversalIO-objs += ./src/main.o ./src/i2c_engine.o ./src/spi_engine.o ./src/uart_engine.o ./src/usb_setup.o

upload: build
	scp -J onyx ./gpioKeys.ko orin:~/driver/

build:
	@make ARCH=arm64 -C /home/wsl/jp621/Linux_for_Tegra/source/kernel/kernel-jammy-src M=$(PWD) modules -j$(shell nproc)

clean:
	@make ARCH=arm64 -C /home/wsl/jp621/Linux_for_Tegra/source/kernel/kernel-jammy-src M=$(PWD) clean