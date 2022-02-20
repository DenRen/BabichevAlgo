#ifndef DEBUG_FUNC_H
#define DEBUG_FUNC_H

#include <errno.h>

void print_error_line (const char strerr[], const char name_file[], int line);

const int MAXLEN_ERRSTR = 256;
const int MAXLEN_LOGSTR = 512;

//#define DEBUG

#ifdef DEBUG
    #define IF_DEBUG_NON_PRINT(act) \
        do {                        \
            act;                    \
        } while (0)

#else
    #define IF_DEBUG_NON_PRINT(act)
#endif

#ifdef DEBUG
    #define IF_DEBUG(act) do {                          \
                              printf ("\tDebug LOG: ");   \
                              act;                      \
                             } while (0)
#else
    #define IF_DEBUG(act)
#endif

#define PRINT_ERROR(str) print_error_line (str, __FILE__, __LINE__);

#define CHECK_PTR(ptr)                      \
    if (ptr == NULL) {                      \
        errno = EINVAL;                     \
        PRINT_ERROR ("Pointer is NULL");    \
        return -1;                          \
    }

#define CHECK_ERR(err_code)                 \
    do {                                    \
        int err = err_code;                 \
        if (err) {                          \
            PRINT_ERROR (#err_code);        \
            return -1;                      \
        }                                   \
    } while (0)

#define CHECK(is_true)                      \
    do {                                    \
        int state = is_true;                \
        if (!state) {                       \
            PRINT_ERROR (#is_true);         \
            return -1;                      \
        }                                   \
    } while (0)

#endif // DEBUG_FUNC_H
