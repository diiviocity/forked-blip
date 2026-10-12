// V comptime_definitions:
// V compile time defines by -d or -define flags:
//     All custom defines      : gcboehm,gcboehm_full,gcboehm_opt,skip_fastc,skip_arm64,skip_wasm,skip_ssa,skip_eval
//     Turned ON custom defines: gcboehm,gcboehm_full,gcboehm_opt,skip_fastc,skip_arm64,skip_wasm,skip_ssa,skip_eval
#define CUSTOM_DEFINE_gcboehm
#define CUSTOM_DEFINE_gcboehm_full
#define CUSTOM_DEFINE_gcboehm_opt
#define CUSTOM_DEFINE_skip_fastc
#define CUSTOM_DEFINE_skip_arm64
#define CUSTOM_DEFINE_skip_wasm
#define CUSTOM_DEFINE_skip_ssa
#define CUSTOM_DEFINE_skip_eval

#ifndef V_THREAD_STACK_SIZE
#define V_THREAD_STACK_SIZE 8388608
#endif
#if defined(_WIN32)
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#endif
#if defined(_WIN32) && !defined(WIN32_FULL) && !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif
#if defined(_MSC_VER) && !defined(_WINSOCKAPI_)
#define _WINSOCKAPI_
#define V_LEGACY_WINSOCK_GUARD
#endif
#include <gc.h>
#ifdef V_LEGACY_WINSOCK_GUARD
#undef V_LEGACY_WINSOCK_GUARD
#ifndef _WINSOCK2API_
#undef _WINSOCKAPI_
#endif
#endif
#define v_gc_set_warn_proc(cb) GC_set_warn_proc((GC_warn_proc)(cb))
#define v_gc_get_warn_proc() ((void *)GC_get_warn_proc())
#define v_gc_set_abort_func(cb) GC_set_abort_func((GC_abort_func)(cb))
#define v_gc_get_abort_func() ((void *)GC_get_abort_func())
#define v_gc_call_abort_func(fn, msg) ((GC_abort_func)(fn))(msg)
#include <io.h>
#define v_gc_set_warn_proc(cb) GC_set_warn_proc((GC_warn_proc)(cb))
#define v_gc_get_warn_proc() ((void *)GC_get_warn_proc())
#define v_gc_set_abort_func(cb) GC_set_abort_func((GC_abort_func)(cb))
#define v_gc_get_abort_func() ((void *)GC_get_abort_func())
#define v_gc_call_abort_func(fn, msg) ((GC_abort_func)(fn))(msg)
#include <fcntl.h>
#include <float.h>
#include <winsock2.h>
#include <sys/stat.h>
#include <errno.h>
#include <process.h>
#include <sys/utime.h>
#include <windows.h>
#include <signal.h>
#define v_signal_with_handler_cast(sig, handler) signal((sig), (void (*)(int))(handler))
#include <synchapi.h>
#define v_signal_with_handler_cast(sig, handler) signal((sig), (void (*)(int))(handler))
#include <math.h>

#include <stdint.h>
#include <stddef.h>
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint8_t byte;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;



#if defined(__STDC_VERSION__) && __STDC_VERSION__ > 201710L
#include <stdbool.h>
#endif
#ifndef __bool_true_false_are_defined
#ifdef _MSC_VER
typedef unsigned char bool;
#else
typedef _Bool bool;
#endif
#define __bool_true_false_are_defined 1
#endif
typedef void* voidptr;
typedef i64 int_literal;
typedef double float_literal;
#if defined(_MSC_VER)
#define V_INT128_STRUCT_ATTR __declspec(align(16))
#else
#define V_INT128_STRUCT_ATTR __attribute__((aligned(16)))
#endif

#if defined(__SIZEOF_INT128__) && !defined(V_INT128_PORTABLE)

typedef unsigned __int128 u128;
typedef __int128 i128;

#define V_INT128_NATIVE 1

/* Generated code builds a wide literal from its halves, so this has to exist on
 * both representations. On the native type the halves are a shift and an or. */


/* A shift count, clamped to the one value the shift helpers have to recognise as
 * out of range. Reading only the low 64 bits let a count of 2^64 look like zero. */


/* The native type is two's complement, so every bit pattern is a value and the
 * conversions below are plain casts. */
































































/* min_i128 / -1 overflows a signed division in C and raises SIGFPE on x86, so
 * that one case is computed as a negation; every other pair divides directly.
 * The portable path answers the same way through its long division. */












/* Negating the minimum value has no representable result, so the negation is
 * computed in unsigned arithmetic and only then reinterpreted. That wraps to
 * the minimum again, the same answer the portable path gives. */






























#else

typedef struct v_int128_s {
	u64 lo;
	u64 hi;
} V_INT128_STRUCT_ATTR v_int128_t;

/* The portable representation is the same C struct for both signednesses: the
 * bits are two's complement either way, and V keeps signedness in its own type
 * table, so there is nothing for C to distinguish. */
typedef v_int128_t u128;
typedef v_int128_t i128;



/* A shift count, clamped to the one value the shift helpers have to recognise as
 * out of range. Anything above the low 64 bits is at least 2^64, which is over
 * the 128 a shift can use. */






/* A negative i64 sign-extends, because the struct holds a two's complement
 * pattern and the value keeps its sign when it widens. */


















/* 2^64 as a double, the weight of the high limb. */
#define V_INT128_TWO64 18446744073709551616.0







/* The magnitude of a signed value as an unsigned one. */






/* The low 64 bits come from the 32-bit limb products; the high 64 bits add the
 * cross products, which is everything a 128-bit result keeps. */




























/* Binary long division, one quotient bit per pass. */






/* A value wider than a double has to be rounded once, from the whole magnitude.
 * Converting each limb on its own rounds twice, and the first rounding throws away
 * the bit that decides the second: with hi = 2^53 + 1 and lo = 2^63, the high limb
 * rounds down to 2^117 and the answer lands 2^65 too low.
 *
 * Keep the top 53 bits and round on the guard bit below them plus every bit after
 * it, the way a hardware conversion does. The scale below is a power of two, which
 * multiplies exactly, so this decides the only rounding there is. */














/* Two's complement multiplication keeps its low 128 bits whatever the signs
 * are, so the unsigned product is the signed result. */




























/* The remainder takes the sign of the dividend, which is what C and V both
 * specify. */


#endif

/* Decimal text for the assert and panic printers.
 *
 * This comes after the main helper block, so it can call the arithmetic helpers
 * whatever representation the build picked. The buffer is static and the text is
 * written back-to-front, which keeps a failing path free of allocation. */


/* Signed decimal. The magnitude comes from the unsigned negation, which is also
 * the only way to print the minimum value. */


struct sync__Channel;
typedef struct sync__Channel* chan;
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ <= 201710L
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
#endif
#if defined(__TINYC__) || defined(_MSC_VER)
#define E_STRUCT_DECL unsigned char _dummy_pad
#define E_STRUCT 0
#else
#define E_STRUCT_DECL
#define E_STRUCT
#endif
#define _S(s) ((string){.str=(u8*)("" s), .len=(sizeof(s)-1), .is_lit=1})
#if !defined(VNORETURN)
#if defined(__TINYC__)
#define VNORETURN __attribute__((noreturn))
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define VNORETURN _Noreturn
#elif defined(__GNUC__) && __GNUC__ >= 2
#define VNORETURN __attribute__((noreturn))
#endif
#ifndef VNORETURN
#define VNORETURN
#endif
#endif
#if defined(_MSC_VER) && !defined(__clang__)
#define __attribute__(x)
#ifndef _Thread_local
#define _Thread_local __declspec(thread)
#endif
#define _Atomic volatile
#define __alignof__(x) __alignof(x)
#endif
typedef ptrdiff_t isize;
typedef size_t usize;
typedef char* charptr;
typedef unsigned char* byteptr;
typedef int (*qsort_callback_func)(const void*, const void*);
#ifndef VCALLCONV
#define VCALLCONV(x)
#endif
#if defined(__linux__) && !defined(__ANDROID__)
#include <features.h>
#endif
#ifdef sprintf
#undef sprintf
#endif
#ifdef snprintf
#undef snprintf
#endif
#ifdef vsnprintf
#undef vsnprintf
#endif
#ifdef memcpy
#undef memcpy
#endif
#ifdef memmove
#undef memmove
#endif
#ifdef memset
#undef memset
#endif
// GCC-compatible compilers also define __GNUC__, so keep V's discriminator
// in sync with the compiler that will consume a portable generated C file.
#if defined(__GNUC__) && !defined(__TINYC__) && !defined(__cplusplus) && !defined(__clang__)
	#define __V_GCC__
#endif
// c_headers
typedef int (*qsort_callback_func)(const void*, const void*);
#if defined(_MSC_VER) && !defined(__clang__)
	#define V_CRT_LINKAGE __declspec(dllimport)
	#define V_CRT_CALL VCALLCONV(cdecl)
#else
	#define V_CRT_LINKAGE
	#define V_CRT_CALL
#endif
#if (defined(__MINGW32__) || defined(__MINGW64__)) && defined(__V_GCC__)
	#define V_CRT_STDIO_LINKAGE __attribute__((dllimport))
#else
	#define V_CRT_STDIO_LINKAGE V_CRT_LINKAGE
#endif
#if (defined(_MSC_VER) && !defined(__clang__)) || defined(__cplusplus)
// Under C++ (g++/clang++), let libc declare FILE/stdio/string/stdlib to keep
// noexcept specifiers consistent — the manual extern "C" prototypes below
// would otherwise conflict with system headers under -std=c++NN.
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef va_copy
	#define va_copy(dest, src) ((dest) = (src))
#endif
#ifndef _TRUNCATE
	#define _TRUNCATE ((size_t)-1)
#endif
#elif defined(__NetBSD__)
// NetBSD exposes stdin/stdout/stderr as macros into a single `__sF[3]`
// array whose element size (sizeof(FILE)) depends on the platform and libc
// version, so we cannot forward-declare them. The FreeBSD-style
// `__stdinp/__stdoutp/__stderrp` symbols also do not exist on NetBSD (see
// vlang/v#27190). Defer to the system headers for FILE, the stdio streams,
// and the libc prototypes that would otherwise clash with the
// `__restrict`-qualified declarations in NetBSD libc.
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#elif defined(__TINYC__) && (defined(__FreeBSD__) || defined(__OpenBSD__))
// TinyCC reports a hard redefinition error if system OpenSSL pulls in
// <stdarg.h> after V has provided its own va_start macro. Include it first,
// but keep V manual FILE declarations on these BSD libc variants.
#include <stdarg.h>
#if defined(__FreeBSD__)
typedef struct __sFILE FILE;
extern FILE* __stdinp;
extern FILE* __stdoutp;
extern FILE* __stderrp;
#define stdin __stdinp
#define stdout __stdoutp
#define stderr __stderrp
#else
typedef struct __sFILE FILE;
#ifndef _STDFILES_DECLARED
	#define _STDFILES_DECLARED
struct __sFstub { long _stub; };
extern struct __sFstub __stdin[];
extern struct __sFstub __stdout[];
extern struct __sFstub __stderr[];
#endif
#define stdin ((struct __sFILE *)__stdin)
#define stdout ((struct __sFILE *)__stdout)
#define stderr ((struct __sFILE *)__stderr)
#endif
#elif (defined(__MINGW32__) || defined(__MINGW64__)) && defined(__V_GCC__)
// mingw-w64 stdio.h provides fprintf/vfprintf as static inline overrides
// when __USE_MINGW_ANSI_STDIO is enabled, so use the system declarations
// instead of the manual formatted-stdio prototypes below.
#include <stdarg.h>
#include <stdio.h>
#elif defined(__MINGW32__) || defined(__MINGW64__) || (defined(__clang__) && (defined(_WIN32) || defined(_WIN64)))
typedef struct _iobuf FILE;
FILE* __cdecl __acrt_iob_func(unsigned index);
#define stdin  (__acrt_iob_func(0))
#define stdout (__acrt_iob_func(1))
#define stderr (__acrt_iob_func(2))
#elif defined(__TINYC__) && (defined(_WIN32) || defined(_WIN64))
#ifndef _FILE_DEFINED
struct _iobuf {
	char *_ptr;
	int _cnt;
	char *_base;
	int _flag;
	int _file;
	int _charbuf;
	int _bufsiz;
	char *_tmpfname;
};
typedef struct _iobuf FILE;
#define _FILE_DEFINED
#endif
	#if defined(_WIN64)
FILE* __cdecl __iob_func(void);
	#else
		#ifdef _MSVCRT_
extern FILE _iob[];
			#define __iob_func() (_iob)
		#else
extern FILE (*_imp___iob)[];
			#define __iob_func() (*_imp___iob)
			#define _iob __iob_func()
		#endif
	#endif
#define stdin (&__iob_func()[0])
#define stdout (&__iob_func()[1])
#define stderr (&__iob_func()[2])
#elif defined(__vinix__)
typedef struct __file FILE;
extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;
struct __thread_data;
struct __threadattr;
// pthread_t handling for vinix builds:
//  - Vinix kernel (freestanding, __STDC_HOSTED__=0): no libc, define
//    pthread_t ourselves so V code that references it compiles.
//  - util-vinix cross-compiled on a libc-providing host (hosted, e.g.
//    glibc on Linux or macOS with -D__vinix__): pull pthread_t from
//    libc to avoid colliding with the libc typedef.
#if defined(__STDC_HOSTED__) && __STDC_HOSTED__ && defined(__has_include) && __has_include(<pthread.h>)
#include <pthread.h>
#else
typedef struct __thread_data *pthread_t;
#endif
typedef __builtin_va_list va_list;
#ifndef va_start
	#define va_start(ap, v) __builtin_va_start(ap, v)
#endif
#ifndef va_arg
	#define va_arg(ap, t) __builtin_va_arg(ap, t)
#endif
#ifndef va_end
	#define va_end(ap) __builtin_va_end(ap)
#endif
#ifndef va_copy
	#define va_copy(dest, src) __builtin_va_copy(dest, src)
#endif
#else
	#if defined(__APPLE__) || defined(__FreeBSD__)
typedef struct __sFILE FILE;
extern FILE* __stdinp;
extern FILE* __stdoutp;
extern FILE* __stderrp;
#define stdin __stdinp
#define stdout __stdoutp
#define stderr __stderrp
	#elif defined(__DragonFly__)
typedef struct __sFILE FILE;
extern FILE* __stdinp;
extern FILE* __stdoutp;
extern FILE* __stderrp;
#define stdin __stdinp
#define stdout __stdoutp
#define stderr __stderrp
	#elif defined(__OpenBSD__)
typedef struct __sFILE FILE;
#ifndef _STDFILES_DECLARED
	#define _STDFILES_DECLARED
struct __sFstub { long _stub; };
extern struct __sFstub __stdin[];
extern struct __sFstub __stdout[];
extern struct __sFstub __stderr[];
#endif
#define stdin ((struct __sFILE *)__stdin)
#define stdout ((struct __sFILE *)__stdout)
#define stderr ((struct __sFILE *)__stderr)
	#elif defined(__BIONIC__)
struct __sFILE;
typedef struct __sFILE FILE;
extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;
	#elif defined(__linux__) && !defined(__GLIBC__) && !defined(__GNU_LIBRARY__) && !defined(__BIONIC__) && !defined(__UCLIBC__)
typedef struct _IO_FILE FILE;
// musl exposes the stdio streams as `FILE *const`, so match that to stay
// compatible with later <stdio.h> includes from headers like miniz.h.
extern FILE* const stdin;
extern FILE* const stdout;
extern FILE* const stderr;
	#else
typedef struct _IO_FILE FILE;
extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;
#if defined(__GLIBC__) || defined(__GNU_LIBRARY__)
// V declares the stdio functions manually here, instead of including <stdio.h>.
// glibc defines L_tmpnam only while <stdio.h> is being processed (it sits behind
// `#ifdef _STDIO_H` in <bits/stdio_lim.h>), and it is the one stdio limit macro that
// <stdio.h> itself uses in a prototype: char *tmpnam(char[L_tmpnam]). So a <stdio.h>
// pulled in later by a module header (sqlite3.h, gc.h, ...) can fail with L_tmpnam
// being undeclared; see vlang/v#28108. Define it here, to the stable glibc value,
// without adding an include. A later identical redefinition by glibc is a no-op.
#ifndef L_tmpnam
#define L_tmpnam 20
#endif
#endif
	#endif
typedef __builtin_va_list va_list;
#ifndef va_start
	#define va_start(ap, v) __builtin_va_start(ap, v)
#endif
#ifndef va_arg
	#define va_arg(ap, t) __builtin_va_arg(ap, t)
#endif
#ifndef va_end
	#define va_end(ap) __builtin_va_end(ap)
#endif
#ifndef va_copy
	#define va_copy(dest, src) __builtin_va_copy(dest, src)
#endif
#endif
#if (!defined(_MSC_VER) || defined(__clang__)) && !defined(__cplusplus) && !defined(__NetBSD__)
// mingw-w64 stdio.h declares these as static __mingw_ovr inline overrides
// when __USE_MINGW_ANSI_STDIO is on. Skip them under gcc+mingw to avoid
// static-after-extern conflicts; clang+mingw needs them because it builds
// with -Werror=implicit-function-declaration and does not hit the conflict.
// NetBSD pulls these prototypes from <stdio.h>/<stdlib.h>/<string.h> via
// the include block above to avoid `__restrict` qualifier conflicts.
#if !((defined(__MINGW32__) || defined(__MINGW64__)) && !defined(__clang__))
V_CRT_LINKAGE int V_CRT_CALL vfprintf(FILE *stream, const char *format, va_list ap);
V_CRT_LINKAGE int V_CRT_CALL vsnprintf(char *str, size_t size, const char *format, va_list ap);
V_CRT_LINKAGE int V_CRT_CALL fprintf(FILE *stream, const char *format, ...);
V_CRT_LINKAGE int V_CRT_CALL printf(const char *format, ...);
V_CRT_LINKAGE int V_CRT_CALL snprintf(char *str, size_t size, const char *format, ...);
V_CRT_LINKAGE int V_CRT_CALL sprintf(char *str, const char *format, ...);
V_CRT_LINKAGE int V_CRT_CALL sscanf(const char *str, const char *format, ...);
V_CRT_LINKAGE int V_CRT_CALL scanf(const char *format, ...);
#endif
V_CRT_LINKAGE int V_CRT_CALL puts(const char *str);
V_CRT_LINKAGE void V_CRT_CALL perror(const char *str);
V_CRT_LINKAGE int V_CRT_CALL fputs(const char *str, FILE *stream);
V_CRT_LINKAGE int V_CRT_CALL getchar(void);
V_CRT_LINKAGE int V_CRT_CALL putchar(int ch);
V_CRT_LINKAGE int V_CRT_CALL getc(FILE *stream);
V_CRT_LINKAGE int V_CRT_CALL fgetc(FILE *stream);
V_CRT_LINKAGE int V_CRT_CALL ungetc(int ch, FILE *stream);
V_CRT_LINKAGE int V_CRT_CALL fflush(FILE *stream);
V_CRT_LINKAGE int V_CRT_CALL feof(FILE *stream);
V_CRT_LINKAGE int V_CRT_CALL ferror(FILE *stream);
V_CRT_LINKAGE void V_CRT_CALL clearerr(FILE *stream);
V_CRT_LINKAGE int V_CRT_CALL setvbuf(FILE *stream, char *buf, int mode, size_t size);
V_CRT_LINKAGE long V_CRT_CALL ftell(FILE *stream);
V_CRT_LINKAGE void V_CRT_CALL rewind(FILE *stream);
V_CRT_LINKAGE FILE * V_CRT_CALL fopen(const char *filename, const char *mode);
V_CRT_LINKAGE FILE * V_CRT_CALL fdopen(int fd, const char *mode);
V_CRT_LINKAGE FILE * V_CRT_CALL freopen(const char *filename, const char *mode, FILE *stream);
V_CRT_LINKAGE int V_CRT_CALL fileno(FILE *stream);
V_CRT_LINKAGE size_t V_CRT_CALL fread(void *ptr, size_t size, size_t items, FILE *stream);
V_CRT_LINKAGE size_t V_CRT_CALL fwrite(const void *ptr, size_t size, size_t items, FILE *stream);
#if defined(__vinix__)
V_CRT_LINKAGE char * V_CRT_CALL fgets(char *str, size_t size, FILE *stream);
#else
V_CRT_LINKAGE char * V_CRT_CALL fgets(char *str, int size, FILE *stream);
#endif
V_CRT_LINKAGE int V_CRT_CALL fclose(FILE *stream);
#if defined(__vinix__)
V_CRT_LINKAGE FILE * V_CRT_CALL popen(char *command, char *mode);
#else
V_CRT_STDIO_LINKAGE FILE * V_CRT_CALL popen(const char *command, const char *mode);
#endif
V_CRT_STDIO_LINKAGE int V_CRT_CALL pclose(FILE *stream);
V_CRT_LINKAGE void * V_CRT_CALL malloc(size_t size);
V_CRT_LINKAGE void * V_CRT_CALL calloc(size_t nitems, size_t size);
V_CRT_LINKAGE void * V_CRT_CALL realloc(void *ptr, size_t size);
V_CRT_LINKAGE void * V_CRT_CALL aligned_alloc(size_t alignment, size_t size);
V_CRT_LINKAGE int V_CRT_CALL posix_memalign(void **memptr, size_t alignment, size_t size);
V_CRT_LINKAGE void V_CRT_CALL free(void *ptr);
V_CRT_LINKAGE int V_CRT_CALL rand(void);
V_CRT_LINKAGE void V_CRT_CALL srand(unsigned int seed);
V_CRT_LINKAGE int V_CRT_CALL atexit(void (*cb)(void));
V_CRT_LINKAGE void V_CRT_CALL exit(int status);
V_CRT_LINKAGE int V_CRT_CALL abs(int n);
V_CRT_LINKAGE int V_CRT_CALL atoi(const char *str);
V_CRT_LINKAGE double V_CRT_CALL atof(const char *str);
V_CRT_LINKAGE char * V_CRT_CALL getenv(const char *name);
V_CRT_LINKAGE int V_CRT_CALL setenv(const char *name, const char *value, int overwrite);
V_CRT_LINKAGE int V_CRT_CALL unsetenv(const char *name);
V_CRT_LINKAGE int V_CRT_CALL system(const char *command);
V_CRT_LINKAGE int V_CRT_CALL remove(const char *path);
V_CRT_LINKAGE int V_CRT_CALL rename(const char *old_path, const char *new_path);
V_CRT_LINKAGE char * V_CRT_CALL realpath(const char *path, char *resolved_path);
V_CRT_LINKAGE int V_CRT_CALL mkstemp(char *stemplate);
V_CRT_LINKAGE void V_CRT_CALL qsort(void *base, size_t items, size_t item_size, qsort_callback_func cb);
#if defined(__vinix__)
V_CRT_LINKAGE int V_CRT_CALL strcmp(char *left, char *right);
V_CRT_LINKAGE int V_CRT_CALL strncmp(char *left, char *right, size_t n);
#else
V_CRT_LINKAGE int V_CRT_CALL strcmp(const char *left, const char *right);
V_CRT_LINKAGE int V_CRT_CALL strncmp(const char *left, const char *right, size_t n);
#endif
#if !defined(_WIN32) && !defined(_WIN64) && !defined(__BIONIC__)
V_CRT_LINKAGE char * V_CRT_CALL strdup(const char *str);
#endif
#if !defined(_WIN32) && !defined(_WIN64)
V_CRT_LINKAGE int V_CRT_CALL strcasecmp(const char *left, const char *right);
V_CRT_LINKAGE int V_CRT_CALL strncasecmp(const char *left, const char *right, size_t n);
#endif
#if defined(__vinix__)
V_CRT_LINKAGE size_t V_CRT_CALL strlen(char *str);
#else
V_CRT_LINKAGE size_t V_CRT_CALL strlen(const char *str);
#endif
V_CRT_LINKAGE char * V_CRT_CALL strerror(int errnum);
V_CRT_LINKAGE void * V_CRT_CALL memcpy(void *dest, const void *src, size_t n);
V_CRT_LINKAGE void * V_CRT_CALL memmove(void *dest, const void *src, size_t n);
V_CRT_LINKAGE void * V_CRT_CALL memset(void *dest, int ch, size_t n);
V_CRT_LINKAGE int V_CRT_CALL memcmp(const void *left, const void *right, size_t n);
// memchr/strchr/strrchr/strstr are the C23 type-generic string functions, and
// glibc 2.42+ implements them as function-like macros over _Generic, so that a
// const-qualified argument yields a const-qualified return type. If any include
// above already pulled in <string.h> (mbedtls/net_sockets.h, netdb.h, dirent.h,
// ... all do, and gcc 15 defaults to -std=gnu23), the name is already a macro
// here, and a declaration like
// `void *memchr(const void *str, int c, size_t n);` expands into the middle of
// a _Generic expression, which fails to parse:
// `error: expected identifier or ( before _Generic`.
// A defined macro also means <string.h> has already declared the real function,
// so skipping the declaration below loses nothing in that case. The reverse
// order stays fine as-is: a <string.h> pulled in later by a module header
// defines the macro after these declarations, which C permits.
#ifndef memchr
V_CRT_LINKAGE void * V_CRT_CALL memchr(const void *str, int c, size_t n);
#endif
#ifndef strchr
V_CRT_LINKAGE char * V_CRT_CALL strchr(const char *str, int c);
#endif
#ifndef strrchr
V_CRT_LINKAGE char * V_CRT_CALL strrchr(const char *str, int c);
#endif
#ifndef strstr
V_CRT_LINKAGE char * V_CRT_CALL strstr(const char *haystack, const char *needle);
#endif
V_CRT_LINKAGE int V_CRT_CALL fseek(FILE *stream, long offset, int whence);
V_CRT_LINKAGE isize V_CRT_CALL getline(char **lineptr, size_t *n, FILE *stream);
#if defined(_WIN32) || defined(_WIN64)
V_CRT_STDIO_LINKAGE int V_CRT_CALL _fseeki64(FILE *stream, i64 offset, int whence);
V_CRT_LINKAGE int V_CRT_CALL fgetpos(FILE *stream, i64 *pos);
V_CRT_STDIO_LINKAGE int V_CRT_CALL _fileno(FILE *stream);
V_CRT_STDIO_LINKAGE FILE * V_CRT_CALL _wfopen(const unsigned short *filename, const unsigned short *mode);
V_CRT_STDIO_LINKAGE int V_CRT_CALL freopen_s(FILE **new_stream, const char *filename, const char *mode, FILE *stream);
V_CRT_STDIO_LINKAGE FILE * V_CRT_CALL _wfreopen(const unsigned short *filename, const unsigned short *mode, FILE *stream);
V_CRT_STDIO_LINKAGE FILE * V_CRT_CALL _wpopen(const unsigned short *command, const unsigned short *mode);
V_CRT_STDIO_LINKAGE int V_CRT_CALL _pclose(FILE *stream);
V_CRT_STDIO_LINKAGE int V_CRT_CALL _wremove(const unsigned short *path);
V_CRT_LINKAGE void * V_CRT_CALL _aligned_malloc(size_t size, size_t alignment);
V_CRT_LINKAGE void * V_CRT_CALL _aligned_realloc(void *memory, size_t size, size_t alignment);
V_CRT_LINKAGE void V_CRT_CALL _aligned_free(void *memory);
V_CRT_LINKAGE unsigned short * V_CRT_CALL _wgetenv(const unsigned short *varname);
V_CRT_LINKAGE int V_CRT_CALL _wputenv(const unsigned short *envstring);
V_CRT_LINKAGE int V_CRT_CALL _putenv_s(const char *name, const char *value);
#endif
#if defined(_MSC_VER) && !defined(__clang__)
#ifndef _TRUNCATE
	#define _TRUNCATE ((size_t)-1)
#endif
V_CRT_LINKAGE int V_CRT_CALL _vscprintf(const char *format, va_list ap);
V_CRT_LINKAGE int V_CRT_CALL _vsnprintf_s(char *buffer, size_t size, size_t count, const char *format, va_list ap);
#endif
#endif
#ifndef _IOFBF
	#define _IOFBF 0
#endif
#ifndef _IOLBF
	#define _IOLBF 1
#endif
#ifndef _IONBF
	#define _IONBF 2
#endif
#ifndef EOF
	#define EOF (-1)
#endif
#ifndef SEEK_SET
	#define SEEK_SET 0
#endif
#ifndef SEEK_CUR
	#define SEEK_CUR 1
#endif
#ifndef SEEK_END
	#define SEEK_END 2
#endif
#ifndef RAND_MAX
enum {
	#if defined(_MSC_VER)
		RAND_MAX = 0x7fff
	#else
		RAND_MAX = 2147483647
	#endif
};
#endif
#undef V_CRT_STDIO_LINKAGE
#undef V_CRT_LINKAGE
#undef V_CRT_CALL
#define V_MANUAL_STDLIB_HEADERS 1
void abort(void);
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#if defined(__has_include)
#if __has_include(<wchar.h>)
#include <wchar.h>
#endif
#else
#include <wchar.h>
#endif
#if defined(_WIN32) && (defined(__TINYC__) || (defined(_MSC_VER) && !defined(__clang__)))
#include "C:/Users/xman/v/thirdparty/stdatomic/win/atomic.h"
#else
#if defined(__OBJC__) && defined(__GNUC__) && !defined(__clang__)
#define _Atomic volatile
#endif
#include <stdatomic.h>
#if defined(__OBJC__) && defined(__GNUC__) && !defined(__clang__)
#undef _Atomic
#endif
#endif
#if defined(__linux__) || defined(__ANDROID__)
#include <sys/syscall.h>
#endif
#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#include <sys/event.h>
#endif
#ifdef _WIN32
#include <io.h>
#include <process.h>
#include <windows.h>
#if defined(_MSC_VER)
#include <intrin.h>
#include <dbghelp.h>
#endif
#else
#include <dirent.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <semaphore.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <utime.h>
#endif
#ifdef __APPLE__
#include <mach/mach_time.h>
#include <mach-o/dyld.h>
#endif
#if defined(__linux__) || defined(__ANDROID__)
#include <sys/epoll.h>
#endif
#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#include <sys/event.h>
#endif
#if defined(__has_include) && !defined(__TINYC__)
	#if __has_include(<execinfo.h>) && !defined(_WIN32)
		#define __V_HAVE_EXECINFO_H 1
		#include <execinfo.h>
	#endif
#elif (defined(__linux__) && (defined(__GLIBC__) || defined(__GNU_LIBRARY__))) || defined(__APPLE__) || defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)
	#define __V_HAVE_EXECINFO_H 1
	#include <execinfo.h>
#endif
#if !defined(__V_HAVE_EXECINFO_H) && !defined(_WIN32)
int backtrace(void** __array, int __size);
char** backtrace_symbols(void* const* __array, int __size);
void backtrace_symbols_fd(void* const* __array, int __size, int __fd);
#endif

#ifndef _WIN32
extern char** environ;
#endif
static void* __v_thread_alloc(size_t size) {
#if defined(_VGCBOEHM) || defined(CUSTOM_DEFINE_gcboehm)
	void* p = GC_MALLOC_UNCOLLECTABLE(size);
#else
	void* p = malloc(size);
#endif
	if (!p) { fprintf(stderr, "V thread allocation failed\n"); abort(); }
	return p;
}
static void __v_thread_free(void* ptr) {
#if defined(_VGCBOEHM) || defined(CUSTOM_DEFINE_gcboehm)
	GC_FREE(ptr);
#else
	free(ptr);
#endif
}
#ifdef _WIN32
typedef struct { HANDLE handle; void* context; } __v_thread;
static bool __v_thread_equal(__v_thread a, __v_thread b) { return a.handle == b.handle; }
typedef void* (*__v_thread_start_fn)(void*);
typedef struct { __v_thread_start_fn start; void* arg; void* result; } __v_windows_thread_context;
static const size_t __v_thread_stack_size = V_THREAD_STACK_SIZE;
static DWORD WINAPI __v_windows_thread_start(void* raw_context) { __v_windows_thread_context* context = (__v_windows_thread_context*)raw_context; context->result = context->start(context->arg); return 0; }
static __v_thread __v_thread_spawn(__v_thread_start_fn start, void* arg, void (*cleanup)(void*)) {
	__v_thread result;
	__v_windows_thread_context* context = (__v_windows_thread_context*)__v_thread_alloc(sizeof(__v_windows_thread_context));
	context->start = start; context->arg = arg; context->result = NULL;
	result.context = context;
	result.handle = CreateThread(NULL, __v_thread_stack_size, __v_windows_thread_start, context, 0, NULL);
	if (!result.handle) { DWORD error = GetLastError(); free(context); if (cleanup) cleanup(arg); fprintf(stderr, "V thread creation failed: %lu\n", (unsigned long)error); abort(); }
	return result;
}
static void* __v_thread_join(__v_thread thread) {
	DWORD rc = WaitForSingleObject(thread.handle, INFINITE);
	if (rc != WAIT_OBJECT_0) { fprintf(stderr, "V thread join failed: %lu\n", (unsigned long)rc); abort(); }
	void* result = ((__v_windows_thread_context*)thread.context)->result;
	if (!CloseHandle(thread.handle)) { DWORD error = GetLastError(); __v_thread_free(thread.context); fprintf(stderr, "V thread handle cleanup failed: %lu\n", (unsigned long)error); abort(); }
	__v_thread_free(thread.context);
	return result;
}
static DWORD WINAPI __v_windows_detached_thread_start(void* raw_context) { __v_windows_thread_context context = *(__v_windows_thread_context*)raw_context; __v_thread_free(raw_context); void* result = context.start(context.arg); if (result) __v_thread_free(result); return 0; }
static __v_thread __v_thread_spawn_detached(__v_thread_start_fn start, void* arg, void (*cleanup)(void*)) {
	__v_windows_thread_context* context = (__v_windows_thread_context*)__v_thread_alloc(sizeof(__v_windows_thread_context));
	context->start = start; context->arg = arg; context->result = NULL;
	HANDLE handle = CreateThread(NULL, __v_thread_stack_size, __v_windows_detached_thread_start, context, 0, NULL);
	if (!handle) { DWORD error = GetLastError(); __v_thread_free(context); if (cleanup) cleanup(arg); fprintf(stderr, "V thread creation failed: %lu\n", (unsigned long)error); abort(); }
	if (!CloseHandle(handle)) { fprintf(stderr, "V thread handle cleanup failed: %lu\n", (unsigned long)GetLastError()); abort(); }
	return (__v_thread){0};
}
static __v_thread __v_thread_spawn_comparable(__v_thread_start_fn start, void* arg, void (*cleanup)(void*)) {
	__v_windows_thread_context* context = (__v_windows_thread_context*)__v_thread_alloc(sizeof(__v_windows_thread_context));
	context->start = start; context->arg = arg; context->result = NULL;
	HANDLE handle = CreateThread(NULL, __v_thread_stack_size, __v_windows_detached_thread_start, context, 0, NULL);
	if (!handle) { DWORD error = GetLastError(); __v_thread_free(context); if (cleanup) cleanup(arg); fprintf(stderr, "V thread creation failed: %lu\n", (unsigned long)error); abort(); }
	return (__v_thread){.handle = handle, .context = NULL};
}
static void __v_thread_release_comparable(__v_thread thread) { if (thread.handle && !CloseHandle(thread.handle)) { fprintf(stderr, "V thread handle cleanup failed: %lu\n", (unsigned long)GetLastError()); abort(); } }
#else
typedef struct { pthread_t handle; } __v_thread;
static bool __v_thread_equal(__v_thread a, __v_thread b) { return pthread_equal(a.handle, b.handle) != 0; }
typedef void* (*__v_thread_start_fn)(void*);
static const size_t __v_thread_stack_size = V_THREAD_STACK_SIZE;
static __v_thread __v_thread_spawn(__v_thread_start_fn start, void* arg, void (*cleanup)(void*)) {
	__v_thread result;
	pthread_attr_t attr;
	int rc = pthread_attr_init(&attr);
	if (rc != 0) { if (cleanup) cleanup(arg); fprintf(stderr, "V thread attribute initialization failed: %d\n", rc); abort(); }
	rc = pthread_attr_setstacksize(&attr, __v_thread_stack_size);
	if (rc != 0) { pthread_attr_destroy(&attr); if (cleanup) cleanup(arg); fprintf(stderr, "V thread stack size setup failed: %d\n", rc); abort(); }
	rc = pthread_create(&result.handle, &attr, (void*)start, arg);
	int attr_rc = pthread_attr_destroy(&attr);
	if (rc != 0) { if (cleanup) cleanup(arg); fprintf(stderr, "V thread creation failed: %d\n", rc); abort(); }
	if (attr_rc != 0) { fprintf(stderr, "V thread attribute cleanup failed: %d\n", attr_rc); abort(); }
	return result;
}
static void* __v_thread_join(__v_thread thread) { void* result = NULL; int rc = pthread_join(thread.handle, &result); if (rc != 0) { fprintf(stderr, "V thread join failed: %d\n", rc); abort(); } return result; }
typedef struct { __v_thread_start_fn start; void* arg; } __v_detached_thread_context;
static void* __v_detached_thread_start(void* raw_context) { __v_detached_thread_context context = *(__v_detached_thread_context*)raw_context; __v_thread_free(raw_context); void* result = context.start(context.arg); if (result) __v_thread_free(result); return NULL; }
static __v_thread __v_thread_spawn_detached(__v_thread_start_fn start, void* arg, void (*cleanup)(void*)) {
	__v_detached_thread_context* context = (__v_detached_thread_context*)__v_thread_alloc(sizeof(__v_detached_thread_context));
	context->start = start; context->arg = arg;
	pthread_t handle;
	pthread_attr_t attr;
	int rc = pthread_attr_init(&attr);
	if (rc != 0) { __v_thread_free(context); if (cleanup) cleanup(arg); fprintf(stderr, "V thread attribute initialization failed: %d\n", rc); abort(); }
	rc = pthread_attr_setstacksize(&attr, __v_thread_stack_size);
	if (rc == 0) rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (rc != 0) { pthread_attr_destroy(&attr); __v_thread_free(context); if (cleanup) cleanup(arg); fprintf(stderr, "V thread attribute setup failed: %d\n", rc); abort(); }
	rc = pthread_create(&handle, &attr, (void*)__v_detached_thread_start, context);
	int attr_rc = pthread_attr_destroy(&attr);
	if (rc != 0) { __v_thread_free(context); if (cleanup) cleanup(arg); fprintf(stderr, "V thread creation failed: %d\n", rc); abort(); }
	if (attr_rc != 0) { fprintf(stderr, "V thread attribute cleanup failed: %d\n", attr_rc); abort(); }
	return (__v_thread){0};
}
static __v_thread __v_thread_spawn_comparable(__v_thread_start_fn start, void* arg, void (*cleanup)(void*)) { return __v_thread_spawn(start, arg, cleanup); }
static void __v_thread_release_comparable(__v_thread thread) { if (thread.handle) { int rc = pthread_detach(thread.handle); if (rc != 0) { fprintf(stderr, "V thread detach failed: %d\n", rc); abort(); } } }
#endif
#define __v_thread_signal_stack_enter(stack_top) ((void)(stack_top), (void*)0)
#define __v_thread_signal_stack_leave(alt_stack) ((void)(alt_stack))
#ifndef __V_architecture
#define __V_architecture 0
#endif
#if defined(__x86_64__) || defined(_M_AMD64)
#define __V_amd64 1
#undef __V_architecture
#define __V_architecture 1
#endif
#if defined(__aarch64__) || defined(__arm64__) || defined(_M_ARM64)
#define __V_arm64 1
#undef __V_architecture
#define __V_architecture 2
#endif
#if defined(__arm__) || defined(_M_ARM)
#define __V_arm32 1
#undef __V_architecture
#define __V_architecture 3
#endif
#if defined(__riscv) && __riscv_xlen == 64
#define __V_rv64 1
#undef __V_architecture
#define __V_architecture 4
#endif
#if defined(__riscv) && __riscv_xlen == 32
#define __V_rv32 1
#undef __V_architecture
#define __V_architecture 5
#endif
#if defined(__i386__) || defined(_M_IX86)
#define __V_x86 1
#undef __V_architecture
#define __V_architecture 6
#endif
#if defined(__s390x__)
#define __V_s390x 1
#undef __V_architecture
#define __V_architecture 7
#endif
#if defined(__powerpc64__) && defined(__LITTLE_ENDIAN__)
#define __V_ppc64le 1
#undef __V_architecture
#define __V_architecture 8
#endif
#if defined(__loongarch64)
#define __V_loongarch64 1
#undef __V_architecture
#define __V_architecture 9
#endif
#if defined(__sparc__)
#define __V_sparc64 1
#undef __V_architecture
#define __V_architecture 10
#endif
#if defined(__powerpc64__) && defined(__BIG_ENDIAN__)
#define __V_ppc64 1
#undef __V_architecture
#define __V_architecture 11
#endif
#if (defined(__powerpc__) || defined(__powerpc) || defined(__POWERPC__) || defined(__ppc__) || defined(__ppc) || defined(__PPC__)) && !defined(__powerpc64__) && !defined(__ppc64__) && !defined(__PPC64__)
#define __V_ppc 1
#undef __V_architecture
#define __V_architecture 12
#endif
#if UINTPTR_MAX == 0xFFFFFFFFFFFFFFFFu
#define TARGET_IS_64BIT 1
#else
#define TARGET_IS_32BIT 1
#endif
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define TARGET_ORDER_IS_BIG 1
#else
#define TARGET_ORDER_IS_LITTLE 1
#endif

#define elem_size element_size
#define c_name types__c_name
#define builtin__string_clone string__clone
#define builtin__tos2 tos2
/* V3CACHE_NATIVE_DIRECTIVES_BEGIN */
#include "C:\Users\xman\v/vlib/builtin/track_heap_checks.h"
#include "C:\Users\xman\v/vlib/builtin/gc_debugger_linux.h"
#define v_gc_set_warn_proc(cb) GC_set_warn_proc((GC_warn_proc)(cb))
#define v_gc_get_warn_proc() ((void *)GC_get_warn_proc())
#define v_gc_set_abort_func(cb) GC_set_abort_func((GC_abort_func)(cb))
#define v_gc_get_abort_func() ((void *)GC_get_abort_func())
#define v_gc_call_abort_func(fn, msg) ((GC_abort_func)(fn))(msg)
#define v_set_unhandled_exception_filter(handler) SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)(handler))
#include "C:\Users\xman\v/vlib/builtin/cfns_windows_tcc.h"
#define v_signal_with_handler_cast(sig, handler) signal((sig), (void (*)(int))(handler))

/* V3CACHE_NATIVE_DIRECTIVES_END */
typedef enum {
	ArrayFlags__noslices = 1,
	ArrayFlags__noshrink = 2,
	ArrayFlags__nogrow = 4,
	ArrayFlags__nofree = 8,
	ArrayFlags__managed = 16,
	ArrayFlags__noscan_data = 32,
	ArrayFlags__is_slice = 64,
} ArrayFlags;

typedef enum {
	ChanState__success = 0,
	ChanState__not_ready = 1,
	ChanState__closed = 2,
} ChanState;

typedef enum {
	GraphemeBreakProperty__other = 0,
	GraphemeBreakProperty__cr = 1,
	GraphemeBreakProperty__lf = 2,
	GraphemeBreakProperty__control = 3,
	GraphemeBreakProperty__extend = 4,
	GraphemeBreakProperty__regional_indicator = 5,
	GraphemeBreakProperty__prepend = 6,
	GraphemeBreakProperty__spacing_mark = 7,
	GraphemeBreakProperty__l = 8,
	GraphemeBreakProperty__v = 9,
	GraphemeBreakProperty__t = 10,
	GraphemeBreakProperty__lv = 11,
	GraphemeBreakProperty__lvt = 12,
	GraphemeBreakProperty__zwj = 13,
} GraphemeBreakProperty;

typedef enum {
	AttributeKind__plain = 0,
	AttributeKind__string = 1,
	AttributeKind__number = 2,
	AttributeKind__bool = 3,
	AttributeKind__comptime_define = 4,
} AttributeKind;

typedef enum {
	MapMode__to_upper = 0,
	MapMode__to_lower = 1,
	MapMode__to_title = 2,
} MapMode;

typedef enum {
	TrimMode__trim_left = 0,
	TrimMode__trim_right = 1,
	TrimMode__trim_both = 2,
} TrimMode;

typedef enum {
	StrIntpType__si_no_str = 0,
	StrIntpType__si_c = 1,
	StrIntpType__si_u8 = 2,
	StrIntpType__si_i8 = 3,
	StrIntpType__si_u16 = 4,
	StrIntpType__si_i16 = 5,
	StrIntpType__si_u32 = 6,
	StrIntpType__si_i32 = 7,
	StrIntpType__si_u64 = 8,
	StrIntpType__si_i64 = 9,
	StrIntpType__si_e32 = 10,
	StrIntpType__si_e64 = 11,
	StrIntpType__si_f32 = 12,
	StrIntpType__si_f64 = 13,
	StrIntpType__si_g32 = 14,
	StrIntpType__si_g64 = 15,
	StrIntpType__si_s = 16,
	StrIntpType__si_p = 17,
	StrIntpType__si_r = 18,
	StrIntpType__si_vp = 19,
} StrIntpType;

typedef enum {
	strings__IndentState__normal = 0,
	strings__IndentState__in_string = 1,
} strings__IndentState;

typedef enum {
	strconv__Align_text__right = 0,
	strconv__Align_text__left = 1,
	strconv__Align_text__center = 2,
} strconv__Align_text;

typedef enum {
	strconv__ParserState__ok = 0,
	strconv__ParserState__pzero = 1,
	strconv__ParserState__mzero = 2,
	strconv__ParserState__pinf = 3,
	strconv__ParserState__minf = 4,
	strconv__ParserState__invalid_number = 5,
	strconv__ParserState__extra_char = 6,
} strconv__ParserState;

typedef enum {
	strconv__Char_parse_state__start = 0,
	strconv__Char_parse_state__norm_char = 1,
	strconv__Char_parse_state__field_char = 2,
	strconv__Char_parse_state__pad_ch = 3,
	strconv__Char_parse_state__len_set_start = 4,
	strconv__Char_parse_state__len_set_in = 5,
	strconv__Char_parse_state__check_type = 6,
	strconv__Char_parse_state__check_float = 7,
	strconv__Char_parse_state__check_float_in = 8,
	strconv__Char_parse_state__reset_params = 9,
} strconv__Char_parse_state;

typedef enum {
	os__WalkDirAction__proceed = 0,
	os__WalkDirAction__skip_dir = 1,
	os__WalkDirAction__stop = 2,
} os__WalkDirAction;

typedef enum {
	os__ProcessState__not_started = 0,
	os__ProcessState__running = 1,
	os__ProcessState__stopped = 2,
	os__ProcessState__exited = 3,
	os__ProcessState__aborted = 4,
	os__ProcessState__closed = 5,
} os__ProcessState;

typedef enum {
	os__Signal__hup = 1,
	os__Signal__v_int = 2,
	os__Signal__quit = 3,
	os__Signal__ill = 4,
	os__Signal__trap = 5,
	os__Signal__abrt = 6,
	os__Signal__bus = 7,
	os__Signal__fpe = 8,
	os__Signal__kill = 9,
	os__Signal__usr1 = 10,
	os__Signal__segv = 11,
	os__Signal__usr2 = 12,
	os__Signal__v_pipe = 13,
	os__Signal__alrm = 14,
	os__Signal__term = 15,
	os__Signal__stkflt = 16,
	os__Signal__chld = 17,
	os__Signal__cont = 18,
	os__Signal__stop = 19,
	os__Signal__tstp = 20,
	os__Signal__ttin = 21,
	os__Signal__ttou = 22,
	os__Signal__urg = 23,
	os__Signal__xcpu = 24,
	os__Signal__xfsz = 25,
	os__Signal__vtalrm = 26,
	os__Signal__prof = 27,
	os__Signal__winch = 28,
	os__Signal__poll = 29,
	os__Signal__pwr = 30,
	os__Signal__sys = 31,
} os__Signal;

typedef enum {
	os__SeekMode__start = 0,
	os__SeekMode__current = 1,
	os__SeekMode__end = 2,
} os__SeekMode;

typedef enum {
	os__FileBufferMode__fully_buffered = _IOFBF,
	os__FileBufferMode__line_buffered = _IOLBF,
	os__FileBufferMode__not_buffered = _IONBF,
} os__FileBufferMode;

typedef enum {
	os__FileType__unknown = 0,
	os__FileType__regular = 1,
	os__FileType__directory = 2,
	os__FileType__character_device = 3,
	os__FileType__block_device = 4,
	os__FileType__fifo = 5,
	os__FileType__symbolic_link = 6,
	os__FileType__socket = 7,
} os__FileType;

typedef enum {
	os__ChildProcessPipeKind__v_stdin = 0,
	os__ChildProcessPipeKind__v_stdout = 1,
	os__ChildProcessPipeKind__v_stderr = 2,
} os__ChildProcessPipeKind;

typedef enum {
	blip__Tag__nil = 1,
	blip__Tag__boolean = 2,
	blip__Tag__integer = 4,
	blip__Tag__v_float = 8,
	blip__Tag__rune = 16,
	blip__Tag__string = 32,
	blip__Tag__symbol = 64,
	blip__Tag__keyword = 128,
	blip__Tag__pair = 256,
	blip__Tag__emptylist = 512,
	blip__Tag__vector = 1024,
	blip__Tag__array = 2048,
	blip__Tag__table = 4096,
	blip__Tag__buffer = 8192,
	blip__Tag__closure = 16384,
	blip__Tag__primitive = 32768,
	blip__Tag__continuation = 65536,
	blip__Tag__struct_ = 131072,
} blip__Tag;

typedef enum {
	blip__KontTag__done = 1,
	blip__KontTag__app_fn = 2,
	blip__KontTag__app_arg = 4,
	blip__KontTag__if_k = 8,
	blip__KontTag__seq = 16,
	blip__KontTag__define_k = 32,
	blip__KontTag__set_k = 64,
	blip__KontTag__and_k = 128,
	blip__KontTag__or_k = 256,
	blip__KontTag__field_k = 512,
	blip__KontTag__assert_k = 1024,
	blip__KontTag__use_k = 2048,
	blip__KontTag__match_k = 4096,
	blip__KontTag__guard_k = 8192,
	blip__KontTag__collect_k = 16384,
	blip__KontTag__try_k = 32768,
} blip__KontTag;

typedef enum {
	machine__Ctl__eval_form = 1,
	machine__Ctl__return_value = 2,
} machine__Ctl;

typedef enum {
	closure__MemoryProtectAtrr__read_exec = 0,
	closure__MemoryProtectAtrr__read_write = 1,
} closure__MemoryProtectAtrr;

typedef u8 reader__DatumTag;
#define reader__DatumTag__nil ((reader__DatumTag)(0))
#define reader__DatumTag__bool ((reader__DatumTag)(1))
#define reader__DatumTag__v_int ((reader__DatumTag)(2))
#define reader__DatumTag__rat ((reader__DatumTag)(3))
#define reader__DatumTag__v_float ((reader__DatumTag)(4))
#define reader__DatumTag__v_char ((reader__DatumTag)(5))
#define reader__DatumTag__str ((reader__DatumTag)(6))
#define reader__DatumTag__sym ((reader__DatumTag)(7))
#define reader__DatumTag__kw ((reader__DatumTag)(8))
#define reader__DatumTag__list ((reader__DatumTag)(9))
#define reader__DatumTag__vector ((reader__DatumTag)(10))
#define reader__DatumTag__table ((reader__DatumTag)(11))
#define reader__DatumTag__array ((reader__DatumTag)(12))
#define reader__DatumTag__bytes ((reader__DatumTag)(13))
#define reader__DatumTag__buffer ((reader__DatumTag)(14))
#define reader__DatumTag__quoted ((reader__DatumTag)(15))
#define reader__DatumTag__quasi ((reader__DatumTag)(16))
#define reader__DatumTag__unquote ((reader__DatumTag)(17))
#define reader__DatumTag__unquote_splice ((reader__DatumTag)(18))

typedef struct AutostrAddrStackState AutostrAddrStackState;
typedef struct ContextRecord ContextRecord;
typedef struct DenseArray DenseArray;
typedef struct Error Error;
typedef struct ExceptionPointers ExceptionPointers;
typedef struct ExceptionRecord ExceptionRecord;
typedef struct MessageError MessageError;
typedef struct None__ None__;
typedef struct OwnershipV3InterfacePayload OwnershipV3InterfacePayload;
typedef struct PanicDebugInfo PanicDebugInfo;
typedef struct PanicFrame PanicFrame;
typedef struct PanicRecord PanicRecord;
typedef struct PanicState PanicState;
typedef struct main__Suite main__Suite;
typedef struct VMapData VMapData;
typedef struct VMemoryBlock VMemoryBlock;
typedef struct VPreallocBlockCache VPreallocBlockCache;
typedef struct VPreallocRange VPreallocRange;
typedef struct VPreallocScope VPreallocScope;
typedef struct array array;
typedef struct blip__Closure blip__Closure;
typedef struct blip__Env blip__Env;
typedef struct blip__EnvArena blip__EnvArena;
typedef struct blip__Kont blip__Kont;
typedef struct blip__NoPayload blip__NoPayload;
typedef struct blip__Pair blip__Pair;
typedef struct blip__StringPayload blip__StringPayload;
typedef struct blip__StructVal blip__StructVal;
typedef struct blip__Table blip__Table;
typedef struct blip__Value blip__Value;
typedef struct blip__Vector blip__Vector;
typedef struct closure__Closure closure__Closure;
typedef struct closure__ClosureLifetimeFrame closure__ClosureLifetimeFrame;
typedef struct closure__ClosureLifetimeRecord closure__ClosureLifetimeRecord;
typedef struct closure__ClosureLifetimeState closure__ClosureLifetimeState;
typedef struct closure__ClosureLiveInfo closure__ClosureLiveInfo;
typedef struct closure__ClosureMutex closure__ClosureMutex;
typedef struct closure__ClosurePage closure__ClosurePage;
typedef struct dl__InterfaceExport dl__InterfaceExport;
typedef struct dl__InterfaceExportTable dl__InterfaceExportTable;
typedef struct host__CaptureHost host__CaptureHost;
typedef struct host__CaptureState_ host__CaptureState_;
typedef struct host__ConsoleHost host__ConsoleHost;
typedef struct machine__Bind machine__Bind;
typedef struct machine__Destructure machine__Destructure;
typedef struct machine__Destructured machine__Destructured;
typedef struct machine__Frame machine__Frame;
typedef struct machine__Group machine__Group;
typedef struct machine__Machine machine__Machine;
typedef struct machine__Matched machine__Matched;
typedef struct machine__ParamSpec machine__ParamSpec;
typedef struct map map;
typedef struct math__ChebSeries math__ChebSeries;
typedef struct os__ContextRecord os__ContextRecord;
typedef struct os__Eof os__Eof;
typedef struct os__ExceptionPointers os__ExceptionPointers;
typedef struct os__ExceptionRecord os__ExceptionRecord;
typedef struct os__ExecutableNotFoundError os__ExecutableNotFoundError;
typedef struct os__FileNotOpenedError os__FileNotOpenedError;
typedef struct os__NotExpected os__NotExpected;
typedef struct os__SizeOfTypeIs0Error os__SizeOfTypeIs0Error;
typedef struct os__WalkDirEntry os__WalkDirEntry;
typedef struct reader__Arena reader__Arena;
typedef struct reader__Datum reader__Datum;
typedef struct reader__Depth reader__Depth;
typedef struct reader__Diagnostic reader__Diagnostic;
typedef struct reader__Forms reader__Forms;
typedef struct reader__ReadResult reader__ReadResult;
typedef struct reader__Reader reader__Reader;
typedef struct strconv__AtoF64Param strconv__AtoF64Param;
typedef struct strconv__Dec32 strconv__Dec32;
typedef struct strconv__Dec64 strconv__Dec64;
typedef struct strconv__PrepNumber strconv__PrepNumber;
typedef struct string string;
typedef struct IError IError;
typedef struct blip__Payload blip__Payload;
typedef struct hash__Hash hash__Hash;
typedef struct hash__Hash32er hash__Hash32er;
typedef struct hash__Hash64er hash__Hash64er;
typedef struct host__Host host__Host;
typedef array Array;

typedef struct multi_return_double_double multi_return_double_double;
typedef struct multi_return_double_i64 multi_return_double_i64;
typedef struct multi_return_u32_u32 multi_return_u32_u32;
typedef struct multi_return_string_string multi_return_string_string;
typedef struct multi_return_i64_i64 multi_return_i64_i64;
typedef struct multi_return_u32_i64 multi_return_u32_i64;
typedef struct multi_return_size_t_size_t multi_return_size_t_size_t;
typedef struct multi_return_size_t_size_t_size_t_size_t_size_t multi_return_size_t_size_t_size_t_size_t_size_t;
typedef struct multi_return_u64_i64 multi_return_u64_i64;
typedef struct multi_return_i64_bool multi_return_i64_bool;
typedef struct multi_return_u32_bool_i64 multi_return_u32_bool_i64;
typedef struct multi_return_string_i64 multi_return_string_i64;
typedef struct multi_return_u32_u32_u32 multi_return_u32_u32_u32;
typedef struct multi_return_int_strconv__PrepNumber multi_return_int_strconv__PrepNumber;
typedef struct multi_return_strconv__Dec32_bool multi_return_strconv__Dec32_bool;
typedef struct multi_return_strconv__Dec64_bool multi_return_strconv__Dec64_bool;
typedef struct multi_return_string_string_string multi_return_string_string_string;
typedef struct multi_return_Array_Array multi_return_Array_Array;
typedef struct multi_return_u64_u64 multi_return_u64_u64;
typedef struct multi_return_i64_i64_i64 multi_return_i64_i64_i64;

typedef double Array_fixed_double_36[36];
typedef double Array_fixed_double_4[4];
typedef i32 Array_fixed_i32_1264[1264];
typedef i64 Array_fixed_i64_3[3];
typedef i64 Array_fixed_i64_64[64];
typedef u16 Array_fixed_u16_112[112];
typedef u16 Array_fixed_u16_133[133];
typedef u16 Array_fixed_u16_14[14];
typedef u16 Array_fixed_u16_16[16];
typedef u16 Array_fixed_u16_260[260];
typedef u16 Array_fixed_u16_424[424];
typedef u32 Array_fixed_u32_10[10];
typedef u32 Array_fixed_u32_508[508];
typedef u64 Array_fixed_u64_18[18];
typedef u64 Array_fixed_u64_2[2];
typedef u64 Array_fixed_u64_20[20];
typedef u64 Array_fixed_u64_309[309];
typedef u64 Array_fixed_u64_31[31];
typedef u64 Array_fixed_u64_324[324];
typedef u64 Array_fixed_u64_47[47];
typedef u64 Array_fixed_u64_584[584];
typedef u64 Array_fixed_u64_652[652];
typedef u8 Array_fixed_u8_15[15];
typedef u8 Array_fixed_u8_256[256];
typedef u8 Array_fixed_u8_32[32];
typedef u8 Array_fixed_u8_364[364];
typedef u8 Array_fixed_u8_6[6];
typedef u8 Array_fixed_u8_64[64];
typedef u8 Array_fixed_u8_65536[65536];
typedef void* Array_fixed_voidptr_11[11];
typedef void* Array_fixed_voidptr_64[64];
typedef void* Array_fixed_voidptr_8[8];

struct __v_result_blip__Value;

typedef struct __v_result_blip__Value (*_fn_ptr_d85a1a99927adfac)(Array);
typedef bool (*_fn_ptr_c95f092653bee64)(void*, void*);
typedef i64 (*_fn_ptr_ee0265116ec69588)(ExceptionPointers*);
typedef i64 (*_fn_ptr_ed95774d3a4e97ab)(void*, void*);
typedef int (*_fn_ptr_24927a217aab3552)(string, os__WalkDirEntry);
typedef u32 (*_fn_ptr_f33ca158569fc3fd)(os__ExceptionPointers*);
typedef u64 (*_fn_ptr_1e8d64a3d95a0da1)(void*);
typedef void* (*_fn_ptr_4104d547b520960a)(void);
typedef void (*_fn_ptr_1ad43b93b076e910)(char*);
typedef void (*_fn_ptr_a73f2ae88698453e)(char*, size_t);
typedef void (*_fn_ptr_21efbbd63959de6f)(i64);
typedef void (*_fn_ptr_22778bd639cd2677)(int);
typedef void (*_fn_ptr_f7530b1a1b506de7)(string);
typedef void (*_fn_ptr_39790037d38ea68)(void);
typedef void (*_fn_ptr_5373f7edc7b60e26)(void*);
typedef void (*_fn_ptr_a6b6a5be4f3dabb3)(void*, string);
typedef void (*_fn_ptr_e827d8554d353483)(void*, u16*, u16*, u16*, u16*, i64);
typedef void (*_fn_ptr_1a3da2026b1489ea)(void*, void*);

typedef struct AutostrAddrStackState AutostrAddrStackState;
typedef struct ContextRecord ContextRecord;
typedef struct DenseArray DenseArray;
typedef struct Error Error;
typedef struct ExceptionPointers ExceptionPointers;
typedef struct ExceptionRecord ExceptionRecord;
typedef struct MessageError MessageError;
typedef struct None__ None__;
typedef struct OwnershipV3InterfacePayload OwnershipV3InterfacePayload;
typedef struct PanicDebugInfo PanicDebugInfo;
typedef struct PanicFrame PanicFrame;
typedef struct PanicRecord PanicRecord;
typedef struct PanicState PanicState;
typedef struct main__Suite main__Suite;
typedef struct VMapData VMapData;
typedef struct VMemoryBlock VMemoryBlock;
typedef struct VPreallocBlockCache VPreallocBlockCache;
typedef struct VPreallocRange VPreallocRange;
typedef struct VPreallocScope VPreallocScope;
typedef struct array array;
typedef struct blip__Closure blip__Closure;
typedef struct blip__Env blip__Env;
typedef struct blip__EnvArena blip__EnvArena;
typedef struct blip__Kont blip__Kont;
typedef struct blip__NoPayload blip__NoPayload;
typedef struct blip__Pair blip__Pair;
typedef struct blip__StringPayload blip__StringPayload;
typedef struct blip__StructVal blip__StructVal;
typedef struct blip__Table blip__Table;
typedef struct blip__Value blip__Value;
typedef struct blip__Vector blip__Vector;
typedef struct closure__Closure closure__Closure;
typedef struct closure__ClosureLifetimeFrame closure__ClosureLifetimeFrame;
typedef struct closure__ClosureLifetimeRecord closure__ClosureLifetimeRecord;
typedef struct closure__ClosureLifetimeState closure__ClosureLifetimeState;
typedef struct closure__ClosureLiveInfo closure__ClosureLiveInfo;
typedef struct closure__ClosureMutex closure__ClosureMutex;
typedef struct closure__ClosurePage closure__ClosurePage;
typedef struct dl__InterfaceExport dl__InterfaceExport;
typedef struct dl__InterfaceExportTable dl__InterfaceExportTable;
typedef struct host__CaptureHost host__CaptureHost;
typedef struct host__CaptureState_ host__CaptureState_;
typedef struct host__ConsoleHost host__ConsoleHost;
typedef struct machine__Bind machine__Bind;
typedef struct machine__Destructure machine__Destructure;
typedef struct machine__Destructured machine__Destructured;
typedef struct machine__Frame machine__Frame;
typedef struct machine__Group machine__Group;
typedef struct machine__Machine machine__Machine;
typedef struct machine__Matched machine__Matched;
typedef struct machine__ParamSpec machine__ParamSpec;
typedef struct map map;
typedef struct math__ChebSeries math__ChebSeries;
typedef struct os__ContextRecord os__ContextRecord;
typedef struct os__Eof os__Eof;
typedef struct os__ExceptionPointers os__ExceptionPointers;
typedef struct os__ExceptionRecord os__ExceptionRecord;
typedef struct os__ExecutableNotFoundError os__ExecutableNotFoundError;
typedef struct os__FileNotOpenedError os__FileNotOpenedError;
typedef struct os__NotExpected os__NotExpected;
typedef struct os__SizeOfTypeIs0Error os__SizeOfTypeIs0Error;
typedef struct os__WalkDirEntry os__WalkDirEntry;
typedef struct reader__Arena reader__Arena;
typedef struct reader__Datum reader__Datum;
typedef struct reader__Depth reader__Depth;
typedef struct reader__Diagnostic reader__Diagnostic;
typedef struct reader__Forms reader__Forms;
typedef struct reader__ReadResult reader__ReadResult;
typedef struct reader__Reader reader__Reader;
typedef struct strconv__AtoF64Param strconv__AtoF64Param;
typedef struct strconv__Dec32 strconv__Dec32;
typedef struct strconv__Dec64 strconv__Dec64;
typedef struct strconv__PrepNumber strconv__PrepNumber;
typedef struct string string;
typedef struct IError IError;
typedef struct blip__Payload blip__Payload;
typedef struct hash__Hash hash__Hash;
typedef struct hash__Hash32er hash__Hash32er;
typedef struct hash__Hash64er hash__Hash64er;
typedef struct host__Host host__Host;
typedef array Array;
struct string {
	u8* str;
	i64 len;
	i64 is_lit;
};

struct IError {
	void* _object;
	union {
		void* _interface_meta;
		struct {
			u32 _typ : 31;
			u32 _object_is_boxed : 1;
		};
	};
};

typedef struct __v_option { bool ok; int value; } __v_option;
typedef struct __v_result { bool ok; union { IError err; int value; }; } __v_result;

struct array {
	void* data;
	i64 offset;
	i64 len;
	i64 cap;
	int flags;
	i64 element_size;
};

struct map {
	VMapData* data;
};

struct blip__Payload {
	void* _object;
	union {
		void* _interface_meta;
		struct {
			u32 _typ : 31;
			u32 _object_is_boxed : 1;
		};
	};
};

struct hash__Hash {
	void* _object;
	union {
		void* _interface_meta;
		struct {
			u32 _typ : 31;
			u32 _object_is_boxed : 1;
		};
	};
};

struct hash__Hash32er {
	void* _object;
	union {
		void* _interface_meta;
		struct {
			u32 _typ : 31;
			u32 _object_is_boxed : 1;
		};
	};
};

struct hash__Hash64er {
	void* _object;
	union {
		void* _interface_meta;
		struct {
			u32 _typ : 31;
			u32 _object_is_boxed : 1;
		};
	};
};

struct host__Host {
	void* _object;
	union {
		void* _interface_meta;
		struct {
			u32 _typ : 31;
			u32 _object_is_boxed : 1;
		};
	};
};

struct AutostrAddrStackState {
	void* addrs[64];
	i64 types[64];
	i64 len;
	i64 overflow_depth;
};

typedef string Array_fixed_string_11[11];
typedef char Array_fixed_char_254[254];
typedef ptrdiff_t Array_fixed_ptrdiff_t_8[8];
typedef char Array_fixed_char_256[256];
struct ContextRecord {
	E_STRUCT_DECL;
};

struct DenseArray {
	i64 key_bytes;
	i64 value_bytes;
	i64 cap;
	i64 len;
	u32 deletes;
	u8* all_deleted;
	u8* keys;
	u8* values;
};

struct Error {
	E_STRUCT_DECL;
};

struct ExceptionPointers {
	ExceptionRecord* exception_record;
	ContextRecord* context_record;
};

struct ExceptionRecord {
	u32 code;
	u32 flags;
	ExceptionRecord* record;
	void* address;
	u32 param_count;
};

struct MessageError {
	string msg;
	i64 code;
};

struct None__ {
	Error Error;
};

struct OwnershipV3InterfacePayload {
	void* payload;
	i64 typ;
	bool is_boxed;
};

struct PanicDebugInfo {
	i64 line_no;
	string file;
	string mod;
	string fn_name;
};

struct PanicFrame {
	PanicFrame* prev;
	void* owner;
	_fn_ptr_5373f7edc7b60e26 jump;
};

struct PanicRecord {
	string msg;
	PanicDebugInfo debug;
	PanicFrame* frame;
	PanicFrame* resume;
	bool recovered;
	bool aborted;
};

struct PanicState {
	PanicFrame* top;
	PanicRecord* records;
	i64 len;
	i64 cap;
};

struct main__Suite {
	i64 fails;
};

struct VMapData {
	i64 key_bytes;
	i64 value_bytes;
	u32 even_index;
	u8 cached_hashbits;
	u8 shift;
	DenseArray key_values;
	u32* metas;
	u32 extra_metas;
	bool has_string_keys;
	_fn_ptr_1e8d64a3d95a0da1 hash_fn;
	_fn_ptr_c95f092653bee64 key_eq_fn;
	_fn_ptr_1a3da2026b1489ea clone_fn;
	_fn_ptr_5373f7edc7b60e26 free_fn;
	i64 count;
};

struct VMemoryBlock {
	u8* current;
	u8* stop;
	u8* start;
	VMemoryBlock* previous;
	VMemoryBlock* next;
	VPreallocScope* scope;
	VPreallocBlockCache* recycle_cache;
	ptrdiff_t min_block_size;
	bool is_scope;
	bool mmap_allocated;
	i64 id;
	i64 mallocs;
};

struct VPreallocBlockCache {
	i64 count;
	ptrdiff_t bytes;
	void* starts[8];
	ptrdiff_t sizes[8];
};

struct VPreallocRange {
	size_t start;
	size_t stop;
};

struct VPreallocScope {
	VMemoryBlock* previous;
	VMemoryBlock* first;
	VMemoryBlock* saved_next;
	size_t min_address;
	size_t max_address;
	VPreallocRange* ranges;
	i64 ranges_len;
	i64 ranges_cap;
	i32 refs;
	i32 free_requested;
	i32 abandoned;
	i32 finalized;
};

struct blip__Closure {
	Array params;
	i32 body;
	i32 env;
	string name;
	i64 arity;
	bool rest;
	string rest_name;
	i64 opt_from;
	Array opt_names;
	Array opt_defaults;
};

struct blip__Env {
	i32 parent;
	Array names;
	Array vals;
};

struct blip__EnvArena {
	Array frames;
};

struct blip__Kont {
	int tag;
	i32 expr;
	i32 env;
	i64 slot;
	i32 rest;
	Array acc;
	string name;
	Array clauses;
	Array binds;
	Array vals;
};

struct blip__NoPayload {
	E_STRUCT_DECL;
};

struct blip__StringPayload {
	int tag;
	string s;
};

struct blip__StructVal {
	string name;
	Array fields;
	map values;
};

struct blip__Table {
	int tag;
	map values;
};

struct blip__Value {
	int tag;
	i64 i;
	double f;
	blip__Payload payload;
};

struct blip__Vector {
	int tag;
	Array data;
};

struct closure__ClosureLifetimeFrame {
	i64 start;
	i64 end;
};

struct closure__ClosureLifetimeRecord {
	void* exec_ptr;
	u64 generation;
};

struct closure__ClosureLifetimeState {
	u64 owner_thread;
	bool active;
	bool disposed;
	i64 suspended;
	i64 frame_start;
	u64 frame_gen;
	u64 generation;
	u64 frame_generation;
	Array records;
	Array frames;
	closure__ClosureLifetimeState* next_free;
};

struct closure__ClosureLiveInfo {
	void* ctx;
	void* drop_data;
	u64 owns_data;
	u64 generation;
};

struct closure__ClosureMutex {
	SRWLOCK closure_mtx;
};

struct closure__ClosurePage {
	closure__ClosurePage* next;
	void* exec_page_start;
};

struct dl__InterfaceExport {
	char* iface;
	char* method;
	u32 typ;
	void* fn_ptr;
};

struct dl__InterfaceExportTable {
	dl__InterfaceExport* entries;
	i64 refs;
	dl__InterfaceExportTable* next;
};

struct host__CaptureHost {
	host__CaptureState_* state;
};

struct host__CaptureState_ {
	Array out;
	string raw;
	Array in;
	map files;
};

struct host__ConsoleHost {
	E_STRUCT_DECL;
};

struct machine__Bind {
	string name;
	blip__Value val;
};

struct machine__Destructure {
	i32 temp;
	Array parts;
};

struct machine__Destructured {
	i32 name;
	i32 accessor;
};

struct machine__Frame {
	i32 id;
	Array kids;
	i64 start;
};

struct machine__Group {
	Array params;
	Array args;
};

struct machine__Machine {
	reader__Arena* arena;
	Array kstack;
	i32 globals;
	blip__EnvArena envs;
	map prims;
	int ctl;
	i32 form;
	i32 env;
	blip__Value val;
	blip__Value val2;
	i64 steps;
	i64 max_steps;
	i64 max_kont;
	Array out;
	i64 gensym;
	host__Host host;
	string source;
	string src_text;
	Array loaded;
	Array macros;
};

struct machine__Matched {
	bool ok;
	Array binds;
};

struct machine__ParamSpec {
	Array names;
	string rest;
	bool is_rest;
	i64 opt_from;
	Array opt_names;
	Array opt_defaults;
	Array destructures;
};

struct math__ChebSeries {
	Array c;
	i64 order;
	double a;
	double b;
};

struct os__ContextRecord {
	E_STRUCT_DECL;
};

struct os__Eof {
	Error Error;
};

struct os__ExceptionPointers {
	os__ExceptionRecord* exception_record;
	os__ContextRecord* context_record;
};

struct os__ExceptionRecord {
	u32 code;
	u32 flags;
	os__ExceptionRecord* record;
	void* address;
	u32 param_count;
};

struct os__ExecutableNotFoundError {
	Error Error;
};

struct os__FileNotOpenedError {
	Error Error;
};

struct os__NotExpected {
	string cause;
	i64 code;
};

struct os__SizeOfTypeIs0Error {
	Error Error;
};

typedef struct __v_option_IError { bool ok; IError value; } __v_option_IError;

struct os__WalkDirEntry {
	string name;
	bool is_dir;
	int typ;
	__v_option_IError err;
};

struct reader__Arena {
	Array nodes;
	Array children;
};

struct reader__Datum {
	reader__DatumTag tag;
	string value;
	i64 i;
	double f;
	i64 den;
	i32 start;
	i32 count;
	i64 line;
	i64 col;
};

struct reader__Depth {
	i64 n;
	bool in_string;
	bool in_comment;
};

struct reader__Diagnostic {
	string msg;
	i64 line;
	i64 col;
	bool incomplete;
};

struct reader__Forms {
	Array forms;
	Array diags;
};

struct reader__ReadResult {
	reader__Arena arena;
	Array forms;
	Array diags;
};

struct reader__Reader {
	string src;
	reader__Arena* arena;
	i64 off;
	i64 line;
	i64 line_at;
	Array diags;
};

struct strconv__AtoF64Param {
	bool allow_extra_chars;
	bool allow_overflow;
};

struct strconv__Dec32 {
	u32 m;
	i64 e;
};

struct strconv__Dec64 {
	u64 m;
	i64 e;
};

struct strconv__PrepNumber {
	bool negative;
	i64 exponent;
	u64 mantissa;
};

struct blip__Pair {
	int tag;
	blip__Value car;
	blip__Value cdr;
};

struct closure__Closure {
	closure__ClosureMutex ClosureMutex;
	void* closure_ptr;
	_fn_ptr_4104d547b520960a closure_get_data;
	i64 closure_cap;
	void* free_closure_ptr;
	closure__ClosurePage* pages;
	i64 v_page_size;
	map live;
	map active_lifetimes;
	u64 next_generation;
	closure__ClosureLifetimeState* free_lifetime_states;
	u64 next_lifetime_generation;
	u64 lifetime_state_allocs;
};

struct multi_return_double_double {
	double arg0;
	double arg1;
};
struct multi_return_double_i64 {
	double arg0;
	i64 arg1;
};
struct multi_return_u32_u32 {
	u32 arg0;
	u32 arg1;
};
struct multi_return_string_string {
	string arg0;
	string arg1;
};
struct multi_return_i64_i64 {
	i64 arg0;
	i64 arg1;
};
struct multi_return_u32_i64 {
	u32 arg0;
	i64 arg1;
};
struct multi_return_size_t_size_t {
	size_t arg0;
	size_t arg1;
};
struct multi_return_size_t_size_t_size_t_size_t_size_t {
	size_t arg0;
	size_t arg1;
	size_t arg2;
	size_t arg3;
	size_t arg4;
};
struct multi_return_u64_i64 {
	u64 arg0;
	i64 arg1;
};
struct multi_return_i64_bool {
	i64 arg0;
	bool arg1;
};
struct multi_return_u32_bool_i64 {
	u32 arg0;
	bool arg1;
	i64 arg2;
};
struct multi_return_string_i64 {
	string arg0;
	i64 arg1;
};
struct multi_return_u32_u32_u32 {
	u32 arg0;
	u32 arg1;
	u32 arg2;
};
struct multi_return_int_strconv__PrepNumber {
	int arg0;
	strconv__PrepNumber arg1;
};
struct multi_return_strconv__Dec32_bool {
	strconv__Dec32 arg0;
	bool arg1;
};
struct multi_return_strconv__Dec64_bool {
	strconv__Dec64 arg0;
	bool arg1;
};
struct multi_return_string_string_string {
	string arg0;
	string arg1;
	string arg2;
};
struct multi_return_Array_Array {
	Array arg0;
	Array arg1;
};
struct multi_return_u64_u64 {
	u64 arg0;
	u64 arg1;
};
struct multi_return_i64_i64_i64 {
	i64 arg0;
	i64 arg1;
	i64 arg2;
};

typedef struct __v_option_blip__Value { bool ok; blip__Value value; } __v_option_blip__Value;
typedef struct __v_option_i32 { bool ok; i32 value; } __v_option_i32;
typedef struct __v_option_i64 { bool ok; i64 value; } __v_option_i64;
typedef struct __v_option_string { bool ok; string value; } __v_option_string;
typedef struct __v_result_Array { bool ok; union { IError err; Array value; }; } __v_result_Array;
typedef struct __v_result_blip__Value { bool ok; union { IError err; blip__Value value; }; } __v_result_blip__Value;
typedef struct __v_result_blip__Vectorptr { bool ok; union { IError err; blip__Vector* value; }; } __v_result_blip__Vectorptr;
typedef struct __v_result_bool { bool ok; union { IError err; bool value; }; } __v_result_bool;
typedef struct __v_result_double { bool ok; union { IError err; double value; }; } __v_result_double;
typedef struct __v_result_i32 { bool ok; union { IError err; i32 value; }; } __v_result_i32;
typedef struct __v_result_i64 { bool ok; union { IError err; i64 value; }; } __v_result_i64;
typedef struct __v_result_machine__Matched { bool ok; union { IError err; machine__Matched value; }; } __v_result_machine__Matched;
typedef struct __v_result_machine__ParamSpec { bool ok; union { IError err; machine__ParamSpec value; }; } __v_result_machine__ParamSpec;
typedef struct __v_result_string { bool ok; union { IError err; string value; }; } __v_result_string;
typedef struct __v_result_u32 { bool ok; union { IError err; u32 value; }; } __v_result_u32;
typedef struct __v_result_u64 { bool ok; union { IError err; u64 value; }; } __v_result_u64;

/* V3CACHE_SOURCE_DIRECTIVES_BEGIN */
/* V3CACHE_SOURCE_DIRECTIVES_END */
#ifndef PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP
#define pthread_rwlockattr_setkind_np(attr, kind) (0)
#endif
array __new_array(i64 len, i64 cap, i64 elem_size);
static inline array __v3_internal_symbol_array_new(i64 elem_size, i64 len, i64 cap) { if (len == 0 && cap == 0) return (array){.element_size = elem_size, .flags = ArrayFlags__managed}; return __new_array(len, cap, elem_size); }
#define array_new(elem_size, len, cap) __v3_internal_symbol_array_new((elem_size), (len), (cap))
#define array_push array__push
void array__push_many(array* a, void* val, i64 size);
#define array_push_many_ptr(a, val, size) array__push_many((a), (void*)(val), (size))
#define array_get array__get
#define array_set(a, i, ...) array__set(&(a), (i), __VA_ARGS__)
array array__clone(array* a);
#define array_slice array__slice
#define array_delete array__delete
#define array_ensure_cap array__ensure_cap
#define map__get_or_set map__get_and_set
#ifndef V_COMMIT_HASH
#define V_COMMIT_HASH ""
#endif
#ifndef memory_order_relaxed
#define memory_order_relaxed 0
#define memory_order_acquire 2
#define memory_order_release 3
#define memory_order_acq_rel 4
#define memory_order_seq_cst 5
#endif
#if defined(_WIN32) && (defined(__TINYC__) || (defined(_MSC_VER) && !defined(__clang__)))
/* V atomic.h supplies atomic_thread_fence on Windows TCC and MSVC. */
#elif defined(__TINYC__) && (defined(__i386__) || defined(__arm__) || defined(__aarch64__) || defined(__riscv) || (defined(__x86_64__) && defined(__linux__)))
extern void _V_atomic_thread_fence(int order);
#define atomic_thread_fence(order) _V_atomic_thread_fence(order)
#define __atomic_thread_fence(order) _V_atomic_thread_fence(order)
#else
#define atomic_thread_fence(order) __atomic_thread_fence(order)
#endif
__attribute__((weak)) void vheap_alloc(void* p, u64 n) { (void)p; (void)n; }
__attribute__((weak)) void vheap_free(void* p) { (void)p; }

#ifdef __TINYC__
extern byte __atomic_exchange_1(byte* ptr, byte val, int order);
extern u16 __atomic_exchange_2(u16* ptr, u16 val, int order);
extern u32 __atomic_exchange_4(u32* ptr, u32 val, int order);
extern u64 __atomic_exchange_8(u64* ptr, u64 val, int order);
extern byte __atomic_load_1(byte* ptr, int order);
extern u16 __atomic_load_2(u16* ptr, int order);
extern u32 __atomic_load_4(u32* ptr, int order);
extern u64 __atomic_load_8(u64* ptr, int order);
extern void __atomic_store_1(byte* ptr, byte val, int order);
extern void __atomic_store_2(u16* ptr, u16 val, int order);
extern void __atomic_store_4(u32* ptr, u32 val, int order);
extern void __atomic_store_8(u64* ptr, u64 val, int order);
extern _Bool __atomic_compare_exchange_1(byte* ptr, byte* expected, byte desired, int success_order, int failure_order);
extern _Bool __atomic_compare_exchange_2(u16* ptr, u16* expected, u16 desired, int success_order, int failure_order);
extern _Bool __atomic_compare_exchange_4(u32* ptr, u32* expected, u32 desired, int success_order, int failure_order);
extern _Bool __atomic_compare_exchange_8(u64* ptr, u64* expected, u64 desired, int success_order, int failure_order);
#endif
#if defined(_MSC_VER) && !defined(__clang__)






#else


#ifdef __TINYC__




#else




#endif
#endif
#if !(defined(_WIN32) && (defined(__TINYC__) || (defined(_MSC_VER) && !defined(__clang__))))










#ifdef __TINYC__




#if UINTPTR_MAX == 0xFFFFFFFF

#else

#endif








#if UINTPTR_MAX == 0xFFFFFFFF


#else


#endif




#if UINTPTR_MAX == 0xFFFFFFFF

#else

#endif




#else
























#endif

#ifdef __TINYC__

#else

#endif
#endif


static const u64 _wyp[4] = {0x2d358dccaa6c78a5ull, 0x8bb84b93962eacc9ull, 0x4b33a62ed433d4a3ull, 0x4d5a2da51de1aa47ull};
#if defined(__SIZEOF_INT128__) && !defined(__TINYC__)

#else

#endif

#define V_WY_LOAD8(p) ((u64)(p)[0] | ((u64)(p)[1] << 8) | ((u64)(p)[2] << 16) | ((u64)(p)[3] << 24) | ((u64)(p)[4] << 32) | ((u64)(p)[5] << 40) | ((u64)(p)[6] << 48) | ((u64)(p)[7] << 56))

#define v_signal_with_handler_cast(sig, handler) signal((sig), ((void (*)(int))(handler)))
string string__clone(string a);
void string__free(string* s);
string string__plus(string s, string a);
string int__str(i64 n);
string i64__str(i64 n);
string u64__str(u64 nn);
string f64__str(double x);
string rune__str(u32 c);
u8* malloc_noscan(ptrdiff_t n);
void* memdup(void* src, ptrdiff_t sz);

static int v3_array_sort_int_cmp(const void* a, const void* b) { i64 av = *(const i64*)a; i64 bv = *(const i64*)b; return (av > bv) - (av < bv); }

static int v3_array_sort_i8_cmp(const void* a, const void* b) { signed char av = *(const signed char*)a; signed char bv = *(const signed char*)b; return (av > bv) - (av < bv); }

static int v3_array_sort_i16_cmp(const void* a, const void* b) { short av = *(const short*)a; short bv = *(const short*)b; return (av > bv) - (av < bv); }

static int v3_array_sort_i64_cmp(const void* a, const void* b) { long long av = *(const long long*)a; long long bv = *(const long long*)b; return (av > bv) - (av < bv); }

static int v3_array_sort_u8_cmp(const void* a, const void* b) { unsigned char av = *(const unsigned char*)a; unsigned char bv = *(const unsigned char*)b; return (av > bv) - (av < bv); }

static int v3_array_sort_u16_cmp(const void* a, const void* b) { unsigned short av = *(const unsigned short*)a; unsigned short bv = *(const unsigned short*)b; return (av > bv) - (av < bv); }

static int v3_array_sort_u32_cmp(const void* a, const void* b) { unsigned av = *(const unsigned*)a; unsigned bv = *(const unsigned*)b; return (av > bv) - (av < bv); }

static int v3_array_sort_u64_cmp(const void* a, const void* b) { unsigned long long av = *(const unsigned long long*)a; unsigned long long bv = *(const unsigned long long*)b; return (av > bv) - (av < bv); }

static int v3_array_sort_isize_cmp(const void* a, const void* b) { ptrdiff_t av = *(const ptrdiff_t*)a; ptrdiff_t bv = *(const ptrdiff_t*)b; return (av > bv) - (av < bv); }

static int v3_array_sort_usize_cmp(const void* a, const void* b) { size_t av = *(const size_t*)a; size_t bv = *(const size_t*)b; return (av > bv) - (av < bv); }

static int v3_array_sort_f32_cmp(const void* a, const void* b) { float av = *(const float*)a; float bv = *(const float*)b; return (av > bv) - (av < bv); }

static int v3_array_sort_f64_cmp(const void* a, const void* b) { double av = *(const double*)a; double bv = *(const double*)b; return (av > bv) - (av < bv); }

static int v3_array_sort_rune_cmp(const void* a, const void* b) { unsigned av = *(const unsigned*)a; unsigned bv = *(const unsigned*)b; return (av > bv) - (av < bv); }

static int v3_array_sort_char_cmp(const void* a, const void* b) { char av = *(const char*)a; char bv = *(const char*)b; return (av > bv) - (av < bv); }

#ifndef V_MANUAL_STDLIB_HEADERS
#ifdef _WIN32
void* _aligned_malloc(size_t size, size_t alignment);
void _aligned_free(void* memblock);
#else
int posix_memalign(void** memptr, size_t alignment, size_t size);
#endif
#endif
void* memdup_align(void* src, ptrdiff_t sz, ptrdiff_t alignment);
void v_free(void* p);


static inline string v3_c_lit(const char* s, int len) { return (string){.str = (u8*)s, .len = len, .is_lit = 1}; }








static inline string v3_f64_fixed(double x, int precision) {
	if (precision < 0) precision = 6;
	if (!isfinite(x)) return f64__str(x);
	int clamped_precision = precision > 35 ? 35 : precision;
	double rounder = 0.5 * pow(10.0, -clamped_precision);
	string decimal = f64__str(fabs(x) + rounder);
	u8 digits[32];
	int digit_count = 0;
	int decimal_pos = -1;
	int exponent = 0;
	int exponent_sign = 1;
	for (int i = 0; i < decimal.len; ++i) {
		u8 c = decimal.str[i];
		if (c >= '0' && c <= '9') {
			digits[digit_count++] = c;
		} else if (c == '.') {
			decimal_pos = digit_count;
		} else if (c == 'e' || c == 'E') {
			++i;
			if (i < decimal.len && decimal.str[i] == '-') {
				exponent_sign = -1;
				++i;
			} else if (i < decimal.len && decimal.str[i] == '+') {
				++i;
			}
			for (; i < decimal.len; ++i) exponent = exponent * 10 + decimal.str[i] - '0';
			break;
		}
	}
	if (decimal_pos < 0) decimal_pos = digit_count;
	decimal_pos += exponent_sign * exponent;
	int whole_digits = decimal_pos > 0 ? decimal_pos : 1;
	int negative = signbit(x) != 0;
	int out_len = negative + whole_digits + (precision > 0 ? precision + 1 : 0);
	u8* out = malloc_noscan((ptrdiff_t)out_len + 1);
	int pos = 0;
	if (negative) out[pos++] = '-';
	if (decimal_pos <= 0) out[pos++] = '0';
	else for (int i = 0; i < whole_digits; ++i) out[pos++] = i < digit_count ? digits[i] : '0';
	if (precision > 0) {
		out[pos++] = '.';
		for (int i = 0; i < precision; ++i) {
			int digit = decimal_pos + i;
			out[pos++] = digit >= 0 && digit < digit_count ? digits[digit] : '0';
		}
	}
	out[pos] = 0;
	return (string){.str = out, .len = out_len, .is_lit = 0};
}





























void* map__get(map* m, void* key, void* zero);
bool map__exists(map* m, void* key);






#ifndef max_int
#define max_int max_i32
#endif
#ifndef min_int
#define min_int min_i32
#endif

static void v3_eprint_lit(const char* s) {
	fprintf(stderr, "%s", s);
}
static void v3_eprintln_string(string s) {
	fprintf(stderr, "%.*s\n", s.len, (char*)s.str);
}

i64 g_autostr_type_stack[64] = {0};
i64 g_autostr_type_stack_len = 0;
#if defined(__TINYC__) && defined(_WIN32)
typedef DWORD (WINAPI *g_autostr_addr_state_fls_alloc_fn)(void (WINAPI *)(void*));
typedef void* (WINAPI *g_autostr_addr_state_fls_get_fn)(DWORD);
typedef BOOL (WINAPI *g_autostr_addr_state_fls_set_fn)(DWORD, void*);
static DWORD g_autostr_addr_state_key = 0xFFFFFFFF;
static g_autostr_addr_state_fls_get_fn g_autostr_addr_state_fls_get;
static g_autostr_addr_state_fls_set_fn g_autostr_addr_state_fls_set;
static void WINAPI g_autostr_addr_state_slot_free(void* p) { free(p); }
static unsigned int g_autostr_addr_state_key_claim;
static unsigned int g_autostr_addr_state_key_ready;
#if defined(__x86_64__) || defined(__i386__)
#define g_autostr_addr_state_key_is_ready() (*(volatile unsigned int*)&g_autostr_addr_state_key_ready)
#else
#define g_autostr_addr_state_key_is_ready() __atomic_add_fetch(&g_autostr_addr_state_key_ready, 0, 5)
#endif
static void g_autostr_addr_state_key_init(void) {
	if (g_autostr_addr_state_key_is_ready()) { return; }
	if (__atomic_add_fetch(&g_autostr_addr_state_key_claim, 1, 5) == 1) {
		void* kernel32 = GetModuleHandleA("kernel32.dll");
		g_autostr_addr_state_fls_alloc_fn fls_alloc = (g_autostr_addr_state_fls_alloc_fn)GetProcAddress(kernel32, "FlsAlloc");
		g_autostr_addr_state_fls_get = (g_autostr_addr_state_fls_get_fn)GetProcAddress(kernel32, "FlsGetValue");
		g_autostr_addr_state_fls_set = (g_autostr_addr_state_fls_set_fn)GetProcAddress(kernel32, "FlsSetValue");
		g_autostr_addr_state_key = fls_alloc && g_autostr_addr_state_fls_get && g_autostr_addr_state_fls_set ? fls_alloc(g_autostr_addr_state_slot_free) : TlsAlloc();
		__atomic_add_fetch(&g_autostr_addr_state_key_ready, 1, 5);
	} else {
		while (!g_autostr_addr_state_key_is_ready()) { Sleep(0); }
	}
}
#undef g_autostr_addr_state_key_is_ready
static AutostrAddrStackState* g_autostr_addr_state_slot(void) { g_autostr_addr_state_key_init(); void* p = g_autostr_addr_state_fls_get ? g_autostr_addr_state_fls_get(g_autostr_addr_state_key) : TlsGetValue(g_autostr_addr_state_key); if (!p) { p = calloc(1, sizeof(AutostrAddrStackState)); if (g_autostr_addr_state_fls_set) g_autostr_addr_state_fls_set(g_autostr_addr_state_key, p); else TlsSetValue(g_autostr_addr_state_key, p); } return (AutostrAddrStackState*)p; }
#define g_autostr_addr_state (*g_autostr_addr_state_slot())
#elif defined(__TINYC__)
static pthread_key_t g_autostr_addr_state_key;
static pthread_once_t g_autostr_addr_state_key_once = PTHREAD_ONCE_INIT;
static void g_autostr_addr_state_key_create(void) { pthread_key_create(&g_autostr_addr_state_key, free); }
static void g_autostr_addr_state_key_init(void) { pthread_once(&g_autostr_addr_state_key_once, g_autostr_addr_state_key_create); }
static AutostrAddrStackState* g_autostr_addr_state_slot(void) { g_autostr_addr_state_key_init(); void* p = pthread_getspecific(g_autostr_addr_state_key); if (!p) { p = calloc(1, sizeof(AutostrAddrStackState)); pthread_setspecific(g_autostr_addr_state_key, p); } return (AutostrAddrStackState*)p; }
#define g_autostr_addr_state (*g_autostr_addr_state_slot())
#elif defined(_MSC_VER)
__declspec(thread) AutostrAddrStackState g_autostr_addr_state = {0};
#elif defined(__cplusplus)
thread_local AutostrAddrStackState g_autostr_addr_state = {0};
#else
_Thread_local AutostrAddrStackState g_autostr_addr_state = {0};
#endif
Array as_cast_type_indexes = {0};
VMapData map_empty_data = {0};
bool v_memory_panic = false;
i64 total_m = ((i64)(0));
i64 g_main_argc = ((i64)(0));
void* g_main_argv = NULL;
void* g_live_reload_info;
void* gc_boehm_default_abort_func;
VMemoryBlock* g_memory_block;
i64 g_prealloc_allocation_count;
i64 g_prealloc_allocated_bytes;
#if defined(__TINYC__) && defined(_WIN32)
typedef DWORD (WINAPI *g_panic_state_fls_alloc_fn)(void (WINAPI *)(void*));
typedef void* (WINAPI *g_panic_state_fls_get_fn)(DWORD);
typedef BOOL (WINAPI *g_panic_state_fls_set_fn)(DWORD, void*);
static DWORD g_panic_state_key = 0xFFFFFFFF;
static g_panic_state_fls_get_fn g_panic_state_fls_get;
static g_panic_state_fls_set_fn g_panic_state_fls_set;
static void WINAPI g_panic_state_slot_free(void* p) { free(p); }
static unsigned int g_panic_state_key_claim;
static unsigned int g_panic_state_key_ready;
#if defined(__x86_64__) || defined(__i386__)
#define g_panic_state_key_is_ready() (*(volatile unsigned int*)&g_panic_state_key_ready)
#else
#define g_panic_state_key_is_ready() __atomic_add_fetch(&g_panic_state_key_ready, 0, 5)
#endif
static void g_panic_state_key_init(void) {
	if (g_panic_state_key_is_ready()) { return; }
	if (__atomic_add_fetch(&g_panic_state_key_claim, 1, 5) == 1) {
		void* kernel32 = GetModuleHandleA("kernel32.dll");
		g_panic_state_fls_alloc_fn fls_alloc = (g_panic_state_fls_alloc_fn)GetProcAddress(kernel32, "FlsAlloc");
		g_panic_state_fls_get = (g_panic_state_fls_get_fn)GetProcAddress(kernel32, "FlsGetValue");
		g_panic_state_fls_set = (g_panic_state_fls_set_fn)GetProcAddress(kernel32, "FlsSetValue");
		g_panic_state_key = fls_alloc && g_panic_state_fls_get && g_panic_state_fls_set ? fls_alloc(g_panic_state_slot_free) : TlsAlloc();
		__atomic_add_fetch(&g_panic_state_key_ready, 1, 5);
	} else {
		while (!g_panic_state_key_is_ready()) { Sleep(0); }
	}
}
#undef g_panic_state_key_is_ready
static PanicState* g_panic_state_slot(void) { g_panic_state_key_init(); void* p = g_panic_state_fls_get ? g_panic_state_fls_get(g_panic_state_key) : TlsGetValue(g_panic_state_key); if (!p) { p = calloc(1, sizeof(PanicState)); if (g_panic_state_fls_set) g_panic_state_fls_set(g_panic_state_key, p); else TlsSetValue(g_panic_state_key, p); } return (PanicState*)p; }
#define g_panic_state (*g_panic_state_slot())
#elif defined(__TINYC__)
static pthread_key_t g_panic_state_key;
static pthread_once_t g_panic_state_key_once = PTHREAD_ONCE_INIT;
static void g_panic_state_key_create(void) { pthread_key_create(&g_panic_state_key, free); }
static void g_panic_state_key_init(void) { pthread_once(&g_panic_state_key_once, g_panic_state_key_create); }
static PanicState* g_panic_state_slot(void) { g_panic_state_key_init(); void* p = pthread_getspecific(g_panic_state_key); if (!p) { p = calloc(1, sizeof(PanicState)); pthread_setspecific(g_panic_state_key, p); } return (PanicState*)p; }
#define g_panic_state (*g_panic_state_slot())
#elif defined(_MSC_VER)
__declspec(thread) PanicState g_panic_state = {0};
#elif defined(__cplusplus)
thread_local PanicState g_panic_state = {0};
#else
_Thread_local PanicState g_panic_state = {0};
#endif
u64 os__g_main_thread_id;
closure__Closure closure__g_closure;
SRWLOCK closure__g_closure_once_lock = SRWLOCK_INIT;
bool closure__g_closure_once_done = false;
dl__InterfaceExportTable* dl__g_dl_interface_exports;

void __v3_cache_bits__init_consts(void);
void __v3_cache_bits__init_consts_defaults(void);
void __v3_cache_blip__init_consts(void);
void __v3_cache_blip__init_consts_defaults(void);
void __v3_cache_builtin__init_consts(void);
void __v3_cache_builtin__init_consts_defaults(void);
void __v3_cache_closure__init_consts(void);
void __v3_cache_closure__init_consts_defaults(void);
void __v3_cache_dl__init_consts(void);
void __v3_cache_dl__init_consts_defaults(void);
void __v3_cache_hash__init_consts(void);
void __v3_cache_hash__init_consts_defaults(void);
void __v3_cache_host__init_consts(void);
void __v3_cache_host__init_consts_defaults(void);
void __v3_cache_internal__init_consts(void);
void __v3_cache_internal__init_consts_defaults(void);
void __v3_cache_machine__init_consts(void);
void __v3_cache_machine__init_consts_defaults(void);
void __v3_cache_math__init_consts(void);
void __v3_cache_math__init_consts_defaults(void);
void __v3_cache_os__init_consts(void);
void __v3_cache_os__init_consts_defaults(void);
void __v3_cache_prims__init_consts(void);
void __v3_cache_prims__init_consts_defaults(void);
void __v3_cache_printer__init_consts(void);
void __v3_cache_printer__init_consts_defaults(void);
void __v3_cache_reader__init_consts(void);
void __v3_cache_reader__init_consts_defaults(void);
void __v3_cache_strconv__init_consts(void);
void __v3_cache_strconv__init_consts_defaults(void);
void __v3_cache_strings__init_consts(void);
void __v3_cache_strings__init_consts_defaults(void);
void __v3_cache_textscanner__init_consts(void);
void __v3_cache_textscanner__init_consts_defaults(void);
void __v3_cache_validate__init_consts(void);
void __v3_cache_validate__init_consts_defaults(void);
/* V3CACHE_LATE_DIRECTIVES_BEGIN */
/* V3CACHE_LATE_DIRECTIVES_END */
string Array_rune__string(Array ra);
void Array_string__free(Array* a);
string Array_string__join(Array a, string sep);
string Array_string__join_lines(Array s);
void Array_string__sort_by_len(Array* s);
void Array_string__sort_ignore_case(Array* s);
string Array_string__str(Array a);
__v_result_u32 Array_u8__byterune(Array b);
string Array_u8__bytestr(Array b);
string Array_u8__hex(Array b);
__v_result_u32 Array_u8__utf8_to_utf32(Array _bytes);
DenseArray DenseArray__clone(DenseArray* d);
i64 Error__code(Error err);
string Error__msg(Error err);
string IError__str(IError err);
i64 MessageError__code(MessageError err);
string MessageError__msg(MessageError err);
string MessageError__str(MessageError err);
string None____str(None__ _0);
string StrIntpType_str(int x);
void Suite__check_example(main__Suite* s, string label, string path, string want);
void Suite__fail(main__Suite* s, string label, string msg);
VMapData* VMapData__clone(VMapData* m);
void* __as_cast(void* obj, i64 obj_type, i64 expected_type, string obj_name, string expected_name);
array __new_array(i64 mylen, i64 cap, i64 elm_size);
array __new_array_noscan(i64 mylen, i64 cap, i64 elm_size);
array array__clone(array* a);
void array__delete(array* a, i64 i);
void array__delete_last(array* a);
void array__ensure_cap(array* a, i64 required);
void array__free(array* a);
void* array__get(array a, i64 i);
void array__insert(array* a, i64 i, void* val);
void array__insert_many(array* a, i64 i, void* val, i64 size);
void* array__pop_left(array* a);
void array__prepend(array* a, void* val);
void array__push(array* a, void* val);
void array__push_many(array* a, void* val, i64 size);
array array__reverse(array a);
void array__set(array* a, i64 i, void* val);
array array__slice(array a, i64 start, i64 _end);
array array__slice_ni(array a, i64 _start, i64 _end);
void array__sort_with_compare(array* a, _fn_ptr_ed95774d3a4e97ab callback);
void array_sort_move(void* dst, i64 di, void* src, i64 si, i64 count, size_t element_size);
void autostr_addr_pop(void);
bool autostr_addr_type_in_stack(void* addr, i64 typ);
void autostr_addr_type_push(void* addr, i64 typ);
int blip__Closure__payload_tag(blip__Closure* c);
void blip__EnvArena__define(blip__EnvArena* ea, i32 id, string name, blip__Value val);
blip__Env* blip__EnvArena__get(blip__EnvArena* ea, i32 id);
__v_option_blip__Value blip__EnvArena__lookup(blip__EnvArena* ea, i32 id, string name);
i32 blip__EnvArena__new_env(blip__EnvArena* ea, i32 parent);
bool blip__EnvArena__set(blip__EnvArena* ea, i32 id, string name, blip__Value val);
int blip__NoPayload__payload_tag(blip__NoPayload* n);
int blip__Pair__payload_tag(blip__Pair* p);
int blip__StringPayload__payload_tag(blip__StringPayload* p);
blip__Value blip__StructVal__at(blip__StructVal* s, i64 i);
blip__Value blip__StructVal__get(blip__StructVal* s, string k);
bool blip__StructVal__has(blip__StructVal* s, string k);
int blip__StructVal__payload_tag(blip__StructVal* s);
void blip__StructVal__set(blip__StructVal* s, string k, blip__Value v);
blip__StructVal* blip__StructVal__with(blip__StructVal* s, string k, blip__Value v);
void blip__Table__delete_at(blip__Table* t, string k);
blip__Value blip__Table__get(blip__Table* t, string k);
bool blip__Table__has(blip__Table* t, string k);
Array blip__Table__keys(blip__Table* t);
int blip__Table__payload_tag(blip__Table* t);
void blip__Table__set_at(blip__Table* t, string k, blip__Value val);
bool blip__Value__as_bool(blip__Value v);
blip__Closure* blip__Value__as_closure(blip__Value v);
double blip__Value__as_float(blip__Value v);
i64 blip__Value__as_int(blip__Value v);
blip__Pair* blip__Value__as_pair(blip__Value v);
string blip__Value__as_string(blip__Value v);
blip__StructVal* blip__Value__as_struct(blip__Value v);
blip__Table* blip__Value__as_table(blip__Value v);
blip__Vector* blip__Value__as_vector(blip__Value v);
bool blip__Value__is_empty_seq(blip__Value v);
bool blip__Value__is_nil(blip__Value v);
bool blip__Value__truthy(blip__Value v);
int blip__Vector__payload_tag(blip__Vector* v);
blip__Value blip__Vector__pop(blip__Vector* v);
void blip__Vector__push(blip__Vector* v, blip__Value val);
void blip__Vector__set_at(blip__Vector* v, i64 i, blip__Value val);
blip__Value blip__boolean(bool b);
blip__Value blip__buffer(map m);
blip__Value blip__cons(blip__Value car, blip__Value cdr);
blip__Value blip__empty_list(void);
blip__Value blip__float(double f);
string blip__format_error(string path, string src, i64 line, i64 col, string msg, i64 length);
blip__Value blip__integer(i64 n);
blip__Value blip__keyword(string name);
blip__Value blip__labelled(Array params, i32 body, i32 env, string name, i64 opt_from, Array opt_names, Array opt_defaults);
blip__Value blip__list_from(Array items);
blip__Value blip__new_closure(Array params, i32 body, i32 env, string name);
blip__Value blip__new_prim(string name);
blip__Value blip__new_rest_closure(Array params, string rest_name, i32 body, i32 env, string name);
blip__Value blip__nil_value(void);
blip__Value blip__rune(u32 r);
blip__Value blip__string(string s);
blip__Value blip__struct_value(string name, Array fields, map vals);
blip__Value blip__symbol(string name);
blip__Value blip__table(map m);
blip__Value blip__vector(Array items);
string bool__str(bool b);
void builtin_init(void);
string byteptr__str(u8* nn);
Array byteptr__vbytes(u8* data, i64 len);
string byteptr__vstring(u8* bp);
string byteptr__vstring_with_len(u8* bp, i64 len);
string char__str(char* cptr);
string charptr__str(char* nn);
string charptr__vstring(char* cp);
string charptr__vstring_with_len(char* cp, i64 len);
void* closure__closure_create_with_data(void* func, void* data, bool owns_data);
void closure__closure_init(void);
void closure__closure_try_destroy(void* closure);
i64 compare_lower_strings(string* a, string* b);
i64 compare_strings_by_len(string* a, string* b);
string data_to_hex_string(u8* data, i64 len);
void* dl__interface_export_find(char* iface, char* method, u32 typ);
void drop_owned_interface_T_IError(IError value);
void drop_owned_result_error(IError err);
void drop_owned_result_error_interface(IError err);
void drop_owned_v3_interface_T_IError(IError value);
IError error(string message);
IError error_with_code(string message, i64 code);
string f32__str(float x);
string f64__str(double x);
string float_literal__str(float_literal d);
bool gc_prepare_for_debugger_init(void);
void gc_restore_roots_after_debugger_init(bool use_manual_roots);
void gc_runtime_init(void);
void host__CaptureHost__give_file(host__CaptureHost* h, string path, string src);
void host__CaptureHost__give_input(host__CaptureHost* h, Array lines);
__v_result_string host__CaptureHost__host_load(host__CaptureHost* h, string path);
void host__CaptureHost__host_print(host__CaptureHost* h, string s);
__v_option_string host__CaptureHost__host_read_line(host__CaptureHost* h);
void host__CaptureHost__host_write(host__CaptureHost* h, string s);
Array host__CaptureHost__lines(host__CaptureHost* h);
string host__CaptureHost__transcript(host__CaptureHost* h);
__v_result_string host__ConsoleHost__host_load(host__ConsoleHost* c, string path);
void host__ConsoleHost__host_print(host__ConsoleHost* c, string s);
__v_option_string host__ConsoleHost__host_read_line(host__ConsoleHost* c);
void host__ConsoleHost__host_write(host__ConsoleHost* c, string s);
host__CaptureHost* host__new_capture(void);
string i128__str(i128 nn);
string i16__str(i16 n);
string i32__str(i32 n);
string i64__str(i64 nn);
string i8__str(i8 n);
u32 impl_utf8_to_utf32(u8* _bytes, i64 _bytes_len);
string int__str(i64 n);
string int_literal__str(int_literal n);
string isize__str(ptrdiff_t x);
bool machine__Machine__already_loaded(machine__Machine* m, string path);
__v_result machine__Machine__any_every(machine__Machine* m, string name, Array args);
__v_result machine__Machine__apply_all(machine__Machine* m, Array all_in);
i32 machine__Machine__apply_destructures(machine__Machine* m, machine__ParamSpec ps, i32 body);
blip__Value machine__Machine__assemble(machine__Machine* m, i32 id, Array kids, map built);
i32 machine__Machine__binding_name(machine__Machine* m, i32 b);
i32 machine__Machine__binding_pattern(machine__Machine* m, i32 b);
i32 machine__Machine__binding_value(machine__Machine* m, i32 b);
__v_result_blip__Value machine__Machine__build_collection(machine__Machine* m, i32 id, Array items);
__v_result machine__Machine__call_closure(machine__Machine* m, blip__Closure* c, Array args);
i32 machine__Machine__call_node(machine__Machine* m, i32 callee, Array args);
__v_result machine__Machine__call_primitive(machine__Machine* m, string name, Array args);
__v_result_blip__Value machine__Machine__call_value(machine__Machine* m, blip__Value f, Array args);
string machine__Machine__callee_name(machine__Machine* m, i32 call);
blip__Value machine__Machine__closure_from_form(machine__Machine* m, i32 n);
i32 machine__Machine__cond_from(machine__Machine* m, Array items);
blip__Value machine__Machine__datum_to_value(machine__Machine* m, i32 id);
string machine__Machine__decorate_error(machine__Machine* m, string msg);
__v_result_Array machine__Machine__destructuring(machine__Machine* m, i32 pattern, i32 value);
__v_result_blip__Value machine__Machine__drive(machine__Machine* m);
blip__Value machine__Machine__empty_collection(machine__Machine* m, i32 id);
__v_result machine__Machine__enter_clause_body(machine__Machine* m, i32 frame, i32 body);
__v_result machine__Machine__eval_collection(machine__Machine* m, i32 id);
__v_result_blip__Value machine__Machine__eval_default(machine__Machine* m, i32 n, Array args);
__v_result machine__Machine__eval_define(machine__Machine* m, Array kids);
__v_result machine__Machine__eval_defmacro(machine__Machine* m, Array kids);
__v_result machine__Machine__eval_field_access(machine__Machine* m, string what);
__v_result machine__Machine__eval_field_update(machine__Machine* m, string what, i32 value_node);
__v_result machine__Machine__eval_import(machine__Machine* m, Array kids);
__v_result machine__Machine__eval_lambda(machine__Machine* m, Array kids);
__v_result machine__Machine__eval_list(machine__Machine* m, i32 id, Array kids);
__v_result machine__Machine__eval_match(machine__Machine* m, Array kids);
__v_result_blip__Value machine__Machine__eval_one(machine__Machine* m, i32 f);
__v_result machine__Machine__eval_pipe(machine__Machine* m, string name, Array kids);
__v_result machine__Machine__eval_require(machine__Machine* m, Array kids);
__v_result machine__Machine__eval_special(machine__Machine* m, string name, i32 id, Array kids);
__v_result_blip__Value machine__Machine__eval_string_lenient(machine__Machine* m, string src);
__v_result machine__Machine__eval_struct(machine__Machine* m, Array kids);
__v_result machine__Machine__eval_symbol(machine__Machine* m, string name);
__v_result machine__Machine__eval_try(machine__Machine* m, Array kids);
__v_result machine__Machine__eval_use(machine__Machine* m, Array kids);
__v_result_blip__Value machine__Machine__expand_macro_value(machine__Machine* m, blip__Value v);
__v_result_blip__Value machine__Machine__field_of(machine__Machine* m, blip__Value v, string field);
__v_result_Array machine__Machine__fill_labels(machine__Machine* m, blip__Closure* c, Array args, i32 caller);
__v_result machine__Machine__fold(machine__Machine* m, string name, Array args);
string machine__Machine__fresh(machine__Machine* m, string base);
void machine__Machine__goto(machine__Machine* m, i32 form);
__v_result machine__Machine__hof(machine__Machine* m, string name, Array args);
i32 machine__Machine__int_node(machine__Machine* m, i64 n);
bool machine__Machine__is_binding_form(machine__Machine* m, i32 b);
bool machine__Machine__is_call_form(machine__Machine* m, i32 n);
bool machine__Machine__is_capturing_tail(machine__Machine* m, i32 n);
bool machine__Machine__is_destructuring(machine__Machine* m, i32 b);
bool machine__Machine__is_macro_closure(machine__Machine* m, blip__Value v);
bool machine__Machine__is_macro_form(machine__Machine* m, blip__Value v);
bool machine__Machine__is_pipe_head(machine__Machine* m, i32 n);
bool machine__Machine__is_update_marker(machine__Machine* m, i32 n);
bool machine__Machine__is_wildcard(machine__Machine* m, i32 n);
blip__Kont machine__Machine__kont(machine__Machine* m, int tag);
i32 machine__Machine__kont_caller(machine__Machine* m);
i32 machine__Machine__list_of(machine__Machine* m, Array items);
i32 machine__Machine__list_of2(machine__Machine* m, Array names, Array vals);
__v_result_blip__Value machine__Machine__load(machine__Machine* m, string path);
__v_result_blip__Value machine__Machine__lookup_key(machine__Machine* m, string key, blip__Value coll);
__v_result_blip__Value machine__Machine__macex(machine__Machine* m, i32 n);
__v_result_blip__Value machine__Machine__macex1(machine__Machine* m, i32 n);
i32 machine__Machine__make_begin(machine__Machine* m, Array forms);
blip__Value machine__Machine__make_closure(machine__Machine* m, machine__ParamSpec spec, i32 body, i32 env, string name);
__v_result_machine__Matched machine__Machine__match_at(machine__Machine* m, i32 pat, blip__Value subject, i64 depth);
i32 machine__Machine__match_clause(machine__Machine* m, i32 subject, i32 pattern);
__v_result_machine__Matched machine__Machine__match_list_at(machine__Machine* m, i32 pat, blip__Value subject, i64 depth);
__v_result_machine__Matched machine__Machine__match_list_tail(machine__Machine* m, Array kids, blip__Value subject, i64 depth);
__v_result_machine__Matched machine__Machine__match_pattern(machine__Machine* m, i32 pat, blip__Value subject);
__v_result_machine__Matched machine__Machine__match_struct_at(machine__Machine* m, i32 pat, blip__Value subject, i64 depth);
__v_result_machine__Matched machine__Machine__match_table_at(machine__Machine* m, i32 pat, blip__Value subject, i64 depth);
i32 machine__Machine__nest_lets(machine__Machine* m, Array groups, Array body);
i32 machine__Machine__nil_node(machine__Machine* m);
i32 machine__Machine__node_of(machine__Machine* m, string head, Array tail);
__v_result_machine__ParamSpec machine__Machine__parse_params(machine__Machine* m, Array plist, string who);
string machine__Machine__pattern_head(machine__Machine* m, i32 p);
i32 machine__Machine__pipe_one(machine__Machine* m, i32 acc, i32 step, string mode);
i32 machine__Machine__pipe_through(machine__Machine* m, i32 acc, i32 step, bool last);
blip__Kont machine__Machine__pop(machine__Machine* m);
__v_result_machine__Matched machine__Machine__positional(machine__Machine* m, Array kids, blip__Value subject, i64 depth);
void machine__Machine__push(machine__Machine* m, blip__Kont k);
blip__Value machine__Machine__quasi_to_value(machine__Machine* m, i32 id);
string machine__Machine__render_node(machine__Machine* m, i32 n);
Array machine__Machine__require_paths(machine__Machine* m, i32 n);
void machine__Machine__reset(machine__Machine* m);
__v_option_string machine__Machine__resolve_module(machine__Machine* m, string name);
string machine__Machine__resolve_path(machine__Machine* m, string path);
__v_result machine__Machine__result_combinator(machine__Machine* m, string name, Array args);
void machine__Machine__ret(machine__Machine* m);
__v_result_blip__Value machine__Machine__run(machine__Machine* m, Array forms);
__v_result_blip__Value machine__Machine__run_checked(machine__Machine* m, reader__Forms res);
__v_result_blip__Value machine__Machine__run_str(machine__Machine* m, string src);
__v_result machine__Machine__sort_by(machine__Machine* m, Array args);
__v_result machine__Machine__step_eval(machine__Machine* m);
__v_result_bool machine__Machine__step_return(machine__Machine* m);
i32 machine__Machine__string_node(machine__Machine* m, string s);
i32 machine__Machine__sym_node(machine__Machine* m, string name);
i32 machine__Machine__transform_case(machine__Machine* m, Array kids);
i32 machine__Machine__transform_cond(machine__Machine* m, Array kids);
__v_result_i32 machine__Machine__transform_dotimes(machine__Machine* m, Array kids);
__v_result_i32 machine__Machine__transform_let(machine__Machine* m, Array kids);
__v_result_i32 machine__Machine__transform_let_assert(machine__Machine* m, Array kids);
i32 machine__Machine__transform_let_star(machine__Machine* m, Array kids);
i32 machine__Machine__transform_let_star_at(machine__Machine* m, Array kids, i64 binds_at);
i32 machine__Machine__transform_letrec(machine__Machine* m, Array kids);
i32 machine__Machine__transform_loop(machine__Machine* m, Array kids);
i32 machine__Machine__transform_match_star(machine__Machine* m, Array kids);
__v_result machine__Machine__try_caught(machine__Machine* m, blip__Kont k);
__v_result machine__Machine__try_clauses(machine__Machine* m, blip__Kont k, blip__Value subject);
__v_result_bool machine__Machine__try_finish(machine__Machine* m, blip__Kont k, bool run_finally);
string machine__Machine__use_name(machine__Machine* m, i64 i);
string machine__Machine__where(machine__Machine* m);
blip__Value machine__datum_literal(reader__Datum* d);
string machine__format_args(Array args);
blip__Value machine__inner_value(blip__Value v);
bool machine__is_result_of(blip__Value v, string kind);
string machine__kind_of(blip__Value v);
bool machine__literal_eq(reader__Datum* d, blip__Value v);
bool machine__machine_builtin(string name);
machine__Machine* machine__new_machine(reader__Arena* a);
machine__Machine* machine__new_machine_with(reader__Arena* a, host__Host h);
machine__Machine* machine__new_standalone(host__Host h);
bool machine__pattern_predicate(string name);
bool machine__pipe_tap(string name);
string machine__plural(i64 n);
string machine__show_value(blip__Value a, bool quoted);
bool machine__special_form(string name);
u8* malloc_noscan(ptrdiff_t n);
void map__clear(map* m);
map map__clone(map* m);
void map__delete(map* m, void* key);
bool map__exists(map* m, void* key);
void map__free(map* m);
void* map__get(map* m, void* key, void* zero);
void* map__get_and_set(map* m, void* key, void* zero);
void* map__get_check(map* m, void* key);
array map__keys(map* m);
void map__reserve(map* m, u32 n);
void map__set(map* m, void* key, void* value);
array map__values(map* m);
void map_clone_int_1(void* dest, void* pkey);
void map_clone_int_16(void* dest, void* pkey);
void map_clone_int_2(void* dest, void* pkey);
void map_clone_int_4(void* dest, void* pkey);
void map_clone_int_8(void* dest, void* pkey);
void map_clone_string(void* dest, void* pkey);
bool map_eq_int_1(void* a, void* b);
bool map_eq_int_16(void* a, void* b);
bool map_eq_int_2(void* a, void* b);
bool map_eq_int_4(void* a, void* b);
bool map_eq_int_8(void* a, void* b);
bool map_eq_string(void* a, void* b);
void map_free_nop(void* _0);
void map_free_string(void* pkey);
u64 map_hash_int_1(void* pkey);
u64 map_hash_int_16(void* pkey);
u64 map_hash_int_2(void* pkey);
u64 map_hash_int_4(void* pkey);
u64 map_hash_int_8(void* pkey);
u64 map_hash_string(void* pkey);
bool map_map_eq(map a, map b);
multi_return_double_double math__ChebSeries__eval_e(math__ChebSeries cs, double x);
double math__abs_T_f64(double a);
double math__asinh(double x);
double math__atanh(double x);
double math__copysign(double x, double y);
double math__cos(double x);
double math__cosh(double x);
double math__exp(double x);
double math__exp2(double x);
double math__expm1(double x);
double math__factorial(double n);
double math__floor(double x);
double math__fmod(double x, double y);
multi_return_double_i64 math__frexp(double x);
double math__gamma(double a);
double math__gamma_too_small(double x, double z);
double math__hypot(double x, double y);
double math__inf(i64 sign);
bool math__is_inf(double f, i64 sign);
bool math__is_nan(double f);
bool math__is_neg_int(double x);
bool math__is_odd_int(double x);
double math__ldexp(double frac, i64 exp);
double math__log(double a);
double math__log10(double x);
double math__log1p(double x);
double math__log2(double x);
double math__log_factorial_asymptotic_expansion(i64 n);
multi_return_double_i64 math__log_gamma_sign(double a);
i64 math__min_T_v_int(i64 a, i64 b);
multi_return_double_double math__modf(double f);
double math__nan(void);
multi_return_double_i64 math__normalize(double x);
double math__poly_n_1_eval(Array c, i64 n, double x);
double math__poly_n_eval(Array c, i64 n, double x);
double math__pow(double x, double y);
bool math__signbit(double x);
double math__sin(double x);
double math__sin_pi(double x_);
multi_return_double_double math__sincos(double x);
double math__sqrt(double a);
multi_return_double_double math__stirling(double x);
double math__tanh(double x);
void* memdup(void* src, ptrdiff_t sz);
void* memdup_align(void* src, ptrdiff_t sz, ptrdiff_t align);
array new_array_from_c_array(i64 len, i64 cap, i64 elm_size, void* c_array);
array new_array_from_c_array_noscan(i64 len, i64 cap, i64 elm_size, void* c_array);
map new_map(i64 key_bytes, i64 value_bytes, _fn_ptr_1e8d64a3d95a0da1 hash_fn, _fn_ptr_c95f092653bee64 key_eq_fn, _fn_ptr_1a3da2026b1489ea clone_fn, _fn_ptr_5373f7edc7b60e26 free_fn);
string none__str(__v_option _0);
string os__ExecutableNotFoundError__msg(os__ExecutableNotFoundError err);
string os__FileNotOpenedError__msg(os__FileNotOpenedError err);
i64 os__NotExpected__code(os__NotExpected err);
string os__NotExpected__msg(os__NotExpected err);
string os__SizeOfTypeIs0Error__msg(os__SizeOfTypeIs0Error err);
bool os__exists(string path);
string os__get_line(void);
bool os__is_dir(string path);
string os__join_path(string base, Array dirs);
string os__join_path_single(string base, string elem);
__v_result_Array os__ls(string path);
__v_result_string os__read_file(string path);
void panic_debug(i64 line_no, string file, string mod, string fn_name, string s);
void panic_frame_done(void* frame);
void panic_frame_pop(void* frame);
void panic_frame_push(void* frame);
void panic_frame_relink(void* frame);
void panic_frames_reset(void);
__v_option_string panic_recover_frame(void* frame);
bool prims__any_float(Array args);
__v_result_double prims__as_f64(blip__Value v);
__v_result_blip__Vectorptr prims__as_vector_value(blip__Value v, string name);
__v_result_blip__Value prims__chain(string name, Array args);
i64 prims__clamp(i64 n, i64 lo, i64 hi);
i64 prims__cmp_f64(double a, double b);
i64 prims__cmp_i64(i64 a, i64 b);
i64 prims__cmp_text(string a, string b);
__v_result_i64 prims__compare(blip__Value a, blip__Value b);
bool prims__is_number(blip__Value v);
bool prims__is_seq(blip__Value v);
__v_result_Array prims__list_slice(blip__Value v);
__v_result_i64 prims__need_int(string name, blip__Value v);
__v_result_string prims__need_str(string name, blip__Value v);
__v_result_blip__Value prims__prim_abs(Array args);
__v_result_blip__Value prims__prim_add(Array args);
__v_result_blip__Value prims__prim_append(Array args);
__v_result_blip__Value prims__prim_array_emptyp(Array args);
__v_result_blip__Value prims__prim_array_length(Array args);
__v_result_blip__Value prims__prim_array_pop(Array args);
__v_result_blip__Value prims__prim_array_push(Array args);
__v_result_blip__Value prims__prim_array_ref(Array args);
__v_result_blip__Value prims__prim_array_set(Array args);
__v_result_blip__Value prims__prim_arrayp(Array args);
__v_result_blip__Value prims__prim_booleanp(Array args);
__v_result_blip__Value prims__prim_car(Array args);
__v_result_blip__Value prims__prim_cdr(Array args);
__v_result_blip__Value prims__prim_code_to_string(Array args);
__v_result_blip__Value prims__prim_concat(Array args);
__v_result_blip__Value prims__prim_cons(Array args);
__v_result_blip__Value prims__prim_count(Array args);
__v_result_blip__Value prims__prim_dec(Array args);
__v_result_blip__Value prims__prim_dissoc_bang(Array args);
__v_result_blip__Value prims__prim_div(Array args);
__v_result_blip__Value prims__prim_drop(Array args);
__v_result_blip__Value prims__prim_eq(Array args);
__v_result_blip__Value prims__prim_eqp(Array args);
__v_result_blip__Value prims__prim_even(Array args);
__v_result_blip__Value prims__prim_expt(Array args);
__v_result_blip__Value prims__prim_floatp(Array args);
__v_result_blip__Value prims__prim_fntionp(Array args);
__v_result_blip__Value prims__prim_frequencies(Array args);
__v_result_blip__Value prims__prim_gcd(Array args);
__v_result_blip__Value prims__prim_ge(Array args);
__v_result_blip__Value prims__prim_gen_math_cos(Array args);
__v_result_blip__Value prims__prim_gen_math_exp(Array args);
__v_result_blip__Value prims__prim_gen_math_factorial(Array args);
__v_result_blip__Value prims__prim_gen_math_floor(Array args);
__v_result_blip__Value prims__prim_gen_math_log(Array args);
__v_result_blip__Value prims__prim_gen_math_log10(Array args);
__v_result_blip__Value prims__prim_gen_math_log2(Array args);
__v_result_blip__Value prims__prim_gen_math_sin(Array args);
__v_result_blip__Value prims__prim_get(Array args);
__v_result_blip__Value prims__prim_gt(Array args);
__v_result_blip__Value prims__prim_has_key(Array args);
__v_result_blip__Value prims__prim_in(Array args);
__v_result_blip__Value prims__prim_inc(Array args);
__v_result_blip__Value prims__prim_integer_to_rune(Array args);
__v_result_blip__Value prims__prim_integerp(Array args);
__v_result_blip__Value prims__prim_keyword(Array args);
__v_result_blip__Value prims__prim_keywordp(Array args);
__v_result_blip__Value prims__prim_last(Array args);
__v_result_blip__Value prims__prim_le(Array args);
__v_result_blip__Value prims__prim_length(Array args);
__v_result_blip__Value prims__prim_list(Array args);
__v_result_blip__Value prims__prim_list_ref(Array args);
__v_result_blip__Value prims__prim_listp(Array args);
__v_result_blip__Value prims__prim_lt(Array args);
__v_result_blip__Value prims__prim_make_struct(Array args);
__v_result_blip__Value prims__prim_max(Array args);
__v_result_blip__Value prims__prim_min(Array args);
__v_result_blip__Value prims__prim_modulo(Array args);
__v_result_blip__Value prims__prim_mul(Array args);
__v_result_blip__Value prims__prim_negative(Array args);
__v_result_blip__Value prims__prim_nilp(Array args);
__v_result_blip__Value prims__prim_not(Array args);
__v_result_blip__Value prims__prim_not_eq(Array args);
__v_result_blip__Value prims__prim_nullp(Array args);
__v_result_blip__Value prims__prim_number_to_string(Array args);
__v_result_blip__Value prims__prim_numberp(Array args);
__v_result_blip__Value prims__prim_odd(Array args);
__v_result_blip__Value prims__prim_pairp(Array args);
__v_result_blip__Value prims__prim_positive(Array args);
__v_result_blip__Value prims__prim_put(Array args);
__v_result_blip__Value prims__prim_put_bang(Array args);
__v_result_blip__Value prims__prim_quotient(Array args);
__v_result_blip__Value prims__prim_range(Array args);
__v_result_blip__Value prims__prim_remainder(Array args);
__v_result_blip__Value prims__prim_remove(Array args);
__v_result_blip__Value prims__prim_reverse(Array args);
__v_result_blip__Value prims__prim_rune_to_integer(Array args);
__v_result_blip__Value prims__prim_runep(Array args);
__v_result_blip__Value prims__prim_sort(Array args);
__v_result_blip__Value prims__prim_sqrt(Array args);
__v_result_blip__Value prims__prim_str(Array args);
__v_result_blip__Value prims__prim_string(Array args);
__v_result_blip__Value prims__prim_string_append(Array args);
__v_result_blip__Value prims__prim_string_blank(Array args);
__v_result_blip__Value prims__prim_string_containsp(Array args);
__v_result_blip__Value prims__prim_string_downcase(Array args);
__v_result_blip__Value prims__prim_string_index_of(Array args);
__v_result_blip__Value prims__prim_string_join(Array args);
__v_result_blip__Value prims__prim_string_length(Array args);
__v_result_blip__Value prims__prim_string_not_blank(Array args);
__v_result_blip__Value prims__prim_string_repeat(Array args);
__v_result_blip__Value prims__prim_string_replace(Array args);
__v_result_blip__Value prims__prim_string_slice(Array args);
__v_result_blip__Value prims__prim_string_split(Array args);
__v_result_blip__Value prims__prim_string_start_withp(Array args);
__v_result_blip__Value prims__prim_string_to_keyword(Array args);
__v_result_blip__Value prims__prim_string_to_number(Array args);
__v_result_blip__Value prims__prim_string_to_rune(Array args);
__v_result_blip__Value prims__prim_string_to_symbol(Array args);
__v_result_blip__Value prims__prim_string_trim(Array args);
__v_result_blip__Value prims__prim_string_upcase(Array args);
__v_result_blip__Value prims__prim_stringp(Array args);
__v_result_blip__Value prims__prim_struct_mutate(Array args);
__v_result_blip__Value prims__prim_struct_name(Array args);
__v_result_blip__Value prims__prim_struct_ref(Array args);
__v_result_blip__Value prims__prim_struct_set(Array args);
__v_result_blip__Value prims__prim_structp(Array args);
__v_result_blip__Value prims__prim_sub(Array args);
__v_result_blip__Value prims__prim_symbol(Array args);
__v_result_blip__Value prims__prim_symbolp(Array args);
__v_result_blip__Value prims__prim_table_keys(Array args);
__v_result_blip__Value prims__prim_tablep(Array args);
__v_result_blip__Value prims__prim_take(Array args);
__v_result_blip__Value prims__prim_vector(Array args);
__v_result_blip__Value prims__prim_vector_append(Array args);
__v_result_blip__Value prims__prim_vector_containsp(Array args);
__v_result_blip__Value prims__prim_vector_emptyp(Array args);
__v_result_blip__Value prims__prim_vector_length(Array args);
__v_result_blip__Value prims__prim_vector_ref(Array args);
__v_result_blip__Value prims__prim_vector_rev(Array args);
__v_result_blip__Value prims__prim_vectorp(Array args);
__v_result_blip__Value prims__prim_zero(Array args);
void prims__register_gen_math(map* p);
string prims__rune_bytes(i64 cp);
__v_result_Array prims__seq(blip__Value v);
blip__Value prims__seq_like(blip__Value src, Array items);
string prims__show(blip__Value a, bool quote_strings);
__v_result_blip__Value prims__sort_by_keyed(Array items, Array keys);
map prims__table(void);
blip__Value prims__tag_is(blip__Value v, int t);
bool prims__value_eq(blip__Value a, blip__Value b);
void print(string s);
string printer__char_literal(i32 cp);
string printer__close_for(string v_open);
string printer__quote_string(string s);
string printer__wrap(string v_open, reader__Arena* a, i32 id);
string printer__wrap1(string prefix, reader__Arena* a, i32 id);
string printer__write(blip__Value v);
string printer__write_datum(reader__Arena* a, i32 id);
string printer__write_list(blip__Value v, Array active, map seen, i64 depth);
string printer__write_seq(blip__Value v, Array active, map seen, i64 depth);
string printer__write_table(blip__Value v, Array active, map seen, i64 depth);
string printer__write_table_body(blip__Value v, Array active, map seen, i64 depth);
string printer__write_value(blip__Value v, Array active, map seen, i64 depth);
string printer__write_vector(blip__Value v, Array active, map seen, i64 depth);
void println(string s);
string ptr_str(void* ptr);
void reader__Arena__add_child(reader__Arena* a, i32 parent, i32 child);
i64 reader__Arena__count(reader__Arena* a);
void reader__Arena__finish(reader__Arena* a, i32 id, Array items);
i32 reader__Arena__float_leaf(reader__Arena* a, double f);
i32 reader__Arena__int_leaf(reader__Arena* a, int tag, i64 n);
Array reader__Arena__kids(reader__Arena* a, i32 id);
i32 reader__Arena__leaf(reader__Arena* a, int tag);
reader__Datum* reader__Arena__node(reader__Arena* a, i32 id);
i32 reader__Arena__open(reader__Arena* a, int tag);
reader__Forms reader__Arena__read_forms(reader__Arena* a, string src);
void reader__Arena__set_pos(reader__Arena* a, i32 id, i64 line, i64 col);
i32 reader__Arena__str_leaf(reader__Arena* a, int tag, string s);
bool reader__DatumTag_is_collection(int t);
bool reader__DatumTag_is_stringy(int t);
string reader__Diagnostic__render(reader__Diagnostic* d, string path);
u8 reader__Reader__at(reader__Reader* r, i64 i);
bool reader__Reader__eof(reader__Reader* r);
void reader__Reader__fail(reader__Reader* r, string msg);
bool reader__Reader__is_delimiter(reader__Reader* r, u8 c);
u8 reader__Reader__next(reader__Reader* r);
u8 reader__Reader__peek(reader__Reader* r);
__v_option_i32 reader__Reader__read(reader__Reader* r);
__v_option_i32 reader__Reader__read_atom(reader__Reader* r);
__v_option_i32 reader__Reader__read_bar_symbol(reader__Reader* r);
__v_option_i32 reader__Reader__read_character(reader__Reader* r);
__v_option_i32 reader__Reader__read_collection(reader__Reader* r, int tag, u8 v_close);
__v_option_i32 reader__Reader__read_hash(reader__Reader* r);
__v_option_i32 reader__Reader__read_inner(reader__Reader* r);
__v_option_i32 reader__Reader__read_string(reader__Reader* r);
string reader__Reader__read_symbol_text(reader__Reader* r);
void reader__Reader__short(reader__Reader* r, string msg);
void reader__Reader__skip_block_comment(reader__Reader* r);
void reader__Reader__skip_ws(reader__Reader* r);
__v_option_i32 reader__Reader__try_number(reader__Reader* r, string s);
reader__Depth reader__depth(string src);
reader__ReadResult reader__read_all(string src);
__v_result_string run_example(string path);
Array rune__bytes(u32 c);
string rune__str(u32 c);
u32 rune__to_lower(u32 c);
string strconv__Dec32__get_string_32(strconv__Dec32 d, bool neg, i64 i_n_digit, i64 i_pad_digit);
string strconv__Dec64__get_string_64(strconv__Dec64 d, bool neg, i64 i_n_digit, i64 i_pad_digit);
__v_result_double strconv__atof64(string s, strconv__AtoF64Param param);
__v_result_i64 strconv__atoi(string s);
__v_result_i64 strconv__atoi64(string s);
string strconv__f32_to_str_l(float f);
string strconv__f64_to_str_l(double f);
string strconv__format_int(i64 n, i64 radix);
string strconv__format_uint(u64 n, i64 radix);
__v_result_u64 strconv__parse_uint(string s, i64 _base, i64 _bit_size);
string string__all_after(string s, string sub);
string string__all_after_last(string s, string sub);
string string__all_before(string s, string sub);
string string__all_before_last(string s, string sub);
u8 string__at(string s, i64 idx);
u8 string__at_i64(string s, i64 idx);
u8 string__at_ni(string s, i64 idx);
u8 string__at_u64(string s, u64 idx);
Array string__bytes(string s);
string string__clone(string a);
bool string__contains(string s, string substr);
bool string__ends_with(string s, string p);
bool string__eq(string s, string a);
void string__free(string* s);
__v_option_i64 string__index(string s, string p);
bool string__is_capital(string s);
__v_option_i64 string__last_index(string s, string needle);
bool string__lt(string s, string a);
string string__plus(string s, string a);
string string__repeat(string s, i64 count);
string string__replace(string s, string rep, string with);
Array string__split(string s, string delim);
bool string__starts_with(string s, string p);
string string__str(string s);
string string__substr(string s, i64 start, i64 _end);
string string__substr_ni(string s, i64 _start, i64 _end);
string string__to_lower(string s);
string string__to_lower_ascii(string s);
string string__to_upper(string s);
string string__trim_space(string s);
string string_plus_many(i64 data_len, string* input_base);
void strings__Builder__free(Array* b);
string strings__Builder__str(Array* b);
void strings__Builder__write_ptr(Array* b, u8* ptr, i64 len);
void strings__Builder__write_runes(Array* b, Array runes);
void strings__Builder__write_string(Array* b, string s);
void strings__Builder__write_u8(Array* b, u8 data);
Array strings__new_builder(i64 initial_size);
string tos(u8* s, i64 len);
string u128__str(u128 nn);
string u16__str(u16 n);
string u32__str(u32 nn);
string u64__str(u64 nn);
string u8__hex(u8 nn);
bool u8__is_capital(u8 c);
bool u8__is_letter(u8 c);
string u8__str(u8 b);
string u8__vstring(u8* bp);
string u8__vstring_with_len(u8* bp, i64 len);
string usize__str(size_t x);
void v_exit(i64 code);
i64 v_fixed_index(i64 i, i64 len);
i64 v_fixed_index_i64(i64 i, i64 len);
i64 v_fixed_index_ni(i64 i, i64 len);
i64 v_fixed_index_u64(u64 i, i64 len);
void v_free(void* ptr);
void v_panic(string s);
i64 v_slice_index_i64(i64 i);
i64 v_slice_index_u64(u64 i);
void* vmemcpy(void* dest, void* const_src, ptrdiff_t n);
string voidptr__str(void* nn);
Array voidptr__vbytes(void* data, i64 len);

DenseArray DenseArray__clone(DenseArray* d);
i64 Error__code(Error err);
string Error__msg(Error err);
string IError__str(IError err);
i64 MessageError__code(MessageError err);
string MessageError__msg(MessageError err);
string MessageError__str(MessageError err);
string None____str(None__ _0);
string StrIntpType_str(int x);
VMapData* VMapData__clone(VMapData* m);
void* __as_cast(void* obj, i64 obj_type, i64 expected_type, string obj_name, string expected_name);
array __new_array(i64 mylen, i64 cap, i64 elm_size);
array __new_array_noscan(i64 mylen, i64 cap, i64 elm_size);
array array__clone(array* a);
void array__delete(array* a, i64 i);
void array__delete_last(array* a);
void array__ensure_cap(array* a, i64 required);
void array__free(array* a);
void* array__get(array a, i64 i);
void array__insert(array* a, i64 i, void* val);
void array__insert_many(array* a, i64 i, void* val, i64 size);
void* array__pop_left(array* a);
void array__prepend(array* a, void* val);
void array__push(array* a, void* val);
void array__push_many(array* a, void* val, i64 size);
array array__reverse(array a);
void array__set(array* a, i64 i, void* val);
array array__slice(array a, i64 start, i64 _end);
array array__slice_ni(array a, i64 _start, i64 _end);
void array__sort_with_compare(array* a, _fn_ptr_ed95774d3a4e97ab callback);
void array_sort_move(void* dst, i64 di, void* src, i64 si, i64 count, size_t element_size);
void autostr_addr_pop(void);
bool autostr_addr_type_in_stack(void* addr, i64 typ);
void autostr_addr_type_push(void* addr, i64 typ);
string bool__str(bool b);
void builtin_init(void);
string byteptr__str(u8* nn);
Array byteptr__vbytes(u8* data, i64 len);
string byteptr__vstring(u8* bp);
string byteptr__vstring_with_len(u8* bp, i64 len);
string char__str(char* cptr);
string charptr__str(char* nn);
string charptr__vstring(char* cp);
string charptr__vstring_with_len(char* cp, i64 len);
void* closure__closure_create_with_data(void* func, void* data, bool owns_data);
void closure__closure_init(void);
void closure__closure_try_destroy(void* closure);
i64 compare_lower_strings(string* a, string* b);
i64 compare_strings_by_len(string* a, string* b);
string data_to_hex_string(u8* data, i64 len);
void* dl__interface_export_find(char* iface, char* method, u32 typ);
void drop_owned_interface_T_IError(IError value);
void drop_owned_result_error(IError err);
void drop_owned_result_error_interface(IError err);
void drop_owned_v3_interface_T_IError(IError value);
IError error(string message);
IError error_with_code(string message, i64 code);
string f32__str(float x);
string f64__str(double x);
string float_literal__str(float_literal d);
bool gc_prepare_for_debugger_init(void);
void gc_restore_roots_after_debugger_init(bool use_manual_roots);
void gc_runtime_init(void);
string i128__str(i128 nn);
string i16__str(i16 n);
string i32__str(i32 n);
string i64__str(i64 nn);
string i8__str(i8 n);
u32 impl_utf8_to_utf32(u8* _bytes, i64 _bytes_len);
string int__str(i64 n);
string int_literal__str(int_literal n);
string isize__str(ptrdiff_t x);
u8* malloc_noscan(ptrdiff_t n);
void map__clear(map* m);
map map__clone(map* m);
void map__delete(map* m, void* key);
bool map__exists(map* m, void* key);
void map__free(map* m);
void* map__get(map* m, void* key, void* zero);
void* map__get_and_set(map* m, void* key, void* zero);
void* map__get_check(map* m, void* key);
array map__keys(map* m);
void map__reserve(map* m, u32 n);
void map__set(map* m, void* key, void* value);
array map__values(map* m);
void map_clone_int_1(void* dest, void* pkey);
void map_clone_int_16(void* dest, void* pkey);
void map_clone_int_2(void* dest, void* pkey);
void map_clone_int_4(void* dest, void* pkey);
void map_clone_int_8(void* dest, void* pkey);
void map_clone_string(void* dest, void* pkey);
bool map_eq_int_1(void* a, void* b);
bool map_eq_int_16(void* a, void* b);
bool map_eq_int_2(void* a, void* b);
bool map_eq_int_4(void* a, void* b);
bool map_eq_int_8(void* a, void* b);
bool map_eq_string(void* a, void* b);
void map_free_nop(void* _0);
void map_free_string(void* pkey);
u64 map_hash_int_1(void* pkey);
u64 map_hash_int_16(void* pkey);
u64 map_hash_int_2(void* pkey);
u64 map_hash_int_4(void* pkey);
u64 map_hash_int_8(void* pkey);
u64 map_hash_string(void* pkey);
bool map_map_eq(map a, map b);
multi_return_double_double math__ChebSeries__eval_e(math__ChebSeries cs, double x);
double math__abs_T_f64(double a);
double math__asinh(double x);
double math__atanh(double x);
double math__copysign(double x, double y);
double math__cos(double x);
double math__cosh(double x);
double math__exp(double x);
double math__exp2(double x);
double math__expm1(double x);
double math__factorial(double n);
double math__floor(double x);
double math__fmod(double x, double y);
multi_return_double_i64 math__frexp(double x);
double math__gamma(double a);
double math__gamma_too_small(double x, double z);
double math__hypot(double x, double y);
double math__inf(i64 sign);
bool math__is_inf(double f, i64 sign);
bool math__is_nan(double f);
bool math__is_neg_int(double x);
bool math__is_odd_int(double x);
double math__ldexp(double frac, i64 exp);
double math__log(double a);
double math__log10(double x);
double math__log1p(double x);
double math__log2(double x);
double math__log_factorial_asymptotic_expansion(i64 n);
multi_return_double_i64 math__log_gamma_sign(double a);
i64 math__min_T_v_int(i64 a, i64 b);
multi_return_double_double math__modf(double f);
double math__nan(void);
multi_return_double_i64 math__normalize(double x);
double math__poly_n_1_eval(Array c, i64 n, double x);
double math__poly_n_eval(Array c, i64 n, double x);
double math__pow(double x, double y);
bool math__signbit(double x);
double math__sin(double x);
double math__sin_pi(double x_);
multi_return_double_double math__sincos(double x);
double math__sqrt(double a);
multi_return_double_double math__stirling(double x);
double math__tanh(double x);
void* memdup(void* src, ptrdiff_t sz);
void* memdup_align(void* src, ptrdiff_t sz, ptrdiff_t align);
array new_array_from_c_array(i64 len, i64 cap, i64 elm_size, void* c_array);
array new_array_from_c_array_noscan(i64 len, i64 cap, i64 elm_size, void* c_array);
map new_map(i64 key_bytes, i64 value_bytes, _fn_ptr_1e8d64a3d95a0da1 hash_fn, _fn_ptr_c95f092653bee64 key_eq_fn, _fn_ptr_1a3da2026b1489ea clone_fn, _fn_ptr_5373f7edc7b60e26 free_fn);
string none__str(__v_option _0);
string os__ExecutableNotFoundError__msg(os__ExecutableNotFoundError err);
string os__FileNotOpenedError__msg(os__FileNotOpenedError err);
i64 os__NotExpected__code(os__NotExpected err);
string os__NotExpected__msg(os__NotExpected err);
string os__SizeOfTypeIs0Error__msg(os__SizeOfTypeIs0Error err);
bool os__exists(string path);
string os__get_line(void);
bool os__is_dir(string path);
string os__join_path(string base, Array dirs);
string os__join_path_single(string base, string elem);
__v_result_Array os__ls(string path);
__v_result_string os__read_file(string path);
void panic_debug(i64 line_no, string file, string mod, string fn_name, string s);
void panic_frame_done(void* frame);
void panic_frame_pop(void* frame);
void panic_frame_push(void* frame);
void panic_frame_relink(void* frame);
void panic_frames_reset(void);
__v_option_string panic_recover_frame(void* frame);
void print(string s);
void println(string s);
string ptr_str(void* ptr);
Array rune__bytes(u32 c);
string rune__str(u32 c);
u32 rune__to_lower(u32 c);
string strconv__Dec32__get_string_32(strconv__Dec32 d, bool neg, i64 i_n_digit, i64 i_pad_digit);
string strconv__Dec64__get_string_64(strconv__Dec64 d, bool neg, i64 i_n_digit, i64 i_pad_digit);
__v_result_double strconv__atof64(string s, strconv__AtoF64Param param);
__v_result_i64 strconv__atoi(string s);
__v_result_i64 strconv__atoi64(string s);
string strconv__f32_to_str_l(float f);
string strconv__f64_to_str_l(double f);
string strconv__format_int(i64 n, i64 radix);
string strconv__format_uint(u64 n, i64 radix);
__v_result_u64 strconv__parse_uint(string s, i64 _base, i64 _bit_size);
string string__all_after(string s, string sub);
string string__all_after_last(string s, string sub);
string string__all_before(string s, string sub);
string string__all_before_last(string s, string sub);
u8 string__at(string s, i64 idx);
u8 string__at_i64(string s, i64 idx);
u8 string__at_ni(string s, i64 idx);
u8 string__at_u64(string s, u64 idx);
Array string__bytes(string s);
string string__clone(string a);
bool string__contains(string s, string substr);
bool string__ends_with(string s, string p);
bool string__eq(string s, string a);
void string__free(string* s);
__v_option_i64 string__index(string s, string p);
bool string__is_capital(string s);
__v_option_i64 string__last_index(string s, string needle);
bool string__lt(string s, string a);
string string__plus(string s, string a);
string string__repeat(string s, i64 count);
string string__replace(string s, string rep, string with);
Array string__split(string s, string delim);
bool string__starts_with(string s, string p);
string string__str(string s);
string string__substr(string s, i64 start, i64 _end);
string string__substr_ni(string s, i64 _start, i64 _end);
string string__to_lower(string s);
string string__to_lower_ascii(string s);
string string__to_upper(string s);
string string__trim_space(string s);
string string_plus_many(i64 data_len, string* input_base);
void strings__Builder__free(Array* b);
string strings__Builder__str(Array* b);
void strings__Builder__write_ptr(Array* b, u8* ptr, i64 len);
void strings__Builder__write_runes(Array* b, Array runes);
void strings__Builder__write_string(Array* b, string s);
void strings__Builder__write_u8(Array* b, u8 data);
Array strings__new_builder(i64 initial_size);
string tos(u8* s, i64 len);
string u128__str(u128 nn);
string u16__str(u16 n);
string u32__str(u32 nn);
string u64__str(u64 nn);
string u8__hex(u8 nn);
bool u8__is_capital(u8 c);
bool u8__is_letter(u8 c);
string u8__str(u8 b);
string u8__vstring(u8* bp);
string u8__vstring_with_len(u8* bp, i64 len);
string usize__str(size_t x);
void v_exit(i64 code);
i64 v_fixed_index(i64 i, i64 len);
i64 v_fixed_index_i64(i64 i, i64 len);
i64 v_fixed_index_ni(i64 i, i64 len);
i64 v_fixed_index_u64(u64 i, i64 len);
void v_free(void* ptr);
void v_panic(string s);
i64 v_slice_index_i64(i64 i);
i64 v_slice_index_u64(u64 i);
void* vmemcpy(void* dest, void* const_src, ptrdiff_t n);
string voidptr__str(void* nn);
Array voidptr__vbytes(void* data, i64 len);

string IError__msg(IError* i);
string Error__msg(Error _recv);
string MessageError__msg(MessageError _recv);
string os__ExecutableNotFoundError__msg(os__ExecutableNotFoundError _recv);
string os__FileNotOpenedError__msg(os__FileNotOpenedError _recv);
string os__NotExpected__msg(os__NotExpected _recv);
string os__SizeOfTypeIs0Error__msg(os__SizeOfTypeIs0Error _recv);
i64 IError__code(IError* i);
i64 Error__code(Error _recv);
i64 MessageError__code(MessageError _recv);
i64 os__NotExpected__code(os__NotExpected _recv);
void host__Host__host_print(host__Host* i, string _a0);
void host__Host__host_write(host__Host* i, string _a0);
__v_option_string host__Host__host_read_line(host__Host* i);
__v_result_string host__Host__host_load(host__Host* i, string _a0);
Array hash__Hash__sum(hash__Hash* i, Array _a0);
i64 hash__Hash__size(hash__Hash* i);
i64 hash__Hash__block_size(hash__Hash* i);
void hash__Hash__free(hash__Hash* i);
void hash__Hash__reset(hash__Hash* i);
__v_result_i64 hash__Hash__write(hash__Hash* i, Array _a0);
u32 hash__Hash32er__sum32(hash__Hash32er* i);
u64 hash__Hash64er__sum64(hash__Hash64er* i);

string ArrayFlags__autostr(ArrayFlags it);
string ChanState__autostr(ChanState it);
string GraphemeBreakProperty__autostr(GraphemeBreakProperty it);
string AttributeKind__autostr(AttributeKind it);
string MapMode__autostr(MapMode it);
string TrimMode__autostr(TrimMode it);
string strings__IndentState__autostr(strings__IndentState it);
string strconv__Align_text__autostr(strconv__Align_text it);
string strconv__ParserState__autostr(strconv__ParserState it);
string strconv__Char_parse_state__autostr(strconv__Char_parse_state it);
string os__WalkDirAction__autostr(os__WalkDirAction it);
string os__ProcessState__autostr(os__ProcessState it);
string os__Signal__autostr(os__Signal it);
string os__SeekMode__autostr(os__SeekMode it);
string os__FileBufferMode__autostr(os__FileBufferMode it);
string os__FileType__autostr(os__FileType it);
string os__ChildProcessPipeKind__autostr(os__ChildProcessPipeKind it);
string blip__Tag__autostr(blip__Tag it);
string blip__KontTag__autostr(blip__KontTag it);
string closure__MemoryProtectAtrr__autostr(closure__MemoryProtectAtrr it);
string reader__DatumTag__autostr(reader__DatumTag it);

static const string _v3_lit_0_14650fb0739d0383 = {"", 0, 1};
static const string _v3_lit_1_44bd31d473cd01cb = {"\n", 1, 1};
static const string _v3_lit_1_44bd4bd473cd2df9 = {" ", 1, 1};
static const string _v3_lit_6_99f02d3504a82c5b = {"  --> ", 6, 1};
static const string _v3_lit_6_7e8af11904d0db3c = {" (and ", 6, 1};
static const string _v3_lit_30_300091d0d8aec6b0 = {" (fn [v] (struct-ref v (quote ", 30, 1};
static const string _v3_lit_5_5d2582c7b6acfdf0 = {" (in ", 5, 1};
static const string _v3_lit_18_389a818e38edfcea = {" (in field access ", 18, 1};
static const string _v3_lit_4_ef98beaa97559659 = {" -- ", 4, 1};
static const string _v3_lit_15_be53b2cb10e1a1c0 = {" ...), it is a ", 15, 1};
static const string _v3_lit_6_11fe221e2ab62bee = {" := : ", 6, 1};
static const string _v3_lit_11_d00d55f7f09898a7 = {" FAILURE(S)", 11, 1};
static const string _v3_lit_15_935b1751495b3b8f = {" [name default]", 15, 1};
static const string _v3_lit_9_97b175148f7d162a = {" argument", 9, 1};
static const string _v3_lit_34_a137a20ed89a4c52 = {" cannot be a destructuring binding", 34, 1};
static const string _v3_lit_51_7bb9acc08615637 = {" cannot be a destructuring binding: it is a literal", 51, 1};
static const string _v3_lit_26_4ace1ee796af487d = {" expects 2 arguments, got ", 26, 1};
static const string _v3_lit_23_adaa30b73fc179ad = {" expects a string, got ", 23, 1};
static const string _v3_lit_32_9f81ea044ae9fb41 = {" expects a vector or array, got ", 32, 1};
static const string _v3_lit_25_107907bb28132e24 = {" expects an integer, got ", 25, 1};
static const string _v3_lit_29_7dca5081abc47265 = {" expects at least 2 arguments", 29, 1};
static const string _v3_lit_35_a0f235541515e79b = {" expects at least 2 arguments, got ", 35, 1};
static const string _v3_lit_35_5281bf074bd6df94 = {" expects at least 3 arguments, got ", 35, 1};
static const string _v3_lit_9_155bc01a1f2a1092 = {" failed: ", 9, 1};
static const string _v3_lit_18_144042484dae3bc = {" field(s) but got ", 18, 1};
static const string _v3_lit_5_c320b6af8ce0b5fd = {" has ", 5, 1};
static const string _v3_lit_17_4776d370e5aa6766 = {" has no arguments", 17, 1};
static const string _v3_lit_14_14368fa9ebdd61d6 = {" has no field ", 14, 1};
static const string _v3_lit_12_b4a90bede6b06415 = {" has no name", 12, 1};
static const string _v3_lit_15_6f0b7687d7432444 = {" has no pattern", 15, 1};
static const string _v3_lit_31_908db3d2edea40f = {" has no such labelled parameter", 31, 1};
static const string _v3_lit_37_7f6f45e5ada2c7e4 = {" is both a fixed and a rest parameter", 37, 1};
static const string _v3_lit_15_68938722c38edfc0 = {" is named twice", 15, 1};
static const string _v3_lit_11_b4105fc71b06f1cf = {" is not a (", 11, 1};
static const string _v3_lit_19_784951e180b3f488 = {" is not a character", 19, 1};
static const string _v3_lit_20_9f2988141826984a = {" is not a code point", 20, 1};
static const string _v3_lit_22_925195d689b3a9c3 = {" is not a field access", 22, 1};
static const string _v3_lit_22_f94af3cc5b978b0c = {" is not a field update", 22, 1};
static const string _v3_lit_45_3d6dbd21f2b0dab6 = {" is not a file path, or a form containing one", 45, 1};
static const string _v3_lit_64_491685a9fc0ec99a = {" is not a known procedure, and a pattern list needs a known head", 64, 1};
static const string _v3_lit_15_4bd17a79af2a715b = {" is not a macro", 15, 1};
static const string _v3_lit_14_4c516eb5bd30ac9e = {" is not a name", 14, 1};
static const string _v3_lit_16_869ea3525e9dc698 = {" is not a struct", 16, 1};
static const string _v3_lit_15_fffbc03f977f8359 = {" is not defined", 15, 1};
static const string _v3_lit_12_3926ab14809ec566 = {" is required", 12, 1};
static const string _v3_lit_8_1b3088722da67dc0 = {" literal", 8, 1};
static const string _v3_lit_6_dbe8ee8a70d5a57f = {" more)", 6, 1};
static const string _v3_lit_17_19823545a3322d36 = {" must be written ", 17, 1};
static const string _v3_lit_44_77642ffa6ebb3888 = {" must be written [name default] or [default]", 44, 1};
static const string _v3_lit_7_c66143f46680e5d2 = {" needs ", 7, 1};
static const string _v3_lit_32_67ed9a70b0946400 = {" needs a [name default] after it", 32, 1};
static const string _v3_lit_24_a29c06cf49e87a0b = {" needs a value to thread", 24, 1};
static const string _v3_lit_4_d5dfe2a966931484 = {" of ", 4, 1};
static const string _v3_lit_22_ef0a88c1130ba183 = {" out of range (length ", 22, 1};
static const string _v3_lit_7_d024e13696b71b91 = {" up in ", 7, 1};
static const string _v3_lit_9_65e5c60de39af7d6 = {" value(s)", 9, 1};
static const string _v3_lit_16_a4c7fcac8501a97b = {" was given twice", 16, 1};
static const string _v3_lit_6_a1f910afef24beb7 = {" with ", 6, 1};
static const string _v3_lit_3_bce9634fd643284f = {" |\n", 3, 1};
static const string _v3_lit_3_bce94d4fd64302ed = {" | ", 3, 1};
static const string _v3_lit_39_39b9307000d3189f = {"! (fn [v x] (__struct-mutate! v (quote ", 39, 1};
static const string _v3_lit_1_44bd49d473cd2a93 = {"\"", 1, 1};
static const string _v3_lit_3_ad6b694fce019f13 = {"\":\n", 3, 1};
static const string _v3_lit_2_9acb8900c59c4bd8 = {"#(", 2, 1};
static const string _v3_lit_6_6f0c4e5b2a1c16de = {"#:when", 6, 1};
static const string _v3_lit_10_26e8cee2f2508a8d = {"#<closure>", 10, 1};
static const string _v3_lit_15_9c9e29760b1998c7 = {"#<continuation>", 15, 1};
static const string _v3_lit_8_5ad82412f08c148b = {"#<datum>", 8, 1};
static const string _v3_lit_12_fe6b253d5f594937 = {"#<primitive ", 12, 1};
static const string _v3_lit_10_edc95ee159574c78 = {"#<unknown>", 10, 1};
static const string _v3_lit_2_9acb7d00c59c3774 = {"#\\", 2, 1};
static const string _v3_lit_9_7c8ef902fe4e32d7 = {"#\\invalid", 9, 1};
static const string _v3_lit_2_9acb4700c59bdbb2 = {"#f", 2, 1};
static const string _v3_lit_2_9acb5500c59bf37c = {"#t", 2, 1};
static const string _v3_lit_1_44bd4cd473cd2fac = {"'", 1, 1};
static const string _v3_lit_1_44bd53d473cd3b91 = {"(", 1, 1};
static const string _v3_lit_2_9af12a00c5bc79a8 = {"()", 2, 1};
static const string _v3_lit_40_d915a60e4a6944cf = {"(cons head tail) needs a head and a tail", 40, 1};
static const string _v3_lit_8_25c1121422da11d2 = {"(define ", 8, 1};
static const string _v3_lit_9_123b237388c8bce = {"(define (", 9, 1};
static const string _v3_lit_12_d97a2f1b85d1ef5d = {"(define set-", 12, 1};
static const string _v3_lit_47_3a69f10856a6b645 = {"(struct Name f: p ...) needs a name and a field", 47, 1};
static const string _v3_lit_1_44bd52d473cd39de = {")", 1, 1};
static const string _v3_lit_24_41dc16bd64858738 = {") (__make-struct (quote ", 24, 1};
static const string _v3_lit_10_8c0d7053025f758f = {") (quote [", 10, 1};
static const string _v3_lit_7_68e2bba7bfbb7bff = {") x)))\n", 7, 1};
static const string _v3_lit_3_fb6b024ffa325e8d = {"))\n", 3, 1};
static const string _v3_lit_4_68bc92e62331abfa = {")))\n", 4, 1};
static const string _v3_lit_5_2a82f70dcdc0c5f7 = {"))))\n", 5, 1};
static const string _v3_lit_6_fce65f749e36cfc0 = {")))))\n", 6, 1};
static const string _v3_lit_1_44bd51d473cd382b = {"*", 1, 1};
static const string _v3_lit_1_44bd50d473cd3678 = {"+", 1, 1};
static const string _v3_lit_1_44bd57d473cd425d = {",", 1, 1};
static const string _v3_lit_2_9afebb00c5c7fa67 = {", ", 2, 1};
static const string _v3_lit_18_4e03d2be2523703d = {", expected byte 0x", 18, 1};
static const string _v3_lit_6_45ecea13c54f3ff1 = {", got ", 6, 1};
static const string _v3_lit_2_9afe5b00c5c75747 = {",@", 2, 1};
static const string _v3_lit_1_44bd56d473cd40aa = {"-", 1, 1};
static const string _v3_lit_29_3db5fd0586ed702 = {"- expects at least 1 argument", 29, 1};
static const string _v3_lit_2_9afb1f00c5c4bb7c = {"->", 2, 1};
static const string _v3_lit_3_1d70f0500d4a3126 = {"->>", 3, 1};
static const string _v3_lit_1_44bd55d473cd3ef7 = {".", 1, 1};

static const string _v3_lit_3_14ac5950084b3b2f = {"...", 3, 1};
static const string _v3_lit_1_44bd54d473cd3d44 = {"/", 1, 1};
static const string _v3_lit_29_b84c690f5a4c5780 = {"/ expects exactly 2 arguments", 29, 1};







static const string _v3_lit_9_558d47ffe2d92c7a = {"01_basics", 9, 1};
static const string _v3_lit_7_75dc7a5b38b5226a = {"02_data", 7, 1};

static const string _v3_lit_8_df3d8914fbfdb3f4 = {"03_match", 8, 1};
static const string _v3_lit_9_f8dd2fff34037007 = {"04_errors", 9, 1};
static const string _v3_lit_12_b366167f60875f44 = {"05_pipelines", 12, 1};
static const string _v3_lit_10_d691db9c711ca9e3 = {"06_modules", 10, 1};





static const string _v3_lit_1_44bd61d473cd535b = {":", 1, 1};
static const string _v3_lit_2_9b20b700c5e4da01 = {": ", 2, 1};
static const string _v3_lit_11_b1208f735e965447 = {": expected ", 11, 1};
static const string _v3_lit_20_e4d543cd5e695 = {": expected at least ", 20, 1};
static const string _v3_lit_8_eb88381b623a2071 = {": field ", 8, 1};
static const string _v3_lit_15_a204aa75be175640 = {": needs a value", 15, 1};
static const string _v3_lit_16_b98d83b1d0b4a2b = {": not a function", 16, 1};
static const string _v3_lit_12_88a87abdc94c4c4c = {": parameter ", 12, 1};
static const string _v3_lit_43_57cc29f0e837da19 = {": the dot must introduce the last parameter", 43, 1};
static const string _v3_lit_31_a18017baa24e937f = {": the dot needs a name after it", 31, 1};
static const string _v3_lit_36_a0ef49fa38cf331e = {": the generated definitions failed: ", 36, 1};
static const string _v3_lit_24_387a7b0f1749397f = {": the parameter name in ", 24, 1};
static const string _v3_lit_2_9b20a200c5e4b652 = {":=", 2, 1};

static const string _v3_lit_1_44bd67d473cd5d8d = {"<", 1, 1};
static const string _v3_lit_2_9b351e00c5f63210 = {"<=", 2, 1};
static const string _v3_lit_1_44bd66d473cd5bda = {"=", 1, 1};
static const string _v3_lit_2_9b319f00c5f3246c = {"=>", 2, 1};
static const string _v3_lit_1_44bd65d473cd5a27 = {">", 1, 1};
static const string _v3_lit_2_9b2e2200c5f01a2e = {">=", 2, 1};
static const string _v3_lit_53_b0e33b0845fe521d = {"? (fn [v] (and (struct? v) (= (struct-name v) (quote ", 53, 1};
static const string _v3_lit_2_9b423c00c600ef66 = {"@[", 2, 1};
static const string _v3_lit_2_9b425c00c60125c6 = {"@{", 2, 1};

static const string _v3_lit_5_6b447c763a12a2d = {"FAIL ", 5, 1};

static const string _v3_lit_1_44bd80d473cd8808 = {"[", 1, 1};


static const string _v3_lit_1_44bd86d473cd923a = {"]", 1, 1};
static const string _v3_lit_24_9be0edfd935b40bc = {"] (__make-struct (quote ", 24, 1};
static const string _v3_lit_3_bcfe9450f899746b = {"]) ", 3, 1};
static const string _v3_lit_1_44bd85d473cd9087 = {"^", 1, 1};
static const string _v3_lit_1_44bd84d473cd8ed4 = {"_", 1, 1};
static const string _v3_lit_8_250a0eb798275844 = {"__cond_f", 8, 1};
static const string _v3_lit_8_250a20b7982776da = {"__cond_t", 8, 1};
static const string _v3_lit_12_f1b6c4dba48c7d8e = {"__ctor (fn [", 12, 1};
static const string _v3_lit_13_fcd81ef130279c17 = {"__make-struct", 13, 1};
static const string _v3_lit_43_c4014c7a0d850ac2 = {"__make-struct needs a name and a field list", 43, 1};
static const string _v3_lit_15_fcb000a30e2db3d5 = {"__make-struct: ", 15, 1};
static const string _v3_lit_16_fdd74acda8495128 = {"__struct-mutate!", 16, 1};
static const string _v3_lit_18_4dedf3bb98c52842 = {"__struct-mutate!: ", 18, 1};

static const string _v3_lit_1_44bd8bd473cd9ab9 = {"`", 1, 1};
static const string _v3_lit_6_ff41e9a110025516 = {"a body", 6, 1};
static const string _v3_lit_8_740f090f5f7c291f = {"a clause", 8, 1};
static const string _v3_lit_47_8b197156a682cfb9 = {"a key used as a function takes 1 argument, got ", 47, 1};
static const string _v3_lit_50_751630a401949a87 = {"a struct destructuring binding needs a struct name", 50, 1};
static const string _v3_lit_59_1a0b0d87736dac9b = {"a struct destructuring field must be written field: pattern", 59, 1};
static const string _v3_lit_48_330e6c49245a6046 = {"a table destructuring binding needs keyword keys", 48, 1};
static const string _v3_lit_54_431276ef32a218e = {"a table literal needs an even number of elements, got ", 54, 1};
static const string _v3_lit_50_a65bb37a8f2760ad = {"a table literal needs keyword or string keys, got ", 50, 1};
static const string _v3_lit_49_d37bcb594e447c3e = {"a table used as a function takes 1 argument, got ", 49, 1};
static const string _v3_lit_54_4e1f88c2111738e6 = {"a vector used as a function needs an integer key, got ", 54, 1};


static const string _v3_lit_3_e16811510db8ba2d = {"abs", 3, 1};

static const string _v3_lit_11_726746d68b4e7079 = {"all-results", 11, 1};
static const string _v3_lit_5_5acb56cc7a626865 = {"alpha", 5, 1};
static const string _v3_lit_52_894a26ade82627c9 = {"an array used as a function takes 1 integer argument", 52, 1};
static const string _v3_lit_3_e1761a510dc506d4 = {"and", 3, 1};
static const string _v3_lit_4_e059a2ba657c62a4 = {"any?", 4, 1};
static const string _v3_lit_6_c4049a7f2a5192d3 = {"append", 6, 1};
static const string _v3_lit_5_f1969731237b5c95 = {"apply", 5, 1};
static const string _v3_lit_40_f8681eaf0b592b41 = {"apply expects at least 2 arguments, got ", 40, 1};
static const string _v3_lit_12_5b0cdad9b5e62f6c = {"array index ", 12, 1};
static const string _v3_lit_12_b365cad16314ea29 = {"array-empty?", 12, 1};
static const string _v3_lit_12_2dcf4e18024c4ff = {"array-length", 12, 1};
static const string _v3_lit_10_e608bb067e3196f9 = {"array-pop!", 10, 1};
static const string _v3_lit_30_32bfc0712bd0270 = {"array-pop!: the array is empty", 30, 1};
static const string _v3_lit_11_6b3ca7bc363d8f9e = {"array-push!", 11, 1};
static const string _v3_lit_9_e763840bbe806fc0 = {"array-ref", 9, 1};
static const string _v3_lit_10_c9c964e9f2b280dc = {"array-set!", 10, 1};
static const string _v3_lit_18_832fcc2f64f63c6e = {"array-set!: index ", 18, 1};
static const string _v3_lit_6_b83d802db439c705 = {"array?", 6, 1};
static const string _v3_lit_4_511736baa561c828 = {"as->", 4, 1};
static const string _v3_lit_6_82fb670e19860b15 = {"assert", 6, 1};
static const string _v3_lit_67_5b68be3d2d032bfc = {"assert needs a test and a message: (assert test \"what it promises\")", 67, 1};
static const string _v3_lit_18_c7a689d4563e2c68 = {"assertion failed: ", 18, 1};
static const string _v3_lit_74_82fc39ccb1421152 = {"attempted to decode too many bytes, utf-8 is limited to four bytes maximum", 74, 1};
static const string _v3_lit_9_17f117cc78c576b2 = {"backspace", 9, 1};
static const string _v3_lit_4_d032deaf8b5514c2 = {"bad ", 4, 1};
static const string _v3_lit_18_f9e7af49166a001c = {"bad hex character ", 18, 1};
static const string _v3_lit_9_d9e8bcd8436389a2 = {"basics ok", 9, 1};
static const string _v3_lit_5_178c76c75eded44 = {"begin", 5, 1};
static const string _v3_lit_4_43492fafcd383414 = {"blip", 4, 1};
static const string _v3_lit_8_d64d97486c8170b7 = {"blip.lip", 8, 1};
static const string _v3_lit_12_e11aadbed58b02ce = {"blip_modules", 12, 1};
static const string _v3_lit_8_f9a8cb4c8fdf71a = {"boolean?", 8, 1};
static const string _v3_lit_13_9fb914b03e330f4 = {"cannot apply ", 13, 1};
static const string _v3_lit_15_3e6c6899639e6bb5 = {"cannot compare ", 15, 1};
static const string _v3_lit_34_92d522d4317d7079 = {"cannot load the standard library: ", 34, 1};
static const string _v3_lit_12_db692466de09c1a1 = {"cannot look ", 12, 1};
static const string _v3_lit_12_35e90eecdfa1b04 = {"cannot read ", 12, 1};
static const string _v3_lit_18_49c6dcceeec4ec08 = {"cannot read field ", 18, 1};
static const string _v3_lit_3_cf45a85103385d33 = {"car", 3, 1};
static const string _v3_lit_24_6883408d9e1856d4 = {"car expects a pair, got ", 24, 1};
static const string _v3_lit_15_2c9b217c089b5e6a = {"carriage-return", 15, 1};
static const string _v3_lit_4_6bb733a878c4261f = {"case", 4, 1};
static const string _v3_lit_5_67b5345556f15ee = {"catch", 5, 1};
static const string _v3_lit_3_cf50be5103428a1a = {"cdr", 3, 1};
static const string _v3_lit_24_64ef0b93792d50d3 = {"cdr expects a pair, got ", 24, 1};
static const string _v3_lit_12_4a3428e3086109ec = {"code->string", 12, 1};
static const string _v3_lit_14_3bcf118c76ef1ee6 = {"code->string: ", 14, 1};
static const string _v3_lit_28_4e65924173c525b4 = {"coeficients can not be empty", 28, 1};
static const string _v3_lit_6_e15e17627c0748db = {"concat", 6, 1};
static const string _v3_lit_4_e6cd82a8beffa2b7 = {"cond", 4, 1};
static const string _v3_lit_6_f9ae4a62895365b0 = {"cond->", 6, 1};
static const string _v3_lit_4_e6cd8fa8beffb8ce = {"cons", 4, 1};
static const string _v3_lit_53_3310abdbe6ff9916 = {"continuation too deep: non-tail recursion is too deep", 53, 1};
static const string _v3_lit_5_cdc802bc54e68fc2 = {"count", 5, 1};
static const string _v3_lit_7_2154afe82494e5a3 = {"data ok", 7, 1};
static const string _v3_lit_5_6decc255050b9c6a = {"debug", 5, 1};
static const string _v3_lit_3_cadb6512638c859 = {"dec", 3, 1};
static const string _v3_lit_3_cadb9512638cd72 = {"def", 3, 1};
static const string _v3_lit_7_5e6ac746f2e375f4 = {"default", 7, 1};
static const string _v3_lit_6_db03959661e15c48 = {"define", 6, 1};
static const string _v3_lit_19_d2b65a373f8ec725 = {"define needs a name", 19, 1};
static const string _v3_lit_45_b8ce7a3d8367c527 = {"define needs a name or a (name . params) list", 45, 1};
static const string _v3_lit_25_7560fb28c8864d70 = {"define needs a value for ", 25, 1};
static const string _v3_lit_8_b71566fdd941000e = {"defmacro", 8, 1};
static const string _v3_lit_21_2d82929f086f423 = {"defmacro needs a name", 21, 1};
static const string _v3_lit_79_aaafad96d483022c = {"defmacro needs a name, a parameter list and a body: (defmacro name (args) body)", 79, 1};
static const string _v3_lit_7_416d70fd06c69f3 = {"display", 7, 1};
static const string _v3_lit_7_14d41c1622031397 = {"dissoc!", 7, 1};
static const string _v3_lit_30_569fddacb49dafda = {"dissoc! expects a buffer, got ", 30, 1};
static const string _v3_lit_16_394a2244e9c390d = {"division by zero", 16, 1};
static const string _v3_lit_2_9bbd5000c66a209e = {"do", 2, 1};
static const string _v3_lit_7_7cc31e91bc6bb38c = {"dotimes", 7, 1};
static const string _v3_lit_41_da173502be11c611 = {"dotimes needs a loop variable and a count", 41, 1};
static const string _v3_lit_49_e04cb13e2395bd5c = {"dotimes needs a symbol as its loop variable, got ", 49, 1};
static const string _v3_lit_4_8b35ade463598efa = {"drop", 4, 1};
static const string _v3_lit_4_f11acfd93b3db69a = {"echo", 4, 1};
static const string _v3_lit_4_6160ced97abcf88a = {"else", 4, 1};
static const string _v3_lit_3_22ee9511fc1c372 = {"eq?", 3, 1};
static const string _v3_lit_3_2249c511fb8ec16 = {"err", 3, 1};
static const string _v3_lit_9_d6b952147854006e = {"err-value", 9, 1};
static const string _v3_lit_4_5d21c6d8e73949ab = {"err?", 4, 1};
static const string _v3_lit_5_7abebb90e73faeeb = {"error", 5, 1};
static const string _v3_lit_7_ac197bc1ded81c19 = {"error: ", 7, 1};
static const string _v3_lit_9_1bca237c921ab45a = {"errors ok", 9, 1};
static const string _v3_lit_6_5397a7f2858c1824 = {"escape", 6, 1};
static const string _v3_lit_5_9960f1b44cf39360 = {"even?", 5, 1};
static const string _v3_lit_6_4179c85f24cdc55b = {"every?", 6, 1};
static const string _v3_lit_22_e9e95c16b32022c7 = {"examples/01_basics.lip", 22, 1};
static const string _v3_lit_20_a4b2332d308e0a8b = {"examples/02_data.lip", 20, 1};
static const string _v3_lit_21_5111a6e7d2346929 = {"examples/03_match.lip", 21, 1};
static const string _v3_lit_22_cd3666ff372783de = {"examples/04_errors.lip", 22, 1};
static const string _v3_lit_25_d97014f86e8d8d59 = {"examples/05_pipelines.lip", 25, 1};
static const string _v3_lit_23_f5eada8bb525a2f6 = {"examples/06_modules.lip", 23, 1};
static const string _v3_lit_24_62055c9e291ef283 = {"examples/lib/geometry.vl", 24, 1};
static const string _v3_lit_20_5e518be6b676c02e = {"examples/lib/text.vl", 20, 1};
static const string _v3_lit_27_681a01cfa2cc9426 = {"examples: all checks passed", 27, 1};
static const string _v3_lit_23_2f94490da85bb157 = {"expected a number, got ", 23, 1};
static const string _v3_lit_25_a047628c58e9d373 = {"expected a sequence, got ", 25, 1};
static const string _v3_lit_4_b426a9d918a0133a = {"expt", 4, 1};
static const string _v3_lit_43_86e2cf4fa9fe552f = {"expt: a negative exponent is not an integer", 43, 1};
static const string _v3_lit_5_cbbfd4228091f9d2 = {"false", 5, 1};
static const string _v3_lit_6_a176b8975d542a87 = {"field ", 6, 1};
static const string _v3_lit_6_cd094ce5950ab6d = {"filter", 6, 1};
static const string _v3_lit_7_3fd9af264aa12b2 = {"finally", 7, 1};
static const string _v3_lit_5_65c222e025703097 = {"first", 5, 1};
static const string _v3_lit_14_f795d06ca952a863 = {"flatten-result", 14, 1};
static const string _v3_lit_16_7c66430ba35aae1 = {"flatten-result: ", 16, 1};
static const string _v3_lit_6_3dfb6893450ba188 = {"float?", 6, 1};
static const string _v3_lit_2_9bb67100c6643a03 = {"fn", 2, 1};
static const string _v3_lit_7_460556b70604cd06 = {"fntion?", 7, 1};
static const string _v3_lit_4_2ca5b0d314618bb0 = {"fold", 4, 1};
static const string _v3_lit_8_99cfadc3c25d49ec = {"for-each", 8, 1};
static const string _v3_lit_6_2ed40dcc7fa0e8ac = {"format", 6, 1};
static const string _v3_lit_11_c464b6e9ce3d5a2b = {"frequencies", 11, 1};
static const string _v3_lit_9_21a00140dab37ed4 = {"function?", 9, 1};
static const string _v3_lit_1_44bd8cd473cd9c6c = {"g", 1, 1};
static const string _v3_lit_3_f03f6e51156ca17b = {"gcd", 3, 1};
static const string _v3_lit_6_95dd72fbb0a05d3e = {"gensym", 6, 1};
static const string _v3_lit_3_f02b6251155be40d = {"get", 3, 1};
static const string _v3_lit_25_96e8caa6cc63a7ee = {"get expects a table, got ", 25, 1};
static const string _v3_lit_5_fd5599a4af821b6f = {"get: ", 5, 1};
static const string _v3_lit_8_138cdc6d5832847c = {"has-key?", 8, 1};
static const string _v3_lit_2_9bc77700c672b768 = {"if", 2, 1};
static const string _v3_lit_60_55597e79ecc5f0ec = {"if needs a test, a consequent, and optionally an alternative", 60, 1};
static const string _v3_lit_6_dc2ba458304519da = {"import", 6, 1};
static const string _v3_lit_48_cd0a1070476a5d81 = {"import needs a package name: (import owner/repo)", 48, 1};
static const string _v3_lit_25_c9afc074fc527c46 = {"import: no package named ", 25, 1};
static const string _v3_lit_3_268b8d5134d6c11d = {"in?", 3, 1};
static const string _v3_lit_5_d9b5b78965f8ba9f = {"in?: ", 5, 1};
static const string _v3_lit_3_268b515134d65b29 = {"inc", 3, 1};
static const string _v3_lit_13_ff3a948ac618be44 = {"integer->rune", 13, 1};
static const string _v3_lit_15_128eb81327dc728e = {"integer->rune: ", 15, 1};
static const string _v3_lit_8_3c2942aa0a336e44 = {"integer?", 8, 1};
static const string _v3_lit_44_7e766e4cc095bb70 = {"interface method IError.code not implemented", 44, 1};
static const string _v3_lit_43_9edf8799b50a549c = {"interface method IError.msg not implemented", 43, 1};

static const string _v3_lit_54_503bb76740bf2bb9 = {"interface method hash__Hash.block_size not implemented", 54, 1};
static const string _v3_lit_48_fb91f183b903b558 = {"interface method hash__Hash.free not implemented", 48, 1};
static const string _v3_lit_49_eb073e2877ead767 = {"interface method hash__Hash.reset not implemented", 49, 1};
static const string _v3_lit_48_e55a586fc0c6b73f = {"interface method hash__Hash.size not implemented", 48, 1};
static const string _v3_lit_47_67a5effad7253ea3 = {"interface method hash__Hash.sum not implemented", 47, 1};
static const string _v3_lit_49_2678ade7cd32896b = {"interface method hash__Hash.write not implemented", 49, 1};
static const string _v3_lit_53_95bddd17dfe45e44 = {"interface method hash__Hash32er.sum32 not implemented", 53, 1};
static const string _v3_lit_53_5f2d8f78123176b0 = {"interface method hash__Hash64er.sum64 not implemented", 53, 1};
static const string _v3_lit_53_b4772ecdc886c3bb = {"interface method host__Host.host_load not implemented", 53, 1};
static const string _v3_lit_54_9c60a14a32d2d278 = {"interface method host__Host.host_print not implemented", 54, 1};
static const string _v3_lit_58_6bd0654b1f8d11c0 = {"interface method host__Host.host_read_line not implemented", 58, 1};
static const string _v3_lit_54_4453cb1ef63c7430 = {"interface method host__Host.host_write not implemented", 54, 1};
static const string _v3_lit_35_9c5d37ee5f7cf9bf = {"is it installed? `blip pkg install ", 35, 1};
static const string _v3_lit_32_8b11618169752c9e = {"is it installed? `blip pkg list`", 32, 1};
static const string _v3_lit_4_90ee9eeb4064e302 = {"keep", 4, 1};
static const string _v3_lit_7_6cd5b4518f37a3de = {"keyword", 7, 1};
static const string _v3_lit_8_26c547965b8b7753 = {"keyword?", 8, 1};
static const string _v3_lit_6_497dd6d1afd39ae4 = {"lambda", 6, 1};
static const string _v3_lit_29_7c3e0b3741c9508f = {"lambda needs a parameter list", 29, 1};
static const string _v3_lit_4_e63a3d26b79c66a3 = {"last", 4, 1};
static const string _v3_lit_15_8ac87f40b4d713dc = {"lazy-map-result", 15, 1};
static const string _v3_lit_11_66673f96f53ffd17 = {"lazy-unwrap", 11, 1};
static const string _v3_lit_6_e34931c25b6d0e4b = {"length", 6, 1};
static const string _v3_lit_33_f7c2dace43731374 = {"length expects a collection, got ", 33, 1};
static const string _v3_lit_3_51f91f514d780114 = {"let", 3, 1};
static const string _v3_lit_55_2851f1c5138787eb = {"let assert failed: the value does not match the pattern", 55, 1};
static const string _v3_lit_38_4b983c01eebf672d = {"let assert needs a pattern and a value", 38, 1};
static const string _v3_lit_61_88d7c75c655d88b5 = {"let assert takes exactly one binding, written [PATTERN VALUE]", 61, 1};
static const string _v3_lit_4_c2517526a2ea1c5a = {"let*", 4, 1};
static const string _v3_lit_10_88fb1f45907c858d = {"let-assert", 10, 1};
static const string _v3_lit_6_3ab33e7aab478c62 = {"letrec", 6, 1};
static const string _v3_lit_11_1477ee6af240911f = {"lib/std.lip", 11, 1};
static const string _v3_lit_8_c411c491539069e9 = {"linefeed", 8, 1};
static const string _v3_lit_4_a1c80526911571eb = {"list", 4, 1};
static const string _v3_lit_14_39c556772144adc4 = {"list is cyclic", 14, 1};
static const string _v3_lit_8_b823e33ad085a91b = {"list-ref", 8, 1};
static const string _v3_lit_16_525bca778f711e8d = {"list-ref: index ", 16, 1};
static const string _v3_lit_5_fc52948887706b3c = {"list?", 5, 1};
static const string _v3_lit_4_8fd1b32686ba76f1 = {"loop", 4, 1};
static const string _v3_lit_5_80da626fc95d20a3 = {"macex", 5, 1};
static const string _v3_lit_45_ec3a369cd5570360 = {"macex needs one form: (macex (macro arg ...))", 45, 1};
static const string _v3_lit_6_5035d5f3293e5816 = {"macex1", 6, 1};
static const string _v3_lit_47_6e22e270bd1e517e = {"macex1 needs one form: (macex1 (macro arg ...))", 47, 1};
static const string _v3_lit_7_89ea4c2f1503ad21 = {"macex: ", 7, 1};
static const string _v3_lit_80_a86204f5fc7b735f = {"macex: still a macro after 8 expansions; a macro is probably expanding to itself", 80, 1};
static const string _v3_lit_33_28fc7a98c0faab94 = {"machine: unexpected continuation ", 33, 1};
static const string _v3_lit_3_46d7e451467792f3 = {"map", 3, 1};
static const string _v3_lit_10_3611d2ccdddbcd03 = {"map-result", 10, 1};
static const string _v3_lit_5_1118636f8a4d8f74 = {"match", 5, 1};
static const string _v3_lit_45_31eec0c4f5819572 = {"match needs a subject and at least one clause", 45, 1};
static const string _v3_lit_8_9861f7f3fb131506 = {"match ok", 8, 1};
static const string _v3_lit_6_5a00548801ca9cba = {"match*", 6, 1};
static const string _v3_lit_7_b8f61d1b0b1a53de = {"match: ", 7, 1};
static const string _v3_lit_26_26b2f1e3ea09f6f7 = {"match: #:when needs a test", 26, 1};
static const string _v3_lit_69_71265a5bfb06d239 = {"match: (struct Name field: p ...) needs a name and at least one field", 69, 1};
static const string _v3_lit_60_a3cd2806f6be2de7 = {"match: a struct pattern field must be written field: pattern", 60, 1};
static const string _v3_lit_43_f7fea9b678468802 = {"match: a struct pattern needs a struct name", 43, 1};
static const string _v3_lit_41_a323e527c200fee3 = {"match: a table pattern needs keyword keys", 41, 1};
static const string _v3_lit_13_e1bc7ef34ca037fe = {"match: field ", 13, 1};
static const string _v3_lit_8_27a5590a3bdfcf8b = {"math-cos", 8, 1};
static const string _v3_lit_36_8a85b00e1213c801 = {"math-cos expects 1 argument(s), got ", 36, 1};
static const string _v3_lit_31_a6649bcc4ab17db1 = {"math-cos expects a number, got ", 31, 1};
static const string _v3_lit_8_398ab20a462c6337 = {"math-exp", 8, 1};
static const string _v3_lit_36_d785998cdf901405 = {"math-exp expects 1 argument(s), got ", 36, 1};
static const string _v3_lit_31_66d385078f94cf7d = {"math-exp expects a number, got ", 31, 1};
static const string _v3_lit_14_afe623133454a3db = {"math-factorial", 14, 1};
static const string _v3_lit_42_baf19a2703724a51 = {"math-factorial expects 1 argument(s), got ", 42, 1};
static const string _v3_lit_37_c544af9ea31da3c1 = {"math-factorial expects a number, got ", 37, 1};
static const string _v3_lit_10_e83816589e31d81e = {"math-floor", 10, 1};
static const string _v3_lit_38_7c054a37c9cfac7c = {"math-floor expects 1 argument(s), got ", 38, 1};
static const string _v3_lit_33_2aab8fb06abeedfe = {"math-floor expects a number, got ", 33, 1};
static const string _v3_lit_8_e9de4e0a188ed4fa = {"math-log", 8, 1};
static const string _v3_lit_36_909523b1cc652040 = {"math-log expects 1 argument(s), got ", 36, 1};
static const string _v3_lit_31_5368a6cbb4b6915a = {"math-log expects a number, got ", 31, 1};
static const string _v3_lit_10_970b56823f25c3f3 = {"math-log10", 10, 1};
static const string _v3_lit_38_32f525d2d0ad85b9 = {"math-log10 expects 1 argument(s), got ", 38, 1};
static const string _v3_lit_33_c4b6c246fd7989d9 = {"math-log10 expects a number, got ", 33, 1};
static const string _v3_lit_9_f3936327bab38fd8 = {"math-log2", 9, 1};
static const string _v3_lit_37_a81874c9f6ec81be = {"math-log2 expects 1 argument(s), got ", 37, 1};
static const string _v3_lit_32_ac2ae2b11cba18a0 = {"math-log2 expects a number, got ", 32, 1};
static const string _v3_lit_8_b075f40a88dd5902 = {"math-sin", 8, 1};
static const string _v3_lit_36_254c287a1deef78 = {"math-sin expects 1 argument(s), got ", 36, 1};
static const string _v3_lit_31_3d4714e1b1905342 = {"math-sin expects a number, got ", 31, 1};
static const string _v3_lit_3_46d7ec514677a08b = {"max", 3, 1};
static const string _v3_lit_3_46bd02514660fe35 = {"min", 3, 1};
static const string _v3_lit_10_3f6d726fb6f18026 = {"modules ok", 10, 1};
static const string _v3_lit_6_791c3c9e9fbc336d = {"modulo", 6, 1};
static const string _v3_lit_14_202ccf22284dba64 = {"modulo by zero", 14, 1};
static const string _v3_lit_7_e35bc820a2302fe7 = {"mutable", 7, 1};
static const string _v3_lit_9_703f5f434a1181a9 = {"negative?", 9, 1};
static const string _v3_lit_7_41566b4aaab421d9 = {"newline", 7, 1};
static const string _v3_lit_3_3ff5d95143129022 = {"nil", 3, 1};
static const string _v3_lit_4_c1506214f88ae147 = {"nil?", 4, 1};
static const string _v3_lit_34_86d0a58855fc88f5 = {"no such file in the capture host: ", 34, 1};
static const string _v3_lit_4_8e37b614dbd95475 = {"none", 4, 1};
static const string _v3_lit_3_3fe1cd514301d2b4 = {"not", 3, 1};
static const string _v3_lit_22_6057764df73d5b94 = {"not expects 1 argument", 22, 1};
static const string _v3_lit_12_a29f3038d3233718 = {"not-a-result", 12, 1};
static const string _v3_lit_4_8e826a14dc18becb = {"not=", 4, 1};
static const string _v3_lit_3_3f96b95142c1c53e = {"nul", 3, 1};
static const string _v3_lit_4_cee237146f424a56 = {"null", 4, 1};
static const string _v3_lit_5_ccae00b90da4706b = {"null?", 5, 1};
static const string _v3_lit_14_6fc210c22fa1fe34 = {"number->string", 14, 1};
static const string _v3_lit_7_1917d0780d948be7 = {"number?", 7, 1};
static const string _v3_lit_4_ec87890da6744229 = {"odd?", 4, 1};
static const string _v3_lit_2_9bce5400c6789a9d = {"ok", 2, 1};
static const string _v3_lit_5_9aa8c9aed04546bf = {"ok   ", 5, 1};
static const string _v3_lit_8_ab91311039da47a1 = {"ok-value", 8, 1};
static const string _v3_lit_3_38335f513eeec146 = {"ok?", 3, 1};
static const string _v3_lit_6_47233efd4b05df50 = {"opaque", 6, 1};
static const string _v3_lit_2_9bce5b00c678a682 = {"or", 2, 1};
static const string _v3_lit_25_1766e2162d456e94 = {"output does not contain \"", 25, 1};
static const string _v3_lit_5_674f3934a3e137f4 = {"pair?", 5, 1};
static const string _v3_lit_5_2a80483481630ae0 = {"param", 5, 1};
static const string _v3_lit_21_158169dd6b184c52 = {"pattern did not match", 21, 1};
static const string _v3_lit_32_d03b5797a75a96c8 = {"pattern nested more than 40 deep", 32, 1};
static const string _v3_lit_12_9517155e21b85148 = {"pipelines ok", 12, 1};
static const string _v3_lit_9_4fa1b00d71d70d01 = {"positive?", 9, 1};
static const string _v3_lit_5_40dc52cfa0615582 = {"print", 5, 1};
static const string _v3_lit_10_e4092a7435a3d745 = {"procedure?", 10, 1};
static const string _v3_lit_7_1e67ecb0563e6794 = {"provide", 7, 1};
static const string _v3_lit_3_7264b3515f33eb20 = {"put", 3, 1};
static const string _v3_lit_4_9507b444c53852b3 = {"put!", 4, 1};
static const string _v3_lit_27_468a469409c03a06 = {"put! expects a buffer, got ", 27, 1};
static const string _v3_lit_5_e49fbd9ccfe8a8c5 = {"quote", 5, 1};
static const string _v3_lit_8_f651b3be55260426 = {"quotient", 8, 1};
static const string _v3_lit_16_5a44d6ce93cd17ff = {"quotient by zero", 16, 1};
static const string _v3_lit_1_44bd99d473cdb283 = {"r", 1, 1};
static const string _v3_lit_2_9bdf1600c686a476 = {"r1", 2, 1};
static const string _v3_lit_5_d42707209fbb037d = {"raise", 5, 1};
static const string _v3_lit_41_e4a84d4f6da833ae = {"raise: an (err ...) value is not readable", 41, 1};
static const string _v3_lit_8_d4bf6126de53209 = {"raised: ", 8, 1};
static const string _v3_lit_5_96f5e4207ce9b718 = {"range", 5, 1};
static const string _v3_lit_30_c719a115a4b85e5f = {"rational with zero denominator", 30, 1};
static const string _v3_lit_9_cae270c2d09c1b24 = {"read-line", 9, 1};
static const string _v3_lit_6_31de9ec2d2adf2e5 = {"reduce", 6, 1};
static const string _v3_lit_8_c8558d5fe1f56c57 = {"reduce: ", 8, 1};
static const string _v3_lit_64_7817c198e5ca3986 = {"reduce: the sequence is empty, so there is nothing to start from", 64, 1};
static const string _v3_lit_6_126715d46b46abf8 = {"reject", 6, 1};
static const string _v3_lit_9_15e2e1252626de9e = {"remainder", 9, 1};
static const string _v3_lit_17_5ada4d5fb12d4be7 = {"remainder by zero", 17, 1};
static const string _v3_lit_6_13e8210fb3d5dc0f = {"remove", 6, 1};
static const string _v3_lit_7_83d47bb3d9452c4e = {"require", 7, 1};
static const string _v3_lit_9_933254007b6730f4 = {"require: ", 9, 1};
static const string _v3_lit_27_53207dbea77125b7 = {"require: no file path given", 27, 1};
static const string _v3_lit_4_47ee913776e3c785 = {"rest", 4, 1};
static const string _v3_lit_6_7488c53f48adf9a1 = {"return", 6, 1};
static const string _v3_lit_7_780c2660960ff787 = {"reverse", 7, 1};
static const string _v3_lit_13_33cef20612002766 = {"rune->integer", 13, 1};
static const string _v3_lit_15_368f10ef53a5181c = {"rune->integer: ", 15, 1};
static const string _v3_lit_5_5c8f2ec760f92884 = {"rune?", 5, 1};
static const string _v3_lit_1_44bd98d473cdb0d0 = {"s", 1, 1};
static const string _v3_lit_4_669eb42cb843a8e0 = {"set!", 4, 1};
static const string _v3_lit_42_d129e9fc51bfae69 = {"set! cannot assign to unbound identifier: ", 42, 1};
static const string _v3_lit_17_e78e6b6945ac097d = {"set! needs a name", 17, 1};
static const string _v3_lit_4_bd51b32ce96497db = {"sort", 4, 1};
static const string _v3_lit_7_3e2e49b6ad015f3b = {"sort-by", 7, 1};
static const string _v3_lit_33_4dcffec9525ab545 = {"sort-by expects 2 arguments, got ", 33, 1};
static const string _v3_lit_9_3d1169acaa77e3c9 = {"sort-by: ", 9, 1};
static const string _v3_lit_5_a5593b4a24922a2f = {"space", 5, 1};
static const string _v3_lit_4_b8fad92c55cca545 = {"sqrt", 4, 1};
static const string _v3_lit_25_65ae29a1c0743a99 = {"sqrt of a negative number", 25, 1};
static const string _v3_lit_50_40465beb1f522fb7 = {"step limit exceeded: the program did not terminate", 50, 1};
static const string _v3_lit_3_58d84e5150db0b3a = {"str", 3, 1};
static const string _v3_lit_6_3850125edc81f186 = {"string", 6, 1};
static const string _v3_lit_15_eb9647638504e0dc = {"string->keyword", 15, 1};
static const string _v3_lit_14_80ba372c0e6c3078 = {"string->number", 14, 1};
static const string _v3_lit_12_e8852f9b97575ce5 = {"string->rune", 12, 1};
static const string _v3_lit_52_9de7b53f6cd64471 = {"string->rune: expected a one-character string, got \"", 52, 1};
static const string _v3_lit_14_68acb05b19e75529 = {"string->symbol", 14, 1};
static const string _v3_lit_13_c2dbf1d17ad22d5 = {"string-append", 13, 1};
static const string _v3_lit_13_bade188409f1efde = {"string-blank?", 13, 1};
static const string _v3_lit_16_35bef6f51e6ad421 = {"string-contains?", 16, 1};
static const string _v3_lit_15_74b513a653904005 = {"string-downcase", 15, 1};
static const string _v3_lit_15_7b8cf9c1a55e8d5f = {"string-index-of", 15, 1};
static const string _v3_lit_11_84f1169aeef50035 = {"string-join", 11, 1};
static const string _v3_lit_13_1cc9ab73c965e0a9 = {"string-length", 13, 1};
static const string _v3_lit_17_e361026ca9583b4e = {"string-not-blank?", 17, 1};
static const string _v3_lit_13_b200ce5844ed589e = {"string-repeat", 13, 1};
static const string _v3_lit_14_81b652afad6b830f = {"string-replace", 14, 1};
static const string _v3_lit_12_7c1279d672f973a5 = {"string-slice", 12, 1};
static const string _v3_lit_12_9e98aa3ad92dd337 = {"string-split", 12, 1};
static const string _v3_lit_18_150f5d003c9d16b = {"string-start-with?", 18, 1};
static const string _v3_lit_11_bac2bbe2efd88755 = {"string-trim", 11, 1};
static const string _v3_lit_13_c9486c3e7201d588 = {"string-upcase", 13, 1};
static const string _v3_lit_7_3200f030b0cdbd5b = {"string?", 7, 1};
static const string _v3_lit_6_14da8f5ec8315a0a = {"struct", 6, 1};
static const string _v3_lit_7_a0bbc80e2bdc355e = {"struct ", 7, 1};
static const string _v3_lit_19_d4c40bc705adc33f = {"struct needs a name", 19, 1};
static const string _v3_lit_59_9011271f10096872 = {"struct needs a name and a field list: (struct Name (f1 f2))", 59, 1};
static const string _v3_lit_11_c29ca55ac7deafa6 = {"struct-name", 11, 1};
static const string _v3_lit_13_2537d281d799aa5c = {"struct-name: ", 13, 1};
static const string _v3_lit_10_999ae46be131259f = {"struct-out", 10, 1};
static const string _v3_lit_10_98c096c20686822 = {"struct-ref", 10, 1};
static const string _v3_lit_12_986253de95362a38 = {"struct-ref: ", 12, 1};
static const string _v3_lit_11_819a09b4259b9006 = {"struct-set!", 11, 1};
static const string _v3_lit_13_17da5e6a1747173c = {"struct-set!: ", 13, 1};
static const string _v3_lit_7_a0bbd30e2bdc480f = {"struct?", 7, 1};
static const string _v3_lit_6_ee267171343aae33 = {"symbol", 6, 1};
static const string _v3_lit_7_e600cf5bbfb5be64 = {"symbol?", 7, 1};
static const string _v3_lit_3_96797d51740b73ba = {"tab", 3, 1};
static const string _v3_lit_10_737a68d3e82b9fdc = {"table-keys", 10, 1};
static const string _v3_lit_6_1138efb0bf19762e = {"table?", 6, 1};
static const string _v3_lit_4_bbcbbc682f613010 = {"take", 4, 1};
static const string _v3_lit_3_96796b51740b5524 = {"tap", 3, 1};
static const string _v3_lit_4_6018685605485a = {"time", 4, 1};
static const string _v3_lit_4_4b66a767efc7182f = {"true", 4, 1};
static const string _v3_lit_3_964d645173e60e54 = {"try", 3, 1};
static const string _v3_lit_16_2402f99f8f5fbd1e = {"try needs a body", 16, 1};
static const string _v3_lit_10_bd4095b378303a32 = {"try-result", 10, 1};
static const string _v3_lit_7_9dfcf628eb4e3267 = {"try: #:", 7, 1};
static const string _v3_lit_20_f0a7b475d116260d = {"unbound identifier: ", 20, 1};
static const string _v3_lit_9_43e38cec2213a79c = {"unclosed ", 9, 1};
static const string _v3_lit_18_7be5ba2484ac0e22 = {"unexpected byte 0x", 18, 1};
static const string _v3_lit_28_e400dbba838a731d = {"unimplemented special form: ", 28, 1};
static const string _v3_lit_16_73711fb201e24a45 = {"unknown # syntax", 16, 1};
static const string _v3_lit_23_b8de0cace035bb6d = {"unknown character name ", 23, 1};
static const string _v3_lit_19_f69465e8a0a70318 = {"unknown primitive: ", 19, 1};
static const string _v3_lit_6_7969d416ee3eef9d = {"unless", 6, 1};
static const string _v3_lit_19_1138739c22f89302 = {"unterminated string", 19, 1};
static const string _v3_lit_26_eb4f1b78b183db39 = {"unterminated string escape", 26, 1};
static const string _v3_lit_21_85aa735d3d99a4db = {"unterminated |symbol|", 21, 1};
static const string _v3_lit_9_fc7de50eaad59b9e = {"unwrap-or", 9, 1};
static const string _v3_lit_3_8ee02351700ae052 = {"use", 3, 1};
static const string _v3_lit_37_e9d8ca360177a979 = {"use needs a call: (use (f x) body...)", 37, 1};
static const string _v3_lit_6_a652a2d3592ea6e0 = {"vector", 6, 1};
static const string _v3_lit_13_d9cb45d4644cb55f = {"vector-append", 13, 1};
static const string _v3_lit_16_d4090a67ad28683 = {"vector-contains?", 16, 1};
static const string _v3_lit_13_de3bb69e93d86c41 = {"vector-empty?", 13, 1};
static const string _v3_lit_13_e95a08079b20c597 = {"vector-length", 13, 1};
static const string _v3_lit_10_f8e88b4c9f2cc737 = {"vector-map", 10, 1};
static const string _v3_lit_10_413a1a4c36a20cb8 = {"vector-ref", 10, 1};
static const string _v3_lit_18_bb3b2b7df22a85f2 = {"vector-ref: index ", 18, 1};
static const string _v3_lit_10_413a2a4c36a227e8 = {"vector-rev", 10, 1};
static const string _v3_lit_7_cd118c208a458ced = {"vector?", 7, 1};
static const string _v3_lit_4_ca78e650452716cf = {"when", 4, 1};
static const string _v3_lit_4_4973567a2a000355 = {"zero", 4, 1};
static const string _v3_lit_5_ceff5b955e05cd1e = {"zero?", 5, 1};
static const string _v3_lit_1_44bda0d473cdbe68 = {"{", 1, 1};
static const string _v3_lit_2_9c0ea100c6af0169 = {"|>", 2, 1};
static const string _v3_lit_1_44bda6d473cdc89a = {"}", 1, 1};
static const string _v3_lit_20_9af357b6465f2ccc = {"~a does not match ~a", 20, 1};

string ArrayFlags__autostr(ArrayFlags it) {
	int __fe_v = (int)it;
	string __fe_res = (string){.str = (u8*)"ArrayFlags{", .len = 11, .is_lit = 1};
	bool __fe_first = true;
	if (ArrayFlags__noslices != 0 && (__fe_v & (int)ArrayFlags__noslices) == (int)ArrayFlags__noslices) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".noslices", .len = 9, .is_lit = 1});
		__fe_first = false;
	}
	if (ArrayFlags__noshrink != 0 && (__fe_v & (int)ArrayFlags__noshrink) == (int)ArrayFlags__noshrink) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".noshrink", .len = 9, .is_lit = 1});
		__fe_first = false;
	}
	if (ArrayFlags__nogrow != 0 && (__fe_v & (int)ArrayFlags__nogrow) == (int)ArrayFlags__nogrow) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".nogrow", .len = 7, .is_lit = 1});
		__fe_first = false;
	}
	if (ArrayFlags__nofree != 0 && (__fe_v & (int)ArrayFlags__nofree) == (int)ArrayFlags__nofree) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".nofree", .len = 7, .is_lit = 1});
		__fe_first = false;
	}
	if (ArrayFlags__managed != 0 && (__fe_v & (int)ArrayFlags__managed) == (int)ArrayFlags__managed) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".managed", .len = 8, .is_lit = 1});
		__fe_first = false;
	}
	if (ArrayFlags__noscan_data != 0 && (__fe_v & (int)ArrayFlags__noscan_data) == (int)ArrayFlags__noscan_data) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".noscan_data", .len = 12, .is_lit = 1});
		__fe_first = false;
	}
	if (ArrayFlags__is_slice != 0 && (__fe_v & (int)ArrayFlags__is_slice) == (int)ArrayFlags__is_slice) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".is_slice", .len = 9, .is_lit = 1});
		__fe_first = false;
	}
	__fe_res = string__plus(__fe_res, (string){.str = (u8*)"}", .len = 1, .is_lit = 1});
	return __fe_res;
}

string ChanState__autostr(ChanState it) {
	if (it == ChanState__success) return (string){.str = (u8*)"success", .len = 7, .is_lit = 1};
	if (it == ChanState__not_ready) return (string){.str = (u8*)"not_ready", .len = 9, .is_lit = 1};
	if (it == ChanState__closed) return (string){.str = (u8*)"closed", .len = 6, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string GraphemeBreakProperty__autostr(GraphemeBreakProperty it) {
	if (it == GraphemeBreakProperty__other) return (string){.str = (u8*)"other", .len = 5, .is_lit = 1};
	if (it == GraphemeBreakProperty__cr) return (string){.str = (u8*)"cr", .len = 2, .is_lit = 1};
	if (it == GraphemeBreakProperty__lf) return (string){.str = (u8*)"lf", .len = 2, .is_lit = 1};
	if (it == GraphemeBreakProperty__control) return (string){.str = (u8*)"control", .len = 7, .is_lit = 1};
	if (it == GraphemeBreakProperty__extend) return (string){.str = (u8*)"extend", .len = 6, .is_lit = 1};
	if (it == GraphemeBreakProperty__regional_indicator) return (string){.str = (u8*)"regional_indicator", .len = 18, .is_lit = 1};
	if (it == GraphemeBreakProperty__prepend) return (string){.str = (u8*)"prepend", .len = 7, .is_lit = 1};
	if (it == GraphemeBreakProperty__spacing_mark) return (string){.str = (u8*)"spacing_mark", .len = 12, .is_lit = 1};
	if (it == GraphemeBreakProperty__l) return (string){.str = (u8*)"l", .len = 1, .is_lit = 1};
	if (it == GraphemeBreakProperty__v) return (string){.str = (u8*)"v", .len = 1, .is_lit = 1};
	if (it == GraphemeBreakProperty__t) return (string){.str = (u8*)"t", .len = 1, .is_lit = 1};
	if (it == GraphemeBreakProperty__lv) return (string){.str = (u8*)"lv", .len = 2, .is_lit = 1};
	if (it == GraphemeBreakProperty__lvt) return (string){.str = (u8*)"lvt", .len = 3, .is_lit = 1};
	if (it == GraphemeBreakProperty__zwj) return (string){.str = (u8*)"zwj", .len = 3, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string AttributeKind__autostr(AttributeKind it) {
	if (it == AttributeKind__plain) return (string){.str = (u8*)"plain", .len = 5, .is_lit = 1};
	if (it == AttributeKind__string) return (string){.str = (u8*)"string", .len = 6, .is_lit = 1};
	if (it == AttributeKind__number) return (string){.str = (u8*)"number", .len = 6, .is_lit = 1};
	if (it == AttributeKind__bool) return (string){.str = (u8*)"bool", .len = 4, .is_lit = 1};
	if (it == AttributeKind__comptime_define) return (string){.str = (u8*)"comptime_define", .len = 15, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string MapMode__autostr(MapMode it) {
	if (it == MapMode__to_upper) return (string){.str = (u8*)"to_upper", .len = 8, .is_lit = 1};
	if (it == MapMode__to_lower) return (string){.str = (u8*)"to_lower", .len = 8, .is_lit = 1};
	if (it == MapMode__to_title) return (string){.str = (u8*)"to_title", .len = 8, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string TrimMode__autostr(TrimMode it) {
	if (it == TrimMode__trim_left) return (string){.str = (u8*)"trim_left", .len = 9, .is_lit = 1};
	if (it == TrimMode__trim_right) return (string){.str = (u8*)"trim_right", .len = 10, .is_lit = 1};
	if (it == TrimMode__trim_both) return (string){.str = (u8*)"trim_both", .len = 9, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string strings__IndentState__autostr(strings__IndentState it) {
	if (it == strings__IndentState__normal) return (string){.str = (u8*)"normal", .len = 6, .is_lit = 1};
	if (it == strings__IndentState__in_string) return (string){.str = (u8*)"in_string", .len = 9, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string strconv__Align_text__autostr(strconv__Align_text it) {
	if (it == strconv__Align_text__right) return (string){.str = (u8*)"right", .len = 5, .is_lit = 1};
	if (it == strconv__Align_text__left) return (string){.str = (u8*)"left", .len = 4, .is_lit = 1};
	if (it == strconv__Align_text__center) return (string){.str = (u8*)"center", .len = 6, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string strconv__ParserState__autostr(strconv__ParserState it) {
	if (it == strconv__ParserState__ok) return (string){.str = (u8*)"ok", .len = 2, .is_lit = 1};
	if (it == strconv__ParserState__pzero) return (string){.str = (u8*)"pzero", .len = 5, .is_lit = 1};
	if (it == strconv__ParserState__mzero) return (string){.str = (u8*)"mzero", .len = 5, .is_lit = 1};
	if (it == strconv__ParserState__pinf) return (string){.str = (u8*)"pinf", .len = 4, .is_lit = 1};
	if (it == strconv__ParserState__minf) return (string){.str = (u8*)"minf", .len = 4, .is_lit = 1};
	if (it == strconv__ParserState__invalid_number) return (string){.str = (u8*)"invalid_number", .len = 14, .is_lit = 1};
	if (it == strconv__ParserState__extra_char) return (string){.str = (u8*)"extra_char", .len = 10, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string strconv__Char_parse_state__autostr(strconv__Char_parse_state it) {
	if (it == strconv__Char_parse_state__start) return (string){.str = (u8*)"start", .len = 5, .is_lit = 1};
	if (it == strconv__Char_parse_state__norm_char) return (string){.str = (u8*)"norm_char", .len = 9, .is_lit = 1};
	if (it == strconv__Char_parse_state__field_char) return (string){.str = (u8*)"field_char", .len = 10, .is_lit = 1};
	if (it == strconv__Char_parse_state__pad_ch) return (string){.str = (u8*)"pad_ch", .len = 6, .is_lit = 1};
	if (it == strconv__Char_parse_state__len_set_start) return (string){.str = (u8*)"len_set_start", .len = 13, .is_lit = 1};
	if (it == strconv__Char_parse_state__len_set_in) return (string){.str = (u8*)"len_set_in", .len = 10, .is_lit = 1};
	if (it == strconv__Char_parse_state__check_type) return (string){.str = (u8*)"check_type", .len = 10, .is_lit = 1};
	if (it == strconv__Char_parse_state__check_float) return (string){.str = (u8*)"check_float", .len = 11, .is_lit = 1};
	if (it == strconv__Char_parse_state__check_float_in) return (string){.str = (u8*)"check_float_in", .len = 14, .is_lit = 1};
	if (it == strconv__Char_parse_state__reset_params) return (string){.str = (u8*)"reset_params", .len = 12, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string os__WalkDirAction__autostr(os__WalkDirAction it) {
	if (it == os__WalkDirAction__proceed) return (string){.str = (u8*)"proceed", .len = 7, .is_lit = 1};
	if (it == os__WalkDirAction__skip_dir) return (string){.str = (u8*)"skip_dir", .len = 8, .is_lit = 1};
	if (it == os__WalkDirAction__stop) return (string){.str = (u8*)"stop", .len = 4, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string os__ProcessState__autostr(os__ProcessState it) {
	if (it == os__ProcessState__not_started) return (string){.str = (u8*)"not_started", .len = 11, .is_lit = 1};
	if (it == os__ProcessState__running) return (string){.str = (u8*)"running", .len = 7, .is_lit = 1};
	if (it == os__ProcessState__stopped) return (string){.str = (u8*)"stopped", .len = 7, .is_lit = 1};
	if (it == os__ProcessState__exited) return (string){.str = (u8*)"exited", .len = 6, .is_lit = 1};
	if (it == os__ProcessState__aborted) return (string){.str = (u8*)"aborted", .len = 7, .is_lit = 1};
	if (it == os__ProcessState__closed) return (string){.str = (u8*)"closed", .len = 6, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string os__Signal__autostr(os__Signal it) {
	if (it == os__Signal__hup) return (string){.str = (u8*)"hup", .len = 3, .is_lit = 1};
	if (it == os__Signal__v_int) return (string){.str = (u8*)"int", .len = 3, .is_lit = 1};
	if (it == os__Signal__quit) return (string){.str = (u8*)"quit", .len = 4, .is_lit = 1};
	if (it == os__Signal__ill) return (string){.str = (u8*)"ill", .len = 3, .is_lit = 1};
	if (it == os__Signal__trap) return (string){.str = (u8*)"trap", .len = 4, .is_lit = 1};
	if (it == os__Signal__abrt) return (string){.str = (u8*)"abrt", .len = 4, .is_lit = 1};
	if (it == os__Signal__bus) return (string){.str = (u8*)"bus", .len = 3, .is_lit = 1};
	if (it == os__Signal__fpe) return (string){.str = (u8*)"fpe", .len = 3, .is_lit = 1};
	if (it == os__Signal__kill) return (string){.str = (u8*)"kill", .len = 4, .is_lit = 1};
	if (it == os__Signal__usr1) return (string){.str = (u8*)"usr1", .len = 4, .is_lit = 1};
	if (it == os__Signal__segv) return (string){.str = (u8*)"segv", .len = 4, .is_lit = 1};
	if (it == os__Signal__usr2) return (string){.str = (u8*)"usr2", .len = 4, .is_lit = 1};
	if (it == os__Signal__v_pipe) return (string){.str = (u8*)"pipe", .len = 4, .is_lit = 1};
	if (it == os__Signal__alrm) return (string){.str = (u8*)"alrm", .len = 4, .is_lit = 1};
	if (it == os__Signal__term) return (string){.str = (u8*)"term", .len = 4, .is_lit = 1};
	if (it == os__Signal__stkflt) return (string){.str = (u8*)"stkflt", .len = 6, .is_lit = 1};
	if (it == os__Signal__chld) return (string){.str = (u8*)"chld", .len = 4, .is_lit = 1};
	if (it == os__Signal__cont) return (string){.str = (u8*)"cont", .len = 4, .is_lit = 1};
	if (it == os__Signal__stop) return (string){.str = (u8*)"stop", .len = 4, .is_lit = 1};
	if (it == os__Signal__tstp) return (string){.str = (u8*)"tstp", .len = 4, .is_lit = 1};
	if (it == os__Signal__ttin) return (string){.str = (u8*)"ttin", .len = 4, .is_lit = 1};
	if (it == os__Signal__ttou) return (string){.str = (u8*)"ttou", .len = 4, .is_lit = 1};
	if (it == os__Signal__urg) return (string){.str = (u8*)"urg", .len = 3, .is_lit = 1};
	if (it == os__Signal__xcpu) return (string){.str = (u8*)"xcpu", .len = 4, .is_lit = 1};
	if (it == os__Signal__xfsz) return (string){.str = (u8*)"xfsz", .len = 4, .is_lit = 1};
	if (it == os__Signal__vtalrm) return (string){.str = (u8*)"vtalrm", .len = 6, .is_lit = 1};
	if (it == os__Signal__prof) return (string){.str = (u8*)"prof", .len = 4, .is_lit = 1};
	if (it == os__Signal__winch) return (string){.str = (u8*)"winch", .len = 5, .is_lit = 1};
	if (it == os__Signal__poll) return (string){.str = (u8*)"poll", .len = 4, .is_lit = 1};
	if (it == os__Signal__pwr) return (string){.str = (u8*)"pwr", .len = 3, .is_lit = 1};
	if (it == os__Signal__sys) return (string){.str = (u8*)"sys", .len = 3, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string os__SeekMode__autostr(os__SeekMode it) {
	if (it == os__SeekMode__start) return (string){.str = (u8*)"start", .len = 5, .is_lit = 1};
	if (it == os__SeekMode__current) return (string){.str = (u8*)"current", .len = 7, .is_lit = 1};
	if (it == os__SeekMode__end) return (string){.str = (u8*)"end", .len = 3, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string os__FileBufferMode__autostr(os__FileBufferMode it) {
	if (it == os__FileBufferMode__fully_buffered) return (string){.str = (u8*)"fully_buffered", .len = 14, .is_lit = 1};
	if (it == os__FileBufferMode__line_buffered) return (string){.str = (u8*)"line_buffered", .len = 13, .is_lit = 1};
	if (it == os__FileBufferMode__not_buffered) return (string){.str = (u8*)"not_buffered", .len = 12, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string os__FileType__autostr(os__FileType it) {
	if (it == os__FileType__unknown) return (string){.str = (u8*)"unknown", .len = 7, .is_lit = 1};
	if (it == os__FileType__regular) return (string){.str = (u8*)"regular", .len = 7, .is_lit = 1};
	if (it == os__FileType__directory) return (string){.str = (u8*)"directory", .len = 9, .is_lit = 1};
	if (it == os__FileType__character_device) return (string){.str = (u8*)"character_device", .len = 16, .is_lit = 1};
	if (it == os__FileType__block_device) return (string){.str = (u8*)"block_device", .len = 12, .is_lit = 1};
	if (it == os__FileType__fifo) return (string){.str = (u8*)"fifo", .len = 4, .is_lit = 1};
	if (it == os__FileType__symbolic_link) return (string){.str = (u8*)"symbolic_link", .len = 13, .is_lit = 1};
	if (it == os__FileType__socket) return (string){.str = (u8*)"socket", .len = 6, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string os__ChildProcessPipeKind__autostr(os__ChildProcessPipeKind it) {
	if (it == os__ChildProcessPipeKind__v_stdin) return (string){.str = (u8*)"stdin", .len = 5, .is_lit = 1};
	if (it == os__ChildProcessPipeKind__v_stdout) return (string){.str = (u8*)"stdout", .len = 6, .is_lit = 1};
	if (it == os__ChildProcessPipeKind__v_stderr) return (string){.str = (u8*)"stderr", .len = 6, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string blip__Tag__autostr(blip__Tag it) {
	int __fe_v = (int)it;
	string __fe_res = (string){.str = (u8*)"Tag{", .len = 4, .is_lit = 1};
	bool __fe_first = true;
	if (blip__Tag__nil != 0 && (__fe_v & (int)blip__Tag__nil) == (int)blip__Tag__nil) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".nil", .len = 4, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__boolean != 0 && (__fe_v & (int)blip__Tag__boolean) == (int)blip__Tag__boolean) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".boolean", .len = 8, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__integer != 0 && (__fe_v & (int)blip__Tag__integer) == (int)blip__Tag__integer) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".integer", .len = 8, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__v_float != 0 && (__fe_v & (int)blip__Tag__v_float) == (int)blip__Tag__v_float) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".float", .len = 6, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__rune != 0 && (__fe_v & (int)blip__Tag__rune) == (int)blip__Tag__rune) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".rune", .len = 5, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__string != 0 && (__fe_v & (int)blip__Tag__string) == (int)blip__Tag__string) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".string", .len = 7, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__symbol != 0 && (__fe_v & (int)blip__Tag__symbol) == (int)blip__Tag__symbol) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".symbol", .len = 7, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__keyword != 0 && (__fe_v & (int)blip__Tag__keyword) == (int)blip__Tag__keyword) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".keyword", .len = 8, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__pair != 0 && (__fe_v & (int)blip__Tag__pair) == (int)blip__Tag__pair) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".pair", .len = 5, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__emptylist != 0 && (__fe_v & (int)blip__Tag__emptylist) == (int)blip__Tag__emptylist) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".emptylist", .len = 10, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__vector != 0 && (__fe_v & (int)blip__Tag__vector) == (int)blip__Tag__vector) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".vector", .len = 7, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__array != 0 && (__fe_v & (int)blip__Tag__array) == (int)blip__Tag__array) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".array", .len = 6, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__table != 0 && (__fe_v & (int)blip__Tag__table) == (int)blip__Tag__table) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".table", .len = 6, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__buffer != 0 && (__fe_v & (int)blip__Tag__buffer) == (int)blip__Tag__buffer) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".buffer", .len = 7, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__closure != 0 && (__fe_v & (int)blip__Tag__closure) == (int)blip__Tag__closure) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".closure", .len = 8, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__primitive != 0 && (__fe_v & (int)blip__Tag__primitive) == (int)blip__Tag__primitive) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".primitive", .len = 10, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__continuation != 0 && (__fe_v & (int)blip__Tag__continuation) == (int)blip__Tag__continuation) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".continuation", .len = 13, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__Tag__struct_ != 0 && (__fe_v & (int)blip__Tag__struct_) == (int)blip__Tag__struct_) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".struct_", .len = 8, .is_lit = 1});
		__fe_first = false;
	}
	__fe_res = string__plus(__fe_res, (string){.str = (u8*)"}", .len = 1, .is_lit = 1});
	return __fe_res;
}

string blip__KontTag__autostr(blip__KontTag it) {
	int __fe_v = (int)it;
	string __fe_res = (string){.str = (u8*)"KontTag{", .len = 8, .is_lit = 1};
	bool __fe_first = true;
	if (blip__KontTag__done != 0 && (__fe_v & (int)blip__KontTag__done) == (int)blip__KontTag__done) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".done", .len = 5, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__KontTag__app_fn != 0 && (__fe_v & (int)blip__KontTag__app_fn) == (int)blip__KontTag__app_fn) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".app_fn", .len = 7, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__KontTag__app_arg != 0 && (__fe_v & (int)blip__KontTag__app_arg) == (int)blip__KontTag__app_arg) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".app_arg", .len = 8, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__KontTag__if_k != 0 && (__fe_v & (int)blip__KontTag__if_k) == (int)blip__KontTag__if_k) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".if_k", .len = 5, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__KontTag__seq != 0 && (__fe_v & (int)blip__KontTag__seq) == (int)blip__KontTag__seq) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".seq", .len = 4, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__KontTag__define_k != 0 && (__fe_v & (int)blip__KontTag__define_k) == (int)blip__KontTag__define_k) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".define_k", .len = 9, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__KontTag__set_k != 0 && (__fe_v & (int)blip__KontTag__set_k) == (int)blip__KontTag__set_k) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".set_k", .len = 6, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__KontTag__and_k != 0 && (__fe_v & (int)blip__KontTag__and_k) == (int)blip__KontTag__and_k) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".and_k", .len = 6, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__KontTag__or_k != 0 && (__fe_v & (int)blip__KontTag__or_k) == (int)blip__KontTag__or_k) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".or_k", .len = 5, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__KontTag__field_k != 0 && (__fe_v & (int)blip__KontTag__field_k) == (int)blip__KontTag__field_k) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".field_k", .len = 8, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__KontTag__assert_k != 0 && (__fe_v & (int)blip__KontTag__assert_k) == (int)blip__KontTag__assert_k) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".assert_k", .len = 9, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__KontTag__use_k != 0 && (__fe_v & (int)blip__KontTag__use_k) == (int)blip__KontTag__use_k) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".use_k", .len = 6, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__KontTag__match_k != 0 && (__fe_v & (int)blip__KontTag__match_k) == (int)blip__KontTag__match_k) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".match_k", .len = 8, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__KontTag__guard_k != 0 && (__fe_v & (int)blip__KontTag__guard_k) == (int)blip__KontTag__guard_k) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".guard_k", .len = 8, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__KontTag__collect_k != 0 && (__fe_v & (int)blip__KontTag__collect_k) == (int)blip__KontTag__collect_k) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".collect_k", .len = 10, .is_lit = 1});
		__fe_first = false;
	}
	if (blip__KontTag__try_k != 0 && (__fe_v & (int)blip__KontTag__try_k) == (int)blip__KontTag__try_k) {
		if (!__fe_first) { __fe_res = string__plus(__fe_res, (string){.str = (u8*)" | ", .len = 3, .is_lit = 1}); }
		__fe_res = string__plus(__fe_res, (string){.str = (u8*)".try_k", .len = 6, .is_lit = 1});
		__fe_first = false;
	}
	__fe_res = string__plus(__fe_res, (string){.str = (u8*)"}", .len = 1, .is_lit = 1});
	return __fe_res;
}

string closure__MemoryProtectAtrr__autostr(closure__MemoryProtectAtrr it) {
	if (it == closure__MemoryProtectAtrr__read_exec) return (string){.str = (u8*)"read_exec", .len = 9, .is_lit = 1};
	if (it == closure__MemoryProtectAtrr__read_write) return (string){.str = (u8*)"read_write", .len = 10, .is_lit = 1};
	return strconv__format_int((i64)it, 10);
}

string reader__DatumTag__autostr(reader__DatumTag it) {
	if (it == reader__DatumTag__nil) return (string){.str = (u8*)"nil", .len = 3, .is_lit = 1};
	if (it == reader__DatumTag__bool) return (string){.str = (u8*)"bool", .len = 4, .is_lit = 1};
	if (it == reader__DatumTag__v_int) return (string){.str = (u8*)"int", .len = 3, .is_lit = 1};
	if (it == reader__DatumTag__rat) return (string){.str = (u8*)"rat", .len = 3, .is_lit = 1};
	if (it == reader__DatumTag__v_float) return (string){.str = (u8*)"float", .len = 5, .is_lit = 1};
	if (it == reader__DatumTag__v_char) return (string){.str = (u8*)"char", .len = 4, .is_lit = 1};
	if (it == reader__DatumTag__str) return (string){.str = (u8*)"str", .len = 3, .is_lit = 1};
	if (it == reader__DatumTag__sym) return (string){.str = (u8*)"sym", .len = 3, .is_lit = 1};
	if (it == reader__DatumTag__kw) return (string){.str = (u8*)"kw", .len = 2, .is_lit = 1};
	if (it == reader__DatumTag__list) return (string){.str = (u8*)"list", .len = 4, .is_lit = 1};
	if (it == reader__DatumTag__vector) return (string){.str = (u8*)"vector", .len = 6, .is_lit = 1};
	if (it == reader__DatumTag__table) return (string){.str = (u8*)"table", .len = 5, .is_lit = 1};
	if (it == reader__DatumTag__array) return (string){.str = (u8*)"array", .len = 5, .is_lit = 1};
	if (it == reader__DatumTag__bytes) return (string){.str = (u8*)"bytes", .len = 5, .is_lit = 1};
	if (it == reader__DatumTag__buffer) return (string){.str = (u8*)"buffer", .len = 6, .is_lit = 1};
	if (it == reader__DatumTag__quoted) return (string){.str = (u8*)"quoted", .len = 6, .is_lit = 1};
	if (it == reader__DatumTag__quasi) return (string){.str = (u8*)"quasi", .len = 5, .is_lit = 1};
	if (it == reader__DatumTag__unquote) return (string){.str = (u8*)"unquote", .len = 7, .is_lit = 1};
	if (it == reader__DatumTag__unquote_splice) return (string){.str = (u8*)"unquote_splice", .len = 14, .is_lit = 1};
	return strconv__format_uint((u64)(u8)it, 10);
}

extern i32 blip__no_default;
extern i32 blip__no_env;
extern i32 blip__no_node;
extern blip__Payload blip__no_payload;
extern IError builtin__none__;
extern i32 machine__no_default;
extern Array math__bernoulli;
extern math__ChebSeries math__cos_cs;
extern Array math__gamma_p;
extern Array math__gamma_q;
extern Array math__lgamma_a;
extern Array math__lgamma_r;
extern Array math__lgamma_s;
extern Array math__lgamma_t;
extern Array math__lgamma_u;
extern Array math__lgamma_v;
extern Array math__lgamma_w;
extern math__ChebSeries math__sin_cs;
extern Array math__tanh_p;
extern Array math__tanh_q;
extern strconv__AtoF64Param prims__strict_float;
extern u8 reader__at_c;
extern u8 reader__backslash;
extern u8 reader__backslash_bs;
extern u8 reader__bar;
extern u8 reader__colon;
extern u8 reader__cr;
extern u8 reader__dot_c;
extern u8 reader__dquote;
extern u8 reader__hash_c;
extern u8 reader__lbrace;
extern u8 reader__lbrack;
extern u8 reader__lparen;
extern u8 reader__minus_c;
extern u8 reader__nl;
extern u8 reader__plus_c;
extern u8 reader__quasi_c;
extern u8 reader__quote_c;
extern u8 reader__rbrace;
extern u8 reader__rbrack;
extern u8 reader__rparen;
extern u8 reader__semi;
extern u8 reader__sp;
extern strconv__AtoF64Param reader__strict_float;
extern u8 reader__tab_c;
extern u8 reader__unquote_c;
extern u8 reader__x_c;
extern u8 reader__zero_c;
#define bits__de_bruijn32 (((u32)(0x077CB531)))
#define bits__de_bruijn64 (((u64)(0x03f79d71b4ca8b09)))
#define bits__m0 (((u64)(0x5555555555555555)))
#define bits__m1 (((u64)(0x3333333333333333)))
#define bits__m2 (((u64)(0x0f0f0f0f0f0f0f0f)))
#define bits__m3 (((u64)(0x00ff00ff00ff00ff)))
#define bits__m4 (((u64)(0x0000ffff0000ffff)))
#define bits__n16 (((u16)(16)))
#define bits__n32 (((u32)(32)))
#define bits__n64 (((u64)(64)))
#define bits__two32 (((u64)(0x100000000)))
#define bits__mask32 ((((u64)(0x100000000))) - (1))
#define strconv__int_size ((((i64)(sizeof(i64)))) * (8))
#define strconv__i64_min_int32 ((((i64)(-(2147483647)))) - (1))
#define strconv__i64_max_int32 ((((i64)(2147483646))) + (1))
#define strconv__mantbits64 (((u32)(52)))
#define strconv__expbits64 (((u32)(11)))
#define strconv__bias64 (1023)
#define strconv__maxexp64 (((u64)(2047)))
#define strconv__rune_error (0xFFFD)
#define strconv__max_rune (0x10FFFF)
#define strconv__pow5_num_bits_32 (61)
#define strconv__pow5_inv_num_bits_32 (59)
#define strconv__pow5_num_bits_64 (121)
#define strconv__pow5_inv_num_bits_64 (122)
#define strconv__single_plus_zero (((u32)(0x00000000)))
#define strconv__single_minus_zero (((u32)(0x80000000)))
#define strconv__single_plus_infinity (((u32)(0x7F800000)))
#define strconv__single_minus_infinity (((u32)(0xFF800000)))
#define strconv__digits (18)
#define strconv__double_plus_zero (((u64)(0x0000000000000000)))
#define strconv__double_minus_zero (((u64)(0x8000000000000000)))
#define strconv__double_plus_infinity (((u64)(0x7FF0000000000000)))
#define strconv__double_minus_infinity (((u64)(0xFFF0000000000000)))
#define strconv__c_dpoint ('.')
#define strconv__c_plus ('+')
#define strconv__c_minus ('-')
#define strconv__c_zero ('0')
#define strconv__c_nine ('9')
#define strconv__c_ten (((u32)(10)))
#define strconv__mantbits32 (((u32)(23)))
#define strconv__expbits32 (((u32)(8)))
#define strconv__bias32 (127)
#define strconv__maxexp32 (255)
#define strconv__max_size_f64_char (512)
#define hash__wyp0 (((u64)(0x2d358dccaa6c78a5)))
#define hash__wyp1 (((u64)(0x8bb84b93962eacc9)))
#define hash__wyp2 (((u64)(0x4b33a62ed433d4a3)))
#define hash__wyp3 (((u64)(0x4d5a2da51de1aa47)))
#define builtin__array_flag_retained_aligned_fixed (((int)((((u32)(1))) << (31))))
#define builtin__autostr_type_stack_max_depth (64)
#define builtin__min_i8 (((i8)(-(128))))
#define builtin__max_i8 (((i8)(127)))
#define builtin__min_i16 (((i16)(-(32768))))
#define builtin__max_i16 (((i16)(32767)))
#define builtin__min_i32 (((i32)(-(2147483648))))
#define builtin__max_i32 (((i32)(2147483647)))
#define builtin__min_i64 (((i64)((-(9223372036854775807)) - (1))))
#define builtin__max_i64 (((i64)(9223372036854775807)))
#define builtin__min_int (((i64)(((i64)((-(9223372036854775807)) - (1))))))
#define builtin__max_int (((i64)(((i64)(9223372036854775807)))))
#define builtin__min_u16 (((u16)(0)))
#define builtin__max_u16 (((u16)(65535)))
#define builtin__min_u32 (((u32)(0)))
#define builtin__max_u32 (((u32)(4294967295)))
#define builtin__min_u64 (((u64)(0)))
#define builtin__max_u64 (((u64)(18446744073709551615)))
#define builtin__hashbits (24)
#define builtin__max_cached_hashbits (16)
#define builtin__init_log_capicity (5)
#define builtin__init_capicity ((1) << (5))
#define builtin__max_load_factor (0.8)
#define builtin__init_even_index (((1) << (5)) - (2))
#define builtin__extra_metas_inc (4)
#define builtin__hash_mask (((u32)(0x00FFFFFF)))
#define builtin__probe_inc (((u32)(0x01000000)))
#define builtin__rune_maps_columns_in_row (4)
#define builtin__rune_maps_ul (-(3))
#define builtin__rune_maps_utl (-(2))
#define builtin__degree (6)
#define builtin__mid_index ((6) - (1))
#define builtin__max_len (((2) * (6)) - (1))
#define builtin__children_bytes ((sizeof(void*)) * (((((2) * (6)) - (1)) + (1))))
#define builtin__replace_stack_buffer_size (10)
#define builtin__max_direct_index_needle_len (16)
#define builtin__kmp_stack_buffer_size (20)
#define builtin__utf8_replacement_rune (((u32)(0xfffd)))
#define builtin__symopt_undname (0x00000002)
#define builtin__symopt_deferred_loads (0x00000004)
#define builtin__symopt_no_cpp (0x00000008)
#define builtin__symopt_load_lines (0x00000010)
#define builtin__symopt_include_32bit_modules (0x00002000)
#define builtin__symopt_allow_zero_address (0x01000000)
#define builtin__symopt_debug (((u32)(0x80000000)))
#define builtin__std_input_handle (((u32)(0xfffffff6)))
#define builtin__std_output_handle (((u32)(0xfffffff5)))
#define builtin__std_error_handle (((u32)(0xfffffff4)))
#define builtin__enable_processed_output (1)
#define builtin__enable_wrap_at_eol_output (2)
#define builtin__enable_virtual_terminal_processing (4)
#define builtin__format_message_allocate_buffer (0x00000100)
#define builtin__format_message_argument_array (0x00002000)
#define builtin__format_message_from_hmodule (0x00000800)
#define builtin__format_message_from_string (0x00000400)
#define builtin__format_message_from_system (0x00001000)
#define builtin__format_message_ignore_inserts (0x00000200)
#define builtin__map_hash_k0 (((u64)(0x2d358dccaa6c78a5)))
#define builtin__map_hash_k1 (((u64)(0x8bb84b93962eacc9)))
#define builtin__map_hash_k2 (((u64)(0x4b33a62ed433d4a3)))
#define builtin__map_hash_k3 (((u64)(0x4d5a2da51de1aa47)))
#define builtin__prealloc_block_size (((16) * (1024)) * (1024))
#define builtin__prealloc_scope_block_size ((256) * (1024))
#define builtin__prealloc_recycle_cache_slots (8)
#define builtin__prealloc_default_align ((sizeof(void*)) * (2))
#define builtin__cp_acp (0)
#define builtin__cp_utf8 (65001)
MessageError builtin__error_sentinel__object = {.msg = {"error", 5, 1}};
IError builtin__error_sentinel = {._typ = 71273906, ._object = &builtin__error_sentinel__object};
#define dl__version (1)
#define dl__rtld_now (0)
#define dl__rtld_lazy (0)
#define dl__rtld_global (0)
#define dl__rtld_local (0)
#define dl__rtld_nodelete (0)
#define dl__rtld_noload (0)
#define os__fslash ('/')
#define os__bslash ('\\')
#define os__dot ('.')
#define os__qmark ('?')
#define os__max_path_len (4096)
#define os__f_ok (0)
#define os__x_ok (1)
#define os__w_ok (2)
#define os__r_ok (4)
#define os__read_lines_chunk_size ((128) * (1024))
#define os__error_code_noent (2)
#define os__error_code_denied (5)
#define os__error_code_perm (5)
#define os__error_code_exist (183)
#define os__error_code_notdir (267)
#define os__max_path_buffer_size ((2) * (4096))
#define os__success (0x0000)
#define os__file_share_read (0x01)
#define os__enable_processed_output (0x01)
#define os__enable_wrap_at_eol_output (0x02)
#define os__enable_virtual_terminal_processing (0x04)
#define os__o_rdonly (0x0000)
#define os__o_wronly (0x0001)
#define os__o_rdwr (0x0002)
#define os__o_append (0x0008)
#define os__o_create (0x0100)
#define os__o_binary (0x8000)
#define os__o_trunc (0x0200)
#define os__o_excl (0x0400)
#define os__o_sync (0x0000)
#define os__o_noctty (0x0000)
#define os__o_nonblock (0x0000)
#define os__key_query_value (0x0001)
#define os__key_set_value (0x0002)
#define os__key_enumerate_sub_keys (0x0008)
#define os__key_wow64_32key (0x0200)
#define os__wm_settingchange (0x001A)
#define os__smto_abortifhung (0x0002)
#define os__buf_size (4096)
#define os__error_code_not_set (((i64)(-(1))))
#define os__windows_file_attribute_reparse_point (((u32)(0x00000400)))
#define os__windows_io_reparse_tag_symlink (((u32)(0xa000000c)))
#define os__windows_filetime_ticks_per_second (((u64)(10000000)))
#define os__windows_filetime_unix_epoch_seconds (((i64)(11644473600)))
#define os__s_ifmt (0xF000)
#define os__s_ifdir (0x4000)
#define os__s_ifreg (0x8000)
#define os__s_iflnk (0xa000)
#define os__s_isuid (04000)
#define os__s_isgid (02000)
#define os__s_isvtx (01000)
#define os__s_irusr (00400)
#define os__s_iwusr (00200)
#define os__s_ixusr (00100)
#define os__s_irgrp (00040)
#define os__s_iwgrp (00020)
#define os__s_ixgrp (00010)
#define os__s_iroth (00004)
#define os__s_iwoth (00002)
#define os__s_ixoth (00001)
#define os__format_message_allocate_buffer (0x00000100)
#define os__format_message_argument_array (0x00002000)
#define os__format_message_from_hmodule (0x00000800)
#define os__format_message_from_string (0x00000400)
#define os__format_message_from_system (0x00001000)
#define os__format_message_ignore_inserts (0x00000200)
#define os__sublang_neutral (0x00)
#define os__sublang_default (0x01)
#define os__lang_neutral (0x00)
#define os__max_error_code (15841)
#define os__fd_stdout (1)
#define os__fd_stderr (2)
#define internal__f64_epsilon (2.2204460492503131e-16)
#define internal__sqrt_f64_epsilon (1.4901161193847656e-08)
#define internal__root3_f64_epsilon (6.0554544523933429e-06)
#define internal__root4_f64_epsilon (1.2207031250000000e-04)
#define internal__root5_f64_epsilon (7.4009597974140505e-04)
#define internal__root6_f64_epsilon (2.4607833005759251e-03)
#define internal__log_f64_epsilon (-(3.6043653389117154e+01))
#define internal__f64_min (2.2250738585072014e-308)
#define internal__sqrt_f64_min (1.4916681462400413e-154)
#define internal__root3_f64_min (2.8126442852362996e-103)
#define internal__root4_f64_min (1.2213386697554620e-77)
#define internal__root5_f64_min (2.9476022969691763e-62)
#define internal__root6_f64_min (5.3034368905798218e-52)
#define internal__log_f64_min (-(7.0839641853226408e+02))
#define internal__f64_max (1.7976931348623157e+308)
#define internal__sqrt_f64_max (1.3407807929942596e+154)
#define internal__root3_f64_max (5.6438030941222897e+102)
#define internal__root4_f64_max (1.1579208923731620e+77)
#define internal__root5_f64_max (4.4765466227572707e+61)
#define internal__root6_f64_max (2.3756689782295612e+51)
#define internal__log_f64_max (7.0978271289338397e+02)
#define internal__f32_epsilon (1.1920928955078125e-07)
#define internal__sqrt_f32_epsilon (3.4526698300124393e-04)
#define internal__root3_f32_epsilon (4.9215666011518501e-03)
#define internal__root4_f32_epsilon (1.8581361171917516e-02)
#define internal__root5_f32_epsilon (4.1234622211652937e-02)
#define internal__root6_f32_epsilon (7.0153878019335827e-02)
#define internal__log_f32_epsilon (-(1.5942385152878742e+01))
#define internal__f32_min (1.1754943508222875e-38)
#define internal__sqrt_f32_min (1.0842021724855044e-19)
#define internal__root3_f32_min (2.2737367544323241e-13)
#define internal__root4_f32_min (3.2927225399135965e-10)
#define internal__root5_f32_min (2.5944428542140822e-08)
#define internal__root6_f32_min (4.7683715820312542e-07)
#define internal__log_f32_min (-(8.7336544750553102e+01))
#define internal__f32_max (3.4028234663852886e+38)
#define internal__sqrt_f32_max (1.8446743523953730e+19)
#define internal__root3_f32_max (6.9814635196223242e+12)
#define internal__root4_f32_max (4.2949672319999986e+09)
#define internal__root5_f32_max (5.0859007855960041e+07)
#define internal__root6_f32_max (2.6422459233807749e+06)
#define internal__log_f32_max (8.8722839052068352e+01)
#define internal__sflt_epsilon (4.8828125000000000e-04)
#define internal__sqrt_sflt_epsilon (2.2097086912079612e-02)
#define internal__root3_sflt_epsilon (7.8745065618429588e-02)
#define internal__root4_sflt_epsilon (1.4865088937534013e-01)
#define internal__root5_sflt_epsilon (2.1763764082403100e-01)
#define internal__root6_sflt_epsilon (2.8061551207734325e-01)
#define internal__log_sflt_epsilon (-(7.6246189861593985e+00))
#define internal__max_int_fact_arg (170)
#define internal__max_f64_fact_arg (171.0)
#define internal__max_long_f64_fact_arg (1755.5)
#define math__uvnan (((u64)(0x7FF8000000000001)))
#define math__uvinf (((u64)(0x7FF0000000000000)))
#define math__uvneginf (((u64)(0xFFF0000000000000)))
#define math__uvone (((u64)(0x3FF0000000000000)))
#define math__mask (0x7FF)
#define math__shift (((64) - (11)) - (1))
#define math__bias (1023)
#define math__normalize_smallest_mask (((u64)((((u64)(1))) << (52))))
#define math__sign_mask (((u64)(0x8000000000000000)))
#define math__frac_mask (((u64)((((((u64)(1))) << (((u64)(((64) - (11)) - (1)))))) - (((u64)(1))))))
#define math__epsilon (2.2204460492503130808472633361816E-16)
#define math__e (2.71828182845904523536028747135266249775724709369995957496696763)
#define math__pi (3.14159265358979323846264338327950288419716939937510582097494459)
#define math__pi_2 ((3.14159265358979323846264338327950288419716939937510582097494459) / (2.0))
#define math__pi_4 ((3.14159265358979323846264338327950288419716939937510582097494459) / (4.0))
#define math__phi (1.61803398874989484820458683436563811772030917980576286213544862)
#define math__tau (6.28318530717958647692528676655900576839433879875021164194988918)
#define math__one_over_tau ((1.0) / (6.28318530717958647692528676655900576839433879875021164194988918))
#define math__one_over_pi ((1.0) / (3.14159265358979323846264338327950288419716939937510582097494459))
#define math__tau_over2 ((6.28318530717958647692528676655900576839433879875021164194988918) / (2.0))
#define math__tau_over4 ((6.28318530717958647692528676655900576839433879875021164194988918) / (4.0))
#define math__tau_over8 ((6.28318530717958647692528676655900576839433879875021164194988918) / (8.0))
#define math__sqrt2 (1.41421356237309504880168872420969807856967187537694807317667974)
#define math__sqrt_3 (1.73205080756887729352744634150587236694280525381038062805580697)
#define math__sqrt_5 (2.23606797749978969640917366873127623544061835961152572427089724)
#define math__sqrt_e (1.64872127070012814684865078781416357165377610071014801157507931)
#define math__sqrt_pi (1.77245385090551602729816748334114518279754945612238712821380779)
#define math__sqrt_tau (2.50662827463100050241576528481104525300698674060993831662992357)
#define math__sqrt_phi (1.27201964951406896425242246173749149171560804184009624861664038)
#define math__ln2 (0.693147180559945309417232121458176568075500134360255254120680009)
#define math__log2_e ((1.0) / (0.693147180559945309417232121458176568075500134360255254120680009))
#define math__ln10 (2.30258509299404568401799145468436420760110148862877297603332790)
#define math__log10_e ((1.0) / (2.30258509299404568401799145468436420760110148862877297603332790))
#define math__two_thirds (0.66666666666666666666666666666666666666666666666666666666666667)
#define math__max_f32 (3.40282346638528859811704183484516925440e+38)
#define math__smallest_non_zero_f32 (1.401298464324817070923729583289916131280e-45)
#define math__max_f64 (1.797693134862315708145274237317043567981e+308)
#define math__smallest_non_zero_f64 (4.940656458412465441765687928682213723651e-324)
#define math__erx (8.45062911510467529297e-01)
#define math__efx (1.28379167095512586316e-01)
#define math__efx8 (1.02703333676410069053e+00)
#define math__pp0 (1.28379167095512558561e-01)
#define math__pp1 (-(3.25042107247001499370e-01))
#define math__pp2 (-(2.84817495755985104766e-02))
#define math__pp3 (-(5.77027029648944159157e-03))
#define math__pp4 (-(2.37630166566501626084e-05))
#define math__qq1 (3.97917223959155352819e-01)
#define math__qq2 (6.50222499887672944485e-02)
#define math__qq3 (5.08130628187576562776e-03)
#define math__qq4 (1.32494738004321644526e-04)
#define math__qq5 (-(3.96022827877536812320e-06))
#define math__pa0 (-(2.36211856075265944077e-03))
#define math__pa1 (4.14856118683748331666e-01)
#define math__pa2 (-(3.72207876035701323847e-01))
#define math__pa3 (3.18346619901161753674e-01)
#define math__pa4 (-(1.10894694282396677476e-01))
#define math__pa5 (3.54783043256182359371e-02)
#define math__pa6 (-(2.16637559486879084300e-03))
#define math__qa1 (1.06420880400844228286e-01)
#define math__qa2 (5.40397917702171048937e-01)
#define math__qa3 (7.18286544141962662868e-02)
#define math__qa4 (1.26171219808761642112e-01)
#define math__qa5 (1.36370839120290507362e-02)
#define math__qa6 (1.19844998467991074170e-02)
#define math__ra0 (-(9.86494403484714822705e-03))
#define math__ra1 (-(6.93858572707181764372e-01))
#define math__ra2 (-(1.05586262253232909814e+01))
#define math__ra3 (-(6.23753324503260060396e+01))
#define math__ra4 (-(1.62396669462573470355e+02))
#define math__ra5 (-(1.84605092906711035994e+02))
#define math__ra6 (-(8.12874355063065934246e+01))
#define math__ra7 (-(9.81432934416914548592e+00))
#define math__sa1 (1.96512716674392571292e+01)
#define math__sa2 (1.37657754143519042600e+02)
#define math__sa3 (4.34565877475229228821e+02)
#define math__sa4 (6.45387271733267880336e+02)
#define math__sa5 (4.29008140027567833386e+02)
#define math__sa6 (1.08635005541779435134e+02)
#define math__sa7 (6.57024977031928170135e+00)
#define math__sa8 (-(6.04244152148580987438e-02))
#define math__rb0 (-(9.86494292470009928597e-03))
#define math__rb1 (-(7.99283237680523006574e-01))
#define math__rb2 (-(1.77579549177547519889e+01))
#define math__rb3 (-(1.60636384855821916062e+02))
#define math__rb4 (-(6.37566443368389627722e+02))
#define math__rb5 (-(1.02509513161107724954e+03))
#define math__rb6 (-(4.83519191608651397019e+02))
#define math__sb1 (3.03380607434824582924e+01)
#define math__sb2 (3.25792512996573918826e+02)
#define math__sb3 (1.53672958608443695994e+03)
#define math__sb4 (3.19985821950859553908e+03)
#define math__sb5 (2.55305040643316442583e+03)
#define math__sb6 (4.74528541206955367215e+02)
#define math__sb7 (-(2.24409524465858183362e+01))
#define math__f64_max_exp (((double)(((double)(1024)))))
#define math__f64_min_exp (((double)(((double)(-(1021))))))
#define math__threshold (7.09782712893383973096e+02)
#define math__ln2_x56 (3.88162421113569373274e+01)
#define math__ln2_halfx3 (1.03972077083991796413e+00)
#define math__ln2_half (3.46573590279972654709e-01)
#define math__ln2hi (6.93147180369123816490e-01)
#define math__ln2lo (1.90821492927058770002e-10)
#define math__inv_ln2 (1.44269504088896338700e+00)
#define math__expm1_q1 (-(3.33333333333331316428e-02))
#define math__expm1_q2 (1.58730158725481460165e-03)
#define math__expm1_q3 (-(7.93650757867487942473e-05))
#define math__expm1_q4 (4.00821782732936239552e-06)
#define math__expm1_q5 (-(2.01099218183624371326e-07))
#define math__log_sqrt_2pi (9.18938533204672741780329736e-1)
#define math__morebits (6.123233995736765886130e-17)
#define math__tan3pio8 (2.41421356237309504880)
#define math__two54 (((double)(1.80143985094819840000e+16)))
#define math__ivln10 (((double)(4.34294481903251816668e-01)))
#define math__log10_2hi (((double)(3.01029995663611771306e-01)))
#define math__log10_2lo (((double)(3.69423907715893078616e-13)))
#define math__modf_maxpowtwo (4.503599627370496000e+15)
#define math__tan_dp1 (7.853981554508209228515625e-1)
#define math__tan_dp2 (7.94662735614792836714e-9)
#define math__tan_dp3 (3.06161699786838294307e-17)
#define math__tan_lossth (1.073741824e+9)
#define closure__assumed_page_size (((i64)(0x4000)))
#define closure__ppc64_architecture (((i64)(11)))

/* V3CACHE_BODY_BEGIN */
/* V3CACHE_MODULE __v3_program_support */
void _vinit() {
	gc_runtime_init();
#if (defined(_VGCBOEHM) || defined(CUSTOM_DEFINE_gcboehm)) && defined(GC_THREADS)
	GC_allow_register_threads();
#endif
	__v3_cache_bits__init_consts_defaults();
	__v3_cache_strings__init_consts_defaults();
	__v3_cache_strconv__init_consts_defaults();
	__v3_cache_blip__init_consts_defaults();
	__v3_cache_hash__init_consts_defaults();
	__v3_cache_builtin__init_consts_defaults();
	__v3_cache_closure__init_consts_defaults();
	__v3_cache_dl__init_consts_defaults();
	__v3_cache_textscanner__init_consts_defaults();
	__v3_cache_validate__init_consts_defaults();
	__v3_cache_os__init_consts_defaults();
	__v3_cache_host__init_consts_defaults();
	__v3_cache_internal__init_consts_defaults();
	__v3_cache_math__init_consts_defaults();
	__v3_cache_reader__init_consts_defaults();
	__v3_cache_printer__init_consts_defaults();
	__v3_cache_prims__init_consts_defaults();
	__v3_cache_machine__init_consts_defaults();
	__v3_cache_bits__init_consts();
	__v3_cache_strings__init_consts();
	__v3_cache_strconv__init_consts();
	__v3_cache_blip__init_consts();
	__v3_cache_hash__init_consts();
	__v3_cache_builtin__init_consts();
	builtin_init();
	__v3_cache_closure__init_consts();
	__v3_cache_dl__init_consts();
	__v3_cache_textscanner__init_consts();
	__v3_cache_validate__init_consts();
	__v3_cache_os__init_consts();
	__v3_cache_host__init_consts();
	__v3_cache_internal__init_consts();
	__v3_cache_math__init_consts();
	__v3_cache_reader__init_consts();
	__v3_cache_printer__init_consts();
	__v3_cache_prims__init_consts();
	__v3_cache_machine__init_consts();
	closure__closure_init();
}

/* V3CACHE_MODULE main */
string IError__msg(IError* i) {
	if (i->_typ == 1938660593) return Error__msg(*(Error*)i->_object);
	if (i->_typ == 71273906) return MessageError__msg(*(MessageError*)i->_object);
	if (i->_typ == 2065246729) return Error__msg(((None__*)i->_object)->Error);
	if (i->_typ == 1743421611) return Error__msg(((os__Eof*)i->_object)->Error);
	if (i->_typ == 227373654) return os__ExecutableNotFoundError__msg(*(os__ExecutableNotFoundError*)i->_object);
	if (i->_typ == 1564013019) return os__FileNotOpenedError__msg(*(os__FileNotOpenedError*)i->_object);
	if (i->_typ == 310309972) return os__NotExpected__msg(*(os__NotExpected*)i->_object);
	if (i->_typ == 721233953) return os__SizeOfTypeIs0Error__msg(*(os__SizeOfTypeIs0Error*)i->_object);
	v_panic(_v3_lit_43_9edf8799b50a549c);
	return (string){0};
}
/* V3CACHE_MODULE main */
i64 IError__code(IError* i) {
	if (i->_typ == 1938660593) return Error__code(*(Error*)i->_object);
	if (i->_typ == 71273906) return MessageError__code(*(MessageError*)i->_object);
	if (i->_typ == 2065246729) return Error__code(((None__*)i->_object)->Error);
	if (i->_typ == 1743421611) return Error__code(((os__Eof*)i->_object)->Error);
	if (i->_typ == 227373654) return Error__code(((os__ExecutableNotFoundError*)i->_object)->Error);
	if (i->_typ == 1564013019) return Error__code(((os__FileNotOpenedError*)i->_object)->Error);
	if (i->_typ == 310309972) return os__NotExpected__code(*(os__NotExpected*)i->_object);
	if (i->_typ == 721233953) return Error__code(((os__SizeOfTypeIs0Error*)i->_object)->Error);
	v_panic(_v3_lit_44_7e766e4cc095bb70);
	return (i64){0};
}
/* V3CACHE_MODULE main */
void host__Host__host_print(host__Host* i, string _a0) {
	switch (i->_typ) {
		case 555330581: host__CaptureHost__host_print((host__CaptureHost*)i->_object, _a0); return;
		case 84543226: host__ConsoleHost__host_print((host__ConsoleHost*)i->_object, _a0); return;
		default: break;
	}
	void* _export = dl__interface_export_find("host__Host", "host_print", i->_typ);
	if (_export != 0) {
		((void (*)(host__Host*, string))_export)(i, _a0);
		return;
	}
	v_panic(_v3_lit_54_9c60a14a32d2d278);
}
/* V3CACHE_MODULE main */
void host__Host__host_write(host__Host* i, string _a0) {
	switch (i->_typ) {
		case 555330581: host__CaptureHost__host_write((host__CaptureHost*)i->_object, _a0); return;
		case 84543226: host__ConsoleHost__host_write((host__ConsoleHost*)i->_object, _a0); return;
		default: break;
	}
	void* _export = dl__interface_export_find("host__Host", "host_write", i->_typ);
	if (_export != 0) {
		((void (*)(host__Host*, string))_export)(i, _a0);
		return;
	}
	v_panic(_v3_lit_54_4453cb1ef63c7430);
}
/* V3CACHE_MODULE main */
__v_option_string host__Host__host_read_line(host__Host* i) {
	switch (i->_typ) {
		case 555330581: return host__CaptureHost__host_read_line((host__CaptureHost*)i->_object);
		case 84543226: return host__ConsoleHost__host_read_line((host__ConsoleHost*)i->_object);
		default: break;
	}
	void* _export = dl__interface_export_find("host__Host", "host_read_line", i->_typ);
	if (_export != 0) {
		return ((__v_option_string (*)(host__Host*))_export)(i);
	}
	v_panic(_v3_lit_58_6bd0654b1f8d11c0);
	return (__v_option_string){0};
}
/* V3CACHE_MODULE main */
__v_result_string host__Host__host_load(host__Host* i, string _a0) {
	switch (i->_typ) {
		case 555330581: return host__CaptureHost__host_load((host__CaptureHost*)i->_object, _a0);
		case 84543226: return host__ConsoleHost__host_load((host__ConsoleHost*)i->_object, _a0);
		default: break;
	}
	void* _export = dl__interface_export_find("host__Host", "host_load", i->_typ);
	if (_export != 0) {
		return ((__v_result_string (*)(host__Host*, string))_export)(i, _a0);
	}
	v_panic(_v3_lit_53_b4772ecdc886c3bb);
	return (__v_result_string){0};
}
/* V3CACHE_MODULE main */
Array hash__Hash__sum(hash__Hash* i, Array _a0) {
	void* _export = dl__interface_export_find("hash__Hash", "sum", i->_typ);
	if (_export != 0) {
		return ((Array (*)(hash__Hash*, Array))_export)(i, _a0);
	}
	v_panic(_v3_lit_47_67a5effad7253ea3);
	return (Array){0};
}
/* V3CACHE_MODULE main */
i64 hash__Hash__size(hash__Hash* i) {
	void* _export = dl__interface_export_find("hash__Hash", "size", i->_typ);
	if (_export != 0) {
		return ((i64 (*)(hash__Hash*))_export)(i);
	}
	v_panic(_v3_lit_48_e55a586fc0c6b73f);
	return (i64){0};
}
/* V3CACHE_MODULE main */
i64 hash__Hash__block_size(hash__Hash* i) {
	void* _export = dl__interface_export_find("hash__Hash", "block_size", i->_typ);
	if (_export != 0) {
		return ((i64 (*)(hash__Hash*))_export)(i);
	}
	v_panic(_v3_lit_54_503bb76740bf2bb9);
	return (i64){0};
}
/* V3CACHE_MODULE main */
void hash__Hash__free(hash__Hash* i) {
	void* _export = dl__interface_export_find("hash__Hash", "free", i->_typ);
	if (_export != 0) {
		((void (*)(hash__Hash*))_export)(i);
		return;
	}
	v_panic(_v3_lit_48_fb91f183b903b558);
}
/* V3CACHE_MODULE main */
void hash__Hash__reset(hash__Hash* i) {
	void* _export = dl__interface_export_find("hash__Hash", "reset", i->_typ);
	if (_export != 0) {
		((void (*)(hash__Hash*))_export)(i);
		return;
	}
	v_panic(_v3_lit_49_eb073e2877ead767);
}
/* V3CACHE_MODULE main */
__v_result_i64 hash__Hash__write(hash__Hash* i, Array _a0) {
	void* _export = dl__interface_export_find("hash__Hash", "write", i->_typ);
	if (_export != 0) {
		return ((__v_result_i64 (*)(hash__Hash*, Array))_export)(i, _a0);
	}
	v_panic(_v3_lit_49_2678ade7cd32896b);
	return (__v_result_i64){0};
}
/* V3CACHE_MODULE main */
u32 hash__Hash32er__sum32(hash__Hash32er* i) {
	void* _export = dl__interface_export_find("hash__Hash32er", "sum32", i->_typ);
	if (_export != 0) {
		return ((u32 (*)(hash__Hash32er*))_export)(i);
	}
	v_panic(_v3_lit_53_95bddd17dfe45e44);
	return (u32){0};
}
/* V3CACHE_MODULE main */
u64 hash__Hash64er__sum64(hash__Hash64er* i) {
	void* _export = dl__interface_export_find("hash__Hash64er", "sum64", i->_typ);
	if (_export != 0) {
		return ((u64 (*)(hash__Hash64er*))_export)(i);
	}
	v_panic(_v3_lit_53_5f2d8f78123176b0);
	return (u64){0};
}

/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN ef7ad775706b0b96 */
string Array_rune__string(Array ra) {
	Array __esc_0 = strings__new_builder(ra.len);
	Array* sb = (Array*)(memdup(&__esc_0, sizeof(Array)));
	strings__Builder__write_runes(sb, ra);
	string res = strings__Builder__str(sb);
	{
		strings__Builder__free(sb);
	}
	return res;
}

/* V3CACHE_FN_END ef7ad775706b0b96 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 6d81238a624cd8be */
void Array_string__free(Array* a) {
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < (*a).len; __for_idx_0++) {
			string* s = (string*)(array_get(*a, __for_idx_0));
			{
				string__free(s);
			}
		}
	}
	array* arr = (array*)(a);
	{
		array__free(arr);
	}
}

/* V3CACHE_FN_END 6d81238a624cd8be */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 8c62bb313c466e98 */
string Array_string__join(Array a, string sep) {
	if (a.len == 0) {
		return _v3_lit_0_14650fb0739d0383;
	}
	i64 len = 0;
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < a.len; __for_idx_0++) {
			string val = *(string*)(array_get(a, __for_idx_0));
			len += val.len + sep.len;
		}
	}
	len -= sep.len;
	string res = (string){.str = malloc_noscan(len + 1), .len = len};
	i64 idx = 0;
	{
		i64 i = 0;
		for (; i < a.len; i++) {
			string val = *(string*)(array_get(a, i));
			{
				vmemcpy((void*)(res.str + idx), (void*)(val.str), val.len);
				idx += val.len;
			}
			if (i != (a.len - 1)) {
				{
					vmemcpy((void*)(res.str + idx), (void*)(sep.str), sep.len);
					idx += sep.len;
				}
			}
		}
	}
	{
		(res.str)[res.len] = 0;
	}
	return res;
}

/* V3CACHE_FN_END 8c62bb313c466e98 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 4a5ab7ffc7e34894 */
inline string Array_string__join_lines(Array s) {
	return Array_string__join(s, _v3_lit_1_44bd31d473cd01cb);
}

/* V3CACHE_FN_END 4a5ab7ffc7e34894 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 3ddfa6603dbe136 */
inline void Array_string__sort_by_len(Array* s) {
	{
		i64 __sort_n_0 = s->len;
		Array __sort_buf_1 = array_new(		sizeof(string), __sort_n_0, __sort_n_0);
		Array __sort_src_2 = *s;
		Array __sort_dst_3 = __sort_buf_1;
		{
			i64 __sort_w_5 = 1;
			for (; __sort_w_5 < __sort_n_0; __sort_w_5 = (i64)(((i64)(			__sort_w_5)) << (1))) {
				{
					i64 __sort_lo_6 = 0;
					for (; __sort_lo_6 < __sort_n_0; ) {
						i64 __sort_mid_7 = __sort_lo_6 + __sort_w_5;
						if (__sort_mid_7 > __sort_n_0) {
							__sort_mid_7 = __sort_n_0;
						}
						i64 __sort_hi_8 = __sort_mid_7 + __sort_w_5;
						if (__sort_hi_8 > __sort_n_0) {
							__sort_hi_8 = __sort_n_0;
						}
						i64 __sort_i_9 = __sort_lo_6;
						i64 __sort_j_10 = __sort_mid_7;
						i64 __sort_k_11 = __sort_lo_6;
						for (; (__sort_i_9 < __sort_mid_7) && (__sort_j_10 < __sort_hi_8); __sort_k_11++) {
							if (compare_strings_by_len(&(*((string*)((__sort_src_2).data) + (__sort_j_10))), &(*((string*)((__sort_src_2).data) + (__sort_i_9)))) < 0) {
								array_sort_move(__sort_dst_3.data, __sort_k_11, __sort_src_2.data, __sort_j_10, 1, sizeof(string));
								__sort_j_10 = __sort_j_10 + 1;
							} else {
								array_sort_move(__sort_dst_3.data, __sort_k_11, __sort_src_2.data, __sort_i_9, 1, sizeof(string));
								__sort_i_9 = __sort_i_9 + 1;
							}
						}
						if (__sort_i_9 < __sort_mid_7) {
							array_sort_move(__sort_dst_3.data, __sort_k_11, __sort_src_2.data, __sort_i_9, __sort_mid_7 - __sort_i_9, sizeof(string));
						} else {
							if (__sort_j_10 < __sort_hi_8) {
								array_sort_move(__sort_dst_3.data, __sort_k_11, __sort_src_2.data, __sort_j_10, __sort_hi_8 - __sort_j_10, sizeof(string));
							}
						}
						__sort_lo_6 = __sort_hi_8;
					}
				}
				Array __sort_swap_4 = __sort_src_2;
				__sort_src_2 = __sort_dst_3;
				__sort_dst_3 = __sort_swap_4;
			}
		}
		if (__sort_src_2.data != s->data) {
			array_sort_move(s->data, 0, __sort_src_2.data, 0, __sort_n_0, sizeof(string));
		}
		array__free(&__sort_buf_1);
	}
	0;
}

/* V3CACHE_FN_END 3ddfa6603dbe136 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 91f9f5cd95270eac */
inline void Array_string__sort_ignore_case(Array* s) {
	{
		i64 __sort_n_0 = s->len;
		Array __sort_buf_1 = array_new(		sizeof(string), __sort_n_0, __sort_n_0);
		Array __sort_src_2 = *s;
		Array __sort_dst_3 = __sort_buf_1;
		{
			i64 __sort_w_5 = 1;
			for (; __sort_w_5 < __sort_n_0; __sort_w_5 = (i64)(((i64)(			__sort_w_5)) << (1))) {
				{
					i64 __sort_lo_6 = 0;
					for (; __sort_lo_6 < __sort_n_0; ) {
						i64 __sort_mid_7 = __sort_lo_6 + __sort_w_5;
						if (__sort_mid_7 > __sort_n_0) {
							__sort_mid_7 = __sort_n_0;
						}
						i64 __sort_hi_8 = __sort_mid_7 + __sort_w_5;
						if (__sort_hi_8 > __sort_n_0) {
							__sort_hi_8 = __sort_n_0;
						}
						i64 __sort_i_9 = __sort_lo_6;
						i64 __sort_j_10 = __sort_mid_7;
						i64 __sort_k_11 = __sort_lo_6;
						for (; (__sort_i_9 < __sort_mid_7) && (__sort_j_10 < __sort_hi_8); __sort_k_11++) {
							if (compare_lower_strings(&(*((string*)((__sort_src_2).data) + (__sort_j_10))), &(*((string*)((__sort_src_2).data) + (__sort_i_9)))) < 0) {
								array_sort_move(__sort_dst_3.data, __sort_k_11, __sort_src_2.data, __sort_j_10, 1, sizeof(string));
								__sort_j_10 = __sort_j_10 + 1;
							} else {
								array_sort_move(__sort_dst_3.data, __sort_k_11, __sort_src_2.data, __sort_i_9, 1, sizeof(string));
								__sort_i_9 = __sort_i_9 + 1;
							}
						}
						if (__sort_i_9 < __sort_mid_7) {
							array_sort_move(__sort_dst_3.data, __sort_k_11, __sort_src_2.data, __sort_i_9, __sort_mid_7 - __sort_i_9, sizeof(string));
						} else {
							if (__sort_j_10 < __sort_hi_8) {
								array_sort_move(__sort_dst_3.data, __sort_k_11, __sort_src_2.data, __sort_j_10, __sort_hi_8 - __sort_j_10, sizeof(string));
							}
						}
						__sort_lo_6 = __sort_hi_8;
					}
				}
				Array __sort_swap_4 = __sort_src_2;
				__sort_src_2 = __sort_dst_3;
				__sort_dst_3 = __sort_swap_4;
			}
		}
		if (__sort_src_2.data != s->data) {
			array_sort_move(s->data, 0, __sort_src_2.data, 0, __sort_n_0, sizeof(string));
		}
		array__free(&__sort_buf_1);
	}
	0;
}

/* V3CACHE_FN_END 91f9f5cd95270eac */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN d14baadceeb70143 */
string Array_string__str(Array a) {
	i64 sb_len = 4;
	if (a.len > 0) {
		sb_len += ((*((string*)((a).data) + (0)))).len;
		sb_len *= a.len;
	}
	sb_len += 2;
	Array __esc_0 = strings__new_builder(sb_len);
	Array* sb = (Array*)(memdup(&__esc_0, sizeof(Array)));
	strings__Builder__write_u8(sb, '[');
	{
		i64 i = 0;
		for (; i < a.len; i++) {
			string val = (*((string*)((a).data) + (i)));
			strings__Builder__write_u8(sb, '\'');
			strings__Builder__write_string(sb, val);
			strings__Builder__write_u8(sb, '\'');
			if (i < (a.len - 1)) {
				strings__Builder__write_string(sb, _v3_lit_2_9afebb00c5c7fa67);
			}
		}
	}
	strings__Builder__write_u8(sb, ']');
	string res = strings__Builder__str(sb);
	{
		strings__Builder__free(sb);
	}
	return res;
}

/* V3CACHE_FN_END d14baadceeb70143 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN bc0952fa81c22c7a */
__v_result_u32 Array_u8__byterune(Array b) {
	__v_result_u32 __or_opt_0 = Array_u8__utf8_to_utf32(b);
	u32 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_u32){.ok = false, .err = __or_opt_0.err};
	}
	u32 r = __or_val_1;
	return (__v_result_u32){.ok = true, .value = (u32)(r)};
	return (__v_result_u32){.ok = true};
}

/* V3CACHE_FN_END bc0952fa81c22c7a */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 2631a74fdd9685a9 */
string Array_u8__bytestr(Array b) {
	{
		u8* buf = malloc_noscan(b.len + 1);
		vmemcpy((void*)(buf), b.data, b.len);
		(buf)[b.len] = 0;
		return tos(buf, b.len);
	}
}

/* V3CACHE_FN_END 2631a74fdd9685a9 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 6dffb420722f5b99 */
string Array_u8__hex(Array b) {
	if (b.len == 0) {
		return _v3_lit_0_14650fb0739d0383;
	}
	return data_to_hex_string(b.data, b.len);
}

/* V3CACHE_FN_END 6dffb420722f5b99 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN f6a12be5286f6c20 */
__v_result_u32 Array_u8__utf8_to_utf32(Array _bytes) {
	if (_bytes.len > 4) {
		return (__v_result_u32){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_74_82fc39ccb1421152, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_u32){.ok = true, .value = impl_utf8_to_utf32(_bytes.data, _bytes.len)};
	return (__v_result_u32){.ok = true};
}

/* V3CACHE_FN_END f6a12be5286f6c20 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN cd3af5947a67101a */
DenseArray DenseArray__clone(DenseArray* d) {
}

/* V3CACHE_FN_END cd3af5947a67101a */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 9cfe42704b8516f4 */
string Error__msg(Error err) {
}

/* V3CACHE_FN_END 9cfe42704b8516f4 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN cdece776767d8361 */
string IError__str(IError err) {
}

/* V3CACHE_FN_END cdece776767d8361 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN df63a4247fa236bb */
string MessageError__msg(MessageError err) {
}

/* V3CACHE_FN_END df63a4247fa236bb */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN a2a6284d72a7ef69 */
string MessageError__str(MessageError err) {
}

/* V3CACHE_FN_END a2a6284d72a7ef69 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 3af66bf95d72bd88 */
string None____str(None__ _0) {
}

/* V3CACHE_FN_END 3af66bf95d72bd88 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 1c545a3f873bab44 */
string StrIntpType_str(int x) {
}

/* V3CACHE_FN_END 1c545a3f873bab44 */
/* V3CACHE_MODULE main */
/* V3CACHE_FN_BEGIN 118e346dc41817f9 */
void Suite__check_example(main__Suite* s, string label, string path, string want) {
	__v_result_string __or_opt_0 = run_example(path);
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		Suite__fail(s, label, IError__msg(&(err)));
		return;
	}
	string out = __or_val_1;
	if (!string__contains(out, want)) {
		Suite__fail(s, label, ({ string __v3_internal_symbol_join_0[4]; __v3_internal_symbol_join_0[0] = _v3_lit_25_1766e2162d456e94; __v3_internal_symbol_join_0[1] = want; __v3_internal_symbol_join_0[2] = _v3_lit_3_ad6b694fce019f13; __v3_internal_symbol_join_0[3] = out; string_plus_many(4, __v3_internal_symbol_join_0); }));
		return;
	}
	println(string__plus(_v3_lit_5_9aa8c9aed04546bf, label));
}

/* V3CACHE_FN_END 118e346dc41817f9 */
/* V3CACHE_MODULE main */
/* V3CACHE_FN_BEGIN c762b6d8a3a9d024 */
void Suite__fail(main__Suite* s, string label, string msg) {
	s->fails++;
	println(({ string __v3_internal_symbol_join_0[4]; __v3_internal_symbol_join_0[0] = _v3_lit_5_6b447c763a12a2d; __v3_internal_symbol_join_0[1] = label; __v3_internal_symbol_join_0[2] = _v3_lit_2_9b20b700c5e4da01; __v3_internal_symbol_join_0[3] = msg; string_plus_many(4, __v3_internal_symbol_join_0); }));
}

/* V3CACHE_FN_END c762b6d8a3a9d024 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN e312c1fa22ebb42a */
VMapData* VMapData__clone(VMapData* m) {
}

/* V3CACHE_FN_END e312c1fa22ebb42a */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN f3150385138295eb */
void* __as_cast(void* obj, i64 obj_type, i64 expected_type, string obj_name, string expected_name) {
}

/* V3CACHE_FN_END f3150385138295eb */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 151d27964087f155 */
array __new_array(i64 mylen, i64 cap, i64 elm_size) {
}

/* V3CACHE_FN_END 151d27964087f155 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN cf44073c39f5a960 */
array __new_array_noscan(i64 mylen, i64 cap, i64 elm_size) {
}

/* V3CACHE_FN_END cf44073c39f5a960 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN e1b85e88591dcc29 */
array array__clone(array* a) {
}

/* V3CACHE_FN_END e1b85e88591dcc29 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN ae733e2b6d4e99a9 */
void array__delete(array* a, i64 i) {
}

/* V3CACHE_FN_END ae733e2b6d4e99a9 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 38d5f91c4feb6896 */
void array__delete_last(array* a) {
}

/* V3CACHE_FN_END 38d5f91c4feb6896 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN cfa62fc8ad33080b */
void array__ensure_cap(array* a, i64 required) {
}

/* V3CACHE_FN_END cfa62fc8ad33080b */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 97d5536e5ced4c76 */
void array__free(array* a) {
}

/* V3CACHE_FN_END 97d5536e5ced4c76 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN f4e6d077efebca82 */
void* array__get(array a, i64 i) {
}

/* V3CACHE_FN_END f4e6d077efebca82 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 9a911c549543d0db */
void array__insert(array* a, i64 i, void* val) {
}

/* V3CACHE_FN_END 9a911c549543d0db */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 46eac399ca1f44a1 */
void array__insert_many(array* a, i64 i, void* val, i64 size) {
}

/* V3CACHE_FN_END 46eac399ca1f44a1 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 2e3d90a361058ee3 */
void* array__pop_left(array* a) {
}

/* V3CACHE_FN_END 2e3d90a361058ee3 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 9972e4211d8bdd26 */
void array__prepend(array* a, void* val) {
}

/* V3CACHE_FN_END 9972e4211d8bdd26 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 71d74b94fe9ec3c */
inline void array__push(array* a, void* val) {
}

/* V3CACHE_FN_END 71d74b94fe9ec3c */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 8a412c3e3559cee4 */
void array__push_many(array* a, void* val, i64 size) {
}

/* V3CACHE_FN_END 8a412c3e3559cee4 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 8aa2c81d6c47cb4c */
array array__reverse(array a) {
}

/* V3CACHE_FN_END 8aa2c81d6c47cb4c */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 2e72411e843d237e */
void array__set(array* a, i64 i, void* val) {
}

/* V3CACHE_FN_END 2e72411e843d237e */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 71f503fb41db3fb0 */
array array__slice(array a, i64 start, i64 _end) {
}

/* V3CACHE_FN_END 71f503fb41db3fb0 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 5c2657e386fc7462 */
array array__slice_ni(array a, i64 _start, i64 _end) {
}

/* V3CACHE_FN_END 5c2657e386fc7462 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 2afd98321f11e829 */
void array__sort_with_compare(array* a, _fn_ptr_ed95774d3a4e97ab callback) {
}

/* V3CACHE_FN_END 2afd98321f11e829 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 1c12243201269031 */
inline void array_sort_move(void* dst, i64 di, void* src, i64 si, i64 count, size_t element_size) {
}

/* V3CACHE_FN_END 1c12243201269031 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN d3978f37fcdeac57 */
void autostr_addr_pop(void) {
}

/* V3CACHE_FN_END d3978f37fcdeac57 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 1ca2b1bde5ee36ed */
bool autostr_addr_type_in_stack(void* addr, i64 typ) {
}

/* V3CACHE_FN_END 1ca2b1bde5ee36ed */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 458c97584dafdfd9 */
void autostr_addr_type_push(void* addr, i64 typ) {
}

/* V3CACHE_FN_END 458c97584dafdfd9 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN fd260127514bca92 */
int blip__Closure__payload_tag(blip__Closure* c) {
	return 16384;
}

/* V3CACHE_FN_END fd260127514bca92 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 9e5049c5c3c077c5 */
void blip__EnvArena__define(blip__EnvArena* ea, i32 id, string name, blip__Value val) {
	blip__Env* frame = blip__EnvArena__get(ea, id);
	{
		i64 i = 0;
		for (; i < frame->names.len; i++) {
			string n = *(string*)(array_get(frame->names, i));
			if (string__eq(n, name)) {
				{ Array* _a0 = &frame->vals; int _i0 = i; array__set(_a0, _i0, &(blip__Value[]){val}); }
				return;
			}
		}
	}
	string __arr_val_0 = string__clone(name);
	array_push(&frame->names, &__arr_val_0);
	blip__Value __arr_val_1 = val;
	array_push(&frame->vals, &__arr_val_1);
}

/* V3CACHE_FN_END 9e5049c5c3c077c5 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN d67474df28109cc6 */
blip__Env* blip__EnvArena__get(blip__EnvArena* ea, i32 id) {
	if (id == blip__no_env) {
		return NULL;
	}
	return &(*(blip__Env*)array_get(ea->frames, (i64)(id)));
}

/* V3CACHE_FN_END d67474df28109cc6 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 5215b06b08be84b6 */
__v_option_blip__Value blip__EnvArena__lookup(blip__EnvArena* ea, i32 id, string name) {
	i32 cur = id;
	while (cur != blip__no_env) {
		blip__Env* frame = blip__EnvArena__get(ea, cur);
		{
			i64 i = 0;
			for (; i < frame->names.len; i++) {
				string n = *(string*)(array_get(frame->names, i));
				if (string__eq(n, name)) {
					return (__v_option_blip__Value){.ok = true, .value = (*(blip__Value*)array_get(frame->vals, i))};
				}
			}
		}
		cur = frame->parent;
	}
	return (__v_option_blip__Value){.ok = false};
	return (__v_option_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 5215b06b08be84b6 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN b399c3fe99939b04 */
i32 blip__EnvArena__new_env(blip__EnvArena* ea, i32 parent) {
	blip__Env __arr_val_0 = (blip__Env){.parent = parent, .names = array_new(	sizeof(string), 0, 0), .vals = array_new(	sizeof(blip__Value), 0, 0)};
	array_push(&ea->frames, &__arr_val_0);
	return (i32)(ea->frames.len - 1);
}

/* V3CACHE_FN_END b399c3fe99939b04 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 70d8c4375850910a */
bool blip__EnvArena__set(blip__EnvArena* ea, i32 id, string name, blip__Value val) {
	i32 cur = id;
	while (cur != blip__no_env) {
		blip__Env* frame = blip__EnvArena__get(ea, cur);
		{
			i64 i = 0;
			for (; i < frame->names.len; i++) {
				string n = *(string*)(array_get(frame->names, i));
				if (string__eq(n, name)) {
					{ Array* _a0 = &frame->vals; int _i0 = i; array__set(_a0, _i0, &(blip__Value[]){val}); }
					return true;
				}
			}
		}
		cur = frame->parent;
	}
	return false;
}

/* V3CACHE_FN_END 70d8c4375850910a */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 818cddf5b06f91e0 */
int blip__NoPayload__payload_tag(blip__NoPayload* n) {
	return 1;
}

/* V3CACHE_FN_END 818cddf5b06f91e0 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 75a32094edb8914f */
int blip__Pair__payload_tag(blip__Pair* p) {
	return p->tag;
}

/* V3CACHE_FN_END 75a32094edb8914f */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN cea0ba242eccdb9e */
int blip__StringPayload__payload_tag(blip__StringPayload* p) {
	return p->tag;
}

/* V3CACHE_FN_END cea0ba242eccdb9e */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN aae598f66de7b4df */
blip__Value blip__StructVal__at(blip__StructVal* s, i64 i) {
	string __map_key_0 = (*(string*)array_get(s->fields, i));
	blip__Value __map_zero_1 = (blip__Value){.tag = 0};
	return *(blip__Value*)(map__get(&s->values, &__map_key_0, &__map_zero_1));
}

/* V3CACHE_FN_END aae598f66de7b4df */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 60101f8ce6f46dc2 */
blip__Value blip__StructVal__get(blip__StructVal* s, string k) {
	string __map_key_0 = k;
	if (map__exists(&s->values, &__map_key_0)) {
		string __map_key_1 = k;
		blip__Value __map_zero_2 = (blip__Value){.tag = 0};
		return *(blip__Value*)(map__get(&s->values, &__map_key_1, &__map_zero_2));
	}
	return blip__nil_value();
}

/* V3CACHE_FN_END 60101f8ce6f46dc2 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN df31380c61f3836 */
bool blip__StructVal__has(blip__StructVal* s, string k) {
	string __map_key_0 = k;
	return map__exists(&s->values, &__map_key_0);
}

/* V3CACHE_FN_END df31380c61f3836 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 4e89bc09f0bde8cd */
int blip__StructVal__payload_tag(blip__StructVal* s) {
	return 131072;
}

/* V3CACHE_FN_END 4e89bc09f0bde8cd */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 9d0190337e28efbe */
void blip__StructVal__set(blip__StructVal* s, string k, blip__Value v) {
	string __map_key_0 = k;
	blip__Value __map_val_1 = v;
	map__set(&s->values, &__map_key_0, &__map_val_1);
}

/* V3CACHE_FN_END 9d0190337e28efbe */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 3038c7885bfd2458 */
blip__StructVal* blip__StructVal__with(blip__StructVal* s, string k, blip__Value v) {
	map mm = new_map(sizeof(string), sizeof(blip__Value), map_hash_string, map_eq_string, map_clone_string, map_free_string);
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < s->fields.len; __for_idx_0++) {
			string f = *(string*)(array_get(s->fields, __for_idx_0));
			if (string__eq(f, k)) {
				string __map_key_1 = f;
				blip__Value __map_val_2 = v;
				map__set(&mm, &__map_key_1, &__map_val_2);
			} else {
				string __map_key_3 = f;
				string __map_key_5 = f;
				blip__Value __map_zero_6 = (blip__Value){.tag = 0};
				blip__Value __map_val_4 = *(blip__Value*)(map__get(&s->values, &__map_key_5, &__map_zero_6));
				map__set(&mm, &__map_key_3, &__map_val_4);
			}
		}
	}
	return (blip__StructVal*)memdup(&(blip__StructVal){.name = s->name, .fields = s->fields, .values = mm}, sizeof(blip__StructVal));
}

/* V3CACHE_FN_END 3038c7885bfd2458 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 2f0ee64679564de1 */
void blip__Table__delete_at(blip__Table* t, string k) {
	{
		map m = t->values;
		string __map_key_0 = k;
		map__delete(&m, &__map_key_0);
		t->values = m;
	}
}

/* V3CACHE_FN_END 2f0ee64679564de1 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN f527ec8e5ee6e602 */
blip__Value blip__Table__get(blip__Table* t, string k) {
	string __map_key_0 = k;
	if (map__exists(&t->values, &__map_key_0)) {
		string __map_key_1 = k;
		blip__Value __map_zero_2 = (blip__Value){.tag = 0};
		return *(blip__Value*)(map__get(&t->values, &__map_key_1, &__map_zero_2));
	}
	return blip__nil_value();
}

/* V3CACHE_FN_END f527ec8e5ee6e602 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 4c83e087260cf576 */
bool blip__Table__has(blip__Table* t, string k) {
	string __map_key_0 = k;
	return map__exists(&t->values, &__map_key_0);
}

/* V3CACHE_FN_END 4c83e087260cf576 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN c25cf52727fd55a0 */
Array blip__Table__keys(blip__Table* t) {
	Array out = array_new(	sizeof(string), 0, 0);
	Array __push_many_0 = map__keys(&t->values);
	array__push_many(&out, __push_many_0.data, __push_many_0.len);
	{
		i64 __sort_n_1 = out.len;
		Array __sort_buf_2 = array_new(		sizeof(string), __sort_n_1, __sort_n_1);
		Array __sort_src_3 = out;
		Array __sort_dst_4 = __sort_buf_2;
		{
			i64 __sort_w_6 = 1;
			for (; __sort_w_6 < __sort_n_1; __sort_w_6 = (i64)(((i64)(			__sort_w_6)) << (1))) {
				{
					i64 __sort_lo_7 = 0;
					for (; __sort_lo_7 < __sort_n_1; ) {
						i64 __sort_mid_8 = __sort_lo_7 + __sort_w_6;
						if (__sort_mid_8 > __sort_n_1) {
							__sort_mid_8 = __sort_n_1;
						}
						i64 __sort_hi_9 = __sort_mid_8 + __sort_w_6;
						if (__sort_hi_9 > __sort_n_1) {
							__sort_hi_9 = __sort_n_1;
						}
						i64 __sort_i_10 = __sort_lo_7;
						i64 __sort_j_11 = __sort_mid_8;
						i64 __sort_k_12 = __sort_lo_7;
						for (; (__sort_i_10 < __sort_mid_8) && (__sort_j_11 < __sort_hi_9); __sort_k_12++) {
							if (string__lt((*((string*)((__sort_src_3).data) + (__sort_j_11))), (*((string*)((__sort_src_3).data) + (__sort_i_10))))) {
								array_sort_move(__sort_dst_4.data, __sort_k_12, __sort_src_3.data, __sort_j_11, 1, sizeof(string));
								__sort_j_11 = __sort_j_11 + 1;
							} else {
								array_sort_move(__sort_dst_4.data, __sort_k_12, __sort_src_3.data, __sort_i_10, 1, sizeof(string));
								__sort_i_10 = __sort_i_10 + 1;
							}
						}
						if (__sort_i_10 < __sort_mid_8) {
							array_sort_move(__sort_dst_4.data, __sort_k_12, __sort_src_3.data, __sort_i_10, __sort_mid_8 - __sort_i_10, sizeof(string));
						} else {
							if (__sort_j_11 < __sort_hi_9) {
								array_sort_move(__sort_dst_4.data, __sort_k_12, __sort_src_3.data, __sort_j_11, __sort_hi_9 - __sort_j_11, sizeof(string));
							}
						}
						__sort_lo_7 = __sort_hi_9;
					}
				}
				Array __sort_swap_5 = __sort_src_3;
				__sort_src_3 = __sort_dst_4;
				__sort_dst_4 = __sort_swap_5;
			}
		}
		if (__sort_src_3.data != out.data) {
			array_sort_move(out.data, 0, __sort_src_3.data, 0, __sort_n_1, sizeof(string));
		}
		array__free(&__sort_buf_2);
	}
	0;
	return out;
}

/* V3CACHE_FN_END c25cf52727fd55a0 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 7b8439c1302d630d */
int blip__Table__payload_tag(blip__Table* t) {
	return t->tag;
}

/* V3CACHE_FN_END 7b8439c1302d630d */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 9f0d8ef5f382246c */
void blip__Table__set_at(blip__Table* t, string k, blip__Value val) {
	{
		map m = t->values;
		string __map_key_0 = k;
		blip__Value __map_val_1 = val;
		map__set(&m, &__map_key_0, &__map_val_1);
		t->values = m;
	}
}

/* V3CACHE_FN_END 9f0d8ef5f382246c */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN c79069318d787b7e */
inline bool blip__Value__as_bool(blip__Value v) {
	if (!(v.tag == 2)) {
		v3_eprint_lit("D:\\MyProjects\\vlip\\vlib\\blip\\mod.v:432: FAIL: fn blip.Value.as_bool: assert v.tag == .boolean\n");
		string _t1 = blip__Tag__autostr(		v.tag);
		if (_t1.len == 5 && memcmp(_t1.str, "v.tag", 5) == 0) {
			v3_eprint_lit("   left value: v.tag\n");
		} else {
			fprintf(stderr, "%s: %s = %.*s\n", "   left value", "v.tag", (int)_t1.len, (char*)_t1.str);
		}
		string _t2 = v3_c_lit("boolean", 7);
		if (_t2.len == 8 && memcmp(_t2.str, ".boolean", 8) == 0) {
			v3_eprint_lit("  right value: .boolean\n");
		} else {
			fprintf(stderr, "%s: %s = %.*s\n", "  right value", ".boolean", (int)_t2.len, (char*)_t2.str);
		}
		v3_eprint_lit("V panic: Assertion failed...\n");
		exit(1);
	}
	return v.i != 0;
}

/* V3CACHE_FN_END c79069318d787b7e */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN de81b7389851949f */
inline blip__Closure* blip__Value__as_closure(blip__Value v) {
	if (!(v.tag == 16384)) {
		v3_eprint_lit("D:\\MyProjects\\vlip\\vlib\\blip\\mod.v:669: FAIL: fn blip.Value.as_closure: assert v.tag == .closure\n");
		string _t1 = blip__Tag__autostr(		v.tag);
		if (_t1.len == 5 && memcmp(_t1.str, "v.tag", 5) == 0) {
			v3_eprint_lit("   left value: v.tag\n");
		} else {
			fprintf(stderr, "%s: %s = %.*s\n", "   left value", "v.tag", (int)_t1.len, (char*)_t1.str);
		}
		string _t2 = v3_c_lit("closure", 7);
		if (_t2.len == 8 && memcmp(_t2.str, ".closure", 8) == 0) {
			v3_eprint_lit("  right value: .closure\n");
		} else {
			fprintf(stderr, "%s: %s = %.*s\n", "  right value", ".closure", (int)_t2.len, (char*)_t2.str);
		}
		v3_eprint_lit("V panic: Assertion failed...\n");
		exit(1);
	}
	return (blip__Closure*)v.payload._object;
}

/* V3CACHE_FN_END de81b7389851949f */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN ac0e6de8770881e6 */
inline double blip__Value__as_float(blip__Value v) {
	if (!(v.tag == 8)) {
		v3_eprint_lit("D:\\MyProjects\\vlip\\vlib\\blip\\mod.v:426: FAIL: fn blip.Value.as_float: assert v.tag == .float\n");
		string _t1 = blip__Tag__autostr(		v.tag);
		if (_t1.len == 5 && memcmp(_t1.str, "v.tag", 5) == 0) {
			v3_eprint_lit("   left value: v.tag\n");
		} else {
			fprintf(stderr, "%s: %s = %.*s\n", "   left value", "v.tag", (int)_t1.len, (char*)_t1.str);
		}
		string _t2 = v3_c_lit("float", 5);
		if (_t2.len == 6 && memcmp(_t2.str, ".float", 6) == 0) {
			v3_eprint_lit("  right value: .float\n");
		} else {
			fprintf(stderr, "%s: %s = %.*s\n", "  right value", ".float", (int)_t2.len, (char*)_t2.str);
		}
		v3_eprint_lit("V panic: Assertion failed...\n");
		exit(1);
	}
	return v.f;
}

/* V3CACHE_FN_END ac0e6de8770881e6 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN b4622bcf71fd028b */
inline i64 blip__Value__as_int(blip__Value v) {
	if (!((v.tag == 4) || (v.tag == 16))) {
		v3_eprint_lit("D:\\MyProjects\\vlip\\vlib\\blip\\mod.v:420: FAIL: fn blip.Value.as_int: assert v.tag == .integer || v.tag == .rune\n");
		v3_eprint_lit("V panic: Assertion failed...\n");
		exit(1);
	}
	return v.i;
}

/* V3CACHE_FN_END b4622bcf71fd028b */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN ebdacded71094ea2 */
inline blip__Pair* blip__Value__as_pair(blip__Value v) {
	if (!(v.tag == 256)) {
		v3_eprint_lit("D:\\MyProjects\\vlip\\vlib\\blip\\mod.v:444: FAIL: fn blip.Value.as_pair: assert v.tag == .pair\n");
		string _t1 = blip__Tag__autostr(		v.tag);
		if (_t1.len == 5 && memcmp(_t1.str, "v.tag", 5) == 0) {
			v3_eprint_lit("   left value: v.tag\n");
		} else {
			fprintf(stderr, "%s: %s = %.*s\n", "   left value", "v.tag", (int)_t1.len, (char*)_t1.str);
		}
		string _t2 = v3_c_lit("pair", 4);
		if (_t2.len == 5 && memcmp(_t2.str, ".pair", 5) == 0) {
			v3_eprint_lit("  right value: .pair\n");
		} else {
			fprintf(stderr, "%s: %s = %.*s\n", "  right value", ".pair", (int)_t2.len, (char*)_t2.str);
		}
		v3_eprint_lit("V panic: Assertion failed...\n");
		exit(1);
	}
	return (blip__Pair*)v.payload._object;
}

/* V3CACHE_FN_END ebdacded71094ea2 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN b8278f1e3456ee6d */
inline string blip__Value__as_string(blip__Value v) {
	if (!((v.tag == 32) || (v.tag == 64) || (v.tag == 128) || (v.tag == 32768))) {
		v3_eprint_lit("D:\\MyProjects\\vlip\\vlib\\blip\\mod.v:438: FAIL: fn blip.Value.as_string: assert v.tag in [.string, .symbol, .keyword, .primitive]\n");
		v3_eprint_lit("V panic: Assertion failed...\n");
		exit(1);
	}
	return (((blip__StringPayload*)v.payload._object))->s;
}

/* V3CACHE_FN_END b8278f1e3456ee6d */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 9bffa30627ee5835 */
inline blip__StructVal* blip__Value__as_struct(blip__Value v) {
	if (!(v.tag == 131072)) {
		v3_eprint_lit("D:\\MyProjects\\vlip\\vlib\\blip\\mod.v:473: FAIL: fn blip.Value.as_struct: assert v.tag == .struct_\n");
		string _t1 = blip__Tag__autostr(		v.tag);
		if (_t1.len == 5 && memcmp(_t1.str, "v.tag", 5) == 0) {
			v3_eprint_lit("   left value: v.tag\n");
		} else {
			fprintf(stderr, "%s: %s = %.*s\n", "   left value", "v.tag", (int)_t1.len, (char*)_t1.str);
		}
		string _t2 = v3_c_lit("struct_", 7);
		if (_t2.len == 8 && memcmp(_t2.str, ".struct_", 8) == 0) {
			v3_eprint_lit("  right value: .struct_\n");
		} else {
			fprintf(stderr, "%s: %s = %.*s\n", "  right value", ".struct_", (int)_t2.len, (char*)_t2.str);
		}
		v3_eprint_lit("V panic: Assertion failed...\n");
		exit(1);
	}
	return (blip__StructVal*)v.payload._object;
}

/* V3CACHE_FN_END 9bffa30627ee5835 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN acfed19945efd1c8 */
inline blip__Table* blip__Value__as_table(blip__Value v) {
	if (!((v.tag == 4096) || (v.tag == 8192))) {
		v3_eprint_lit("D:\\MyProjects\\vlip\\vlib\\blip\\mod.v:456: FAIL: fn blip.Value.as_table: assert v.tag == .table || v.tag == .buffer\n");
		v3_eprint_lit("V panic: Assertion failed...\n");
		exit(1);
	}
	return (blip__Table*)v.payload._object;
}

/* V3CACHE_FN_END acfed19945efd1c8 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN bc6559794c9df0df */
inline blip__Vector* blip__Value__as_vector(blip__Value v) {
	if (!((v.tag == 1024) || (v.tag == 2048))) {
		v3_eprint_lit("D:\\MyProjects\\vlip\\vlib\\blip\\mod.v:450: FAIL: fn blip.Value.as_vector: assert v.tag == .vector || v.tag == .array\n");
		v3_eprint_lit("V panic: Assertion failed...\n");
		exit(1);
	}
	return (blip__Vector*)v.payload._object;
}

/* V3CACHE_FN_END bc6559794c9df0df */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 858910e2ddc0180f */
inline bool blip__Value__is_empty_seq(blip__Value v) {
	return (v.tag == 1) || (v.tag == 512);
}

/* V3CACHE_FN_END 858910e2ddc0180f */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 9f54b7d73a0dad79 */
inline bool blip__Value__is_nil(blip__Value v) {
	return v.tag == 1;
}

/* V3CACHE_FN_END 9f54b7d73a0dad79 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN a1365b069dcbbc3d */
inline bool blip__Value__truthy(blip__Value v) {
	return !((v.tag == 2) && (v.i == 0));
}

/* V3CACHE_FN_END a1365b069dcbbc3d */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN ed44274c6888ec8a */
int blip__Vector__payload_tag(blip__Vector* v) {
	return v->tag;
}

/* V3CACHE_FN_END ed44274c6888ec8a */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN a68bf20f40177a98 */
blip__Value blip__Vector__pop(blip__Vector* v) {
	{
		Array d = array__clone(&v->data);
		blip__Value last = (*((blip__Value*)((d).data) + (d.len - 1)));
		d = array_slice(		d, 0, 		d.len - 1);
		v->data = d;
		return last;
	}
}

/* V3CACHE_FN_END a68bf20f40177a98 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 2acae01ef2935d11 */
void blip__Vector__push(blip__Vector* v, blip__Value val) {
	{
		Array d = array__clone(&v->data);
		blip__Value __arr_val_0 = val;
		array_push(&d, &__arr_val_0);
		v->data = d;
	}
}

/* V3CACHE_FN_END 2acae01ef2935d11 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN c2d574b23cc8f73d */
void blip__Vector__set_at(blip__Vector* v, i64 i, blip__Value val) {
	{
		{ Array* __v3_internal_symbol_array_store_base_0 = &v->data; int __v3_internal_symbol_array_store_index_0 = i; blip__Value __v3_internal_symbol_array_store_value_0 = val; (*((blip__Value*)((__v3_internal_symbol_array_store_base_0)->data) + (__v3_internal_symbol_array_store_index_0))) = __v3_internal_symbol_array_store_value_0; }
	}
}

/* V3CACHE_FN_END c2d574b23cc8f73d */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 76085834929e576c */
blip__Value blip__boolean(bool b) {
	i64 __if_val_0 = {0};
	if (b) {
		__if_val_0 = 1;
	} else {
		__if_val_0 = 0;
	}
	return (blip__Value){.tag = 2, .i = __if_val_0, .payload = blip__no_payload};
}

/* V3CACHE_FN_END 76085834929e576c */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 4992737dcd9f0958 */
blip__Value blip__buffer(map m) {
	blip__Table* __iface_src_0 = (blip__Table*)memdup(&(blip__Table){.tag = 8192, .values = m}, sizeof(blip__Table));
	return (blip__Value){.tag = 8192, .payload = (blip__Payload){._typ = 1517345688, ._object = __iface_src_0}};
}

/* V3CACHE_FN_END 4992737dcd9f0958 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 9065388fcf8f4621 */
blip__Value blip__cons(blip__Value car, blip__Value cdr) {
	blip__Pair* __iface_src_0 = (blip__Pair*)memdup(&(blip__Pair){.tag = 256, .car = car, .cdr = cdr}, sizeof(blip__Pair));
	return (blip__Value){.tag = 256, .payload = (blip__Payload){._typ = 1821533960, ._object = __iface_src_0}};
}

/* V3CACHE_FN_END 9065388fcf8f4621 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN e328c7dd9038d6b8 */
blip__Value blip__empty_list(void) {
	return (blip__Value){.tag = 512, .payload = blip__no_payload};
}

/* V3CACHE_FN_END e328c7dd9038d6b8 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 7c3ff08017775a92 */
blip__Value blip__float(double f) {
	return (blip__Value){.tag = 8, .f = f, .payload = blip__no_payload};
}

/* V3CACHE_FN_END 7c3ff08017775a92 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 57422650fa5f284c */
string blip__format_error(string path, string src, i64 line, i64 col, string msg, i64 length) {
	if (line <= 0) {
		return string__plus(_v3_lit_7_ac197bc1ded81c19, msg);
	}
	Array lines = string__split(src, _v3_lit_1_44bd31d473cd01cb);
	if (line > lines.len) {
		return string__plus(_v3_lit_7_ac197bc1ded81c19, msg);
	}
	string src_line = (*(string*)array_get(lines, line - 1));
	i64 max_src = 200;
	if (src_line.len > max_src) {
		src_line = string__plus(string__substr(		src_line, 0, 		max_src), _v3_lit_3_14ac5950084b3b2f);
	}
	string ln = int__str(line);
	string gutter = string__repeat(_v3_lit_1_44bd4bd473cd2df9, ln.len);
	string out = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_7_ac197bc1ded81c19; __v3_internal_symbol_join_0[1] = msg; __v3_internal_symbol_join_0[2] = _v3_lit_1_44bd31d473cd01cb; string_plus_many(3, __v3_internal_symbol_join_0); });
	out = string__plus(out, ({ string __v3_internal_symbol_join_1[7]; __v3_internal_symbol_join_1[0] = _v3_lit_6_99f02d3504a82c5b; __v3_internal_symbol_join_1[1] = path; __v3_internal_symbol_join_1[2] = _v3_lit_1_44bd61d473cd535b; __v3_internal_symbol_join_1[3] = int__str(line); __v3_internal_symbol_join_1[4] = _v3_lit_1_44bd61d473cd535b; __v3_internal_symbol_join_1[5] = int__str(col); __v3_internal_symbol_join_1[6] = _v3_lit_1_44bd31d473cd01cb; string_plus_many(7, __v3_internal_symbol_join_1); }));
	out = ({ string __v3_internal_symbol_join_2[3]; __v3_internal_symbol_join_2[0] = out; __v3_internal_symbol_join_2[1] = gutter; __v3_internal_symbol_join_2[2] = _v3_lit_3_bce9634fd643284f; string_plus_many(3, __v3_internal_symbol_join_2); });
	out = string__plus(out, ({ string __v3_internal_symbol_join_3[4]; __v3_internal_symbol_join_3[0] = ln; __v3_internal_symbol_join_3[1] = _v3_lit_3_bce94d4fd64302ed; __v3_internal_symbol_join_3[2] = src_line; __v3_internal_symbol_join_3[3] = _v3_lit_1_44bd31d473cd01cb; string_plus_many(4, __v3_internal_symbol_join_3); }));
	out = ({ string __v3_internal_symbol_join_4[3]; __v3_internal_symbol_join_4[0] = out; __v3_internal_symbol_join_4[1] = gutter; __v3_internal_symbol_join_4[2] = _v3_lit_3_bce94d4fd64302ed; string_plus_many(3, __v3_internal_symbol_join_4); });
	i64 underline_col = col - 1;
	if (underline_col > 0) {
		out = string__plus(out, string__repeat(_v3_lit_1_44bd4bd473cd2df9, underline_col));
	}
	out = string__plus(out, string__repeat(_v3_lit_1_44bd85d473cd9087, length));
	out = string__plus(out, _v3_lit_1_44bd31d473cd01cb);
	return out;
}

/* V3CACHE_FN_END 57422650fa5f284c */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 424ff99038fdc52a */
blip__Value blip__integer(i64 n) {
	return (blip__Value){.tag = 4, .i = n, .payload = blip__no_payload};
}

/* V3CACHE_FN_END 424ff99038fdc52a */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 97ae0ef9cf3d21f5 */
blip__Value blip__keyword(string name) {
	blip__StringPayload* __iface_src_0 = (blip__StringPayload*)memdup(&(blip__StringPayload){.tag = 128, .s = name}, sizeof(blip__StringPayload));
	return (blip__Value){.tag = 128, .payload = (blip__Payload){._typ = 979559813, ._object = __iface_src_0}};
}

/* V3CACHE_FN_END 97ae0ef9cf3d21f5 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 69d99b90f42d48f3 */
blip__Value blip__labelled(Array params, i32 body, i32 env, string name, i64 opt_from, Array opt_names, Array opt_defaults) {
	blip__Closure* __iface_src_0 = (blip__Closure*)memdup(&(blip__Closure){.params = params, .body = body, .env = env, .name = name, .arity = params.len, .opt_from = opt_from, .opt_names = opt_names, .opt_defaults = opt_defaults, .rest_name = _v3_lit_0_14650fb0739d0383}, sizeof(blip__Closure));
	return (blip__Value){.tag = 16384, .payload = (blip__Payload){._typ = 1619372465, ._object = __iface_src_0}};
}

/* V3CACHE_FN_END 69d99b90f42d48f3 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN a0ed2aeb2d7945d7 */
blip__Value blip__list_from(Array items) {
	if (items.len == 0) {
		return blip__empty_list();
	}
	blip__Value out = blip__empty_list();
	i64 i = items.len - 1;
	while (i >= 0) {
		out = blip__cons((*(blip__Value*)array_get(items, i)), out);
		i--;
	}
	return out;
}

/* V3CACHE_FN_END a0ed2aeb2d7945d7 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 54ce2f87d9899110 */
blip__Value blip__new_closure(Array params, i32 body, i32 env, string name) {
	blip__Closure* __iface_src_0 = (blip__Closure*)memdup(&(blip__Closure){.params = params, .body = body, .env = env, .name = name, .arity = params.len, .opt_from = -1, .rest_name = _v3_lit_0_14650fb0739d0383, .opt_names = array_new(sizeof(string), 0, 0), .opt_defaults = array_new(sizeof(i32), 0, 0)}, sizeof(blip__Closure));
	return (blip__Value){.tag = 16384, .payload = (blip__Payload){._typ = 1619372465, ._object = __iface_src_0}};
}

/* V3CACHE_FN_END 54ce2f87d9899110 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN f1b7d70cfc08c17 */
blip__Value blip__new_prim(string name) {
	blip__StringPayload* __iface_src_0 = (blip__StringPayload*)memdup(&(blip__StringPayload){.tag = 32768, .s = name}, sizeof(blip__StringPayload));
	return (blip__Value){.tag = 32768, .payload = (blip__Payload){._typ = 979559813, ._object = __iface_src_0}};
}

/* V3CACHE_FN_END f1b7d70cfc08c17 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 63c5afa229343bbb */
blip__Value blip__new_rest_closure(Array params, string rest_name, i32 body, i32 env, string name) {
	blip__Closure* __iface_src_0 = (blip__Closure*)memdup(&(blip__Closure){.params = params, .body = body, .env = env, .name = name, .arity = params.len, .rest = true, .rest_name = rest_name, .opt_from = -1, .opt_names = array_new(sizeof(string), 0, 0), .opt_defaults = array_new(sizeof(i32), 0, 0)}, sizeof(blip__Closure));
	return (blip__Value){.tag = 16384, .payload = (blip__Payload){._typ = 1619372465, ._object = __iface_src_0}};
}

/* V3CACHE_FN_END 63c5afa229343bbb */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 7e6fd87852cf3967 */
blip__Value blip__nil_value(void) {
	return (blip__Value){.tag = 1, .payload = blip__no_payload};
}

/* V3CACHE_FN_END 7e6fd87852cf3967 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN a4f99afc14653a2a */
blip__Value blip__rune(u32 r) {
	return (blip__Value){.tag = 16, .i = (i64)(r), .payload = blip__no_payload};
}

/* V3CACHE_FN_END a4f99afc14653a2a */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 2b0020d1fd029699 */
blip__Value blip__string(string s) {
	blip__StringPayload* __iface_src_0 = (blip__StringPayload*)memdup(&(blip__StringPayload){.tag = 32, .s = s}, sizeof(blip__StringPayload));
	return (blip__Value){.tag = 32, .payload = (blip__Payload){._typ = 979559813, ._object = __iface_src_0}};
}

/* V3CACHE_FN_END 2b0020d1fd029699 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN adb868a53181cae3 */
blip__Value blip__struct_value(string name, Array fields, map vals) {
	blip__StructVal* __iface_src_0 = (blip__StructVal*)memdup(&(blip__StructVal){.name = name, .fields = fields, .values = vals}, sizeof(blip__StructVal));
	return (blip__Value){.tag = 131072, .payload = (blip__Payload){._typ = 2048012492, ._object = __iface_src_0}};
}

/* V3CACHE_FN_END adb868a53181cae3 */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN f024d6217be097e */
blip__Value blip__symbol(string name) {
	blip__StringPayload* __iface_src_0 = (blip__StringPayload*)memdup(&(blip__StringPayload){.tag = 64, .s = name}, sizeof(blip__StringPayload));
	return (blip__Value){.tag = 64, .payload = (blip__Payload){._typ = 979559813, ._object = __iface_src_0}};
}

/* V3CACHE_FN_END f024d6217be097e */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN 9cd443b1cc696eec */
blip__Value blip__table(map m) {
	blip__Table* __iface_src_0 = (blip__Table*)memdup(&(blip__Table){.tag = 4096, .values = m}, sizeof(blip__Table));
	return (blip__Value){.tag = 4096, .payload = (blip__Payload){._typ = 1517345688, ._object = __iface_src_0}};
}

/* V3CACHE_FN_END 9cd443b1cc696eec */
/* V3CACHE_MODULE blip */
/* V3CACHE_FN_BEGIN c9555e0c642b73d3 */
blip__Value blip__vector(Array items) {
	blip__Vector* __iface_src_0 = (blip__Vector*)memdup(&(blip__Vector){.tag = 1024, .data = items}, sizeof(blip__Vector));
	return (blip__Value){.tag = 1024, .payload = (blip__Payload){._typ = 1742209763, ._object = __iface_src_0}};
}

/* V3CACHE_FN_END c9555e0c642b73d3 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 18a1fc1ce8723cc8 */
string bool__str(bool b) {
}

/* V3CACHE_FN_END 18a1fc1ce8723cc8 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN b310e5ec385d1b19 */
void builtin_init(void) {
}

/* V3CACHE_FN_END b310e5ec385d1b19 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 9f47f07530c7f6bc */
string byteptr__str(u8* nn) {
}

/* V3CACHE_FN_END 9f47f07530c7f6bc */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN d8ada0e5048190be */
Array byteptr__vbytes(u8* data, i64 len) {
}

/* V3CACHE_FN_END d8ada0e5048190be */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN d66359397b85c740 */
string byteptr__vstring(u8* bp) {
}

/* V3CACHE_FN_END d66359397b85c740 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 1b781fd9aee01b89 */
string byteptr__vstring_with_len(u8* bp, i64 len) {
}

/* V3CACHE_FN_END 1b781fd9aee01b89 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN b208e1fb839d9900 */
string char__str(char* cptr) {
}

/* V3CACHE_FN_END b208e1fb839d9900 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN d42284ffb0e7024e */
string charptr__str(char* nn) {
}

/* V3CACHE_FN_END d42284ffb0e7024e */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 89051a2808efeac6 */
string charptr__vstring(char* cp) {
}

/* V3CACHE_FN_END 89051a2808efeac6 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN c6d0f14d7d53446b */
string charptr__vstring_with_len(char* cp, i64 len) {
}

/* V3CACHE_FN_END c6d0f14d7d53446b */
/* V3CACHE_MODULE closure */
/* V3CACHE_FN_BEGIN c663baa253bc7a40 */
void* closure__closure_create_with_data(void* func, void* data, bool owns_data) {
}

/* V3CACHE_FN_END c663baa253bc7a40 */
/* V3CACHE_MODULE closure */
/* V3CACHE_FN_BEGIN a21d0b1574d1ce94 */
void closure__closure_init(void) {
}

/* V3CACHE_FN_END a21d0b1574d1ce94 */
/* V3CACHE_MODULE closure */
/* V3CACHE_FN_BEGIN eae3f4c57acf093c */
void closure__closure_try_destroy(void* closure) {
}

/* V3CACHE_FN_END eae3f4c57acf093c */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 73e6fdf2ea857acb */
i64 compare_lower_strings(string* a, string* b) {
}

/* V3CACHE_FN_END 73e6fdf2ea857acb */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN ead6305bd9ab380d */
i64 compare_strings_by_len(string* a, string* b) {
}

/* V3CACHE_FN_END ead6305bd9ab380d */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 260445d15a87def5 */
string data_to_hex_string(u8* data, i64 len) {
}

/* V3CACHE_FN_END 260445d15a87def5 */
/* V3CACHE_MODULE dl */
/* V3CACHE_FN_BEGIN bf133d3e9a659b95 */
void* dl__interface_export_find(char* iface, char* method, u32 typ) {
}

/* V3CACHE_FN_END bf133d3e9a659b95 */
/* V3CACHE_MODULE __v3_program_specializations */
/* V3CACHE_FN_BEGIN 7717952bcdd6bf61 */
void drop_owned_interface_T_IError(IError value) {
	({
		extern IError builtin__none__;
		extern IError builtin__error_sentinel;
		if ((		value)._object_is_boxed && ((		value)._object) != NULL && ((		value)._object) != builtin__none__._object && ((		value)._object) != builtin__error_sentinel._object) {
			switch ((		value)._typ) {
			case 71273906:
				string__free(&(((MessageError*)((		value)._object))->msg));
				break;
			case 310309972:
				string__free(&((*((os__NotExpected*)((		value)._object))).cause));
				break;
			default: break;
			}
			GC_FREE(((		value)._object));
			(		value)._object = NULL;
			(		value)._object_is_boxed = false;
		}
	});
}

/* V3CACHE_FN_END 7717952bcdd6bf61 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 433ff7d4230bb557 */
void drop_owned_result_error(IError err) {
}

/* V3CACHE_FN_END 433ff7d4230bb557 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 92014e975cdd30bb */
void drop_owned_result_error_interface(IError err) {
	drop_owned_interface_T_IError(err);
}

/* V3CACHE_FN_END 92014e975cdd30bb */
/* V3CACHE_MODULE __v3_program_specializations */
/* V3CACHE_FN_BEGIN 8e201f28e0ca50f5 */
void drop_owned_v3_interface_T_IError(IError value) {
	{
		IError owned = value;
		OwnershipV3InterfacePayload* raw_interface = (OwnershipV3InterfacePayload*)((void*)(&owned));
		if (raw_interface->is_boxed) {
			({
				extern IError builtin__none__;
				extern IError builtin__error_sentinel;
				if ((				owned)._object_is_boxed && ((				owned)._object) != NULL && ((				owned)._object) != builtin__none__._object && ((				owned)._object) != builtin__error_sentinel._object) {
					switch ((				owned)._typ) {
					case 71273906:
						string__free(&(((MessageError*)((				owned)._object))->msg));
						break;
					case 310309972:
						string__free(&((*((os__NotExpected*)((				owned)._object))).cause));
						break;
					default: break;
					}
					GC_FREE(((				owned)._object));
					(				owned)._object = NULL;
					(				owned)._object_is_boxed = false;
				}
			});
		}
	}
}

/* V3CACHE_FN_END 8e201f28e0ca50f5 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 6f876b630d74c899 */
inline IError error(string message) {
}

/* V3CACHE_FN_END 6f876b630d74c899 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 5361a11111080b3c */
inline IError error_with_code(string message, i64 code) {
}

/* V3CACHE_FN_END 5361a11111080b3c */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN f2073db7c92fc7f9 */
inline string f32__str(float x) {
}

/* V3CACHE_FN_END f2073db7c92fc7f9 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN a7016a52193d1988 */
inline string f64__str(double x) {
}

/* V3CACHE_FN_END a7016a52193d1988 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN d7fe0a13fca5b09a */
inline string float_literal__str(float_literal d) {
}

/* V3CACHE_FN_END d7fe0a13fca5b09a */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 6a6ca1337623052e */
bool gc_prepare_for_debugger_init(void) {
}

/* V3CACHE_FN_END 6a6ca1337623052e */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN f29671eeb270e8f8 */
void gc_restore_roots_after_debugger_init(bool use_manual_roots) {
}

/* V3CACHE_FN_END f29671eeb270e8f8 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 73c67a8a0f9fd117 */
inline void gc_runtime_init(void) {
}

/* V3CACHE_FN_END 73c67a8a0f9fd117 */
/* V3CACHE_MODULE host */
/* V3CACHE_FN_BEGIN e9db377c9287b27 */
void host__CaptureHost__give_file(host__CaptureHost* h, string path, string src) {
	{
		string __map_key_0 = path;
		string __map_val_1 = src;
		map__set(&h->state->files, &__map_key_0, &__map_val_1);
	}
}

/* V3CACHE_FN_END e9db377c9287b27 */
/* V3CACHE_MODULE host */
/* V3CACHE_FN_BEGIN 580918781b171d05 */
void host__CaptureHost__give_input(host__CaptureHost* h, Array lines) {
	{
		{
			i64 __for_idx_0 = 0;
			for (; __for_idx_0 < lines.len; __for_idx_0++) {
				string line = *(string*)(array_get(lines, __for_idx_0));
				string __arr_val_1 = string__clone(line);
				array_push(&h->state->in, &__arr_val_1);
			}
		}
	}
}

/* V3CACHE_FN_END 580918781b171d05 */
/* V3CACHE_MODULE host */
/* V3CACHE_FN_BEGIN eb96365aa5470486 */
__v_result_string host__CaptureHost__host_load(host__CaptureHost* h, string path) {
	string __map_key_0 = path;
	if (map__exists(&h->state->files, &__map_key_0)) {
		string __map_key_1 = path;
		string __map_zero_2 = _v3_lit_0_14650fb0739d0383;
		return (__v_result_string){.ok = true, .value = *(string*)(map__get(&h->state->files, &__map_key_1, &__map_zero_2))};
	}
	return (__v_result_string){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_34_86d0a58855fc88f5, path), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	return (__v_result_string){.ok = true};
}

/* V3CACHE_FN_END eb96365aa5470486 */
/* V3CACHE_MODULE host */
/* V3CACHE_FN_BEGIN d32749cab2bcd529 */
void host__CaptureHost__host_print(host__CaptureHost* h, string s) {
	{
		string __arr_val_0 = string__clone(s);
		array_push(&h->state->out, &__arr_val_0);
		h->state->raw = string__plus(h->state->raw, string__plus(s, _v3_lit_1_44bd31d473cd01cb));
	}
}

/* V3CACHE_FN_END d32749cab2bcd529 */
/* V3CACHE_MODULE host */
/* V3CACHE_FN_BEGIN 1a2605b164bae45 */
__v_option_string host__CaptureHost__host_read_line(host__CaptureHost* h) {
	{
		if (h->state->in.len == 0) {
			return (__v_option_string){.ok = false};
		}
		string first = (*((string*)((h->state->in).data) + (0)));
		h->state->in = array_slice(		h->state->in, 		1, (		h->state->in).len);
		return (__v_option_string){.ok = true, .value = first};
	}
	return (__v_option_string){.ok = true};
}

/* V3CACHE_FN_END 1a2605b164bae45 */
/* V3CACHE_MODULE host */
/* V3CACHE_FN_BEGIN 2162daac7fe61d75 */
void host__CaptureHost__host_write(host__CaptureHost* h, string s) {
	{
		h->state->raw = string__plus(h->state->raw, s);
	}
}

/* V3CACHE_FN_END 2162daac7fe61d75 */
/* V3CACHE_MODULE host */
/* V3CACHE_FN_BEGIN fc75872c222f378a */
Array host__CaptureHost__lines(host__CaptureHost* h) {
	return h->state->out;
}

/* V3CACHE_FN_END fc75872c222f378a */
/* V3CACHE_MODULE host */
/* V3CACHE_FN_BEGIN 9d59f40ecd56b295 */
string host__CaptureHost__transcript(host__CaptureHost* h) {
	return h->state->raw;
}

/* V3CACHE_FN_END 9d59f40ecd56b295 */
/* V3CACHE_MODULE host */
/* V3CACHE_FN_BEGIN 3687c31da92de6f3 */
__v_result_string host__ConsoleHost__host_load(host__ConsoleHost* c, string path) {
	return os__read_file(path);
	return (__v_result_string){.ok = true};
}

/* V3CACHE_FN_END 3687c31da92de6f3 */
/* V3CACHE_MODULE host */
/* V3CACHE_FN_BEGIN d11280ba78170a82 */
void host__ConsoleHost__host_print(host__ConsoleHost* c, string s) {
	println(s);
}

/* V3CACHE_FN_END d11280ba78170a82 */
/* V3CACHE_MODULE host */
/* V3CACHE_FN_BEGIN 5be4750d463f405a */
__v_option_string host__ConsoleHost__host_read_line(host__ConsoleHost* c) {
	return (__v_option_string){.ok = true, .value = os__get_line()};
	return (__v_option_string){.ok = true};
}

/* V3CACHE_FN_END 5be4750d463f405a */
/* V3CACHE_MODULE host */
/* V3CACHE_FN_BEGIN 3e996d95046fd2b2 */
void host__ConsoleHost__host_write(host__ConsoleHost* c, string s) {
	print(s);
}

/* V3CACHE_FN_END 3e996d95046fd2b2 */
/* V3CACHE_MODULE host */
/* V3CACHE_FN_BEGIN aee427715bffaa30 */
host__CaptureHost* host__new_capture(void) {
	return (host__CaptureHost*)memdup(&(host__CaptureHost){.state = (host__CaptureState_*)memdup(&(host__CaptureState_){.out = array_new(	sizeof(string), 0, 0), .raw = _v3_lit_0_14650fb0739d0383, .in = array_new(	sizeof(string), 0, 0), .files = new_map(sizeof(string), sizeof(string), map_hash_string, map_eq_string, map_clone_string, map_free_string)}, sizeof(host__CaptureState_))}, sizeof(host__CaptureHost));
}

/* V3CACHE_FN_END aee427715bffaa30 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN be44866083b66e44 */
string i128__str(i128 nn) {
}

/* V3CACHE_FN_END be44866083b66e44 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 917ab8a0d6b21aec */
string i16__str(i16 n) {
}

/* V3CACHE_FN_END 917ab8a0d6b21aec */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 6151b9951c4b49ba */
string i32__str(i32 n) {
}

/* V3CACHE_FN_END 6151b9951c4b49ba */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 96495f87586ad88b */
inline string i64__str(i64 nn) {
}

/* V3CACHE_FN_END 96495f87586ad88b */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 6a0b10e5d6e41423 */
string i8__str(i8 n) {
}

/* V3CACHE_FN_END 6a0b10e5d6e41423 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN ded882c2a1b108ca */
u32 impl_utf8_to_utf32(u8* _bytes, i64 _bytes_len) {
}

/* V3CACHE_FN_END ded882c2a1b108ca */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 9c679968f50b4063 */
string int__str(i64 n) {
}

/* V3CACHE_FN_END 9c679968f50b4063 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN b08d8c850c65d30b */
inline string int_literal__str(int_literal n) {
}

/* V3CACHE_FN_END b08d8c850c65d30b */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN b278596283196bce */
string isize__str(ptrdiff_t x) {
}

/* V3CACHE_FN_END b278596283196bce */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN c969be2343481e18 */
bool machine__Machine__already_loaded(machine__Machine* m, string path) {
	bool __contains_0 = false;
	{
		i64 __contains_idx_1 = 0;
		for (; __contains_idx_1 < m->loaded.len; __contains_idx_1++) {
			if (string__eq(*(string*)(array_get(m->loaded, __contains_idx_1)), path)) {
				__contains_0 = true;
			}
		}
	}
	return __contains_0;
}

/* V3CACHE_FN_END c969be2343481e18 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN e2427759b0297ee8 */
__v_result machine__Machine__any_every(machine__Machine* m, string name, Array args) {
	if (args.len < 2) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = name; __v3_internal_symbol_join_0[1] = _v3_lit_26_4ace1ee796af487d; __v3_internal_symbol_join_0[2] = int__str(args.len); string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_Array __or_opt_0 = prims__seq((*(blip__Value*)array_get(args, 1)));
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[3]; __v3_internal_symbol_join_1[0] = name; __v3_internal_symbol_join_1[1] = _v3_lit_2_9b20b700c5e4da01; __v3_internal_symbol_join_1[2] = IError__msg(&(err)); string_plus_many(3, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	Array items = __or_val_1;
	i64 i = 0;
	while (i < items.len) {
		Array one = array_new(		sizeof(blip__Value), 0, 0);
		blip__Value __arr_val_2 = (*(blip__Value*)array_get(items, i));
		array_push(&one, &__arr_val_2);
		__v_result_blip__Value __or_opt_3 = machine__Machine__call_value(m, (*(blip__Value*)array_get(args, 0)), one);
		blip__Value __or_val_4 = {0};
		if (__or_opt_3.ok) {
			__or_val_4 = __or_opt_3.value;
		} else {
			return (__v_result){.ok = false, .err = __or_opt_3.err};
		}
		blip__Value v = __or_val_4;
		if (string__eq(name, _v3_lit_4_e059a2ba657c62a4) && blip__Value__truthy(v)) {
			m->val = v;
			machine__Machine__ret(m);
			return (__v_result){.ok = true};
		}
		if (string__eq(name, _v3_lit_6_4179c85f24cdc55b) && !blip__Value__truthy(v)) {
			m->val = v;
			machine__Machine__ret(m);
			return (__v_result){.ok = true};
		}
		i++;
	}
	m->val = blip__boolean(string__eq(name, _v3_lit_6_4179c85f24cdc55b));
	machine__Machine__ret(m);
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END e2427759b0297ee8 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 50f6afd995c84c26 */
__v_result machine__Machine__apply_all(machine__Machine* m, Array all_in) {
	Array all = array__clone(&all_in);
	blip__Value callee = (*(blip__Value*)array_get(all, 0));
	Array args = array_new(	sizeof(blip__Value), 0, 0);
	{
		i64 i = 1;
		for (; i < all.len; i++) {
			blip__Value __arr_val_0 = (*(blip__Value*)array_get(all, i));
			array_push(&args, &__arr_val_0);
		}
	}
	{
		int __match_tmp_1 = callee.tag;
		if (__match_tmp_1 == 16384) {
			__v_result __or_opt_2 = machine__Machine__call_closure(m, blip__Value__as_closure(callee), args);
			if (!__or_opt_2.ok) {
				return (__v_result){.ok = false, .err = __or_opt_2.err};
			}
		} else if (__match_tmp_1 == 32768) {
			__v_result __or_opt_3 = machine__Machine__call_primitive(m, blip__Value__as_string(callee), args);
			if (!__or_opt_3.ok) {
				return (__v_result){.ok = false, .err = __or_opt_3.err};
			}
		} else if ((__match_tmp_1 == 128) || (__match_tmp_1 == 32) || (__match_tmp_1 == 64)) {
			if (args.len != 1) {
				return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_47_8b197156a682cfb9, int__str(args.len)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			__v_result_blip__Value __or_opt_4 = machine__Machine__lookup_key(m, blip__Value__as_string(callee), (*(blip__Value*)array_get(args, 0)));
			blip__Value __or_val_5 = {0};
			if (__or_opt_4.ok) {
				__or_val_5 = __or_opt_4.value;
			} else {
				return (__v_result){.ok = false, .err = __or_opt_4.err};
			}
			m->val = __or_val_5;
			machine__Machine__ret(m);
		} else if ((__match_tmp_1 == 4096) || (__match_tmp_1 == 8192)) {
			if (args.len != 1) {
				return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_49_d37bcb594e447c3e, int__str(args.len)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			__v_result_blip__Value __or_opt_6 = machine__Machine__lookup_key(m, blip__Value__as_string((*(blip__Value*)array_get(args, 0))), callee);
			blip__Value __or_val_7 = {0};
			if (__or_opt_6.ok) {
				__or_val_7 = __or_opt_6.value;
			} else {
				return (__v_result){.ok = false, .err = __or_opt_6.err};
			}
			m->val = __or_val_7;
			machine__Machine__ret(m);
		} else if ((__match_tmp_1 == 2048) || (__match_tmp_1 == 1024)) {
			if ((args.len != 1) || (((*(blip__Value*)array_get(args, 0))).tag != 4)) {
				return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_52_894a26ade82627c9, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			Array data = (blip__Value__as_vector(callee))->data;
			i64 idx = blip__Value__as_int((*(blip__Value*)array_get(args, 0)));
			if ((idx < 0) || (idx >= data.len)) {
				return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[5]; __v3_internal_symbol_join_0[0] = _v3_lit_12_5b0cdad9b5e62f6c; __v3_internal_symbol_join_0[1] = i64__str(idx); __v3_internal_symbol_join_0[2] = _v3_lit_22_ef0a88c1130ba183; __v3_internal_symbol_join_0[3] = int__str(data.len); __v3_internal_symbol_join_0[4] = _v3_lit_1_44bd52d473cd39de; string_plus_many(5, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			m->val = (*(blip__Value*)array_get(data, idx));
			machine__Machine__ret(m);
		} else {
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[3]; __v3_internal_symbol_join_1[0] = _v3_lit_13_9fb914b03e330f4; __v3_internal_symbol_join_1[1] = printer__write(callee); __v3_internal_symbol_join_1[2] = _v3_lit_16_b98d83b1d0b4a2b; string_plus_many(3, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
	}
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END 50f6afd995c84c26 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 3cef4d42f6ee09c4 */
i32 machine__Machine__apply_destructures(machine__Machine* m, machine__ParamSpec ps, i32 body) {
	Array groups = array_new(	sizeof(machine__Group), 0, 0);
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < ps.destructures.len; __for_idx_0++) {
			machine__Destructure d = *(machine__Destructure*)(array_get(ps.destructures, __for_idx_0));
			Array names = __new_array_noscan(0, 0, sizeof(i32));
			Array vals = __new_array_noscan(0, 0, sizeof(i32));
			{
				i64 __for_idx_1 = 0;
				for (; __for_idx_1 < d.parts.len; __for_idx_1++) {
					machine__Destructured p = *(machine__Destructured*)(array_get(d.parts, __for_idx_1));
					i32 __arr_val_2 = p.name;
					array_push(&names, &__arr_val_2);
					i32 __arr_val_3 = p.accessor;
					array_push(&vals, &__arr_val_3);
				}
			}
			machine__Group __arr_val_4 = (machine__Group){.params = names, .args = vals};
			array_push(&groups, &__arr_val_4);
		}
	}
	if (groups.len == 0) {
		return body;
	}
	return machine__Machine__nest_lets(m, groups, new_array_from_c_array_noscan(1, 1, sizeof(i32), (i32[1]){body}));
}

/* V3CACHE_FN_END 3cef4d42f6ee09c4 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 868e88aca790e7fc */
blip__Value machine__Machine__assemble(machine__Machine* m, i32 id, Array kids, map built) {
	reader__Datum* d = reader__Arena__node(m->arena, id);
	{
		reader__DatumTag __match_tmp_0 = d->tag;
		if (__match_tmp_0 == reader__DatumTag__nil) {
			return blip__nil_value();
		} else if (__match_tmp_0 == reader__DatumTag__bool) {
			return blip__boolean(d->i != 0);
		} else if (__match_tmp_0 == reader__DatumTag__v_int) {
			return blip__integer(d->i);
		} else if (__match_tmp_0 == reader__DatumTag__v_float) {
			return blip__float(d->f);
		} else if (__match_tmp_0 == reader__DatumTag__v_char) {
			return blip__rune((u32)(d->i));
		} else if (__match_tmp_0 == reader__DatumTag__str) {
			return blip__string(d->value);
		} else if (__match_tmp_0 == reader__DatumTag__sym) {
			return blip__symbol(d->value);
		} else if (__match_tmp_0 == reader__DatumTag__kw) {
			return blip__keyword(d->value);
		} else if (__match_tmp_0 == reader__DatumTag__quoted) {
			return machine__Machine__datum_to_value(m, (*(i32*)array_get(reader__Arena__kids(m->arena, id), 0)));
		} else if ((__match_tmp_0 == reader__DatumTag__unquote) || (__match_tmp_0 == reader__DatumTag__unquote_splice)) {
			return machine__Machine__quasi_to_value(m, id);
		} else if (__match_tmp_0 == reader__DatumTag__list) {
			Array items = array_new(			sizeof(blip__Value), 0, 0);
			i64 ki = 0;
			while (ki < kids.len) {
				i32 child = (*(i32*)array_get(kids, ki));
				reader__Datum* cd = reader__Arena__node(m->arena, child);
				if (cd->tag == reader__DatumTag__unquote_splice) {
					blip__Value spliced = machine__Machine__quasi_to_value(m, child);
					__v_result_Array __or_opt_1 = prims__seq(spliced);
					Array __or_val_2 = {0};
					if (__or_opt_1.ok) {
						__or_val_2 = __or_opt_1.value;
					} else {
						IError err = __or_opt_1.err;
						return blip__nil_value();
					}
					Array parts = __or_val_2;
					i64 si = 0;
					while (si < parts.len) {
						blip__Value __arr_val_3 = (*(blip__Value*)array_get(parts, si));
						array_push(&items, &__arr_val_3);
						si++;
					}
				} else {
					i32 __map_key_4 = child;
					blip__Value __map_zero_5 = (blip__Value){.tag = 0};
					blip__Value __arr_val_6 = *(blip__Value*)(map__get(&built, &__map_key_4, &__map_zero_5));
					array_push(&items, &__arr_val_6);
				}
				ki++;
			}
			return blip__list_from(items);
		} else if (__match_tmp_0 == reader__DatumTag__vector) {
			Array items = array_new(			sizeof(blip__Value), 0, 0);
			i64 ki = 0;
			while (ki < kids.len) {
				i32 __map_key_7 = (*(i32*)array_get(kids, ki));
				blip__Value __map_zero_8 = (blip__Value){.tag = 0};
				blip__Value __arr_val_9 = *(blip__Value*)(map__get(&built, &__map_key_7, &__map_zero_8));
				array_push(&items, &__arr_val_9);
				ki++;
			}
			return blip__vector(items);
		} else if (__match_tmp_0 == reader__DatumTag__table) {
			map mm = new_map(sizeof(string), sizeof(blip__Value), map_hash_string, map_eq_string, map_clone_string, map_free_string);
			i64 i = 0;
			while ((i + 1) < kids.len) {
				i32 __map_key_11 = (*(i32*)array_get(kids, i));
				blip__Value __map_zero_12 = (blip__Value){.tag = 0};
				string __map_key_10 = blip__Value__as_string(*(blip__Value*)(map__get(&built, &__map_key_11, &__map_zero_12)));
				i32 __map_key_14 = (*(i32*)array_get(kids, i + 1));
				blip__Value __map_zero_15 = (blip__Value){.tag = 0};
				blip__Value __map_val_13 = *(blip__Value*)(map__get(&built, &__map_key_14, &__map_zero_15));
				map__set(&mm, &__map_key_10, &__map_val_13);
				i += 2;
			}
			return blip__table(mm);
		} else if (__match_tmp_0 == reader__DatumTag__array) {
			Array items = array_new(			sizeof(blip__Value), 0, 0);
			i64 ki = 0;
			while (ki < kids.len) {
				i32 __map_key_16 = (*(i32*)array_get(kids, ki));
				blip__Value __map_zero_17 = (blip__Value){.tag = 0};
				blip__Value __arr_val_18 = *(blip__Value*)(map__get(&built, &__map_key_16, &__map_zero_17));
				array_push(&items, &__arr_val_18);
				ki++;
			}
			blip__Vector* __iface_src_19 = (blip__Vector*)memdup(&(blip__Vector){.tag = 2048, .data = items}, sizeof(blip__Vector));
			return (blip__Value){.tag = 2048, .payload = (blip__Payload){._typ = 1742209763, ._object = __iface_src_19}};
		} else if (__match_tmp_0 == reader__DatumTag__buffer) {
			map mm = new_map(sizeof(string), sizeof(blip__Value), map_hash_string, map_eq_string, map_clone_string, map_free_string);
			i64 i = 0;
			while ((i + 1) < kids.len) {
				i32 __map_key_21 = (*(i32*)array_get(kids, i));
				blip__Value __map_zero_22 = (blip__Value){.tag = 0};
				string __map_key_20 = blip__Value__as_string(*(blip__Value*)(map__get(&built, &__map_key_21, &__map_zero_22)));
				i32 __map_key_24 = (*(i32*)array_get(kids, i + 1));
				blip__Value __map_zero_25 = (blip__Value){.tag = 0};
				blip__Value __map_val_23 = *(blip__Value*)(map__get(&built, &__map_key_24, &__map_zero_25));
				map__set(&mm, &__map_key_20, &__map_val_23);
				i += 2;
			}
			return blip__buffer(mm);
		} else {
			return blip__nil_value();
		}
	}
}

/* V3CACHE_FN_END 868e88aca790e7fc */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN ebf1e39681e05137 */
i32 machine__Machine__binding_name(machine__Machine* m, i32 b) {
	if (machine__Machine__is_binding_form(m, b)) {
		Array k = reader__Arena__kids(m->arena, b);
		if (k.len > 0) {
			return (*(i32*)array_get(k, 0));
		}
	}
	return b;
}

/* V3CACHE_FN_END ebf1e39681e05137 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 3a2f6815bd5386ae */
i32 machine__Machine__binding_pattern(machine__Machine* m, i32 b) {
	return (*(i32*)array_get(reader__Arena__kids(m->arena, b), 0));
}

/* V3CACHE_FN_END 3a2f6815bd5386ae */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 3b0a0645355f381b */
i32 machine__Machine__binding_value(machine__Machine* m, i32 b) {
	if (machine__Machine__is_binding_form(m, b)) {
		Array k = reader__Arena__kids(m->arena, b);
		if (k.len > 1) {
			return (*(i32*)array_get(k, 1));
		}
	}
	return b;
}

/* V3CACHE_FN_END 3b0a0645355f381b */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 76b4b8d7036884c7 */
__v_result_blip__Value machine__Machine__build_collection(machine__Machine* m, i32 id, Array items) {
	reader__DatumTag tag = (reader__Arena__node(m->arena, id))->tag;
	if ((tag == reader__DatumTag__vector) || (tag == reader__DatumTag__array)) {
		return (__v_result_blip__Value){.ok = true, .value = blip__vector(items)};
	}
	if ((({ i64 _t1 = (i64)(items.len); i64 _t2 = (i64)(2); if (_t2 == 0) v_panic(_S("modulo by zero")); (i64)(_t1 % _t2); })) != 0) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_54_431276ef32a218e, int__str(items.len)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	map pairs = new_map(sizeof(string), sizeof(blip__Value), map_hash_string, map_eq_string, map_clone_string, map_free_string);
	i64 i = 0;
	while ((i + 1) < items.len) {
		blip__Value key = (*(blip__Value*)array_get(items, i));
		string __match_val_0 = _v3_lit_0_14650fb0739d0383;
		int __match_tmp_1 = key.tag;
		if (__match_tmp_1 == 128) {
			__match_val_0 = blip__Value__as_string(key);
		} else if (__match_tmp_1 == 32) {
			__match_val_0 = blip__Value__as_string(key);
		} else {
			return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_50_a65bb37a8f2760ad, printer__write(key)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		string ks = __match_val_0;
		string __map_key_2 = ks;
		blip__Value __map_val_3 = (*(blip__Value*)array_get(items, i + 1));
		map__set(&pairs, &__map_key_2, &__map_val_3);
		i += 2;
	}
	if (tag == reader__DatumTag__buffer) {
		return (__v_result_blip__Value){.ok = true, .value = blip__buffer(pairs)};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__table(pairs)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 76b4b8d7036884c7 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN b36a8d82d74fbdfa */
__v_result machine__Machine__call_closure(machine__Machine* m, blip__Closure* c, Array args) {
	Array use_args = args;
	if (c->opt_from >= 0) {
		__v_result_Array __or_opt_0 = machine__Machine__fill_labels(m, c, args, machine__Machine__kont_caller(m));
		Array __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result){.ok = false, .err = __or_opt_0.err};
		}
		use_args = __or_val_1;
	}
	if (c->rest) {
		if (use_args.len < c->arity) {
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[7]; __v3_internal_symbol_join_0[0] = c->name; __v3_internal_symbol_join_0[1] = _v3_lit_20_e4d543cd5e695; __v3_internal_symbol_join_0[2] = int__str(c->arity); __v3_internal_symbol_join_0[3] = _v3_lit_9_97b175148f7d162a; __v3_internal_symbol_join_0[4] = machine__plural(c->arity); __v3_internal_symbol_join_0[5] = _v3_lit_6_45ecea13c54f3ff1; __v3_internal_symbol_join_0[6] = int__str(use_args.len); string_plus_many(7, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		i32 frame = blip__EnvArena__new_env(&m->envs, c->env);
		i64 i = 0;
		while (i < c->arity) {
			blip__EnvArena__define(&m->envs, frame, (*(string*)array_get(c->params, i)), (*(blip__Value*)array_get(use_args, i)));
			i++;
		}
		blip__EnvArena__define(&m->envs, frame, c->rest_name, blip__list_from(array_slice(		use_args, 		c->arity, (		use_args).len)));
		m->env = frame;
		machine__Machine__goto(m, c->body);
		return (__v_result){.ok = true};
	}
	if (use_args.len != c->arity) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[7]; __v3_internal_symbol_join_1[0] = c->name; __v3_internal_symbol_join_1[1] = _v3_lit_11_b1208f735e965447; __v3_internal_symbol_join_1[2] = int__str(c->arity); __v3_internal_symbol_join_1[3] = _v3_lit_9_97b175148f7d162a; __v3_internal_symbol_join_1[4] = machine__plural(c->arity); __v3_internal_symbol_join_1[5] = _v3_lit_6_45ecea13c54f3ff1; __v3_internal_symbol_join_1[6] = int__str(use_args.len); string_plus_many(7, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	i32 frame = blip__EnvArena__new_env(&m->envs, c->env);
	i64 i = 0;
	while (i < c->params.len) {
		blip__EnvArena__define(&m->envs, frame, (*(string*)array_get(c->params, i)), (*(blip__Value*)array_get(use_args, i)));
		i++;
	}
	m->env = frame;
	machine__Machine__goto(m, c->body);
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END b36a8d82d74fbdfa */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 1525074b86533b9d */
i32 machine__Machine__call_node(machine__Machine* m, i32 callee, Array args) {
	Array items = __new_array_noscan(0, 0, sizeof(i32));
	i32 __arr_val_0 = callee;
	array_push(&items, &__arr_val_0);
	{
		i64 __for_idx_1 = 0;
		for (; __for_idx_1 < args.len; __for_idx_1++) {
			i32 a = *(i32*)(array_get(args, __for_idx_1));
			i32 __arr_val_2 = a;
			array_push(&items, &__arr_val_2);
		}
	}
	return machine__Machine__list_of(m, items);
}

/* V3CACHE_FN_END 1525074b86533b9d */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN d8cd9568abbcbf56 */
__v_result machine__Machine__call_primitive(machine__Machine* m, string name, Array args) {
	if (string__eq(name, _v3_lit_5_f1969731237b5c95)) {
		if (args.len < 2) {
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_40_f8681eaf0b592b41, int__str(args.len)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		Array all = array_new(		sizeof(blip__Value), 0, 0);
		blip__Value __arr_val_0 = (*(blip__Value*)array_get(args, 0));
		array_push(&all, &__arr_val_0);
		i64 i = 1;
		while (i < args.len) {
			__v_result_Array __or_opt_1 = prims__seq((*(blip__Value*)array_get(args, i)));
			Array __or_val_2 = {0};
			if (__or_opt_1.ok) {
				__or_val_2 = __or_opt_1.value;
			} else {
				IError err = __or_opt_1.err;
				blip__Value __arr_val_3 = (*(blip__Value*)array_get(args, i));
				array_push(&all, &__arr_val_3);
				i++;
				continue;
			}
			Array elems = __or_val_2;
			{
				i64 __for_idx_4 = 0;
				for (; __for_idx_4 < elems.len; __for_idx_4++) {
					blip__Value e = *(blip__Value*)(array_get(elems, __for_idx_4));
					blip__Value __arr_val_5 = e;
					array_push(&all, &__arr_val_5);
				}
			}
			i++;
		}
		__v_result __or_opt_6 = machine__Machine__apply_all(m, all);
		if (!__or_opt_6.ok) {
			return (__v_result){.ok = false, .err = __or_opt_6.err};
		}
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_5_7abebb90e73faeeb)) {
		Array __arr_lit_7 = array_new(		sizeof(blip__Value), 0, 2);
		blip__Value __arr_val_8 = blip__symbol(_v3_lit_3_2249c511fb8ec16);
		array_push(&__arr_lit_7, &__arr_val_8);
		blip__Value __arr_val_9 = blip__string(machine__format_args(args));
		array_push(&__arr_lit_7, &__arr_val_9);
		m->val = blip__list_from(__arr_lit_7);
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_5_d42707209fbb037d)) {
		i64 idx = -1;
		{
			i64 i = 0;
			for (; i < m->kstack.len; i++) {
				if (((*(blip__Kont*)array_get(m->kstack, i))).tag == 32768) {
					idx = i;
				}
			}
		}
		string msg = _v3_lit_0_14650fb0739d0383;
		if ((args.len == 1) && (((*(blip__Value*)array_get(args, 0))).tag == 256)) {
			__v_result_Array __or_opt_10 = prims__seq((*(blip__Value*)array_get(args, 0)));
			Array __or_val_11 = {0};
			if (__or_opt_10.ok) {
				__or_val_11 = __or_opt_10.value;
			} else {
				IError err = __or_opt_10.err;
				return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_41_e4a84d4f6da833ae, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			Array parts = __or_val_11;
			if (parts.len > 1) {
				msg = blip__Value__as_string((*(blip__Value*)array_get(parts, 1)));
			} else {
				msg = printer__write((*(blip__Value*)array_get(args, 0)));
			}
		} else {
			msg = machine__format_args(args);
		}
		if (idx < 0) {
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_8_d4bf6126de53209, msg), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		blip__Kont k = (*(blip__Kont*)array_get(m->kstack, idx));
		m->kstack = array_slice(		m->kstack, 0, 		idx);
		if (k.expr == blip__no_node) {
			if (k.rest == blip__no_node) {
				return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_8_d4bf6126de53209, msg), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			blip__Kont k2 = k;
			k2.slot = -4;
			m->val2 = blip__string(msg);
			machine__Machine__push(m, k2);
			m->env = k.env;
			machine__Machine__goto(m, k.rest);
			return (__v_result){.ok = true};
		}
		Array __arr_lit_12 = array_new(		sizeof(blip__Value), 0, 2);
		blip__Value __arr_val_13 = blip__symbol(_v3_lit_3_2249c511fb8ec16);
		array_push(&__arr_lit_12, &__arr_val_13);
		blip__Value __arr_val_14 = blip__string(msg);
		array_push(&__arr_lit_12, &__arr_val_14);
		m->val2 = blip__list_from(__arr_lit_12);
		return machine__Machine__try_caught(m, k);
	} else if (string__eq(name, _v3_lit_6_2ed40dcc7fa0e8ac)) {
		m->val = blip__string(machine__format_args(args));
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_5_40dc52cfa0615582) || string__eq(name, _v3_lit_7_416d70fd06c69f3)) {
		string text = _v3_lit_0_14650fb0739d0383;
		i64 i = 0;
		while (i < args.len) {
			if (i > 0) {
				text = string__plus(text, _v3_lit_1_44bd4bd473cd2df9);
			}
			text = string__plus(text, machine__show_value((*(blip__Value*)array_get(args, i)), string__eq(name, _v3_lit_5_40dc52cfa0615582)));
			i++;
		}
		string __arr_val_15 = string__clone(text);
		array_push(&m->out, &__arr_val_15);
		host__Host__host_print(&(m->host), text);
		m->val = blip__nil_value();
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_9_cae270c2d09c1b24)) {
		__v_option_string __or_opt_16 = host__Host__host_read_line(&(m->host));
		string __or_val_17 = {0};
		if (__or_opt_16.ok) {
			__or_val_17 = __or_opt_16.value;
		} else {
			__or_val_17 = _v3_lit_0_14650fb0739d0383;
		}
		string line = __or_val_17;
		m->val = blip__string(line);
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_6_95dd72fbb0a05d3e)) {
		m->val = blip__symbol(string__plus(_v3_lit_1_44bd8cd473cd9c6c, int__str(m->steps)));
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_4_f11acfd93b3db69a)) {
		string __if_val_18 = {0};
		if (args.len == 0) {
			__if_val_18 = _v3_lit_0_14650fb0739d0383;
		} else {
			__if_val_18 = machine__show_value((*(blip__Value*)array_get(args, 0)), false);
		}
		string text = __if_val_18;
		host__Host__host_print(&(m->host), text);
		if (args.len == 0) {
			m->val = blip__nil_value();
		} else {
			m->val = (*(blip__Value*)array_get(args, 0));
		}
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_3_46d7e451467792f3) || string__eq(name, _v3_lit_6_cd094ce5950ab6d) || string__eq(name, _v3_lit_6_126715d46b46abf8) || string__eq(name, _v3_lit_4_90ee9eeb4064e302) || string__eq(name, _v3_lit_8_99cfadc3c25d49ec) || string__eq(name, _v3_lit_10_f8e88b4c9f2cc737)) {
		return machine__Machine__hof(m, name, args);
	} else if (string__eq(name, _v3_lit_4_2ca5b0d314618bb0) || string__eq(name, _v3_lit_6_31de9ec2d2adf2e5)) {
		return machine__Machine__fold(m, name, args);
	} else if (string__eq(name, _v3_lit_4_e059a2ba657c62a4) || string__eq(name, _v3_lit_6_4179c85f24cdc55b)) {
		return machine__Machine__any_every(m, name, args);
	} else if (string__eq(name, _v3_lit_7_3e2e49b6ad015f3b)) {
		return machine__Machine__sort_by(m, args);
	} else if (string__eq(name, _v3_lit_2_9bce5400c6789a9d)) {
		Array items = array_new(		sizeof(blip__Value), 0, 0);
		blip__Value __arr_val_19 = blip__symbol(_v3_lit_2_9bce5400c6789a9d);
		array_push(&items, &__arr_val_19);
		{
			i64 __for_idx_20 = 0;
			for (; __for_idx_20 < args.len; __for_idx_20++) {
				blip__Value a = *(blip__Value*)(array_get(args, __for_idx_20));
				blip__Value __arr_val_21 = a;
				array_push(&items, &__arr_val_21);
			}
		}
		m->val = blip__list_from(items);
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_3_2249c511fb8ec16)) {
		Array items = array_new(		sizeof(blip__Value), 0, 0);
		blip__Value __arr_val_22 = blip__symbol(_v3_lit_3_2249c511fb8ec16);
		array_push(&items, &__arr_val_22);
		{
			i64 __for_idx_23 = 0;
			for (; __for_idx_23 < args.len; __for_idx_23++) {
				blip__Value a = *(blip__Value*)(array_get(args, __for_idx_23));
				blip__Value __arr_val_24 = a;
				array_push(&items, &__arr_val_24);
			}
		}
		m->val = blip__list_from(items);
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_3_38335f513eeec146) || string__eq(name, _v3_lit_4_5d21c6d8e73949ab)) {
		string __if_val_25 = {0};
		if (string__eq(name, _v3_lit_3_38335f513eeec146)) {
			__if_val_25 = _v3_lit_2_9bce5400c6789a9d;
		} else {
			__if_val_25 = _v3_lit_3_2249c511fb8ec16;
		}
		string kind = __if_val_25;
		m->val = blip__boolean(machine__is_result_of((*(blip__Value*)array_get(args, 0)), kind));
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_8_ab91311039da47a1) || string__eq(name, _v3_lit_9_d6b952147854006e)) {
		string __if_val_26 = {0};
		if (string__eq(name, _v3_lit_8_ab91311039da47a1)) {
			__if_val_26 = _v3_lit_2_9bce5400c6789a9d;
		} else {
			__if_val_26 = _v3_lit_3_2249c511fb8ec16;
		}
		string kind = __if_val_26;
		if (!machine__is_result_of((*(blip__Value*)array_get(args, 0)), kind)) {
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[7]; __v3_internal_symbol_join_0[0] = name; __v3_internal_symbol_join_0[1] = _v3_lit_2_9b20b700c5e4da01; __v3_internal_symbol_join_0[2] = printer__write((*(blip__Value*)array_get(args, 0))); __v3_internal_symbol_join_0[3] = _v3_lit_11_b4105fc71b06f1cf; __v3_internal_symbol_join_0[4] = kind; __v3_internal_symbol_join_0[5] = _v3_lit_15_be53b2cb10e1a1c0; __v3_internal_symbol_join_0[6] = machine__kind_of((*(blip__Value*)array_get(args, 0))); string_plus_many(7, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		__v_result_Array __or_opt_27 = prims__seq((*(blip__Value*)array_get(args, 0)));
		Array __or_val_28 = {0};
		if (__or_opt_27.ok) {
			__or_val_28 = __or_opt_27.value;
		} else {
			IError err = __or_opt_27.err;
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[3]; __v3_internal_symbol_join_1[0] = name; __v3_internal_symbol_join_1[1] = _v3_lit_2_9b20b700c5e4da01; __v3_internal_symbol_join_1[2] = IError__msg(&(err)); string_plus_many(3, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		Array rest = __or_val_28;
		i64 i = 1;
		if (i < rest.len) {
			m->val = (*(blip__Value*)array_get(rest, i));
		} else {
			m->val = blip__nil_value();
		}
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_9_fc7de50eaad59b9e)) {
		if (machine__is_result_of((*(blip__Value*)array_get(args, 0)), _v3_lit_2_9bce5400c6789a9d)) {
			__v_result_Array __or_opt_29 = prims__seq((*(blip__Value*)array_get(args, 0)));
			Array __or_val_30 = {0};
			if (__or_opt_29.ok) {
				__or_val_30 = __or_opt_29.value;
			} else {
				return (__v_result){.ok = false, .err = __or_opt_29.err};
			}
			Array rest = __or_val_30;
			if (rest.len > 1) {
				m->val = (*(blip__Value*)array_get(rest, 1));
			} else {
				m->val = blip__nil_value();
			}
		} else {
			m->val = (*(blip__Value*)array_get(args, 1));
		}
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_14_f795d06ca952a863)) {
		if (machine__is_result_of((*(blip__Value*)array_get(args, 0)), _v3_lit_2_9bce5400c6789a9d)) {
			__v_result_Array __or_opt_31 = prims__seq((*(blip__Value*)array_get(args, 0)));
			Array __or_val_32 = {0};
			if (__or_opt_31.ok) {
				__or_val_32 = __or_opt_31.value;
			} else {
				IError err = __or_opt_31.err;
				return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_16_7c66430ba35aae1, IError__msg(&(err))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			Array rest = __or_val_32;
			if (rest.len > 1) {
				blip__Value inner = (*(blip__Value*)array_get(rest, 1));
				if (machine__is_result_of(inner, _v3_lit_2_9bce5400c6789a9d) || machine__is_result_of(inner, _v3_lit_3_2249c511fb8ec16)) {
					m->val = inner;
					machine__Machine__ret(m);
					return (__v_result){.ok = true};
				}
			}
		}
		m->val = (*(blip__Value*)array_get(args, 0));
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_11_726746d68b4e7079)) {
		Array vals = array_new(		sizeof(blip__Value), 0, 0);
		i64 i = 0;
		while (i < args.len) {
			if (!machine__is_result_of((*(blip__Value*)array_get(args, i)), _v3_lit_2_9bce5400c6789a9d)) {
				m->val = (*(blip__Value*)array_get(args, i));
				machine__Machine__ret(m);
				return (__v_result){.ok = true};
			}
			__v_result_Array __or_opt_33 = prims__seq((*(blip__Value*)array_get(args, i)));
			Array __or_val_34 = {0};
			if (__or_opt_33.ok) {
				__or_val_34 = __or_opt_33.value;
			} else {
				return (__v_result){.ok = false, .err = __or_opt_33.err};
			}
			Array rest = __or_val_34;
			if (rest.len > 1) {
				blip__Value __arr_val_35 = (*(blip__Value*)array_get(rest, 1));
				array_push(&vals, &__arr_val_35);
			}
			i++;
		}
		Array __arr_lit_36 = array_new(		sizeof(blip__Value), 0, 2);
		blip__Value __arr_val_37 = blip__symbol(_v3_lit_2_9bce5400c6789a9d);
		array_push(&__arr_lit_36, &__arr_val_37);
		blip__Value __arr_val_38 = blip__vector(vals);
		array_push(&__arr_lit_36, &__arr_val_38);
		m->val = blip__list_from(__arr_lit_36);
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_10_3611d2ccdddbcd03) || string__eq(name, _v3_lit_10_bd4095b378303a32) || string__eq(name, _v3_lit_15_8ac87f40b4d713dc) || string__eq(name, _v3_lit_11_66673f96f53ffd17)) {
		return machine__Machine__result_combinator(m, name, args);
	} else {
	}
	string __map_key_41 = name;
	void* __map_ptr_42 = map__get_check(&m->prims, &__map_key_41);
	_fn_ptr_d85a1a99927adfac __map_val_43 = {0};
	if (__map_ptr_42 != NULL) {
		__map_val_43 = *(_fn_ptr_d85a1a99927adfac*)(__map_ptr_42);
	} else {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_19_f69465e8a0a70318, name), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	_fn_ptr_d85a1a99927adfac f = __map_val_43;
	Array call_args = array_new(	sizeof(blip__Value), 0, 0);
	{
		i64 __for_idx_44 = 0;
		for (; __for_idx_44 < args.len; __for_idx_44++) {
			blip__Value a = *(blip__Value*)(array_get(args, __for_idx_44));
			blip__Value __arr_val_45 = a;
			array_push(&call_args, &__arr_val_45);
		}
	}
	__v_result_blip__Value __or_opt_46 = f(call_args);
	blip__Value __or_val_47 = {0};
	if (__or_opt_46.ok) {
		__or_val_47 = __or_opt_46.value;
	} else {
		IError err = __or_opt_46.err;
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_2[3]; __v3_internal_symbol_join_2[0] = name; __v3_internal_symbol_join_2[1] = _v3_lit_2_9b20b700c5e4da01; __v3_internal_symbol_join_2[2] = IError__msg(&(err)); string_plus_many(3, __v3_internal_symbol_join_2); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	blip__Value res = __or_val_47;
	m->val = res;
	machine__Machine__ret(m);
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END d8cd9568abbcbf56 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN c27770235e11cef6 */
__v_result_blip__Value machine__Machine__call_value(machine__Machine* m, blip__Value f, Array args) {
	Array saved_k = m->kstack;
	int saved_ctl = m->ctl;
	i32 saved_form = m->form;
	i32 saved_env = m->env;
	blip__Value saved_val = m->val;
	m->kstack = array_new(	sizeof(blip__Kont), 0, 0);
	m->ctl = 1;
	m->env = m->globals;
	Array all = array_new(	sizeof(blip__Value), 0, 0);
	blip__Value __arr_val_0 = f;
	array_push(&all, &__arr_val_0);
	{
		i64 __for_idx_1 = 0;
		for (; __for_idx_1 < args.len; __for_idx_1++) {
			blip__Value a = *(blip__Value*)(array_get(args, __for_idx_1));
			blip__Value __arr_val_2 = a;
			array_push(&all, &__arr_val_2);
		}
	}
	__v_result __or_opt_3 = machine__Machine__apply_all(m, all);
	if (!__or_opt_3.ok) {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_3.err};
	}
	__v_result_blip__Value __or_opt_4 = machine__Machine__drive(m);
	blip__Value __or_val_5 = {0};
	if (__or_opt_4.ok) {
		__or_val_5 = __or_opt_4.value;
	} else {
		IError err = __or_opt_4.err;
		__or_val_5 = blip__nil_value();
	}
	blip__Value out = __or_val_5;
	m->kstack = saved_k;
	m->ctl = saved_ctl;
	m->form = saved_form;
	m->env = saved_env;
	m->val = saved_val;
	return (__v_result_blip__Value){.ok = true, .value = out};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END c27770235e11cef6 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN dd6a7d987a03e146 */
string machine__Machine__callee_name(machine__Machine* m, i32 call) {
	reader__Datum* d = reader__Arena__node(m->arena, call);
	if (d->tag == reader__DatumTag__sym) {
		return d->value;
	}
	if (d->tag == reader__DatumTag__list) {
		Array kids = reader__Arena__kids(m->arena, call);
		if (kids.len > 0) {
			reader__Datum* h = reader__Arena__node(m->arena, (*(i32*)array_get(kids, 0)));
			if (h->tag == reader__DatumTag__sym) {
				return h->value;
			}
		}
	}
	return _v3_lit_2_9bdf1600c686a476;
}

/* V3CACHE_FN_END dd6a7d987a03e146 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 86effd58426b6a59 */
blip__Value machine__Machine__closure_from_form(machine__Machine* m, i32 n) {
	return blip__new_closure(array_new(	sizeof(string), 0, 0), n, m->env, _v3_lit_7_5e6ac746f2e375f4);
}

/* V3CACHE_FN_END 86effd58426b6a59 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN d28d57679b28bf85 */
i32 machine__Machine__cond_from(machine__Machine* m, Array items) {
	if (items.len == 0) {
		return machine__Machine__nil_node(m);
	}
	Array clause = reader__Arena__kids(m->arena, (*(i32*)array_get(items, 0)));
	if (clause.len == 0) {
		return machine__Machine__nil_node(m);
	}
	reader__Datum* head = reader__Arena__node(m->arena, (*(i32*)array_get(clause, 0)));
	if ((head->tag == reader__DatumTag__sym) && string__eq(head->value, _v3_lit_4_6160ced97abcf88a)) {
		if (clause.len == 1) {
			return machine__Machine__nil_node(m);
		}
		return machine__Machine__make_begin(m, array_slice(		clause, 		1, (		clause).len));
	}
	if (clause.len == 1) {
		i32 t = machine__Machine__sym_node(m, _v3_lit_8_250a20b7982776da);
		Array __arr_lit_0 = __new_array_noscan(0, 3, sizeof(i32));
		i32 __arr_val_1 = t;
		array_push(&__arr_lit_0, &__arr_val_1);
		i32 __arr_val_2 = t;
		array_push(&__arr_lit_0, &__arr_val_2);
		i32 __arr_val_3 = machine__Machine__nil_node(m);
		array_push(&__arr_lit_0, &__arr_val_3);
		i32 body = machine__Machine__node_of(m, _v3_lit_2_9bc77700c672b768, __arr_lit_0);
		Array __arr_lit_4 = __new_array_noscan(0, 2, sizeof(i32));
		i32 __arr_val_5 = machine__Machine__list_of(m, new_array_from_c_array_noscan(1, 1, sizeof(i32), (i32[1]){t}));
		array_push(&__arr_lit_4, &__arr_val_5);
		i32 __arr_val_6 = body;
		array_push(&__arr_lit_4, &__arr_val_6);
		i32 lam = machine__Machine__node_of(m, _v3_lit_6_497dd6d1afd39ae4, __arr_lit_4);
		Array __arr_lit_7 = __new_array_noscan(0, 1, sizeof(i32));
		i32 __arr_val_8 = (*(i32*)array_get(clause, 0));
		array_push(&__arr_lit_7, &__arr_val_8);
		return machine__Machine__call_node(m, lam, __arr_lit_7);
	}
	if ((clause.len == 3) && ((reader__Arena__node(m->arena, (*(i32*)array_get(clause, 1))))->tag == reader__DatumTag__sym) && string__eq((reader__Arena__node(m->arena, (*(i32*)array_get(clause, 1))))->value, _v3_lit_2_9b319f00c5f3246c)) {
		i32 f = machine__Machine__sym_node(m, _v3_lit_8_250a0eb798275844);
		i32 rest = machine__Machine__cond_from(m, array_slice(		items, 		1, (		items).len));
		Array __arr_lit_9 = __new_array_noscan(0, 3, sizeof(i32));
		i32 __arr_val_10 = (*(i32*)array_get(clause, 0));
		array_push(&__arr_lit_9, &__arr_val_10);
		i32 __arr_val_11 = machine__Machine__call_node(m, f, __new_array_noscan(0, 0, sizeof(i32)));
		array_push(&__arr_lit_9, &__arr_val_11);
		i32 __arr_val_12 = rest;
		array_push(&__arr_lit_9, &__arr_val_12);
		i32 body = machine__Machine__node_of(m, _v3_lit_2_9bc77700c672b768, __arr_lit_9);
		Array __arr_lit_13 = __new_array_noscan(0, 2, sizeof(i32));
		i32 __arr_val_14 = machine__Machine__list_of(m, new_array_from_c_array_noscan(1, 1, sizeof(i32), (i32[1]){f}));
		array_push(&__arr_lit_13, &__arr_val_14);
		i32 __arr_val_15 = body;
		array_push(&__arr_lit_13, &__arr_val_15);
		i32 lam = machine__Machine__node_of(m, _v3_lit_6_497dd6d1afd39ae4, __arr_lit_13);
		Array __arr_lit_16 = __new_array_noscan(0, 1, sizeof(i32));
		i32 __arr_val_17 = (*(i32*)array_get(clause, 2));
		array_push(&__arr_lit_16, &__arr_val_17);
		return machine__Machine__call_node(m, lam, __arr_lit_16);
	}
	i32 rest = machine__Machine__cond_from(m, array_slice(	items, 	1, (	items).len));
	Array __arr_lit_18 = __new_array_noscan(0, 3, sizeof(i32));
	i32 __arr_val_19 = (*(i32*)array_get(clause, 0));
	array_push(&__arr_lit_18, &__arr_val_19);
	i32 __arr_val_20 = machine__Machine__make_begin(m, array_slice(	clause, 	1, (	clause).len));
	array_push(&__arr_lit_18, &__arr_val_20);
	i32 __arr_val_21 = rest;
	array_push(&__arr_lit_18, &__arr_val_21);
	return machine__Machine__node_of(m, _v3_lit_2_9bc77700c672b768, __arr_lit_18);
}

/* V3CACHE_FN_END d28d57679b28bf85 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 434c54f4ed91fc6d */
blip__Value machine__Machine__datum_to_value(machine__Machine* m, i32 id) {
	map built = new_map(sizeof(i32), sizeof(blip__Value), map_hash_int_4, map_eq_int_4, map_clone_int_4, map_free_nop);
	Array work = array_new(	sizeof(machine__Frame), 0, 0);
	machine__Frame __arr_val_0 = (machine__Frame){.id = id, .kids = reader__Arena__kids(m->arena, id), .start = 0};
	array_push(&work, &__arr_val_0);
	while (work.len > 0) {
		machine__Frame f = (*(machine__Frame*)array_get(work, work.len - 1));
		work = array_slice(		work, 0, 		work.len - 1);
		if (f.start < f.kids.len) {
			machine__Frame __arr_val_1 = (machine__Frame){.id = f.id, .kids = f.kids, .start = f.start + 1};
			array_push(&work, &__arr_val_1);
			machine__Frame __arr_val_2 = (machine__Frame){.id = (*(i32*)array_get(f.kids, f.start)), .kids = __new_array_noscan(0, 0, sizeof(i32)), .start = 0};
			array_push(&work, &__arr_val_2);
			continue;
		}
		i32 __map_key_3 = f.id;
		blip__Value __map_val_4 = machine__Machine__assemble(m, f.id, f.kids, built);
		map__set(&built, &__map_key_3, &__map_val_4);
	}
	i32 __map_key_5 = id;
	blip__Value __map_zero_6 = (blip__Value){.tag = 0};
	return *(blip__Value*)(map__get(&built, &__map_key_5, &__map_zero_6));
}

/* V3CACHE_FN_END 434c54f4ed91fc6d */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN b6092df9a4c7c552 */
string machine__Machine__decorate_error(machine__Machine* m, string msg) {
	if (string__eq(m->src_text, _v3_lit_0_14650fb0739d0383) || (m->form == blip__no_node)) {
		return msg;
	}
	reader__Datum* d = reader__Arena__node(m->arena, m->form);
	if (d->line == 0) {
		return msg;
	}
	i64 length = 1;
	if (string__starts_with(msg, _v3_lit_20_f0a7b475d116260d)) {
		length = msg.len - 20;
	} else if (string__starts_with(msg, _v3_lit_42_d129e9fc51bfae69)) {
		string prefix = _v3_lit_42_d129e9fc51bfae69;
		length = msg.len - 42;
	}
	return blip__format_error(machine__Machine__where(m), m->src_text, d->line, d->col, msg, length);
}

/* V3CACHE_FN_END b6092df9a4c7c552 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 679e043de0329263 */
__v_result_Array machine__Machine__destructuring(machine__Machine* m, i32 pattern, i32 value) {
	reader__Datum* d = reader__Arena__node(m->arena, pattern);
	if (d->tag == reader__DatumTag__sym) {
		if (string__eq(d->value, _v3_lit_1_44bd84d473cd8ed4)) {
			return (__v_result_Array){.ok = true, .value = array_new(			sizeof(machine__Destructured), 0, 0)};
		}
		Array __arr_lit_0 = array_new(		sizeof(machine__Destructured), 0, 1);
		machine__Destructured __arr_val_1 = (machine__Destructured){.name = pattern, .accessor = value};
		array_push(&__arr_lit_0, &__arr_val_1);
		return (__v_result_Array){.ok = true, .value = __arr_lit_0};
	}
	if ((d->tag == reader__DatumTag__kw) || (d->tag == reader__DatumTag__v_int) || (d->tag == reader__DatumTag__str)) {
		return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(machine__Machine__render_node(m, pattern), _v3_lit_51_7bb9acc08615637), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	if (d->tag == reader__DatumTag__table) {
		Array kids = reader__Arena__kids(m->arena, pattern);
		Array out = array_new(		sizeof(machine__Destructured), 0, 0);
		i64 i = 0;
		while ((i + 1) < kids.len) {
			reader__Datum* k = reader__Arena__node(m->arena, (*(i32*)array_get(kids, i)));
			if (k->tag != reader__DatumTag__kw) {
				return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_48_330e6c49245a6046, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			Array __arr_lit_2 = __new_array_noscan(0, 2, sizeof(i32));
			i32 __arr_val_3 = value;
			array_push(&__arr_lit_2, &__arr_val_3);
			i32 __arr_val_4 = (*(i32*)array_get(kids, i));
			array_push(&__arr_lit_2, &__arr_val_4);
			machine__Destructured __arr_val_5 = (machine__Destructured){.name = (*(i32*)array_get(kids, i + 1)), .accessor = machine__Machine__call_node(m, machine__Machine__sym_node(m, _v3_lit_3_f02b6251155be40d), __arr_lit_2)};
			array_push(&out, &__arr_val_5);
			i += 2;
		}
		return (__v_result_Array){.ok = true, .value = out};
	}
	if (d->tag != reader__DatumTag__list) {
		return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(machine__Machine__render_node(m, pattern), _v3_lit_34_a137a20ed89a4c52), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	Array kids = reader__Arena__kids(m->arena, pattern);
	if (kids.len == 0) {
		return (__v_result_Array){.ok = true, .value = array_new(		sizeof(machine__Destructured), 0, 0)};
	}
	string head = machine__Machine__pattern_head(m, pattern);
	if (string__eq(head, _v3_lit_4_e6cd8fa8beffb8ce)) {
		if (kids.len != 3) {
			return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_40_d915a60e4a6944cf, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		Array __arr_lit_6 = array_new(		sizeof(machine__Destructured), 0, 2);
		machine__Destructured __arr_val_7 = (machine__Destructured){.name = (*(i32*)array_get(kids, 1)), .accessor = machine__Machine__call_node(m, machine__Machine__sym_node(m, _v3_lit_3_cf45a85103385d33), new_array_from_c_array_noscan(1, 1, sizeof(i32), (i32[1]){value}))};
		array_push(&__arr_lit_6, &__arr_val_7);
		machine__Destructured __arr_val_8 = (machine__Destructured){.name = (*(i32*)array_get(kids, 2)), .accessor = machine__Machine__call_node(m, machine__Machine__sym_node(m, _v3_lit_3_cf50be5103428a1a), new_array_from_c_array_noscan(1, 1, sizeof(i32), (i32[1]){value}))};
		array_push(&__arr_lit_6, &__arr_val_8);
		return (__v_result_Array){.ok = true, .value = __arr_lit_6};
	}
	if (string__eq(head, _v3_lit_4_a1c80526911571eb) || string__eq(head, _v3_lit_6_a652a2d3592ea6e0) || string__eq(head, _v3_lit_0_14650fb0739d0383)) {
		Array out = array_new(		sizeof(machine__Destructured), 0, 0);
		i64 i = 1;
		while (i < kids.len) {
			Array __arr_lit_11 = __new_array_noscan(0, 2, sizeof(i32));
			i32 __arr_val_12 = value;
			array_push(&__arr_lit_11, &__arr_val_12);
			i32 __arr_val_13 = machine__Machine__int_node(m, (i64)(i - 1));
			array_push(&__arr_lit_11, &__arr_val_13);
			__v_result_Array __or_opt_9 = machine__Machine__destructuring(m, (*(i32*)array_get(kids, i)), machine__Machine__call_node(m, machine__Machine__sym_node(m, _v3_lit_8_b823e33ad085a91b), __arr_lit_11));
			Array __or_val_10 = {0};
			if (__or_opt_9.ok) {
				__or_val_10 = __or_opt_9.value;
			} else {
				return (__v_result_Array){.ok = false, .err = __or_opt_9.err};
			}
			array__push_many(&out, __or_val_10.data, __or_val_10.len);
			i++;
		}
		return (__v_result_Array){.ok = true, .value = out};
	}
	if (string__eq(head, _v3_lit_6_14da8f5ec8315a0a)) {
		if (kids.len < 2) {
			return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_47_3a69f10856a6b645, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		reader__Datum* nm = reader__Arena__node(m->arena, (*(i32*)array_get(kids, 1)));
		if (nm->tag != reader__DatumTag__sym) {
			return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_50_751630a401949a87, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		Array out = array_new(		sizeof(machine__Destructured), 0, 0);
		i64 i = 2;
		while (i < kids.len) {
			reader__Datum* f = reader__Arena__node(m->arena, (*(i32*)array_get(kids, i)));
			string fname = _v3_lit_0_14650fb0739d0383;
			if (f->tag == reader__DatumTag__kw) {
				fname = f->value;
			} else if ((f->tag == reader__DatumTag__sym) && string__ends_with(f->value, _v3_lit_1_44bd61d473cd535b)) {
				fname = string__substr(				f->value, 0, 				f->value.len - 1);
			} else {
				return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_59_1a0b0d87736dac9b, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			if ((i + 1) >= kids.len) {
				return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_6_a176b8975d542a87; __v3_internal_symbol_join_0[1] = fname; __v3_internal_symbol_join_0[2] = _v3_lit_12_b4a90bede6b06415; string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			__v_result_Array __or_opt_14 = machine__Machine__destructuring(m, (*(i32*)array_get(kids, i + 1)), machine__Machine__call_node(m, machine__Machine__sym_node(m, ({ string __v3_internal_symbol_join_1[3]; __v3_internal_symbol_join_1[0] = string__to_lower(nm->value); __v3_internal_symbol_join_1[1] = _v3_lit_1_44bd56d473cd40aa; __v3_internal_symbol_join_1[2] = fname; string_plus_many(3, __v3_internal_symbol_join_1); })), new_array_from_c_array_noscan(1, 1, sizeof(i32), (i32[1]){value})));
			Array __or_val_15 = {0};
			if (__or_opt_14.ok) {
				__or_val_15 = __or_opt_14.value;
			} else {
				return (__v_result_Array){.ok = false, .err = __or_opt_14.err};
			}
			array__push_many(&out, __or_val_15.data, __or_val_15.len);
			i += 2;
		}
		return (__v_result_Array){.ok = true, .value = out};
	}
	return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(machine__Machine__render_node(m, pattern), _v3_lit_34_a137a20ed89a4c52), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	return (__v_result_Array){.ok = true};
}

/* V3CACHE_FN_END 679e043de0329263 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN a1b1197d57c0d702 */
__v_result_blip__Value machine__Machine__drive(machine__Machine* m) {
	for (;;) {
		m->steps++;
		if (m->steps > m->max_steps) {
			return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_50_40465beb1f522fb7, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		if (m->kstack.len > m->max_kont) {
			return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_53_3310abdbe6ff9916, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		if (m->ctl == 1) {
			__v_result __or_opt_0 = machine__Machine__step_eval(m);
			if (!__or_opt_0.ok) {
				IError err = __or_opt_0.err;
				return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = machine__Machine__decorate_error(m, IError__msg(&(err))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
		} else {
			__v_result_bool __or_opt_1 = machine__Machine__step_return(m);
			bool __or_val_2 = {0};
			if (__or_opt_1.ok) {
				__or_val_2 = __or_opt_1.value;
			} else {
				return (__v_result_blip__Value){.ok = false, .err = __or_opt_1.err};
			}
			if (!__or_val_2) {
				return (__v_result_blip__Value){.ok = true, .value = m->val};
			}
		}
	}
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END a1b1197d57c0d702 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 6a344e2529d53b52 */
blip__Value machine__Machine__empty_collection(machine__Machine* m, i32 id) {
	if ((reader__Arena__node(m->arena, id))->tag == reader__DatumTag__table) {
		return blip__table(new_map(sizeof(string), sizeof(blip__Value), map_hash_string, map_eq_string, map_clone_string, map_free_string));
	}
	return blip__vector(array_new(	sizeof(blip__Value), 0, 0));
}

/* V3CACHE_FN_END 6a344e2529d53b52 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN e2594eedd7e22a27 */
__v_result machine__Machine__enter_clause_body(machine__Machine* m, i32 frame, i32 body) {
	reader__Datum* bd = reader__Arena__node(m->arena, body);
	if (bd->tag != reader__DatumTag__list) {
		m->env = frame;
		machine__Machine__goto(m, body);
		return (__v_result){.ok = true};
	}
	Array bkids = reader__Arena__kids(m->arena, body);
	if (bkids.len > 0) {
		reader__Datum* head = reader__Arena__node(m->arena, (*(i32*)array_get(bkids, 0)));
		if ((head->tag != reader__DatumTag__sym) || !string__eq(head->value, _v3_lit_5_178c76c75eded44)) {
			m->env = frame;
			machine__Machine__goto(m, body);
			return (__v_result){.ok = true};
		}
	}
	if (bkids.len <= 1) {
		m->env = frame;
		m->val = blip__empty_list();
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	}
	if (bkids.len == 2) {
		m->env = frame;
		machine__Machine__goto(m, (*(i32*)array_get(bkids, 1)));
		return (__v_result){.ok = true};
	}
	blip__Kont nf = machine__Machine__kont(m, 16);
	nf.rest = body;
	nf.env = frame;
	nf.slot = 1;
	m->env = frame;
	machine__Machine__push(m, nf);
	machine__Machine__goto(m, (*(i32*)array_get(bkids, 1)));
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END e2594eedd7e22a27 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN c5335e06947f021f */
__v_result machine__Machine__eval_collection(machine__Machine* m, i32 id) {
	Array ekids = reader__Arena__kids(m->arena, id);
	if (ekids.len == 0) {
		m->val = machine__Machine__empty_collection(m, id);
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	}
	blip__Kont cf = machine__Machine__kont(m, 16384);
	cf.rest = id;
	cf.expr = id;
	cf.slot = 0;
	cf.acc = array_new(	sizeof(blip__Value), 0, 0);
	cf.env = m->env;
	machine__Machine__push(m, cf);
	machine__Machine__goto(m, (*(i32*)array_get(ekids, 0)));
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END c5335e06947f021f */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN e3c7b8a83ca474b8 */
__v_result_blip__Value machine__Machine__eval_default(machine__Machine* m, i32 n, Array args) {
	Array a = array_new(	sizeof(blip__Value), 0, 0);
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < args.len; __for_idx_0++) {
			blip__Value x = *(blip__Value*)(array_get(args, __for_idx_0));
			blip__Value __arr_val_1 = x;
			array_push(&a, &__arr_val_1);
		}
	}
	Array all = array_new(	sizeof(blip__Value), 0, 0);
	blip__Value __arr_val_2 = machine__Machine__closure_from_form(m, n);
	array_push(&all, &__arr_val_2);
	{
		i64 __for_idx_3 = 0;
		for (; __for_idx_3 < a.len; __for_idx_3++) {
			blip__Value x = *(blip__Value*)(array_get(a, __for_idx_3));
			blip__Value __arr_val_4 = x;
			array_push(&all, &__arr_val_4);
		}
	}
	return machine__Machine__call_value(m, (*(blip__Value*)array_get(all, 0)), array_slice(	all, 	1, (	all).len));
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END e3c7b8a83ca474b8 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN d144fabd0e3e31ce */
__v_result machine__Machine__eval_define(machine__Machine* m, Array kids) {
	if (kids.len < 2) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_45_b8ce7a3d8367c527, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	reader__Datum* target = reader__Arena__node(m->arena, (*(i32*)array_get(kids, 1)));
	if (target->tag == reader__DatumTag__sym) {
		if (kids.len < 3) {
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_25_7560fb28c8864d70, target->value), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		blip__Kont nf = machine__Machine__kont(m, 32);
		nf.name = target->value;
		nf.env = m->env;
		machine__Machine__push(m, nf);
		machine__Machine__goto(m, (*(i32*)array_get(kids, 2)));
		return (__v_result){.ok = true};
	}
	if (target->tag == reader__DatumTag__list) {
		Array sig = reader__Arena__kids(m->arena, (*(i32*)array_get(kids, 1)));
		if ((sig.len == 0) || ((reader__Arena__node(m->arena, (*(i32*)array_get(sig, 0))))->tag != reader__DatumTag__sym)) {
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_19_d2b65a373f8ec725, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		string fname = (reader__Arena__node(m->arena, (*(i32*)array_get(sig, 0))))->value;
		__v_result_machine__ParamSpec __or_opt_0 = machine__Machine__parse_params(m, array_slice(		sig, 		1, (		sig).len), fname);
		machine__ParamSpec __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result){.ok = false, .err = __or_opt_0.err};
		}
		machine__ParamSpec spec = __or_val_1;
		i32 body = machine__Machine__apply_destructures(m, spec, machine__Machine__make_begin(m, array_slice(		kids, 		2, (		kids).len)));
		blip__EnvArena__define(&m->envs, m->env, fname, machine__Machine__make_closure(m, spec, body, m->env, fname));
		m->val = blip__symbol(fname);
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	}
	return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_45_b8ce7a3d8367c527, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END d144fabd0e3e31ce */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 9653724b8bf34408 */
__v_result machine__Machine__eval_defmacro(machine__Machine* m, Array kids) {
	if (kids.len < 4) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_79_aaafad96d483022c, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	reader__Datum* name_node = reader__Arena__node(m->arena, (*(i32*)array_get(kids, 1)));
	if (name_node->tag != reader__DatumTag__sym) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_21_2d82929f086f423, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	string name = name_node->value;
	__v_result_machine__ParamSpec __or_opt_0 = machine__Machine__parse_params(m, reader__Arena__kids(m->arena, (*(i32*)array_get(kids, 2))), name);
	machine__ParamSpec __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result){.ok = false, .err = __or_opt_0.err};
	}
	machine__ParamSpec spec = __or_val_1;
	i32 body = machine__Machine__apply_destructures(m, spec, machine__Machine__make_begin(m, array_slice(	kids, 	3, (	kids).len)));
	blip__EnvArena__define(&m->envs, m->env, name, machine__Machine__make_closure(m, spec, body, m->env, name));
	m->val = blip__symbol(name);
	machine__Machine__ret(m);
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END 9653724b8bf34408 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN bd08d4ebd811d082 */
__v_result machine__Machine__eval_field_access(machine__Machine* m, string what) {
	__v_option_i64 __or_opt_0 = string__last_index(what, _v3_lit_1_44bd55d473cd3ef7);
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(what, _v3_lit_22_925195d689b3a9c3), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	i64 dot = __or_val_1;
	string base_name = string__substr(	what, 0, 	dot);
	string field = string__substr(	what, 	dot + 1, (	what).len);
	__v_option_blip__Value __if_guard_2 = blip__EnvArena__lookup(&m->envs, m->env, base_name);
	if (__if_guard_2.ok) {
		blip__Value v = __if_guard_2.value;
		__v_result_blip__Value __or_opt_3 = machine__Machine__field_of(m, v, field);
		blip__Value __or_val_4 = {0};
		if (__or_opt_3.ok) {
			__or_val_4 = __or_opt_3.value;
		} else {
			return (__v_result){.ok = false, .err = __or_opt_3.err};
		}
		m->val = __or_val_4;
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	}
	return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[5]; __v3_internal_symbol_join_0[0] = _v3_lit_20_f0a7b475d116260d; __v3_internal_symbol_join_0[1] = base_name; __v3_internal_symbol_join_0[2] = _v3_lit_18_389a818e38edfcea; __v3_internal_symbol_join_0[3] = what; __v3_internal_symbol_join_0[4] = _v3_lit_1_44bd52d473cd39de; string_plus_many(5, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END bd08d4ebd811d082 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN ea51ab63af419c17 */
__v_result machine__Machine__eval_field_update(machine__Machine* m, string what, i32 value_node) {
	__v_option_i64 __or_opt_0 = string__last_index(what, _v3_lit_1_44bd55d473cd3ef7);
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(what, _v3_lit_22_f94af3cc5b978b0c), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	i64 dot = __or_val_1;
	string base_name = string__substr(	what, 0, 	dot);
	string field = string__substr(	what, 	dot + 1, (	what).len);
	__v_option_blip__Value __or_opt_2 = blip__EnvArena__lookup(&m->envs, m->env, base_name);
	blip__Value __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[5]; __v3_internal_symbol_join_0[0] = _v3_lit_20_f0a7b475d116260d; __v3_internal_symbol_join_0[1] = base_name; __v3_internal_symbol_join_0[2] = _v3_lit_5_5d2582c7b6acfdf0; __v3_internal_symbol_join_0[3] = what; __v3_internal_symbol_join_0[4] = _v3_lit_1_44bd52d473cd39de; string_plus_many(5, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	blip__Value base = __or_val_3;
	blip__Kont nf = machine__Machine__kont(m, 512);
	nf.name = field;
	nf.acc = array_new(	sizeof(blip__Value), 0, 0);
	blip__Value __arr_val_4 = base;
	array_push(&nf.acc, &__arr_val_4);
	machine__Machine__push(m, nf);
	machine__Machine__goto(m, value_node);
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END ea51ab63af419c17 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN bff9da5836bec658 */
__v_result machine__Machine__eval_import(machine__Machine* m, Array kids) {
	if (kids.len < 2) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_48_cd0a1070476a5d81, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	reader__Datum* d = reader__Arena__node(m->arena, (*(i32*)array_get(kids, 1)));
	string name = string__trim_space(d->value);
	if (string__eq(name, _v3_lit_0_14650fb0739d0383)) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_48_cd0a1070476a5d81, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_option_string __or_opt_0 = machine__Machine__resolve_module(m, name);
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		string __if_val_2 = {0};
		if (string__contains(name, _v3_lit_1_44bd54d473cd3d44)) {
			__if_val_2 = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_35_9c5d37ee5f7cf9bf; __v3_internal_symbol_join_0[1] = name; __v3_internal_symbol_join_0[2] = _v3_lit_1_44bd8bd473cd9ab9; string_plus_many(3, __v3_internal_symbol_join_0); });
		} else {
			__if_val_2 = _v3_lit_32_8b11618169752c9e;
		}
		string hint = __if_val_2;
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[4]; __v3_internal_symbol_join_1[0] = _v3_lit_25_c9afc074fc527c46; __v3_internal_symbol_join_1[1] = name; __v3_internal_symbol_join_1[2] = _v3_lit_4_ef98beaa97559659; __v3_internal_symbol_join_1[3] = hint; string_plus_many(4, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	string path = __or_val_1;
	__v_result_blip__Value __or_opt_3 = machine__Machine__load(m, path);
	if (!__or_opt_3.ok) {
		return (__v_result){.ok = false, .err = __or_opt_3.err};
	}
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END bff9da5836bec658 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 1a8d222cdb456e6a */
__v_result machine__Machine__eval_lambda(machine__Machine* m, Array kids) {
	if (kids.len < 2) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_29_7c3e0b3741c9508f, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	Array plist = reader__Arena__kids(m->arena, (*(i32*)array_get(kids, 1)));
	__v_result_machine__ParamSpec __or_opt_0 = machine__Machine__parse_params(m, plist, _v3_lit_6_497dd6d1afd39ae4);
	machine__ParamSpec __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result){.ok = false, .err = __or_opt_0.err};
	}
	machine__ParamSpec spec = __or_val_1;
	i32 body = machine__Machine__apply_destructures(m, spec, machine__Machine__make_begin(m, array_slice(	kids, 	2, (	kids).len)));
	m->val = machine__Machine__make_closure(m, spec, body, m->env, _v3_lit_6_497dd6d1afd39ae4);
	machine__Machine__ret(m);
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END 1a8d222cdb456e6a */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 7c1da725469daf7b */
__v_result machine__Machine__eval_list(machine__Machine* m, i32 id, Array kids) {
	reader__Datum* head = reader__Arena__node(m->arena, (*(i32*)array_get(kids, 0)));
	if (head->tag == reader__DatumTag__sym) {
		if (machine__special_form(head->value)) {
			return machine__Machine__eval_special(m, head->value, id, kids);
		}
		if (string__contains(head->value, _v3_lit_1_44bd55d473cd3ef7)) {
			if ((kids.len == 3) && machine__Machine__is_update_marker(m, (*(i32*)array_get(kids, 1)))) {
				return machine__Machine__eval_field_update(m, head->value, (*(i32*)array_get(kids, 2)));
			}
			return machine__Machine__eval_field_access(m, head->value);
		}
	}
	blip__Kont nf = machine__Machine__kont(m, 2);
	nf.rest = id;
	nf.env = m->env;
	machine__Machine__push(m, nf);
	machine__Machine__goto(m, (*(i32*)array_get(kids, 0)));
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END 7c1da725469daf7b */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 1067eee2205589ec */
__v_result machine__Machine__eval_match(machine__Machine* m, Array kids) {
	if (kids.len < 2) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_45_31eec0c4f5819572, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	blip__Kont nf = machine__Machine__kont(m, 4096);
	nf.rest = 0;
	nf.slot = 0;
	Array __arr_lit_0 = __new_array_noscan(0, 0, sizeof(i32));
	nf.clauses = __arr_lit_0;
	i64 i = 2;
	while (i < kids.len) {
		i32 __arr_val_1 = (*(i32*)array_get(kids, i));
		array_push(&nf.clauses, &__arr_val_1);
		i++;
	}
	nf.env = m->env;
	machine__Machine__push(m, nf);
	machine__Machine__goto(m, (*(i32*)array_get(kids, 1)));
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END 1067eee2205589ec */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN ff2abc4518527477 */
__v_result_blip__Value machine__Machine__eval_one(machine__Machine* m, i32 f) {
	machine__Machine__reset(m);
	m->form = f;
	m->ctl = 1;
	return machine__Machine__drive(m);
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END ff2abc4518527477 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 743434f1aae68d53 */
__v_result machine__Machine__eval_pipe(machine__Machine* m, string name, Array kids) {
	if (kids.len < 2) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(name, _v3_lit_24_a29c06cf49e87a0b), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	i32 acc = (*(i32*)array_get(kids, 1));
	i64 i = 2;
	while (i < kids.len) {
		i32 step = (*(i32*)array_get(kids, i));
		reader__Datum* d = reader__Arena__node(m->arena, step);
		if ((d->tag == reader__DatumTag__list) && machine__Machine__is_pipe_head(m, step)) {
			Array inner = reader__Arena__kids(m->arena, step);
			i64 j = 2;
			while (j < inner.len) {
				acc = machine__Machine__pipe_one(m, acc, (*(i32*)array_get(inner, j)), name);
				j++;
			}
			i++;
			continue;
		}
		acc = machine__Machine__pipe_one(m, acc, step, name);
		i++;
	}
	machine__Machine__goto(m, acc);
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END 743434f1aae68d53 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 925698905f325b8e */
__v_result machine__Machine__eval_require(machine__Machine* m, Array kids) {
	Array paths = array_new(	sizeof(string), 0, 0);
	i64 i = 1;
	while (i < kids.len) {
		Array part = machine__Machine__require_paths(m, (*(i32*)array_get(kids, i)));
		i64 j = 0;
		while (j < part.len) {
			string __arr_val_0 = string__clone((*(string*)array_get(part, j)));
			array_push(&paths, &__arr_val_0);
			j++;
		}
		if (part.len == 0) {
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_9_933254007b6730f4; __v3_internal_symbol_join_0[1] = machine__Machine__render_node(m, (*(i32*)array_get(kids, i))); __v3_internal_symbol_join_0[2] = _v3_lit_45_3d6dbd21f2b0dab6; string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		i++;
	}
	if (paths.len == 0) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_27_53207dbea77125b7, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	i64 k = 0;
	while (k < paths.len) {
		string full = machine__Machine__resolve_path(m, (*(string*)array_get(paths, k)));
		if (!machine__Machine__already_loaded(m, full)) {
			__v_result_blip__Value __or_opt_1 = machine__Machine__load(m, full);
			if (!__or_opt_1.ok) {
				IError err = __or_opt_1.err;
				return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_9_933254007b6730f4, IError__msg(&(err))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			string __arr_val_2 = string__clone(full);
			array_push(&m->loaded, &__arr_val_2);
		}
		k++;
	}
	m->val = blip__nil_value();
	machine__Machine__ret(m);
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END 925698905f325b8e */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 63438678b682331a */
__v_result machine__Machine__eval_special(machine__Machine* m, string name, i32 id, Array kids) {
	if (string__eq(name, _v3_lit_5_e49fbd9ccfe8a8c5)) {
		m->val = machine__Machine__datum_to_value(m, (*(i32*)array_get(kids, 1)));
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_2_9bc77700c672b768)) {
		if (kids.len < 3) {
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_60_55597e79ecc5f0ec, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		blip__Kont nf = machine__Machine__kont(m, 8);
		nf.rest = id;
		nf.env = m->env;
		nf.env = m->env;
		machine__Machine__push(m, nf);
		machine__Machine__goto(m, (*(i32*)array_get(kids, 1)));
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_6_db03959661e15c48)) {
		return machine__Machine__eval_define(m, kids);
	} else if (string__eq(name, _v3_lit_3_cadb9512638cd72)) {
		return machine__Machine__eval_define(m, kids);
	} else if (string__eq(name, _v3_lit_8_b71566fdd941000e)) {
		return machine__Machine__eval_defmacro(m, kids);
	} else if (string__eq(name, _v3_lit_6_5035d5f3293e5816)) {
		if (kids.len != 2) {
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_47_6e22e270bd1e517e, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		__v_result_blip__Value __or_opt_0 = machine__Machine__macex1(m, (*(i32*)array_get(kids, 1)));
		blip__Value __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result){.ok = false, .err = __or_opt_0.err};
		}
		m->val = __or_val_1;
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_5_80da626fc95d20a3)) {
		if (kids.len != 2) {
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_45_ec3a369cd5570360, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		__v_result_blip__Value __or_opt_2 = machine__Machine__macex(m, (*(i32*)array_get(kids, 1)));
		blip__Value __or_val_3 = {0};
		if (__or_opt_2.ok) {
			__or_val_3 = __or_opt_2.value;
		} else {
			return (__v_result){.ok = false, .err = __or_opt_2.err};
		}
		m->val = __or_val_3;
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_4_669eb42cb843a8e0)) {
		reader__Datum* target = reader__Arena__node(m->arena, (*(i32*)array_get(kids, 1)));
		if (target->tag != reader__DatumTag__sym) {
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_17_e78e6b6945ac097d, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		blip__Kont nf = machine__Machine__kont(m, 64);
		nf.name = target->value;
		nf.env = m->env;
		machine__Machine__push(m, nf);
		machine__Machine__goto(m, (*(i32*)array_get(kids, 2)));
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_6_497dd6d1afd39ae4)) {
		return machine__Machine__eval_lambda(m, kids);
	} else if (string__eq(name, _v3_lit_2_9bb67100c6643a03)) {
		Array __arr_lit_4 = __new_array_noscan(0, 3, sizeof(i32));
		i32 __arr_val_5 = (*(i32*)array_get(kids, 0));
		array_push(&__arr_lit_4, &__arr_val_5);
		i32 __arr_val_6 = machine__Machine__make_begin(m, array_slice(		kids, 		1, 		2));
		array_push(&__arr_lit_4, &__arr_val_6);
		i32 __arr_val_7 = machine__Machine__make_begin(m, array_slice(		kids, 		2, (		kids).len));
		array_push(&__arr_lit_4, &__arr_val_7);
		return machine__Machine__eval_lambda(m, __arr_lit_4);
	} else if (string__eq(name, _v3_lit_5_178c76c75eded44)) {
		if (kids.len == 1) {
			m->val = blip__nil_value();
			machine__Machine__ret(m);
			return (__v_result){.ok = true};
		}
		if (kids.len == 2) {
			machine__Machine__goto(m, (*(i32*)array_get(kids, 1)));
			return (__v_result){.ok = true};
		}
		blip__Kont nf = machine__Machine__kont(m, 16);
		nf.rest = id;
		nf.env = m->env;
		nf.slot = 1;
		if (2 < kids.len) {
			machine__Machine__push(m, nf);
		}
		machine__Machine__goto(m, (*(i32*)array_get(kids, 1)));
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_3_e1761a510dc506d4) || string__eq(name, _v3_lit_2_9bce5b00c678a682)) {
		if (kids.len == 1) {
			m->val = blip__boolean(string__eq(name, _v3_lit_3_e1761a510dc506d4));
			machine__Machine__ret(m);
			return (__v_result){.ok = true};
		}
		int __if_val_8 = {0};
		if (string__eq(name, _v3_lit_3_e1761a510dc506d4)) {
			__if_val_8 = 128;
		} else {
			__if_val_8 = 256;
		}
		int tag = __if_val_8;
		blip__Kont nf = machine__Machine__kont(m, tag);
		nf.rest = id;
		nf.env = m->env;
		nf.slot = 1;
		machine__Machine__push(m, nf);
		machine__Machine__goto(m, (*(i32*)array_get(kids, 1)));
		return (__v_result){.ok = true};
	} else {
	}
	if (string__eq(name, _v3_lit_3_51f91f514d780114)) {
		__v_result_i32 __or_opt_9 = machine__Machine__transform_let(m, kids);
		i32 __or_val_10 = {0};
		if (__or_opt_9.ok) {
			__or_val_10 = __or_opt_9.value;
		} else {
			return (__v_result){.ok = false, .err = __or_opt_9.err};
		}
		machine__Machine__goto(m, __or_val_10);
	} else if (string__eq(name, _v3_lit_4_c2517526a2ea1c5a)) {
		machine__Machine__goto(m, machine__Machine__transform_let_star(m, kids));
	} else if (string__eq(name, _v3_lit_6_3ab33e7aab478c62)) {
		machine__Machine__goto(m, machine__Machine__transform_letrec(m, kids));
	} else if (string__eq(name, _v3_lit_4_ca78e650452716cf)) {
		i32 body = machine__Machine__node_of(m, _v3_lit_5_178c76c75eded44, array_slice(		kids, 		1, (		kids).len));
		Array __arr_lit_11 = __new_array_noscan(0, 3, sizeof(i32));
		i32 __arr_val_12 = (*(i32*)array_get(kids, 1));
		array_push(&__arr_lit_11, &__arr_val_12);
		i32 __arr_val_13 = body;
		array_push(&__arr_lit_11, &__arr_val_13);
		i32 __arr_val_14 = machine__Machine__nil_node(m);
		array_push(&__arr_lit_11, &__arr_val_14);
		machine__Machine__goto(m, machine__Machine__node_of(m, _v3_lit_2_9bc77700c672b768, __arr_lit_11));
	} else if (string__eq(name, _v3_lit_6_7969d416ee3eef9d)) {
		i32 body = machine__Machine__node_of(m, _v3_lit_5_178c76c75eded44, array_slice(		kids, 		1, (		kids).len));
		Array __arr_lit_15 = __new_array_noscan(0, 3, sizeof(i32));
		i32 __arr_val_16 = (*(i32*)array_get(kids, 1));
		array_push(&__arr_lit_15, &__arr_val_16);
		i32 __arr_val_17 = machine__Machine__nil_node(m);
		array_push(&__arr_lit_15, &__arr_val_17);
		i32 __arr_val_18 = body;
		array_push(&__arr_lit_15, &__arr_val_18);
		machine__Machine__goto(m, machine__Machine__node_of(m, _v3_lit_2_9bc77700c672b768, __arr_lit_15));
	} else if (string__eq(name, _v3_lit_4_e6cd82a8beffa2b7)) {
		machine__Machine__goto(m, machine__Machine__transform_cond(m, kids));
	} else if (string__eq(name, _v3_lit_4_6bb733a878c4261f)) {
		machine__Machine__goto(m, machine__Machine__transform_case(m, kids));
	} else if (string__eq(name, _v3_lit_4_8fd1b32686ba76f1)) {
		machine__Machine__goto(m, machine__Machine__transform_loop(m, kids));
	} else if (string__eq(name, _v3_lit_7_7cc31e91bc6bb38c)) {
		__v_result_i32 __or_opt_19 = machine__Machine__transform_dotimes(m, kids);
		i32 __or_val_20 = {0};
		if (__or_opt_19.ok) {
			__or_val_20 = __or_opt_19.value;
		} else {
			return (__v_result){.ok = false, .err = __or_opt_19.err};
		}
		machine__Machine__goto(m, __or_val_20);
	} else if (string__eq(name, _v3_lit_3_8ee02351700ae052)) {
		return machine__Machine__eval_use(m, kids);
	} else if (string__eq(name, _v3_lit_6_dc2ba458304519da)) {
		return machine__Machine__eval_import(m, kids);
	} else if (string__eq(name, _v3_lit_3_964d645173e60e54)) {
		return machine__Machine__eval_try(m, kids);
	} else if (string__eq(name, _v3_lit_2_9bbd5000c66a209e)) {
		machine__Machine__goto(m, machine__Machine__make_begin(m, array_slice(		kids, 		1, (		kids).len)));
	} else if (string__eq(name, _v3_lit_5_1118636f8a4d8f74)) {
		return machine__Machine__eval_match(m, kids);
	} else if (string__eq(name, _v3_lit_6_5a00548801ca9cba)) {
		machine__Machine__goto(m, machine__Machine__transform_match_star(m, kids));
	} else if (string__eq(name, _v3_lit_10_88fb1f45907c858d)) {
		__v_result_i32 __or_opt_21 = machine__Machine__transform_let_assert(m, kids);
		i32 __or_val_22 = {0};
		if (__or_opt_21.ok) {
			__or_val_22 = __or_opt_21.value;
		} else {
			return (__v_result){.ok = false, .err = __or_opt_21.err};
		}
		machine__Machine__goto(m, __or_val_22);
	} else if (string__eq(name, _v3_lit_6_14da8f5ec8315a0a)) {
		return machine__Machine__eval_struct(m, kids);
	} else if (string__eq(name, _v3_lit_10_999ae46be131259f)) {
		m->val = blip__nil_value();
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_7_1e67ecb0563e6794)) {
		m->val = blip__nil_value();
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_7_83d47bb3d9452c4e)) {
		return machine__Machine__eval_require(m, kids);
	} else if (string__eq(name, _v3_lit_6_82fb670e19860b15)) {
		if (kids.len < 3) {
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_67_5b68be3d2d032bfc, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		blip__Kont af = machine__Machine__kont(m, 1024);
		af.env = m->env;
		af.rest = (*(i32*)array_get(kids, 2));
		machine__Machine__push(m, af);
		machine__Machine__goto(m, (*(i32*)array_get(kids, 1)));
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_2_9afb1f00c5c4bb7c) || string__eq(name, _v3_lit_3_1d70f0500d4a3126) || string__eq(name, _v3_lit_2_9c0ea100c6af0169) || string__eq(name, _v3_lit_4_511736baa561c828) || string__eq(name, _v3_lit_6_f9ae4a62895365b0)) {
		return machine__Machine__eval_pipe(m, name, kids);
	} else {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_28_e400dbba838a731d, name), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END 63438678b682331a */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 9868377d9bf9ced4 */
__v_result_blip__Value machine__Machine__eval_string_lenient(machine__Machine* m, string src) {
	reader__Forms res = reader__Arena__read_forms(m->arena, src);
	blip__Value last = blip__nil_value();
	i64 i = 0;
	while (i < res.forms.len) {
		__v_result_blip__Value __or_opt_0 = machine__Machine__eval_one(m, (*(i32*)array_get(res.forms, i)));
		blip__Value __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
		}
		last = __or_val_1;
		i++;
	}
	return (__v_result_blip__Value){.ok = true, .value = last};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 9868377d9bf9ced4 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 7e7504b56eda1530 */
__v_result machine__Machine__eval_struct(machine__Machine* m, Array kids) {
	if (kids.len < 3) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_59_9011271f10096872, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	reader__Datum* name_node = reader__Arena__node(m->arena, (*(i32*)array_get(kids, 1)));
	if (name_node->tag != reader__DatumTag__sym) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_19_d4c40bc705adc33f, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	string name = name_node->value;
	Array fl = reader__Arena__kids(m->arena, (*(i32*)array_get(kids, 2)));
	Array fields = array_new(	sizeof(string), 0, 0);
	i64 i = 0;
	while (i < fl.len) {
		reader__Datum* d = reader__Arena__node(m->arena, (*(i32*)array_get(fl, i)));
		if (d->tag != reader__DatumTag__sym) {
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[5]; __v3_internal_symbol_join_0[0] = _v3_lit_7_a0bbc80e2bdc355e; __v3_internal_symbol_join_0[1] = name; __v3_internal_symbol_join_0[2] = _v3_lit_8_eb88381b623a2071; __v3_internal_symbol_join_0[3] = int__str(i); __v3_internal_symbol_join_0[4] = _v3_lit_14_4c516eb5bd30ac9e; string_plus_many(5, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		string __arr_val_0 = string__clone(d->value);
		array_push(&fields, &__arr_val_0);
		i++;
	}
	bool mutable = false;
	bool opaque = false;
	i = 3;
	while (i < kids.len) {
		reader__Datum* d = reader__Arena__node(m->arena, (*(i32*)array_get(kids, i)));
		if (d->tag == reader__DatumTag__kw) {
			if (string__eq(d->value, _v3_lit_7_e35bc820a2302fe7)) {
				mutable = true;
			}
			if (string__eq(d->value, _v3_lit_6_47233efd4b05df50)) {
				opaque = true;
			}
		}
		i++;
	}
	string low = string__to_lower(name);
	string src = _v3_lit_0_14650fb0739d0383;
	Array params = array_new(	sizeof(string), 0, 0);
	Array quoted_fields = array_new(	sizeof(string), 0, 0);
	{
		i64 __for_idx_1 = 0;
		for (; __for_idx_1 < fields.len; __for_idx_1++) {
			string f = *(string*)(array_get(fields, __for_idx_1));
			string __arr_val_2 = string__clone(f);
			array_push(&params, &__arr_val_2);
			string __arr_val_3 = ({ string __v3_internal_symbol_join_1[3]; __v3_internal_symbol_join_1[0] = _v3_lit_1_44bd49d473cd2a93; __v3_internal_symbol_join_1[1] = f; __v3_internal_symbol_join_1[2] = _v3_lit_1_44bd49d473cd2a93; string_plus_many(3, __v3_internal_symbol_join_1); });
			array_push(&quoted_fields, &__arr_val_3);
		}
	}
	src = string__plus(src, ({ string __v3_internal_symbol_join_2[11]; __v3_internal_symbol_join_2[0] = _v3_lit_9_123b237388c8bce; __v3_internal_symbol_join_2[1] = name; __v3_internal_symbol_join_2[2] = _v3_lit_1_44bd4bd473cd2df9; __v3_internal_symbol_join_2[3] = Array_string__join(params, _v3_lit_1_44bd4bd473cd2df9); __v3_internal_symbol_join_2[4] = _v3_lit_24_41dc16bd64858738; __v3_internal_symbol_join_2[5] = name; __v3_internal_symbol_join_2[6] = _v3_lit_10_8c0d7053025f758f; __v3_internal_symbol_join_2[7] = Array_string__join(quoted_fields, _v3_lit_1_44bd4bd473cd2df9); __v3_internal_symbol_join_2[8] = _v3_lit_3_bcfe9450f899746b; __v3_internal_symbol_join_2[9] = Array_string__join(params, _v3_lit_1_44bd4bd473cd2df9); __v3_internal_symbol_join_2[10] = _v3_lit_3_fb6b024ffa325e8d; string_plus_many(11, __v3_internal_symbol_join_2); }));
	src = string__plus(src, ({ string __v3_internal_symbol_join_3[5]; __v3_internal_symbol_join_3[0] = _v3_lit_8_25c1121422da11d2; __v3_internal_symbol_join_3[1] = low; __v3_internal_symbol_join_3[2] = _v3_lit_53_b0e33b0845fe521d; __v3_internal_symbol_join_3[3] = name; __v3_internal_symbol_join_3[4] = _v3_lit_6_fce65f749e36cfc0; string_plus_many(5, __v3_internal_symbol_join_3); }));
	{
		i64 __for_idx_4 = 0;
		for (; __for_idx_4 < fields.len; __for_idx_4++) {
			string f = *(string*)(array_get(fields, __for_idx_4));
			src = string__plus(src, ({ string __v3_internal_symbol_join_4[7]; __v3_internal_symbol_join_4[0] = _v3_lit_8_25c1121422da11d2; __v3_internal_symbol_join_4[1] = low; __v3_internal_symbol_join_4[2] = _v3_lit_1_44bd56d473cd40aa; __v3_internal_symbol_join_4[3] = f; __v3_internal_symbol_join_4[4] = _v3_lit_30_300091d0d8aec6b0; __v3_internal_symbol_join_4[5] = f; __v3_internal_symbol_join_4[6] = _v3_lit_5_2a82f70dcdc0c5f7; string_plus_many(7, __v3_internal_symbol_join_4); }));
			if (mutable) {
				src = string__plus(src, ({ string __v3_internal_symbol_join_5[7]; __v3_internal_symbol_join_5[0] = _v3_lit_12_d97a2f1b85d1ef5d; __v3_internal_symbol_join_5[1] = low; __v3_internal_symbol_join_5[2] = _v3_lit_1_44bd56d473cd40aa; __v3_internal_symbol_join_5[3] = f; __v3_internal_symbol_join_5[4] = _v3_lit_39_39b9307000d3189f; __v3_internal_symbol_join_5[5] = f; __v3_internal_symbol_join_5[6] = _v3_lit_7_68e2bba7bfbb7bff; string_plus_many(7, __v3_internal_symbol_join_5); }));
			}
		}
	}
	if (opaque) {
		src = string__plus(({ string __v3_internal_symbol_join_6[11]; __v3_internal_symbol_join_6[0] = _v3_lit_8_25c1121422da11d2; __v3_internal_symbol_join_6[1] = name; __v3_internal_symbol_join_6[2] = _v3_lit_12_f1b6c4dba48c7d8e; __v3_internal_symbol_join_6[3] = Array_string__join(params, _v3_lit_1_44bd4bd473cd2df9); __v3_internal_symbol_join_6[4] = _v3_lit_24_9be0edfd935b40bc; __v3_internal_symbol_join_6[5] = name; __v3_internal_symbol_join_6[6] = _v3_lit_10_8c0d7053025f758f; __v3_internal_symbol_join_6[7] = Array_string__join(quoted_fields, _v3_lit_1_44bd4bd473cd2df9); __v3_internal_symbol_join_6[8] = _v3_lit_3_bcfe9450f899746b; __v3_internal_symbol_join_6[9] = Array_string__join(params, _v3_lit_1_44bd4bd473cd2df9); __v3_internal_symbol_join_6[10] = _v3_lit_4_68bc92e62331abfa; string_plus_many(11, __v3_internal_symbol_join_6); }), src);
	}
	__v_result_blip__Value __or_opt_5 = machine__Machine__eval_string_lenient(m, src);
	if (!__or_opt_5.ok) {
		IError err = __or_opt_5.err;
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_7[4]; __v3_internal_symbol_join_7[0] = _v3_lit_7_a0bbc80e2bdc355e; __v3_internal_symbol_join_7[1] = name; __v3_internal_symbol_join_7[2] = _v3_lit_36_a0ef49fa38cf331e; __v3_internal_symbol_join_7[3] = IError__msg(&(err)); string_plus_many(4, __v3_internal_symbol_join_7); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	m->val = blip__symbol(name);
	machine__Machine__ret(m);
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END 7e7504b56eda1530 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 683612db2b7679ff */
__v_result machine__Machine__eval_symbol(machine__Machine* m, string name) {
	if (string__eq(name, _v3_lit_3_3ff5d95143129022) || string__eq(name, _v3_lit_4_8e37b614dbd95475)) {
		m->val = blip__nil_value();
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_4_4b66a767efc7182f)) {
		m->val = blip__boolean(true);
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else if (string__eq(name, _v3_lit_5_cbbfd4228091f9d2)) {
		m->val = blip__boolean(false);
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else {
	}
	__v_option_blip__Value __if_guard_0 = blip__EnvArena__lookup(&m->envs, m->env, name);
	if (__if_guard_0.ok) {
		blip__Value found = __if_guard_0.value;
		m->val = found;
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	}
	if (string__contains(name, _v3_lit_1_44bd55d473cd3ef7)) {
		return machine__Machine__eval_field_access(m, name);
	}
	string __map_key_1 = name;
	if (map__exists(&m->prims, &__map_key_1) || machine__machine_builtin(name)) {
		m->val = blip__new_prim(name);
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	}
	return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_20_f0a7b475d116260d, name), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END 683612db2b7679ff */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 7acd7f156eed9764 */
__v_result machine__Machine__eval_try(machine__Machine* m, Array kids) {
	if (kids.len < 2) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_16_2402f99f8f5fbd1e, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	Array body = __new_array_noscan(0, 0, sizeof(i32));
	i32 catch = blip__no_node;
	i32 finally = blip__no_node;
	i64 i = 1;
	while (i < kids.len) {
		reader__Datum* d = reader__Arena__node(m->arena, (*(i32*)array_get(kids, i)));
		if ((d->tag == reader__DatumTag__kw) && (string__eq(d->value, _v3_lit_5_67b5345556f15ee) || string__eq(d->value, _v3_lit_7_3fd9af264aa12b2))) {
			if ((i + 1) >= kids.len) {
				string __interp_part_1 = _v3_lit_7_9dfcf628eb4e3267;
				string __interp_part_2 = d->value;
				string __interp_part_3 = _v3_lit_7_c66143f46680e5d2;
				string __if_val_0 = {0};
				if (string__eq(d->value, _v3_lit_5_67b5345556f15ee)) {
					__if_val_0 = _v3_lit_8_740f090f5f7c291f;
				} else {
					__if_val_0 = _v3_lit_6_ff41e9a110025516;
				}
				string __interp_part_4 = __if_val_0;
				return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[4]; __v3_internal_symbol_join_0[0] = __interp_part_1; __v3_internal_symbol_join_0[1] = __interp_part_2; __v3_internal_symbol_join_0[2] = __interp_part_3; __v3_internal_symbol_join_0[3] = __interp_part_4; string_plus_many(4, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			if (string__eq(d->value, _v3_lit_5_67b5345556f15ee)) {
				catch = (*(i32*)array_get(kids, i + 1));
			} else {
				finally = (*(i32*)array_get(kids, i + 1));
			}
			i += 2;
			continue;
		}
		i32 __arr_val_5 = (*(i32*)array_get(kids, i));
		array_push(&body, &__arr_val_5);
		i++;
	}
	if (body.len == 0) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_16_2402f99f8f5fbd1e, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	blip__Kont tf = machine__Machine__kont(m, 32768);
	tf.clauses = body;
	tf.expr = catch;
	tf.rest = finally;
	tf.slot = 0;
	tf.env = m->env;
	machine__Machine__push(m, tf);
	machine__Machine__goto(m, (*(i32*)array_get(body, 0)));
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END 7acd7f156eed9764 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN f4bcf01bfc7538b6 */
__v_result machine__Machine__eval_use(machine__Machine* m, Array kids) {
	if (kids.len < 2) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_37_e9d8ca360177a979, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	blip__Kont nf = machine__Machine__kont(m, 2048);
	nf.rest = (i32)(0);
	nf.slot = 0;
	nf.env = m->env;
	nf.clauses = kids;
	machine__Machine__push(m, nf);
	machine__Machine__goto(m, (*(i32*)array_get(kids, 1)));
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END f4bcf01bfc7538b6 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 3c00e917063739af */
__v_result_blip__Value machine__Machine__expand_macro_value(machine__Machine* m, blip__Value v) {
	if (!machine__Machine__is_macro_form(m, v)) {
		return (__v_result_blip__Value){.ok = true, .value = v};
	}
	string name = blip__Value__as_string((blip__Value__as_pair(v))->car);
	__v_option_blip__Value __or_opt_0 = blip__EnvArena__lookup(&m->envs, m->globals, name);
	blip__Value __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_7_89ea4c2f1503ad21; __v3_internal_symbol_join_0[1] = name; __v3_internal_symbol_join_0[2] = _v3_lit_15_fffbc03f977f8359; string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	blip__Value macro = __or_val_1;
	if (!machine__Machine__is_macro_closure(m, macro)) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[3]; __v3_internal_symbol_join_1[0] = _v3_lit_7_89ea4c2f1503ad21; __v3_internal_symbol_join_1[1] = name; __v3_internal_symbol_join_1[2] = _v3_lit_15_4bd17a79af2a715b; string_plus_many(3, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_Array __or_opt_2 = prims__seq((blip__Value__as_pair(v))->cdr);
	Array __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		IError err = __or_opt_2.err;
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_2[3]; __v3_internal_symbol_join_2[0] = _v3_lit_7_89ea4c2f1503ad21; __v3_internal_symbol_join_2[1] = name; __v3_internal_symbol_join_2[2] = _v3_lit_17_4776d370e5aa6766; string_plus_many(3, __v3_internal_symbol_join_2); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	Array args = __or_val_3;
	__v_result_blip__Value __or_opt_4 = machine__Machine__call_value(m, macro, args);
	blip__Value __or_val_5 = {0};
	if (__or_opt_4.ok) {
		__or_val_5 = __or_opt_4.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_4.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = __or_val_5};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 3c00e917063739af */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN ca2f5eaffb554f48 */
__v_result_blip__Value machine__Machine__field_of(machine__Machine* m, blip__Value v, string field) {
	if (v.tag == 131072) {
		blip__StructVal* s = blip__Value__as_struct(v);
		if (!blip__StructVal__has(s, field)) {
			return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = s->name; __v3_internal_symbol_join_0[1] = _v3_lit_14_14368fa9ebdd61d6; __v3_internal_symbol_join_0[2] = field; string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		return (__v_result_blip__Value){.ok = true, .value = blip__StructVal__get(s, field)};
	}
	if ((v.tag == 4096) || (v.tag == 8192)) {
		return (__v_result_blip__Value){.ok = true, .value = blip__Table__get(blip__Value__as_table(v), field)};
	}
	return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[4]; __v3_internal_symbol_join_1[0] = _v3_lit_18_49c6dcceeec4ec08; __v3_internal_symbol_join_1[1] = field; __v3_internal_symbol_join_1[2] = _v3_lit_4_d5dfe2a966931484; __v3_internal_symbol_join_1[3] = printer__write(v); string_plus_many(4, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END ca2f5eaffb554f48 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 5e5f8143f13755a1 */
__v_result_Array machine__Machine__fill_labels(machine__Machine* m, blip__Closure* c, Array args, i32 caller) {
	Array slots = array_new(	sizeof(blip__Value), 0, 0);
	Array filled = __new_array_noscan(0, 0, sizeof(bool));
	i64 i = 0;
	while (i < c->params.len) {
		blip__Value __arr_val_0 = blip__nil_value();
		array_push(&slots, &__arr_val_0);
		bool __arr_val_1 = false;
		array_push(&filled, &__arr_val_1);
		i++;
	}
	i64 a = 0;
	while (a < args.len) {
		blip__Value v = (*(blip__Value*)array_get(args, a));
		if (v.tag == 128) {
			i64 slot = -1;
			i64 j = c->opt_from;
			while (j < c->params.len) {
				if (string__eq((*(string*)array_get(c->opt_names, j - c->opt_from)), blip__Value__as_string(v))) {
					slot = j;
					break;
				}
				j++;
			}
			if (slot < 0) {
				return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[4]; __v3_internal_symbol_join_0[0] = blip__Value__as_string(v); __v3_internal_symbol_join_0[1] = _v3_lit_2_9b20b700c5e4da01; __v3_internal_symbol_join_0[2] = c->name; __v3_internal_symbol_join_0[3] = _v3_lit_31_908db3d2edea40f; string_plus_many(4, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			if ((a + 1) >= args.len) {
				return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(blip__Value__as_string(v), _v3_lit_15_a204aa75be175640), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			if ((*(bool*)array_get(filled, slot))) {
				return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[4]; __v3_internal_symbol_join_1[0] = c->name; __v3_internal_symbol_join_1[1] = _v3_lit_2_9b20b700c5e4da01; __v3_internal_symbol_join_1[2] = blip__Value__as_string(v); __v3_internal_symbol_join_1[3] = _v3_lit_16_a4c7fcac8501a97b; string_plus_many(4, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			{ Array* _a2 = &slots; int _i2 = slot; array__set(_a2, _i2, &(blip__Value[]){(*(blip__Value*)array_get(args, a + 1))}); }
			{ Array* __v3_internal_symbol_scalar_store_base_3 = &filled; int __v3_internal_symbol_scalar_store_index_3 = slot; bool __v3_internal_symbol_scalar_store_value_3 = true; if (__v3_internal_symbol_scalar_store_index_3 >= 0 && __v3_internal_symbol_scalar_store_index_3 < __v3_internal_symbol_scalar_store_base_3->len) { ((bool*)__v3_internal_symbol_scalar_store_base_3->data)[__v3_internal_symbol_scalar_store_index_3] = __v3_internal_symbol_scalar_store_value_3; } else { array__set(__v3_internal_symbol_scalar_store_base_3, __v3_internal_symbol_scalar_store_index_3, &__v3_internal_symbol_scalar_store_value_3); } }
			a += 2;
			continue;
		}
		i64 slot = -1;
		i64 j = 0;
		while (j < c->params.len) {
			if (!(*(bool*)array_get(filled, j))) {
				slot = j;
				break;
			}
			j++;
		}
		if (slot < 0) {
			return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_4[7]; __v3_internal_symbol_join_4[0] = c->name; __v3_internal_symbol_join_4[1] = _v3_lit_11_b1208f735e965447; __v3_internal_symbol_join_4[2] = int__str(c->arity); __v3_internal_symbol_join_4[3] = _v3_lit_9_97b175148f7d162a; __v3_internal_symbol_join_4[4] = machine__plural(c->arity); __v3_internal_symbol_join_4[5] = _v3_lit_6_45ecea13c54f3ff1; __v3_internal_symbol_join_4[6] = int__str(args.len); string_plus_many(7, __v3_internal_symbol_join_4); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		{ Array* _a5 = &slots; int _i5 = slot; array__set(_a5, _i5, &(blip__Value[]){v}); }
		{ Array* __v3_internal_symbol_scalar_store_base_6 = &filled; int __v3_internal_symbol_scalar_store_index_6 = slot; bool __v3_internal_symbol_scalar_store_value_6 = true; if (__v3_internal_symbol_scalar_store_index_6 >= 0 && __v3_internal_symbol_scalar_store_index_6 < __v3_internal_symbol_scalar_store_base_6->len) { ((bool*)__v3_internal_symbol_scalar_store_base_6->data)[__v3_internal_symbol_scalar_store_index_6] = __v3_internal_symbol_scalar_store_value_6; } else { array__set(__v3_internal_symbol_scalar_store_base_6, __v3_internal_symbol_scalar_store_index_6, &__v3_internal_symbol_scalar_store_value_6); } }
		a++;
	}
	i32 saved_env = m->env;
	m->env = caller;
	i64 j = c->opt_from;
	while (j < c->params.len) {
		if (!(*(bool*)array_get(filled, j))) {
			i32 d = (*(i32*)array_get(c->opt_defaults, j - c->opt_from));
			if (d == blip__no_default) {
				m->env = saved_env;
				return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_7[4]; __v3_internal_symbol_join_7[0] = c->name; __v3_internal_symbol_join_7[1] = _v3_lit_2_9b20b700c5e4da01; __v3_internal_symbol_join_7[2] = (*(string*)array_get(c->opt_names, j - c->opt_from)); __v3_internal_symbol_join_7[3] = _v3_lit_12_3926ab14809ec566; string_plus_many(4, __v3_internal_symbol_join_7); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			Array one = array_new(			sizeof(blip__Value), 0, 0);
			__v_result_blip__Value __or_opt_2 = machine__Machine__eval_default(m, d, one);
			blip__Value __or_val_3 = {0};
			if (__or_opt_2.ok) {
				__or_val_3 = __or_opt_2.value;
			} else {
				return (__v_result_Array){.ok = false, .err = __or_opt_2.err};
			}
			{ Array* _a8 = &slots; int _i8 = j; array__set(_a8, _i8, &(blip__Value[]){__or_val_3}); }
		}
		j++;
	}
	m->env = saved_env;
	return (__v_result_Array){.ok = true, .value = slots};
	return (__v_result_Array){.ok = true};
}

/* V3CACHE_FN_END 5e5f8143f13755a1 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN badf73ce25555ead */
__v_result machine__Machine__fold(machine__Machine* m, string name, Array args) {
	if (string__eq(name, _v3_lit_6_31de9ec2d2adf2e5) && (args.len == 2)) {
		__v_result_Array __or_opt_0 = prims__seq((*(blip__Value*)array_get(args, 1)));
		Array __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			IError err = __or_opt_0.err;
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_8_c8558d5fe1f56c57, IError__msg(&(err))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		Array items = __or_val_1;
		if (items.len == 0) {
			return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_64_7817c198e5ca3986, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		Array call = array_new(		sizeof(blip__Value), 0, 0);
		blip__Value __arr_val_2 = (*(blip__Value*)array_get(args, 0));
		array_push(&call, &__arr_val_2);
		i64 i = 1;
		while (i < items.len) {
			blip__Value __arr_val_3 = (*(blip__Value*)array_get(items, i));
			array_push(&call, &__arr_val_3);
			i++;
		}
		__v_result_blip__Value __or_opt_4 = machine__Machine__call_value(m, (*(blip__Value*)array_get(args, 0)), call);
		blip__Value __or_val_5 = {0};
		if (__or_opt_4.ok) {
			__or_val_5 = __or_opt_4.value;
		} else {
			return (__v_result){.ok = false, .err = __or_opt_4.err};
		}
		m->val = __or_val_5;
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	}
	if (args.len < 3) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = name; __v3_internal_symbol_join_0[1] = _v3_lit_35_5281bf074bd6df94; __v3_internal_symbol_join_0[2] = int__str(args.len); string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	blip__Value acc = (*(blip__Value*)array_get(args, 1));
	__v_result_Array __or_opt_6 = prims__seq((*(blip__Value*)array_get(args, 2)));
	Array __or_val_7 = {0};
	if (__or_opt_6.ok) {
		__or_val_7 = __or_opt_6.value;
	} else {
		IError err = __or_opt_6.err;
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[3]; __v3_internal_symbol_join_1[0] = name; __v3_internal_symbol_join_1[1] = _v3_lit_2_9b20b700c5e4da01; __v3_internal_symbol_join_1[2] = IError__msg(&(err)); string_plus_many(3, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	Array items = __or_val_7;
	i64 i = 0;
	while (i < items.len) {
		Array call = array_new(		sizeof(blip__Value), 0, 0);
		blip__Value __arr_val_8 = acc;
		array_push(&call, &__arr_val_8);
		blip__Value __arr_val_9 = (*(blip__Value*)array_get(items, i));
		array_push(&call, &__arr_val_9);
		__v_result_blip__Value __or_opt_10 = machine__Machine__call_value(m, (*(blip__Value*)array_get(args, 0)), call);
		blip__Value __or_val_11 = {0};
		if (__or_opt_10.ok) {
			__or_val_11 = __or_opt_10.value;
		} else {
			return (__v_result){.ok = false, .err = __or_opt_10.err};
		}
		acc = __or_val_11;
		i++;
	}
	m->val = acc;
	machine__Machine__ret(m);
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END badf73ce25555ead */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN a7e9d3b9d64bc38e */
string machine__Machine__fresh(machine__Machine* m, string base) {
	m->gensym++;
	return ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = base; __v3_internal_symbol_join_0[1] = _v3_lit_1_44bd84d473cd8ed4; __v3_internal_symbol_join_0[2] = int__str(m->gensym); string_plus_many(3, __v3_internal_symbol_join_0); });
}

/* V3CACHE_FN_END a7e9d3b9d64bc38e */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN b6f1faaaba7314c5 */
void machine__Machine__goto(machine__Machine* m, i32 form) {
	m->form = form;
	m->ctl = 1;
}

/* V3CACHE_FN_END b6f1faaaba7314c5 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 75e1b382f7e29441 */
__v_result machine__Machine__hof(machine__Machine* m, string name, Array args) {
	if (args.len < 2) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = name; __v3_internal_symbol_join_0[1] = _v3_lit_35_a0f235541515e79b; __v3_internal_symbol_join_0[2] = int__str(args.len); string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	blip__Value f = (*(blip__Value*)array_get(args, 0));
	blip__Value src = (*(blip__Value*)array_get(args, 1));
	__v_result_Array __or_opt_0 = prims__seq(src);
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[3]; __v3_internal_symbol_join_1[0] = name; __v3_internal_symbol_join_1[1] = _v3_lit_2_9b20b700c5e4da01; __v3_internal_symbol_join_1[2] = IError__msg(&(err)); string_plus_many(3, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	Array items = __or_val_1;
	bool want_vector = (src.tag == 1024) || (src.tag == 2048) || string__eq(name, _v3_lit_10_f8e88b4c9f2cc737);
	bool keep = string__eq(name, _v3_lit_6_cd094ce5950ab6d) || string__eq(name, _v3_lit_4_90ee9eeb4064e302);
	bool drop = string__eq(name, _v3_lit_6_126715d46b46abf8);
	Array out = array_new(	sizeof(blip__Value), 0, 0);
	i64 i = 0;
	while (i < items.len) {
		Array one = array_new(		sizeof(blip__Value), 0, 0);
		blip__Value __arr_val_2 = (*(blip__Value*)array_get(items, i));
		array_push(&one, &__arr_val_2);
		__v_result_blip__Value __or_opt_3 = machine__Machine__call_value(m, f, one);
		blip__Value __or_val_4 = {0};
		if (__or_opt_3.ok) {
			__or_val_4 = __or_opt_3.value;
		} else {
			return (__v_result){.ok = false, .err = __or_opt_3.err};
		}
		blip__Value v = __or_val_4;
		if (drop) {
			if (!blip__Value__truthy(v)) {
				blip__Value __arr_val_5 = (*(blip__Value*)array_get(items, i));
				array_push(&out, &__arr_val_5);
			}
		} else if (keep) {
			if (blip__Value__truthy(v)) {
				blip__Value __arr_val_6 = (*(blip__Value*)array_get(items, i));
				array_push(&out, &__arr_val_6);
			}
		} else {
			blip__Value __arr_val_7 = v;
			array_push(&out, &__arr_val_7);
		}
		i++;
	}
	if (string__eq(name, _v3_lit_8_99cfadc3c25d49ec)) {
		m->val = blip__nil_value();
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	}
	if (want_vector) {
		m->val = blip__vector(out);
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	}
	m->val = blip__list_from(out);
	machine__Machine__ret(m);
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END 75e1b382f7e29441 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN d546d89d248eb7a */
i32 machine__Machine__int_node(machine__Machine* m, i64 n) {
	return reader__Arena__int_leaf(m->arena, reader__DatumTag__v_int, n);
}

/* V3CACHE_FN_END d546d89d248eb7a */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 814ceb00cc9df101 */
bool machine__Machine__is_binding_form(machine__Machine* m, i32 b) {
	reader__DatumTag tag = (reader__Arena__node(m->arena, b))->tag;
	return (tag == reader__DatumTag__list) || (tag == reader__DatumTag__vector);
}

/* V3CACHE_FN_END 814ceb00cc9df101 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN a2fdec8978b80bd8 */
bool machine__Machine__is_call_form(machine__Machine* m, i32 n) {
	return (reader__Arena__node(m->arena, n))->tag == reader__DatumTag__list;
}

/* V3CACHE_FN_END a2fdec8978b80bd8 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN a8ecc20ecee01da6 */
bool machine__Machine__is_capturing_tail(machine__Machine* m, i32 n) {
	reader__Datum* d = reader__Arena__node(m->arena, n);
	return (d->tag == reader__DatumTag__sym) && !string__eq(d->value, _v3_lit_1_44bd84d473cd8ed4);
}

/* V3CACHE_FN_END a8ecc20ecee01da6 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN eb02173a94b173b4 */
bool machine__Machine__is_destructuring(machine__Machine* m, i32 b) {
	if (!machine__Machine__is_binding_form(m, b)) {
		return false;
	}
	Array k = reader__Arena__kids(m->arena, b);
	if (k.len == 0) {
		return false;
	}
	reader__Datum* bd = reader__Arena__node(m->arena, (*(i32*)array_get(k, 0)));
	return (bd->tag == reader__DatumTag__list) || (bd->tag == reader__DatumTag__table) || (bd->tag == reader__DatumTag__vector);
}

/* V3CACHE_FN_END eb02173a94b173b4 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 2896cf6b7c8cdde3 */
bool machine__Machine__is_macro_closure(machine__Machine* m, blip__Value v) {
	if (v.tag != 16384) {
		return false;
	}
	string __contains_needle_0 = (blip__Value__as_closure(v))->name;
	bool __contains_1 = false;
	{
		i64 __contains_idx_2 = 0;
		for (; __contains_idx_2 < m->macros.len; __contains_idx_2++) {
			if (string__eq(*(string*)(array_get(m->macros, __contains_idx_2)), __contains_needle_0)) {
				__contains_1 = true;
			}
		}
	}
	return __contains_1;
}

/* V3CACHE_FN_END 2896cf6b7c8cdde3 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 71262c8568843184 */
bool machine__Machine__is_macro_form(machine__Machine* m, blip__Value v) {
	if (v.tag != 256) {
		return false;
	}
	blip__Value head = (blip__Value__as_pair(v))->car;
	if (head.tag != 64) {
		return false;
	}
	__v_option_blip__Value __or_opt_0 = blip__EnvArena__lookup(&m->envs, m->globals, blip__Value__as_string(head));
	blip__Value __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return false;
	}
	blip__Value found = __or_val_1;
	return (found.tag == 16384) && machine__Machine__is_macro_closure(m, found);
}

/* V3CACHE_FN_END 71262c8568843184 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 175358b7afb21320 */
bool machine__Machine__is_pipe_head(machine__Machine* m, i32 n) {
	Array kids = reader__Arena__kids(m->arena, n);
	if (kids.len == 0) {
		return false;
	}
	reader__Datum* h = reader__Arena__node(m->arena, (*(i32*)array_get(kids, 0)));
	return (h->tag == reader__DatumTag__sym) && (string__eq(h->value, _v3_lit_2_9afb1f00c5c4bb7c) || string__eq(h->value, _v3_lit_3_1d70f0500d4a3126) || string__eq(h->value, _v3_lit_2_9c0ea100c6af0169) || string__eq(h->value, _v3_lit_4_511736baa561c828) || string__eq(h->value, _v3_lit_6_f9ae4a62895365b0));
}

/* V3CACHE_FN_END 175358b7afb21320 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 279f3ed9a32c0325 */
bool machine__Machine__is_update_marker(machine__Machine* m, i32 n) {
	reader__Datum* d = reader__Arena__node(m->arena, n);
	if (d->tag == reader__DatumTag__kw) {
		return string__eq(d->value, _v3_lit_1_44bd66d473cd5bda);
	}
	return (d->tag == reader__DatumTag__sym) && string__eq(d->value, _v3_lit_2_9b20a200c5e4b652);
}

/* V3CACHE_FN_END 279f3ed9a32c0325 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 544892d7e0cc7e80 */
bool machine__Machine__is_wildcard(machine__Machine* m, i32 n) {
	reader__Datum* d = reader__Arena__node(m->arena, n);
	return (d->tag == reader__DatumTag__sym) && string__eq(d->value, _v3_lit_1_44bd84d473cd8ed4);
}

/* V3CACHE_FN_END 544892d7e0cc7e80 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 6f96f3acaab1dca0 */
blip__Kont machine__Machine__kont(machine__Machine* m, int tag) {
	return (blip__Kont){.tag = tag, .acc = array_new(sizeof(blip__Value), 0, 0), .name = _v3_lit_0_14650fb0739d0383, .clauses = array_new(sizeof(i32), 0, 0), .binds = array_new(sizeof(string), 0, 0), .vals = array_new(sizeof(blip__Value), 0, 0)};
}

/* V3CACHE_FN_END 6f96f3acaab1dca0 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 87de2d71b9d95218 */
i32 machine__Machine__kont_caller(machine__Machine* m) {
	if (m->kstack.len == 0) {
		return m->globals;
	}
	return ((*(blip__Kont*)array_get(m->kstack, m->kstack.len - 1))).env;
}

/* V3CACHE_FN_END 87de2d71b9d95218 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN f9e10ecf55ec6ed6 */
i32 machine__Machine__list_of(machine__Machine* m, Array items) {
	i32 id = reader__Arena__open(m->arena, reader__DatumTag__list);
	reader__Arena__finish(m->arena, id, items);
	return id;
}

/* V3CACHE_FN_END f9e10ecf55ec6ed6 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 837a534efebae556 */
i32 machine__Machine__list_of2(machine__Machine* m, Array names, Array vals) {
	Array items = __new_array_noscan(0, 0, sizeof(i32));
	i64 i = 0;
	while (i < names.len) {
		Array __arr_lit_0 = __new_array_noscan(0, 2, sizeof(i32));
		i32 __arr_val_1 = (*(i32*)array_get(names, i));
		array_push(&__arr_lit_0, &__arr_val_1);
		i32 __arr_val_2 = (*(i32*)array_get(vals, i));
		array_push(&__arr_lit_0, &__arr_val_2);
		i32 __arr_val_3 = machine__Machine__list_of(m, __arr_lit_0);
		array_push(&items, &__arr_val_3);
		i++;
	}
	return machine__Machine__list_of(m, items);
}

/* V3CACHE_FN_END 837a534efebae556 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 858fd49968086ee4 */
__v_result_blip__Value machine__Machine__load(machine__Machine* m, string path) {
	__v_result_string __or_opt_0 = host__Host__host_load(&(m->host), path);
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	string src = __or_val_1;
	string prev = m->source;
	m->source = path;
	m->src_text = src;
	reader__Forms res = reader__Arena__read_forms(m->arena, src);
	__v_result_blip__Value __or_opt_2 = machine__Machine__run_checked(m, res);
	blip__Value __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	blip__Value out = __or_val_3;
	m->source = prev;
	return (__v_result_blip__Value){.ok = true, .value = out};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 858fd49968086ee4 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 3e9c667000ced8f6 */
__v_result_blip__Value machine__Machine__lookup_key(machine__Machine* m, string key, blip__Value coll) {
	{
		int __match_tmp_0 = coll.tag;
		if ((__match_tmp_0 == 4096) || (__match_tmp_0 == 8192)) {
			return (__v_result_blip__Value){.ok = true, .value = blip__Table__get(blip__Value__as_table(coll), key)};
		} else if ((__match_tmp_0 == 2048) || (__match_tmp_0 == 1024)) {
			i64 n = (blip__Value__as_vector(coll))->data.len;
			__v_result_i64 __or_opt_1 = strconv__atoi(key);
			i64 __or_val_2 = {0};
			if (__or_opt_1.ok) {
				__or_val_2 = __or_opt_1.value;
			} else {
				IError err = __or_opt_1.err;
				return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_54_4e1f88c2111738e6, key), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			i64 i = __or_val_2;
			if ((i < 0) || (i >= n)) {
				return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[5]; __v3_internal_symbol_join_0[0] = _v3_lit_12_5b0cdad9b5e62f6c; __v3_internal_symbol_join_0[1] = int__str(i); __v3_internal_symbol_join_0[2] = _v3_lit_22_ef0a88c1130ba183; __v3_internal_symbol_join_0[3] = int__str(n); __v3_internal_symbol_join_0[4] = _v3_lit_1_44bd52d473cd39de; string_plus_many(5, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			return (__v_result_blip__Value){.ok = true, .value = (*(blip__Value*)array_get((blip__Value__as_vector(coll))->data, i))};
		} else {
			return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[4]; __v3_internal_symbol_join_1[0] = _v3_lit_12_db692466de09c1a1; __v3_internal_symbol_join_1[1] = key; __v3_internal_symbol_join_1[2] = _v3_lit_7_d024e13696b71b91; __v3_internal_symbol_join_1[3] = printer__write(coll); string_plus_many(4, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
	}
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 3e9c667000ced8f6 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 1734db0a86f76bc4 */
__v_result_blip__Value machine__Machine__macex(machine__Machine* m, i32 n) {
	__v_result_blip__Value __or_opt_0 = machine__Machine__macex1(m, n);
	blip__Value __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	blip__Value v = __or_val_1;
	i64 i = 0;
	while (i < 8) {
		if (!machine__Machine__is_macro_form(m, v)) {
			return (__v_result_blip__Value){.ok = true, .value = v};
		}
		__v_result_blip__Value __or_opt_2 = machine__Machine__expand_macro_value(m, v);
		blip__Value __or_val_3 = {0};
		if (__or_opt_2.ok) {
			__or_val_3 = __or_opt_2.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
		}
		v = __or_val_3;
		i++;
	}
	if (machine__Machine__is_macro_form(m, v)) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_80_a86204f5fc7b735f, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_blip__Value){.ok = true, .value = v};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 1734db0a86f76bc4 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 68a76de3587b3a1b */
__v_result_blip__Value machine__Machine__macex1(machine__Machine* m, i32 n) {
	return machine__Machine__expand_macro_value(m, machine__Machine__datum_to_value(m, n));
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 68a76de3587b3a1b */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 516f6788a82afb10 */
i32 machine__Machine__make_begin(machine__Machine* m, Array forms) {
	if (forms.len == 0) {
		return machine__Machine__nil_node(m);
	}
	if (forms.len == 1) {
		return (*(i32*)array_get(forms, 0));
	}
	return machine__Machine__node_of(m, _v3_lit_5_178c76c75eded44, forms);
}

/* V3CACHE_FN_END 516f6788a82afb10 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN b226ef8221743dfc */
blip__Value machine__Machine__make_closure(machine__Machine* m, machine__ParamSpec spec, i32 body, i32 env, string name) {
	if (spec.opt_from >= 0) {
		return blip__labelled(spec.names, body, env, name, spec.opt_from, spec.opt_names, spec.opt_defaults);
	}
	if (spec.is_rest) {
		return blip__new_rest_closure(spec.names, spec.rest, body, env, name);
	}
	return blip__new_closure(spec.names, body, env, name);
}

/* V3CACHE_FN_END b226ef8221743dfc */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN ef47b9f7f4801e10 */
__v_result_machine__Matched machine__Machine__match_at(machine__Machine* m, i32 pat, blip__Value subject, i64 depth) {
	if (depth > 40) {
		return (__v_result_machine__Matched){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_32_d03b5797a75a96c8, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	reader__Datum* d = reader__Arena__node(m->arena, pat);
	{
		reader__DatumTag __match_tmp_0 = d->tag;
		if (__match_tmp_0 == reader__DatumTag__sym) {
			if (string__eq(d->value, _v3_lit_1_44bd84d473cd8ed4)) {
				return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = true, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
			}
			if (string__eq(d->value, _v3_lit_4_6160ced97abcf88a)) {
				return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = true, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
			}
			Array __arr_lit_1 = array_new(			sizeof(machine__Bind), 0, 1);
			machine__Bind __arr_val_2 = (machine__Bind){.name = d->value, .val = subject};
			array_push(&__arr_lit_1, &__arr_val_2);
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = true, .binds = __arr_lit_1}};
		} else if (__match_tmp_0 == reader__DatumTag__kw) {
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = prims__value_eq(subject, blip__keyword(d->value)), .binds = array_new(sizeof(machine__Bind), 0, 0)}};
		} else if ((__match_tmp_0 == reader__DatumTag__v_int) || (__match_tmp_0 == reader__DatumTag__v_float) || (__match_tmp_0 == reader__DatumTag__str) || (__match_tmp_0 == reader__DatumTag__bool) || (__match_tmp_0 == reader__DatumTag__v_char) || (__match_tmp_0 == reader__DatumTag__rat)) {
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = machine__literal_eq(d, subject), .binds = array_new(sizeof(machine__Bind), 0, 0)}};
		} else if (__match_tmp_0 == reader__DatumTag__nil) {
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = blip__Value__is_empty_seq(subject), .binds = array_new(sizeof(machine__Bind), 0, 0)}};
		} else if (__match_tmp_0 == reader__DatumTag__vector) {
			Array kids = reader__Arena__kids(m->arena, pat);
			if (subject.tag != 1024) {
				return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
			}
			Array data = (blip__Value__as_vector(subject))->data;
			if (data.len != kids.len) {
				return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
			}
			Array binds = array_new(			sizeof(machine__Bind), 0, 0);
			i64 i = 0;
			while (i < kids.len) {
				__v_result_machine__Matched __or_opt_3 = machine__Machine__match_at(m, (*(i32*)array_get(kids, i)), (*(blip__Value*)array_get(data, i)), depth + 1);
				machine__Matched __or_val_4 = {0};
				if (__or_opt_3.ok) {
					__or_val_4 = __or_opt_3.value;
				} else {
					return (__v_result_machine__Matched){.ok = false, .err = __or_opt_3.err};
				}
				machine__Matched sub = __or_val_4;
				if (!sub.ok) {
					return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
				}
				array__push_many(&binds, sub.binds.data, sub.binds.len);
				i++;
			}
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = true, .binds = binds}};
		} else if (__match_tmp_0 == reader__DatumTag__table) {
			return machine__Machine__match_table_at(m, pat, subject, depth);
		} else if (__match_tmp_0 == reader__DatumTag__list) {
			return machine__Machine__match_list_at(m, pat, subject, depth);
		} else {
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
		}
	}
	return (__v_result_machine__Matched){.ok = true};
}

/* V3CACHE_FN_END ef47b9f7f4801e10 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN a6fea992fddc9969 */
i32 machine__Machine__match_clause(machine__Machine* m, i32 subject, i32 pattern) {
	Array __arr_lit_0 = __new_array_noscan(0, 2, sizeof(i32));
	i32 __arr_val_1 = machine__Machine__string_node(m, _v3_lit_21_158169dd6b184c52);
	array_push(&__arr_lit_0, &__arr_val_1);
	Array __arr_lit_3 = __new_array_noscan(0, 3, sizeof(i32));
	i32 __arr_val_4 = machine__Machine__string_node(m, _v3_lit_20_9af357b6465f2ccc);
	array_push(&__arr_lit_3, &__arr_val_4);
	i32 __arr_val_5 = subject;
	array_push(&__arr_lit_3, &__arr_val_5);
	i32 __arr_val_6 = pattern;
	array_push(&__arr_lit_3, &__arr_val_6);
	i32 __arr_val_2 = machine__Machine__call_node(m, machine__Machine__sym_node(m, _v3_lit_6_2ed40dcc7fa0e8ac), __arr_lit_3);
	array_push(&__arr_lit_0, &__arr_val_2);
	i32 fail = machine__Machine__node_of(m, _v3_lit_5_7abebb90e73faeeb, __arr_lit_0);
	Array __arr_lit_7 = __new_array_noscan(0, 3, sizeof(i32));
	i32 __arr_val_8 = subject;
	array_push(&__arr_lit_7, &__arr_val_8);
	Array __arr_lit_10 = __new_array_noscan(0, 2, sizeof(i32));
	i32 __arr_val_11 = pattern;
	array_push(&__arr_lit_10, &__arr_val_11);
	i32 __arr_val_12 = machine__Machine__sym_node(m, _v3_lit_3_3ff5d95143129022);
	array_push(&__arr_lit_10, &__arr_val_12);
	i32 __arr_val_9 = machine__Machine__list_of(m, __arr_lit_10);
	array_push(&__arr_lit_7, &__arr_val_9);
	Array __arr_lit_14 = __new_array_noscan(0, 2, sizeof(i32));
	i32 __arr_val_15 = machine__Machine__sym_node(m, _v3_lit_1_44bd84d473cd8ed4);
	array_push(&__arr_lit_14, &__arr_val_15);
	i32 __arr_val_16 = fail;
	array_push(&__arr_lit_14, &__arr_val_16);
	i32 __arr_val_13 = machine__Machine__list_of(m, __arr_lit_14);
	array_push(&__arr_lit_7, &__arr_val_13);
	return machine__Machine__node_of(m, _v3_lit_5_1118636f8a4d8f74, __arr_lit_7);
}

/* V3CACHE_FN_END a6fea992fddc9969 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 5ec7da5ffe0591f3 */
__v_result_machine__Matched machine__Machine__match_list_at(machine__Machine* m, i32 pat, blip__Value subject, i64 depth) {
	Array kids = reader__Arena__kids(m->arena, pat);
	if (kids.len == 0) {
		return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = blip__Value__is_empty_seq(subject), .binds = array_new(sizeof(machine__Bind), 0, 0)}};
	}
	string head = machine__Machine__pattern_head(m, pat);
	if (string__eq(head, _v3_lit_3_e1761a510dc506d4)) {
		Array binds = array_new(		sizeof(machine__Bind), 0, 0);
		i64 i = 1;
		while (i < kids.len) {
			__v_result_machine__Matched __or_opt_0 = machine__Machine__match_at(m, (*(i32*)array_get(kids, i)), subject, depth + 1);
			machine__Matched __or_val_1 = {0};
			if (__or_opt_0.ok) {
				__or_val_1 = __or_opt_0.value;
			} else {
				return (__v_result_machine__Matched){.ok = false, .err = __or_opt_0.err};
			}
			machine__Matched sub = __or_val_1;
			if (!sub.ok) {
				return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
			}
			array__push_many(&binds, sub.binds.data, sub.binds.len);
			i++;
		}
		return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = true, .binds = binds}};
	} else if (string__eq(head, _v3_lit_2_9bce5b00c678a682)) {
		i64 i = 1;
		while (i < kids.len) {
			__v_result_machine__Matched __or_opt_2 = machine__Machine__match_at(m, (*(i32*)array_get(kids, i)), subject, depth + 1);
			machine__Matched __or_val_3 = {0};
			if (__or_opt_2.ok) {
				__or_val_3 = __or_opt_2.value;
			} else {
				return (__v_result_machine__Matched){.ok = false, .err = __or_opt_2.err};
			}
			machine__Matched sub = __or_val_3;
			if (sub.ok) {
				return (__v_result_machine__Matched){.ok = true, .value = sub};
			}
			i++;
		}
		return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
	} else if (string__eq(head, _v3_lit_4_e6cd8fa8beffb8ce)) {
		if (subject.tag != 256) {
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
		}
		Array binds = array_new(		sizeof(machine__Bind), 0, 0);
		i64 i = 1;
		while (i < kids.len) {
			__v_result_machine__Matched __or_opt_4 = machine__Machine__match_at(m, (*(i32*)array_get(kids, i)), (blip__Value__as_pair(subject))->car, depth + 1);
			machine__Matched __or_val_5 = {0};
			if (__or_opt_4.ok) {
				__or_val_5 = __or_opt_4.value;
			} else {
				return (__v_result_machine__Matched){.ok = false, .err = __or_opt_4.err};
			}
			machine__Matched sub = __or_val_5;
			if (!sub.ok) {
				return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
			}
			array__push_many(&binds, sub.binds.data, sub.binds.len);
			i++;
		}
		__v_result_machine__Matched __or_opt_6 = machine__Machine__match_at(m, (*(i32*)array_get(kids, kids.len - 1)), (blip__Value__as_pair(subject))->cdr, depth + 1);
		machine__Matched __or_val_7 = {0};
		if (__or_opt_6.ok) {
			__or_val_7 = __or_opt_6.value;
		} else {
			return (__v_result_machine__Matched){.ok = false, .err = __or_opt_6.err};
		}
		machine__Matched sub = __or_val_7;
		if (!sub.ok) {
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
		}
		array__push_many(&binds, sub.binds.data, sub.binds.len);
		return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = true, .binds = binds}};
	} else if (string__eq(head, _v3_lit_4_a1c80526911571eb) || string__eq(head, _v3_lit_6_a652a2d3592ea6e0)) {
		__v_result_Array __or_opt_8 = prims__seq(subject);
		Array __or_val_9 = {0};
		if (__or_opt_8.ok) {
			__or_val_9 = __or_opt_8.value;
		} else {
			IError err = __or_opt_8.err;
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
		}
		Array items = __or_val_9;
		Array rest = array_slice(		kids, 		1, (		kids).len);
		if ((rest.len == 0) || machine__Machine__is_wildcard(m, (*(i32*)array_get(rest, rest.len - 1)))) {
			if ((rest.len - 1) != items.len) {
				return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
			}
		} else if (rest.len != items.len) {
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
		}
		Array binds = array_new(		sizeof(machine__Bind), 0, 0);
		i64 i = 0;
		while (i < rest.len) {
			blip__Value want = (*(blip__Value*)array_get(items, i));
			if (machine__Machine__is_wildcard(m, (*(i32*)array_get(rest, i)))) {
				i++;
				continue;
			}
			__v_result_machine__Matched __or_opt_10 = machine__Machine__match_at(m, (*(i32*)array_get(rest, i)), want, depth + 1);
			machine__Matched __or_val_11 = {0};
			if (__or_opt_10.ok) {
				__or_val_11 = __or_opt_10.value;
			} else {
				return (__v_result_machine__Matched){.ok = false, .err = __or_opt_10.err};
			}
			machine__Matched sub = __or_val_11;
			if (!sub.ok) {
				return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
			}
			array__push_many(&binds, sub.binds.data, sub.binds.len);
			i++;
		}
		return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = true, .binds = binds}};
	} else if (string__eq(head, _v3_lit_6_14da8f5ec8315a0a)) {
		return machine__Machine__match_struct_at(m, pat, subject, depth);
	} else if (string__eq(head, _v3_lit_0_14650fb0739d0383)) {
		return machine__Machine__positional(m, kids, subject, depth);
	} else {
		if (!machine__pattern_predicate(head)) {
			return machine__Machine__positional(m, kids, subject, depth);
		}
		Array call = array_new(		sizeof(blip__Value), 0, 0);
		blip__Value __arr_val_12 = subject;
		array_push(&call, &__arr_val_12);
		i64 i = 1;
		while (i < kids.len) {
			blip__Value __arr_val_13 = machine__datum_literal(reader__Arena__node(m->arena, (*(i32*)array_get(kids, i))));
			array_push(&call, &__arr_val_13);
			i++;
		}
		string __map_key_16 = head;
		void* __map_ptr_17 = map__get_check(&m->prims, &__map_key_16);
		_fn_ptr_d85a1a99927adfac __map_val_18 = {0};
		if (__map_ptr_17 != NULL) {
			__map_val_18 = *(_fn_ptr_d85a1a99927adfac*)(__map_ptr_17);
		} else {
			return (__v_result_machine__Matched){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_7_b8f61d1b0b1a53de; __v3_internal_symbol_join_0[1] = head; __v3_internal_symbol_join_0[2] = _v3_lit_64_491685a9fc0ec99a; string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		_fn_ptr_d85a1a99927adfac f = __map_val_18;
		__v_result_blip__Value __or_opt_19 = f(call);
		blip__Value __or_val_20 = {0};
		if (__or_opt_19.ok) {
			__or_val_20 = __or_opt_19.value;
		} else {
			IError err = __or_opt_19.err;
			return (__v_result_machine__Matched){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[4]; __v3_internal_symbol_join_1[0] = _v3_lit_7_b8f61d1b0b1a53de; __v3_internal_symbol_join_1[1] = head; __v3_internal_symbol_join_1[2] = _v3_lit_9_155bc01a1f2a1092; __v3_internal_symbol_join_1[3] = IError__msg(&(err)); string_plus_many(4, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		blip__Value res = __or_val_20;
		return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = blip__Value__truthy(res), .binds = array_new(sizeof(machine__Bind), 0, 0)}};
	}
	return (__v_result_machine__Matched){.ok = true};
}

/* V3CACHE_FN_END 5ec7da5ffe0591f3 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 95045f95296ad59a */
__v_result_machine__Matched machine__Machine__match_list_tail(machine__Machine* m, Array kids, blip__Value subject, i64 depth) {
	__v_result_Array __or_opt_0 = prims__seq(subject);
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
	}
	Array items = __or_val_1;
	Array rest = array_slice(	kids, 	1, (	kids).len);
	Array binds = array_new(	sizeof(machine__Bind), 0, 0);
	i64 i = 0;
	while (i < rest.len) {
		if ((i == (rest.len - 1)) && machine__Machine__is_capturing_tail(m, (*(i32*)array_get(rest, i)))) {
			blip__Value tail = blip__empty_list();
			if (items.len > i) {
				tail = blip__list_from(array_slice(				items, 				i, (				items).len));
			}
			__v_result_machine__Matched __or_opt_2 = machine__Machine__match_at(m, (*(i32*)array_get(rest, i)), tail, depth + 1);
			machine__Matched __or_val_3 = {0};
			if (__or_opt_2.ok) {
				__or_val_3 = __or_opt_2.value;
			} else {
				return (__v_result_machine__Matched){.ok = false, .err = __or_opt_2.err};
			}
			machine__Matched sub = __or_val_3;
			if (!sub.ok) {
				return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
			}
			array__push_many(&binds, sub.binds.data, sub.binds.len);
			i++;
			continue;
		}
		if (i >= items.len) {
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
		}
		__v_result_machine__Matched __or_opt_4 = machine__Machine__match_at(m, (*(i32*)array_get(rest, i)), (*(blip__Value*)array_get(items, i)), depth + 1);
		machine__Matched __or_val_5 = {0};
		if (__or_opt_4.ok) {
			__or_val_5 = __or_opt_4.value;
		} else {
			return (__v_result_machine__Matched){.ok = false, .err = __or_opt_4.err};
		}
		machine__Matched sub = __or_val_5;
		if (!sub.ok) {
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
		}
		array__push_many(&binds, sub.binds.data, sub.binds.len);
		i++;
	}
	return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = true, .binds = binds}};
	return (__v_result_machine__Matched){.ok = true};
}

/* V3CACHE_FN_END 95045f95296ad59a */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 7609d1521979abcd */
__v_result_machine__Matched machine__Machine__match_pattern(machine__Machine* m, i32 pat, blip__Value subject) {
	return machine__Machine__match_at(m, pat, subject, 0);
	return (__v_result_machine__Matched){.ok = true};
}

/* V3CACHE_FN_END 7609d1521979abcd */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN d1bd6ea4ebc7a44e */
__v_result_machine__Matched machine__Machine__match_struct_at(machine__Machine* m, i32 pat, blip__Value subject, i64 depth) {
	Array kids = reader__Arena__kids(m->arena, pat);
	if (kids.len < 2) {
		return (__v_result_machine__Matched){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_69_71265a5bfb06d239, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	reader__Datum* name_node = reader__Arena__node(m->arena, (*(i32*)array_get(kids, 1)));
	if (name_node->tag != reader__DatumTag__sym) {
		return (__v_result_machine__Matched){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_43_f7fea9b678468802, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	if (subject.tag != 131072) {
		return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
	}
	blip__StructVal* s = blip__Value__as_struct(subject);
	if (!string__eq(s->name, name_node->value)) {
		return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
	}
	Array binds = array_new(	sizeof(machine__Bind), 0, 0);
	i64 i = 2;
	while (i < kids.len) {
		reader__Datum* d = reader__Arena__node(m->arena, (*(i32*)array_get(kids, i)));
		string field = _v3_lit_0_14650fb0739d0383;
		if (d->tag == reader__DatumTag__kw) {
			field = d->value;
		} else if ((d->tag == reader__DatumTag__sym) && string__ends_with(d->value, _v3_lit_1_44bd61d473cd535b)) {
			field = string__substr(			d->value, 0, 			d->value.len - 1);
		} else {
			return (__v_result_machine__Matched){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_60_a3cd2806f6be2de7, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		if (!blip__StructVal__has(s, field)) {
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
		}
		if ((i + 1) >= kids.len) {
			return (__v_result_machine__Matched){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_13_e1bc7ef34ca037fe; __v3_internal_symbol_join_0[1] = field; __v3_internal_symbol_join_0[2] = _v3_lit_15_6f0b7687d7432444; string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		__v_result_machine__Matched __or_opt_0 = machine__Machine__match_at(m, (*(i32*)array_get(kids, i + 1)), blip__StructVal__get(s, field), depth + 1);
		machine__Matched __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result_machine__Matched){.ok = false, .err = __or_opt_0.err};
		}
		machine__Matched sub = __or_val_1;
		if (!sub.ok) {
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
		}
		array__push_many(&binds, sub.binds.data, sub.binds.len);
		i += 2;
	}
	return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = true, .binds = binds}};
	return (__v_result_machine__Matched){.ok = true};
}

/* V3CACHE_FN_END d1bd6ea4ebc7a44e */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 567cd20c991512eb */
__v_result_machine__Matched machine__Machine__match_table_at(machine__Machine* m, i32 pat, blip__Value subject, i64 depth) {
	if (!((subject.tag == 4096) || (subject.tag == 8192) || (subject.tag == 131072))) {
		return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
	}
	Array kids = reader__Arena__kids(m->arena, pat);
	blip__Table* tbl = blip__Value__as_table(subject);
	Array binds = array_new(	sizeof(machine__Bind), 0, 0);
	i64 i = 0;
	while ((i + 1) < kids.len) {
		reader__Datum* key = reader__Arena__node(m->arena, (*(i32*)array_get(kids, i)));
		string k = _v3_lit_0_14650fb0739d0383;
		if (key->tag == reader__DatumTag__kw) {
			k = key->value;
		} else if ((key->tag == reader__DatumTag__str) || (key->tag == reader__DatumTag__sym)) {
			k = key->value;
		} else {
			return (__v_result_machine__Matched){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_41_a323e527c200fee3, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		if (!blip__Table__has(tbl, k)) {
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
		}
		__v_result_machine__Matched __or_opt_0 = machine__Machine__match_at(m, (*(i32*)array_get(kids, i + 1)), blip__Table__get(tbl, k), depth + 1);
		machine__Matched __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result_machine__Matched){.ok = false, .err = __or_opt_0.err};
		}
		machine__Matched sub = __or_val_1;
		if (!sub.ok) {
			return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = false, .binds = array_new(sizeof(machine__Bind), 0, 0)}};
		}
		array__push_many(&binds, sub.binds.data, sub.binds.len);
		i += 2;
	}
	return (__v_result_machine__Matched){.ok = true, .value = (machine__Matched){.ok = true, .binds = binds}};
	return (__v_result_machine__Matched){.ok = true};
}

/* V3CACHE_FN_END 567cd20c991512eb */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN efe9f92b3a4f6f7d */
i32 machine__Machine__nest_lets(machine__Machine* m, Array groups, Array body) {
	if (groups.len == 0) {
		return machine__Machine__make_begin(m, body);
	}
	machine__Group g = (*(machine__Group*)array_get(groups, 0));
	Array __arr_lit_0 = __new_array_noscan(0, 2, sizeof(i32));
	i32 __arr_val_1 = machine__Machine__list_of(m, g.params);
	array_push(&__arr_lit_0, &__arr_val_1);
	i32 __arr_val_2 = machine__Machine__nest_lets(m, array_slice(	groups, 	1, (	groups).len), body);
	array_push(&__arr_lit_0, &__arr_val_2);
	i32 lam = machine__Machine__node_of(m, _v3_lit_6_497dd6d1afd39ae4, __arr_lit_0);
	return machine__Machine__call_node(m, lam, g.args);
}

/* V3CACHE_FN_END efe9f92b3a4f6f7d */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN d785bcf1eefd6e98 */
i32 machine__Machine__nil_node(machine__Machine* m) {
	return reader__Arena__leaf(m->arena, reader__DatumTag__nil);
}

/* V3CACHE_FN_END d785bcf1eefd6e98 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN dfe3f7de7b18150e */
i32 machine__Machine__node_of(machine__Machine* m, string head, Array tail) {
	Array items = __new_array_noscan(0, 0, sizeof(i32));
	i32 __arr_val_0 = machine__Machine__sym_node(m, head);
	array_push(&items, &__arr_val_0);
	i64 ti = 0;
	while (ti < tail.len) {
		i32 __arr_val_1 = (*(i32*)array_get(tail, ti));
		array_push(&items, &__arr_val_1);
		ti++;
	}
	return machine__Machine__list_of(m, items);
}

/* V3CACHE_FN_END dfe3f7de7b18150e */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN d77fa72ff17d36ea */
__v_result_machine__ParamSpec machine__Machine__parse_params(machine__Machine* m, Array plist, string who) {
	machine__ParamSpec spec = (machine__ParamSpec){.names = array_new(	sizeof(string), 0, 0), .opt_names = array_new(	sizeof(string), 0, 0), .opt_defaults = __new_array_noscan(0, 0, sizeof(i32)), .opt_from = -1, .rest = _v3_lit_0_14650fb0739d0383, .destructures = array_new(sizeof(machine__Destructure), 0, 0)};
	i64 i = 0;
	while (i < plist.len) {
		reader__Datum* part = reader__Arena__node(m->arena, (*(i32*)array_get(plist, i)));
		if ((part->tag == reader__DatumTag__list) || (part->tag == reader__DatumTag__table)) {
			string tmp = machine__Machine__fresh(m, _v3_lit_5_2a80483481630ae0);
			i32 temp_node = machine__Machine__sym_node(m, tmp);
			string __arr_val_0 = string__clone(tmp);
			array_push(&spec.names, &__arr_val_0);
			__v_result_Array __or_opt_1 = machine__Machine__destructuring(m, (*(i32*)array_get(plist, i)), temp_node);
			Array __or_val_2 = {0};
			if (__or_opt_1.ok) {
				__or_val_2 = __or_opt_1.value;
			} else {
				return (__v_result_machine__ParamSpec){.ok = false, .err = __or_opt_1.err};
			}
			machine__Destructure __arr_val_3 = (machine__Destructure){.temp = temp_node, .parts = __or_val_2};
			array_push(&spec.destructures, &__arr_val_3);
			i++;
			continue;
		}
		if (part->tag == reader__DatumTag__kw) {
			if ((i + 1) >= plist.len) {
				return (__v_result_machine__ParamSpec){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[4]; __v3_internal_symbol_join_0[0] = who; __v3_internal_symbol_join_0[1] = _v3_lit_2_9b20b700c5e4da01; __v3_internal_symbol_join_0[2] = machine__Machine__render_node(m, (*(i32*)array_get(plist, i))); __v3_internal_symbol_join_0[3] = _v3_lit_32_67ed9a70b0946400; string_plus_many(4, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			reader__Datum* holder = reader__Arena__node(m->arena, (*(i32*)array_get(plist, i + 1)));
			if (holder->tag != reader__DatumTag__vector) {
				return (__v_result_machine__ParamSpec){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[6]; __v3_internal_symbol_join_1[0] = who; __v3_internal_symbol_join_1[1] = _v3_lit_2_9b20b700c5e4da01; __v3_internal_symbol_join_1[2] = machine__Machine__render_node(m, (*(i32*)array_get(plist, i))); __v3_internal_symbol_join_1[3] = _v3_lit_17_19823545a3322d36; __v3_internal_symbol_join_1[4] = machine__Machine__render_node(m, (*(i32*)array_get(plist, i))); __v3_internal_symbol_join_1[5] = _v3_lit_15_935b1751495b3b8f; string_plus_many(6, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			Array inner = reader__Arena__kids(m->arena, (*(i32*)array_get(plist, i + 1)));
			if ((inner.len != 1) && (inner.len != 2)) {
				return (__v_result_machine__ParamSpec){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_2[4]; __v3_internal_symbol_join_2[0] = who; __v3_internal_symbol_join_2[1] = _v3_lit_2_9b20b700c5e4da01; __v3_internal_symbol_join_2[2] = machine__Machine__render_node(m, (*(i32*)array_get(plist, i))); __v3_internal_symbol_join_2[3] = _v3_lit_44_77642ffa6ebb3888; string_plus_many(4, __v3_internal_symbol_join_2); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			string nm_name = _v3_lit_0_14650fb0739d0383;
			i32 def = machine__no_default;
			if (inner.len == 2) {
				reader__Datum* nn = reader__Arena__node(m->arena, (*(i32*)array_get(inner, 0)));
				if (nn->tag != reader__DatumTag__sym) {
					return (__v_result_machine__ParamSpec){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_3[4]; __v3_internal_symbol_join_3[0] = who; __v3_internal_symbol_join_3[1] = _v3_lit_24_387a7b0f1749397f; __v3_internal_symbol_join_3[2] = machine__Machine__render_node(m, (*(i32*)array_get(plist, i + 1))); __v3_internal_symbol_join_3[3] = _v3_lit_14_4c516eb5bd30ac9e; string_plus_many(4, __v3_internal_symbol_join_3); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
				}
				nm_name = nn->value;
				def = (*(i32*)array_get(inner, 1));
			} else {
				nm_name = part->value;
				def = (*(i32*)array_get(inner, 0));
			}
			if (spec.opt_from < 0) {
				spec.opt_from = spec.names.len;
			}
			string __arr_val_4 = string__clone(part->value);
			array_push(&spec.opt_names, &__arr_val_4);
			string __arr_val_5 = string__clone(nm_name);
			array_push(&spec.names, &__arr_val_5);
			i32 __arr_val_6 = def;
			array_push(&spec.opt_defaults, &__arr_val_6);
			i += 2;
			continue;
		}
		if (part->tag != reader__DatumTag__sym) {
			return (__v_result_machine__ParamSpec){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_4[4]; __v3_internal_symbol_join_4[0] = who; __v3_internal_symbol_join_4[1] = _v3_lit_12_88a87abdc94c4c4c; __v3_internal_symbol_join_4[2] = int__str(i); __v3_internal_symbol_join_4[3] = _v3_lit_14_4c516eb5bd30ac9e; string_plus_many(4, __v3_internal_symbol_join_4); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		if (string__eq(part->value, _v3_lit_1_44bd55d473cd3ef7)) {
			if ((i + 2) != plist.len) {
				return (__v_result_machine__ParamSpec){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(who, _v3_lit_43_57cc29f0e837da19), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			reader__Datum* nm = reader__Arena__node(m->arena, (*(i32*)array_get(plist, i + 1)));
			if (nm->tag != reader__DatumTag__sym) {
				return (__v_result_machine__ParamSpec){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(who, _v3_lit_31_a18017baa24e937f), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			spec.rest = nm->value;
			spec.is_rest = true;
			i += 2;
			continue;
		}
		bool dup = false;
		Array seen = array_new(		sizeof(string), 0, 0);
		array__push_many(&seen, spec.names.data, spec.names.len);
		{
			i64 __for_idx_7 = 0;
			for (; __for_idx_7 < seen.len; __for_idx_7++) {
				string n = *(string*)(array_get(seen, __for_idx_7));
				if (string__eq(n, part->value)) {
					dup = true;
				}
			}
		}
		if (dup) {
			return (__v_result_machine__ParamSpec){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_5[4]; __v3_internal_symbol_join_5[0] = who; __v3_internal_symbol_join_5[1] = _v3_lit_12_88a87abdc94c4c4c; __v3_internal_symbol_join_5[2] = part->value; __v3_internal_symbol_join_5[3] = _v3_lit_15_68938722c38edfc0; string_plus_many(4, __v3_internal_symbol_join_5); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		string __arr_val_8 = string__clone(part->value);
		array_push(&spec.names, &__arr_val_8);
		i++;
	}
	if (spec.is_rest) {
		{
			i64 __for_idx_9 = 0;
			for (; __for_idx_9 < spec.names.len; __for_idx_9++) {
				string n = *(string*)(array_get(spec.names, __for_idx_9));
				if (string__eq(n, spec.rest)) {
					return (__v_result_machine__ParamSpec){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_6[4]; __v3_internal_symbol_join_6[0] = who; __v3_internal_symbol_join_6[1] = _v3_lit_2_9b20b700c5e4da01; __v3_internal_symbol_join_6[2] = spec.rest; __v3_internal_symbol_join_6[3] = _v3_lit_37_7f6f45e5ada2c7e4; string_plus_many(4, __v3_internal_symbol_join_6); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
				}
			}
		}
	}
	return (__v_result_machine__ParamSpec){.ok = true, .value = spec};
	return (__v_result_machine__ParamSpec){.ok = true};
}

/* V3CACHE_FN_END d77fa72ff17d36ea */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN d603962ab1ce792e */
string machine__Machine__pattern_head(machine__Machine* m, i32 p) {
	reader__Datum* d = reader__Arena__node(m->arena, p);
	if (d->tag != reader__DatumTag__list) {
		return _v3_lit_0_14650fb0739d0383;
	}
	Array kids = reader__Arena__kids(m->arena, p);
	if (kids.len == 0) {
		return _v3_lit_0_14650fb0739d0383;
	}
	reader__Datum* h = reader__Arena__node(m->arena, (*(i32*)array_get(kids, 0)));
	if (h->tag != reader__DatumTag__sym) {
		return _v3_lit_0_14650fb0739d0383;
	}
	return h->value;
}

/* V3CACHE_FN_END d603962ab1ce792e */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 61e37981b020f641 */
i32 machine__Machine__pipe_one(machine__Machine* m, i32 acc, i32 step, string mode) {
	reader__Datum* d = reader__Arena__node(m->arena, step);
	if ((d->tag == reader__DatumTag__sym) && machine__pipe_tap(d->value)) {
		return machine__Machine__pipe_through(m, acc, step, true);
	}
	string slot = mode;
	if (string__eq(mode, _v3_lit_2_9c0ea100c6af0169)) {
		string __if_val_0 = {0};
		if (machine__Machine__is_call_form(m, step)) {
			__if_val_0 = _v3_lit_2_9afb1f00c5c4bb7c;
		} else {
			__if_val_0 = _v3_lit_3_1d70f0500d4a3126;
		}
		slot = __if_val_0;
	}
	return machine__Machine__pipe_through(m, acc, step, string__eq(slot, _v3_lit_3_1d70f0500d4a3126));
}

/* V3CACHE_FN_END 61e37981b020f641 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN a7673264bdb7f4a2 */
i32 machine__Machine__pipe_through(machine__Machine* m, i32 acc, i32 step, bool last) {
	reader__Datum* d = reader__Arena__node(m->arena, step);
	if (d->tag != reader__DatumTag__list) {
		return machine__Machine__list_of(m, new_array_from_c_array(2, 2, sizeof(i32), (i32[2]){step, acc}));
	}
	Array skids = reader__Arena__kids(m->arena, step);
	if (last) {
		Array items = __new_array_noscan(0, 0, sizeof(i32));
		i64 si = 0;
		while (si < skids.len) {
			i32 __arr_val_0 = (*(i32*)array_get(skids, si));
			array_push(&items, &__arr_val_0);
			si++;
		}
		i32 __arr_val_1 = acc;
		array_push(&items, &__arr_val_1);
		return machine__Machine__list_of(m, items);
	}
	Array items = __new_array_noscan(0, 0, sizeof(i32));
	i32 __arr_val_2 = (*(i32*)array_get(skids, 0));
	array_push(&items, &__arr_val_2);
	i32 __arr_val_3 = acc;
	array_push(&items, &__arr_val_3);
	i64 j = 1;
	while (j < skids.len) {
		i32 __arr_val_4 = (*(i32*)array_get(skids, j));
		array_push(&items, &__arr_val_4);
		j++;
	}
	return machine__Machine__list_of(m, items);
}

/* V3CACHE_FN_END a7673264bdb7f4a2 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 6cc9d2be43f3df8b */
blip__Kont machine__Machine__pop(machine__Machine* m) {
	blip__Kont last = (*(blip__Kont*)array_get(m->kstack, m->kstack.len - 1));
	m->kstack = array_slice(	m->kstack, 0, 	m->kstack.len - 1);
	return last;
}

/* V3CACHE_FN_END 6cc9d2be43f3df8b */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN c5819edc58b3a1b3 */
__v_result_machine__Matched machine__Machine__positional(machine__Machine* m, Array kids, blip__Value subject, i64 depth) {
	return machine__Machine__match_list_tail(m, kids, subject, depth);
	return (__v_result_machine__Matched){.ok = true};
}

/* V3CACHE_FN_END c5819edc58b3a1b3 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN a796abfa26ce6be8 */
void machine__Machine__push(machine__Machine* m, blip__Kont k) {
	blip__Kont __derived_clone_0 = k;
	__derived_clone_0.acc = array__clone(&__derived_clone_0.acc);
	__derived_clone_0.name = string__clone(__derived_clone_0.name);
	__derived_clone_0.clauses = array__clone(&__derived_clone_0.clauses);
	Array __derived_clone_array_1 = array__clone(&__derived_clone_0.binds);
	{
		i64 __derived_clone_array_idx_2 = 0;
		for (; __derived_clone_array_idx_2 < __derived_clone_array_1.len; __derived_clone_array_idx_2++) {
			{ Array* _a0 = &__derived_clone_array_1; int _i0 = __derived_clone_array_idx_2; array__set(_a0, _i0, &(string[]){string__clone(*(string*)(array_get(__derived_clone_0.binds, __derived_clone_array_idx_2)))}); }
		}
	}
	__derived_clone_0.binds = __derived_clone_array_1;
	__derived_clone_0.vals = array__clone(&__derived_clone_0.vals);
	blip__Kont __arr_val_3 = __derived_clone_0;
	array_push(&m->kstack, &__arr_val_3);
}

/* V3CACHE_FN_END a796abfa26ce6be8 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 38b4a999efce76d3 */
blip__Value machine__Machine__quasi_to_value(machine__Machine* m, i32 id) {
	reader__Datum* d = reader__Arena__node(m->arena, id);
	if ((d->tag == reader__DatumTag__unquote) || (d->tag == reader__DatumTag__unquote_splice)) {
		i32 inner = (*(i32*)array_get(reader__Arena__kids(m->arena, id), 0));
		Array saved_k = m->kstack;
		int saved_ctl = m->ctl;
		i32 saved_form = m->form;
		i32 saved_env = m->env;
		__v_result_blip__Value __or_opt_0 = machine__Machine__eval_one(m, inner);
		blip__Value __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			IError err = __or_opt_0.err;
			__or_val_1 = blip__nil_value();
		}
		blip__Value out = __or_val_1;
		m->kstack = saved_k;
		m->ctl = saved_ctl;
		m->form = saved_form;
		m->env = saved_env;
		return out;
	}
	return machine__Machine__datum_to_value(m, id);
}

/* V3CACHE_FN_END 38b4a999efce76d3 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 22b858d2c68eaf61 */
string machine__Machine__render_node(machine__Machine* m, i32 n) {
	if (n == blip__no_node) {
		return _v3_lit_0_14650fb0739d0383;
	}
	return printer__write_datum(m->arena, n);
}

/* V3CACHE_FN_END 22b858d2c68eaf61 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 470a475098dfaf2e */
Array machine__Machine__require_paths(machine__Machine* m, i32 n) {
	reader__Datum* d = reader__Arena__node(m->arena, n);
	if (d->tag == reader__DatumTag__str) {
		Array __arr_lit_0 = array_new(		sizeof(string), 0, 1);
		string __arr_val_1 = d->value;
		array_push(&__arr_lit_0, &__arr_val_1);
		return __arr_lit_0;
	}
	if (d->tag != reader__DatumTag__list) {
		return array_new(		sizeof(string), 0, 0);
	}
	Array out = array_new(	sizeof(string), 0, 0);
	Array kids = reader__Arena__kids(m->arena, n);
	i64 i = 0;
	while (i < kids.len) {
		Array part = machine__Machine__require_paths(m, (*(i32*)array_get(kids, i)));
		i64 j = 0;
		while (j < part.len) {
			string __arr_val_2 = string__clone((*(string*)array_get(part, j)));
			array_push(&out, &__arr_val_2);
			j++;
		}
		i++;
	}
	return out;
}

/* V3CACHE_FN_END 470a475098dfaf2e */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN ce59357b961e9ad7 */
void machine__Machine__reset(machine__Machine* m) {
	m->kstack = array_new(	sizeof(blip__Kont), 0, 0);
	m->ctl = 1;
	m->env = m->globals;
	m->form = blip__no_node;
	m->val = blip__nil_value();
}

/* V3CACHE_FN_END ce59357b961e9ad7 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 46ddfe2cb12bdeb7 */
__v_option_string machine__Machine__resolve_module(machine__Machine* m, string name) {
	if (string__eq(name, _v3_lit_0_14650fb0739d0383)) {
		return (__v_option_string){.ok = false};
	}
	string base = _v3_lit_12_e11aadbed58b02ce;
	string direct = os__join_path_single(os__join_path_single(base, name), _v3_lit_8_d64d97486c8170b7);
	if (os__exists(direct)) {
		return (__v_option_string){.ok = true, .value = direct};
	}
	if (string__contains(name, _v3_lit_1_44bd54d473cd3d44)) {
		return (__v_option_string){.ok = false};
	}
	__v_result_Array __or_opt_0 = os__ls(base);
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return (__v_option_string){.ok = false};
	}
	Array entries = __or_val_1;
	{
		i64 __for_idx_2 = 0;
		for (; __for_idx_2 < entries.len; __for_idx_2++) {
			string owner = *(string*)(array_get(entries, __for_idx_2));
			if (string__starts_with(owner, _v3_lit_1_44bd55d473cd3ef7)) {
				continue;
			}
			string candidate = os__join_path_single(os__join_path_single(base, owner), name);
			if (!os__is_dir(candidate)) {
				continue;
			}
			string entry = os__join_path_single(candidate, _v3_lit_8_d64d97486c8170b7);
			if (os__exists(entry)) {
				return (__v_option_string){.ok = true, .value = entry};
			}
		}
	}
	return (__v_option_string){.ok = false};
	return (__v_option_string){.ok = true};
}

/* V3CACHE_FN_END 46ddfe2cb12bdeb7 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 71dcd3e4741dabb6 */
string machine__Machine__resolve_path(machine__Machine* m, string path) {
	if (string__starts_with(path, _v3_lit_1_44bd54d473cd3d44) || string__starts_with(path, _v3_lit_1_44bd55d473cd3ef7) || string__eq(m->source, _v3_lit_0_14650fb0739d0383)) {
		return path;
	}
	string base = m->source;
	i64 cut = -1;
	i64 i = 0;
	while (i < base.len) {
		u8 c = string__at(base, i);
		if ((c == '/') || (c == '\\')) {
			cut = i;
		}
		i++;
	}
	if (cut < 0) {
		return path;
	}
	return ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = string__substr(	base, 0, 	cut); __v3_internal_symbol_join_0[1] = _v3_lit_1_44bd54d473cd3d44; __v3_internal_symbol_join_0[2] = path; string_plus_many(3, __v3_internal_symbol_join_0); });
}

/* V3CACHE_FN_END 71dcd3e4741dabb6 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 5ce1b25cd0968d46 */
__v_result machine__Machine__result_combinator(machine__Machine* m, string name, Array args) {
	if (args.len != 2) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = name; __v3_internal_symbol_join_0[1] = _v3_lit_26_4ace1ee796af487d; __v3_internal_symbol_join_0[2] = int__str(args.len); string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	bool lazy_unwrap = string__eq(name, _v3_lit_11_66673f96f53ffd17);
	if (machine__is_result_of((*(blip__Value*)array_get(args, 0)), _v3_lit_2_9bce5400c6789a9d)) {
		if (lazy_unwrap) {
			__v_result_Array __or_opt_0 = prims__seq((*(blip__Value*)array_get(args, 0)));
			Array __or_val_1 = {0};
			if (__or_opt_0.ok) {
				__or_val_1 = __or_opt_0.value;
			} else {
				return (__v_result){.ok = false, .err = __or_opt_0.err};
			}
			Array rest = __or_val_1;
			if (rest.len > 1) {
				m->val = (*(blip__Value*)array_get(rest, 1));
			} else {
				m->val = blip__nil_value();
			}
			machine__Machine__ret(m);
			return (__v_result){.ok = true};
		}
	} else if (lazy_unwrap) {
		__v_result_blip__Value __or_opt_2 = machine__Machine__call_value(m, (*(blip__Value*)array_get(args, 1)), array_new(		sizeof(blip__Value), 0, 0));
		blip__Value __or_val_3 = {0};
		if (__or_opt_2.ok) {
			__or_val_3 = __or_opt_2.value;
		} else {
			return (__v_result){.ok = false, .err = __or_opt_2.err};
		}
		m->val = __or_val_3;
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	} else {
		m->val = (*(blip__Value*)array_get(args, 0));
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	}
	Array one = array_new(	sizeof(blip__Value), 0, 0);
	blip__Value __arr_val_4 = machine__inner_value((*(blip__Value*)array_get(args, 0)));
	array_push(&one, &__arr_val_4);
	__v_result_blip__Value __or_opt_5 = machine__Machine__call_value(m, (*(blip__Value*)array_get(args, 1)), one);
	blip__Value __or_val_6 = {0};
	if (__or_opt_5.ok) {
		__or_val_6 = __or_opt_5.value;
	} else {
		return (__v_result){.ok = false, .err = __or_opt_5.err};
	}
	blip__Value v = __or_val_6;
	if (machine__is_result_of(v, _v3_lit_3_2249c511fb8ec16)) {
		m->val = v;
		machine__Machine__ret(m);
		return (__v_result){.ok = true};
	}
	Array __arr_lit_7 = array_new(	sizeof(blip__Value), 0, 2);
	blip__Value __arr_val_8 = blip__symbol(_v3_lit_2_9bce5400c6789a9d);
	array_push(&__arr_lit_7, &__arr_val_8);
	blip__Value __arr_val_9 = v;
	array_push(&__arr_lit_7, &__arr_val_9);
	m->val = blip__list_from(__arr_lit_7);
	machine__Machine__ret(m);
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END 5ce1b25cd0968d46 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 35a1dcac7a4a0707 */
void machine__Machine__ret(machine__Machine* m) {
	m->ctl = 2;
}

/* V3CACHE_FN_END 35a1dcac7a4a0707 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN a9354cac2a3bd7d1 */
__v_result_blip__Value machine__Machine__run(machine__Machine* m, Array forms) {
	blip__Value last = blip__nil_value();
	i64 i = 0;
	while (i < forms.len) {
		machine__Machine__reset(m);
		m->form = (*(i32*)array_get(forms, i));
		m->ctl = 1;
		__v_result_blip__Value __or_opt_0 = machine__Machine__drive(m);
		blip__Value __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
		}
		last = __or_val_1;
		i++;
	}
	return (__v_result_blip__Value){.ok = true, .value = last};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END a9354cac2a3bd7d1 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 70978daaf6dd1ad7 */
__v_result_blip__Value machine__Machine__run_checked(machine__Machine* m, reader__Forms res) {
	if (res.diags.len > 0) {
		reader__Diagnostic d = (*(reader__Diagnostic*)array_get(res.diags, 0));
		if (!string__eq(m->src_text, _v3_lit_0_14650fb0739d0383)) {
			return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = blip__format_error(machine__Machine__where(m), m->src_text, d.line, d.col, d.msg, 1), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		string __interp_part_1 = machine__Machine__where(m);
		string __interp_part_2 = _v3_lit_1_44bd61d473cd535b;
		string __interp_part_3 = int__str(d.line);
		string __interp_part_4 = _v3_lit_1_44bd61d473cd535b;
		string __interp_part_5 = int__str(d.col);
		string __interp_part_6 = _v3_lit_2_9b20b700c5e4da01;
		string __interp_part_7 = d.msg;
		string __if_val_0 = {0};
		if (res.diags.len > 1) {
			__if_val_0 = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_6_7e8af11904d0db3c; __v3_internal_symbol_join_0[1] = int__str(res.diags.len - 1); __v3_internal_symbol_join_0[2] = _v3_lit_6_dbe8ee8a70d5a57f; string_plus_many(3, __v3_internal_symbol_join_0); });
		} else {
			__if_val_0 = _v3_lit_0_14650fb0739d0383;
		}
		string __interp_part_8 = __if_val_0;
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[8]; __v3_internal_symbol_join_1[0] = __interp_part_1; __v3_internal_symbol_join_1[1] = __interp_part_2; __v3_internal_symbol_join_1[2] = __interp_part_3; __v3_internal_symbol_join_1[3] = __interp_part_4; __v3_internal_symbol_join_1[4] = __interp_part_5; __v3_internal_symbol_join_1[5] = __interp_part_6; __v3_internal_symbol_join_1[6] = __interp_part_7; __v3_internal_symbol_join_1[7] = __interp_part_8; string_plus_many(8, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return machine__Machine__run(m, res.forms);
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 70978daaf6dd1ad7 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 569d5743c3e31149 */
__v_result_blip__Value machine__Machine__run_str(machine__Machine* m, string src) {
	m->src_text = src;
	reader__Forms res = reader__Arena__read_forms(m->arena, src);
	return machine__Machine__run_checked(m, res);
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 569d5743c3e31149 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN bf23d5ac7273d7b2 */
__v_result machine__Machine__sort_by(machine__Machine* m, Array args) {
	if (args.len != 2) {
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_33_4dcffec9525ab545, int__str(args.len)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_Array __or_opt_0 = prims__seq((*(blip__Value*)array_get(args, 1)));
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_9_3d1169acaa77e3c9, IError__msg(&(err))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	Array items = __or_val_1;
	Array keys = array_new(	sizeof(blip__Value), 0, 0);
	i64 i = 0;
	while (i < items.len) {
		Array one = array_new(		sizeof(blip__Value), 0, 0);
		blip__Value __arr_val_2 = (*(blip__Value*)array_get(items, i));
		array_push(&one, &__arr_val_2);
		__v_result_blip__Value __or_opt_3 = machine__Machine__call_value(m, (*(blip__Value*)array_get(args, 0)), one);
		blip__Value __or_val_4 = {0};
		if (__or_opt_3.ok) {
			__or_val_4 = __or_opt_3.value;
		} else {
			return (__v_result){.ok = false, .err = __or_opt_3.err};
		}
		blip__Value __arr_val_5 = __or_val_4;
		array_push(&keys, &__arr_val_5);
		i++;
	}
	__v_result_blip__Value __or_opt_6 = prims__sort_by_keyed(items, keys);
	blip__Value __or_val_7 = {0};
	if (__or_opt_6.ok) {
		__or_val_7 = __or_opt_6.value;
	} else {
		return (__v_result){.ok = false, .err = __or_opt_6.err};
	}
	m->val = __or_val_7;
	machine__Machine__ret(m);
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END bf23d5ac7273d7b2 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN bda2f1aaf48c95b1 */
__v_result machine__Machine__step_eval(machine__Machine* m) {
	i32 id = m->form;
	reader__Datum* d = reader__Arena__node(m->arena, id);
	{
		reader__DatumTag __match_tmp_0 = d->tag;
		if (__match_tmp_0 == reader__DatumTag__nil) {
			m->val = blip__nil_value();
			machine__Machine__ret(m);
			return (__v_result){.ok = true};
		} else if (__match_tmp_0 == reader__DatumTag__bool) {
			m->val = blip__boolean(d->i != 0);
			machine__Machine__ret(m);
			return (__v_result){.ok = true};
		} else if (__match_tmp_0 == reader__DatumTag__v_int) {
			m->val = blip__integer(d->i);
			machine__Machine__ret(m);
			return (__v_result){.ok = true};
		} else if (__match_tmp_0 == reader__DatumTag__v_char) {
			m->val = blip__rune((u32)(d->i));
			machine__Machine__ret(m);
			return (__v_result){.ok = true};
		} else if (__match_tmp_0 == reader__DatumTag__v_float) {
			m->val = blip__float(d->f);
			machine__Machine__ret(m);
			return (__v_result){.ok = true};
		} else if (__match_tmp_0 == reader__DatumTag__str) {
			m->val = blip__string(d->value);
			machine__Machine__ret(m);
			return (__v_result){.ok = true};
		} else if (__match_tmp_0 == reader__DatumTag__kw) {
			m->val = blip__keyword(d->value);
			machine__Machine__ret(m);
			return (__v_result){.ok = true};
		} else if (__match_tmp_0 == reader__DatumTag__sym) {
			return machine__Machine__eval_symbol(m, d->value);
		} else if (__match_tmp_0 == reader__DatumTag__quoted) {
			m->val = machine__Machine__datum_to_value(m, (*(i32*)array_get(reader__Arena__kids(m->arena, id), 0)));
			machine__Machine__ret(m);
			return (__v_result){.ok = true};
		} else if (__match_tmp_0 == reader__DatumTag__quasi) {
			m->val = machine__Machine__quasi_to_value(m, (*(i32*)array_get(reader__Arena__kids(m->arena, id), 0)));
			machine__Machine__ret(m);
			return (__v_result){.ok = true};
		} else if (__match_tmp_0 == reader__DatumTag__list) {
			Array kids = reader__Arena__kids(m->arena, id);
			if (kids.len == 0) {
				m->val = blip__empty_list();
				machine__Machine__ret(m);
				return (__v_result){.ok = true};
			}
			return machine__Machine__eval_list(m, id, kids);
		} else if ((__match_tmp_0 == reader__DatumTag__table) || (__match_tmp_0 == reader__DatumTag__vector) || (__match_tmp_0 == reader__DatumTag__array)) {
			return machine__Machine__eval_collection(m, id);
		} else {
			m->val = machine__Machine__datum_to_value(m, id);
			machine__Machine__ret(m);
			return (__v_result){.ok = true};
		}
	}
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END bda2f1aaf48c95b1 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN c15a4b35c6754d61 */
__v_result_bool machine__Machine__step_return(machine__Machine* m) {
	if (m->kstack.len == 0) {
		return (__v_result_bool){.ok = true, .value = false};
	}
	blip__Kont k = machine__Machine__pop(m);
	{
		int __match_tmp_0 = k.tag;
		if (__match_tmp_0 == 2) {
			Array kids = reader__Arena__kids(m->arena, k.rest);
			if (kids.len == 1) {
				Array all = array_new(				sizeof(blip__Value), 0, 0);
				blip__Value __arr_val_1 = m->val;
				array_push(&all, &__arr_val_1);
				__v_result __or_opt_2 = machine__Machine__apply_all(m, all);
				if (!__or_opt_2.ok) {
					return (__v_result_bool){.ok = false, .err = __or_opt_2.err};
				}
			} else {
				blip__Kont k2 = machine__Machine__kont(m, 4);
				k2.rest = k.rest;
				k2.env = k.env;
				k2.slot = 1;
				k2.acc = array_new(				sizeof(blip__Value), 0, 0);
				blip__Value __arr_val_3 = m->val;
				array_push(&k2.acc, &__arr_val_3);
				machine__Machine__push(m, k2);
				m->env = k.env;
				machine__Machine__goto(m, (*(i32*)array_get(kids, 1)));
			}
		} else if (__match_tmp_0 == 4) {
			blip__Value __arr_val_4 = m->val;
			array_push(&k.acc, &__arr_val_4);
			Array kids = reader__Arena__kids(m->arena, k.rest);
			if (k.acc.len < kids.len) {
				blip__Kont k2 = k;
				k2.slot = k.acc.len;
				machine__Machine__push(m, k2);
				m->env = k.env;
				machine__Machine__goto(m, (*(i32*)array_get(kids, k.acc.len)));
			} else {
				__v_result __or_opt_5 = machine__Machine__apply_all(m, k.acc);
				if (!__or_opt_5.ok) {
					return (__v_result_bool){.ok = false, .err = __or_opt_5.err};
				}
			}
		} else if (__match_tmp_0 == 8) {
			Array kids = reader__Arena__kids(m->arena, k.rest);
			m->env = k.env;
			if (blip__Value__truthy(m->val)) {
				machine__Machine__goto(m, (*(i32*)array_get(kids, 2)));
			} else if (kids.len > 3) {
				machine__Machine__goto(m, (*(i32*)array_get(kids, 3)));
			} else {
				m->val = blip__nil_value();
				machine__Machine__ret(m);
			}
		} else if (__match_tmp_0 == 16) {
			Array kids = reader__Arena__kids(m->arena, k.rest);
			i64 next = k.slot + 1;
			if (next < kids.len) {
				blip__Kont k2 = k;
				k2.slot = next;
				if ((next + 1) < kids.len) {
					machine__Machine__push(m, k2);
				}
				m->env = k.env;
				machine__Machine__goto(m, (*(i32*)array_get(kids, next)));
			} else {
				machine__Machine__ret(m);
			}
		} else if (__match_tmp_0 == 32) {
			m->env = k.env;
			blip__EnvArena__define(&m->envs, m->env, k.name, m->val);
			m->val = blip__symbol(k.name);
			machine__Machine__ret(m);
		} else if (__match_tmp_0 == 64) {
			m->env = k.env;
			if (!blip__EnvArena__set(&m->envs, m->env, k.name, m->val)) {
				return (__v_result_bool){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_42_d129e9fc51bfae69, k.name), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			m->val = blip__nil_value();
			machine__Machine__ret(m);
		} else if (__match_tmp_0 == 128) {
			if (!blip__Value__truthy(m->val)) {
				machine__Machine__ret(m);
			} else {
				Array kids = reader__Arena__kids(m->arena, k.rest);
				if (k.slot < kids.len) {
					blip__Kont k2 = k;
					k2.slot = k.slot + 1;
					machine__Machine__push(m, k2);
					machine__Machine__goto(m, (*(i32*)array_get(kids, k.slot)));
				} else {
					machine__Machine__ret(m);
				}
			}
		} else if (__match_tmp_0 == 256) {
			if (blip__Value__truthy(m->val)) {
				machine__Machine__ret(m);
			} else {
				Array kids = reader__Arena__kids(m->arena, k.rest);
				if (k.slot < kids.len) {
					blip__Kont k2 = k;
					k2.slot = k.slot + 1;
					machine__Machine__push(m, k2);
					machine__Machine__goto(m, (*(i32*)array_get(kids, k.slot)));
				} else {
					machine__Machine__ret(m);
				}
			}
		} else if (__match_tmp_0 == 512) {
			blip__Value base = (*(blip__Value*)array_get(k.acc, 0));
			if (base.tag != 131072) {
				return (__v_result_bool){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[4]; __v3_internal_symbol_join_0[0] = k.name; __v3_internal_symbol_join_0[1] = _v3_lit_6_11fe221e2ab62bee; __v3_internal_symbol_join_0[2] = printer__write(base); __v3_internal_symbol_join_0[3] = _v3_lit_16_869ea3525e9dc698; string_plus_many(4, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			if (!blip__StructVal__has(blip__Value__as_struct(base), k.name)) {
				return (__v_result_bool){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[3]; __v3_internal_symbol_join_1[0] = (blip__Value__as_struct(base))->name; __v3_internal_symbol_join_1[1] = _v3_lit_14_14368fa9ebdd61d6; __v3_internal_symbol_join_1[2] = k.name; string_plus_many(3, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			blip__StructVal* next = blip__StructVal__with(blip__Value__as_struct(base), k.name, m->val);
			m->val = (blip__Value){.tag = 131072, .payload = (blip__Payload){._typ = 2048012492, ._object = next}};
			machine__Machine__ret(m);
		} else if (__match_tmp_0 == 2048) {
			Array kids = k.clauses;
			if (!machine__is_result_of(m->val, _v3_lit_2_9bce5400c6789a9d)) {
				machine__Machine__ret(m);
				return (__v_result_bool){.ok = true, .value = true};
			}
			__v_result_Array __or_opt_6 = prims__seq(m->val);
			Array __or_val_7 = {0};
			if (__or_opt_6.ok) {
				__or_val_7 = __or_opt_6.value;
			} else {
				IError err = __or_opt_6.err;
				machine__Machine__ret(m);
				return (__v_result_bool){.ok = true, .value = true};
			}
			Array parts = __or_val_7;
			i32 frame = blip__EnvArena__new_env(&m->envs, k.env);
			if (parts.len == 2) {
				blip__EnvArena__define(&m->envs, frame, machine__Machine__callee_name(m, (*(i32*)array_get(kids, 1))), (*(blip__Value*)array_get(parts, 1)));
			} else {
				i64 idx = 0;
				{
					i64 pi = 1;
					for (; pi < parts.len; pi++) {
						blip__EnvArena__define(&m->envs, frame, machine__Machine__use_name(m, idx), (*(blip__Value*)array_get(parts, pi)));
						idx++;
					}
				}
			}
			__v_result __or_opt_8 = machine__Machine__enter_clause_body(m, frame, machine__Machine__make_begin(m, array_slice(			kids, 			2, (			kids).len)));
			if (!__or_opt_8.ok) {
				return (__v_result_bool){.ok = false, .err = __or_opt_8.err};
			}
			return (__v_result_bool){.ok = true, .value = true};
		} else if (__match_tmp_0 == 4096) {
			__v_result __or_opt_9 = machine__Machine__try_clauses(m, k, m->val);
			if (!__or_opt_9.ok) {
				return (__v_result_bool){.ok = false, .err = __or_opt_9.err};
			}
			return (__v_result_bool){.ok = true, .value = true};
		} else if (__match_tmp_0 == 8192) {
			if (blip__Value__truthy(m->val)) {
				machine__Machine__goto(m, k.rest);
				return (__v_result_bool){.ok = true, .value = true};
			}
			blip__Kont next = k;
			next.slot = k.slot + 1;
			blip__Value subj = blip__nil_value();
			if (k.acc.len > 0) {
				subj = (*(blip__Value*)array_get(k.acc, 0));
			}
			m->env = k.env;
			__v_result __or_opt_10 = machine__Machine__try_clauses(m, next, subj);
			if (!__or_opt_10.ok) {
				return (__v_result_bool){.ok = false, .err = __or_opt_10.err};
			}
			return (__v_result_bool){.ok = true, .value = true};
		} else if (__match_tmp_0 == 32768) {
			if (k.slot == -1) {
				m->val = m->val2;
				machine__Machine__ret(m);
				return (__v_result_bool){.ok = true, .value = true};
			}
			if (k.slot == -3) {
				return machine__Machine__try_finish(m, k, true);
			}
			if (k.slot == -4) {
				return (__v_result_bool){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_8_d4bf6126de53209, blip__Value__as_string(m->val2)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			i64 next = k.slot + 1;
			if (next < k.clauses.len) {
				blip__Kont k2 = k;
				k2.slot = next;
				machine__Machine__push(m, k2);
				m->env = k.env;
				machine__Machine__goto(m, (*(i32*)array_get(k.clauses, next)));
				return (__v_result_bool){.ok = true, .value = true};
			}
			m->val2 = m->val;
			return machine__Machine__try_finish(m, k, true);
		} else if (__match_tmp_0 == 16384) {
			blip__Kont k2 = k;
			blip__Value __arr_val_11 = m->val;
			array_push(&k2.acc, &__arr_val_11);
			Array kids = reader__Arena__kids(m->arena, k.rest);
			i64 next = k.slot + 1;
			if (next < kids.len) {
				k2.slot = next;
				machine__Machine__push(m, k2);
				m->env = k.env;
				machine__Machine__goto(m, (*(i32*)array_get(kids, next)));
				return (__v_result_bool){.ok = true, .value = true};
			}
			__v_result_blip__Value __or_opt_12 = machine__Machine__build_collection(m, k.rest, k2.acc);
			blip__Value __or_val_13 = {0};
			if (__or_opt_12.ok) {
				__or_val_13 = __or_opt_12.value;
			} else {
				return (__v_result_bool){.ok = false, .err = __or_opt_12.err};
			}
			m->val = __or_val_13;
			machine__Machine__ret(m);
			return (__v_result_bool){.ok = true, .value = true};
		} else if (__match_tmp_0 == 1024) {
			if (blip__Value__truthy(m->val)) {
				m->val = blip__nil_value();
				machine__Machine__ret(m);
				return (__v_result_bool){.ok = true, .value = true};
			}
			return (__v_result_bool){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_18_c7a689d4563e2c68, machine__Machine__render_node(m, k.rest)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		} else {
			return (__v_result_bool){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_33_28fc7a98c0faab94, blip__KontTag__autostr(k.tag)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
	}
	return (__v_result_bool){.ok = true, .value = true};
	return (__v_result_bool){.ok = true};
}

/* V3CACHE_FN_END c15a4b35c6754d61 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 44a096ebfea5596c */
i32 machine__Machine__string_node(machine__Machine* m, string s) {
	return reader__Arena__str_leaf(m->arena, reader__DatumTag__str, s);
}

/* V3CACHE_FN_END 44a096ebfea5596c */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN e97b9b3e64fd9da8 */
i32 machine__Machine__sym_node(machine__Machine* m, string name) {
	return reader__Arena__str_leaf(m->arena, reader__DatumTag__sym, name);
}

/* V3CACHE_FN_END e97b9b3e64fd9da8 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 6523e28c9eaee2f7 */
i32 machine__Machine__transform_case(machine__Machine* m, Array kids) {
	i32 subject = (*(i32*)array_get(kids, 1));
	Array items = __new_array_noscan(0, 0, sizeof(i32));
	i64 ci = 2;
	while (ci < kids.len) {
		Array clause = reader__Arena__kids(m->arena, (*(i32*)array_get(kids, ci)));
		if (clause.len == 0) {
			ci++;
			continue;
		}
		reader__Datum* head = reader__Arena__node(m->arena, (*(i32*)array_get(clause, 0)));
		if ((head->tag == reader__DatumTag__sym) && string__eq(head->value, _v3_lit_4_6160ced97abcf88a)) {
			i32 body = machine__Machine__nil_node(m);
			if (clause.len > 1) {
				body = machine__Machine__make_begin(m, array_slice(				clause, 				1, (				clause).len));
			}
			Array __arr_lit_0 = __new_array_noscan(0, 2, sizeof(i32));
			i32 __arr_val_1 = machine__Machine__sym_node(m, _v3_lit_4_6160ced97abcf88a);
			array_push(&__arr_lit_0, &__arr_val_1);
			i32 __arr_val_2 = body;
			array_push(&__arr_lit_0, &__arr_val_2);
			i32 __arr_val_3 = machine__Machine__list_of(m, __arr_lit_0);
			array_push(&items, &__arr_val_3);
			break;
		}
		Array tests = reader__Arena__kids(m->arena, (*(i32*)array_get(clause, 0)));
		if (tests.len == 0) {
			ci++;
			continue;
		}
		i32 body = machine__Machine__nil_node(m);
		if (clause.len > 1) {
			body = machine__Machine__make_begin(m, array_slice(			clause, 			1, (			clause).len));
		}
		Array __arr_lit_4 = __new_array_noscan(0, 2, sizeof(i32));
		i32 __arr_val_5 = subject;
		array_push(&__arr_lit_4, &__arr_val_5);
		i32 __arr_val_6 = (*(i32*)array_get(tests, 0));
		array_push(&__arr_lit_4, &__arr_val_6);
		i32 combined = machine__Machine__call_node(m, machine__Machine__sym_node(m, _v3_lit_1_44bd66d473cd5bda), __arr_lit_4);
		i64 j = 1;
		while (j < tests.len) {
			Array __arr_lit_7 = __new_array_noscan(0, 2, sizeof(i32));
			i32 __arr_val_8 = combined;
			array_push(&__arr_lit_7, &__arr_val_8);
			Array __arr_lit_10 = __new_array_noscan(0, 2, sizeof(i32));
			i32 __arr_val_11 = subject;
			array_push(&__arr_lit_10, &__arr_val_11);
			i32 __arr_val_12 = (*(i32*)array_get(tests, j));
			array_push(&__arr_lit_10, &__arr_val_12);
			i32 __arr_val_9 = machine__Machine__call_node(m, machine__Machine__sym_node(m, _v3_lit_1_44bd66d473cd5bda), __arr_lit_10);
			array_push(&__arr_lit_7, &__arr_val_9);
			combined = machine__Machine__node_of(m, _v3_lit_2_9bce5b00c678a682, __arr_lit_7);
			j++;
		}
		i32 __arr_val_13 = machine__Machine__list_of(m, new_array_from_c_array(2, 2, sizeof(i32), (i32[2]){combined, body}));
		array_push(&items, &__arr_val_13);
		ci++;
	}
	return machine__Machine__cond_from(m, items);
}

/* V3CACHE_FN_END 6523e28c9eaee2f7 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 91e6159948fe2e87 */
i32 machine__Machine__transform_cond(machine__Machine* m, Array kids) {
	Array items = __new_array_noscan(0, 0, sizeof(i32));
	i64 ci = 1;
	while (ci < kids.len) {
		i32 __arr_val_0 = (*(i32*)array_get(kids, ci));
		array_push(&items, &__arr_val_0);
		ci++;
	}
	return machine__Machine__cond_from(m, items);
}

/* V3CACHE_FN_END 91e6159948fe2e87 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 43ed701a7978d91c */
__v_result_i32 machine__Machine__transform_dotimes(machine__Machine* m, Array kids) {
	if (kids.len < 3) {
		return (__v_result_i32){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_41_da173502be11c611, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	reader__Datum* loopvar = reader__Arena__node(m->arena, (*(i32*)array_get(kids, 1)));
	if (loopvar->tag != reader__DatumTag__sym) {
		return (__v_result_i32){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_49_e04cb13e2395bd5c, printer__write_datum(m->arena, (*(i32*)array_get(kids, 1)))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	Array __arr_lit_0 = __new_array_noscan(0, 2, sizeof(i32));
	i32 __arr_val_1 = (*(i32*)array_get(kids, 1));
	array_push(&__arr_lit_0, &__arr_val_1);
	i32 __arr_val_2 = (*(i32*)array_get(kids, 2));
	array_push(&__arr_lit_0, &__arr_val_2);
	i32 lt = machine__Machine__call_node(m, machine__Machine__sym_node(m, _v3_lit_1_44bd67d473cd5d8d), __arr_lit_0);
	Array items = __new_array_noscan(0, 0, sizeof(i32));
	i32 __arr_val_3 = (*(i32*)array_get(kids, 1));
	array_push(&items, &__arr_val_3);
	i32 __arr_val_4 = machine__Machine__int_node(m, 0);
	array_push(&items, &__arr_val_4);
	i32 __arr_val_5 = lt;
	array_push(&items, &__arr_val_5);
	i64 i = 3;
	while (i < kids.len) {
		i32 __arr_val_6 = (*(i32*)array_get(kids, i));
		array_push(&items, &__arr_val_6);
		i++;
	}
	return (__v_result_i32){.ok = true, .value = machine__Machine__node_of(m, _v3_lit_4_8fd1b32686ba76f1, items)};
	return (__v_result_i32){.ok = true};
}

/* V3CACHE_FN_END 43ed701a7978d91c */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 3138deb28e0d5d58 */
__v_result_i32 machine__Machine__transform_let(machine__Machine* m, Array kids) {
	if ((kids.len > 1) && ((reader__Arena__node(m->arena, (*(i32*)array_get(kids, 1))))->tag == reader__DatumTag__sym) && string__eq((reader__Arena__node(m->arena, (*(i32*)array_get(kids, 1))))->value, _v3_lit_6_82fb670e19860b15)) {
		return (machine__Machine__transform_let_assert(m, array_slice(		kids, 		1, (		kids).len)));
	}
	if ((kids.len > 2) && ((reader__Arena__node(m->arena, (*(i32*)array_get(kids, 1))))->tag == reader__DatumTag__sym)) {
		Array __arr_lit_0 = __new_array_noscan(0, 1, sizeof(i32));
		i32 __arr_val_1 = (*(i32*)array_get(kids, 1));
		array_push(&__arr_lit_0, &__arr_val_1);
		Array __arr_lit_2 = __new_array_noscan(0, 1, sizeof(i32));
		i32 __arr_val_3 = (*(i32*)array_get(kids, 2));
		array_push(&__arr_lit_2, &__arr_val_3);
		i32 pair = machine__Machine__list_of2(m, __arr_lit_0, __arr_lit_2);
		Array __arr_lit_4 = __new_array_noscan(0, 2, sizeof(i32));
		i32 __arr_val_5 = (*(i32*)array_get(kids, 0));
		array_push(&__arr_lit_4, &__arr_val_5);
		i32 __arr_val_6 = pair;
		array_push(&__arr_lit_4, &__arr_val_6);
		Array rest = __arr_lit_4;
		array__push_many(&rest, (array_slice(		kids, 		3, (		kids).len)).data, (array_slice(		kids, 		3, (		kids).len)).len);
		return machine__Machine__transform_let(m, rest);
	}
	Array binds = reader__Arena__kids(m->arena, (*(i32*)array_get(kids, 1)));
	if (binds.len == 0) {
		return (__v_result_i32){.ok = true, .value = machine__Machine__make_begin(m, array_slice(		kids, 		2, (		kids).len))};
	}
	Array params = __new_array_noscan(0, 0, sizeof(i32));
	Array args = __new_array_noscan(0, 0, sizeof(i32));
	Array destructures = array_new(	sizeof(machine__Destructure), 0, 0);
	i64 i = 0;
	while (i < binds.len) {
		if (machine__Machine__is_destructuring(m, (*(i32*)array_get(binds, i)))) {
			i32 tmp = machine__Machine__sym_node(m, machine__Machine__fresh(m, _v3_lit_3_51f91f514d780114));
			i32 __arr_val_7 = tmp;
			array_push(&params, &__arr_val_7);
			i32 __arr_val_8 = machine__Machine__binding_value(m, (*(i32*)array_get(binds, i)));
			array_push(&args, &__arr_val_8);
			__v_result_Array __or_opt_9 = machine__Machine__destructuring(m, machine__Machine__binding_pattern(m, (*(i32*)array_get(binds, i))), tmp);
			Array __or_val_10 = {0};
			if (__or_opt_9.ok) {
				__or_val_10 = __or_opt_9.value;
			} else {
				return (__v_result_i32){.ok = false, .err = __or_opt_9.err};
			}
			machine__Destructure __arr_val_11 = (machine__Destructure){.temp = tmp, .parts = __or_val_10};
			array_push(&destructures, &__arr_val_11);
		} else {
			i32 __arr_val_12 = machine__Machine__binding_name(m, (*(i32*)array_get(binds, i)));
			array_push(&params, &__arr_val_12);
			i32 __arr_val_13 = machine__Machine__binding_value(m, (*(i32*)array_get(binds, i)));
			array_push(&args, &__arr_val_13);
		}
		i++;
	}
	Array dgroups = array_new(	sizeof(machine__Group), 0, 0);
	{
		i64 __for_idx_14 = 0;
		for (; __for_idx_14 < destructures.len; __for_idx_14++) {
			machine__Destructure d = *(machine__Destructure*)(array_get(destructures, __for_idx_14));
			Array names = __new_array_noscan(0, 0, sizeof(i32));
			Array vals = __new_array_noscan(0, 0, sizeof(i32));
			{
				i64 __for_idx_15 = 0;
				for (; __for_idx_15 < d.parts.len; __for_idx_15++) {
					machine__Destructured p = *(machine__Destructured*)(array_get(d.parts, __for_idx_15));
					i32 __arr_val_16 = p.name;
					array_push(&names, &__arr_val_16);
					i32 __arr_val_17 = p.accessor;
					array_push(&vals, &__arr_val_17);
				}
			}
			machine__Group __arr_val_18 = (machine__Group){.params = names, .args = vals};
			array_push(&dgroups, &__arr_val_18);
		}
	}
	i32 inner = machine__Machine__nest_lets(m, dgroups, array_slice(	kids, 	2, (	kids).len));
	Array __arr_lit_19 = __new_array_noscan(0, 2, sizeof(i32));
	i32 __arr_val_20 = machine__Machine__list_of(m, params);
	array_push(&__arr_lit_19, &__arr_val_20);
	i32 __arr_val_21 = inner;
	array_push(&__arr_lit_19, &__arr_val_21);
	i32 lam = machine__Machine__node_of(m, _v3_lit_6_497dd6d1afd39ae4, __arr_lit_19);
	return (__v_result_i32){.ok = true, .value = machine__Machine__call_node(m, lam, args)};
	return (__v_result_i32){.ok = true};
}

/* V3CACHE_FN_END 3138deb28e0d5d58 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN d42b0f179bf30bff */
__v_result_i32 machine__Machine__transform_let_assert(machine__Machine* m, Array kids) {
	if (kids.len < 3) {
		return (__v_result_i32){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_38_4b983c01eebf672d, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	Array bindings = reader__Arena__kids(m->arena, (*(i32*)array_get(kids, 1)));
	if (bindings.len != 2) {
		return (__v_result_i32){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_61_88d7c75c655d88b5, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	Array __arr_lit_0 = __new_array_noscan(0, 1, sizeof(i32));
	i32 __arr_val_1 = machine__Machine__string_node(m, _v3_lit_55_2851f1c5138787eb);
	array_push(&__arr_lit_0, &__arr_val_1);
	i32 fail = machine__Machine__node_of(m, _v3_lit_5_7abebb90e73faeeb, __arr_lit_0);
	Array __arr_lit_2 = __new_array_noscan(0, 3, sizeof(i32));
	i32 __arr_val_3 = (*(i32*)array_get(bindings, 1));
	array_push(&__arr_lit_2, &__arr_val_3);
	Array __arr_lit_5 = __new_array_noscan(0, 2, sizeof(i32));
	i32 __arr_val_6 = (*(i32*)array_get(bindings, 0));
	array_push(&__arr_lit_5, &__arr_val_6);
	i32 __arr_val_7 = machine__Machine__make_begin(m, array_slice(	kids, 	2, (	kids).len));
	array_push(&__arr_lit_5, &__arr_val_7);
	i32 __arr_val_4 = machine__Machine__list_of(m, __arr_lit_5);
	array_push(&__arr_lit_2, &__arr_val_4);
	Array __arr_lit_9 = __new_array_noscan(0, 2, sizeof(i32));
	i32 __arr_val_10 = machine__Machine__sym_node(m, _v3_lit_1_44bd84d473cd8ed4);
	array_push(&__arr_lit_9, &__arr_val_10);
	i32 __arr_val_11 = fail;
	array_push(&__arr_lit_9, &__arr_val_11);
	i32 __arr_val_8 = machine__Machine__list_of(m, __arr_lit_9);
	array_push(&__arr_lit_2, &__arr_val_8);
	return (__v_result_i32){.ok = true, .value = machine__Machine__node_of(m, _v3_lit_5_1118636f8a4d8f74, __arr_lit_2)};
	return (__v_result_i32){.ok = true};
}

/* V3CACHE_FN_END d42b0f179bf30bff */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 3eef7378061f7d85 */
i32 machine__Machine__transform_let_star(machine__Machine* m, Array kids) {
	return machine__Machine__transform_let_star_at(m, kids, 0);
}

/* V3CACHE_FN_END 3eef7378061f7d85 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN e74db96a644e3c15 */
i32 machine__Machine__transform_let_star_at(machine__Machine* m, Array kids, i64 binds_at) {
	Array binds = reader__Arena__kids(m->arena, (*(i32*)array_get(kids, 1)));
	if (binds_at >= binds.len) {
		return machine__Machine__make_begin(m, array_slice(		kids, 		2, (		kids).len));
	}
	i32 inner = machine__Machine__transform_let_star_at(m, kids, binds_at + 1);
	Array one = __new_array_noscan(0, 0, sizeof(i32));
	i32 __arr_val_0 = (*(i32*)array_get(binds, binds_at));
	array_push(&one, &__arr_val_0);
	Array items = __new_array_noscan(0, 0, sizeof(i32));
	i32 __arr_val_1 = machine__Machine__sym_node(m, _v3_lit_3_51f91f514d780114);
	array_push(&items, &__arr_val_1);
	i32 __arr_val_2 = machine__Machine__list_of(m, one);
	array_push(&items, &__arr_val_2);
	i32 __arr_val_3 = inner;
	array_push(&items, &__arr_val_3);
	return machine__Machine__list_of(m, items);
}

/* V3CACHE_FN_END e74db96a644e3c15 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN dc36b9cd5b90e938 */
i32 machine__Machine__transform_letrec(machine__Machine* m, Array kids) {
	Array binds = reader__Arena__kids(m->arena, (*(i32*)array_get(kids, 1)));
	if (binds.len == 0) {
		return machine__Machine__make_begin(m, array_slice(		kids, 		2, (		kids).len));
	}
	i32 nilq = reader__Arena__open(m->arena, reader__DatumTag__quoted);
	Array __arr_lit_0 = __new_array_noscan(0, 1, sizeof(i32));
	i32 __arr_val_1 = machine__Machine__nil_node(m);
	array_push(&__arr_lit_0, &__arr_val_1);
	reader__Arena__finish(m->arena, nilq, __arr_lit_0);
	Array params = __new_array_noscan(0, 0, sizeof(i32));
	Array stmts = __new_array_noscan(0, 0, sizeof(i32));
	Array holes = __new_array_noscan(0, 0, sizeof(i32));
	i64 idx = 0;
	while (idx < binds.len) {
		i32 b = (*(i32*)array_get(binds, idx));
		i32 name = machine__Machine__binding_name(m, b);
		i32 __arr_val_2 = name;
		array_push(&params, &__arr_val_2);
		i32 __arr_val_3 = nilq;
		array_push(&holes, &__arr_val_3);
		Array __arr_lit_4 = __new_array_noscan(0, 2, sizeof(i32));
		i32 __arr_val_5 = name;
		array_push(&__arr_lit_4, &__arr_val_5);
		i32 __arr_val_6 = machine__Machine__binding_value(m, b);
		array_push(&__arr_lit_4, &__arr_val_6);
		i32 __arr_val_7 = machine__Machine__node_of(m, _v3_lit_4_669eb42cb843a8e0, __arr_lit_4);
		array_push(&stmts, &__arr_val_7);
		idx++;
	}
	i64 bi = 2;
	while (bi < kids.len) {
		i32 __arr_val_8 = (*(i32*)array_get(kids, bi));
		array_push(&stmts, &__arr_val_8);
		bi++;
	}
	Array __arr_lit_9 = __new_array_noscan(0, 2, sizeof(i32));
	i32 __arr_val_10 = machine__Machine__list_of(m, params);
	array_push(&__arr_lit_9, &__arr_val_10);
	i32 __arr_val_11 = machine__Machine__make_begin(m, stmts);
	array_push(&__arr_lit_9, &__arr_val_11);
	i32 lam = machine__Machine__node_of(m, _v3_lit_6_497dd6d1afd39ae4, __arr_lit_9);
	return machine__Machine__call_node(m, lam, holes);
}

/* V3CACHE_FN_END dc36b9cd5b90e938 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 498d61970c4aa311 */
i32 machine__Machine__transform_loop(machine__Machine* m, Array kids) {
	if (kids.len < 4) {
		return machine__Machine__make_begin(m, array_slice(		kids, 		2, (		kids).len));
	}
	i32 name = (*(i32*)array_get(kids, 1));
	i32 init = (*(i32*)array_get(kids, 2));
	i32 test = (*(i32*)array_get(kids, 3));
	i32 fnname = machine__Machine__sym_node(m, machine__Machine__fresh(m, _v3_lit_4_8fd1b32686ba76f1));
	Array stmts = __new_array_noscan(0, 0, sizeof(i32));
	i64 bi = 4;
	while (bi < kids.len) {
		i32 __arr_val_0 = (*(i32*)array_get(kids, bi));
		array_push(&stmts, &__arr_val_0);
		bi++;
	}
	Array __arr_lit_1 = __new_array_noscan(0, 2, sizeof(i32));
	i32 __arr_val_2 = name;
	array_push(&__arr_lit_1, &__arr_val_2);
	Array __arr_lit_4 = __new_array_noscan(0, 2, sizeof(i32));
	i32 __arr_val_5 = name;
	array_push(&__arr_lit_4, &__arr_val_5);
	i32 __arr_val_6 = machine__Machine__int_node(m, 1);
	array_push(&__arr_lit_4, &__arr_val_6);
	i32 __arr_val_3 = machine__Machine__call_node(m, machine__Machine__sym_node(m, _v3_lit_1_44bd50d473cd3678), __arr_lit_4);
	array_push(&__arr_lit_1, &__arr_val_3);
	i32 __arr_val_7 = machine__Machine__node_of(m, _v3_lit_4_669eb42cb843a8e0, __arr_lit_1);
	array_push(&stmts, &__arr_val_7);
	i32 __arr_val_8 = machine__Machine__call_node(m, fnname, new_array_from_c_array_noscan(1, 1, sizeof(i32), (i32[1]){name}));
	array_push(&stmts, &__arr_val_8);
	i32 body = machine__Machine__make_begin(m, stmts);
	Array __arr_lit_9 = __new_array_noscan(0, 3, sizeof(i32));
	i32 __arr_val_10 = test;
	array_push(&__arr_lit_9, &__arr_val_10);
	i32 __arr_val_11 = body;
	array_push(&__arr_lit_9, &__arr_val_11);
	i32 __arr_val_12 = machine__Machine__nil_node(m);
	array_push(&__arr_lit_9, &__arr_val_12);
	i32 inner = machine__Machine__node_of(m, _v3_lit_2_9bc77700c672b768, __arr_lit_9);
	Array __arr_lit_13 = __new_array_noscan(0, 2, sizeof(i32));
	i32 __arr_val_14 = machine__Machine__list_of(m, new_array_from_c_array_noscan(1, 1, sizeof(i32), (i32[1]){name}));
	array_push(&__arr_lit_13, &__arr_val_14);
	i32 __arr_val_15 = inner;
	array_push(&__arr_lit_13, &__arr_val_15);
	i32 lam = machine__Machine__node_of(m, _v3_lit_6_497dd6d1afd39ae4, __arr_lit_13);
	i32 bindform = machine__Machine__list_of(m, new_array_from_c_array(2, 2, sizeof(i32), (i32[2]){fnname, lam}));
	Array __arr_lit_16 = __new_array_noscan(0, 2, sizeof(i32));
	i32 __arr_val_17 = machine__Machine__list_of(m, new_array_from_c_array_noscan(1, 1, sizeof(i32), (i32[1]){bindform}));
	array_push(&__arr_lit_16, &__arr_val_17);
	i32 __arr_val_18 = machine__Machine__call_node(m, fnname, new_array_from_c_array_noscan(1, 1, sizeof(i32), (i32[1]){init}));
	array_push(&__arr_lit_16, &__arr_val_18);
	i32 letrec = machine__Machine__node_of(m, _v3_lit_6_3ab33e7aab478c62, __arr_lit_16);
	return letrec;
}

/* V3CACHE_FN_END 498d61970c4aa311 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 44319f69cb1bc4b5 */
i32 machine__Machine__transform_match_star(machine__Machine* m, Array kids) {
	Array subjects = reader__Arena__kids(m->arena, (*(i32*)array_get(kids, 1)));
	Array stmts = __new_array_noscan(0, 0, sizeof(i32));
	i32 inner = machine__Machine__call_node(m, machine__Machine__sym_node(m, _v3_lit_4_a1c80526911571eb), subjects);
	Array clauses = __new_array_noscan(0, 0, sizeof(i32));
	i64 c = 2;
	while (c < kids.len) {
		Array ckids = reader__Arena__kids(m->arena, (*(i32*)array_get(kids, c)));
		if (ckids.len == 0) {
			c++;
			continue;
		}
		Array body = __new_array_noscan(0, 0, sizeof(i32));
		i64 bi = 1;
		while (bi < ckids.len) {
			i32 __arr_val_0 = (*(i32*)array_get(ckids, bi));
			array_push(&body, &__arr_val_0);
			bi++;
		}
		Array pat = __new_array_noscan(0, 0, sizeof(i32));
		i32 __arr_val_1 = machine__Machine__sym_node(m, _v3_lit_4_a1c80526911571eb);
		array_push(&pat, &__arr_val_1);
		i64 p = 0;
		while (p < ckids.len) {
			i32 __arr_val_2 = (*(i32*)array_get(ckids, p));
			array_push(&pat, &__arr_val_2);
			p++;
		}
		Array __arr_lit_3 = __new_array_noscan(0, 2, sizeof(i32));
		i32 __arr_val_4 = machine__Machine__list_of(m, pat);
		array_push(&__arr_lit_3, &__arr_val_4);
		i32 __arr_val_5 = machine__Machine__make_begin(m, body);
		array_push(&__arr_lit_3, &__arr_val_5);
		i32 __arr_val_6 = machine__Machine__list_of(m, __arr_lit_3);
		array_push(&clauses, &__arr_val_6);
		c++;
	}
	Array items = __new_array_noscan(0, 0, sizeof(i32));
	i32 __arr_val_7 = machine__Machine__sym_node(m, _v3_lit_5_1118636f8a4d8f74);
	array_push(&items, &__arr_val_7);
	i32 __arr_val_8 = inner;
	array_push(&items, &__arr_val_8);
	i64 ci = 0;
	while (ci < clauses.len) {
		i32 __arr_val_9 = (*(i32*)array_get(clauses, ci));
		array_push(&items, &__arr_val_9);
		ci++;
	}
	return machine__Machine__list_of(m, items);
}

/* V3CACHE_FN_END 44319f69cb1bc4b5 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 6fa86e166de8f9c2 */
__v_result machine__Machine__try_caught(machine__Machine* m, blip__Kont k) {
	blip__Kont cf = machine__Machine__kont(m, 4096);
	cf.env = k.env;
	cf.slot = 0;
	Array __arr_lit_0 = __new_array_noscan(0, 1, sizeof(i32));
	i32 __arr_val_1 = k.expr;
	array_push(&__arr_lit_0, &__arr_val_1);
	cf.clauses = __arr_lit_0;
	blip__Kont k2 = k;
	k2.slot = -3;
	machine__Machine__push(m, k2);
	machine__Machine__push(m, cf);
	machine__Machine__ret(m);
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END 6fa86e166de8f9c2 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 4a78bd9a0baa8e96 */
__v_result machine__Machine__try_clauses(machine__Machine* m, blip__Kont k, blip__Value subject) {
	i64 i = k.slot;
	while (i < k.clauses.len) {
		i32 clause = (*(i32*)array_get(k.clauses, i));
		Array ckids = reader__Arena__kids(m->arena, clause);
		if (ckids.len == 0) {
			i++;
			continue;
		}
		i64 pi = 0;
		i32 guard = (i32)(-1);
		if (((reader__Arena__node(m->arena, (*(i32*)array_get(ckids, 0))))->tag == reader__DatumTag__kw) && string__eq((reader__Arena__node(m->arena, (*(i32*)array_get(ckids, 0))))->value, _v3_lit_6_6f0c4e5b2a1c16de)) {
			if (ckids.len < 2) {
				return (__v_result){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_26_26b2f1e3ea09f6f7, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			guard = (*(i32*)array_get(ckids, 1));
			pi = 2;
		}
		i32 pattern = (i32)(-1);
		if (pi < ckids.len) {
			pattern = (*(i32*)array_get(ckids, pi));
			pi++;
		}
		bool is_else = false;
		if (pattern != (i32)(-1)) {
			reader__Datum* pd = reader__Arena__node(m->arena, pattern);
			is_else = (pd->tag == reader__DatumTag__sym) && string__eq(pd->value, _v3_lit_4_6160ced97abcf88a);
		}
		machine__Matched res = (machine__Matched){.ok = true, .binds = array_new(sizeof(machine__Bind), 0, 0)};
		if (!is_else) {
			__v_result_machine__Matched __or_opt_0 = machine__Machine__match_pattern(m, pattern, subject);
			machine__Matched __or_val_1 = {0};
			if (__or_opt_0.ok) {
				__or_val_1 = __or_opt_0.value;
			} else {
				return (__v_result){.ok = false, .err = __or_opt_0.err};
			}
			res = __or_val_1;
		}
		if (res.ok) {
			Array body = __new_array_noscan(0, 0, sizeof(i32));
			i64 b = pi;
			while (b < ckids.len) {
				i32 __arr_val_2 = (*(i32*)array_get(ckids, b));
				array_push(&body, &__arr_val_2);
				b++;
			}
			i32 frame = blip__EnvArena__new_env(&m->envs, k.env);
			i64 n = 0;
			while (n < res.binds.len) {
				blip__EnvArena__define(&m->envs, frame, ((*(machine__Bind*)array_get(res.binds, n))).name, ((*(machine__Bind*)array_get(res.binds, n))).val);
				n++;
			}
			if (guard != (i32)(-1)) {
				blip__Kont gf = machine__Machine__kont(m, 8192);
				gf.env = k.env;
				gf.slot = i;
				gf.clauses = k.clauses;
				gf.rest = machine__Machine__make_begin(m, body);
				gf.acc = array_new(				sizeof(blip__Value), 0, 0);
				blip__Value __arr_val_3 = subject;
				array_push(&gf.acc, &__arr_val_3);
				m->env = frame;
				machine__Machine__push(m, gf);
				machine__Machine__goto(m, guard);
				return (__v_result){.ok = true};
			}
			return machine__Machine__enter_clause_body(m, frame, machine__Machine__make_begin(m, body));
		}
		i++;
	}
	m->env = k.env;
	m->val = blip__nil_value();
	machine__Machine__ret(m);
	return (__v_result){.ok = true};
}

/* V3CACHE_FN_END 4a78bd9a0baa8e96 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 5a88addaa95f2c43 */
__v_result_bool machine__Machine__try_finish(machine__Machine* m, blip__Kont k, bool run_finally) {
	if (run_finally && (k.rest != blip__no_node)) {
		blip__Kont k2 = k;
		k2.slot = -1;
		machine__Machine__push(m, k2);
		m->env = k.env;
		machine__Machine__goto(m, k.rest);
		return (__v_result_bool){.ok = true, .value = true};
	}
	m->val = m->val2;
	machine__Machine__ret(m);
	return (__v_result_bool){.ok = true, .value = true};
	return (__v_result_bool){.ok = true};
}

/* V3CACHE_FN_END 5a88addaa95f2c43 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 13e3ada53bbcc8e1 */
string machine__Machine__use_name(machine__Machine* m, i64 i) {
	return string__plus(_v3_lit_1_44bd99d473cdb283, int__str((i + 1)));
}

/* V3CACHE_FN_END 13e3ada53bbcc8e1 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN f9a76f0d0c37cf1f */
string machine__Machine__where(machine__Machine* m) {
	if (string__eq(m->source, _v3_lit_0_14650fb0739d0383)) {
		return _v3_lit_4_43492fafcd383414;
	}
	return m->source;
}

/* V3CACHE_FN_END f9a76f0d0c37cf1f */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 495d6ed81064fdd */
blip__Value machine__datum_literal(reader__Datum* d) {
	{
		reader__DatumTag __match_tmp_0 = d->tag;
		if (__match_tmp_0 == reader__DatumTag__nil) {
			return blip__nil_value();
		} else if (__match_tmp_0 == reader__DatumTag__bool) {
			return blip__boolean(d->i != 0);
		} else if (__match_tmp_0 == reader__DatumTag__v_int) {
			return blip__integer(d->i);
		} else if (__match_tmp_0 == reader__DatumTag__v_float) {
			return blip__float(d->f);
		} else if (__match_tmp_0 == reader__DatumTag__v_char) {
			return blip__rune((u32)(d->i));
		} else if (__match_tmp_0 == reader__DatumTag__str) {
			return blip__string(d->value);
		} else if (__match_tmp_0 == reader__DatumTag__sym) {
			return blip__symbol(d->value);
		} else if (__match_tmp_0 == reader__DatumTag__kw) {
			return blip__keyword(d->value);
		} else {
			return blip__nil_value();
		}
	}
}

/* V3CACHE_FN_END 495d6ed81064fdd */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN a4a39b1c1f357c48 */
string machine__format_args(Array args) {
	if (args.len == 0) {
		return _v3_lit_0_14650fb0739d0383;
	}
	string fm = blip__Value__as_string((*(blip__Value*)array_get(args, 0)));
	Array out = __new_array_noscan(0, 0, sizeof(u8));
	i64 i = 0;
	i64 next = 1;
	string spare = _v3_lit_0_14650fb0739d0383;
	while (i < fm.len) {
		u8 c = string__at(fm, i);
		if ((c == (u8)(126)) && ((i + 1) < fm.len)) {
			u8 which = string__at(fm, i + 1);
			if ((which == (u8)(97)) || (which == (u8)(115))) {
				if (next < args.len) {
					spare = machine__show_value((*(blip__Value*)array_get(args, next)), which == (u8)(115));
					next++;
				}
				Array __push_many_0 = string__bytes(spare);
				array__push_many(&out, __push_many_0.data, __push_many_0.len);
				i += 2;
				continue;
			}
		}
		u8 __arr_val_1 = c;
		array_push(&out, &__arr_val_1);
		i++;
	}
	return Array_u8__bytestr(out);
}

/* V3CACHE_FN_END a4a39b1c1f357c48 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 1601e4d23e315245 */
blip__Value machine__inner_value(blip__Value v) {
	if (v.tag != 256) {
		return blip__nil_value();
	}
	return (blip__Value__as_pair((blip__Value__as_pair(v))->cdr))->car;
}

/* V3CACHE_FN_END 1601e4d23e315245 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 6980b604239316a3 */
bool machine__is_result_of(blip__Value v, string kind) {
	if (v.tag != 256) {
		return false;
	}
	blip__Value head = (blip__Value__as_pair(v))->car;
	if (head.tag != 64) {
		return false;
	}
	return string__eq(blip__Value__as_string(head), kind);
}

/* V3CACHE_FN_END 6980b604239316a3 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 3877a967e62c8df5 */
string machine__kind_of(blip__Value v) {
	if (v.tag == 256) {
		blip__Value head = (blip__Value__as_pair(v))->car;
		if (head.tag == 64) {
			return blip__Value__as_string(head);
		}
	}
	return _v3_lit_12_a29f3038d3233718;
}

/* V3CACHE_FN_END 3877a967e62c8df5 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN e6b844b6770914c8 */
bool machine__literal_eq(reader__Datum* d, blip__Value v) {
	return prims__value_eq(machine__datum_literal(d), v);
}

/* V3CACHE_FN_END e6b844b6770914c8 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN b3d7979d03866de0 */
bool machine__machine_builtin(string name) {
	return string__eq(name, _v3_lit_5_f1969731237b5c95) || string__eq(name, _v3_lit_5_7abebb90e73faeeb) || string__eq(name, _v3_lit_5_d42707209fbb037d) || string__eq(name, _v3_lit_6_2ed40dcc7fa0e8ac) || string__eq(name, _v3_lit_6_95dd72fbb0a05d3e) || string__eq(name, _v3_lit_5_40dc52cfa0615582) || string__eq(name, _v3_lit_7_416d70fd06c69f3) || string__eq(name, _v3_lit_4_f11acfd93b3db69a) || string__eq(name, _v3_lit_9_cae270c2d09c1b24) || string__eq(name, _v3_lit_3_46d7e451467792f3) || string__eq(name, _v3_lit_6_cd094ce5950ab6d) || string__eq(name, _v3_lit_6_126715d46b46abf8) || string__eq(name, _v3_lit_4_90ee9eeb4064e302) || string__eq(name, _v3_lit_4_2ca5b0d314618bb0) || string__eq(name, _v3_lit_6_31de9ec2d2adf2e5) || string__eq(name, _v3_lit_8_99cfadc3c25d49ec) || string__eq(name, _v3_lit_10_f8e88b4c9f2cc737) || string__eq(name, _v3_lit_4_e059a2ba657c62a4) || string__eq(name, _v3_lit_6_4179c85f24cdc55b) || string__eq(name, _v3_lit_7_3e2e49b6ad015f3b) || string__eq(name, _v3_lit_2_9bce5400c6789a9d) || string__eq(name, _v3_lit_3_2249c511fb8ec16) || string__eq(name, _v3_lit_3_38335f513eeec146) || string__eq(name, _v3_lit_4_5d21c6d8e73949ab) || string__eq(name, _v3_lit_8_ab91311039da47a1) || string__eq(name, _v3_lit_9_d6b952147854006e) || string__eq(name, _v3_lit_10_3611d2ccdddbcd03) || string__eq(name, _v3_lit_9_fc7de50eaad59b9e) || string__eq(name, _v3_lit_10_bd4095b378303a32) || string__eq(name, _v3_lit_11_66673f96f53ffd17) || string__eq(name, _v3_lit_15_8ac87f40b4d713dc) || string__eq(name, _v3_lit_14_f795d06ca952a863) || string__eq(name, _v3_lit_11_726746d68b4e7079);
}

/* V3CACHE_FN_END b3d7979d03866de0 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 5c56aa471635937b */
machine__Machine* machine__new_machine(reader__Arena* a) {
	host__ConsoleHost* __iface_src_0 = (host__ConsoleHost*)memdup(&(host__ConsoleHost){E_STRUCT}, sizeof(host__ConsoleHost));
	return machine__new_machine_with(a, (host__Host){._typ = 84543226, ._object = __iface_src_0});
}

/* V3CACHE_FN_END 5c56aa471635937b */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 40b452af165005b4 */
machine__Machine* machine__new_machine_with(reader__Arena* a, host__Host h) {
	machine__Machine* m = (machine__Machine*)memdup(&(machine__Machine){.arena = a, .kstack = array_new(	sizeof(blip__Kont), 0, 0), .envs = (blip__EnvArena){.frames = array_new(sizeof(blip__Env), 0, 0)}, .globals = blip__no_env, .env = blip__no_env, .prims = prims__table(), .host = h, .ctl = 1, .max_steps = 100000000, .max_kont = 4000000, .val = (blip__Value){.tag = 0}, .val2 = (blip__Value){.tag = 0}, .out = array_new(sizeof(string), 0, 0), .source = _v3_lit_0_14650fb0739d0383, .src_text = _v3_lit_0_14650fb0739d0383, .loaded = array_new(sizeof(string), 0, 0), .macros = array_new(sizeof(string), 0, 0)}, sizeof(machine__Machine));
	m->globals = blip__EnvArena__new_env(&m->envs, blip__no_env);
	m->env = m->globals;
	return m;
}

/* V3CACHE_FN_END 40b452af165005b4 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 34ee0927ae4898b3 */
machine__Machine* machine__new_standalone(host__Host h) {
	reader__Arena* a = (reader__Arena*)memdup(&(reader__Arena){.nodes = array_new(	sizeof(reader__Datum), 0, 0), .children = __new_array_noscan(0, 0, sizeof(i32))}, sizeof(reader__Arena));
	return machine__new_machine_with(a, h);
}

/* V3CACHE_FN_END 34ee0927ae4898b3 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 8fb99ccda8959eb0 */
bool machine__pattern_predicate(string name) {
	return string__eq(name, _v3_lit_1_44bd67d473cd5d8d) || string__eq(name, _v3_lit_1_44bd65d473cd5a27) || string__eq(name, _v3_lit_2_9b351e00c5f63210) || string__eq(name, _v3_lit_2_9b2e2200c5f01a2e) || string__eq(name, _v3_lit_1_44bd66d473cd5bda) || string__eq(name, _v3_lit_4_8e826a14dc18becb) || string__eq(name, _v3_lit_3_22ee9511fc1c372) || string__eq(name, _v3_lit_5_ceff5b955e05cd1e) || string__eq(name, _v3_lit_5_9960f1b44cf39360) || string__eq(name, _v3_lit_4_ec87890da6744229) || string__eq(name, _v3_lit_9_4fa1b00d71d70d01) || string__eq(name, _v3_lit_9_703f5f434a1181a9) || string__eq(name, _v3_lit_5_fc52948887706b3c) || string__eq(name, _v3_lit_5_674f3934a3e137f4) || string__eq(name, _v3_lit_7_cd118c208a458ced) || string__eq(name, _v3_lit_6_1138efb0bf19762e) || string__eq(name, _v3_lit_6_b83d802db439c705) || string__eq(name, _v3_lit_7_3200f030b0cdbd5b) || string__eq(name, _v3_lit_7_e600cf5bbfb5be64) || string__eq(name, _v3_lit_8_26c547965b8b7753) || string__eq(name, _v3_lit_8_3c2942aa0a336e44) || string__eq(name, _v3_lit_6_3dfb6893450ba188) || string__eq(name, _v3_lit_7_1917d0780d948be7) || string__eq(name, _v3_lit_8_f9a8cb4c8fdf71a) || string__eq(name, _v3_lit_5_5c8f2ec760f92884) || string__eq(name, _v3_lit_4_c1506214f88ae147) || string__eq(name, _v3_lit_5_ccae00b90da4706b) || string__eq(name, _v3_lit_9_21a00140dab37ed4);
}

/* V3CACHE_FN_END 8fb99ccda8959eb0 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN 2e0ab086638fdad5 */
bool machine__pipe_tap(string name) {
	return string__eq(name, _v3_lit_4_f11acfd93b3db69a) || string__eq(name, _v3_lit_3_96796b51740b5524) || string__eq(name, _v3_lit_5_6decc255050b9c6a);
}

/* V3CACHE_FN_END 2e0ab086638fdad5 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN ec5a8d11c000f9f5 */
string machine__plural(i64 n) {
	if (n == 1) {
		return _v3_lit_0_14650fb0739d0383;
	} else {
		return _v3_lit_1_44bd98d473cdb0d0;
	}
}

/* V3CACHE_FN_END ec5a8d11c000f9f5 */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN ff41ded3e2b6d52e */
string machine__show_value(blip__Value a, bool quoted) {
	if ((a.tag == 32) && !quoted) {
		return blip__Value__as_string(a);
	}
	return printer__write(a);
}

/* V3CACHE_FN_END ff41ded3e2b6d52e */
/* V3CACHE_MODULE machine */
/* V3CACHE_FN_BEGIN ed12ff682031ed65 */
bool machine__special_form(string name) {
	return string__eq(name, _v3_lit_5_e49fbd9ccfe8a8c5) || string__eq(name, _v3_lit_2_9bc77700c672b768) || string__eq(name, _v3_lit_6_db03959661e15c48) || string__eq(name, _v3_lit_4_669eb42cb843a8e0) || string__eq(name, _v3_lit_6_497dd6d1afd39ae4) || string__eq(name, _v3_lit_2_9bb67100c6643a03) || string__eq(name, _v3_lit_5_178c76c75eded44) || string__eq(name, _v3_lit_3_51f91f514d780114) || string__eq(name, _v3_lit_4_c2517526a2ea1c5a) || string__eq(name, _v3_lit_6_3ab33e7aab478c62) || string__eq(name, _v3_lit_3_e1761a510dc506d4) || string__eq(name, _v3_lit_2_9bce5b00c678a682) || string__eq(name, _v3_lit_4_ca78e650452716cf) || string__eq(name, _v3_lit_6_7969d416ee3eef9d) || string__eq(name, _v3_lit_4_e6cd82a8beffa2b7) || string__eq(name, _v3_lit_4_6bb733a878c4261f) || string__eq(name, _v3_lit_4_8fd1b32686ba76f1) || string__eq(name, _v3_lit_7_7cc31e91bc6bb38c) || string__eq(name, _v3_lit_3_8ee02351700ae052) || string__eq(name, _v3_lit_5_1118636f8a4d8f74) || string__eq(name, _v3_lit_6_5a00548801ca9cba) || string__eq(name, _v3_lit_6_dc2ba458304519da) || string__eq(name, _v3_lit_6_14da8f5ec8315a0a) || string__eq(name, _v3_lit_10_999ae46be131259f) || string__eq(name, _v3_lit_7_1e67ecb0563e6794) || string__eq(name, _v3_lit_7_83d47bb3d9452c4e) || string__eq(name, _v3_lit_10_88fb1f45907c858d) || string__eq(name, _v3_lit_2_9bbd5000c66a209e) || string__eq(name, _v3_lit_4_6018685605485a) || string__eq(name, _v3_lit_6_82fb670e19860b15) || string__eq(name, _v3_lit_2_9afb1f00c5c4bb7c) || string__eq(name, _v3_lit_3_1d70f0500d4a3126) || string__eq(name, _v3_lit_2_9c0ea100c6af0169) || string__eq(name, _v3_lit_4_511736baa561c828) || string__eq(name, _v3_lit_6_f9ae4a62895365b0) || string__eq(name, _v3_lit_3_cadb9512638cd72) || string__eq(name, _v3_lit_8_b71566fdd941000e) || string__eq(name, _v3_lit_3_964d645173e60e54) || string__eq(name, _v3_lit_5_80da626fc95d20a3) || string__eq(name, _v3_lit_6_5035d5f3293e5816);
}

/* V3CACHE_FN_END ed12ff682031ed65 */
/* V3CACHE_MODULE main */
/* V3CACHE_FN_BEGIN 765a149e92e84459 */
int wmain(int argc, wchar_t** argv) {
	g_main_argc = argc;
	g_main_argv = argv;
	if (getenv("VEXE") == NULL || getenv("VEXE")[0] == 0) {
		const char* v3_vexe = "C:\\Users\\xman\\v\\v.exe";
		if (v3_vexe[0] != 0) {
#ifdef _WIN32
			_putenv_s("VEXE", v3_vexe);
#else
			setenv("VEXE", v3_vexe, 1);
#endif
		}
	}
	_vinit();
	main__Suite s = (main__Suite){0};
	Suite__check_example(&s, _v3_lit_9_558d47ffe2d92c7a, _v3_lit_22_e9e95c16b32022c7, _v3_lit_9_d9e8bcd8436389a2);
	Suite__check_example(&s, _v3_lit_7_75dc7a5b38b5226a, _v3_lit_20_a4b2332d308e0a8b, _v3_lit_7_2154afe82494e5a3);
	Suite__check_example(&s, _v3_lit_8_df3d8914fbfdb3f4, _v3_lit_21_5111a6e7d2346929, _v3_lit_8_9861f7f3fb131506);
	Suite__check_example(&s, _v3_lit_9_f8dd2fff34037007, _v3_lit_22_cd3666ff372783de, _v3_lit_9_1bca237c921ab45a);
	Suite__check_example(&s, _v3_lit_12_b366167f60875f44, _v3_lit_25_d97014f86e8d8d59, _v3_lit_12_9517155e21b85148);
	Suite__check_example(&s, _v3_lit_10_d691db9c711ca9e3, _v3_lit_23_f5eada8bb525a2f6, _v3_lit_10_3f6d726fb6f18026);
	if (s.fails > 0) {
		println(string__plus(int__str(s.fails), _v3_lit_11_d00d55f7f09898a7));
		return 0;
	}
	println(_v3_lit_27_681a01cfa2cc9426);
	return 0;
}

/* V3CACHE_FN_END 765a149e92e84459 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 26f003fefba9f272 */
u8* malloc_noscan(ptrdiff_t n) {
}

/* V3CACHE_FN_END 26f003fefba9f272 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 969b6a7af9304990 */
void map__clear(map* m) {
}

/* V3CACHE_FN_END 969b6a7af9304990 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 98b0a7ce3408148c */
map map__clone(map* m) {
}

/* V3CACHE_FN_END 98b0a7ce3408148c */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 3b75756a58f28462 */
void map__delete(map* m, void* key) {
}

/* V3CACHE_FN_END 3b75756a58f28462 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 3fedcb1759b34bfb */
bool map__exists(map* m, void* key) {
}

/* V3CACHE_FN_END 3fedcb1759b34bfb */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 3f94ae5f1aeaef99 */
void map__free(map* m) {
}

/* V3CACHE_FN_END 3f94ae5f1aeaef99 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 58f8fff33b97b12f */
void* map__get(map* m, void* key, void* zero) {
}

/* V3CACHE_FN_END 58f8fff33b97b12f */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN b977bcd3d4f94174 */
void* map__get_and_set(map* m, void* key, void* zero) {
}

/* V3CACHE_FN_END b977bcd3d4f94174 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN dd00462f38bd459a */
void* map__get_check(map* m, void* key) {
}

/* V3CACHE_FN_END dd00462f38bd459a */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN e21abe33d65a4f27 */
array map__keys(map* m) {
}

/* V3CACHE_FN_END e21abe33d65a4f27 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN e85216c1b64ad27 */
void map__reserve(map* m, u32 n) {
}

/* V3CACHE_FN_END e85216c1b64ad27 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 67bf6f8dce4953db */
void map__set(map* m, void* key, void* value) {
}

/* V3CACHE_FN_END 67bf6f8dce4953db */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN bc907480a8d9b061 */
array map__values(map* m) {
}

/* V3CACHE_FN_END bc907480a8d9b061 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 3dd5059d2330a43d */
inline void map_clone_int_1(void* dest, void* pkey) {
}

/* V3CACHE_FN_END 3dd5059d2330a43d */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 42574c02cc4535d7 */
inline void map_clone_int_16(void* dest, void* pkey) {
}

/* V3CACHE_FN_END 42574c02cc4535d7 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 3ddf059d2338f8c2 */
inline void map_clone_int_2(void* dest, void* pkey) {
}

/* V3CACHE_FN_END 3ddf059d2338f8c2 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 3de7059d2340ca70 */
inline void map_clone_int_4(void* dest, void* pkey) {
}

/* V3CACHE_FN_END 3de7059d2340ca70 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 3df3059d2349a1cc */
inline void map_clone_int_8(void* dest, void* pkey) {
}

/* V3CACHE_FN_END 3df3059d2349a1cc */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN a6ae0984cdf9e563 */
inline void map_clone_string(void* dest, void* pkey) {
}

/* V3CACHE_FN_END a6ae0984cdf9e563 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 8de92b3892880de2 */
inline bool map_eq_int_1(void* a, void* b) {
}

/* V3CACHE_FN_END 8de92b3892880de2 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN aa878e20fc910816 */
inline bool map_eq_int_16(void* a, void* b) {
}

/* V3CACHE_FN_END aa878e20fc910816 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 8ddf2b38927fb95d */
inline bool map_eq_int_2(void* a, void* b) {
}

/* V3CACHE_FN_END 8ddf2b38927fb95d */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 8df32b3892906267 */
inline bool map_eq_int_4(void* a, void* b) {
}

/* V3CACHE_FN_END 8df32b3892906267 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 8e012b38929c9fc3 */
inline bool map_eq_int_8(void* a, void* b) {
}

/* V3CACHE_FN_END 8e012b38929c9fc3 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 15c1646a3ca2586a */
inline bool map_eq_string(void* a, void* b) {
}

/* V3CACHE_FN_END 15c1646a3ca2586a */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 90dc093dc66f0196 */
inline void map_free_nop(void* _0) {
}

/* V3CACHE_FN_END 90dc093dc66f0196 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 4b5fdcb711dd212 */
inline void map_free_string(void* pkey) {
}

/* V3CACHE_FN_END 4b5fdcb711dd212 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN ba0f4eef1ad523dc */
u64 map_hash_int_1(void* pkey) {
}

/* V3CACHE_FN_END ba0f4eef1ad523dc */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN fc6c434a978d7448 */
u64 map_hash_int_16(void* pkey) {
}

/* V3CACHE_FN_END fc6c434a978d7448 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN ba114eef1ad5a6b3 */
u64 map_hash_int_2(void* pkey) {
}

/* V3CACHE_FN_END ba114eef1ad5a6b3 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN ba194eef1add7861 */
u64 map_hash_int_4(void* pkey) {
}

/* V3CACHE_FN_END ba194eef1add7861 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN b9f14eef1abc264d */
u64 map_hash_int_8(void* pkey) {
}

/* V3CACHE_FN_END b9f14eef1abc264d */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN bc323ec258ab6008 */
u64 map_hash_string(void* pkey) {
}

/* V3CACHE_FN_END bc323ec258ab6008 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 89ffbbde3ceb5d05 */
bool map_map_eq(map a, map b) {
}

/* V3CACHE_FN_END 89ffbbde3ceb5d05 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 4e04df2694c7380d */
multi_return_double_double math__ChebSeries__eval_e(math__ChebSeries cs, double x) {
	double d = 0.0;
	double dd = 0.0;
	double y = (((2.0 * x) - cs.a) - cs.b) / (cs.b - cs.a);
	double y2 = 2.0 * y;
	double e_ = 0.0;
	double temp = 0.0;
	{
		i64 j = cs.order;
		for (; j >= 1; j--) {
			temp = d;
			d = ((y2 * d) - dd) + (*(double*)array_get(cs.c, j));
			e_ += (math__abs_T_f64(y2 * temp) + math__abs_T_f64(dd)) + math__abs_T_f64((*(double*)array_get(cs.c, j)));
			dd = temp;
		}
	}
	temp = d;
	d = ((y * d) - dd) + (0.5 * (*(double*)array_get(cs.c, 0)));
	e_ += (math__abs_T_f64(y * temp) + math__abs_T_f64(dd)) + (0.5 * math__abs_T_f64((*(double*)array_get(cs.c, 0))));
	return (multi_return_double_double){d, ((double)(internal__f64_epsilon) * e_) + math__abs_T_f64((*(double*)array_get(cs.c, cs.order)))};
}

/* V3CACHE_FN_END 4e04df2694c7380d */
/* V3CACHE_MODULE __v3_program_specializations */
/* V3CACHE_FN_BEGIN c8d7ede1d11ec8e4 */
inline double math__abs_T_f64(double a) {
	if (a < 0) {
		return -a;
	} else {
		return a;
	}
}

/* V3CACHE_FN_END c8d7ede1d11ec8e4 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 7090833805a9aa57 */
double math__asinh(double x) {
	double a = math__abs_T_f64(x);
	double __if_val_0 = {0};
	if (x < 0) {
		__if_val_0 = -1.0;
	} else {
		__if_val_0 = 1.0;
	}
	double s = __if_val_0;
	if (a > (1.0 / internal__sqrt_f64_epsilon)) {
		return s * (math__log(a) + math__ln2);
	} else if (a > 2.0) {
		return s * math__log((2.0 * a) + (1.0 / (a + math__sqrt((a * a) + 1.0))));
	} else if (a > internal__sqrt_f64_epsilon) {
		double a2 = a * a;
		return s * math__log1p(a + (a2 / (1.0 + math__sqrt(1.0 + a2))));
	} else {
		return x;
	}
}

/* V3CACHE_FN_END 7090833805a9aa57 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 7cf358c71a53a736 */
double math__atanh(double x) {
	double a = math__abs_T_f64(x);
	double __if_val_0 = {0};
	if (x < 0) {
		__if_val_0 = -1.0;
	} else {
		__if_val_0 = 1.0;
	}
	double s = __if_val_0;
	if (a > 1.0) {
		return math__nan();
	} else if (a == 1.0) {
		if (x < 0) {
			return math__inf(-1);
		} else {
			return math__inf(1);
		}
	} else if (a >= 0.5) {
		return (s * 0.5) * math__log1p((2.0 * a) / (1.0 - a));
	} else if (a > internal__f64_epsilon) {
		return (s * 0.5) * math__log1p((2.0 * a) + (((2.0 * a) * a) / (1.0 - a)));
	} else {
		return x;
	}
}

/* V3CACHE_FN_END 7cf358c71a53a736 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 601ab2b62d9e53f6 */
inline double math__copysign(double x, double y) {
}

/* V3CACHE_FN_END 601ab2b62d9e53f6 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 9a9a684eca844441 */
double math__cos(double x) {
	double p1 = 7.85398125648498535156e-1;
	double p2 = 3.77489470793079817668e-8;
	double p3 = 2.69515142907905952645e-15;
	double abs_x = math__abs_T_f64(x);
	if (abs_x < internal__root4_f64_epsilon) {
		double x2 = x * x;
		return 1.0 - (0.5 * x2);
	} else {
		i64 sgn_result = 1;
		double y = math__floor(abs_x / (0.25 * math__pi));
		i64 octant = (i64)(y - math__ldexp(math__floor(math__ldexp(y, -3)), 3));
		if ((octant & 1) == 1) {
			octant++;
			octant &= 7;
			y += 1.0;
		}
		if (octant > 3) {
			octant -= 4;
			sgn_result = -sgn_result;
		}
		if (octant > 1) {
			sgn_result = -sgn_result;
		}
		double z = ((abs_x - (y * p1)) - (y * p2)) - (y * p3);
		double result = 0.0;
		if (octant == 0) {
			double t = ((8.0 * math__abs_T_f64(z)) / math__pi) - 1.0;
			multi_return_double_double __multi_ret_0 = math__ChebSeries__eval_e(math__cos_cs, t);
			double cos_cs_val = __multi_ret_0.arg0;
			result = 1.0 - (((0.5 * z) * z) * (1.0 - ((z * z) * cos_cs_val)));
		} else {
			double t = ((8.0 * math__abs_T_f64(z)) / math__pi) - 1.0;
			multi_return_double_double __multi_ret_1 = math__ChebSeries__eval_e(math__sin_cs, t);
			double sin_cs_val = __multi_ret_1.arg0;
			result = z * (1.0 + ((z * z) * sin_cs_val));
		}
		result *= (double)(sgn_result);
		return result;
	}
}

/* V3CACHE_FN_END 9a9a684eca844441 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 3a23fbe2200709c1 */
double math__cosh(double x) {
	double abs_x = math__abs_T_f64(x);
	if (abs_x > 21) {
		return math__exp(abs_x) * 0.5;
	}
	double ex = math__exp(abs_x);
	return (ex + (1.0 / ex)) * 0.5;
}

/* V3CACHE_FN_END 3a23fbe2200709c1 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 8c34d91edd283dfd */
double math__exp(double x) {
}

/* V3CACHE_FN_END 8c34d91edd283dfd */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 6505f171ca8431bb */
double math__exp2(double x) {
}

/* V3CACHE_FN_END 6505f171ca8431bb */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN eb0b9b5a679b8867 */
double math__expm1(double x) {
	if (math__is_inf(x, 1) || math__is_nan(x)) {
		return x;
	}
	if (math__is_inf(x, -1)) {
		return (double)((double)(-1));
	}
	if (math__abs_T_f64(x) < math__ln2) {
		double i = 1.0;
		double sum = x;
		double term = x / 1.0;
		i++;
		term *= x / (double)(i);
		sum += term;
		while (math__abs_T_f64(term) > (math__abs_T_f64(sum) * internal__f64_epsilon)) {
			i++;
			term *= x / (double)(i);
			sum += term;
		}
		return sum;
	} else {
		return math__exp(x) - 1;
	}
}

/* V3CACHE_FN_END eb0b9b5a679b8867 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN b0dcd11a136aa9bd */
double math__factorial(double n) {
}

/* V3CACHE_FN_END b0dcd11a136aa9bd */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 96e50ee82b4c4e8e */
double math__floor(double x) {
}

/* V3CACHE_FN_END 96e50ee82b4c4e8e */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN f395c4fe1867ace0 */
double math__fmod(double x, double y) {
	if ((y == 0) || math__is_inf(x, 0) || math__is_nan(x) || math__is_nan(y)) {
		return math__nan();
	}
	double abs_y = math__abs_T_f64(y);
	multi_return_double_i64 __multi_ret_0 = math__frexp(abs_y);
	double abs_y_fr = __multi_ret_0.arg0;
	i64 abs_y_exp = __multi_ret_0.arg1;
	double r = x;
	if (x < 0) {
		r = -x;
	}
	while (r >= abs_y) {
		multi_return_double_i64 __multi_ret_1 = math__frexp(r);
		double rfr = __multi_ret_1.arg0;
		i64 rexp = __multi_ret_1.arg1;
		if (rfr < abs_y_fr) {
			rexp = rexp - 1;
		}
		r = r - math__ldexp(abs_y, rexp - abs_y_exp);
	}
	if (x < 0) {
		r = -r;
	}
	return r;
}

/* V3CACHE_FN_END f395c4fe1867ace0 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN fdee797a6c103199 */
multi_return_double_i64 math__frexp(double x) {
}

/* V3CACHE_FN_END fdee797a6c103199 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 67315617f8f8ef47 */
double math__gamma(double a) {
	double x = a;
	if (math__is_neg_int(x) || math__is_inf(x, -1) || math__is_nan(x)) {
		return math__nan();
	}
	if (math__is_inf(x, 1)) {
		return math__inf(1);
	}
	if (x == 0.0) {
		return math__copysign(math__inf(1), x);
	}
	double q = math__abs_T_f64(x);
	double p = math__floor(q);
	if (q > 33) {
		if (x >= 0) {
			multi_return_double_double __multi_ret_0 = math__stirling(x);
			double v_y1 = __multi_ret_0.arg0;
			double y2 = __multi_ret_0.arg1;
			return v_y1 * y2;
		}
		i64 signgam = 1;
		i64 ip = (i64)(p);
		if ((ip & 1) == 0) {
			signgam = -1;
		}
		double z = q - p;
		if (z > 0.5) {
			p = p + 1;
			z = q - p;
		}
		z = q * math__sin(math__pi * z);
		if (z == 0) {
			return math__inf(signgam);
		}
		multi_return_double_double __multi_ret_1 = math__stirling(q);
		double sq1 = __multi_ret_1.arg0;
		double sq2 = __multi_ret_1.arg1;
		double absz = math__abs_T_f64(z);
		double d = (absz * sq1) * sq2;
		if (math__is_inf(d, 0)) {
			z = ((math__pi / absz) / sq1) / sq2;
		} else {
			z = math__pi / d;
		}
		return (double)((double)(signgam)) * z;
	}
	double z = 1.0;
	while (x >= 3) {
		x = x - 1;
		z = z * x;
	}
	while (x < 0) {
		if (x > -1e-09) {
			return math__gamma_too_small(x, z);
		}
		z = z / x;
		x = x + 1;
	}
	while (x < 2) {
		if (x < 1e-09) {
			return math__gamma_too_small(x, z);
		}
		z = z / x;
		x = x + 1;
	}
	if (x == 2) {
		return z;
	}
	x = x - 2;
	p = (((((((((((x * (*(double*)array_get(math__gamma_p, 0))) + (*(double*)array_get(math__gamma_p, 1))) * x) + (*(double*)array_get(math__gamma_p, 2))) * x) + (*(double*)array_get(math__gamma_p, 3))) * x) + (*(double*)array_get(math__gamma_p, 4))) * x) + (*(double*)array_get(math__gamma_p, 5))) * x) + (*(double*)array_get(math__gamma_p, 6));
	q = (((((((((((((x * (*(double*)array_get(math__gamma_q, 0))) + (*(double*)array_get(math__gamma_q, 1))) * x) + (*(double*)array_get(math__gamma_q, 2))) * x) + (*(double*)array_get(math__gamma_q, 3))) * x) + (*(double*)array_get(math__gamma_q, 4))) * x) + (*(double*)array_get(math__gamma_q, 5))) * x) + (*(double*)array_get(math__gamma_q, 6))) * x) + (*(double*)array_get(math__gamma_q, 7));
	return (z * p) / q;
}

/* V3CACHE_FN_END 67315617f8f8ef47 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 96052c253383fce0 */
double math__gamma_too_small(double x, double z) {
}

/* V3CACHE_FN_END 96052c253383fce0 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN c52244b0a1e50cf6 */
double math__hypot(double x, double y) {
	double p = math__abs_T_f64(x);
	double q = math__abs_T_f64(y);
	if (math__is_inf(p, 1) || math__is_inf(q, 1)) {
		return math__inf(1);
	}
	if (math__is_nan(p) || math__is_nan(q)) {
		return math__nan();
	}
	if (p < q) {
		double __assign_0 = q;
		double __assign_1 = p;
		p = __assign_0;
		q = __assign_1;
	}
	if (p == 0.0) {
		return 0.0;
	}
	q = q / p;
	return p * math__sqrt(1 + (q * q));
}

/* V3CACHE_FN_END c52244b0a1e50cf6 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 2fad3efb5409ca2d */
double math__inf(i64 sign) {
}

/* V3CACHE_FN_END 2fad3efb5409ca2d */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 888426fd00a11696 */
bool math__is_inf(double f, i64 sign) {
}

/* V3CACHE_FN_END 888426fd00a11696 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 5e454fc1a1ba15a0 */
bool math__is_nan(double f) {
}

/* V3CACHE_FN_END 5e454fc1a1ba15a0 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 7ee61e764bae1db */
bool math__is_neg_int(double x) {
}

/* V3CACHE_FN_END 7ee61e764bae1db */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 5e6eb747ee72298e */
bool math__is_odd_int(double x) {
}

/* V3CACHE_FN_END 5e6eb747ee72298e */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN c4e7f03aed9ab3c5 */
double math__ldexp(double frac, i64 exp) {
}

/* V3CACHE_FN_END c4e7f03aed9ab3c5 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 1ec187e3e8af50a2 */
double math__log(double a) {
}

/* V3CACHE_FN_END 1ec187e3e8af50a2 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN f7263d2eb6337495 */
double math__log10(double x) {
}

/* V3CACHE_FN_END f7263d2eb6337495 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN f64c3d2eb579d0d5 */
double math__log1p(double x) {
}

/* V3CACHE_FN_END f64c3d2eb579d0d5 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN efae45445fc8c8fa */
double math__log2(double x) {
}

/* V3CACHE_FN_END efae45445fc8c8fa */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN c661622a0ce5c8da */
double math__log_factorial_asymptotic_expansion(i64 n) {
	i64 m = 6;
	Array term = __new_array_noscan(0, 0, sizeof(double));
	double xx = (double)((double)((n + 1) * (n + 1)));
	double xj = (double)((double)(n + 1));
	double log_fact = (math__log_sqrt_2pi - xj) + ((xj - 0.5) * math__log(xj));
	i64 i = 0;
	{
		i = 0;
		for (; i < m; i++) {
			double __arr_val_0 = (*(double*)array_get(math__bernoulli, i)) / xj;
			array_push(&term, &__arr_val_0);
			xj *= xx;
		}
	}
	double sum = (*(double*)array_get(term, m - 1));
	{
		i = m - 2;
		for (; i >= 0; i--) {
			if (math__abs_T_f64(sum) <= math__abs_T_f64((*(double*)array_get(term, i)))) {
				break;
			}
			sum = (*(double*)array_get(term, i));
		}
	}
	while (i >= 0) {
		sum += (*(double*)array_get(term, i));
		i--;
	}
	return log_fact + sum;
}

/* V3CACHE_FN_END c661622a0ce5c8da */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 95b9e8e5b54b20c6 */
multi_return_double_i64 math__log_gamma_sign(double a) {
	double x = a;
	i64 sgn = 1;
	if (math__is_nan(x)) {
		return (multi_return_double_i64){x, sgn};
	}
	if (math__is_inf(x, 1)) {
		return (multi_return_double_i64){x, sgn};
	}
	if (x == 0.0) {
		return (multi_return_double_i64){math__inf(1), sgn};
	}
	bool neg = false;
	if (x < 0) {
		x = -x;
		neg = true;
	}
	if (x < math__exp2((double)(-70))) {
		if (neg) {
			sgn = -1;
		}
		return (multi_return_double_i64){-math__log(x), sgn};
	}
	double nadj = 0.0;
	if (neg) {
		if (x >= math__exp2((double)(52))) {
			return (multi_return_double_i64){math__inf(1), sgn};
		}
		double t = math__sin_pi(x);
		if (t == 0) {
			return (multi_return_double_i64){math__inf(1), sgn};
		}
		nadj = math__log(math__pi / math__abs_T_f64(t * x));
		if (t < 0) {
			sgn = -1;
		}
	}
	double lgamma = 0.0;
	if ((x == 1) || (x == 2)) {
		return (multi_return_double_i64){0.0, sgn};
	} else if (x < 2) {
		double ymin = 1.461632144968362245;
		double tc = 1.46163214496836224576e+00;
		double y = 0.0;
		i64 i = 0;
		if (x <= 0.9) {
			lgamma = -math__log(x);
			if (x >= ((ymin - 1) + 0.27)) {
				y = 1.0 - x;
				i = 0;
			} else if (x >= ((ymin - 1) - 0.27)) {
				y = x - (tc - 1);
				i = 1;
			} else {
				y = x;
				i = 2;
			}
		} else {
			lgamma = (double)(0);
			if (x >= (ymin + 0.27)) {
				y = (double)((double)(2)) - x;
				i = 0;
			} else if (x >= (ymin - 0.27)) {
				y = x - tc;
				i = 1;
			} else {
				y = x - 1;
				i = 2;
			}
		}
		if (i == 0) {
			double z = y * y;
			double gamma_p1 = (*(double*)array_get(math__lgamma_a, 0)) + (z * ((*(double*)array_get(math__lgamma_a, 2)) + (z * ((*(double*)array_get(math__lgamma_a, 4)) + (z * ((*(double*)array_get(math__lgamma_a, 6)) + (z * ((*(double*)array_get(math__lgamma_a, 8)) + (z * (*(double*)array_get(math__lgamma_a, 10)))))))))));
			double gamma_p2 = z * ((*(double*)array_get(math__lgamma_a, 1)) + (z * ((*(double*)array_get(math__lgamma_a, 3)) + (z * ((*(double*)array_get(math__lgamma_a, 5)) + (z * ((*(double*)array_get(math__lgamma_a, 7)) + (z * ((*(double*)array_get(math__lgamma_a, 9)) + (z * (*(double*)array_get(math__lgamma_a, 11))))))))))));
			double p = (y * gamma_p1) + gamma_p2;
			lgamma += (p - (0.5 * y));
		} else if (i == 1) {
			double z = y * y;
			double w = z * y;
			double gamma_p1 = (*(double*)array_get(math__lgamma_t, 0)) + (w * ((*(double*)array_get(math__lgamma_t, 3)) + (w * ((*(double*)array_get(math__lgamma_t, 6)) + (w * ((*(double*)array_get(math__lgamma_t, 9)) + (w * (*(double*)array_get(math__lgamma_t, 12)))))))));
			double gamma_p2 = (*(double*)array_get(math__lgamma_t, 1)) + (w * ((*(double*)array_get(math__lgamma_t, 4)) + (w * ((*(double*)array_get(math__lgamma_t, 7)) + (w * ((*(double*)array_get(math__lgamma_t, 10)) + (w * (*(double*)array_get(math__lgamma_t, 13)))))))));
			double gamma_p3 = (*(double*)array_get(math__lgamma_t, 2)) + (w * ((*(double*)array_get(math__lgamma_t, 5)) + (w * ((*(double*)array_get(math__lgamma_t, 8)) + (w * ((*(double*)array_get(math__lgamma_t, 11)) + (w * (*(double*)array_get(math__lgamma_t, 14)))))))));
			double tf = -1.21486290535849611461e-01;
			double tt = -3.63867699703950536541e-18;
			double p = (z * gamma_p1) - (tt - (w * (gamma_p2 + (y * gamma_p3))));
			lgamma += (tf + p);
		} else if (i == 2) {
			double gamma_p1 = y * ((*(double*)array_get(math__lgamma_u, 0)) + (y * ((*(double*)array_get(math__lgamma_u, 1)) + (y * ((*(double*)array_get(math__lgamma_u, 2)) + (y * ((*(double*)array_get(math__lgamma_u, 3)) + (y * ((*(double*)array_get(math__lgamma_u, 4)) + (y * (*(double*)array_get(math__lgamma_u, 5))))))))))));
			double gamma_p2 = 1.0 + (y * ((*(double*)array_get(math__lgamma_v, 1)) + (y * ((*(double*)array_get(math__lgamma_v, 2)) + (y * ((*(double*)array_get(math__lgamma_v, 3)) + (y * ((*(double*)array_get(math__lgamma_v, 4)) + (y * (*(double*)array_get(math__lgamma_v, 5)))))))))));
			lgamma += ((-0.5 * y) + (gamma_p1 / gamma_p2));
		}
	} else if (x < 8) {
		i64 i = (i64)(x);
		double y = x - (double)((double)(i));
		double tmp23456 = (*(double*)array_get(math__lgamma_s, 2)) + (y * ((*(double*)array_get(math__lgamma_s, 3)) + (y * ((*(double*)array_get(math__lgamma_s, 4)) + (y * ((*(double*)array_get(math__lgamma_s, 5)) + (y * (*(double*)array_get(math__lgamma_s, 6)))))))));
		double p = y * ((*(double*)array_get(math__lgamma_s, 0)) + (y * ((*(double*)array_get(math__lgamma_s, 1)) + (y * tmp23456))));
		double tmpr23456 = (*(double*)array_get(math__lgamma_r, 2)) + (y * ((*(double*)array_get(math__lgamma_r, 3)) + (y * ((*(double*)array_get(math__lgamma_r, 4)) + (y * ((*(double*)array_get(math__lgamma_r, 5)) + (y * (*(double*)array_get(math__lgamma_r, 6)))))))));
		double q = 1.0 + (y * ((*(double*)array_get(math__lgamma_r, 1)) + (y * tmpr23456)));
		lgamma = (0.5 * y) + (p / q);
		double z = 1.0;
		if (i == 7) {
			z *= (y + 6);
			z *= (y + 5);
			z *= (y + 4);
			z *= (y + 3);
			z *= (y + 2);
			lgamma += math__log(z);
		} else if (i == 6) {
			z *= (y + 5);
			z *= (y + 4);
			z *= (y + 3);
			z *= (y + 2);
			lgamma += math__log(z);
		} else if (i == 5) {
			z *= (y + 4);
			z *= (y + 3);
			z *= (y + 2);
			lgamma += math__log(z);
		} else if (i == 4) {
			z *= (y + 3);
			z *= (y + 2);
			lgamma += math__log(z);
		} else if (i == 3) {
			z *= (y + 2);
			lgamma += math__log(z);
		}
	} else if (x < math__exp2((double)(58))) {
		double t = math__log(x);
		double z = 1.0 / x;
		double y = z * z;
		double w = (*(double*)array_get(math__lgamma_w, 0)) + (z * ((*(double*)array_get(math__lgamma_w, 1)) + (y * ((*(double*)array_get(math__lgamma_w, 2)) + (y * ((*(double*)array_get(math__lgamma_w, 3)) + (y * ((*(double*)array_get(math__lgamma_w, 4)) + (y * ((*(double*)array_get(math__lgamma_w, 5)) + (y * (*(double*)array_get(math__lgamma_w, 6)))))))))))));
		lgamma = ((x - 0.5) * (t - 1.0)) + w;
	} else {
		lgamma = x * (math__log(x) - 1.0);
	}
	if (neg) {
		lgamma = nadj - lgamma;
	}
	return (multi_return_double_i64){lgamma, sgn};
}

/* V3CACHE_FN_END 95b9e8e5b54b20c6 */
/* V3CACHE_MODULE __v3_program_specializations */
/* V3CACHE_FN_BEGIN 80a9d29833dc39c8 */
inline i64 math__min_T_v_int(i64 a, i64 b) {
	if (a < b) {
		return a;
	} else {
		return b;
	}
}

/* V3CACHE_FN_END 80a9d29833dc39c8 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN ddc90f63dbb74848 */
multi_return_double_double math__modf(double f) {
	double abs_f = math__abs_T_f64(f);
	double i = 0.0;
	if (abs_f >= math__modf_maxpowtwo) {
		i = f;
	} else {
		i = abs_f + math__modf_maxpowtwo;
		i -= math__modf_maxpowtwo;
		while (i > abs_f) {
			i -= 1.0;
		}
		if (math__signbit(f)) {
			i = -i;
		}
	}
	return (multi_return_double_double){i, math__copysign(f - i, f)};
}

/* V3CACHE_FN_END ddc90f63dbb74848 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 3198d9f59d4d02d7 */
double math__nan(void) {
}

/* V3CACHE_FN_END 3198d9f59d4d02d7 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN bccd8142d3dcaea1 */
multi_return_double_i64 math__normalize(double x) {
	double smallest_normal = 2.2250738585072014e-308;
	if (math__abs_T_f64(x) < smallest_normal) {
		return (multi_return_double_i64){x * math__normalize_smallest_mask, -52};
	}
	return (multi_return_double_i64){x, 0};
}

/* V3CACHE_FN_END bccd8142d3dcaea1 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 647503e134da00e */
double math__poly_n_1_eval(Array c, i64 n, double x) {
	if (c.len == 0) {
		v_panic(_v3_lit_28_4e65924173c525b4);
	}
	i64 len = (i64)(math__min_T_v_int(c.len, n)) - 1;
	double ans = (*(double*)array_get(c, len - 1));
	Array __in_lhs_0 = array_slice(	c, 0, 	len - 1);
	{
		i64 __for_idx_1 = 0;
		for (; __for_idx_1 < __in_lhs_0.len; __for_idx_1++) {
			double e_ = *(double*)(array_get(__in_lhs_0, __for_idx_1));
			ans = e_ + (x * ans);
		}
	}
	return ans;
}

/* V3CACHE_FN_END 647503e134da00e */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 8e7f6bd9785e0d7e */
double math__poly_n_eval(Array c, i64 n, double x) {
	if (c.len == 0) {
		v_panic(_v3_lit_28_4e65924173c525b4);
	}
	i64 len = (i64)(math__min_T_v_int(c.len, n));
	double ans = (*(double*)array_get(c, len - 1));
	Array __in_lhs_0 = array_slice(	c, 0, 	len - 1);
	{
		i64 __for_idx_1 = 0;
		for (; __for_idx_1 < __in_lhs_0.len; __for_idx_1++) {
			double e_ = *(double*)(array_get(__in_lhs_0, __for_idx_1));
			ans = e_ + (x * ans);
		}
	}
	return ans;
}

/* V3CACHE_FN_END 8e7f6bd9785e0d7e */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 2d8b68cbf47d9c66 */
double math__pow(double x, double y) {
	if ((y == 0) || (x == 1)) {
		return (double)(1);
	} else if (y == 1) {
		return x;
	} else if (math__is_nan(x) || math__is_nan(y)) {
		return math__nan();
	} else if (y == 2) {
		return x * x;
	} else if (y == 3) {
		return (x * x) * x;
	} else if (x == 0) {
		if (y < 0) {
			if (math__is_odd_int(y)) {
				return math__copysign(math__inf(1), x);
			}
			return math__inf(1);
		} else if (y > 0) {
			if (math__is_odd_int(y)) {
				return x;
			}
			return (double)(0);
		}
	} else if (math__is_inf(y, 0)) {
		if (x == -1) {
			return (double)(1);
		} else if ((math__abs_T_f64(x) < 1) == math__is_inf(y, 1)) {
			return (double)(0);
		} else {
			return math__inf(1);
		}
	} else if (math__is_inf(x, 0)) {
		if (math__is_inf(x, -1)) {
			return math__pow(1 / x, -y);
		}
		if (y < 0) {
			return (double)(0);
		} else if (y > 0) {
			return math__inf(1);
		}
	} else if (y == 0.5) {
		return math__sqrt(x);
	} else if (y == -0.5) {
		return 1 / math__sqrt(x);
	}
	multi_return_double_double __multi_ret_0 = math__modf(math__abs_T_f64(y));
	double yi = __multi_ret_0.arg0;
	double yf = __multi_ret_0.arg1;
	if ((yf != 0) && (x < 0)) {
		return math__nan();
	}
	if (yi >= ((u64)(((u64)(	(u64)(1))) << (63)))) {
		if (x == -1) {
			return (double)(1);
		} else if ((math__abs_T_f64(x) < 1) == (y > 0)) {
			return (double)(0);
		} else {
			return math__inf(1);
		}
	}
	if (yf == 0.0) {
		double result = x;
		{
			i64 __discard_20_23275_23276 = 1;
			for (; __discard_20_23275_23276 < (i64)(yi); __discard_20_23275_23276++) {
				result *= x;
			}
		}
		if (y > 0) {
			return result;
		}
		return 1 / result;
	}
	double a1 = 1.0;
	i64 ae = 0;
	if (yf != 0) {
		if (yf > 0.5) {
			yf--;
			yi++;
		}
		a1 = math__exp(yf * math__log(x));
	}
	multi_return_double_i64 __multi_ret_1 = math__frexp(x);
	double x1 = __multi_ret_1.arg0;
	i64 xe = __multi_ret_1.arg1;
	{
		i64 i = (i64)(yi);
		for (; i != 0; i >>= 1) {
			if ((xe < -((i64)(((i64)(			1)) << (12)))) || (((i64)(((i64)(			1)) << (12))) < xe)) {
				ae += xe;
				break;
			}
			if ((i & 1) == 1) {
				a1 *= x1;
				ae += xe;
			}
			x1 *= x1;
						xe = (i64)(((i64)(			xe)) << (1));
			if (x1 < .5) {
				x1 += x1;
				xe--;
			}
		}
	}
	if (y < 0) {
		a1 = 1 / a1;
		ae = -ae;
	}
	return math__ldexp(a1, ae);
}

/* V3CACHE_FN_END 2d8b68cbf47d9c66 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN d39e229c853f8258 */
inline bool math__signbit(double x) {
}

/* V3CACHE_FN_END d39e229c853f8258 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 5ecd36d2ea41778e */
double math__sin(double x) {
	double p1 = 7.85398125648498535156e-1;
	double p2 = 3.77489470793079817668e-8;
	double p3 = 2.69515142907905952645e-15;
	i64 __if_val_0 = {0};
	if (x < 0) {
		__if_val_0 = -1;
	} else {
		__if_val_0 = 1;
	}
	i64 sgn_x = __if_val_0;
	double abs_x = math__abs_T_f64(x);
	if (abs_x < internal__root4_f64_epsilon) {
		double x2 = x * x;
		return x * (1.0 - (x2 / 6.0));
	} else {
		i64 sgn_result = sgn_x;
		double y = math__floor(abs_x / (0.25 * math__pi));
		i64 octant = (i64)(y - math__ldexp(math__floor(math__ldexp(y, -3)), 3));
		if ((octant & 1) == 1) {
			octant++;
			octant &= 7;
			y += 1.0;
		}
		if (octant > 3) {
			octant -= 4;
			sgn_result = -sgn_result;
		}
		double z = ((abs_x - (y * p1)) - (y * p2)) - (y * p3);
		double result = 0.0;
		if (octant == 0) {
			double t = ((8.0 * math__abs_T_f64(z)) / math__pi) - 1.0;
			multi_return_double_double __multi_ret_1 = math__ChebSeries__eval_e(math__sin_cs, t);
			double sin_cs_val = __multi_ret_1.arg0;
			result = z * (1.0 + ((z * z) * sin_cs_val));
		} else {
			double t = ((8.0 * math__abs_T_f64(z)) / math__pi) - 1.0;
			multi_return_double_double __multi_ret_2 = math__ChebSeries__eval_e(math__cos_cs, t);
			double cos_cs_val = __multi_ret_2.arg0;
			result = 1.0 - (((0.5 * z) * z) * (1.0 - ((z * z) * cos_cs_val)));
		}
		result *= (double)(sgn_result);
		return result;
	}
}

/* V3CACHE_FN_END 5ecd36d2ea41778e */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 22bffa61c76bf802 */
double math__sin_pi(double x_) {
}

/* V3CACHE_FN_END 22bffa61c76bf802 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN a01e75c6f22d7119 */
multi_return_double_double math__sincos(double x) {
	if (math__is_nan(x)) {
		return (multi_return_double_double){x, x};
	}
	double p1 = 7.85398125648498535156e-1;
	double p2 = 3.77489470793079817668e-8;
	double p3 = 2.69515142907905952645e-15;
	i64 __if_val_0 = {0};
	if (x < 0) {
		__if_val_0 = -1;
	} else {
		__if_val_0 = 1;
	}
	i64 sgn_x = __if_val_0;
	double abs_x = math__abs_T_f64(x);
	if (math__is_inf(x, sgn_x)) {
		return (multi_return_double_double){math__nan(), math__nan()};
	}
	if (abs_x < internal__root4_f64_epsilon) {
		double x2 = x * x;
		return (multi_return_double_double){x * (1.0 - (x2 / 6.0)), 1.0 - (0.5 * x2)};
	} else {
		i64 sgn_result_sin = sgn_x;
		i64 sgn_result_cos = 1;
		double y = math__floor(abs_x / (0.25 * math__pi));
		i64 octant = (i64)(y - math__ldexp(math__floor(math__ldexp(y, -3)), 3));
		if ((octant & 1) == 1) {
			octant++;
			octant &= 7;
			y += 1.0;
		}
		if (octant > 3) {
			octant -= 4;
			sgn_result_sin = -sgn_result_sin;
			sgn_result_cos = -sgn_result_cos;
		}
		i64 __if_val_1 = {0};
		if (octant > 1) {
			__if_val_1 = -sgn_result_cos;
		} else {
			__if_val_1 = sgn_result_cos;
		}
		sgn_result_cos = __if_val_1;
		double z = ((abs_x - (y * p1)) - (y * p2)) - (y * p3);
		double t = ((8.0 * math__abs_T_f64(z)) / math__pi) - 1.0;
		multi_return_double_double __multi_ret_2 = math__ChebSeries__eval_e(math__sin_cs, t);
		double sin_cs_val = __multi_ret_2.arg0;
		multi_return_double_double __multi_ret_3 = math__ChebSeries__eval_e(math__cos_cs, t);
		double cos_cs_val = __multi_ret_3.arg0;
		double result_sin = 0.0;
		double result_cos = 0.0;
		if (octant == 0) {
			result_sin = z * (1.0 + ((z * z) * sin_cs_val));
			result_cos = 1.0 - (((0.5 * z) * z) * (1.0 - ((z * z) * cos_cs_val)));
		} else {
			result_sin = 1.0 - (((0.5 * z) * z) * (1.0 - ((z * z) * cos_cs_val)));
			result_cos = z * (1.0 + ((z * z) * sin_cs_val));
		}
		result_sin *= (double)(sgn_result_sin);
		result_cos *= (double)(sgn_result_cos);
		return (multi_return_double_double){result_sin, result_cos};
	}
}

/* V3CACHE_FN_END a01e75c6f22d7119 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 1c053d225bc0b298 */
inline double math__sqrt(double a) {
}

/* V3CACHE_FN_END 1c053d225bc0b298 */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 8f4655fa56ae16ea */
multi_return_double_double math__stirling(double x) {
}

/* V3CACHE_FN_END 8f4655fa56ae16ea */
/* V3CACHE_MODULE math */
/* V3CACHE_FN_BEGIN 9c952bc2a05f5d65 */
double math__tanh(double x) {
	double maxlog = 8.8029691931113054295988e+01;
	double z = math__abs_T_f64(x);
	if (z > (0.5 * maxlog)) {
		if (x < 0) {
			return (double)((double)(-1));
		}
		return 1.0;
	} else if (z >= 0.625) {
		double s = math__exp(2.0 * z);
		z = 1.0 - (2.0 / (s + 1.0));
		if (x < 0) {
			z = -z;
		}
	} else {
		if (x == 0) {
			return x;
		}
		double s = x * x;
		z = x + (((x * s) * (((((*(double*)array_get(math__tanh_p, 0)) * s) + (*(double*)array_get(math__tanh_p, 1))) * s) + (*(double*)array_get(math__tanh_p, 2)))) / (((((s + (*(double*)array_get(math__tanh_q, 0))) * s) + (*(double*)array_get(math__tanh_q, 1))) * s) + (*(double*)array_get(math__tanh_q, 2))));
	}
	return z;
}

/* V3CACHE_FN_END 9c952bc2a05f5d65 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 430352dd206a039 */
void* memdup(void* src, ptrdiff_t sz) {
}

/* V3CACHE_FN_END 430352dd206a039 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN bf281433e23d60d7 */
void* memdup_align(void* src, ptrdiff_t sz, ptrdiff_t align) {
}

/* V3CACHE_FN_END bf281433e23d60d7 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 66c2f03fe1a145e8 */
array new_array_from_c_array(i64 len, i64 cap, i64 elm_size, void* c_array) {
}

/* V3CACHE_FN_END 66c2f03fe1a145e8 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN e14fdb71ea9685b3 */
array new_array_from_c_array_noscan(i64 len, i64 cap, i64 elm_size, void* c_array) {
}

/* V3CACHE_FN_END e14fdb71ea9685b3 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 23f64eedac2eda06 */
map new_map(i64 key_bytes, i64 value_bytes, _fn_ptr_1e8d64a3d95a0da1 hash_fn, _fn_ptr_c95f092653bee64 key_eq_fn, _fn_ptr_1a3da2026b1489ea clone_fn, _fn_ptr_5373f7edc7b60e26 free_fn) {
}

/* V3CACHE_FN_END 23f64eedac2eda06 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN b8528a93afab1cc6 */
string none__str(__v_option _0) {
}

/* V3CACHE_FN_END b8528a93afab1cc6 */
/* V3CACHE_MODULE os */
/* V3CACHE_FN_BEGIN 83a18214bd65da37 */
string os__ExecutableNotFoundError__msg(os__ExecutableNotFoundError err) {
}

/* V3CACHE_FN_END 83a18214bd65da37 */
/* V3CACHE_MODULE os */
/* V3CACHE_FN_BEGIN 7844ed005ee1bc3e */
string os__FileNotOpenedError__msg(os__FileNotOpenedError err) {
}

/* V3CACHE_FN_END 7844ed005ee1bc3e */
/* V3CACHE_MODULE os */
/* V3CACHE_FN_BEGIN c6542cbf45cf36ad */
string os__NotExpected__msg(os__NotExpected err) {
}

/* V3CACHE_FN_END c6542cbf45cf36ad */
/* V3CACHE_MODULE os */
/* V3CACHE_FN_BEGIN afc54cfb4d13484c */
string os__SizeOfTypeIs0Error__msg(os__SizeOfTypeIs0Error err) {
}

/* V3CACHE_FN_END afc54cfb4d13484c */
/* V3CACHE_MODULE os */
/* V3CACHE_FN_BEGIN 2ce72caf6e0ee31b */
bool os__exists(string path) {
}

/* V3CACHE_FN_END 2ce72caf6e0ee31b */
/* V3CACHE_MODULE os */
/* V3CACHE_FN_BEGIN 1ac47f4271d138b6 */
string os__get_line(void) {
}

/* V3CACHE_FN_END 1ac47f4271d138b6 */
/* V3CACHE_MODULE os */
/* V3CACHE_FN_BEGIN c7cfeb6fd940804b */
bool os__is_dir(string path) {
}

/* V3CACHE_FN_END c7cfeb6fd940804b */
/* V3CACHE_MODULE os */
/* V3CACHE_FN_BEGIN 18ca026fab484df5 */
string os__join_path(string base, Array dirs) {
}

/* V3CACHE_FN_END 18ca026fab484df5 */
/* V3CACHE_MODULE os */
/* V3CACHE_FN_BEGIN 2c14a3c84eb84df2 */
string os__join_path_single(string base, string elem) {
}

/* V3CACHE_FN_END 2c14a3c84eb84df2 */
/* V3CACHE_MODULE os */
/* V3CACHE_FN_BEGIN 15f4d5bc6c2a07e0 */
__v_result_Array os__ls(string path) {
	return (__v_result_Array){.ok = true};
}

/* V3CACHE_FN_END 15f4d5bc6c2a07e0 */
/* V3CACHE_MODULE os */
/* V3CACHE_FN_BEGIN 39d2941fe83896ce */
__v_result_string os__read_file(string path) {
	return (__v_result_string){.ok = true};
}

/* V3CACHE_FN_END 39d2941fe83896ce */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 556ec9417d66f66e */
VNORETURN void panic_debug(i64 line_no, string file, string mod, string fn_name, string s) {
}

/* V3CACHE_FN_END 556ec9417d66f66e */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN c0bbe34ff9820e01 */
void panic_frame_done(void* frame) {
}

/* V3CACHE_FN_END c0bbe34ff9820e01 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 5adc2b3036e4ba4c */
void panic_frame_pop(void* frame) {
}

/* V3CACHE_FN_END 5adc2b3036e4ba4c */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN ee693e99d94872ad */
void panic_frame_push(void* frame) {
}

/* V3CACHE_FN_END ee693e99d94872ad */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 3a2623fe78d4b3e2 */
void panic_frame_relink(void* frame) {
}

/* V3CACHE_FN_END 3a2623fe78d4b3e2 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 5ea124a25d4dea65 */
void panic_frames_reset(void) {
}

/* V3CACHE_FN_END 5ea124a25d4dea65 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN c8c8a888580d79d */
__v_option_string panic_recover_frame(void* frame) {
	return (__v_option_string){.ok = true};
}

/* V3CACHE_FN_END c8c8a888580d79d */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 600af3695aafa406 */
bool prims__any_float(Array args) {
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < args.len; __for_idx_0++) {
			blip__Value a = *(blip__Value*)(array_get(args, __for_idx_0));
			if (a.tag == 8) {
				return true;
			}
		}
	}
	return false;
}

/* V3CACHE_FN_END 600af3695aafa406 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 83baa12464db6f52 */
__v_result_double prims__as_f64(blip__Value v) {
	{
		int __match_tmp_0 = v.tag;
		if (__match_tmp_0 == 4) {
			return (__v_result_double){.ok = true, .value = (double)((double)(blip__Value__as_int(v)))};
		} else if (__match_tmp_0 == 8) {
			return (__v_result_double){.ok = true, .value = blip__Value__as_float(v)};
		} else {
			return (__v_result_double){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_23_2f94490da85bb157, printer__write(v)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
	}
	return (__v_result_double){.ok = true};
}

/* V3CACHE_FN_END 83baa12464db6f52 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 2595cfe607a0689b */
__v_result_blip__Vectorptr prims__as_vector_value(blip__Value v, string name) {
	if (!((v.tag == 1024) || (v.tag == 2048))) {
		return (__v_result_blip__Vectorptr){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = name; __v3_internal_symbol_join_0[1] = _v3_lit_32_9f81ea044ae9fb41; __v3_internal_symbol_join_0[2] = printer__write(v); string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_blip__Vectorptr){.ok = true, .value = blip__Value__as_vector(v)};
	return (__v_result_blip__Vectorptr){.ok = true};
}

/* V3CACHE_FN_END 2595cfe607a0689b */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 353ebedc0e2e7e7c */
__v_result_blip__Value prims__chain(string name, Array args) {
	if (args.len < 2) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(name, _v3_lit_29_7dca5081abc47265), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	i64 i = 0;
	while ((i + 1) < args.len) {
		__v_result_i64 __or_opt_0 = prims__compare((*(blip__Value*)array_get(args, i)), (*(blip__Value*)array_get(args, i + 1)));
		i64 __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
		}
		i64 c = __or_val_1;
		bool __match_val_2 = false;
		if (string__eq(name, _v3_lit_1_44bd67d473cd5d8d)) {
			__match_val_2 = c < 0;
		} else if (string__eq(name, _v3_lit_1_44bd65d473cd5a27)) {
			__match_val_2 = c > 0;
		} else if (string__eq(name, _v3_lit_2_9b351e00c5f63210)) {
			__match_val_2 = c <= 0;
		} else {
			__match_val_2 = c >= 0;
		}
		bool ok = __match_val_2;
		if (!ok) {
			return (__v_result_blip__Value){.ok = true, .value = blip__boolean(false)};
		}
		i++;
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(true)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 353ebedc0e2e7e7c */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 4fd59a0849b1639e */
i64 prims__clamp(i64 n, i64 lo, i64 hi) {
	if (n < lo) {
		return lo;
	}
	if (n > hi) {
		return hi;
	}
	return n;
}

/* V3CACHE_FN_END 4fd59a0849b1639e */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 5e8eeba13015ffa2 */
i64 prims__cmp_f64(double a, double b) {
	if (a < b) {
		return -1;
	}
	if (a > b) {
		return 1;
	}
	return 0;
}

/* V3CACHE_FN_END 5e8eeba13015ffa2 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 92f40988c9ba9235 */
i64 prims__cmp_i64(i64 a, i64 b) {
	if (a < b) {
		return -1;
	}
	if (a > b) {
		return 1;
	}
	return 0;
}

/* V3CACHE_FN_END 92f40988c9ba9235 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN c9932a8d07ceb585 */
i64 prims__cmp_text(string a, string b) {
	if (string__lt(a, b)) {
		return -1;
	}
	if (string__lt(b, a)) {
		return 1;
	}
	return 0;
}

/* V3CACHE_FN_END c9932a8d07ceb585 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN d7026101496afdee */
__v_result_i64 prims__compare(blip__Value a, blip__Value b) {
	if ((a.tag == 4) && (b.tag == 4)) {
		return (__v_result_i64){.ok = true, .value = prims__cmp_i64(blip__Value__as_int(a), blip__Value__as_int(b))};
	}
	if ((a.tag == 8) || (b.tag == 8)) {
		if (!prims__is_number(a) || !prims__is_number(b)) {
			return (__v_result_i64){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[4]; __v3_internal_symbol_join_0[0] = _v3_lit_15_3e6c6899639e6bb5; __v3_internal_symbol_join_0[1] = printer__write(a); __v3_internal_symbol_join_0[2] = _v3_lit_6_a1f910afef24beb7; __v3_internal_symbol_join_0[3] = printer__write(b); string_plus_many(4, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		__v_result_double __or_opt_0 = prims__as_f64(a);
		double __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result_i64){.ok = false, .err = __or_opt_0.err};
		}
		__v_result_double __or_opt_2 = prims__as_f64(b);
		double __or_val_3 = {0};
		if (__or_opt_2.ok) {
			__or_val_3 = __or_opt_2.value;
		} else {
			return (__v_result_i64){.ok = false, .err = __or_opt_2.err};
		}
		return (__v_result_i64){.ok = true, .value = prims__cmp_f64(__or_val_1, __or_val_3)};
	}
	if (((a.tag == 32) || (a.tag == 64) || (a.tag == 128)) && ((b.tag == 32) || (b.tag == 64) || (b.tag == 128))) {
		return (__v_result_i64){.ok = true, .value = prims__cmp_text(blip__Value__as_string(a), blip__Value__as_string(b))};
	}
	return (__v_result_i64){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[4]; __v3_internal_symbol_join_1[0] = _v3_lit_15_3e6c6899639e6bb5; __v3_internal_symbol_join_1[1] = printer__write(a); __v3_internal_symbol_join_1[2] = _v3_lit_6_a1f910afef24beb7; __v3_internal_symbol_join_1[3] = printer__write(b); string_plus_many(4, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	return (__v_result_i64){.ok = true};
}

/* V3CACHE_FN_END d7026101496afdee */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN a098d8cd3be2becb */
bool prims__is_number(blip__Value v) {
	return (v.tag == 4) || (v.tag == 8);
}

/* V3CACHE_FN_END a098d8cd3be2becb */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 500a27a9e834c443 */
bool prims__is_seq(blip__Value v) {
	return (v.tag == 256) || (v.tag == 512) || (v.tag == 1) || (v.tag == 1024) || (v.tag == 2048);
}

/* V3CACHE_FN_END 500a27a9e834c443 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN ee1cdbede5b06910 */
__v_result_Array prims__list_slice(blip__Value v) {
	Array out = array_new(	sizeof(blip__Value), 0, 0);
	blip__Value cur = v;
	i64 guard = 0;
	while (cur.tag == 256) {
		if (guard > 50000000) {
			return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_14_39c556772144adc4, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		blip__Value __arr_val_0 = (blip__Value__as_pair(cur))->car;
		array_push(&out, &__arr_val_0);
		cur = (blip__Value__as_pair(cur))->cdr;
		guard++;
	}
	return (__v_result_Array){.ok = true, .value = out};
	return (__v_result_Array){.ok = true};
}

/* V3CACHE_FN_END ee1cdbede5b06910 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN d225277b0bd7d3c9 */
__v_result_i64 prims__need_int(string name, blip__Value v) {
	if (v.tag != 4) {
		return (__v_result_i64){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = name; __v3_internal_symbol_join_0[1] = _v3_lit_25_107907bb28132e24; __v3_internal_symbol_join_0[2] = printer__write(v); string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_i64){.ok = true, .value = blip__Value__as_int(v)};
	return (__v_result_i64){.ok = true};
}

/* V3CACHE_FN_END d225277b0bd7d3c9 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN f5edfdaf8d13ef7f */
__v_result_string prims__need_str(string name, blip__Value v) {
	if (!((v.tag == 32) || (v.tag == 64) || (v.tag == 128))) {
		return (__v_result_string){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = name; __v3_internal_symbol_join_0[1] = _v3_lit_23_adaa30b73fc179ad; __v3_internal_symbol_join_0[2] = printer__write(v); string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_string){.ok = true, .value = blip__Value__as_string(v)};
	return (__v_result_string){.ok = true};
}

/* V3CACHE_FN_END f5edfdaf8d13ef7f */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 963c06c5da9d1cfa */
__v_result_blip__Value prims__prim_abs(Array args) {
	if (((*(blip__Value*)array_get(args, 0))).tag == 8) {
		double f = blip__Value__as_float((*(blip__Value*)array_get(args, 0)));
		double __if_val_0 = {0};
		if (f < 0.0) {
			__if_val_0 = -f;
		} else {
			__if_val_0 = f;
		}
		return (__v_result_blip__Value){.ok = true, .value = blip__float(__if_val_0)};
	}
	__v_result_i64 __or_opt_1 = prims__need_int(_v3_lit_3_e16811510db8ba2d, (*(blip__Value*)array_get(args, 0)));
	i64 __or_val_2 = {0};
	if (__or_opt_1.ok) {
		__or_val_2 = __or_opt_1.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_1.err};
	}
	i64 n = __or_val_2;
	i64 __if_val_3 = {0};
	if (n < 0) {
		__if_val_3 = -n;
	} else {
		__if_val_3 = n;
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__integer(__if_val_3)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 963c06c5da9d1cfa */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN cc8efcc5fa0d159f */
__v_result_blip__Value prims__prim_add(Array args) {
	if (prims__any_float(args)) {
		double f = 0.0;
		{
			i64 __for_idx_0 = 0;
			for (; __for_idx_0 < args.len; __for_idx_0++) {
				blip__Value a = *(blip__Value*)(array_get(args, __for_idx_0));
				__v_result_double __or_opt_1 = prims__as_f64(a);
				double __or_val_2 = {0};
				if (__or_opt_1.ok) {
					__or_val_2 = __or_opt_1.value;
				} else {
					return (__v_result_blip__Value){.ok = false, .err = __or_opt_1.err};
				}
				f += __or_val_2;
			}
		}
		return (__v_result_blip__Value){.ok = true, .value = blip__float(f)};
	}
	i64 sum = (i64)(0);
	{
		i64 __for_idx_3 = 0;
		for (; __for_idx_3 < args.len; __for_idx_3++) {
			blip__Value a = *(blip__Value*)(array_get(args, __for_idx_3));
			__v_result_i64 __or_opt_4 = prims__need_int(_v3_lit_1_44bd50d473cd3678, a);
			i64 __or_val_5 = {0};
			if (__or_opt_4.ok) {
				__or_val_5 = __or_opt_4.value;
			} else {
				return (__v_result_blip__Value){.ok = false, .err = __or_opt_4.err};
			}
			sum += __or_val_5;
		}
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__integer(sum)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END cc8efcc5fa0d159f */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN a90b5ff0cb383b18 */
__v_result_blip__Value prims__prim_append(Array args) {
	Array out = array_new(	sizeof(blip__Value), 0, 0);
	i64 i = 0;
	while ((i + 1) < args.len) {
		__v_result_Array __or_opt_0 = prims__list_slice((*(blip__Value*)array_get(args, i)));
		Array __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
		}
		Array part = __or_val_1;
		{
			i64 __for_idx_2 = 0;
			for (; __for_idx_2 < part.len; __for_idx_2++) {
				blip__Value p = *(blip__Value*)(array_get(part, __for_idx_2));
				blip__Value __arr_val_3 = p;
				array_push(&out, &__arr_val_3);
			}
		}
		i++;
	}
	__v_result_Array __or_opt_4 = prims__list_slice((*(blip__Value*)array_get(args, args.len - 1)));
	Array __or_val_5 = {0};
	if (__or_opt_4.ok) {
		__or_val_5 = __or_opt_4.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_4.err};
	}
	Array tail = __or_val_5;
	{
		i64 __for_idx_6 = 0;
		for (; __for_idx_6 < tail.len; __for_idx_6++) {
			blip__Value p = *(blip__Value*)(array_get(tail, __for_idx_6));
			blip__Value __arr_val_7 = p;
			array_push(&out, &__arr_val_7);
		}
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__list_from(out)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END a90b5ff0cb383b18 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN ff57ac09af43e71b */
__v_result_blip__Value prims__prim_array_emptyp(Array args) {
	__v_result_blip__Vectorptr __or_opt_0 = prims__as_vector_value((*(blip__Value*)array_get(args, 0)), _v3_lit_12_b365cad16314ea29);
	blip__Vector* __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(__or_val_1->data.len == 0)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END ff57ac09af43e71b */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 23dd13c9b5422ada */
__v_result_blip__Value prims__prim_array_length(Array args) {
	__v_result_blip__Vectorptr __or_opt_0 = prims__as_vector_value((*(blip__Value*)array_get(args, 0)), _v3_lit_12_2dcf4e18024c4ff);
	blip__Vector* __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__integer(__or_val_1->data.len)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 23dd13c9b5422ada */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN eeaf5044d08ab591 */
__v_result_blip__Value prims__prim_array_pop(Array args) {
	__v_result_blip__Vectorptr __or_opt_0 = prims__as_vector_value((*(blip__Value*)array_get(args, 0)), _v3_lit_10_e608bb067e3196f9);
	blip__Vector* __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	blip__Vector* vec = __or_val_1;
	if (vec->data.len == 0) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_30_32bfc0712bd0270, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__Vector__pop(vec)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END eeaf5044d08ab591 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 150306c5408ac912 */
__v_result_blip__Value prims__prim_array_push(Array args) {
	__v_result_blip__Vectorptr __or_opt_0 = prims__as_vector_value((*(blip__Value*)array_get(args, 0)), _v3_lit_11_6b3ca7bc363d8f9e);
	blip__Vector* __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	blip__Vector* vec = __or_val_1;
	blip__Vector__push(vec, (*(blip__Value*)array_get(args, 1)));
	return (__v_result_blip__Value){.ok = true, .value = (*(blip__Value*)array_get(args, 0))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 150306c5408ac912 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN af63d656e7cfc7e3 */
__v_result_blip__Value prims__prim_array_ref(Array args) {
	return prims__prim_vector_ref(args);
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END af63d656e7cfc7e3 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 854b0a4bfef984f0 */
__v_result_blip__Value prims__prim_array_set(Array args) {
	__v_result_blip__Vectorptr __or_opt_0 = prims__as_vector_value((*(blip__Value*)array_get(args, 0)), _v3_lit_10_c9c964e9f2b280dc);
	blip__Vector* __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	blip__Vector* vec = __or_val_1;
	__v_result_i64 __or_opt_2 = prims__need_int(_v3_lit_10_c9c964e9f2b280dc, (*(blip__Value*)array_get(args, 1)));
	i64 __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	i64 i = __or_val_3;
	if ((i < 0) || (i >= vec->data.len)) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[5]; __v3_internal_symbol_join_0[0] = _v3_lit_18_832fcc2f64f63c6e; __v3_internal_symbol_join_0[1] = i64__str(i); __v3_internal_symbol_join_0[2] = _v3_lit_22_ef0a88c1130ba183; __v3_internal_symbol_join_0[3] = int__str(vec->data.len); __v3_internal_symbol_join_0[4] = _v3_lit_1_44bd52d473cd39de; string_plus_many(5, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	blip__Vector__set_at(vec, (i64)(i), (*(blip__Value*)array_get(args, 2)));
	return (__v_result_blip__Value){.ok = true, .value = blip__nil_value()};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 854b0a4bfef984f0 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN ed12c04aaa95e161 */
__v_result_blip__Value prims__prim_arrayp(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = prims__tag_is((*(blip__Value*)array_get(args, 0)), 2048)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END ed12c04aaa95e161 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 55a9526ac4c436f8 */
__v_result_blip__Value prims__prim_booleanp(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(((*(blip__Value*)array_get(args, 0))).tag == 2)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 55a9526ac4c436f8 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 45d495b402dd90a8 */
__v_result_blip__Value prims__prim_car(Array args) {
	if (((*(blip__Value*)array_get(args, 0))).tag != 256) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_24_6883408d9e1856d4, printer__write((*(blip__Value*)array_get(args, 0)))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_blip__Value){.ok = true, .value = (blip__Value__as_pair((*(blip__Value*)array_get(args, 0))))->car};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 45d495b402dd90a8 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 1a24c8b3ea02a27b */
__v_result_blip__Value prims__prim_cdr(Array args) {
	if (((*(blip__Value*)array_get(args, 0))).tag != 256) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_24_64ef0b93792d50d3, printer__write((*(blip__Value*)array_get(args, 0)))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_blip__Value){.ok = true, .value = (blip__Value__as_pair((*(blip__Value*)array_get(args, 0))))->cdr};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 1a24c8b3ea02a27b */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN a804bdab0d8adc1b */
__v_result_blip__Value prims__prim_code_to_string(Array args) {
	__v_result_i64 __or_opt_0 = prims__need_int(_v3_lit_12_4a3428e3086109ec, (*(blip__Value*)array_get(args, 0)));
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	i64 n = __or_val_1;
	if ((n < 0) || (n > 0x10FFFF)) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_14_3bcf118c76ef1ee6; __v3_internal_symbol_join_0[1] = i64__str(n); __v3_internal_symbol_join_0[2] = _v3_lit_20_9f2988141826984a; string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__string(rune__str((u32)((u32)(n))))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END a804bdab0d8adc1b */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 4d963e1bd42fb730 */
__v_result_blip__Value prims__prim_concat(Array args) {
	Array out = array_new(	sizeof(blip__Value), 0, 0);
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < args.len; __for_idx_0++) {
			blip__Value a = *(blip__Value*)(array_get(args, __for_idx_0));
			__v_result_Array __or_opt_1 = prims__seq(a);
			Array __or_val_2 = {0};
			if (__or_opt_1.ok) {
				__or_val_2 = __or_opt_1.value;
			} else {
				return (__v_result_blip__Value){.ok = false, .err = __or_opt_1.err};
			}
			Array part = __or_val_2;
			{
				i64 __for_idx_3 = 0;
				for (; __for_idx_3 < part.len; __for_idx_3++) {
					blip__Value p = *(blip__Value*)(array_get(part, __for_idx_3));
					blip__Value __arr_val_4 = p;
					array_push(&out, &__arr_val_4);
				}
			}
		}
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__list_from(out)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 4d963e1bd42fb730 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 99233c1556aaee4f */
__v_result_blip__Value prims__prim_cons(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = blip__cons((*(blip__Value*)array_get(args, 0)), (*(blip__Value*)array_get(args, 1)))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 99233c1556aaee4f */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 2932436c08c9fddb */
__v_result_blip__Value prims__prim_count(Array args) {
	__v_result_Array __or_opt_0 = prims__seq((*(blip__Value*)array_get(args, 0)));
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__integer(__or_val_1.len)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 2932436c08c9fddb */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN e165d1acf05701e6 */
__v_result_blip__Value prims__prim_dec(Array args) {
	if (((*(blip__Value*)array_get(args, 0))).tag == 8) {
		return (__v_result_blip__Value){.ok = true, .value = blip__float(blip__Value__as_float((*(blip__Value*)array_get(args, 0))) - 1.0)};
	}
	__v_result_i64 __or_opt_0 = prims__need_int(_v3_lit_3_cadb6512638c859, (*(blip__Value*)array_get(args, 0)));
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__integer(__or_val_1 - 1)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END e165d1acf05701e6 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 3dd3accdb29b5164 */
__v_result_blip__Value prims__prim_dissoc_bang(Array args) {
	if (((*(blip__Value*)array_get(args, 0))).tag != 8192) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_30_569fddacb49dafda, printer__write((*(blip__Value*)array_get(args, 0)))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_7_14d41c1622031397, (*(blip__Value*)array_get(args, 1)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	blip__Table__delete_at(blip__Value__as_table((*(blip__Value*)array_get(args, 0))), __or_val_1);
	return (__v_result_blip__Value){.ok = true, .value = (*(blip__Value*)array_get(args, 0))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 3dd3accdb29b5164 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 59275ad0542dbcf */
__v_result_blip__Value prims__prim_div(Array args) {
	if (args.len != 2) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_29_b84c690f5a4c5780, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	if ((((*(blip__Value*)array_get(args, 0))).tag == 8) || (((*(blip__Value*)array_get(args, 1))).tag == 8)) {
		__v_result_double __or_opt_0 = prims__as_f64((*(blip__Value*)array_get(args, 1)));
		double __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
		}
		double d = __or_val_1;
		if (d == 0.0) {
			return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_16_394a2244e9c390d, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		__v_result_double __or_opt_2 = prims__as_f64((*(blip__Value*)array_get(args, 0)));
		double __or_val_3 = {0};
		if (__or_opt_2.ok) {
			__or_val_3 = __or_opt_2.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
		}
		return (__v_result_blip__Value){.ok = true, .value = blip__float(__or_val_3 / d)};
	}
	__v_result_i64 __or_opt_4 = prims__need_int(_v3_lit_1_44bd54d473cd3d44, (*(blip__Value*)array_get(args, 1)));
	i64 __or_val_5 = {0};
	if (__or_opt_4.ok) {
		__or_val_5 = __or_opt_4.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_4.err};
	}
	i64 b = __or_val_5;
	if (b == 0) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_16_394a2244e9c390d, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_i64 __or_opt_6 = prims__need_int(_v3_lit_1_44bd54d473cd3d44, (*(blip__Value*)array_get(args, 0)));
	i64 __or_val_7 = {0};
	if (__or_opt_6.ok) {
		__or_val_7 = __or_opt_6.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_6.err};
	}
	i64 a = __or_val_7;
	if ((({ i64 _t1 = (i64)(a); i64 _t2 = (i64)(b); if (_t2 == 0) v_panic(_S("modulo by zero")); (i64)(_t1 % _t2); })) == 0) {
		return (__v_result_blip__Value){.ok = true, .value = blip__integer(({ i64 _t3 = (i64)(a); i64 _t4 = (i64)(b); if (_t4 == 0) v_panic(_S("division by zero")); (i64)(_t3 / _t4); }))};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__float((double)((double)(a)) / (double)((double)(b)))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 59275ad0542dbcf */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 82e1765953ad8e3b */
__v_result_blip__Value prims__prim_drop(Array args) {
	__v_result_Array __or_opt_0 = prims__list_slice((*(blip__Value*)array_get(args, 0)));
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	Array items = __or_val_1;
	__v_result_i64 __or_opt_2 = prims__need_int(_v3_lit_4_8b35ade463598efa, (*(blip__Value*)array_get(args, 1)));
	i64 __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	i64 n = prims__clamp(__or_val_3, 0, (i64)(items.len));
	return (__v_result_blip__Value){.ok = true, .value = blip__list_from(array_slice(	items, 	n, (	items).len))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 82e1765953ad8e3b */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 4830b40c24bc10b6 */
__v_result_blip__Value prims__prim_eq(Array args) {
	i64 i = 0;
	while ((i + 1) < args.len) {
		if (!prims__value_eq((*(blip__Value*)array_get(args, i)), (*(blip__Value*)array_get(args, i + 1)))) {
			return (__v_result_blip__Value){.ok = true, .value = blip__boolean(false)};
		}
		i++;
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(true)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 4830b40c24bc10b6 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 68aa39a26d205cc8 */
__v_result_blip__Value prims__prim_eqp(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(prims__value_eq((*(blip__Value*)array_get(args, 0)), (*(blip__Value*)array_get(args, 1))))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 68aa39a26d205cc8 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 1273c8c3a48bbc2c */
__v_result_blip__Value prims__prim_even(Array args) {
	__v_result_i64 __or_opt_0 = prims__need_int(_v3_lit_5_9960f1b44cf39360, (*(blip__Value*)array_get(args, 0)));
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean((({ i64 _t1 = (i64)(__or_val_1); i64 _t2 = (i64)(2); if (_t2 == 0) v_panic(_S("modulo by zero")); (i64)(_t1 % _t2); })) == 0)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 1273c8c3a48bbc2c */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 6439fb1f320147b */
__v_result_blip__Value prims__prim_expt(Array args) {
	if ((((*(blip__Value*)array_get(args, 0))).tag == 8) || (((*(blip__Value*)array_get(args, 1))).tag == 8)) {
		__v_result_double __or_opt_0 = prims__as_f64((*(blip__Value*)array_get(args, 0)));
		double __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
		}
		__v_result_double __or_opt_2 = prims__as_f64((*(blip__Value*)array_get(args, 1)));
		double __or_val_3 = {0};
		if (__or_opt_2.ok) {
			__or_val_3 = __or_opt_2.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
		}
		return (__v_result_blip__Value){.ok = true, .value = blip__float(math__pow(__or_val_1, __or_val_3))};
	}
	i64 out = (i64)(1);
	__v_result_i64 __or_opt_4 = prims__need_int(_v3_lit_4_b426a9d918a0133a, (*(blip__Value*)array_get(args, 0)));
	i64 __or_val_5 = {0};
	if (__or_opt_4.ok) {
		__or_val_5 = __or_opt_4.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_4.err};
	}
	i64 base = __or_val_5;
	__v_result_i64 __or_opt_6 = prims__need_int(_v3_lit_4_b426a9d918a0133a, (*(blip__Value*)array_get(args, 1)));
	i64 __or_val_7 = {0};
	if (__or_opt_6.ok) {
		__or_val_7 = __or_opt_6.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_6.err};
	}
	i64 n = __or_val_7;
	if (n < 0) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_43_86e2cf4fa9fe552f, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	while (n > 0) {
		if ((n & 1) == 1) {
			out *= base;
		}
		base *= base;
		n /= 2;
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__integer(out)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 6439fb1f320147b */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 2dba365bc34e4e5a */
__v_result_blip__Value prims__prim_floatp(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = prims__tag_is((*(blip__Value*)array_get(args, 0)), 8)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 2dba365bc34e4e5a */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN b25dd0c02ef51a34 */
__v_result_blip__Value prims__prim_fntionp(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean((((*(blip__Value*)array_get(args, 0))).tag == 16384) || (((*(blip__Value*)array_get(args, 0))).tag == 32768))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END b25dd0c02ef51a34 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 4bead62a2f1d6de0 */
__v_result_blip__Value prims__prim_frequencies(Array args) {
	__v_result_Array __or_opt_0 = prims__seq((*(blip__Value*)array_get(args, 0)));
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	Array items = __or_val_1;
	map counts = new_map(sizeof(string), sizeof(blip__Value), map_hash_string, map_eq_string, map_clone_string, map_free_string);
	map first = new_map(sizeof(string), sizeof(blip__Value), map_hash_string, map_eq_string, map_clone_string, map_free_string);
	Array order = array_new(	sizeof(string), 0, 0);
	i64 i = 0;
	while (i < items.len) {
		blip__Value v = (*(blip__Value*)array_get(items, i));
		string __if_val_2 = {0};
		if (v.tag == 32) {
			__if_val_2 = blip__Value__as_string(v);
		} else {
			__if_val_2 = printer__write(v);
		}
		string key = __if_val_2;
		string __map_key_3 = key;
		if (!map__exists(&counts, &__map_key_3)) {
			string __arr_val_4 = string__clone(key);
			array_push(&order, &__arr_val_4);
			string __map_key_5 = key;
			blip__Value __map_val_6 = blip__integer(0);
			map__set(&counts, &__map_key_5, &__map_val_6);
			string __map_key_7 = key;
			blip__Value __map_val_8 = v;
			map__set(&first, &__map_key_7, &__map_val_8);
		}
		string __map_key_9 = key;
		blip__Value __map_zero_10 = (blip__Value){.tag = 0};
		i64 old = blip__Value__as_int(*(blip__Value*)(map__get(&counts, &__map_key_9, &__map_zero_10)));
		string __map_key_11 = key;
		blip__Value __map_val_12 = blip__integer(old + 1);
		map__set(&counts, &__map_key_11, &__map_val_12);
		i++;
	}
	Array out = array_new(	sizeof(blip__Value), 0, 0);
	{
		i64 __for_idx_13 = 0;
		for (; __for_idx_13 < order.len; __for_idx_13++) {
			string k = *(string*)(array_get(order, __for_idx_13));
			Array __arr_lit_14 = array_new(			sizeof(blip__Value), 0, 2);
			string __map_key_16 = k;
			blip__Value __map_zero_17 = (blip__Value){.tag = 0};
			blip__Value __arr_val_15 = *(blip__Value*)(map__get(&first, &__map_key_16, &__map_zero_17));
			array_push(&__arr_lit_14, &__arr_val_15);
			string __map_key_19 = k;
			blip__Value __map_zero_20 = (blip__Value){.tag = 0};
			blip__Value __arr_val_18 = *(blip__Value*)(map__get(&counts, &__map_key_19, &__map_zero_20));
			array_push(&__arr_lit_14, &__arr_val_18);
			blip__Value __arr_val_21 = blip__list_from(__arr_lit_14);
			array_push(&out, &__arr_val_21);
		}
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__list_from(out)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 4bead62a2f1d6de0 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 840f0f904129c030 */
__v_result_blip__Value prims__prim_gcd(Array args) {
	__v_result_i64 __or_opt_0 = prims__need_int(_v3_lit_3_f03f6e51156ca17b, (*(blip__Value*)array_get(args, 0)));
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	i64 a = __or_val_1;
	if (a < 0) {
		a = -a;
	}
	__v_result_i64 __or_opt_2 = prims__need_int(_v3_lit_3_f03f6e51156ca17b, (*(blip__Value*)array_get(args, 1)));
	i64 __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	i64 b = __or_val_3;
	if (b < 0) {
		b = -b;
	}
	while (b != 0) {
		i64 t = ({ i64 _t1 = (i64)(a); i64 _t2 = (i64)(b); if (_t2 == 0) v_panic(_S("modulo by zero")); (i64)(_t1 % _t2); });
		a = b;
		b = t;
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__integer(a)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 840f0f904129c030 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 35dc620c1a123438 */
__v_result_blip__Value prims__prim_ge(Array args) {
	return prims__chain(_v3_lit_2_9b2e2200c5f01a2e, args);
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 35dc620c1a123438 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN c99aa1993a5a716a */
__v_result_blip__Value prims__prim_gen_math_cos(Array args) {
	if (args.len != 1) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_36_8a85b00e1213c801, int__str(args.len)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_double __or_opt_0 = prims__as_f64((*(blip__Value*)array_get(args, 0)));
	double __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_31_a6649bcc4ab17db1, printer__write((*(blip__Value*)array_get(args, 0)))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	double a0 = __or_val_1;
	return (__v_result_blip__Value){.ok = true, .value = blip__float(math__cos(a0))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END c99aa1993a5a716a */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 26766487c4be856a */
__v_result_blip__Value prims__prim_gen_math_exp(Array args) {
	if (args.len != 1) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_36_d785998cdf901405, int__str(args.len)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_double __or_opt_0 = prims__as_f64((*(blip__Value*)array_get(args, 0)));
	double __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_31_66d385078f94cf7d, printer__write((*(blip__Value*)array_get(args, 0)))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	double a0 = __or_val_1;
	return (__v_result_blip__Value){.ok = true, .value = blip__float(math__exp(a0))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 26766487c4be856a */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 821fe3a10967b6c6 */
__v_result_blip__Value prims__prim_gen_math_factorial(Array args) {
	if (args.len != 1) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_42_baf19a2703724a51, int__str(args.len)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_double __or_opt_0 = prims__as_f64((*(blip__Value*)array_get(args, 0)));
	double __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_37_c544af9ea31da3c1, printer__write((*(blip__Value*)array_get(args, 0)))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	double a0 = __or_val_1;
	return (__v_result_blip__Value){.ok = true, .value = blip__float(math__factorial(a0))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 821fe3a10967b6c6 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 7814f3731bd4c27d */
__v_result_blip__Value prims__prim_gen_math_floor(Array args) {
	if (args.len != 1) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_38_7c054a37c9cfac7c, int__str(args.len)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_double __or_opt_0 = prims__as_f64((*(blip__Value*)array_get(args, 0)));
	double __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_33_2aab8fb06abeedfe, printer__write((*(blip__Value*)array_get(args, 0)))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	double a0 = __or_val_1;
	return (__v_result_blip__Value){.ok = true, .value = blip__float(math__floor(a0))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 7814f3731bd4c27d */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN a5bfc13b1b49e3d1 */
__v_result_blip__Value prims__prim_gen_math_log(Array args) {
	if (args.len != 1) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_36_909523b1cc652040, int__str(args.len)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_double __or_opt_0 = prims__as_f64((*(blip__Value*)array_get(args, 0)));
	double __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_31_5368a6cbb4b6915a, printer__write((*(blip__Value*)array_get(args, 0)))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	double a0 = __or_val_1;
	return (__v_result_blip__Value){.ok = true, .value = blip__float(math__log(a0))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END a5bfc13b1b49e3d1 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 2957db3ec8375086 */
__v_result_blip__Value prims__prim_gen_math_log10(Array args) {
	if (args.len != 1) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_38_32f525d2d0ad85b9, int__str(args.len)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_double __or_opt_0 = prims__as_f64((*(blip__Value*)array_get(args, 0)));
	double __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_33_c4b6c246fd7989d9, printer__write((*(blip__Value*)array_get(args, 0)))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	double a0 = __or_val_1;
	return (__v_result_blip__Value){.ok = true, .value = blip__float(math__log10(a0))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 2957db3ec8375086 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN ef82856f5f39eab7 */
__v_result_blip__Value prims__prim_gen_math_log2(Array args) {
	if (args.len != 1) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_37_a81874c9f6ec81be, int__str(args.len)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_double __or_opt_0 = prims__as_f64((*(blip__Value*)array_get(args, 0)));
	double __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_32_ac2ae2b11cba18a0, printer__write((*(blip__Value*)array_get(args, 0)))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	double a0 = __or_val_1;
	return (__v_result_blip__Value){.ok = true, .value = blip__float(math__log2(a0))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END ef82856f5f39eab7 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN a867b7117fc79819 */
__v_result_blip__Value prims__prim_gen_math_sin(Array args) {
	if (args.len != 1) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_36_254c287a1deef78, int__str(args.len)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_double __or_opt_0 = prims__as_f64((*(blip__Value*)array_get(args, 0)));
	double __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_31_3d4714e1b1905342, printer__write((*(blip__Value*)array_get(args, 0)))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	double a0 = __or_val_1;
	return (__v_result_blip__Value){.ok = true, .value = blip__float(math__sin(a0))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END a867b7117fc79819 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 95eb61904b6fa672 */
__v_result_blip__Value prims__prim_get(Array args) {
	if (args.len >= 3) {
		return (__v_result_blip__Value){.ok = true, .value = (*(blip__Value*)array_get(args, 2))};
	}
	if ((((*(blip__Value*)array_get(args, 0))).tag == 4096) || (((*(blip__Value*)array_get(args, 0))).tag == 8192)) {
		__v_result_string __or_opt_0 = prims__need_str(_v3_lit_3_f02b6251155be40d, (*(blip__Value*)array_get(args, 1)));
		string __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
		}
		return (__v_result_blip__Value){.ok = true, .value = blip__Table__get(blip__Value__as_table((*(blip__Value*)array_get(args, 0))), __or_val_1)};
	}
	if ((((*(blip__Value*)array_get(args, 0))).tag == 256) || (((*(blip__Value*)array_get(args, 0))).tag == 512) || (((*(blip__Value*)array_get(args, 0))).tag == 1024) || (((*(blip__Value*)array_get(args, 0))).tag == 2048)) {
		__v_result_i64 __or_opt_2 = prims__need_int(_v3_lit_3_f02b6251155be40d, (*(blip__Value*)array_get(args, 1)));
		i64 __or_val_3 = {0};
		if (__or_opt_2.ok) {
			__or_val_3 = __or_opt_2.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
		}
		i64 idx = __or_val_3;
		__v_result_Array __or_opt_4 = prims__seq((*(blip__Value*)array_get(args, 0)));
		Array __or_val_5 = {0};
		if (__or_opt_4.ok) {
			__or_val_5 = __or_opt_4.value;
		} else {
			IError err = __or_opt_4.err;
			return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_5_fd5599a4af821b6f, IError__msg(&(err))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		Array items = __or_val_5;
		if ((idx < 0) || (idx >= items.len)) {
			return (__v_result_blip__Value){.ok = true, .value = (*(blip__Value*)array_get(args, 2))};
		}
		return (__v_result_blip__Value){.ok = true, .value = (*(blip__Value*)array_get(items, idx))};
	}
	return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_25_96e8caa6cc63a7ee, printer__write((*(blip__Value*)array_get(args, 0)))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 95eb61904b6fa672 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 360e620c1a3bdad1 */
__v_result_blip__Value prims__prim_gt(Array args) {
	return prims__chain(_v3_lit_1_44bd65d473cd5a27, args);
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 360e620c1a3bdad1 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 17d7915bbea46a46 */
__v_result_blip__Value prims__prim_has_key(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_8_138cdc6d5832847c, (*(blip__Value*)array_get(args, 1)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(blip__Table__has(blip__Value__as_table((*(blip__Value*)array_get(args, 0))), __or_val_1))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 17d7915bbea46a46 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 2726100c12799301 */
__v_result_blip__Value prims__prim_in(Array args) {
	__v_result_Array __or_opt_0 = prims__seq((*(blip__Value*)array_get(args, 1)));
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_5_d9b5b78965f8ba9f, IError__msg(&(err))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	Array items = __or_val_1;
	i64 i = 0;
	while (i < items.len) {
		if (prims__value_eq((*(blip__Value*)array_get(args, 0)), (*(blip__Value*)array_get(items, i)))) {
			return (__v_result_blip__Value){.ok = true, .value = blip__boolean(true)};
		}
		i++;
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(false)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 2726100c12799301 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN af428365cc8d4e */
__v_result_blip__Value prims__prim_inc(Array args) {
	if (((*(blip__Value*)array_get(args, 0))).tag == 8) {
		return (__v_result_blip__Value){.ok = true, .value = blip__float(blip__Value__as_float((*(blip__Value*)array_get(args, 0))) + 1.0)};
	}
	__v_result_i64 __or_opt_0 = prims__need_int(_v3_lit_3_268b515134d65b29, (*(blip__Value*)array_get(args, 0)));
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__integer(__or_val_1 + 1)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END af428365cc8d4e */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN a4e9f6d369dc7f47 */
__v_result_blip__Value prims__prim_integer_to_rune(Array args) {
	__v_result_i64 __or_opt_0 = prims__need_int(_v3_lit_13_ff3a948ac618be44, (*(blip__Value*)array_get(args, 0)));
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	i64 n = __or_val_1;
	if ((n < 0) || (n > 0x10FFFF)) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_15_128eb81327dc728e; __v3_internal_symbol_join_0[1] = i64__str(n); __v3_internal_symbol_join_0[2] = _v3_lit_20_9f2988141826984a; string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__rune((u32)(n))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END a4e9f6d369dc7f47 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN b3960f8be4e641b6 */
__v_result_blip__Value prims__prim_integerp(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = prims__tag_is((*(blip__Value*)array_get(args, 0)), 4)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END b3960f8be4e641b6 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN fea53cc960c9c677 */
__v_result_blip__Value prims__prim_keyword(Array args) {
	if (((*(blip__Value*)array_get(args, 0))).tag == 128) {
		return (__v_result_blip__Value){.ok = true, .value = (*(blip__Value*)array_get(args, 0))};
	}
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_7_6cd5b4518f37a3de, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__keyword(__or_val_1)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END fea53cc960c9c677 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 7e1ad22f782ed15b */
__v_result_blip__Value prims__prim_keywordp(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = prims__tag_is((*(blip__Value*)array_get(args, 0)), 128)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 7e1ad22f782ed15b */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 6980aa29d72a90e0 */
__v_result_blip__Value prims__prim_last(Array args) {
	__v_result_Array __or_opt_0 = prims__seq((*(blip__Value*)array_get(args, 0)));
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	Array items = __or_val_1;
	if (items.len == 0) {
		return (__v_result_blip__Value){.ok = true, .value = blip__nil_value()};
	}
	return (__v_result_blip__Value){.ok = true, .value = (*(blip__Value*)array_get(items, items.len - 1))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 6980aa29d72a90e0 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN a84950c0181bbe1 */
__v_result_blip__Value prims__prim_le(Array args) {
	return prims__chain(_v3_lit_2_9b351e00c5f63210, args);
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END a84950c0181bbe1 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 5e9b1a09db3907f0 */
__v_result_blip__Value prims__prim_length(Array args) {
	{
		int __match_tmp_0 = ((*(blip__Value*)array_get(args, 0))).tag;
		if (__match_tmp_0 == 256) {
			i64 n = (i64)(0);
			blip__Value cur = (*(blip__Value*)array_get(args, 0));
			while (cur.tag == 256) {
				n++;
				cur = (blip__Value__as_pair(cur))->cdr;
			}
			return (__v_result_blip__Value){.ok = true, .value = blip__integer(n)};
		} else if (__match_tmp_0 == 1024) {
			return (__v_result_blip__Value){.ok = true, .value = blip__integer((blip__Value__as_vector((*(blip__Value*)array_get(args, 0))))->data.len)};
		} else if (__match_tmp_0 == 32) {
			return (__v_result_blip__Value){.ok = true, .value = blip__integer((blip__Value__as_string((*(blip__Value*)array_get(args, 0)))).len)};
		} else if ((__match_tmp_0 == 1) || (__match_tmp_0 == 512)) {
			return (__v_result_blip__Value){.ok = true, .value = blip__integer(0)};
		} else {
			return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_33_f7c2dace43731374, printer__write((*(blip__Value*)array_get(args, 0)))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
	}
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 5e9b1a09db3907f0 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 7e6d19e7c21ebb28 */
__v_result_blip__Value prims__prim_list(Array args) {
	Array items = array_new(	sizeof(blip__Value), 0, 0);
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < args.len; __for_idx_0++) {
			blip__Value a = *(blip__Value*)(array_get(args, __for_idx_0));
			blip__Value __arr_val_1 = a;
			array_push(&items, &__arr_val_1);
		}
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__list_from(items)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 7e6d19e7c21ebb28 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 6de14acbc2f58c7e */
__v_result_blip__Value prims__prim_list_ref(Array args) {
	__v_result_Array __or_opt_0 = prims__list_slice((*(blip__Value*)array_get(args, 0)));
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	Array items = __or_val_1;
	__v_result_i64 __or_opt_2 = prims__need_int(_v3_lit_8_b823e33ad085a91b, (*(blip__Value*)array_get(args, 1)));
	i64 __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	i64 i = __or_val_3;
	if ((i < 0) || (i >= items.len)) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[5]; __v3_internal_symbol_join_0[0] = _v3_lit_16_525bca778f711e8d; __v3_internal_symbol_join_0[1] = i64__str(i); __v3_internal_symbol_join_0[2] = _v3_lit_22_ef0a88c1130ba183; __v3_internal_symbol_join_0[3] = int__str(items.len); __v3_internal_symbol_join_0[4] = _v3_lit_1_44bd52d473cd39de; string_plus_many(5, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_blip__Value){.ok = true, .value = (*(blip__Value*)array_get(items, i))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 6de14acbc2f58c7e */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN f3ccdbcedba526fe */
__v_result_blip__Value prims__prim_listp(Array args) {
	if (blip__Value__is_empty_seq((*(blip__Value*)array_get(args, 0)))) {
		return (__v_result_blip__Value){.ok = true, .value = blip__boolean(true)};
	}
	if (((*(blip__Value*)array_get(args, 0))).tag != 256) {
		return (__v_result_blip__Value){.ok = true, .value = blip__boolean(false)};
	}
	blip__Value slow = (*(blip__Value*)array_get(args, 0));
	blip__Value fast = (*(blip__Value*)array_get(args, 0));
	while ((fast.tag == 256) && ((blip__Value__as_pair(fast))->cdr.tag == 256)) {
		fast = (blip__Value__as_pair((blip__Value__as_pair(fast))->cdr))->car;
		if (fast.tag == 1) {
			return (__v_result_blip__Value){.ok = true, .value = blip__boolean(true)};
		}
		slow = (blip__Value__as_pair(slow))->cdr;
		if (prims__value_eq(fast, slow)) {
			return (__v_result_blip__Value){.ok = true, .value = blip__boolean(false)};
		}
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean((blip__Value__as_pair(fast))->cdr.tag == 1)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END f3ccdbcedba526fe */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN abe950c01b33428 */
__v_result_blip__Value prims__prim_lt(Array args) {
	return prims__chain(_v3_lit_1_44bd67d473cd5d8d, args);
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END abe950c01b33428 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN e90d5fa8a35fd7d4 */
__v_result_blip__Value prims__prim_make_struct(Array args) {
	if (args.len < 2) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_43_c4014c7a0d850ac2, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_13_fcd81ef130279c17, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	string name = __or_val_1;
	Array fields = array_new(	sizeof(string), 0, 0);
	blip__Value fv = (*(blip__Value*)array_get(args, 1));
	if (fv.tag == 1024) {
		Array d = (blip__Value__as_vector(fv))->data;
		i64 i = 0;
		while (i < d.len) {
			__v_result_string __or_opt_2 = prims__need_str(_v3_lit_13_fcd81ef130279c17, (*(blip__Value*)array_get(d, i)));
			string __or_val_3 = {0};
			if (__or_opt_2.ok) {
				__or_val_3 = __or_opt_2.value;
			} else {
				return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
			}
			string __arr_val_4 = __or_val_3;
			array_push(&fields, &__arr_val_4);
			i++;
		}
	} else {
		string __arr_val_5 = string__clone(name);
		array_push(&fields, &__arr_val_5);
	}
	if ((args.len - 2) != fields.len) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[7]; __v3_internal_symbol_join_0[0] = _v3_lit_15_fcb000a30e2db3d5; __v3_internal_symbol_join_0[1] = name; __v3_internal_symbol_join_0[2] = _v3_lit_5_c320b6af8ce0b5fd; __v3_internal_symbol_join_0[3] = int__str(fields.len); __v3_internal_symbol_join_0[4] = _v3_lit_18_144042484dae3bc; __v3_internal_symbol_join_0[5] = int__str(args.len - 2); __v3_internal_symbol_join_0[6] = _v3_lit_9_65e5c60de39af7d6; string_plus_many(7, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	map vals = new_map(sizeof(string), sizeof(blip__Value), map_hash_string, map_eq_string, map_clone_string, map_free_string);
	i64 i = 0;
	while (i < fields.len) {
		string __map_key_6 = (*(string*)array_get(fields, i));
		blip__Value __map_val_7 = (*(blip__Value*)array_get(args, i + 2));
		map__set(&vals, &__map_key_6, &__map_val_7);
		i++;
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__struct_value(name, fields, vals)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END e90d5fa8a35fd7d4 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 6973960a53b2198 */
__v_result_blip__Value prims__prim_max(Array args) {
	blip__Value best = (*(blip__Value*)array_get(args, 0));
	i64 i = 1;
	while (i < args.len) {
		__v_result_i64 __or_opt_0 = prims__compare((*(blip__Value*)array_get(args, i)), best);
		i64 __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
		}
		if (__or_val_1 > 0) {
			best = (*(blip__Value*)array_get(args, i));
		}
		i++;
	}
	return (__v_result_blip__Value){.ok = true, .value = best};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 6973960a53b2198 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 4b048160cbbf78d2 */
__v_result_blip__Value prims__prim_min(Array args) {
	blip__Value best = (*(blip__Value*)array_get(args, 0));
	i64 i = 1;
	while (i < args.len) {
		__v_result_i64 __or_opt_0 = prims__compare((*(blip__Value*)array_get(args, i)), best);
		i64 __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
		}
		if (__or_val_1 < 0) {
			best = (*(blip__Value*)array_get(args, i));
		}
		i++;
	}
	return (__v_result_blip__Value){.ok = true, .value = best};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 4b048160cbbf78d2 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 93942566f569ed12 */
__v_result_blip__Value prims__prim_modulo(Array args) {
	__v_result_i64 __or_opt_0 = prims__need_int(_v3_lit_6_791c3c9e9fbc336d, (*(blip__Value*)array_get(args, 1)));
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	i64 b = __or_val_1;
	if (b == 0) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_14_202ccf22284dba64, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_i64 __or_opt_2 = prims__need_int(_v3_lit_6_791c3c9e9fbc336d, (*(blip__Value*)array_get(args, 0)));
	i64 __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	i64 a = __or_val_3;
	i64 m = ({ i64 _t1 = (i64)(a); i64 _t2 = (i64)(b); if (_t2 == 0) v_panic(_S("modulo by zero")); (i64)(_t1 % _t2); });
	if ((m != 0) && ((m < 0) != (b < 0))) {
		m += b;
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__integer(m)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 93942566f569ed12 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 5968056043282a50 */
__v_result_blip__Value prims__prim_mul(Array args) {
	if (prims__any_float(args)) {
		double f = 1.0;
		{
			i64 __for_idx_0 = 0;
			for (; __for_idx_0 < args.len; __for_idx_0++) {
				blip__Value a = *(blip__Value*)(array_get(args, __for_idx_0));
				__v_result_double __or_opt_1 = prims__as_f64(a);
				double __or_val_2 = {0};
				if (__or_opt_1.ok) {
					__or_val_2 = __or_opt_1.value;
				} else {
					return (__v_result_blip__Value){.ok = false, .err = __or_opt_1.err};
				}
				f *= __or_val_2;
			}
		}
		return (__v_result_blip__Value){.ok = true, .value = blip__float(f)};
	}
	i64 acc = (i64)(1);
	{
		i64 __for_idx_3 = 0;
		for (; __for_idx_3 < args.len; __for_idx_3++) {
			blip__Value a = *(blip__Value*)(array_get(args, __for_idx_3));
			__v_result_i64 __or_opt_4 = prims__need_int(_v3_lit_1_44bd51d473cd382b, a);
			i64 __or_val_5 = {0};
			if (__or_opt_4.ok) {
				__or_val_5 = __or_opt_4.value;
			} else {
				return (__v_result_blip__Value){.ok = false, .err = __or_opt_4.err};
			}
			acc *= __or_val_5;
		}
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__integer(acc)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 5968056043282a50 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN c85abbc417150e25 */
__v_result_blip__Value prims__prim_negative(Array args) {
	__v_result_i64 __or_opt_0 = prims__compare((*(blip__Value*)array_get(args, 0)), blip__integer(0));
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(__or_val_1 < 0)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END c85abbc417150e25 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 556d87583c1a5617 */
__v_result_blip__Value prims__prim_nilp(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(blip__Value__is_empty_seq((*(blip__Value*)array_get(args, 0))))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 556d87583c1a5617 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN d83f6b59b1f2905d */
__v_result_blip__Value prims__prim_not(Array args) {
	if (args.len != 1) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_22_6057764df73d5b94, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(!blip__Value__truthy((*(blip__Value*)array_get(args, 0))))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END d83f6b59b1f2905d */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN fc9d01626ee58cac */
__v_result_blip__Value prims__prim_not_eq(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(!prims__value_eq((*(blip__Value*)array_get(args, 0)), (*(blip__Value*)array_get(args, 1))))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END fc9d01626ee58cac */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN f19a6db1df26713b */
__v_result_blip__Value prims__prim_nullp(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(blip__Value__is_empty_seq((*(blip__Value*)array_get(args, 0))))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END f19a6db1df26713b */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 8e99f88c68c244ab */
__v_result_blip__Value prims__prim_number_to_string(Array args) {
	if (((*(blip__Value*)array_get(args, 0))).tag == 8) {
		return (__v_result_blip__Value){.ok = true, .value = blip__string(f64__str(blip__Value__as_float((*(blip__Value*)array_get(args, 0)))))};
	}
	__v_result_i64 __or_opt_0 = prims__need_int(_v3_lit_14_6fc210c22fa1fe34, (*(blip__Value*)array_get(args, 0)));
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__string(strconv__format_int(__or_val_1, 10))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 8e99f88c68c244ab */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 8ed0f3fe1317450f */
__v_result_blip__Value prims__prim_numberp(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(prims__is_number((*(blip__Value*)array_get(args, 0))))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 8ed0f3fe1317450f */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN b64dc84ecd9492d5 */
__v_result_blip__Value prims__prim_odd(Array args) {
	__v_result_i64 __or_opt_0 = prims__need_int(_v3_lit_4_ec87890da6744229, (*(blip__Value*)array_get(args, 0)));
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean((({ i64 _t1 = (i64)(__or_val_1); i64 _t2 = (i64)(2); if (_t2 == 0) v_panic(_S("modulo by zero")); (i64)(_t1 % _t2); })) != 0)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END b64dc84ecd9492d5 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 76c437cfaef2c636 */
__v_result_blip__Value prims__prim_pairp(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = prims__tag_is((*(blip__Value*)array_get(args, 0)), 256)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 76c437cfaef2c636 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN e20865eac5f7b44d */
__v_result_blip__Value prims__prim_positive(Array args) {
	__v_result_i64 __or_opt_0 = prims__compare((*(blip__Value*)array_get(args, 0)), blip__integer(0));
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(__or_val_1 > 0)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END e20865eac5f7b44d */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 9341f14871580ba1 */
__v_result_blip__Value prims__prim_put(Array args) {
	map mm = map__clone(&(blip__Value__as_table((*(blip__Value*)array_get(args, 0))))->values);
	__v_result_string __or_opt_1 = prims__need_str(_v3_lit_3_7264b3515f33eb20, (*(blip__Value*)array_get(args, 1)));
	string __or_val_2 = {0};
	if (__or_opt_1.ok) {
		__or_val_2 = __or_opt_1.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_1.err};
	}
	string __map_key_0 = __or_val_2;
	blip__Value __map_val_3 = (*(blip__Value*)array_get(args, 2));
	map__set(&mm, &__map_key_0, &__map_val_3);
	return (__v_result_blip__Value){.ok = true, .value = blip__table(mm)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 9341f14871580ba1 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 7970d9526a6de1d0 */
__v_result_blip__Value prims__prim_put_bang(Array args) {
	if (((*(blip__Value*)array_get(args, 0))).tag != 8192) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_27_468a469409c03a06, printer__write((*(blip__Value*)array_get(args, 0)))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_4_9507b444c53852b3, (*(blip__Value*)array_get(args, 1)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	blip__Table__set_at(blip__Value__as_table((*(blip__Value*)array_get(args, 0))), __or_val_1, (*(blip__Value*)array_get(args, 2)));
	return (__v_result_blip__Value){.ok = true, .value = blip__nil_value()};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 7970d9526a6de1d0 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN cdf0de8288cde187 */
__v_result_blip__Value prims__prim_quotient(Array args) {
	__v_result_i64 __or_opt_0 = prims__need_int(_v3_lit_8_f651b3be55260426, (*(blip__Value*)array_get(args, 1)));
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	i64 b = __or_val_1;
	if (b == 0) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_16_5a44d6ce93cd17ff, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_i64 __or_opt_2 = prims__need_int(_v3_lit_8_f651b3be55260426, (*(blip__Value*)array_get(args, 0)));
	i64 __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	i64 a = __or_val_3;
	i64 q = ({ i64 _t1 = (i64)(a); i64 _t2 = (i64)(b); if (_t2 == 0) v_panic(_S("division by zero")); (i64)(_t1 / _t2); });
	if (((({ i64 _t3 = (i64)(a); i64 _t4 = (i64)(b); if (_t4 == 0) v_panic(_S("modulo by zero")); (i64)(_t3 % _t4); })) != 0) && ((a < 0) != (b < 0))) {
		q--;
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__integer(q)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END cdf0de8288cde187 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 855e5fd151aa2cb1 */
__v_result_blip__Value prims__prim_range(Array args) {
	i64 from = (i64)(0);
	i64 to = (i64)(0);
	if (args.len == 1) {
		__v_result_i64 __or_opt_0 = prims__need_int(_v3_lit_5_96f5e4207ce9b718, (*(blip__Value*)array_get(args, 0)));
		i64 __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
		}
		to = __or_val_1;
	} else {
		__v_result_i64 __or_opt_2 = prims__need_int(_v3_lit_5_96f5e4207ce9b718, (*(blip__Value*)array_get(args, 0)));
		i64 __or_val_3 = {0};
		if (__or_opt_2.ok) {
			__or_val_3 = __or_opt_2.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
		}
		from = __or_val_3;
		__v_result_i64 __or_opt_4 = prims__need_int(_v3_lit_5_96f5e4207ce9b718, (*(blip__Value*)array_get(args, 1)));
		i64 __or_val_5 = {0};
		if (__or_opt_4.ok) {
			__or_val_5 = __or_opt_4.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_4.err};
		}
		to = __or_val_5;
	}
	Array out = array_new(	sizeof(blip__Value), 0, 0);
	i64 i = from;
	while (i <= to) {
		blip__Value __arr_val_6 = blip__integer(i);
		array_push(&out, &__arr_val_6);
		i++;
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__list_from(out)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 855e5fd151aa2cb1 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN b144317b146b1c3f */
__v_result_blip__Value prims__prim_remainder(Array args) {
	__v_result_i64 __or_opt_0 = prims__need_int(_v3_lit_9_15e2e1252626de9e, (*(blip__Value*)array_get(args, 1)));
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	i64 b = __or_val_1;
	if (b == 0) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_17_5ada4d5fb12d4be7, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	__v_result_i64 __or_opt_2 = prims__need_int(_v3_lit_9_15e2e1252626de9e, (*(blip__Value*)array_get(args, 0)));
	i64 __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__integer(({ i64 _t1 = (i64)(__or_val_3); i64 _t2 = (i64)(b); if (_t2 == 0) v_panic(_S("modulo by zero")); (i64)(_t1 % _t2); }))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END b144317b146b1c3f */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN dc046ecef54eb9f4 */
__v_result_blip__Value prims__prim_remove(Array args) {
	map mm = map__clone(&(blip__Value__as_table((*(blip__Value*)array_get(args, 0))))->values);
	__v_result_string __or_opt_1 = prims__need_str(_v3_lit_6_13e8210fb3d5dc0f, (*(blip__Value*)array_get(args, 1)));
	string __or_val_2 = {0};
	if (__or_opt_1.ok) {
		__or_val_2 = __or_opt_1.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_1.err};
	}
	string __map_key_0 = __or_val_2;
	map__delete(&mm, &__map_key_0);
	return (__v_result_blip__Value){.ok = true, .value = blip__table(mm)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END dc046ecef54eb9f4 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN a3402403696b67dc */
__v_result_blip__Value prims__prim_reverse(Array args) {
	__v_result_Array __or_opt_0 = prims__seq((*(blip__Value*)array_get(args, 0)));
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	Array items = __or_val_1;
	Array out = array_new(	sizeof(blip__Value), 0, 0);
	i64 i = items.len - 1;
	while (i >= 0) {
		blip__Value __arr_val_2 = (*(blip__Value*)array_get(items, i));
		array_push(&out, &__arr_val_2);
		i--;
	}
	return (__v_result_blip__Value){.ok = true, .value = prims__seq_like((*(blip__Value*)array_get(args, 0)), out)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END a3402403696b67dc */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN c00d210fbb2c9c81 */
__v_result_blip__Value prims__prim_rune_to_integer(Array args) {
	blip__Value v = (*(blip__Value*)array_get(args, 0));
	if (v.tag == 16) {
		return (__v_result_blip__Value){.ok = true, .value = blip__integer(blip__Value__as_int(v))};
	}
	if (v.tag == 4) {
		return (__v_result_blip__Value){.ok = true, .value = v};
	}
	return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_15_368f10ef53a5181c; __v3_internal_symbol_join_0[1] = printer__write(v); __v3_internal_symbol_join_0[2] = _v3_lit_19_784951e180b3f488; string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END c00d210fbb2c9c81 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 927f4e3bba17025e */
__v_result_blip__Value prims__prim_runep(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(((*(blip__Value*)array_get(args, 0))).tag == 16)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 927f4e3bba17025e */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN ae4def814a591cb8 */
__v_result_blip__Value prims__prim_sort(Array args) {
	__v_result_Array __or_opt_0 = prims__seq((*(blip__Value*)array_get(args, 0)));
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	Array items = __or_val_1;
	Array ordered = array__clone(&items);
	i64 i = 1;
	while (i < ordered.len) {
		i64 j = i;
		while ((j > 0) && ({__v_result_i64 __or_opt_2 = prims__compare((*(blip__Value*)array_get(ordered, j - 1)), (*(blip__Value*)array_get(ordered, j)));
		i64 __or_val_3 = {0};
		if (__or_opt_2.ok) {
			__or_val_3 = __or_opt_2.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
		}
		__or_val_3 > 0;})) {
			blip__Value tmp = (*(blip__Value*)array_get(ordered, j - 1));
			{ Array* _a0 = &ordered; int _i0 = j - 1; array__set(_a0, _i0, &(blip__Value[]){(*(blip__Value*)array_get(ordered, j))}); }
			{ Array* _a1 = &ordered; int _i1 = j; array__set(_a1, _i1, &(blip__Value[]){tmp}); }
			j--;
		}
		i++;
	}
	return (__v_result_blip__Value){.ok = true, .value = prims__seq_like((*(blip__Value*)array_get(args, 0)), ordered)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END ae4def814a591cb8 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN e2a7c7d5338b1802 */
__v_result_blip__Value prims__prim_sqrt(Array args) {
	blip__Value a = (*(blip__Value*)array_get(args, 0));
	if (a.tag == 8) {
		__v_result_double __or_opt_0 = prims__as_f64(a);
		double __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
		}
		double f = __or_val_1;
		if (f < 0.0) {
			return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_25_65ae29a1c0743a99, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
		return (__v_result_blip__Value){.ok = true, .value = blip__float(math__sqrt(f))};
	}
	__v_result_i64 __or_opt_2 = prims__need_int(_v3_lit_4_b8fad92c55cca545, a);
	i64 __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	i64 n = __or_val_3;
	if (n < 0) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_25_65ae29a1c0743a99, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	i64 r = (i64)(math__sqrt((double)((double)(n))));
	if (((i64)(r) * (i64)(r)) == n) {
		return (__v_result_blip__Value){.ok = true, .value = blip__integer(r)};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__float(math__sqrt((double)((double)(n))))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END e2a7c7d5338b1802 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN d04a38300fdfa5db */
__v_result_blip__Value prims__prim_str(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = blip__string(prims__show((*(blip__Value*)array_get(args, 0)), true))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END d04a38300fdfa5db */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 5570091784b19a4f */
__v_result_blip__Value prims__prim_string(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_6_3850125edc81f186, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__string(__or_val_1)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 5570091784b19a4f */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 55d92a64b38ac528 */
__v_result_blip__Value prims__prim_string_append(Array args) {
	Array buf = __new_array_noscan(0, 0, sizeof(u8));
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < args.len; __for_idx_0++) {
			blip__Value a = *(blip__Value*)(array_get(args, __for_idx_0));
			__v_result_string __or_opt_1 = prims__need_str(_v3_lit_13_c2dbf1d17ad22d5, a);
			string __or_val_2 = {0};
			if (__or_opt_1.ok) {
				__or_val_2 = __or_opt_1.value;
			} else {
				return (__v_result_blip__Value){.ok = false, .err = __or_opt_1.err};
			}
			Array __push_many_3 = string__bytes(__or_val_2);
			array__push_many(&buf, __push_many_3.data, __push_many_3.len);
		}
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__string(Array_u8__bytestr(buf))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 55d92a64b38ac528 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN b80aac18e8f3e044 */
__v_result_blip__Value prims__prim_string_blank(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_13_bade188409f1efde, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	bool __str_cmp_2 = string__eq(string__trim_space(__or_val_1), _v3_lit_0_14650fb0739d0383);
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(__str_cmp_2)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END b80aac18e8f3e044 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN a95c6310c2accbc7 */
__v_result_blip__Value prims__prim_string_containsp(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_16_35bef6f51e6ad421, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	string hay = __or_val_1;
	__v_result_string __or_opt_2 = prims__need_str(_v3_lit_16_35bef6f51e6ad421, (*(blip__Value*)array_get(args, 1)));
	string __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	string needle = __or_val_3;
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(string__contains(hay, needle))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END a95c6310c2accbc7 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 52928b6a11a466b8 */
__v_result_blip__Value prims__prim_string_downcase(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_15_74b513a653904005, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__string(string__to_lower(__or_val_1))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 52928b6a11a466b8 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 6969361f48e3da60 */
__v_result_blip__Value prims__prim_string_index_of(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_15_7b8cf9c1a55e8d5f, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	string hay = __or_val_1;
	if (((*(blip__Value*)array_get(args, 1))).tag == 16) {
		string needle = prims__rune_bytes(blip__Value__as_int((*(blip__Value*)array_get(args, 1))));
		__v_option_i64 __or_opt_2 = string__index(hay, needle);
		i64 __or_val_3 = {0};
		if (__or_opt_2.ok) {
			__or_val_3 = __or_opt_2.value;
		} else {
			return (__v_result_blip__Value){.ok = true, .value = blip__integer(-1)};
		}
		i64 i = __or_val_3;
		return (__v_result_blip__Value){.ok = true, .value = blip__integer((i64)(i))};
	}
	__v_result_string __or_opt_4 = prims__need_str(_v3_lit_15_7b8cf9c1a55e8d5f, (*(blip__Value*)array_get(args, 1)));
	string __or_val_5 = {0};
	if (__or_opt_4.ok) {
		__or_val_5 = __or_opt_4.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_4.err};
	}
	string needle = __or_val_5;
	__v_option_i64 __or_opt_6 = string__index(hay, needle);
	i64 __or_val_7 = {0};
	if (__or_opt_6.ok) {
		__or_val_7 = __or_opt_6.value;
	} else {
		return (__v_result_blip__Value){.ok = true, .value = blip__integer(-1)};
	}
	i64 i = __or_val_7;
	return (__v_result_blip__Value){.ok = true, .value = blip__integer((i64)(i))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 6969361f48e3da60 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 7631331dda1cc2b0 */
__v_result_blip__Value prims__prim_string_join(Array args) {
	__v_result_Array __or_opt_0 = prims__seq((*(blip__Value*)array_get(args, 0)));
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	Array items = __or_val_1;
	string sep = _v3_lit_0_14650fb0739d0383;
	if (args.len > 1) {
		__v_result_string __or_opt_2 = prims__need_str(_v3_lit_11_84f1169aeef50035, (*(blip__Value*)array_get(args, 1)));
		string __or_val_3 = {0};
		if (__or_opt_2.ok) {
			__or_val_3 = __or_opt_2.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
		}
		sep = __or_val_3;
	}
	Array out = __new_array_noscan(0, 0, sizeof(u8));
	i64 i = 0;
	while (i < items.len) {
		if (i > 0) {
			Array __push_many_4 = string__bytes(sep);
			array__push_many(&out, __push_many_4.data, __push_many_4.len);
		}
		__v_result_string __or_opt_5 = prims__need_str(_v3_lit_11_84f1169aeef50035, (*(blip__Value*)array_get(items, i)));
		string __or_val_6 = {0};
		if (__or_opt_5.ok) {
			__or_val_6 = __or_opt_5.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_5.err};
		}
		Array __push_many_7 = string__bytes(__or_val_6);
		array__push_many(&out, __push_many_7.data, __push_many_7.len);
		i++;
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__string(Array_u8__bytestr(out))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 7631331dda1cc2b0 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 12c3e77868e94f20 */
__v_result_blip__Value prims__prim_string_length(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_13_1cc9ab73c965e0a9, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__integer(__or_val_1.len)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 12c3e77868e94f20 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 8f2876fd07907202 */
__v_result_blip__Value prims__prim_string_not_blank(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_17_e361026ca9583b4e, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	bool __str_cmp_2 = !string__eq(string__trim_space(__or_val_1), _v3_lit_0_14650fb0739d0383);
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(__str_cmp_2)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 8f2876fd07907202 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN c85496e8653cdb35 */
__v_result_blip__Value prims__prim_string_repeat(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_13_b200ce5844ed589e, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	__v_result_i64 __or_opt_2 = prims__need_int(_v3_lit_13_b200ce5844ed589e, (*(blip__Value*)array_get(args, 1)));
	i64 __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__string(string__repeat(__or_val_1, (i64)(__or_val_3)))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END c85496e8653cdb35 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 896f6a3104b3ef62 */
__v_result_blip__Value prims__prim_string_replace(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_14_81b652afad6b830f, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	string s = __or_val_1;
	__v_result_string __or_opt_2 = prims__need_str(_v3_lit_14_81b652afad6b830f, (*(blip__Value*)array_get(args, 1)));
	string __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	__v_result_string __or_opt_4 = prims__need_str(_v3_lit_14_81b652afad6b830f, (*(blip__Value*)array_get(args, 2)));
	string __or_val_5 = {0};
	if (__or_opt_4.ok) {
		__or_val_5 = __or_opt_4.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_4.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__string(string__replace(s, __or_val_3, __or_val_5))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 896f6a3104b3ef62 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 535aada1d72390f0 */
__v_result_blip__Value prims__prim_string_slice(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_12_7c1279d672f973a5, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	string s = __or_val_1;
	__v_result_i64 __or_opt_2 = prims__need_int(_v3_lit_12_7c1279d672f973a5, (*(blip__Value*)array_get(args, 1)));
	i64 __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	i64 start = __or_val_3;
	__v_result_i64 __or_opt_4 = prims__need_int(_v3_lit_12_7c1279d672f973a5, (*(blip__Value*)array_get(args, 2)));
	i64 __or_val_5 = {0};
	if (__or_opt_4.ok) {
		__or_val_5 = __or_opt_4.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_4.err};
	}
	i64 end = __or_val_5;
	i64 lo = prims__clamp(start, 0, (i64)(s.len));
	i64 hi = prims__clamp(end, lo, (i64)(s.len));
	return (__v_result_blip__Value){.ok = true, .value = blip__string(string__substr(	s, 	(i64)(lo), 	(i64)(hi)))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 535aada1d72390f0 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN c84f1427d2ed8356 */
__v_result_blip__Value prims__prim_string_split(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_12_9e98aa3ad92dd337, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	string s = __or_val_1;
	__v_result_string __or_opt_2 = prims__need_str(_v3_lit_12_9e98aa3ad92dd337, (*(blip__Value*)array_get(args, 1)));
	string __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	string sep = __or_val_3;
	Array items = array_new(	sizeof(blip__Value), 0, 0);
	Array __in_lhs_4 = string__split(s, sep);
	{
		i64 __for_idx_5 = 0;
		for (; __for_idx_5 < __in_lhs_4.len; __for_idx_5++) {
			string part = *(string*)(array_get(__in_lhs_4, __for_idx_5));
			blip__Value __arr_val_6 = blip__string(part);
			array_push(&items, &__arr_val_6);
		}
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__list_from(items)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END c84f1427d2ed8356 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN d379dcb59ede3003 */
__v_result_blip__Value prims__prim_string_start_withp(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_18_150f5d003c9d16b, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	__v_result_string __or_opt_2 = prims__need_str(_v3_lit_18_150f5d003c9d16b, (*(blip__Value*)array_get(args, 1)));
	string __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(string__starts_with(__or_val_1, __or_val_3))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END d379dcb59ede3003 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 9dc6709ac63de307 */
__v_result_blip__Value prims__prim_string_to_keyword(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_15_eb9647638504e0dc, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__keyword(__or_val_1)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 9dc6709ac63de307 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN b444adb6b0d4a303 */
__v_result_blip__Value prims__prim_string_to_number(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_14_80ba372c0e6c3078, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	string s = __or_val_1;
	__v_result_i64 __if_guard_2 = strconv__atoi64(s);
	if (__if_guard_2.ok) {
		i64 n = __if_guard_2.value;
		return (__v_result_blip__Value){.ok = true, .value = blip__integer(n)};
	}
	__v_result_double __if_guard_3 = strconv__atof64(s, prims__strict_float);
	if (__if_guard_3.ok) {
		double f = __if_guard_3.value;
		return (__v_result_blip__Value){.ok = true, .value = blip__float(f)};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__nil_value()};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END b444adb6b0d4a303 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN d5fa084789d4d858 */
__v_result_blip__Value prims__prim_string_to_rune(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_12_e8852f9b97575ce5, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	string s = __or_val_1;
	if (s.len != 1) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_52_9de7b53f6cd64471; __v3_internal_symbol_join_0[1] = s; __v3_internal_symbol_join_0[2] = _v3_lit_1_44bd49d473cd2a93; string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__rune(string__at(s, 0))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END d5fa084789d4d858 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 37fce85f45947728 */
__v_result_blip__Value prims__prim_string_to_symbol(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_14_68acb05b19e75529, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__symbol(__or_val_1)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 37fce85f45947728 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 64ccc1d6285262b8 */
__v_result_blip__Value prims__prim_string_trim(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_11_bac2bbe2efd88755, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__string(string__trim_space(__or_val_1))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 64ccc1d6285262b8 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN a85845e20e5f01db */
__v_result_blip__Value prims__prim_string_upcase(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_13_c9486c3e7201d588, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__string(string__to_upper(__or_val_1))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END a85845e20e5f01db */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN dd5822f67866b7c3 */
__v_result_blip__Value prims__prim_stringp(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = prims__tag_is((*(blip__Value*)array_get(args, 0)), 32)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END dd5822f67866b7c3 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 5bf5ecf1db48803a */
__v_result_blip__Value prims__prim_struct_mutate(Array args) {
	if (((*(blip__Value*)array_get(args, 0))).tag != 131072) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_18_4dedf3bb98c52842; __v3_internal_symbol_join_0[1] = printer__write((*(blip__Value*)array_get(args, 0))); __v3_internal_symbol_join_0[2] = _v3_lit_16_869ea3525e9dc698; string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	blip__StructVal* s = blip__Value__as_struct((*(blip__Value*)array_get(args, 0)));
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_16_fdd74acda8495128, (*(blip__Value*)array_get(args, 1)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	string k = __or_val_1;
	if (!blip__StructVal__has(s, k)) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[4]; __v3_internal_symbol_join_1[0] = _v3_lit_18_4dedf3bb98c52842; __v3_internal_symbol_join_1[1] = s->name; __v3_internal_symbol_join_1[2] = _v3_lit_14_14368fa9ebdd61d6; __v3_internal_symbol_join_1[3] = k; string_plus_many(4, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	blip__StructVal__set(s, k, (*(blip__Value*)array_get(args, 2)));
	return (__v_result_blip__Value){.ok = true, .value = (*(blip__Value*)array_get(args, 0))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 5bf5ecf1db48803a */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN cf1f1651c4ae19c5 */
__v_result_blip__Value prims__prim_struct_name(Array args) {
	if (((*(blip__Value*)array_get(args, 0))).tag != 131072) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_13_2537d281d799aa5c; __v3_internal_symbol_join_0[1] = printer__write((*(blip__Value*)array_get(args, 0))); __v3_internal_symbol_join_0[2] = _v3_lit_16_869ea3525e9dc698; string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__symbol((blip__Value__as_struct((*(blip__Value*)array_get(args, 0))))->name)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END cf1f1651c4ae19c5 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN a299aab824bc0d49 */
__v_result_blip__Value prims__prim_struct_ref(Array args) {
	if (((*(blip__Value*)array_get(args, 0))).tag != 131072) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_12_986253de95362a38; __v3_internal_symbol_join_0[1] = printer__write((*(blip__Value*)array_get(args, 0))); __v3_internal_symbol_join_0[2] = _v3_lit_16_869ea3525e9dc698; string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	blip__StructVal* s = blip__Value__as_struct((*(blip__Value*)array_get(args, 0)));
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_10_98c096c20686822, (*(blip__Value*)array_get(args, 1)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	string k = __or_val_1;
	if (!blip__StructVal__has(s, k)) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[4]; __v3_internal_symbol_join_1[0] = _v3_lit_12_986253de95362a38; __v3_internal_symbol_join_1[1] = s->name; __v3_internal_symbol_join_1[2] = _v3_lit_14_14368fa9ebdd61d6; __v3_internal_symbol_join_1[3] = k; string_plus_many(4, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__StructVal__get(s, k)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END a299aab824bc0d49 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 91ffe6ac27efe50a */
__v_result_blip__Value prims__prim_struct_set(Array args) {
	if (((*(blip__Value*)array_get(args, 0))).tag != 131072) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_13_17da5e6a1747173c; __v3_internal_symbol_join_0[1] = printer__write((*(blip__Value*)array_get(args, 0))); __v3_internal_symbol_join_0[2] = _v3_lit_16_869ea3525e9dc698; string_plus_many(3, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	blip__StructVal* s = blip__Value__as_struct((*(blip__Value*)array_get(args, 0)));
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_11_819a09b4259b9006, (*(blip__Value*)array_get(args, 1)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	string k = __or_val_1;
	if (!blip__StructVal__has(s, k)) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_1[4]; __v3_internal_symbol_join_1[0] = _v3_lit_13_17da5e6a1747173c; __v3_internal_symbol_join_1[1] = s->name; __v3_internal_symbol_join_1[2] = _v3_lit_14_14368fa9ebdd61d6; __v3_internal_symbol_join_1[3] = k; string_plus_many(4, __v3_internal_symbol_join_1); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	blip__StructVal* __iface_src_2 = blip__StructVal__with(s, k, (*(blip__Value*)array_get(args, 2)));
	return (__v_result_blip__Value){.ok = true, .value = (blip__Value){.tag = 131072, .payload = (blip__Payload){._typ = 2048012492, ._object = __iface_src_2}}};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 91ffe6ac27efe50a */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 14b05b1133a4c5cf */
__v_result_blip__Value prims__prim_structp(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(((*(blip__Value*)array_get(args, 0))).tag == 131072)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 14b05b1133a4c5cf */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN dacf6130165bb2c4 */
__v_result_blip__Value prims__prim_sub(Array args) {
	if (args.len == 0) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = _v3_lit_29_3db5fd0586ed702, .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	if (args.len == 1) {
		__v_result_i64 __or_opt_0 = prims__need_int(_v3_lit_1_44bd56d473cd40aa, (*(blip__Value*)array_get(args, 0)));
		i64 __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
		}
		return (__v_result_blip__Value){.ok = true, .value = blip__integer(-__or_val_1)};
	}
	if (prims__any_float(args)) {
		__v_result_double __or_opt_2 = prims__as_f64((*(blip__Value*)array_get(args, 0)));
		double __or_val_3 = {0};
		if (__or_opt_2.ok) {
			__or_val_3 = __or_opt_2.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
		}
		double f = __or_val_3;
		{
			i64 i = 1;
			for (; i < args.len; i++) {
				__v_result_double __or_opt_4 = prims__as_f64((*(blip__Value*)array_get(args, i)));
				double __or_val_5 = {0};
				if (__or_opt_4.ok) {
					__or_val_5 = __or_opt_4.value;
				} else {
					return (__v_result_blip__Value){.ok = false, .err = __or_opt_4.err};
				}
				f -= __or_val_5;
			}
		}
		return (__v_result_blip__Value){.ok = true, .value = blip__float(f)};
	}
	__v_result_i64 __or_opt_6 = prims__need_int(_v3_lit_1_44bd56d473cd40aa, (*(blip__Value*)array_get(args, 0)));
	i64 __or_val_7 = {0};
	if (__or_opt_6.ok) {
		__or_val_7 = __or_opt_6.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_6.err};
	}
	i64 acc = __or_val_7;
	{
		i64 i = 1;
		for (; i < args.len; i++) {
			__v_result_i64 __or_opt_8 = prims__need_int(_v3_lit_1_44bd56d473cd40aa, (*(blip__Value*)array_get(args, i)));
			i64 __or_val_9 = {0};
			if (__or_opt_8.ok) {
				__or_val_9 = __or_opt_8.value;
			} else {
				return (__v_result_blip__Value){.ok = false, .err = __or_opt_8.err};
			}
			acc -= __or_val_9;
		}
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__integer(acc)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END dacf6130165bb2c4 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 3f769f721ed98ad8 */
__v_result_blip__Value prims__prim_symbol(Array args) {
	__v_result_string __or_opt_0 = prims__need_str(_v3_lit_6_ee267171343aae33, (*(blip__Value*)array_get(args, 0)));
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__symbol(__or_val_1)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 3f769f721ed98ad8 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN ae6515ea6a31f5ee */
__v_result_blip__Value prims__prim_symbolp(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = prims__tag_is((*(blip__Value*)array_get(args, 0)), 64)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END ae6515ea6a31f5ee */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN af01b75a44dceeab */
__v_result_blip__Value prims__prim_table_keys(Array args) {
	Array keys = blip__Table__keys(blip__Value__as_table((*(blip__Value*)array_get(args, 0))));
	Array items = array_new(	sizeof(blip__Value), 0, 0);
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < keys.len; __for_idx_0++) {
			string k = *(string*)(array_get(keys, __for_idx_0));
			blip__Value __arr_val_1 = blip__keyword(k);
			array_push(&items, &__arr_val_1);
		}
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__list_from(items)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END af01b75a44dceeab */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 2469385167dc2be4 */
__v_result_blip__Value prims__prim_tablep(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = prims__tag_is((*(blip__Value*)array_get(args, 0)), 4096)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 2469385167dc2be4 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN d47dfaebf17af9c9 */
__v_result_blip__Value prims__prim_take(Array args) {
	__v_result_Array __or_opt_0 = prims__list_slice((*(blip__Value*)array_get(args, 0)));
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	Array items = __or_val_1;
	__v_result_i64 __or_opt_2 = prims__need_int(_v3_lit_4_bbcbbc682f613010, (*(blip__Value*)array_get(args, 1)));
	i64 __or_val_3 = {0};
	if (__or_opt_2.ok) {
		__or_val_3 = __or_opt_2.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_2.err};
	}
	i64 n = prims__clamp(__or_val_3, 0, (i64)(items.len));
	Array head = array_new(	sizeof(blip__Value), 0, 0);
	i64 i = 0;
	while (i < n) {
		blip__Value __arr_val_4 = (*(blip__Value*)array_get(items, i));
		array_push(&head, &__arr_val_4);
		i++;
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__list_from(head)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END d47dfaebf17af9c9 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 221851141ae75f99 */
__v_result_blip__Value prims__prim_vector(Array args) {
	Array items = array_new(	sizeof(blip__Value), 0, 0);
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < args.len; __for_idx_0++) {
			blip__Value a = *(blip__Value*)(array_get(args, __for_idx_0));
			blip__Value __arr_val_1 = a;
			array_push(&items, &__arr_val_1);
		}
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__vector(items)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 221851141ae75f99 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN cf0adbf833a99016 */
__v_result_blip__Value prims__prim_vector_append(Array args) {
	Array out = array_new(	sizeof(blip__Value), 0, 0);
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < args.len; __for_idx_0++) {
			blip__Value a = *(blip__Value*)(array_get(args, __for_idx_0));
			__v_result_blip__Vectorptr __or_opt_1 = prims__as_vector_value(a, _v3_lit_13_d9cb45d4644cb55f);
			blip__Vector* __or_val_2 = {0};
			if (__or_opt_1.ok) {
				__or_val_2 = __or_opt_1.value;
			} else {
				return (__v_result_blip__Value){.ok = false, .err = __or_opt_1.err};
			}
			Array d = __or_val_2->data;
			{
				i64 __for_idx_3 = 0;
				for (; __for_idx_3 < d.len; __for_idx_3++) {
					blip__Value x = *(blip__Value*)(array_get(d, __for_idx_3));
					blip__Value __arr_val_4 = x;
					array_push(&out, &__arr_val_4);
				}
			}
		}
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__vector(out)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END cf0adbf833a99016 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 847b790a79a06cb1 */
__v_result_blip__Value prims__prim_vector_containsp(Array args) {
	__v_result_blip__Vectorptr __or_opt_0 = prims__as_vector_value((*(blip__Value*)array_get(args, 0)), _v3_lit_16_d4090a67ad28683);
	blip__Vector* __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	Array data = __or_val_1->data;
	i64 i = 0;
	while (i < data.len) {
		if (prims__value_eq((*(blip__Value*)array_get(data, i)), (*(blip__Value*)array_get(args, 1)))) {
			return (__v_result_blip__Value){.ok = true, .value = blip__boolean(true)};
		}
		i++;
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(false)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 847b790a79a06cb1 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 9a4926fd51fed41b */
__v_result_blip__Value prims__prim_vector_emptyp(Array args) {
	__v_result_blip__Vectorptr __or_opt_0 = prims__as_vector_value((*(blip__Value*)array_get(args, 0)), _v3_lit_13_de3bb69e93d86c41);
	blip__Vector* __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(__or_val_1->data.len == 0)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 9a4926fd51fed41b */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN bece8ebd57fd17da */
__v_result_blip__Value prims__prim_vector_length(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = blip__integer((blip__Value__as_vector((*(blip__Value*)array_get(args, 0))))->data.len)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END bece8ebd57fd17da */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 13a9916dfb1c8ee3 */
__v_result_blip__Value prims__prim_vector_ref(Array args) {
	__v_result_i64 __or_opt_0 = prims__need_int(_v3_lit_10_413a1a4c36a20cb8, (*(blip__Value*)array_get(args, 1)));
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	i64 i = __or_val_1;
	Array data = (blip__Value__as_vector((*(blip__Value*)array_get(args, 0))))->data;
	if ((i < 0) || (i >= data.len)) {
		return (__v_result_blip__Value){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = ({ string __v3_internal_symbol_join_0[5]; __v3_internal_symbol_join_0[0] = _v3_lit_18_bb3b2b7df22a85f2; __v3_internal_symbol_join_0[1] = i64__str(i); __v3_internal_symbol_join_0[2] = _v3_lit_22_ef0a88c1130ba183; __v3_internal_symbol_join_0[3] = int__str(data.len); __v3_internal_symbol_join_0[4] = _v3_lit_1_44bd52d473cd39de; string_plus_many(5, __v3_internal_symbol_join_0); }), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	return (__v_result_blip__Value){.ok = true, .value = (*(blip__Value*)array_get(data, i))};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 13a9916dfb1c8ee3 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 1373916dfaeeff73 */
__v_result_blip__Value prims__prim_vector_rev(Array args) {
	__v_result_blip__Vectorptr __or_opt_0 = prims__as_vector_value((*(blip__Value*)array_get(args, 0)), _v3_lit_10_413a2a4c36a227e8);
	blip__Vector* __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	Array data = __or_val_1->data;
	Array out = array_new(	sizeof(blip__Value), 0, 0);
	i64 i = data.len - 1;
	while (i >= 0) {
		blip__Value __arr_val_2 = (*(blip__Value*)array_get(data, i));
		array_push(&out, &__arr_val_2);
		i--;
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__vector(out)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 1373916dfaeeff73 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN d51d2b29b5d03661 */
__v_result_blip__Value prims__prim_vectorp(Array args) {
	return (__v_result_blip__Value){.ok = true, .value = prims__tag_is((*(blip__Value*)array_get(args, 0)), 1024)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END d51d2b29b5d03661 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN c8901af45c3fcb7a */
__v_result_blip__Value prims__prim_zero(Array args) {
	__v_result_i64 __or_opt_0 = prims__compare((*(blip__Value*)array_get(args, 0)), blip__integer(0));
	i64 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_result_blip__Value){.ok = false, .err = __or_opt_0.err};
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__boolean(__or_val_1 == 0)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END c8901af45c3fcb7a */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN a0aa414116827c37 */
void prims__register_gen_math(map* p) {
	string __map_key_0 = _v3_lit_8_27a5590a3bdfcf8b;
	_fn_ptr_d85a1a99927adfac __map_val_1 = prims__prim_gen_math_cos;
	map__set(p, &__map_key_0, &__map_val_1);
	string __map_key_2 = _v3_lit_8_398ab20a462c6337;
	_fn_ptr_d85a1a99927adfac __map_val_3 = prims__prim_gen_math_exp;
	map__set(p, &__map_key_2, &__map_val_3);
	string __map_key_4 = _v3_lit_14_afe623133454a3db;
	_fn_ptr_d85a1a99927adfac __map_val_5 = prims__prim_gen_math_factorial;
	map__set(p, &__map_key_4, &__map_val_5);
	string __map_key_6 = _v3_lit_10_e83816589e31d81e;
	_fn_ptr_d85a1a99927adfac __map_val_7 = prims__prim_gen_math_floor;
	map__set(p, &__map_key_6, &__map_val_7);
	string __map_key_8 = _v3_lit_8_e9de4e0a188ed4fa;
	_fn_ptr_d85a1a99927adfac __map_val_9 = prims__prim_gen_math_log;
	map__set(p, &__map_key_8, &__map_val_9);
	string __map_key_10 = _v3_lit_10_970b56823f25c3f3;
	_fn_ptr_d85a1a99927adfac __map_val_11 = prims__prim_gen_math_log10;
	map__set(p, &__map_key_10, &__map_val_11);
	string __map_key_12 = _v3_lit_9_f3936327bab38fd8;
	_fn_ptr_d85a1a99927adfac __map_val_13 = prims__prim_gen_math_log2;
	map__set(p, &__map_key_12, &__map_val_13);
	string __map_key_14 = _v3_lit_8_b075f40a88dd5902;
	_fn_ptr_d85a1a99927adfac __map_val_15 = prims__prim_gen_math_sin;
	map__set(p, &__map_key_14, &__map_val_15);
}

/* V3CACHE_FN_END a0aa414116827c37 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 983e73c09a630393 */
string prims__rune_bytes(i64 cp) {
	Array buf = __new_array_noscan(0, 0, sizeof(u8));
	u8 __arr_val_0 = (u8)(cp);
	array_push(&buf, &__arr_val_0);
	return Array_u8__bytestr(buf);
}

/* V3CACHE_FN_END 983e73c09a630393 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 7da7b1db83c5ce2e */
__v_result_Array prims__seq(blip__Value v) {
	{
		int __match_tmp_0 = v.tag;
		if (__match_tmp_0 == 256) {
			return prims__list_slice(v);
		} else if ((__match_tmp_0 == 512) || (__match_tmp_0 == 1)) {
			return (__v_result_Array){.ok = true, .value = array_new(			sizeof(blip__Value), 0, 0)};
		} else if ((__match_tmp_0 == 1024) || (__match_tmp_0 == 2048)) {
			return (__v_result_Array){.ok = true, .value = array__clone(&(blip__Value__as_vector(v))->data)};
		} else {
			return (__v_result_Array){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_25_a047628c58e9d373, printer__write(v)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
		}
	}
	return (__v_result_Array){.ok = true};
}

/* V3CACHE_FN_END 7da7b1db83c5ce2e */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 129338e9da10014e */
blip__Value prims__seq_like(blip__Value src, Array items) {
	if (src.tag == 2048) {
		blip__Vector* __iface_src_0 = (blip__Vector*)memdup(&(blip__Vector){.tag = 2048, .data = items}, sizeof(blip__Vector));
		return (blip__Value){.tag = 2048, .payload = (blip__Payload){._typ = 1742209763, ._object = __iface_src_0}};
	}
	if (src.tag == 1024) {
		return blip__vector(items);
	}
	return blip__list_from(items);
}

/* V3CACHE_FN_END 129338e9da10014e */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN f5ccb710ef7a134 */
string prims__show(blip__Value a, bool quote_strings) {
	if ((a.tag == 32) && !quote_strings) {
		return blip__Value__as_string(a);
	}
	return printer__write(a);
}

/* V3CACHE_FN_END f5ccb710ef7a134 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 24c8d17a1a441b6c */
__v_result_blip__Value prims__sort_by_keyed(Array items, Array keys) {
	Array order = __new_array_noscan(0, 0, sizeof(i64));
	i64 i = 0;
	while (i < items.len) {
		i64 __arr_val_0 = i;
		array_push(&order, &__arr_val_0);
		i++;
	}
	i64 a = 0;
	while (a < order.len) {
		i64 b = a;
		while ((b > 0) && ({__v_result_i64 __or_opt_1 = prims__compare((*(blip__Value*)array_get(keys, (*(i64*)array_get(order, b - 1)))), (*(blip__Value*)array_get(keys, (*(i64*)array_get(order, b)))));
		i64 __or_val_2 = {0};
		if (__or_opt_1.ok) {
			__or_val_2 = __or_opt_1.value;
		} else {
			return (__v_result_blip__Value){.ok = false, .err = __or_opt_1.err};
		}
		__or_val_2 > 0;})) {
			i64 tmp = (*(i64*)array_get(order, b - 1));
			{ Array* __v3_internal_symbol_scalar_store_base_0 = &order; int __v3_internal_symbol_scalar_store_index_0 = b - 1; i64 __v3_internal_symbol_scalar_store_value_0 = (*(i64*)array_get(order, b)); if (__v3_internal_symbol_scalar_store_index_0 >= 0 && __v3_internal_symbol_scalar_store_index_0 < __v3_internal_symbol_scalar_store_base_0->len) { ((i64*)__v3_internal_symbol_scalar_store_base_0->data)[__v3_internal_symbol_scalar_store_index_0] = __v3_internal_symbol_scalar_store_value_0; } else { array__set(__v3_internal_symbol_scalar_store_base_0, __v3_internal_symbol_scalar_store_index_0, &__v3_internal_symbol_scalar_store_value_0); } }
			{ Array* __v3_internal_symbol_scalar_store_base_1 = &order; int __v3_internal_symbol_scalar_store_index_1 = b; i64 __v3_internal_symbol_scalar_store_value_1 = tmp; if (__v3_internal_symbol_scalar_store_index_1 >= 0 && __v3_internal_symbol_scalar_store_index_1 < __v3_internal_symbol_scalar_store_base_1->len) { ((i64*)__v3_internal_symbol_scalar_store_base_1->data)[__v3_internal_symbol_scalar_store_index_1] = __v3_internal_symbol_scalar_store_value_1; } else { array__set(__v3_internal_symbol_scalar_store_base_1, __v3_internal_symbol_scalar_store_index_1, &__v3_internal_symbol_scalar_store_value_1); } }
			b--;
		}
		a++;
	}
	Array out = array_new(	sizeof(blip__Value), 0, 0);
	{
		i64 __for_idx_3 = 0;
		for (; __for_idx_3 < order.len; __for_idx_3++) {
			i64 idx = *(i64*)(array_get(order, __for_idx_3));
			blip__Value __arr_val_4 = (*(blip__Value*)array_get(items, idx));
			array_push(&out, &__arr_val_4);
		}
	}
	return (__v_result_blip__Value){.ok = true, .value = blip__list_from(out)};
	return (__v_result_blip__Value){.ok = true};
}

/* V3CACHE_FN_END 24c8d17a1a441b6c */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN 28560d21215c1c57 */
map prims__table(void) {
	map p = new_map(sizeof(string), sizeof(_fn_ptr_d85a1a99927adfac), map_hash_string, map_eq_string, map_clone_string, map_free_string);
	string __map_key_0 = _v3_lit_1_44bd50d473cd3678;
	_fn_ptr_d85a1a99927adfac __map_val_1 = prims__prim_add;
	map__set(&p, &__map_key_0, &__map_val_1);
	string __map_key_2 = _v3_lit_1_44bd56d473cd40aa;
	_fn_ptr_d85a1a99927adfac __map_val_3 = prims__prim_sub;
	map__set(&p, &__map_key_2, &__map_val_3);
	string __map_key_4 = _v3_lit_1_44bd51d473cd382b;
	_fn_ptr_d85a1a99927adfac __map_val_5 = prims__prim_mul;
	map__set(&p, &__map_key_4, &__map_val_5);
	string __map_key_6 = _v3_lit_1_44bd54d473cd3d44;
	_fn_ptr_d85a1a99927adfac __map_val_7 = prims__prim_div;
	map__set(&p, &__map_key_6, &__map_val_7);
	string __map_key_8 = _v3_lit_1_44bd66d473cd5bda;
	_fn_ptr_d85a1a99927adfac __map_val_9 = prims__prim_eq;
	map__set(&p, &__map_key_8, &__map_val_9);
	string __map_key_10 = _v3_lit_1_44bd67d473cd5d8d;
	_fn_ptr_d85a1a99927adfac __map_val_11 = prims__prim_lt;
	map__set(&p, &__map_key_10, &__map_val_11);
	string __map_key_12 = _v3_lit_1_44bd65d473cd5a27;
	_fn_ptr_d85a1a99927adfac __map_val_13 = prims__prim_gt;
	map__set(&p, &__map_key_12, &__map_val_13);
	string __map_key_14 = _v3_lit_2_9b351e00c5f63210;
	_fn_ptr_d85a1a99927adfac __map_val_15 = prims__prim_le;
	map__set(&p, &__map_key_14, &__map_val_15);
	string __map_key_16 = _v3_lit_2_9b2e2200c5f01a2e;
	_fn_ptr_d85a1a99927adfac __map_val_17 = prims__prim_ge;
	map__set(&p, &__map_key_16, &__map_val_17);
	string __map_key_18 = _v3_lit_3_3fe1cd514301d2b4;
	_fn_ptr_d85a1a99927adfac __map_val_19 = prims__prim_not;
	map__set(&p, &__map_key_18, &__map_val_19);
	string __map_key_20 = _v3_lit_3_cf45a85103385d33;
	_fn_ptr_d85a1a99927adfac __map_val_21 = prims__prim_car;
	map__set(&p, &__map_key_20, &__map_val_21);
	string __map_key_22 = _v3_lit_3_cf50be5103428a1a;
	_fn_ptr_d85a1a99927adfac __map_val_23 = prims__prim_cdr;
	map__set(&p, &__map_key_22, &__map_val_23);
	string __map_key_24 = _v3_lit_4_e6cd8fa8beffb8ce;
	_fn_ptr_d85a1a99927adfac __map_val_25 = prims__prim_cons;
	map__set(&p, &__map_key_24, &__map_val_25);
	string __map_key_26 = _v3_lit_4_a1c80526911571eb;
	_fn_ptr_d85a1a99927adfac __map_val_27 = prims__prim_list;
	map__set(&p, &__map_key_26, &__map_val_27);
	string __map_key_28 = _v3_lit_6_e34931c25b6d0e4b;
	_fn_ptr_d85a1a99927adfac __map_val_29 = prims__prim_length;
	map__set(&p, &__map_key_28, &__map_val_29);
	string __map_key_30 = _v3_lit_7_780c2660960ff787;
	_fn_ptr_d85a1a99927adfac __map_val_31 = prims__prim_reverse;
	map__set(&p, &__map_key_30, &__map_val_31);
	string __map_key_32 = _v3_lit_6_c4049a7f2a5192d3;
	_fn_ptr_d85a1a99927adfac __map_val_33 = prims__prim_append;
	map__set(&p, &__map_key_32, &__map_val_33);
	string __map_key_34 = _v3_lit_5_ccae00b90da4706b;
	_fn_ptr_d85a1a99927adfac __map_val_35 = prims__prim_nullp;
	map__set(&p, &__map_key_34, &__map_val_35);
	string __map_key_36 = _v3_lit_4_c1506214f88ae147;
	_fn_ptr_d85a1a99927adfac __map_val_37 = prims__prim_nilp;
	map__set(&p, &__map_key_36, &__map_val_37);
	string __map_key_38 = _v3_lit_5_674f3934a3e137f4;
	_fn_ptr_d85a1a99927adfac __map_val_39 = prims__prim_pairp;
	map__set(&p, &__map_key_38, &__map_val_39);
	string __map_key_40 = _v3_lit_5_fc52948887706b3c;
	_fn_ptr_d85a1a99927adfac __map_val_41 = prims__prim_listp;
	map__set(&p, &__map_key_40, &__map_val_41);
	string __map_key_42 = _v3_lit_7_cd118c208a458ced;
	_fn_ptr_d85a1a99927adfac __map_val_43 = prims__prim_vectorp;
	map__set(&p, &__map_key_42, &__map_val_43);
	string __map_key_44 = _v3_lit_6_1138efb0bf19762e;
	_fn_ptr_d85a1a99927adfac __map_val_45 = prims__prim_tablep;
	map__set(&p, &__map_key_44, &__map_val_45);
	string __map_key_46 = _v3_lit_6_b83d802db439c705;
	_fn_ptr_d85a1a99927adfac __map_val_47 = prims__prim_arrayp;
	map__set(&p, &__map_key_46, &__map_val_47);
	string __map_key_48 = _v3_lit_7_3200f030b0cdbd5b;
	_fn_ptr_d85a1a99927adfac __map_val_49 = prims__prim_stringp;
	map__set(&p, &__map_key_48, &__map_val_49);
	string __map_key_50 = _v3_lit_7_e600cf5bbfb5be64;
	_fn_ptr_d85a1a99927adfac __map_val_51 = prims__prim_symbolp;
	map__set(&p, &__map_key_50, &__map_val_51);
	string __map_key_52 = _v3_lit_8_26c547965b8b7753;
	_fn_ptr_d85a1a99927adfac __map_val_53 = prims__prim_keywordp;
	map__set(&p, &__map_key_52, &__map_val_53);
	string __map_key_54 = _v3_lit_8_3c2942aa0a336e44;
	_fn_ptr_d85a1a99927adfac __map_val_55 = prims__prim_integerp;
	map__set(&p, &__map_key_54, &__map_val_55);
	string __map_key_56 = _v3_lit_6_3dfb6893450ba188;
	_fn_ptr_d85a1a99927adfac __map_val_57 = prims__prim_floatp;
	map__set(&p, &__map_key_56, &__map_val_57);
	string __map_key_58 = _v3_lit_7_1917d0780d948be7;
	_fn_ptr_d85a1a99927adfac __map_val_59 = prims__prim_numberp;
	map__set(&p, &__map_key_58, &__map_val_59);
	string __map_key_60 = _v3_lit_6_a652a2d3592ea6e0;
	_fn_ptr_d85a1a99927adfac __map_val_61 = prims__prim_vector;
	map__set(&p, &__map_key_60, &__map_val_61);
	string __map_key_62 = _v3_lit_10_413a1a4c36a20cb8;
	_fn_ptr_d85a1a99927adfac __map_val_63 = prims__prim_vector_ref;
	map__set(&p, &__map_key_62, &__map_val_63);
	string __map_key_64 = _v3_lit_13_e95a08079b20c597;
	_fn_ptr_d85a1a99927adfac __map_val_65 = prims__prim_vector_length;
	map__set(&p, &__map_key_64, &__map_val_65);
	string __map_key_66 = _v3_lit_3_f02b6251155be40d;
	_fn_ptr_d85a1a99927adfac __map_val_67 = prims__prim_get;
	map__set(&p, &__map_key_66, &__map_val_67);
	string __map_key_68 = _v3_lit_8_138cdc6d5832847c;
	_fn_ptr_d85a1a99927adfac __map_val_69 = prims__prim_has_key;
	map__set(&p, &__map_key_68, &__map_val_69);
	string __map_key_70 = _v3_lit_3_7264b3515f33eb20;
	_fn_ptr_d85a1a99927adfac __map_val_71 = prims__prim_put;
	map__set(&p, &__map_key_70, &__map_val_71);
	string __map_key_72 = _v3_lit_6_13e8210fb3d5dc0f;
	_fn_ptr_d85a1a99927adfac __map_val_73 = prims__prim_remove;
	map__set(&p, &__map_key_72, &__map_val_73);
	string __map_key_74 = _v3_lit_10_737a68d3e82b9fdc;
	_fn_ptr_d85a1a99927adfac __map_val_75 = prims__prim_table_keys;
	map__set(&p, &__map_key_74, &__map_val_75);
	string __map_key_76 = _v3_lit_13_c2dbf1d17ad22d5;
	_fn_ptr_d85a1a99927adfac __map_val_77 = prims__prim_string_append;
	map__set(&p, &__map_key_76, &__map_val_77);
	string __map_key_78 = _v3_lit_13_1cc9ab73c965e0a9;
	_fn_ptr_d85a1a99927adfac __map_val_79 = prims__prim_string_length;
	map__set(&p, &__map_key_78, &__map_val_79);
	string __map_key_80 = _v3_lit_15_74b513a653904005;
	_fn_ptr_d85a1a99927adfac __map_val_81 = prims__prim_string_downcase;
	map__set(&p, &__map_key_80, &__map_val_81);
	string __map_key_82 = _v3_lit_13_c9486c3e7201d588;
	_fn_ptr_d85a1a99927adfac __map_val_83 = prims__prim_string_upcase;
	map__set(&p, &__map_key_82, &__map_val_83);
	string __map_key_84 = _v3_lit_12_9e98aa3ad92dd337;
	_fn_ptr_d85a1a99927adfac __map_val_85 = prims__prim_string_split;
	map__set(&p, &__map_key_84, &__map_val_85);
	string __map_key_86 = _v3_lit_11_bac2bbe2efd88755;
	_fn_ptr_d85a1a99927adfac __map_val_87 = prims__prim_string_trim;
	map__set(&p, &__map_key_86, &__map_val_87);
	string __map_key_88 = _v3_lit_13_bade188409f1efde;
	_fn_ptr_d85a1a99927adfac __map_val_89 = prims__prim_string_blank;
	map__set(&p, &__map_key_88, &__map_val_89);
	string __map_key_90 = _v3_lit_6_3850125edc81f186;
	_fn_ptr_d85a1a99927adfac __map_val_91 = prims__prim_string;
	map__set(&p, &__map_key_90, &__map_val_91);
	string __map_key_92 = _v3_lit_6_ee267171343aae33;
	_fn_ptr_d85a1a99927adfac __map_val_93 = prims__prim_symbol;
	map__set(&p, &__map_key_92, &__map_val_93);
	string __map_key_94 = _v3_lit_7_6cd5b4518f37a3de;
	_fn_ptr_d85a1a99927adfac __map_val_95 = prims__prim_keyword;
	map__set(&p, &__map_key_94, &__map_val_95);
	string __map_key_96 = _v3_lit_14_80ba372c0e6c3078;
	_fn_ptr_d85a1a99927adfac __map_val_97 = prims__prim_string_to_number;
	map__set(&p, &__map_key_96, &__map_val_97);
	string __map_key_98 = _v3_lit_14_6fc210c22fa1fe34;
	_fn_ptr_d85a1a99927adfac __map_val_99 = prims__prim_number_to_string;
	map__set(&p, &__map_key_98, &__map_val_99);
	string __map_key_100 = _v3_lit_5_ceff5b955e05cd1e;
	_fn_ptr_d85a1a99927adfac __map_val_101 = prims__prim_zero;
	map__set(&p, &__map_key_100, &__map_val_101);
	string __map_key_102 = _v3_lit_5_9960f1b44cf39360;
	_fn_ptr_d85a1a99927adfac __map_val_103 = prims__prim_even;
	map__set(&p, &__map_key_102, &__map_val_103);
	string __map_key_104 = _v3_lit_4_ec87890da6744229;
	_fn_ptr_d85a1a99927adfac __map_val_105 = prims__prim_odd;
	map__set(&p, &__map_key_104, &__map_val_105);
	string __map_key_106 = _v3_lit_9_4fa1b00d71d70d01;
	_fn_ptr_d85a1a99927adfac __map_val_107 = prims__prim_positive;
	map__set(&p, &__map_key_106, &__map_val_107);
	string __map_key_108 = _v3_lit_9_703f5f434a1181a9;
	_fn_ptr_d85a1a99927adfac __map_val_109 = prims__prim_negative;
	map__set(&p, &__map_key_108, &__map_val_109);
	string __map_key_110 = _v3_lit_3_46bd02514660fe35;
	_fn_ptr_d85a1a99927adfac __map_val_111 = prims__prim_min;
	map__set(&p, &__map_key_110, &__map_val_111);
	string __map_key_112 = _v3_lit_3_46d7ec514677a08b;
	_fn_ptr_d85a1a99927adfac __map_val_113 = prims__prim_max;
	map__set(&p, &__map_key_112, &__map_val_113);
	string __map_key_114 = _v3_lit_3_e16811510db8ba2d;
	_fn_ptr_d85a1a99927adfac __map_val_115 = prims__prim_abs;
	map__set(&p, &__map_key_114, &__map_val_115);
	string __map_key_116 = _v3_lit_3_58d84e5150db0b3a;
	_fn_ptr_d85a1a99927adfac __map_val_117 = prims__prim_str;
	map__set(&p, &__map_key_116, &__map_val_117);
	string __map_key_118 = _v3_lit_8_b823e33ad085a91b;
	_fn_ptr_d85a1a99927adfac __map_val_119 = prims__prim_list_ref;
	map__set(&p, &__map_key_118, &__map_val_119);
	string __map_key_120 = _v3_lit_4_bbcbbc682f613010;
	_fn_ptr_d85a1a99927adfac __map_val_121 = prims__prim_take;
	map__set(&p, &__map_key_120, &__map_val_121);
	string __map_key_122 = _v3_lit_4_8b35ade463598efa;
	_fn_ptr_d85a1a99927adfac __map_val_123 = prims__prim_drop;
	map__set(&p, &__map_key_122, &__map_val_123);
	string __map_key_124 = _v3_lit_5_65c222e025703097;
	_fn_ptr_d85a1a99927adfac __map_val_125 = prims__prim_car;
	map__set(&p, &__map_key_124, &__map_val_125);
	string __map_key_126 = _v3_lit_4_47ee913776e3c785;
	_fn_ptr_d85a1a99927adfac __map_val_127 = prims__prim_cdr;
	map__set(&p, &__map_key_126, &__map_val_127);
	string __map_key_128 = _v3_lit_4_e63a3d26b79c66a3;
	_fn_ptr_d85a1a99927adfac __map_val_129 = prims__prim_last;
	map__set(&p, &__map_key_128, &__map_val_129);
	string __map_key_130 = _v3_lit_4_8e826a14dc18becb;
	_fn_ptr_d85a1a99927adfac __map_val_131 = prims__prim_not_eq;
	map__set(&p, &__map_key_130, &__map_val_131);
	string __map_key_132 = _v3_lit_3_22ee9511fc1c372;
	_fn_ptr_d85a1a99927adfac __map_val_133 = prims__prim_eqp;
	map__set(&p, &__map_key_132, &__map_val_133);
	string __map_key_134 = _v3_lit_3_268b8d5134d6c11d;
	_fn_ptr_d85a1a99927adfac __map_val_135 = prims__prim_in;
	map__set(&p, &__map_key_134, &__map_val_135);
	string __map_key_136 = _v3_lit_6_e15e17627c0748db;
	_fn_ptr_d85a1a99927adfac __map_val_137 = prims__prim_concat;
	map__set(&p, &__map_key_136, &__map_val_137);
	string __map_key_138 = _v3_lit_5_96f5e4207ce9b718;
	_fn_ptr_d85a1a99927adfac __map_val_139 = prims__prim_range;
	map__set(&p, &__map_key_138, &__map_val_139);
	string __map_key_140 = _v3_lit_5_cdc802bc54e68fc2;
	_fn_ptr_d85a1a99927adfac __map_val_141 = prims__prim_count;
	map__set(&p, &__map_key_140, &__map_val_141);
	string __map_key_142 = _v3_lit_4_bd51b32ce96497db;
	_fn_ptr_d85a1a99927adfac __map_val_143 = prims__prim_sort;
	map__set(&p, &__map_key_142, &__map_val_143);
	string __map_key_144 = _v3_lit_11_c464b6e9ce3d5a2b;
	_fn_ptr_d85a1a99927adfac __map_val_145 = prims__prim_frequencies;
	map__set(&p, &__map_key_144, &__map_val_145);
	string __map_key_146 = _v3_lit_10_413a2a4c36a227e8;
	_fn_ptr_d85a1a99927adfac __map_val_147 = prims__prim_vector_rev;
	map__set(&p, &__map_key_146, &__map_val_147);
	string __map_key_148 = _v3_lit_13_d9cb45d4644cb55f;
	_fn_ptr_d85a1a99927adfac __map_val_149 = prims__prim_vector_append;
	map__set(&p, &__map_key_148, &__map_val_149);
	string __map_key_150 = _v3_lit_13_de3bb69e93d86c41;
	_fn_ptr_d85a1a99927adfac __map_val_151 = prims__prim_vector_emptyp;
	map__set(&p, &__map_key_150, &__map_val_151);
	string __map_key_152 = _v3_lit_16_d4090a67ad28683;
	_fn_ptr_d85a1a99927adfac __map_val_153 = prims__prim_vector_containsp;
	map__set(&p, &__map_key_152, &__map_val_153);
	string __map_key_154 = _v3_lit_10_c9c964e9f2b280dc;
	_fn_ptr_d85a1a99927adfac __map_val_155 = prims__prim_array_set;
	map__set(&p, &__map_key_154, &__map_val_155);
	string __map_key_156 = _v3_lit_9_e763840bbe806fc0;
	_fn_ptr_d85a1a99927adfac __map_val_157 = prims__prim_array_ref;
	map__set(&p, &__map_key_156, &__map_val_157);
	string __map_key_158 = _v3_lit_11_6b3ca7bc363d8f9e;
	_fn_ptr_d85a1a99927adfac __map_val_159 = prims__prim_array_push;
	map__set(&p, &__map_key_158, &__map_val_159);
	string __map_key_160 = _v3_lit_10_e608bb067e3196f9;
	_fn_ptr_d85a1a99927adfac __map_val_161 = prims__prim_array_pop;
	map__set(&p, &__map_key_160, &__map_val_161);
	string __map_key_162 = _v3_lit_12_2dcf4e18024c4ff;
	_fn_ptr_d85a1a99927adfac __map_val_163 = prims__prim_array_length;
	map__set(&p, &__map_key_162, &__map_val_163);
	string __map_key_164 = _v3_lit_12_b365cad16314ea29;
	_fn_ptr_d85a1a99927adfac __map_val_165 = prims__prim_array_emptyp;
	map__set(&p, &__map_key_164, &__map_val_165);
	string __map_key_166 = _v3_lit_4_9507b444c53852b3;
	_fn_ptr_d85a1a99927adfac __map_val_167 = prims__prim_put_bang;
	map__set(&p, &__map_key_166, &__map_val_167);
	string __map_key_168 = _v3_lit_7_14d41c1622031397;
	_fn_ptr_d85a1a99927adfac __map_val_169 = prims__prim_dissoc_bang;
	map__set(&p, &__map_key_168, &__map_val_169);
	string __map_key_170 = _v3_lit_15_7b8cf9c1a55e8d5f;
	_fn_ptr_d85a1a99927adfac __map_val_171 = prims__prim_string_index_of;
	map__set(&p, &__map_key_170, &__map_val_171);
	string __map_key_172 = _v3_lit_17_e361026ca9583b4e;
	_fn_ptr_d85a1a99927adfac __map_val_173 = prims__prim_string_not_blank;
	map__set(&p, &__map_key_172, &__map_val_173);
	string __map_key_174 = _v3_lit_16_35bef6f51e6ad421;
	_fn_ptr_d85a1a99927adfac __map_val_175 = prims__prim_string_containsp;
	map__set(&p, &__map_key_174, &__map_val_175);
	string __map_key_176 = _v3_lit_18_150f5d003c9d16b;
	_fn_ptr_d85a1a99927adfac __map_val_177 = prims__prim_string_start_withp;
	map__set(&p, &__map_key_176, &__map_val_177);
	string __map_key_178 = _v3_lit_11_84f1169aeef50035;
	_fn_ptr_d85a1a99927adfac __map_val_179 = prims__prim_string_join;
	map__set(&p, &__map_key_178, &__map_val_179);
	string __map_key_180 = _v3_lit_13_b200ce5844ed589e;
	_fn_ptr_d85a1a99927adfac __map_val_181 = prims__prim_string_repeat;
	map__set(&p, &__map_key_180, &__map_val_181);
	string __map_key_182 = _v3_lit_12_7c1279d672f973a5;
	_fn_ptr_d85a1a99927adfac __map_val_183 = prims__prim_string_slice;
	map__set(&p, &__map_key_182, &__map_val_183);
	string __map_key_184 = _v3_lit_14_81b652afad6b830f;
	_fn_ptr_d85a1a99927adfac __map_val_185 = prims__prim_string_replace;
	map__set(&p, &__map_key_184, &__map_val_185);
	string __map_key_186 = _v3_lit_14_68acb05b19e75529;
	_fn_ptr_d85a1a99927adfac __map_val_187 = prims__prim_string_to_symbol;
	map__set(&p, &__map_key_186, &__map_val_187);
	string __map_key_188 = _v3_lit_15_eb9647638504e0dc;
	_fn_ptr_d85a1a99927adfac __map_val_189 = prims__prim_string_to_keyword;
	map__set(&p, &__map_key_188, &__map_val_189);
	string __map_key_190 = _v3_lit_8_f9a8cb4c8fdf71a;
	_fn_ptr_d85a1a99927adfac __map_val_191 = prims__prim_booleanp;
	map__set(&p, &__map_key_190, &__map_val_191);
	string __map_key_192 = _v3_lit_5_5c8f2ec760f92884;
	_fn_ptr_d85a1a99927adfac __map_val_193 = prims__prim_runep;
	map__set(&p, &__map_key_192, &__map_val_193);
	string __map_key_194 = _v3_lit_7_460556b70604cd06;
	_fn_ptr_d85a1a99927adfac __map_val_195 = prims__prim_fntionp;
	map__set(&p, &__map_key_194, &__map_val_195);
	string __map_key_196 = _v3_lit_10_e4092a7435a3d745;
	_fn_ptr_d85a1a99927adfac __map_val_197 = prims__prim_fntionp;
	map__set(&p, &__map_key_196, &__map_val_197);
	string __map_key_198 = _v3_lit_6_791c3c9e9fbc336d;
	_fn_ptr_d85a1a99927adfac __map_val_199 = prims__prim_modulo;
	map__set(&p, &__map_key_198, &__map_val_199);
	string __map_key_200 = _v3_lit_8_f651b3be55260426;
	_fn_ptr_d85a1a99927adfac __map_val_201 = prims__prim_quotient;
	map__set(&p, &__map_key_200, &__map_val_201);
	string __map_key_202 = _v3_lit_9_15e2e1252626de9e;
	_fn_ptr_d85a1a99927adfac __map_val_203 = prims__prim_remainder;
	map__set(&p, &__map_key_202, &__map_val_203);
	string __map_key_204 = _v3_lit_3_f03f6e51156ca17b;
	_fn_ptr_d85a1a99927adfac __map_val_205 = prims__prim_gcd;
	map__set(&p, &__map_key_204, &__map_val_205);
	string __map_key_206 = _v3_lit_4_b426a9d918a0133a;
	_fn_ptr_d85a1a99927adfac __map_val_207 = prims__prim_expt;
	map__set(&p, &__map_key_206, &__map_val_207);
	string __map_key_208 = _v3_lit_4_b8fad92c55cca545;
	_fn_ptr_d85a1a99927adfac __map_val_209 = prims__prim_sqrt;
	map__set(&p, &__map_key_208, &__map_val_209);
	string __map_key_210 = _v3_lit_4_4973567a2a000355;
	_fn_ptr_d85a1a99927adfac __map_val_211 = prims__prim_zero;
	map__set(&p, &__map_key_210, &__map_val_211);
	string __map_key_212 = _v3_lit_3_268b515134d65b29;
	_fn_ptr_d85a1a99927adfac __map_val_213 = prims__prim_inc;
	map__set(&p, &__map_key_212, &__map_val_213);
	string __map_key_214 = _v3_lit_3_cadb6512638c859;
	_fn_ptr_d85a1a99927adfac __map_val_215 = prims__prim_dec;
	map__set(&p, &__map_key_214, &__map_val_215);
	string __map_key_216 = _v3_lit_7_a0bbd30e2bdc480f;
	_fn_ptr_d85a1a99927adfac __map_val_217 = prims__prim_structp;
	map__set(&p, &__map_key_216, &__map_val_217);
	string __map_key_218 = _v3_lit_11_c29ca55ac7deafa6;
	_fn_ptr_d85a1a99927adfac __map_val_219 = prims__prim_struct_name;
	map__set(&p, &__map_key_218, &__map_val_219);
	string __map_key_220 = _v3_lit_10_98c096c20686822;
	_fn_ptr_d85a1a99927adfac __map_val_221 = prims__prim_struct_ref;
	map__set(&p, &__map_key_220, &__map_val_221);
	string __map_key_222 = _v3_lit_11_819a09b4259b9006;
	_fn_ptr_d85a1a99927adfac __map_val_223 = prims__prim_struct_set;
	map__set(&p, &__map_key_222, &__map_val_223);
	string __map_key_224 = _v3_lit_16_fdd74acda8495128;
	_fn_ptr_d85a1a99927adfac __map_val_225 = prims__prim_struct_mutate;
	map__set(&p, &__map_key_224, &__map_val_225);
	string __map_key_226 = _v3_lit_13_ff3a948ac618be44;
	_fn_ptr_d85a1a99927adfac __map_val_227 = prims__prim_integer_to_rune;
	map__set(&p, &__map_key_226, &__map_val_227);
	string __map_key_228 = _v3_lit_13_33cef20612002766;
	_fn_ptr_d85a1a99927adfac __map_val_229 = prims__prim_rune_to_integer;
	map__set(&p, &__map_key_228, &__map_val_229);
	string __map_key_230 = _v3_lit_12_e8852f9b97575ce5;
	_fn_ptr_d85a1a99927adfac __map_val_231 = prims__prim_string_to_rune;
	map__set(&p, &__map_key_230, &__map_val_231);
	string __map_key_232 = _v3_lit_12_4a3428e3086109ec;
	_fn_ptr_d85a1a99927adfac __map_val_233 = prims__prim_code_to_string;
	map__set(&p, &__map_key_232, &__map_val_233);
	string __map_key_234 = _v3_lit_13_fcd81ef130279c17;
	_fn_ptr_d85a1a99927adfac __map_val_235 = prims__prim_make_struct;
	map__set(&p, &__map_key_234, &__map_val_235);
	prims__register_gen_math(&p);
	return p;
}

/* V3CACHE_FN_END 28560d21215c1c57 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN a6139ff809caf588 */
blip__Value prims__tag_is(blip__Value v, int t) {
	return blip__boolean(v.tag == t);
}

/* V3CACHE_FN_END a6139ff809caf588 */
/* V3CACHE_MODULE prims */
/* V3CACHE_FN_BEGIN e1655c913e2a26ef */
bool prims__value_eq(blip__Value a, blip__Value b) {
	if (a.tag != b.tag) {
		return false;
	}
	{
		int __match_tmp_0 = a.tag;
		if ((__match_tmp_0 == 4) || (__match_tmp_0 == 16) || (__match_tmp_0 == 2)) {
			return a.i == b.i;
		} else if (__match_tmp_0 == 8) {
			return a.f == b.f;
		} else if ((__match_tmp_0 == 1) || (__match_tmp_0 == 512)) {
			return true;
		} else if ((__match_tmp_0 == 32) || (__match_tmp_0 == 64) || (__match_tmp_0 == 128)) {
			return string__eq(blip__Value__as_string(a), blip__Value__as_string(b));
		} else if (__match_tmp_0 == 256) {
			blip__Value x = a;
			blip__Value y = b;
			while ((x.tag == 256) && (y.tag == 256)) {
				if (!prims__value_eq((blip__Value__as_pair(x))->car, (blip__Value__as_pair(y))->car)) {
					return false;
				}
				x = (blip__Value__as_pair(x))->cdr;
				y = (blip__Value__as_pair(y))->cdr;
			}
			return blip__Value__is_empty_seq(x) && blip__Value__is_empty_seq(y);
		} else if (__match_tmp_0 == 1024) {
			Array x = (blip__Value__as_vector(a))->data;
			Array y = (blip__Value__as_vector(b))->data;
			if (x.len != y.len) {
				return false;
			}
			i64 i = 0;
			while (i < x.len) {
				if (!prims__value_eq((*(blip__Value*)array_get(x, i)), (*(blip__Value*)array_get(y, i)))) {
					return false;
				}
				i++;
			}
			return true;
		} else if (__match_tmp_0 == 4096) {
			blip__Table* x = blip__Value__as_table(a);
			blip__Table* y = blip__Value__as_table(b);
			if (x->values.data->count != y->values.data->count) {
				return false;
			}
			for (int __mi_0 = 0; __mi_0 < (			x->values).data->key_values.len; __mi_0++) {
				if ((			x->values).data->key_values.all_deleted && (			x->values).data->key_values.all_deleted[__mi_0]) continue;
				string k = *(string*)((			x->values).data->key_values.keys + __mi_0 * (			x->values).data->key_values.key_bytes);
				k = string__clone(k);
				blip__Value v = *(blip__Value*)((			x->values).data->key_values.values + __mi_0 * (			x->values).data->key_values.value_bytes);
				bool found = false;
				for (int __mi_1 = 0; __mi_1 < (				y->values).data->key_values.len; __mi_1++) {
					if ((				y->values).data->key_values.all_deleted && (				y->values).data->key_values.all_deleted[__mi_1]) continue;
					string k2 = *(string*)((				y->values).data->key_values.keys + __mi_1 * (				y->values).data->key_values.key_bytes);
					k2 = string__clone(k2);
					blip__Value v2 = *(blip__Value*)((				y->values).data->key_values.values + __mi_1 * (				y->values).data->key_values.value_bytes);
					if (string__eq(k2, k)) {
						found = true;
						if (!prims__value_eq(v, v2)) {
							return false;
						}
					}
				}
				if (!found) {
					return false;
				}
			}
			return true;
		} else if (__match_tmp_0 == 131072) {
			blip__StructVal* x = blip__Value__as_struct(a);
			blip__StructVal* y = blip__Value__as_struct(b);
			if (!string__eq(x->name, y->name) || (x->fields.len != y->fields.len)) {
				return false;
			}
			i64 i = 0;
			while (i < x->fields.len) {
				if (!string__eq((*(string*)array_get(x->fields, i)), (*(string*)array_get(y->fields, i)))) {
					return false;
				}
				if (!prims__value_eq(blip__StructVal__at(x, i), blip__StructVal__at(y, i))) {
					return false;
				}
				i++;
			}
			return true;
		} else {
			return false;
		}
	}
}

/* V3CACHE_FN_END e1655c913e2a26ef */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN e1b62b31037bdb46 */
void print(string s) {
}

/* V3CACHE_FN_END e1b62b31037bdb46 */
/* V3CACHE_MODULE printer */
/* V3CACHE_FN_BEGIN d3c4d924eb2c2347 */
string printer__char_literal(i32 cp) {
	if ((cp < 0) || (cp > 0x10FFFF)) {
		return _v3_lit_9_7c8ef902fe4e32d7;
	}
	string __match_val_0 = _v3_lit_0_14650fb0739d0383;
	if (cp == 10) {
		__match_val_0 = _v3_lit_7_41566b4aaab421d9;
	} else if (cp == 32) {
		__match_val_0 = _v3_lit_5_a5593b4a24922a2f;
	} else if (cp == 9) {
		__match_val_0 = _v3_lit_3_96797d51740b73ba;
	} else if (cp == 13) {
		__match_val_0 = _v3_lit_6_7488c53f48adf9a1;
	} else if (cp == 0) {
		__match_val_0 = _v3_lit_4_cee237146f424a56;
	} else if (cp == 8) {
		__match_val_0 = _v3_lit_9_17f117cc78c576b2;
	} else if (cp == 27) {
		__match_val_0 = _v3_lit_6_5397a7f2858c1824;
	} else {
		__match_val_0 = _v3_lit_0_14650fb0739d0383;
	}
	string named = __match_val_0;
	if (!string__eq(named, _v3_lit_0_14650fb0739d0383)) {
		return string__plus(_v3_lit_2_9acb7d00c59c3774, named);
	}
	Array buf = __new_array_noscan(0, 0, sizeof(u8));
	u8 __arr_val_1 = (u8)(cp);
	array_push(&buf, &__arr_val_1);
	return string__plus(_v3_lit_2_9acb7d00c59c3774, Array_u8__bytestr(buf));
}

/* V3CACHE_FN_END d3c4d924eb2c2347 */
/* V3CACHE_MODULE printer */
/* V3CACHE_FN_BEGIN afc02c2a2fc74913 */
string printer__close_for(string v_open) {
	if (string__eq(v_open, _v3_lit_1_44bd53d473cd3b91)) {
		return _v3_lit_1_44bd52d473cd39de;
	} else if (string__eq(v_open, _v3_lit_1_44bd80d473cd8808)) {
		return _v3_lit_1_44bd86d473cd923a;
	} else if (string__eq(v_open, _v3_lit_1_44bda0d473cdbe68)) {
		return _v3_lit_1_44bda6d473cdc89a;
	} else if (string__eq(v_open, _v3_lit_2_9b423c00c600ef66)) {
		return _v3_lit_1_44bd86d473cd923a;
	} else if (string__eq(v_open, _v3_lit_2_9acb8900c59c4bd8)) {
		return _v3_lit_1_44bd52d473cd39de;
	} else {
		return _v3_lit_1_44bd52d473cd39de;
	}
}

/* V3CACHE_FN_END afc02c2a2fc74913 */
/* V3CACHE_MODULE printer */
/* V3CACHE_FN_BEGIN f586c254c0c272e3 */
string printer__quote_string(string s) {
	u8 bs = (u8)(92);
	u8 dq = (u8)(34);
	u8 nl = (u8)(10);
	u8 tab = (u8)(9);
	u8 cr = (u8)(13);
	Array out = __new_array_noscan(0, 0, sizeof(u8));
	u8 __arr_val_0 = dq;
	array_push(&out, &__arr_val_0);
	{
		i64 __for_idx_1 = 0;
		for (; __for_idx_1 < s.len; __for_idx_1++) {
			u8 ch = string__at(s, __for_idx_1);
			if (ch == dq) {
				u8 __arr_val_2 = bs;
				array_push(&out, &__arr_val_2);
				u8 __arr_val_3 = dq;
				array_push(&out, &__arr_val_3);
			} else if (ch == bs) {
				u8 __arr_val_4 = bs;
				array_push(&out, &__arr_val_4);
				u8 __arr_val_5 = bs;
				array_push(&out, &__arr_val_5);
			} else if (ch == nl) {
				u8 __arr_val_6 = bs;
				array_push(&out, &__arr_val_6);
				u8 __arr_val_7 = (u8)(110);
				array_push(&out, &__arr_val_7);
			} else if (ch == tab) {
				u8 __arr_val_8 = bs;
				array_push(&out, &__arr_val_8);
				u8 __arr_val_9 = (u8)(116);
				array_push(&out, &__arr_val_9);
			} else if (ch == cr) {
				u8 __arr_val_10 = bs;
				array_push(&out, &__arr_val_10);
				u8 __arr_val_11 = (u8)(114);
				array_push(&out, &__arr_val_11);
			} else {
				u8 __arr_val_12 = ch;
				array_push(&out, &__arr_val_12);
			}
		}
	}
	u8 __arr_val_13 = dq;
	array_push(&out, &__arr_val_13);
	return Array_u8__bytestr(out);
}

/* V3CACHE_FN_END f586c254c0c272e3 */
/* V3CACHE_MODULE printer */
/* V3CACHE_FN_BEGIN e41231a87f4be113 */
string printer__wrap(string v_open, reader__Arena* a, i32 id) {
	Array kids = reader__Arena__kids(a, id);
	Array parts = array_new(	sizeof(string), 0, 0);
	i64 ki = 0;
	while (ki < kids.len) {
		string __arr_val_0 = printer__write_datum(a, (*(i32*)array_get(kids, ki)));
		array_push(&parts, &__arr_val_0);
		ki++;
	}
	string out = v_open;
	i64 i = 0;
	while (i < parts.len) {
		if (i > 0) {
			out = string__plus(out, _v3_lit_1_44bd4bd473cd2df9);
		}
		out = string__plus(out, (*(string*)array_get(parts, i)));
		i++;
	}
	out = string__plus(out, printer__close_for(v_open));
	return out;
}

/* V3CACHE_FN_END e41231a87f4be113 */
/* V3CACHE_MODULE printer */
/* V3CACHE_FN_BEGIN d9884d5050438d06 */
string printer__wrap1(string prefix, reader__Arena* a, i32 id) {
	Array kids = reader__Arena__kids(a, id);
	if (kids.len == 0) {
		return prefix;
	}
	return string__plus(prefix, printer__write_datum(a, (*(i32*)array_get(kids, 0))));
}

/* V3CACHE_FN_END d9884d5050438d06 */
/* V3CACHE_MODULE printer */
/* V3CACHE_FN_BEGIN 10f1610e4eb4328e */
string printer__write(blip__Value v) {
	return printer__write_value(v, __new_array_noscan(0, 0, sizeof(u64)), new_map(sizeof(u64), sizeof(i64), map_hash_int_8, map_eq_int_8, map_clone_int_8, map_free_nop), 0);
}

/* V3CACHE_FN_END 10f1610e4eb4328e */
/* V3CACHE_MODULE printer */
/* V3CACHE_FN_BEGIN 115532686f35438e */
string printer__write_datum(reader__Arena* a, i32 id) {
	reader__Datum* d = reader__Arena__node(a, id);
	{
		reader__DatumTag __match_tmp_0 = d->tag;
		if (__match_tmp_0 == reader__DatumTag__nil) {
			return _v3_lit_3_3ff5d95143129022;
		} else if (__match_tmp_0 == reader__DatumTag__bool) {
			if (d->i != 0) {
				return _v3_lit_2_9acb5500c59bf37c;
			} else {
				return _v3_lit_2_9acb4700c59bdbb2;
			}
		} else if (__match_tmp_0 == reader__DatumTag__v_int) {
			return strconv__format_int(d->i, 10);
		} else if (__match_tmp_0 == reader__DatumTag__rat) {
			return ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = i64__str(d->i); __v3_internal_symbol_join_0[1] = _v3_lit_1_44bd54d473cd3d44; __v3_internal_symbol_join_0[2] = i64__str(d->den); string_plus_many(3, __v3_internal_symbol_join_0); });
		} else if (__match_tmp_0 == reader__DatumTag__v_float) {
			return f64__str(d->f);
		} else if (__match_tmp_0 == reader__DatumTag__v_char) {
			return printer__char_literal((i32)(d->i));
		} else if (__match_tmp_0 == reader__DatumTag__str) {
			return printer__quote_string(d->value);
		} else if (__match_tmp_0 == reader__DatumTag__sym) {
			return d->value;
		} else if (__match_tmp_0 == reader__DatumTag__kw) {
			return string__plus(_v3_lit_1_44bd61d473cd535b, d->value);
		} else if (__match_tmp_0 == reader__DatumTag__list) {
			return printer__wrap(_v3_lit_1_44bd53d473cd3b91, a, id);
		} else if (__match_tmp_0 == reader__DatumTag__vector) {
			return printer__wrap(_v3_lit_1_44bd80d473cd8808, a, id);
		} else if (__match_tmp_0 == reader__DatumTag__table) {
			return printer__wrap(_v3_lit_1_44bda0d473cdbe68, a, id);
		} else if (__match_tmp_0 == reader__DatumTag__array) {
			return printer__wrap(_v3_lit_2_9b423c00c600ef66, a, id);
		} else if (__match_tmp_0 == reader__DatumTag__bytes) {
			return printer__wrap(_v3_lit_2_9acb8900c59c4bd8, a, id);
		} else if (__match_tmp_0 == reader__DatumTag__quoted) {
			return printer__wrap1(_v3_lit_1_44bd4cd473cd2fac, a, id);
		} else if (__match_tmp_0 == reader__DatumTag__quasi) {
			return printer__wrap1(_v3_lit_1_44bd8bd473cd9ab9, a, id);
		} else if (__match_tmp_0 == reader__DatumTag__unquote) {
			return printer__wrap1(_v3_lit_1_44bd57d473cd425d, a, id);
		} else if (__match_tmp_0 == reader__DatumTag__unquote_splice) {
			return printer__wrap1(_v3_lit_2_9afe5b00c5c75747, a, id);
		} else {
			return _v3_lit_8_5ad82412f08c148b;
		}
	}
}

/* V3CACHE_FN_END 115532686f35438e */
/* V3CACHE_MODULE printer */
/* V3CACHE_FN_BEGIN 1049fe7724a2621f */
string printer__write_list(blip__Value v, Array active, map seen, i64 depth) {
	Array path = __new_array_noscan(0, active.len + 1, sizeof(u64));
	array__push_many(&path, active.data, active.len);
	Array parts = array_new(	sizeof(string), 0, 0);
	blip__Value cur = v;
	while (cur.tag == 256) {
		string __arr_val_0 = printer__write_value((blip__Value__as_pair(cur))->car, path, seen, depth + 1);
		array_push(&parts, &__arr_val_0);
		cur = (blip__Value__as_pair(cur))->cdr;
	}
	string out = _v3_lit_1_44bd53d473cd3b91;
	i64 i = 0;
	while (i < parts.len) {
		if (i > 0) {
			out = string__plus(out, _v3_lit_1_44bd4bd473cd2df9);
		}
		out = string__plus(out, (*(string*)array_get(parts, i)));
		i++;
	}
	out = string__plus(out, _v3_lit_1_44bd52d473cd39de);
	return out;
}

/* V3CACHE_FN_END 1049fe7724a2621f */
/* V3CACHE_MODULE printer */
/* V3CACHE_FN_BEGIN 11046519a5fb373e */
string printer__write_seq(blip__Value v, Array active, map seen, i64 depth) {
	blip__Vector* vec = blip__Value__as_vector(v);
	Array parts = array_new(	sizeof(string), 0, 0);
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < vec->data.len; __for_idx_0++) {
			blip__Value item = *(blip__Value*)(array_get(vec->data, __for_idx_0));
			string __arr_val_1 = printer__write_value(item, active, seen, depth + 1);
			array_push(&parts, &__arr_val_1);
		}
	}
	string out = _v3_lit_0_14650fb0739d0383;
	i64 i = 0;
	while (i < parts.len) {
		if (i > 0) {
			out = string__plus(out, _v3_lit_1_44bd4bd473cd2df9);
		}
		out = string__plus(out, (*(string*)array_get(parts, i)));
		i++;
	}
	return out;
}

/* V3CACHE_FN_END 11046519a5fb373e */
/* V3CACHE_MODULE printer */
/* V3CACHE_FN_BEGIN a020cd07c0c29247 */
string printer__write_table(blip__Value v, Array active, map seen, i64 depth) {
	return ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_1_44bda0d473cdbe68; __v3_internal_symbol_join_0[1] = printer__write_table_body(v, active, seen, depth); __v3_internal_symbol_join_0[2] = _v3_lit_1_44bda6d473cdc89a; string_plus_many(3, __v3_internal_symbol_join_0); });
}

/* V3CACHE_FN_END a020cd07c0c29247 */
/* V3CACHE_MODULE printer */
/* V3CACHE_FN_BEGIN 53a1004945fdbc80 */
string printer__write_table_body(blip__Value v, Array active, map seen, i64 depth) {
	Array keys = blip__Table__keys(blip__Value__as_table(v));
	{
		i64 __sort_n_0 = keys.len;
		Array __sort_buf_1 = array_new(		sizeof(string), __sort_n_0, __sort_n_0);
		Array __sort_src_2 = keys;
		Array __sort_dst_3 = __sort_buf_1;
		{
			i64 __sort_w_5 = 1;
			for (; __sort_w_5 < __sort_n_0; __sort_w_5 = (i64)(((i64)(			__sort_w_5)) << (1))) {
				{
					i64 __sort_lo_6 = 0;
					for (; __sort_lo_6 < __sort_n_0; ) {
						i64 __sort_mid_7 = __sort_lo_6 + __sort_w_5;
						if (__sort_mid_7 > __sort_n_0) {
							__sort_mid_7 = __sort_n_0;
						}
						i64 __sort_hi_8 = __sort_mid_7 + __sort_w_5;
						if (__sort_hi_8 > __sort_n_0) {
							__sort_hi_8 = __sort_n_0;
						}
						i64 __sort_i_9 = __sort_lo_6;
						i64 __sort_j_10 = __sort_mid_7;
						i64 __sort_k_11 = __sort_lo_6;
						for (; (__sort_i_9 < __sort_mid_7) && (__sort_j_10 < __sort_hi_8); __sort_k_11++) {
							if (string__lt((*((string*)((__sort_src_2).data) + (__sort_j_10))), (*((string*)((__sort_src_2).data) + (__sort_i_9))))) {
								array_sort_move(__sort_dst_3.data, __sort_k_11, __sort_src_2.data, __sort_j_10, 1, sizeof(string));
								__sort_j_10 = __sort_j_10 + 1;
							} else {
								array_sort_move(__sort_dst_3.data, __sort_k_11, __sort_src_2.data, __sort_i_9, 1, sizeof(string));
								__sort_i_9 = __sort_i_9 + 1;
							}
						}
						if (__sort_i_9 < __sort_mid_7) {
							array_sort_move(__sort_dst_3.data, __sort_k_11, __sort_src_2.data, __sort_i_9, __sort_mid_7 - __sort_i_9, sizeof(string));
						} else {
							if (__sort_j_10 < __sort_hi_8) {
								array_sort_move(__sort_dst_3.data, __sort_k_11, __sort_src_2.data, __sort_j_10, __sort_hi_8 - __sort_j_10, sizeof(string));
							}
						}
						__sort_lo_6 = __sort_hi_8;
					}
				}
				Array __sort_swap_4 = __sort_src_2;
				__sort_src_2 = __sort_dst_3;
				__sort_dst_3 = __sort_swap_4;
			}
		}
		if (__sort_src_2.data != keys.data) {
			array_sort_move(keys.data, 0, __sort_src_2.data, 0, __sort_n_0, sizeof(string));
		}
		array__free(&__sort_buf_1);
	}
	0;
	Array parts = array_new(	sizeof(string), 0, 0);
	{
		i64 __for_idx_12 = 0;
		for (; __for_idx_12 < keys.len; __for_idx_12++) {
			string k = *(string*)(array_get(keys, __for_idx_12));
			string __arr_val_13 = ({ string __v3_internal_symbol_join_0[4]; __v3_internal_symbol_join_0[0] = _v3_lit_1_44bd61d473cd535b; __v3_internal_symbol_join_0[1] = k; __v3_internal_symbol_join_0[2] = _v3_lit_1_44bd4bd473cd2df9; __v3_internal_symbol_join_0[3] = printer__write_value(blip__Table__get(blip__Value__as_table(v), k), active, seen, depth + 1); string_plus_many(4, __v3_internal_symbol_join_0); });
			array_push(&parts, &__arr_val_13);
		}
	}
	string out = _v3_lit_0_14650fb0739d0383;
	i64 i = 0;
	while (i < parts.len) {
		if (i > 0) {
			out = string__plus(out, _v3_lit_1_44bd4bd473cd2df9);
		}
		out = string__plus(out, (*(string*)array_get(parts, i)));
		i++;
	}
	return out;
}

/* V3CACHE_FN_END 53a1004945fdbc80 */
/* V3CACHE_MODULE printer */
/* V3CACHE_FN_BEGIN 1e749b4bd97c1f30 */
string printer__write_value(blip__Value v, Array active, map seen, i64 depth) {
	if (depth > 40) {
		return _v3_lit_3_14ac5950084b3b2f;
	}
	{
		int __match_tmp_0 = v.tag;
		if (__match_tmp_0 == 1) {
			return _v3_lit_3_3ff5d95143129022;
		} else if (__match_tmp_0 == 512) {
			return _v3_lit_2_9af12a00c5bc79a8;
		} else if (__match_tmp_0 == 2) {
			if (blip__Value__as_bool(v)) {
				return _v3_lit_2_9acb5500c59bf37c;
			} else {
				return _v3_lit_2_9acb4700c59bdbb2;
			}
		} else if (__match_tmp_0 == 4) {
			return strconv__format_int(blip__Value__as_int(v), 10);
		} else if (__match_tmp_0 == 16) {
			return printer__char_literal((i32)(v.i));
		} else if (__match_tmp_0 == 8) {
			double f = blip__Value__as_float(v);
			bool whole = f == math__floor(f);
			if (whole && (f > -1e15) && (f < 1e15)) {
				return v3_f64_fixed(f, 1);
			}
			return f64__str(f);
		} else if (__match_tmp_0 == 32) {
			return printer__quote_string(blip__Value__as_string(v));
		} else if (__match_tmp_0 == 64) {
			return blip__Value__as_string(v);
		} else if (__match_tmp_0 == 128) {
			return string__plus(_v3_lit_1_44bd61d473cd535b, blip__Value__as_string(v));
		} else if (__match_tmp_0 == 256) {
			return printer__write_list(v, active, seen, depth);
		} else if (__match_tmp_0 == 1024) {
			return printer__write_vector(v, active, seen, depth);
		} else if (__match_tmp_0 == 4096) {
			return printer__write_table(v, active, seen, depth);
		} else if (__match_tmp_0 == 131072) {
			blip__StructVal* s = blip__Value__as_struct(v);
			Array parts = array_new(			sizeof(string), 0, 0);
			i64 i = 0;
			while (i < s->fields.len) {
				string __arr_val_1 = printer__write_value(blip__StructVal__at(s, i), active, seen, depth + 1);
				array_push(&parts, &__arr_val_1);
				i++;
			}
			return ({ string __v3_internal_symbol_join_0[5]; __v3_internal_symbol_join_0[0] = _v3_lit_1_44bd53d473cd3b91; __v3_internal_symbol_join_0[1] = s->name; __v3_internal_symbol_join_0[2] = _v3_lit_1_44bd4bd473cd2df9; __v3_internal_symbol_join_0[3] = Array_string__join(parts, _v3_lit_1_44bd4bd473cd2df9); __v3_internal_symbol_join_0[4] = _v3_lit_1_44bd52d473cd39de; string_plus_many(5, __v3_internal_symbol_join_0); });
		} else if (__match_tmp_0 == 2048) {
			return ({ string __v3_internal_symbol_join_1[3]; __v3_internal_symbol_join_1[0] = _v3_lit_2_9b423c00c600ef66; __v3_internal_symbol_join_1[1] = printer__write_seq(v, active, seen, depth); __v3_internal_symbol_join_1[2] = _v3_lit_1_44bd86d473cd923a; string_plus_many(3, __v3_internal_symbol_join_1); });
		} else if (__match_tmp_0 == 8192) {
			return ({ string __v3_internal_symbol_join_2[3]; __v3_internal_symbol_join_2[0] = _v3_lit_2_9b425c00c60125c6; __v3_internal_symbol_join_2[1] = printer__write_table_body(v, active, seen, depth); __v3_internal_symbol_join_2[2] = _v3_lit_1_44bda6d473cdc89a; string_plus_many(3, __v3_internal_symbol_join_2); });
		} else if (__match_tmp_0 == 16384) {
			return _v3_lit_10_26e8cee2f2508a8d;
		} else if (__match_tmp_0 == 32768) {
			return ({ string __v3_internal_symbol_join_3[3]; __v3_internal_symbol_join_3[0] = _v3_lit_12_fe6b253d5f594937; __v3_internal_symbol_join_3[1] = blip__Value__as_string(v); __v3_internal_symbol_join_3[2] = _v3_lit_1_44bd65d473cd5a27; string_plus_many(3, __v3_internal_symbol_join_3); });
		} else if (__match_tmp_0 == 65536) {
			return _v3_lit_15_9c9e29760b1998c7;
		} else {
			return _v3_lit_10_edc95ee159574c78;
		}
	}
}

/* V3CACHE_FN_END 1e749b4bd97c1f30 */
/* V3CACHE_MODULE printer */
/* V3CACHE_FN_BEGIN 53e0e33ebee822ae */
string printer__write_vector(blip__Value v, Array active, map seen, i64 depth) {
	Array path = __new_array_noscan(0, active.len + 1, sizeof(u64));
	array__push_many(&path, active.data, active.len);
	return ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_1_44bd80d473cd8808; __v3_internal_symbol_join_0[1] = printer__write_seq(v, path, seen, depth); __v3_internal_symbol_join_0[2] = _v3_lit_1_44bd86d473cd923a; string_plus_many(3, __v3_internal_symbol_join_0); });
}

/* V3CACHE_FN_END 53e0e33ebee822ae */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 1cf14de5ee2a5e18 */
void println(string s) {
}

/* V3CACHE_FN_END 1cf14de5ee2a5e18 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 1d138efb69c7e959 */
string ptr_str(void* ptr) {
}

/* V3CACHE_FN_END 1d138efb69c7e959 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN ce8f869a8f3c5316 */
void reader__Arena__add_child(reader__Arena* a, i32 parent, i32 child) {
	i32 start = (i32)(a->children.len);
	i32 __arr_val_0 = child;
	array_push(&a->children, &__arr_val_0);
	reader__Datum* node = &(*(reader__Datum*)array_get(a->nodes, (i64)(parent)));
	node->count = 1;
	node->start = start;
}

/* V3CACHE_FN_END ce8f869a8f3c5316 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN bedbbde857b00415 */
i64 reader__Arena__count(reader__Arena* a) {
	return a->nodes.len;
}

/* V3CACHE_FN_END bedbbde857b00415 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 75e486cad52dd6f3 */
void reader__Arena__finish(reader__Arena* a, i32 id, Array items) {
	i32 start = (i32)(a->children.len);
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < items.len; __for_idx_0++) {
			i32 it = *(i32*)(array_get(items, __for_idx_0));
			i32 __arr_val_1 = it;
			array_push(&a->children, &__arr_val_1);
		}
	}
	reader__Datum* node = &(*(reader__Datum*)array_get(a->nodes, (i64)(id)));
	node->start = start;
	node->count = (i32)(items.len);
}

/* V3CACHE_FN_END 75e486cad52dd6f3 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 4b768fdf422a0e2b */
i32 reader__Arena__float_leaf(reader__Arena* a, double f) {
	reader__Datum __arr_val_0 = (reader__Datum){.tag = reader__DatumTag__v_float, .f = f, .value = _v3_lit_0_14650fb0739d0383};
	array_push(&a->nodes, &__arr_val_0);
	return (i32)(a->nodes.len - 1);
}

/* V3CACHE_FN_END 4b768fdf422a0e2b */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN d56ed706a13e4d74 */
i32 reader__Arena__int_leaf(reader__Arena* a, int tag, i64 n) {
	reader__Datum __arr_val_0 = (reader__Datum){.tag = tag, .i = n, .value = _v3_lit_0_14650fb0739d0383};
	array_push(&a->nodes, &__arr_val_0);
	return (i32)(a->nodes.len - 1);
}

/* V3CACHE_FN_END d56ed706a13e4d74 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 4d23d9a6cc15e18d */
Array reader__Arena__kids(reader__Arena* a, i32 id) {
	reader__Datum d = (*(reader__Datum*)array_get(a->nodes, (i64)(id)));
	Array out = __new_array_noscan(0, 0, sizeof(i32));
	i64 n = (i64)(d.start);
	i64 i = 0;
	while (i < (i64)(d.count)) {
		i32 __arr_val_0 = (*(i32*)array_get(a->children, n + i));
		array_push(&out, &__arr_val_0);
		i++;
	}
	return out;
}

/* V3CACHE_FN_END 4d23d9a6cc15e18d */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 5aa4dc7b602c44f2 */
i32 reader__Arena__leaf(reader__Arena* a, int tag) {
	reader__Datum __arr_val_0 = (reader__Datum){.tag = tag, .value = _v3_lit_0_14650fb0739d0383};
	array_push(&a->nodes, &__arr_val_0);
	return (i32)(a->nodes.len - 1);
}

/* V3CACHE_FN_END 5aa4dc7b602c44f2 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN b274759e48925d2 */
reader__Datum* reader__Arena__node(reader__Arena* a, i32 id) {
	return &(*(reader__Datum*)array_get(a->nodes, (i64)(id)));
}

/* V3CACHE_FN_END b274759e48925d2 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 3b3b85334d932626 */
i32 reader__Arena__open(reader__Arena* a, int tag) {
	reader__Datum __arr_val_0 = (reader__Datum){.tag = tag, .value = _v3_lit_0_14650fb0739d0383};
	array_push(&a->nodes, &__arr_val_0);
	return (i32)(a->nodes.len - 1);
}

/* V3CACHE_FN_END 3b3b85334d932626 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 6fe0fddb247a9b06 */
reader__Forms reader__Arena__read_forms(reader__Arena* a, string src) {
	reader__Reader r = (reader__Reader){.src = src, .arena = a, .line = 1, .diags = array_new(sizeof(reader__Diagnostic), 0, 0)};
	Array forms = __new_array_noscan(0, 0, sizeof(i32));
	for (;;) {
		i64 before = r.off;
		__v_option_i32 __or_opt_0 = reader__Reader__read(&r);
		i32 __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			break;
		}
		i32 id = __or_val_1;
		i32 __arr_val_2 = id;
		array_push(&forms, &__arr_val_2);
		if (r.off == before) {
			break;
		}
	}
	return (reader__Forms){.forms = forms, .diags = r.diags};
}

/* V3CACHE_FN_END 6fe0fddb247a9b06 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 50355bbe47e32f97 */
void reader__Arena__set_pos(reader__Arena* a, i32 id, i64 line, i64 col) {
	reader__Datum* node = &(*(reader__Datum*)array_get(a->nodes, (i64)(id)));
	node->line = line;
	node->col = col;
}

/* V3CACHE_FN_END 50355bbe47e32f97 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 45a4841178df29ba */
i32 reader__Arena__str_leaf(reader__Arena* a, int tag, string s) {
	reader__Datum __arr_val_0 = (reader__Datum){.tag = tag, .value = s};
	array_push(&a->nodes, &__arr_val_0);
	return (i32)(a->nodes.len - 1);
}

/* V3CACHE_FN_END 45a4841178df29ba */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN f14fad6aac208ea9 */
bool reader__DatumTag_is_collection(int t) {
	return (t == reader__DatumTag__list) || (t == reader__DatumTag__vector) || (t == reader__DatumTag__table) || (t == reader__DatumTag__array) || (t == reader__DatumTag__bytes) || (t == reader__DatumTag__buffer);
}

/* V3CACHE_FN_END f14fad6aac208ea9 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN e7ca67ef2b9e41ad */
bool reader__DatumTag_is_stringy(int t) {
	return (t == reader__DatumTag__str) || (t == reader__DatumTag__sym) || (t == reader__DatumTag__kw);
}

/* V3CACHE_FN_END e7ca67ef2b9e41ad */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN e4ecd222ea5053fa */
string reader__Diagnostic__render(reader__Diagnostic* d, string path) {
	return ({ string __v3_internal_symbol_join_0[7]; __v3_internal_symbol_join_0[0] = path; __v3_internal_symbol_join_0[1] = _v3_lit_1_44bd61d473cd535b; __v3_internal_symbol_join_0[2] = int__str(d->line); __v3_internal_symbol_join_0[3] = _v3_lit_1_44bd61d473cd535b; __v3_internal_symbol_join_0[4] = int__str(d->col); __v3_internal_symbol_join_0[5] = _v3_lit_2_9b20b700c5e4da01; __v3_internal_symbol_join_0[6] = d->msg; string_plus_many(7, __v3_internal_symbol_join_0); });
}

/* V3CACHE_FN_END e4ecd222ea5053fa */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN f507de41f4c75931 */
inline u8 reader__Reader__at(reader__Reader* r, i64 i) {
	if ((i < 0) || (i >= r->src.len)) {
		return 0;
	}
	return string__at(r->src, i);
}

/* V3CACHE_FN_END f507de41f4c75931 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 88d13b367caeee4e */
inline bool reader__Reader__eof(reader__Reader* r) {
	return r->off >= r->src.len;
}

/* V3CACHE_FN_END 88d13b367caeee4e */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 41669abbea9834da */
void reader__Reader__fail(reader__Reader* r, string msg) {
	reader__Diagnostic __arr_val_0 = (reader__Diagnostic){.msg = msg, .line = r->line, .col = (r->off - r->line_at) + 1};
	array_push(&r->diags, &__arr_val_0);
}

/* V3CACHE_FN_END 41669abbea9834da */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 1592479dde166974 */
bool reader__Reader__is_delimiter(reader__Reader* r, u8 c) {
	return (c == ' ') || (c == reader__tab_c) || (c == reader__cr) || (c == reader__nl) || (c == reader__lparen) || (c == reader__rparen) || (c == reader__lbrack) || (c == reader__rbrack) || (c == reader__lbrace) || (c == reader__rbrace) || (c == reader__dquote) || (c == reader__semi) || (c == 0);
}

/* V3CACHE_FN_END 1592479dde166974 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 9bfec25aa66458e1 */
u8 reader__Reader__next(reader__Reader* r) {
	u8 c = reader__Reader__peek(r);
	r->off++;
	if (c == reader__nl) {
		r->line++;
		r->line_at = r->off;
	}
	return c;
}

/* V3CACHE_FN_END 9bfec25aa66458e1 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 89ad3320de265487 */
inline u8 reader__Reader__peek(reader__Reader* r) {
	return reader__Reader__at(r, r->off);
}

/* V3CACHE_FN_END 89ad3320de265487 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 7ff3938b53557872 */
__v_option_i32 reader__Reader__read(reader__Reader* r) {
	reader__Reader__skip_ws(r);
	if (reader__Reader__eof(r)) {
		return (__v_option_i32){.ok = false};
	}
	i64 line = r->line;
	i64 col = (r->off - r->line_at) + 1;
	__v_option_i32 __or_opt_0 = reader__Reader__read_inner(r);
	i32 __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		return (__v_option_i32){.ok = false};
	}
	i32 id = __or_val_1;
	reader__Arena__set_pos(r->arena, id, line, col);
	return (__v_option_i32){.ok = true, .value = id};
	return (__v_option_i32){.ok = true};
}

/* V3CACHE_FN_END 7ff3938b53557872 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 173a31bb6af850b0 */
__v_option_i32 reader__Reader__read_atom(reader__Reader* r) {
	i64 start = r->off;
	Array buf = __new_array_noscan(0, 16, sizeof(u8));
	while (!reader__Reader__eof(r) && !reader__Reader__is_delimiter(r, reader__Reader__peek(r))) {
		u8 __arr_val_0 = reader__Reader__next(r);
		array_push(&buf, &__arr_val_0);
	}
	if (buf.len == 0) {
		reader__Reader__fail(r, string__plus(_v3_lit_18_7be5ba2484ac0e22, u8__hex(reader__Reader__peek(r))));
		reader__Reader__next(r);
		return reader__Reader__read(r);
	}
	string s = Array_u8__bytestr(buf);
	if (string__at(s, 0) == ':') {
		return (__v_option_i32){.ok = true, .value = reader__Arena__str_leaf(r->arena, reader__DatumTag__kw, string__substr(		s, 		1, (		s).len))};
	}
	__v_option_i32 __if_guard_1 = reader__Reader__try_number(r, s);
	if (__if_guard_1.ok) {
		i32 id = __if_guard_1.value;
		return (__v_option_i32){.ok = true, .value = id};
	}
	(void)(start);
	return (__v_option_i32){.ok = true, .value = reader__Arena__str_leaf(r->arena, reader__DatumTag__sym, s)};
	return (__v_option_i32){.ok = true};
}

/* V3CACHE_FN_END 173a31bb6af850b0 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 50e418a5c0860e3d */
__v_option_i32 reader__Reader__read_bar_symbol(reader__Reader* r) {
	reader__Reader__next(r);
	Array buf = __new_array_noscan(0, 16, sizeof(u8));
	while (!reader__Reader__eof(r) && (reader__Reader__peek(r) != '|')) {
		u8 __arr_val_0 = reader__Reader__next(r);
		array_push(&buf, &__arr_val_0);
	}
	if (reader__Reader__eof(r)) {
		reader__Reader__short(r, _v3_lit_21_85aa735d3d99a4db);
	} else {
		reader__Reader__next(r);
	}
	return (__v_option_i32){.ok = true, .value = reader__Arena__str_leaf(r->arena, reader__DatumTag__sym, Array_u8__bytestr(buf))};
	return (__v_option_i32){.ok = true};
}

/* V3CACHE_FN_END 50e418a5c0860e3d */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN a63b571fed0d062e */
__v_option_i32 reader__Reader__read_character(reader__Reader* r) {
	Array name = __new_array_noscan(0, 0, sizeof(u8));
	while (!reader__Reader__eof(r) && !reader__Reader__is_delimiter(r, reader__Reader__peek(r))) {
		u8 __arr_val_0 = reader__Reader__next(r);
		array_push(&name, &__arr_val_0);
	}
	string s = Array_u8__bytestr(name);
	u8 __match_val_1 = 0;
	if (string__eq(s, _v3_lit_7_41566b4aaab421d9) || string__eq(s, _v3_lit_8_c411c491539069e9)) {
		__match_val_1 = (u8)(10);
	} else if (string__eq(s, _v3_lit_5_a5593b4a24922a2f)) {
		__match_val_1 = (u8)(32);
	} else if (string__eq(s, _v3_lit_3_96797d51740b73ba)) {
		__match_val_1 = (u8)(9);
	} else if (string__eq(s, _v3_lit_6_7488c53f48adf9a1) || string__eq(s, _v3_lit_15_2c9b217c089b5e6a)) {
		__match_val_1 = (u8)(13);
	} else if (string__eq(s, _v3_lit_9_17f117cc78c576b2)) {
		__match_val_1 = (u8)(8);
	} else if (string__eq(s, _v3_lit_6_5397a7f2858c1824)) {
		__match_val_1 = (u8)(27);
	} else if (string__eq(s, _v3_lit_4_cee237146f424a56) || string__eq(s, _v3_lit_3_3f96b95142c1c53e)) {
		__match_val_1 = (u8)(0);
	} else if (string__eq(s, _v3_lit_5_5acb56cc7a626865)) {
		__match_val_1 = (u8)(7);
	} else {
		u8 __if_val_2 = {0};
		if (s.len == 1) {
			__if_val_2 = (u8)(string__at(s, 0));
		} else {
			if ((s.len > 2) && ((string__at(s, 0) == reader__x_c) || (string__at(s, 0) == 'X'))) {
				__v_result_u64 __or_opt_3 = strconv__parse_uint(string__substr(				s, 				1, (				s).len), 16, 32);
				u64 __or_val_4 = {0};
				if (__or_opt_3.ok) {
					__or_val_4 = __or_opt_3.value;
				} else {
					IError err = __or_opt_3.err;
					reader__Reader__fail(r, string__plus(_v3_lit_18_f9e7af49166a001c, s));
					return (__v_option_i32){.ok = true, .value = reader__Arena__int_leaf(r->arena, reader__DatumTag__v_char, 63)};
				}
				u64 hexv = __or_val_4;
				__if_val_2 = (u8)(hexv);
			} else {
				if (s.len == 2) {
					u8 __match_val_5 = 0;
					u8 __match_tmp_6 = (u8)(string__at(s, 1));
					if (__match_tmp_6 == reader__nl) {
						__match_val_5 = (u8)(10);
					} else if (__match_tmp_6 == reader__tab_c) {
						__match_val_5 = (u8)(9);
					} else if (__match_tmp_6 == reader__cr) {
						__match_val_5 = (u8)(13);
					} else if (__match_tmp_6 == reader__zero_c) {
						__match_val_5 = (u8)(0);
					} else {
						__match_val_5 = (u8)(string__at(s, 1));
					}
					__if_val_2 = __match_val_5;
				} else {
					reader__Reader__fail(r, string__plus(_v3_lit_23_b8de0cace035bb6d, s));
					__if_val_2 = (u8)(63);
				}
			}
		}
		__match_val_1 = __if_val_2;
	}
	u8 cp = __match_val_1;
	return (__v_option_i32){.ok = true, .value = reader__Arena__int_leaf(r->arena, reader__DatumTag__v_char, (i64)(cp))};
	return (__v_option_i32){.ok = true};
}

/* V3CACHE_FN_END a63b571fed0d062e */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN ab04703dfd6e2ac7 */
__v_option_i32 reader__Reader__read_collection(reader__Reader* r, int tag, u8 v_close) {
	i32 id = reader__Arena__open(r->arena, tag);
	Array items = __new_array_noscan(0, 0, sizeof(i32));
	for (;;) {
		reader__Reader__skip_ws(r);
		if (reader__Reader__eof(r)) {
			reader__Reader__short(r, ({ string __v3_internal_symbol_join_0[4]; __v3_internal_symbol_join_0[0] = _v3_lit_9_43e38cec2213a79c; __v3_internal_symbol_join_0[1] = reader__DatumTag__autostr(tag); __v3_internal_symbol_join_0[2] = _v3_lit_18_4e03d2be2523703d; __v3_internal_symbol_join_0[3] = u8__hex(v_close); string_plus_many(4, __v3_internal_symbol_join_0); }));
			break;
		}
		if (reader__Reader__peek(r) == v_close) {
			reader__Reader__next(r);
			break;
		}
		i64 before = r->off;
		__v_option_i32 __or_opt_0 = reader__Reader__read(r);
		i32 __or_val_1 = {0};
		if (__or_opt_0.ok) {
			__or_val_1 = __or_opt_0.value;
		} else {
			break;
		}
		i32 child = __or_val_1;
		if (r->off == before) {
			break;
		}
		i32 __arr_val_2 = child;
		array_push(&items, &__arr_val_2);
	}
	reader__Arena__finish(r->arena, id, items);
	return (__v_option_i32){.ok = true, .value = id};
	return (__v_option_i32){.ok = true};
}

/* V3CACHE_FN_END ab04703dfd6e2ac7 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 2b6095baf94db7b */
__v_option_i32 reader__Reader__read_hash(reader__Reader* r) {
	reader__Reader__next(r);
	u8 c = reader__Reader__peek(r);
	if (c == reader__colon) {
		reader__Reader__next(r);
		string name = reader__Reader__read_symbol_text(r);
		return (__v_option_i32){.ok = true, .value = reader__Arena__str_leaf(r->arena, reader__DatumTag__kw, name)};
	} else if (c == reader__lparen) {
		reader__Reader__next(r);
		return reader__Reader__read_collection(r, reader__DatumTag__bytes, reader__rparen);
	} else if (c == reader__backslash) {
		reader__Reader__next(r);
		return reader__Reader__read_character(r);
	} else if (c == '"') {
		return reader__Reader__read_string(r);
	} else if ((c == 't') || (c == 'f')) {
		reader__Reader__next(r);
		reader__Arena* __order_snapshot_0 = r->arena;
		i64 __if_val_1 = {0};
		if (c == 't') {
			__if_val_1 = 1;
		} else {
			__if_val_1 = 0;
		}
		return (__v_option_i32){.ok = true, .value = reader__Arena__int_leaf(__order_snapshot_0, reader__DatumTag__bool, __if_val_1)};
	} else {
		reader__Reader__fail(r, _v3_lit_16_73711fb201e24a45);
		reader__Reader__next(r);
		return reader__Reader__read(r);
	}
	return (__v_option_i32){.ok = true};
}

/* V3CACHE_FN_END 2b6095baf94db7b */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 342c2fe93e3d1e87 */
__v_option_i32 reader__Reader__read_inner(reader__Reader* r) {
	u8 c = reader__Reader__peek(r);
	if (c == reader__lparen) {
		reader__Reader__next(r);
		return reader__Reader__read_collection(r, reader__DatumTag__list, reader__rparen);
	} else if (c == reader__lbrack) {
		reader__Reader__next(r);
		return reader__Reader__read_collection(r, reader__DatumTag__vector, reader__rbrack);
	} else if (c == reader__lbrace) {
		reader__Reader__next(r);
		return reader__Reader__read_collection(r, reader__DatumTag__table, reader__rbrace);
	} else if (c == reader__at_c) {
		u8 nxt = reader__Reader__at(r, r->off + 1);
		if (nxt == reader__lparen) {
			reader__Reader__next(r);
			reader__Reader__next(r);
			return reader__Reader__read_collection(r, reader__DatumTag__array, reader__rparen);
		}
		if (nxt == reader__lbrack) {
			reader__Reader__next(r);
			reader__Reader__next(r);
			return reader__Reader__read_collection(r, reader__DatumTag__array, reader__rbrack);
		}
		if (nxt == reader__lbrace) {
			reader__Reader__next(r);
			reader__Reader__next(r);
			return reader__Reader__read_collection(r, reader__DatumTag__buffer, reader__rbrace);
		}
		return reader__Reader__read_atom(r);
	} else if (c == ((reader__rparen | reader__rbrack) | reader__rbrace)) {
		reader__Reader__fail(r, string__plus(_v3_lit_18_7be5ba2484ac0e22, u8__hex(c)));
		reader__Reader__next(r);
		return reader__Reader__read(r);
	} else if ((c == reader__quote_c) || (c == reader__quasi_c) || (c == reader__unquote_c)) {
		reader__Reader__next(r);
		reader__DatumTag __match_val_0 = 0;
		if (c == reader__quote_c) {
			__match_val_0 = reader__DatumTag__quoted;
		} else if (c == reader__quasi_c) {
			__match_val_0 = reader__DatumTag__quasi;
		} else {
			__match_val_0 = reader__DatumTag__unquote;
		}
		reader__DatumTag tag = __match_val_0;
		if ((tag == reader__DatumTag__unquote) && (reader__Reader__peek(r) == reader__at_c)) {
			reader__Reader__next(r);
			__v_option_i32 __or_opt_1 = reader__Reader__read(r);
			i32 __or_val_2 = {0};
			if (__or_opt_1.ok) {
				__or_val_2 = __or_opt_1.value;
			} else {
				return (__v_option_i32){.ok = false};
			}
			i32 inner = __or_val_2;
			i32 id = reader__Arena__leaf(r->arena, reader__DatumTag__unquote_splice);
			reader__Arena__add_child(r->arena, id, inner);
			return (__v_option_i32){.ok = true, .value = id};
		}
		__v_option_i32 __or_opt_3 = reader__Reader__read(r);
		i32 __or_val_4 = {0};
		if (__or_opt_3.ok) {
			__or_val_4 = __or_opt_3.value;
		} else {
			return (__v_option_i32){.ok = false};
		}
		i32 inner = __or_val_4;
		i32 id = reader__Arena__leaf(r->arena, tag);
		reader__Arena__add_child(r->arena, id, inner);
		return (__v_option_i32){.ok = true, .value = id};
	} else if (c == '"') {
		return reader__Reader__read_string(r);
	} else if (c == '|') {
		u8 nxt = reader__Reader__at(r, r->off + 1);
		if ((nxt == (u8)(0)) || (nxt == reader__sp) || (nxt == reader__tab_c) || (nxt == reader__nl) || (nxt == reader__cr) || (nxt == reader__rparen) || (nxt == reader__rbrack) || (nxt == reader__rbrace)) {
			return reader__Reader__read_bar_symbol(r);
		}
		Array op = __new_array_noscan(0, 0, sizeof(u8));
		while (!reader__Reader__eof(r)) {
			u8 ch = reader__Reader__peek(r);
			if ((ch == reader__sp) || (ch == reader__tab_c) || (ch == reader__nl) || (ch == reader__cr) || (ch == reader__rparen) || (ch == reader__rbrack) || (ch == reader__rbrace)) {
				break;
			}
			u8 __arr_val_5 = reader__Reader__next(r);
			array_push(&op, &__arr_val_5);
		}
		return (__v_option_i32){.ok = true, .value = reader__Arena__str_leaf(r->arena, reader__DatumTag__sym, Array_u8__bytestr(op))};
	} else if (c == '#') {
		return reader__Reader__read_hash(r);
	} else {
		return reader__Reader__read_atom(r);
	}
	return (__v_option_i32){.ok = true};
}

/* V3CACHE_FN_END 342c2fe93e3d1e87 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 64a4d3977ff1fefc */
__v_option_i32 reader__Reader__read_string(reader__Reader* r) {
	reader__Reader__next(r);
	Array buf = __new_array_noscan(0, 32, sizeof(u8));
	for (;;) {
		if (reader__Reader__eof(r)) {
			reader__Reader__short(r, _v3_lit_19_1138739c22f89302);
			break;
		}
		u8 c = reader__Reader__next(r);
		if (c == reader__dquote) {
			break;
		}
		if (c != '\\') {
			u8 __arr_val_0 = c;
			array_push(&buf, &__arr_val_0);
			continue;
		}
		if (reader__Reader__eof(r)) {
			reader__Reader__fail(r, _v3_lit_26_eb4f1b78b183db39);
			break;
		}
		u8 e = reader__Reader__next(r);
		if (e == 'n') {
			u8 __arr_val_1 = '\n';
			array_push(&buf, &__arr_val_1);
		} else if (e == 't') {
			u8 __arr_val_2 = '\t';
			array_push(&buf, &__arr_val_2);
		} else if (e == 'r') {
			u8 __arr_val_3 = '\r';
			array_push(&buf, &__arr_val_3);
		} else if (e == '0') {
			u8 __arr_val_4 = 0;
			array_push(&buf, &__arr_val_4);
		} else if (e == '\\') {
			u8 __arr_val_5 = '\\';
			array_push(&buf, &__arr_val_5);
		} else if (e == '"') {
			u8 __arr_val_6 = '"';
			array_push(&buf, &__arr_val_6);
		} else {
			u8 __arr_val_7 = '\\';
			array_push(&buf, &__arr_val_7);
			u8 __arr_val_8 = e;
			array_push(&buf, &__arr_val_8);
		}
	}
	return (__v_option_i32){.ok = true, .value = reader__Arena__str_leaf(r->arena, reader__DatumTag__str, Array_u8__bytestr(buf))};
	return (__v_option_i32){.ok = true};
}

/* V3CACHE_FN_END 64a4d3977ff1fefc */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 784a0570fe60234d */
string reader__Reader__read_symbol_text(reader__Reader* r) {
	Array buf = __new_array_noscan(0, 16, sizeof(u8));
	while (!reader__Reader__eof(r) && !reader__Reader__is_delimiter(r, reader__Reader__peek(r))) {
		u8 __arr_val_0 = reader__Reader__next(r);
		array_push(&buf, &__arr_val_0);
	}
	return Array_u8__bytestr(buf);
}

/* V3CACHE_FN_END 784a0570fe60234d */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN c922dca19ff9852a */
void reader__Reader__short(reader__Reader* r, string msg) {
	reader__Diagnostic __arr_val_0 = (reader__Diagnostic){.msg = msg, .line = r->line, .col = (r->off - r->line_at) + 1, .incomplete = true};
	array_push(&r->diags, &__arr_val_0);
}

/* V3CACHE_FN_END c922dca19ff9852a */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 657b92e2e36d1eb7 */
void reader__Reader__skip_block_comment(reader__Reader* r) {
	reader__Reader__next(r);
	reader__Reader__next(r);
	i64 depth = 1;
	while (!reader__Reader__eof(r) && (depth > 0)) {
		u8 c = reader__Reader__next(r);
		if ((c == reader__hash_c) && (reader__Reader__peek(r) == reader__bar)) {
			reader__Reader__next(r);
			depth++;
		} else if ((c == reader__bar) && (reader__Reader__peek(r) == reader__hash_c)) {
			reader__Reader__next(r);
			depth--;
		}
	}
}

/* V3CACHE_FN_END 657b92e2e36d1eb7 */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 37f262a732b95fe */
void reader__Reader__skip_ws(reader__Reader* r) {
	while (!reader__Reader__eof(r)) {
		u8 c = reader__Reader__peek(r);
		if ((c == ' ') || (c == reader__tab_c) || (c == reader__cr) || (c == reader__nl)) {
			reader__Reader__next(r);
			continue;
		}
		if (c == reader__semi) {
			while (!reader__Reader__eof(r) && (reader__Reader__peek(r) != reader__nl)) {
				reader__Reader__next(r);
			}
			continue;
		}
		if ((c == reader__hash_c) && (reader__Reader__at(r, r->off + 1) == reader__bar)) {
			reader__Reader__skip_block_comment(r);
			continue;
		}
		if ((c == reader__hash_c) && (reader__Reader__at(r, r->off + 1) == reader__semi)) {
			reader__Reader__next(r);
			reader__Reader__next(r);
			reader__Reader__skip_ws(r);
			reader__Reader__read(r);
			continue;
		}
		break;
	}
}

/* V3CACHE_FN_END 37f262a732b95fe */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 34553d3870ef4c8b */
__v_option_i32 reader__Reader__try_number(reader__Reader* r, string s) {
	if (s.len == 0) {
		return (__v_option_i32){.ok = false};
	}
	u8 first = string__at(s, 0);
	bool is_digit = (first >= '0') && (first <= '9');
	if (!is_digit && (first != reader__minus_c) && (first != reader__plus_c) && (first != reader__dot_c)) {
		return (__v_option_i32){.ok = false};
	}
	bool has_digit = false;
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < s.len; __for_idx_0++) {
			u8 ch = string__at(s, __for_idx_0);
			if ((ch >= '0') && (ch <= '9')) {
				has_digit = true;
				break;
			}
		}
	}
	if (!has_digit) {
		return (__v_option_i32){.ok = false};
	}
	__v_option_i64 __or_opt_1 = string__index(s, _v3_lit_1_44bd54d473cd3d44);
	i64 __or_val_2 = {0};
	if (__or_opt_1.ok) {
		__or_val_2 = __or_opt_1.value;
	} else {
		__or_val_2 = -1;
	}
	i64 idx = __or_val_2;
	if (idx > 0) {
		__v_result_i64 __or_opt_3 = strconv__atoi64(string__substr(		s, 0, 		idx));
		i64 __or_val_4 = {0};
		if (__or_opt_3.ok) {
			__or_val_4 = __or_opt_3.value;
		} else {
			IError err = __or_opt_3.err;
			return (__v_option_i32){.ok = false};
		}
		i64 n = __or_val_4;
		__v_result_i64 __or_opt_5 = strconv__atoi64(string__substr(		s, 		idx + 1, (		s).len));
		i64 __or_val_6 = {0};
		if (__or_opt_5.ok) {
			__or_val_6 = __or_opt_5.value;
		} else {
			IError err = __or_opt_5.err;
			return (__v_option_i32){.ok = false};
		}
		i64 d = __or_val_6;
		if (d == 0) {
			reader__Reader__fail(r, _v3_lit_30_c719a115a4b85e5f);
			return (__v_option_i32){.ok = true, .value = reader__Arena__int_leaf(r->arena, reader__DatumTag__v_int, 0)};
		}
		i32 id = reader__Arena__leaf(r->arena, reader__DatumTag__rat);
		reader__Datum* node = reader__Arena__node(r->arena, id);
		node->i = n;
		node->den = d;
		return (__v_option_i32){.ok = true, .value = id};
	}
	if ((s.len > 2) && (string__at(s, 0) == reader__zero_c)) {
		i64 __match_val_7 = 0;
		u8 __match_tmp_8 = string__at(s, 1);
		if ((__match_tmp_8 == 'x') || (__match_tmp_8 == 'X')) {
			__match_val_7 = 16;
		} else if ((__match_tmp_8 == 'b') || (__match_tmp_8 == 'B')) {
			__match_val_7 = 2;
		} else if ((__match_tmp_8 == 'o') || (__match_tmp_8 == 'O')) {
			__match_val_7 = 8;
		} else {
			__match_val_7 = 0;
		}
		i64 base = __match_val_7;
		if (base != 0) {
			__v_result_u64 __or_opt_9 = strconv__parse_uint(string__substr(			s, 			2, (			s).len), base, 64);
			u64 __or_val_10 = {0};
			if (__or_opt_9.ok) {
				__or_val_10 = __or_opt_9.value;
			} else {
				IError err = __or_opt_9.err;
				reader__Reader__fail(r, ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_4_d032deaf8b5514c2; __v3_internal_symbol_join_0[1] = s; __v3_internal_symbol_join_0[2] = _v3_lit_8_1b3088722da67dc0; string_plus_many(3, __v3_internal_symbol_join_0); }));
				return (__v_option_i32){.ok = true, .value = reader__Arena__int_leaf(r->arena, reader__DatumTag__v_int, 0)};
			}
			u64 v = __or_val_10;
			return (__v_option_i32){.ok = true, .value = reader__Arena__int_leaf(r->arena, reader__DatumTag__v_int, (i64)(v))};
		}
	}
	__v_result_i64 __if_guard_11 = strconv__atoi64(s);
	if (__if_guard_11.ok) {
		i64 n = __if_guard_11.value;
		return (__v_option_i32){.ok = true, .value = reader__Arena__int_leaf(r->arena, reader__DatumTag__v_int, n)};
	}
	__v_result_double __if_guard_12 = strconv__atof64(s, reader__strict_float);
	if (__if_guard_12.ok) {
		double f = __if_guard_12.value;
		return (__v_option_i32){.ok = true, .value = reader__Arena__float_leaf(r->arena, f)};
	}
	if (s.len > 1) {
		__v_result_double __if_guard_13 = strconv__atof64(string__plus(u8__str(reader__zero_c), s), reader__strict_float);
		if (__if_guard_13.ok) {
			double f = __if_guard_13.value;
			return (__v_option_i32){.ok = true, .value = reader__Arena__float_leaf(r->arena, f)};
		}
	}
	return (__v_option_i32){.ok = false};
	return (__v_option_i32){.ok = true};
}

/* V3CACHE_FN_END 34553d3870ef4c8b */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN 4054e29b583843fc */
reader__Depth reader__depth(string src) {
	reader__Depth d = (reader__Depth){.n = 0};
	i64 i = 0;
	while (i < src.len) {
		u8 c = string__at(src, i);
		if ((c == reader__hash_c) && ((i + 1) < src.len) && (string__at(src, i + 1) == reader__bar)) {
			i64 level = 1;
			i += 2;
			while (((i + 1) < src.len) && (level > 0)) {
				if ((string__at(src, i) == reader__hash_c) && (string__at(src, i + 1) == reader__bar)) {
					level++;
					i += 2;
				} else if ((string__at(src, i) == reader__bar) && (string__at(src, i + 1) == reader__hash_c)) {
					level--;
					i += 2;
				} else {
					i++;
				}
			}
			continue;
		}
		if (c == reader__semi) {
			while ((i < src.len) && (string__at(src, i) != reader__nl)) {
				i++;
			}
			continue;
		}
		if (c == reader__dquote) {
			i++;
			bool closed = false;
			while (i < src.len) {
				if (string__at(src, i) == reader__backslash_bs) {
					i += 2;
					continue;
				}
				if (string__at(src, i) == reader__dquote) {
					closed = true;
					i++;
					break;
				}
				i++;
			}
			if (!closed) {
				d.in_string = true;
			}
			continue;
		}
		if ((c == reader__lparen) || (c == reader__lbrack) || (c == reader__lbrace)) {
			d.n++;
		} else if ((c == reader__rparen) || (c == reader__rbrack) || (c == reader__rbrace)) {
			if (d.n > 0) {
				d.n--;
			}
		}
		i++;
	}
	d.in_comment = (d.n > 0) || d.in_string;
	return d;
}

/* V3CACHE_FN_END 4054e29b583843fc */
/* V3CACHE_MODULE reader */
/* V3CACHE_FN_BEGIN dbb096d3d51b46ef */
reader__ReadResult reader__read_all(string src) {
	reader__Arena* a = (reader__Arena*)memdup(&(reader__Arena){.nodes = array_new(sizeof(reader__Datum), 0, 0), .children = array_new(sizeof(i32), 0, 0)}, sizeof(reader__Arena));
	reader__Forms f = reader__Arena__read_forms(a, src);
	return (reader__ReadResult){.arena = *a, .forms = f.forms, .diags = f.diags};
}

/* V3CACHE_FN_END dbb096d3d51b46ef */
/* V3CACHE_MODULE main */
/* V3CACHE_FN_BEGIN 6e4ea2d6fc1e63fc */
__v_result_string run_example(string path) {
	__v_result_string __or_opt_0 = os__read_file(path);
	string __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return (__v_result_string){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_12_35e90eecdfa1b04, path), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	string src = __or_val_1;
	host__CaptureHost* h = host__new_capture();
	Array __in_lhs_2 = new_array_from_c_array(3, 3, sizeof(string), (string[3]){_v3_lit_11_1477ee6af240911f, _v3_lit_24_62055c9e291ef283, _v3_lit_20_5e518be6b676c02e});
	{
		i64 __for_idx_3 = 0;
		for (; __for_idx_3 < __in_lhs_2.len; __for_idx_3++) {
			string f = *(string*)(array_get(__in_lhs_2, __for_idx_3));
			__v_result_string __or_opt_4 = os__read_file(f);
			string __or_val_5 = {0};
			if (__or_opt_4.ok) {
				__or_val_5 = __or_opt_4.value;
			} else {
				IError err = __or_opt_4.err;
				return (__v_result_string){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_12_35e90eecdfa1b04, f), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
			}
			string body = __or_val_5;
			host__CaptureHost__give_file(h, f, body);
		}
	}
	machine__Machine* m = machine__new_standalone((host__Host){._typ = 555330581, ._object = h});
	__v_result_blip__Value __or_opt_6 = machine__Machine__load(m, _v3_lit_11_1477ee6af240911f);
	if (!__or_opt_6.ok) {
		IError err = __or_opt_6.err;
		return (__v_result_string){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = string__plus(_v3_lit_34_92d522d4317d7079, IError__msg(&(err))), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	m->source = path;
	__v_result_blip__Value __or_opt_7 = machine__Machine__run_str(m, src);
	if (!__or_opt_7.ok) {
		IError err = __or_opt_7.err;
		return (__v_result_string){.ok = false, .err = (IError){._typ = 71273906, ._object = (MessageError*)memdup(&(MessageError){.msg = IError__msg(&(err)), .code = 0}, sizeof(MessageError)), ._object_is_boxed = true}};
	}
	string out = _v3_lit_0_14650fb0739d0383;
	{
		i64 __for_idx_8 = 0;
		for (; __for_idx_8 < m->out.len; __for_idx_8++) {
			string line = *(string*)(array_get(m->out, __for_idx_8));
			out = string__plus(out, line);
			out = string__plus(out, _v3_lit_1_44bd31d473cd01cb);
		}
	}
	return (__v_result_string){.ok = true, .value = out};
	return (__v_result_string){.ok = true};
}

/* V3CACHE_FN_END 6e4ea2d6fc1e63fc */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 771bcc34f758b3a0 */
Array rune__bytes(u32 c) {
}

/* V3CACHE_FN_END 771bcc34f758b3a0 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN c6c08188333e5cec */
string rune__str(u32 c) {
}

/* V3CACHE_FN_END c6c08188333e5cec */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN e73f63fdab9bae4 */
u32 rune__to_lower(u32 c) {
}

/* V3CACHE_FN_END e73f63fdab9bae4 */
/* V3CACHE_MODULE strconv */
/* V3CACHE_FN_BEGIN 51171b42d83a5308 */
string strconv__Dec32__get_string_32(strconv__Dec32 d, bool neg, i64 i_n_digit, i64 i_pad_digit) {
}

/* V3CACHE_FN_END 51171b42d83a5308 */
/* V3CACHE_MODULE strconv */
/* V3CACHE_FN_BEGIN 2abb6be42d803164 */
string strconv__Dec64__get_string_64(strconv__Dec64 d, bool neg, i64 i_n_digit, i64 i_pad_digit) {
}

/* V3CACHE_FN_END 2abb6be42d803164 */
/* V3CACHE_MODULE strconv */
/* V3CACHE_FN_BEGIN 6993baae6fc01473 */
__v_result_double strconv__atof64(string s, strconv__AtoF64Param param) {
	return (__v_result_double){.ok = true};
}

/* V3CACHE_FN_END 6993baae6fc01473 */
/* V3CACHE_MODULE strconv */
/* V3CACHE_FN_BEGIN 6686180ca5e440ba */
__v_result_i64 strconv__atoi(string s) {
	return (__v_result_i64){.ok = true};
}

/* V3CACHE_FN_END 6686180ca5e440ba */
/* V3CACHE_MODULE strconv */
/* V3CACHE_FN_BEGIN 24c37ce990316384 */
__v_result_i64 strconv__atoi64(string s) {
	return (__v_result_i64){.ok = true};
}

/* V3CACHE_FN_END 24c37ce990316384 */
/* V3CACHE_MODULE strconv */
/* V3CACHE_FN_BEGIN b6aa098e930774cb */
string strconv__f32_to_str_l(float f) {
}

/* V3CACHE_FN_END b6aa098e930774cb */
/* V3CACHE_MODULE strconv */
/* V3CACHE_FN_BEGIN 25f771e8099d4fbc */
string strconv__f64_to_str_l(double f) {
}

/* V3CACHE_FN_END 25f771e8099d4fbc */
/* V3CACHE_MODULE strconv */
/* V3CACHE_FN_BEGIN 14a63b02e15e709a */
string strconv__format_int(i64 n, i64 radix) {
}

/* V3CACHE_FN_END 14a63b02e15e709a */
/* V3CACHE_MODULE strconv */
/* V3CACHE_FN_BEGIN 2d9cbf77a5c0069d */
string strconv__format_uint(u64 n, i64 radix) {
}

/* V3CACHE_FN_END 2d9cbf77a5c0069d */
/* V3CACHE_MODULE strconv */
/* V3CACHE_FN_BEGIN edf39031acd25a21 */
__v_result_u64 strconv__parse_uint(string s, i64 _base, i64 _bit_size) {
	return (__v_result_u64){.ok = true};
}

/* V3CACHE_FN_END edf39031acd25a21 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN aad8d2ab851281d0 */
string string__all_after(string s, string sub) {
}

/* V3CACHE_FN_END aad8d2ab851281d0 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN c5fdff0935c4d64d */
string string__all_after_last(string s, string sub) {
}

/* V3CACHE_FN_END c5fdff0935c4d64d */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 30cf751e0216fd6d */
string string__all_before(string s, string sub) {
}

/* V3CACHE_FN_END 30cf751e0216fd6d */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 389f56d03695c7ca */
string string__all_before_last(string s, string sub) {
}

/* V3CACHE_FN_END 389f56d03695c7ca */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 6d7b5216b5a368f7 */
u8 string__at(string s, i64 idx) {
}

/* V3CACHE_FN_END 6d7b5216b5a368f7 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN a1beb1dd4e9119dd */
u8 string__at_i64(string s, i64 idx) {
}

/* V3CACHE_FN_END a1beb1dd4e9119dd */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN c6024c470962fe8b */
u8 string__at_ni(string s, i64 idx) {
}

/* V3CACHE_FN_END c6024c470962fe8b */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN f1e720f4562d3b89 */
u8 string__at_u64(string s, u64 idx) {
}

/* V3CACHE_FN_END f1e720f4562d3b89 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 4c86afa6354ca1d7 */
Array string__bytes(string s) {
}

/* V3CACHE_FN_END 4c86afa6354ca1d7 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN a2a7d2f0ad19fa21 */
string string__clone(string a) {
}

/* V3CACHE_FN_END a2a7d2f0ad19fa21 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 5ff0f1b1d9cd7ef3 */
bool string__contains(string s, string substr) {
}

/* V3CACHE_FN_END 5ff0f1b1d9cd7ef3 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 93f42c8415c1fe2f */
bool string__ends_with(string s, string p) {
}

/* V3CACHE_FN_END 93f42c8415c1fe2f */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 4476c61851394362 */
bool string__eq(string s, string a) {
}

/* V3CACHE_FN_END 4476c61851394362 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN ce56b0f10a4f138e */
void string__free(string* s) {
}

/* V3CACHE_FN_END ce56b0f10a4f138e */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 57c9d07b21d5f876 */
__v_option_i64 string__index(string s, string p) {
	return (__v_option_i64){.ok = true};
}

/* V3CACHE_FN_END 57c9d07b21d5f876 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN adba8717d0aee0d1 */
bool string__is_capital(string s) {
}

/* V3CACHE_FN_END adba8717d0aee0d1 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 5f85710c6f848eb1 */
inline __v_option_i64 string__last_index(string s, string needle) {
	return (__v_option_i64){.ok = true};
}

/* V3CACHE_FN_END 5f85710c6f848eb1 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN d8354b5a18ea0a1a */
bool string__lt(string s, string a) {
}

/* V3CACHE_FN_END d8354b5a18ea0a1a */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN d8034b5a18c06381 */
string string__plus(string s, string a) {
}

/* V3CACHE_FN_END d8034b5a18c06381 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 7bcf64c4023c876d */
string string__repeat(string s, i64 count) {
}

/* V3CACHE_FN_END 7bcf64c4023c876d */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 21160dcd952ae79a */
string string__replace(string s, string rep, string with) {
}

/* V3CACHE_FN_END 21160dcd952ae79a */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 1bf8ef796247069e */
inline Array string__split(string s, string delim) {
}

/* V3CACHE_FN_END 1bf8ef796247069e */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 3d65f13d2d04ca8 */
bool string__starts_with(string s, string p) {
}

/* V3CACHE_FN_END 3d65f13d2d04ca8 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN cef57108be1657b3 */
inline string string__str(string s) {
}

/* V3CACHE_FN_END cef57108be1657b3 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 92bb7430fbdf8af7 */
string string__substr(string s, i64 start, i64 _end) {
}

/* V3CACHE_FN_END 92bb7430fbdf8af7 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN ced3764e4b49b48b */
string string__substr_ni(string s, i64 _start, i64 _end) {
}

/* V3CACHE_FN_END ced3764e4b49b48b */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN c1ae5a75ec8c3d9 */
string string__to_lower(string s) {
}

/* V3CACHE_FN_END c1ae5a75ec8c3d9 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN bac42a24944e71b9 */
string string__to_lower_ascii(string s) {
}

/* V3CACHE_FN_END bac42a24944e71b9 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 2e8c4004c736ffa */
string string__to_upper(string s) {
}

/* V3CACHE_FN_END 2e8c4004c736ffa */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 68f17335f44afb5b */
inline string string__trim_space(string s) {
}

/* V3CACHE_FN_END 68f17335f44afb5b */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 9fbc0c60ddeb3c23 */
string string_plus_many(i64 data_len, string* input_base) {
}

/* V3CACHE_FN_END 9fbc0c60ddeb3c23 */
/* V3CACHE_MODULE strings */
/* V3CACHE_FN_BEGIN 65bfc01578694daa */
void strings__Builder__free(Array* b) {
}

/* V3CACHE_FN_END 65bfc01578694daa */
/* V3CACHE_MODULE strings */
/* V3CACHE_FN_BEGIN 5feecdd2df832a17 */
string strings__Builder__str(Array* b) {
}

/* V3CACHE_FN_END 5feecdd2df832a17 */
/* V3CACHE_MODULE strings */
/* V3CACHE_FN_BEGIN edc4c9ccc2cae580 */
void strings__Builder__write_ptr(Array* b, u8* ptr, i64 len) {
}

/* V3CACHE_FN_END edc4c9ccc2cae580 */
/* V3CACHE_MODULE strings */
/* V3CACHE_FN_BEGIN 216c7ff11db691d */
void strings__Builder__write_runes(Array* b, Array runes) {
}

/* V3CACHE_FN_END 216c7ff11db691d */
/* V3CACHE_MODULE strings */
/* V3CACHE_FN_BEGIN d436fc8602af45eb */
inline void strings__Builder__write_string(Array* b, string s) {
}

/* V3CACHE_FN_END d436fc8602af45eb */
/* V3CACHE_MODULE strings */
/* V3CACHE_FN_BEGIN d8a3011cc6d33a73 */
inline void strings__Builder__write_u8(Array* b, u8 data) {
}

/* V3CACHE_FN_END d8a3011cc6d33a73 */
/* V3CACHE_MODULE strings */
/* V3CACHE_FN_BEGIN e1fc6ba514f343e7 */
Array strings__new_builder(i64 initial_size) {
}

/* V3CACHE_FN_END e1fc6ba514f343e7 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 6419a1bad2a07701 */
string tos(u8* s, i64 len) {
}

/* V3CACHE_FN_END 6419a1bad2a07701 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 991346ce4dd42fd0 */
string u128__str(u128 nn) {
}

/* V3CACHE_FN_END 991346ce4dd42fd0 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN ca01e7d7c5aed2e8 */
string u16__str(u16 n) {
}

/* V3CACHE_FN_END ca01e7d7c5aed2e8 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN dc277f34542cf6de */
inline string u32__str(u32 nn) {
}

/* V3CACHE_FN_END dc277f34542cf6de */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN ce3d4fd2986b025f */
inline string u64__str(u64 nn) {
}

/* V3CACHE_FN_END ce3d4fd2986b025f */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 520c12e57ea672a9 */
string u8__hex(u8 nn) {
}

/* V3CACHE_FN_END 520c12e57ea672a9 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 1f8fe71add3fde15 */
inline bool u8__is_capital(u8 c) {
}

/* V3CACHE_FN_END 1f8fe71add3fde15 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 92de461bf24e75d5 */
inline bool u8__is_letter(u8 c) {
}

/* V3CACHE_FN_END 92de461bf24e75d5 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN f16b5a0ee54d14df */
string u8__str(u8 b) {
}

/* V3CACHE_FN_END f16b5a0ee54d14df */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 79cc1af48db1fa3f */
string u8__vstring(u8* bp) {
}

/* V3CACHE_FN_END 79cc1af48db1fa3f */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 562ccc34a2499bc0 */
string u8__vstring_with_len(u8* bp, i64 len) {
}

/* V3CACHE_FN_END 562ccc34a2499bc0 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN be2d20dcc428333a */
string usize__str(size_t x) {
}

/* V3CACHE_FN_END be2d20dcc428333a */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN de4bd470e750e457 */
VNORETURN void v_exit(i64 code) {
}

/* V3CACHE_FN_END de4bd470e750e457 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 83f8779ba5841bb7 */
inline i64 v_fixed_index(i64 i, i64 len) {
}

/* V3CACHE_FN_END 83f8779ba5841bb7 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN fc92c9b896fde99d */
inline i64 v_fixed_index_i64(i64 i, i64 len) {
}

/* V3CACHE_FN_END fc92c9b896fde99d */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 85b8789e0ba16e4b */
inline i64 v_fixed_index_ni(i64 i, i64 len) {
}

/* V3CACHE_FN_END 85b8789e0ba16e4b */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 4cb938cf9e96a549 */
inline i64 v_fixed_index_u64(u64 i, i64 len) {
}

/* V3CACHE_FN_END 4cb938cf9e96a549 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 9ccd2c51146824fd */
void v_free(void* ptr) {
}

/* V3CACHE_FN_END 9ccd2c51146824fd */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN eb68d7910db8dda2 */
VNORETURN void v_panic(string s) {
}

/* V3CACHE_FN_END eb68d7910db8dda2 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN f9da893f4fe01d2d */
inline i64 v_slice_index_i64(i64 i) {
}

/* V3CACHE_FN_END f9da893f4fe01d2d */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 1fd1795fe4e9f739 */
inline i64 v_slice_index_u64(u64 i) {
}

/* V3CACHE_FN_END 1fd1795fe4e9f739 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN ce148ad37aff7b16 */
inline void* vmemcpy(void* dest, void* const_src, ptrdiff_t n) {
}

/* V3CACHE_FN_END ce148ad37aff7b16 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN fc42d5dbe49e60c0 */
string voidptr__str(void* nn) {
}

/* V3CACHE_FN_END fc42d5dbe49e60c0 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 1f7938591f09bfca */
Array voidptr__vbytes(void* data, i64 len) {
}

/* V3CACHE_FN_END 1f7938591f09bfca */
// THE END.
/* V3CACHE_BODY_END */
