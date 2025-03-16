#define __percpu __attribute__((section(".data..percpu")))
#define DECLARE_PER_CPU(type, name) extern __percpu type name