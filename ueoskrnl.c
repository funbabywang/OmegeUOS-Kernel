/*
 * ueoskrnl.c - Omege UOS 内核（MinGW-w64 版本，Native 子系统演示）
 * 编译后为 Native 子系统可执行文件，但运行在用户态。
 */

#include <windows.h>   // 用于类型定义（如 ULONG、PVOID），但不调用 API
#include <stddef.h>

// 导出的全局变量
__declspec(dllexport) ULONG KernelVersion = 0x1000;

// 模拟内存池
#define MIN_MEMORY_SIZE (8 * 1024 * 1024)  // 8MB
static unsigned char memory_pool[MIN_MEMORY_SIZE];
static unsigned char *pool_ptr = memory_pool;

__declspec(dllexport) PVOID ExAllocatePoolWithTag(ULONG PoolType, SIZE_T NumberOfBytes, ULONG Tag)
{
    // 极简分配器
    if (pool_ptr + NumberOfBytes > memory_pool + MIN_MEMORY_SIZE)
        return NULL;
    PVOID ptr = pool_ptr;
    pool_ptr += NumberOfBytes;
    return ptr;
}

__declspec(dllexport) VOID ExFreePoolWithTag(PVOID P, ULONG Tag)
{
    // 不实现释放，留空
}

// 线程模拟
typedef struct {
    void (*StartRoutine)(PVOID);
    PVOID StartContext;
} KTHREAD, *PKTHREAD;

__declspec(dllexport) NTSTATUS KeInitializeThread(PKTHREAD Thread,
                                                  void (*StartRoutine)(PVOID),
                                                  PVOID StartContext,
                                                  ULONG Priority)
{
    Thread->StartRoutine = StartRoutine;
    Thread->StartContext = StartContext;
    return 0;  // STATUS_SUCCESS
}

__declspec(dllexport) NTSTATUS PsCreateSystemThread(PHANDLE ThreadHandle,
                                                     ULONG DesiredAccess,
                                                     PVOID ObjectAttributes,
                                                     HANDLE ProcessHandle,
                                                     PVOID ClientId,
                                                     void (*StartRoutine)(PVOID),
                                                     PVOID StartContext)
{
    PKTHREAD thread = ExAllocatePoolWithTag(0, sizeof(KTHREAD), 0);
    if (!thread) return 0xC0000017;  // STATUS_NO_MEMORY
    KeInitializeThread(thread, StartRoutine, StartContext, 8);
    *ThreadHandle = (HANDLE)thread;  // 简化：句柄直接指向线程对象
    return 0;
}

// Kernel 进程主函数
static void KernelProcessMain(PVOID Context)
{
    for (;;) {
        // 模拟内核循环
        __asm__("pause");  // 用户态下用 pause 替代 hlt
    }
}

// 蓝屏死机模拟
__declspec(dllexport) VOID KeBugCheck(ULONG BugCheckCode)
{
    for (;;) {
        __asm__("pause");
    }
}

// 入口点（必须与链接脚本指定的入口一致）
void KiSystemStartup(void)
{
    HANDLE hThread;
    NTSTATUS status;

    // 检查内存是否足够（始终通过，因为 memory_pool 静态分配）
    // 创建 Kernel 进程（线程）
    status = PsCreateSystemThread(&hThread, 0, NULL, NULL, NULL,
                                   KernelProcessMain, NULL);
    if (status != 0) {
        KeBugCheck(0xDEADBEEF);
    }

    // 主线程进入空闲循环
    for (;;) {
        __asm__("pause");
    }
}

// 可选的 DriverEntry（为满足导出需求）
__declspec(dllexport) NTSTATUS DriverEntry(PVOID DriverObject, PVOID RegistryPath)
{
    return 0;
}