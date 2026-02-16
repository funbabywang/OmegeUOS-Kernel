# 内核
这个内核编译大小是12MB左右，如果大小不一跟我联系就行
# 解释一下“应用程序无法在 Win32 模式运行”
因为这个内核的子系统是 Native，所以意味着不能在 Win32 运行（类似 Windows 的 ntoskrnl.exe），这个的模式是 Intel CPU 的 Ring-0 权限（内核权限），所以不在用户态
# 关于
Omege UOS是Omege System 改名的
这个内核可以在 QEMU 里测试，可以用的，会操作硬件，可以启动，不过你没有其他系统文件可能就没有什么用了
# 文件解释
linker.ld 是一个连接器，有大用，没有 linker.ld 就不能编译成功，dummy.dat 是指定文件编译后大小为12.0 MB，因为没有 dummy.dat 编译出来就会小很多（大小）
ueoskrnl.def 是一个模块定义文件，ueoskrnl.c 则是除了硬件抽象层外所有代码，hal.h 就是硬件抽象层了
resource.rc 是资源文件
# 编译
可以使用 x64_x86 GCC ELF先编译成ELF再连接成ueoskrnl.exe
也可以使用 Visual Studio 内置编译器（cl.exe），Visual Studio 需要使用 Visual Studio 开发者命令提示符，这里内置了 cl 的 Path，GCC 则需要配置 Path 不同命令提示符<img width="234" height="103" alt="OmegeUOS-Logo" src="https://github.com/user-attachments/assets/69c8fc06-8040-4380-a3be-27a6c03017f8" />
