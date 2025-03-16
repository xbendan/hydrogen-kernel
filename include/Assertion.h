// static inline void
// assert(bool condition, const char* message)
// {
//     if (!condition) {
//         // printf("Assertion failed: %s\n", message);
//         // exit(1);
//     }
// }
#include <Terminate.h>

#define assert(condition, message)                                             \
    if (!(condition)) {                                                        \
        Std::terminate(message);                                               \
    }

#define assertEquals(a, b, action)                                             \
    if ((a) != (b)) {                                                          \
        action;                                                                \
    }

#define Requires(condition)                                                    \
    if (!(condition)) {                                                        \
        return;                                                                \
    }

#define RequiresOrValue(condition, ret)                                        \
    if (!(condition)) {                                                        \
        return ret;                                                            \
    }

#define RequiresOrNull(condition)                                              \
    if (!(condition)) {                                                        \
        return nullptr;                                                        \
    }

#define RequiresOrZero(condition)                                              \
    if (!(condition)) {                                                        \
        return 0;                                                              \
    }

#define OS_VALIDATE_OR_ELSE(condition, action, elseAction)                     \
    if (condition) {                                                           \
        action;                                                                \
    } else {                                                                   \
        elseAction;                                                            \
    }