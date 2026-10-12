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
typedef struct dl__InterfaceExport dl__InterfaceExport;
typedef struct dl__InterfaceExportTable dl__InterfaceExportTable;
typedef struct map map;
typedef struct os__ContextRecord os__ContextRecord;
typedef struct os__Eof os__Eof;
typedef struct os__ExceptionPointers os__ExceptionPointers;
typedef struct os__ExceptionRecord os__ExceptionRecord;
typedef struct os__ExecutableNotFoundError os__ExecutableNotFoundError;
typedef struct os__FileNotOpenedError os__FileNotOpenedError;
typedef struct os__NotExpected os__NotExpected;
typedef struct os__SizeOfTypeIs0Error os__SizeOfTypeIs0Error;
typedef struct os__WalkDirEntry os__WalkDirEntry;
typedef struct strconv__Dec32 strconv__Dec32;
typedef struct strconv__Dec64 strconv__Dec64;
typedef struct strconv__PrepNumber strconv__PrepNumber;
typedef struct string string;
typedef struct IError IError;
typedef struct hash__Hash hash__Hash;
typedef struct hash__Hash32er hash__Hash32er;
typedef struct hash__Hash64er hash__Hash64er;
typedef array Array;

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
typedef struct multi_return_double_i64 multi_return_double_i64;

typedef double Array_fixed_double_36[36];
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
typedef u8 Array_fixed_u8_256[256];
typedef u8 Array_fixed_u8_32[32];
typedef u8 Array_fixed_u8_364[364];
typedef u8 Array_fixed_u8_64[64];
typedef u8 Array_fixed_u8_65536[65536];
typedef void* Array_fixed_voidptr_11[11];
typedef void* Array_fixed_voidptr_64[64];
typedef void* Array_fixed_voidptr_8[8];

typedef bool (*_fn_ptr_c95f092653bee64)(void*, void*);
typedef i64 (*_fn_ptr_ee0265116ec69588)(ExceptionPointers*);
typedef i64 (*_fn_ptr_ed95774d3a4e97ab)(void*, void*);
typedef int (*_fn_ptr_24927a217aab3552)(string, os__WalkDirEntry);
typedef u32 (*_fn_ptr_f33ca158569fc3fd)(os__ExceptionPointers*);
typedef u64 (*_fn_ptr_1e8d64a3d95a0da1)(void*);
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
typedef struct dl__InterfaceExport dl__InterfaceExport;
typedef struct dl__InterfaceExportTable dl__InterfaceExportTable;
typedef struct map map;
typedef struct os__ContextRecord os__ContextRecord;
typedef struct os__Eof os__Eof;
typedef struct os__ExceptionPointers os__ExceptionPointers;
typedef struct os__ExceptionRecord os__ExceptionRecord;
typedef struct os__ExecutableNotFoundError os__ExecutableNotFoundError;
typedef struct os__FileNotOpenedError os__FileNotOpenedError;
typedef struct os__NotExpected os__NotExpected;
typedef struct os__SizeOfTypeIs0Error os__SizeOfTypeIs0Error;
typedef struct os__WalkDirEntry os__WalkDirEntry;
typedef struct strconv__Dec32 strconv__Dec32;
typedef struct strconv__Dec64 strconv__Dec64;
typedef struct strconv__PrepNumber strconv__PrepNumber;
typedef struct string string;
typedef struct IError IError;
typedef struct hash__Hash hash__Hash;
typedef struct hash__Hash32er hash__Hash32er;
typedef struct hash__Hash64er hash__Hash64er;
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
	i64 checks;
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
struct multi_return_double_i64 {
	double arg0;
	i64 arg1;
};

typedef struct __v_option_i64 { bool ok; i64 value; } __v_option_i64;
typedef struct __v_option_string { bool ok; string value; } __v_option_string;
typedef struct __v_result_Array { bool ok; union { IError err; Array value; }; } __v_result_Array;
typedef struct __v_result_i64 { bool ok; union { IError err; i64 value; }; } __v_result_i64;
typedef struct __v_result_string { bool ok; union { IError err; string value; }; } __v_result_string;
typedef struct __v_result_u32 { bool ok; union { IError err; u32 value; }; } __v_result_u32;

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
dl__InterfaceExportTable* dl__g_dl_interface_exports;

void __v3_cache_bits__init_consts(void);
void __v3_cache_bits__init_consts_defaults(void);
void __v3_cache_builtin__init_consts(void);
void __v3_cache_builtin__init_consts_defaults(void);
void __v3_cache_dl__init_consts(void);
void __v3_cache_dl__init_consts_defaults(void);
void __v3_cache_hash__init_consts(void);
void __v3_cache_hash__init_consts_defaults(void);
void __v3_cache_os__init_consts(void);
void __v3_cache_os__init_consts_defaults(void);
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
i64 Error__code(Error err);
string Error__msg(Error err);
string IError__str(IError err);
i64 MessageError__code(MessageError err);
string MessageError__msg(MessageError err);
string StrIntpType_str(int x);
void Suite__record(main__Suite* s, bool ok, string good, string bad);
void* __as_cast(void* obj, i64 obj_type, i64 expected_type, string obj_name, string expected_name);
array __new_array(i64 mylen, i64 cap, i64 elm_size);
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
Array byteptr__vbytes(u8* data, i64 len);
string byteptr__vstring(u8* bp);
string byteptr__vstring_with_len(u8* bp, i64 len);
string charptr__vstring(char* cp);
string charptr__vstring_with_len(char* cp, i64 len);
void collect(string dir, Array* out);
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
bool gc_prepare_for_debugger_init(void);
void gc_restore_roots_after_debugger_init(bool use_manual_roots);
void gc_runtime_init(void);
string i16__str(i16 n);
string i32__str(i32 n);
string i64__str(i64 nn);
string i8__str(i8 n);
u32 impl_utf8_to_utf32(u8* _bytes, i64 _bytes_len);
string int__str(i64 n);
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
void* memdup(void* src, ptrdiff_t sz);
void* memdup_align(void* src, ptrdiff_t sz, ptrdiff_t align);
array new_array_from_c_array(i64 len, i64 cap, i64 elm_size, void* c_array);
array new_array_from_c_array_noscan(i64 len, i64 cap, i64 elm_size, void* c_array);
map new_map(i64 key_bytes, i64 value_bytes, _fn_ptr_1e8d64a3d95a0da1 hash_fn, _fn_ptr_c95f092653bee64 key_eq_fn, _fn_ptr_1a3da2026b1489ea clone_fn, _fn_ptr_5373f7edc7b60e26 free_fn);
string os__ExecutableNotFoundError__msg(os__ExecutableNotFoundError err);
string os__FileNotOpenedError__msg(os__FileNotOpenedError err);
i64 os__NotExpected__code(os__NotExpected err);
string os__NotExpected__msg(os__NotExpected err);
string os__SizeOfTypeIs0Error__msg(os__SizeOfTypeIs0Error err);
bool os__exists(string path);
string os__file_name(string path);
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
void println(string s);
string ptr_str(void* ptr);
Array pub_fns(string dir);
string read_corpus(Array paths);
Array rune__bytes(u32 c);
string rune__str(u32 c);
u32 rune__to_lower(u32 c);
string strconv__Dec32__get_string_32(strconv__Dec32 d, bool neg, i64 i_n_digit, i64 i_pad_digit);
string strconv__Dec64__get_string_64(strconv__Dec64 d, bool neg, i64 i_n_digit, i64 i_pad_digit);
string strconv__f32_to_str_l(float f);
string strconv__f64_to_str_l(double f);
string strconv__format_int(i64 n, i64 radix);
string strconv__format_uint(u64 n, i64 radix);
string string__all_after(string s, string sub);
string string__all_after_last(string s, string sub);
string string__all_before(string s, string sub);
string string__all_before_last(string s, string sub);
u8 string__at(string s, i64 idx);
u8 string__at_i64(string s, i64 idx);
u8 string__at_ni(string s, i64 idx);
u8 string__at_u64(string s, u64 idx);
string string__clone(string a);
bool string__contains(string s, string substr);
bool string__ends_with(string s, string p);
bool string__eq(string s, string a);
void string__free(string* s);
__v_option_i64 string__index(string s, string p);
bool string__is_capital(string s);
bool string__lt(string s, string a);
string string__plus(string s, string a);
Array string__split_into_lines(string s);
bool string__starts_with(string s, string p);
string string__substr(string s, i64 start, i64 _end);
string string__substr_ni(string s, i64 _start, i64 _end);
string string__to_lower_ascii(string s);
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
string u64__str(u64 nn);
bool u8__is_capital(u8 c);
bool u8__is_letter(u8 c);
string u8__vstring(u8* bp);
string u8__vstring_with_len(u8* bp, i64 len);
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
Array voidptr__vbytes(void* data, i64 len);

i64 Error__code(Error err);
string Error__msg(Error err);
string IError__str(IError err);
i64 MessageError__code(MessageError err);
string MessageError__msg(MessageError err);
string StrIntpType_str(int x);
void* __as_cast(void* obj, i64 obj_type, i64 expected_type, string obj_name, string expected_name);
array __new_array(i64 mylen, i64 cap, i64 elm_size);
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
Array byteptr__vbytes(u8* data, i64 len);
string byteptr__vstring(u8* bp);
string byteptr__vstring_with_len(u8* bp, i64 len);
string charptr__vstring(char* cp);
string charptr__vstring_with_len(char* cp, i64 len);
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
bool gc_prepare_for_debugger_init(void);
void gc_restore_roots_after_debugger_init(bool use_manual_roots);
void gc_runtime_init(void);
string i16__str(i16 n);
string i32__str(i32 n);
string i64__str(i64 nn);
string i8__str(i8 n);
u32 impl_utf8_to_utf32(u8* _bytes, i64 _bytes_len);
string int__str(i64 n);
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
void* memdup(void* src, ptrdiff_t sz);
void* memdup_align(void* src, ptrdiff_t sz, ptrdiff_t align);
array new_array_from_c_array(i64 len, i64 cap, i64 elm_size, void* c_array);
array new_array_from_c_array_noscan(i64 len, i64 cap, i64 elm_size, void* c_array);
map new_map(i64 key_bytes, i64 value_bytes, _fn_ptr_1e8d64a3d95a0da1 hash_fn, _fn_ptr_c95f092653bee64 key_eq_fn, _fn_ptr_1a3da2026b1489ea clone_fn, _fn_ptr_5373f7edc7b60e26 free_fn);
string os__ExecutableNotFoundError__msg(os__ExecutableNotFoundError err);
string os__FileNotOpenedError__msg(os__FileNotOpenedError err);
i64 os__NotExpected__code(os__NotExpected err);
string os__NotExpected__msg(os__NotExpected err);
string os__SizeOfTypeIs0Error__msg(os__SizeOfTypeIs0Error err);
bool os__exists(string path);
string os__file_name(string path);
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
void println(string s);
string ptr_str(void* ptr);
Array rune__bytes(u32 c);
string rune__str(u32 c);
u32 rune__to_lower(u32 c);
string strconv__Dec32__get_string_32(strconv__Dec32 d, bool neg, i64 i_n_digit, i64 i_pad_digit);
string strconv__Dec64__get_string_64(strconv__Dec64 d, bool neg, i64 i_n_digit, i64 i_pad_digit);
string strconv__f32_to_str_l(float f);
string strconv__f64_to_str_l(double f);
string strconv__format_int(i64 n, i64 radix);
string strconv__format_uint(u64 n, i64 radix);
string string__all_after(string s, string sub);
string string__all_after_last(string s, string sub);
string string__all_before(string s, string sub);
string string__all_before_last(string s, string sub);
u8 string__at(string s, i64 idx);
u8 string__at_i64(string s, i64 idx);
u8 string__at_ni(string s, i64 idx);
u8 string__at_u64(string s, u64 idx);
string string__clone(string a);
bool string__contains(string s, string substr);
bool string__ends_with(string s, string p);
bool string__eq(string s, string a);
void string__free(string* s);
__v_option_i64 string__index(string s, string p);
bool string__is_capital(string s);
bool string__lt(string s, string a);
string string__plus(string s, string a);
Array string__split_into_lines(string s);
bool string__starts_with(string s, string p);
string string__substr(string s, i64 start, i64 _end);
string string__substr_ni(string s, i64 _start, i64 _end);
string string__to_lower_ascii(string s);
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
string u64__str(u64 nn);
bool u8__is_capital(u8 c);
bool u8__is_letter(u8 c);
string u8__vstring(u8* bp);
string u8__vstring_with_len(u8* bp, i64 len);
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

static const string _v3_lit_0_14650fb0739d0383 = {"", 0, 1};
static const string _v3_lit_1_44bd31d473cd01cb = {"\n", 1, 1};
static const string _v3_lit_1_44bd4bd473cd2df9 = {" ", 1, 1};
static const string _v3_lit_16_2b06bc7dcefc0449 = {"   not engaged: ", 16, 1};
static const string _v3_lit_11_d00d55f7f09898a7 = {" FAILURE(S)", 11, 1};
static const string _v3_lit_34_a59492d68476e0ec = {" is engaged by a test or a program", 34, 1};
static const string _v3_lit_17_6776d00fb840cc45 = {" is in the corpus", 17, 1};
static const string _v3_lit_27_73f31a94c2a48618 = {" is missing from the corpus", 27, 1};
static const string _v3_lit_36_cbaaea8a426a0c7d = {" public functions found in vlib/blip", 36, 1};
static const string _v3_lit_1_44bd53d473cd3b91 = {"(", 1, 1};
static const string _v3_lit_1_44bd52d473cd39de = {")", 1, 1};
static const string _v3_lit_2_9afebb00c5c7fa67 = {", ", 2, 1};


static const string _v3_lit_4_f4cf6cfcb2922dbd = {".git", 4, 1};
static const string _v3_lit_2_9af75900c5c13533 = {".v", 2, 1};








static const string _v3_lit_13_36c3f301c017bfb1 = {"01_basics.lip", 13, 1};
static const string _v3_lit_11_a8eac674b06d0881 = {"02_data.lip", 11, 1};

static const string _v3_lit_12_c5858cb0700d9517 = {"03_match.lip", 12, 1};
static const string _v3_lit_13_f165c16b849e6abc = {"04_errors.lip", 13, 1};
static const string _v3_lit_16_dc26546f10464cc7 = {"05_pipelines.lip", 16, 1};
static const string _v3_lit_14_a895830e99b6f118 = {"06_modules.lip", 14, 1};







static const string _v3_lit_5_6b447c763a12a2d = {"FAIL ", 5, 1};



static const string _v3_lit_7_622193d14801b764 = {"_test.v", 7, 1};




static const string _v3_lit_31_f2ed3b4015f82701 = {"api coverage: all checks passed", 31, 1};
static const string _v3_lit_74_82fc39ccb1421152 = {"attempted to decode too many bytes, utf-8 is limited to four bytes maximum", 74, 1};
static const string _v3_lit_15_f649e8e7605cf4d4 = {"binding_forms.v", 15, 1};
static const string _v3_lit_6_cdea6911cc47f68 = {"blip.v", 6, 1};
static const string _v3_lit_12_e11aadbed58b02ce = {"blip_modules", 12, 1};
static const string _v3_lit_13_36ad6a993b8bd625 = {"brainfuck.lip", 13, 1};
static const string _v3_lit_4_2cf6d4e42e437073 = {"dist", 4, 1};
static const string _v3_lit_11_334da7bc7c829e2c = {"embedding.v", 11, 1};
static const string _v3_lit_13_2a97e542b15fa56d = {"every one of ", 13, 1};
static const string _v3_lit_8_fd339d8b807891d0 = {"examples", 8, 1};
static const string _v3_lit_14_bee16b8dd505c6f6 = {"examples_run.v", 14, 1};
static const string _v3_lit_10_2094d6939b7cf8c4 = {"genprims.v", 10, 1};
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
static const string _v3_lit_8_83580fe0367d241a = {"json.lip", 8, 1};
static const string _v3_lit_64_c288b68a776176dc = {"no public functions found -- the scan is broken, not the library", 64, 1};
static const string _v3_lit_12_d63d354d67eafb0d = {"node_modules", 12, 1};
static const string _v3_lit_5_9aa8c9aed04546bf = {"ok   ", 5, 1};
static const string _v3_lit_8_3ad8ed42f15f6952 = {"programs", 8, 1};
static const string _v3_lit_14_dd326b997a810a04 = {"programs_run.v", 14, 1};
static const string _v3_lit_7_59848b6e4ae2099e = {"pub fn ", 7, 1};
static const string _v3_lit_14_c43d81be0ba6ca4d = {"reader_probe.v", 14, 1};
static const string _v3_lit_13_8043bbdded23052a = {"repl_golden.v", 13, 1};
static const string _v3_lit_8_8ac0c356cefdcd94 = {"server.v", 8, 1};
static const string _v3_lit_8_9cfb24994a8d562f = {"site_out", 8, 1};
static const string _v3_lit_12_77799fddea2c1f30 = {"some are not", 12, 1};
static const string _v3_lit_12_bc3eb26b5388048f = {"tail_calls.v", 12, 1};
static const string _v3_lit_5_64086fe48f6be28c = {"tests", 5, 1};
static const string _v3_lit_3_96876351741784e8 = {"tmp", 3, 1};
static const string _v3_lit_1_44bd9dd473cdb94f = {"v", 1, 1};
static const string _v3_lit_9_29c80a4b32576b2c = {"vlib/blip", 9, 1};

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

extern IError builtin__none__;
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
	__v3_cache_hash__init_consts_defaults();
	__v3_cache_builtin__init_consts_defaults();
	__v3_cache_dl__init_consts_defaults();
	__v3_cache_textscanner__init_consts_defaults();
	__v3_cache_validate__init_consts_defaults();
	__v3_cache_os__init_consts_defaults();
	__v3_cache_bits__init_consts();
	__v3_cache_strings__init_consts();
	__v3_cache_strconv__init_consts();
	__v3_cache_hash__init_consts();
	__v3_cache_builtin__init_consts();
	builtin_init();
	__v3_cache_dl__init_consts();
	__v3_cache_textscanner__init_consts();
	__v3_cache_validate__init_consts();
	__v3_cache_os__init_consts();
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
/* V3CACHE_FN_BEGIN cdece776767d8361 */
string IError__str(IError err) {
}

/* V3CACHE_FN_END cdece776767d8361 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 1c545a3f873bab44 */
string StrIntpType_str(int x) {
}

/* V3CACHE_FN_END 1c545a3f873bab44 */
/* V3CACHE_MODULE main */
/* V3CACHE_FN_BEGIN 3ef97d32e9c21b9d */
void Suite__record(main__Suite* s, bool ok, string good, string bad) {
	s->checks++;
	if (ok) {
		println(string__plus(_v3_lit_5_9aa8c9aed04546bf, good));
	} else {
		s->fails++;
		println(string__plus(_v3_lit_5_6b447c763a12a2d, bad));
	}
}

/* V3CACHE_FN_END 3ef97d32e9c21b9d */
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
/* V3CACHE_FN_BEGIN 89051a2808efeac6 */
string charptr__vstring(char* cp) {
}

/* V3CACHE_FN_END 89051a2808efeac6 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN c6d0f14d7d53446b */
string charptr__vstring_with_len(char* cp, i64 len) {
}

/* V3CACHE_FN_END c6d0f14d7d53446b */
/* V3CACHE_MODULE main */
/* V3CACHE_FN_BEGIN 54cf552e71a1040 */
void collect(string dir, Array* out) {
	__v_result_Array __or_opt_0 = os__ls(dir);
	Array __or_val_1 = {0};
	if (__or_opt_0.ok) {
		__or_val_1 = __or_opt_0.value;
	} else {
		IError err = __or_opt_0.err;
		return;
	}
	Array entries = __or_val_1;
	{
		i64 __for_idx_2 = 0;
		for (; __for_idx_2 < entries.len; __for_idx_2++) {
			string e = *(string*)(array_get(entries, __for_idx_2));
			string p = os__join_path_single(dir, e);
			if (os__is_dir(p)) {
				if (string__eq(e, _v3_lit_4_f4cf6cfcb2922dbd) || string__eq(e, _v3_lit_8_9cfb24994a8d562f) || string__eq(e, _v3_lit_1_44bd9dd473cdb94f) || string__eq(e, _v3_lit_3_96876351741784e8) || string__eq(e, _v3_lit_4_2cf6d4e42e437073) || string__eq(e, _v3_lit_12_d63d354d67eafb0d) || string__eq(e, _v3_lit_12_e11aadbed58b02ce)) {
					continue;
				}
				collect(p, out);
				continue;
			}
			string __arr_val_3 = string__clone(p);
			array_push(out, &__arr_val_3);
		}
	}
}

/* V3CACHE_FN_END 54cf552e71a1040 */
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
/* V3CACHE_MODULE main */
/* V3CACHE_FN_BEGIN 2cc9312c745b4f57 */
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
	Array names = pub_fns(_v3_lit_9_29c80a4b32576b2c);
	Array corpus_files = array_new(	sizeof(string), 0, 0);
	collect(_v3_lit_5_64086fe48f6be28c, &corpus_files);
	Array __in_lhs_0 = new_array_from_c_array(3, 3, sizeof(string), (string[3]){_v3_lit_6_cdea6911cc47f68, _v3_lit_8_8ac0c356cefdcd94, _v3_lit_14_c43d81be0ba6ca4d});
	{
		i64 __for_idx_1 = 0;
		for (; __for_idx_1 < __in_lhs_0.len; __for_idx_1++) {
			string cli = *(string*)(array_get(__in_lhs_0, __for_idx_1));
			if (os__exists(cli)) {
				string __arr_val_2 = string__clone(cli);
				array_push(&corpus_files, &__arr_val_2);
			}
		}
	}
	Array __arr_lit_3 = array_new(	sizeof(string), 0, 8);
	string __arr_val_4 = os__join_path_single(_v3_lit_8_fd339d8b807891d0, _v3_lit_13_36c3f301c017bfb1);
	array_push(&__arr_lit_3, &__arr_val_4);
	string __arr_val_5 = os__join_path_single(_v3_lit_8_fd339d8b807891d0, _v3_lit_11_a8eac674b06d0881);
	array_push(&__arr_lit_3, &__arr_val_5);
	string __arr_val_6 = os__join_path_single(_v3_lit_8_fd339d8b807891d0, _v3_lit_12_c5858cb0700d9517);
	array_push(&__arr_lit_3, &__arr_val_6);
	string __arr_val_7 = os__join_path_single(_v3_lit_8_fd339d8b807891d0, _v3_lit_13_f165c16b849e6abc);
	array_push(&__arr_lit_3, &__arr_val_7);
	string __arr_val_8 = os__join_path_single(_v3_lit_8_fd339d8b807891d0, _v3_lit_16_dc26546f10464cc7);
	array_push(&__arr_lit_3, &__arr_val_8);
	string __arr_val_9 = os__join_path_single(_v3_lit_8_fd339d8b807891d0, _v3_lit_14_a895830e99b6f118);
	array_push(&__arr_lit_3, &__arr_val_9);
	string __arr_val_10 = os__join_path_single(_v3_lit_8_3ad8ed42f15f6952, _v3_lit_13_36ad6a993b8bd625);
	array_push(&__arr_lit_3, &__arr_val_10);
	string __arr_val_11 = os__join_path_single(_v3_lit_8_3ad8ed42f15f6952, _v3_lit_8_83580fe0367d241a);
	array_push(&__arr_lit_3, &__arr_val_11);
	{
		i64 __for_idx_12 = 0;
		for (; __for_idx_12 < __arr_lit_3.len; __for_idx_12++) {
			string extra = *(string*)(array_get(__arr_lit_3, __for_idx_12));
			if (os__exists(extra)) {
				string __arr_val_13 = string__clone(extra);
				array_push(&corpus_files, &__arr_val_13);
			}
		}
	}
	string corpus = read_corpus(corpus_files);
	Array missing = array_new(	sizeof(string), 0, 0);
	{
		i64 __for_idx_14 = 0;
		for (; __for_idx_14 < names.len; __for_idx_14++) {
			string n = *(string*)(array_get(names, __for_idx_14));
			if (!string__contains(corpus, string__plus(n, _v3_lit_1_44bd53d473cd3b91))) {
				string __arr_val_15 = string__clone(n);
				array_push(&missing, &__arr_val_15);
			}
		}
	}
	Suite__record(&s, names.len > 0, string__plus(int__str(names.len), _v3_lit_36_cbaaea8a426a0c7d), _v3_lit_64_c288b68a776176dc);
	if (missing.len == 0) {
		Suite__record(&s, true, ({ string __v3_internal_symbol_join_0[3]; __v3_internal_symbol_join_0[0] = _v3_lit_13_2a97e542b15fa56d; __v3_internal_symbol_join_0[1] = int__str(names.len); __v3_internal_symbol_join_0[2] = _v3_lit_34_a59492d68476e0ec; string_plus_many(3, __v3_internal_symbol_join_0); }), _v3_lit_12_77799fddea2c1f30);
	} else {
		println(string__plus(_v3_lit_16_2b06bc7dcefc0449, Array_string__join(missing, _v3_lit_1_44bd4bd473cd2df9)));
		s.fails++;
	}
	Array __in_lhs_16 = new_array_from_c_array(7, 7, sizeof(string), (string[7]){_v3_lit_12_bc3eb26b5388048f, _v3_lit_15_f649e8e7605cf4d4, _v3_lit_11_334da7bc7c829e2c, _v3_lit_13_8043bbdded23052a, _v3_lit_14_bee16b8dd505c6f6, _v3_lit_14_dd326b997a810a04, _v3_lit_10_2094d6939b7cf8c4});
	{
		i64 __for_idx_17 = 0;
		for (; __for_idx_17 < __in_lhs_16.len; __for_idx_17++) {
			string required = *(string*)(array_get(__in_lhs_16, __for_idx_17));
			bool __any_18 = false;
			{
				i64 __any_idx_19 = 0;
				for (; __any_idx_19 < corpus_files.len; __any_idx_19++) {
					string __any_it_20 = *(string*)(array_get(corpus_files, __any_idx_19));
					if (string__eq(os__file_name(__any_it_20), required)) {
						__any_18 = true;
					}
				}
			}
			Suite__record(&s, __any_18, string__plus(required, _v3_lit_17_6776d00fb840cc45), string__plus(required, _v3_lit_27_73f31a94c2a48618));
		}
	}
	if (s.fails > 0) {
		println(string__plus(int__str(s.fails), _v3_lit_11_d00d55f7f09898a7));
		return 0;
	}
	println(_v3_lit_31_f2ed3b4015f82701);
	return 0;
}

/* V3CACHE_FN_END 2cc9312c745b4f57 */
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
/* V3CACHE_MODULE os */
/* V3CACHE_FN_BEGIN 2ce72caf6e0ee31b */
bool os__exists(string path) {
}

/* V3CACHE_FN_END 2ce72caf6e0ee31b */
/* V3CACHE_MODULE os */
/* V3CACHE_FN_BEGIN 518a68b59a0ae6fb */
string os__file_name(string path) {
}

/* V3CACHE_FN_END 518a68b59a0ae6fb */
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
/* V3CACHE_MODULE main */
/* V3CACHE_FN_BEGIN cbcd7866e66d70f9 */
Array pub_fns(string dir) {
	Array files = array_new(	sizeof(string), 0, 0);
	collect(dir, &files);
	Array names = array_new(	sizeof(string), 0, 0);
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < files.len; __for_idx_0++) {
			string f = *(string*)(array_get(files, __for_idx_0));
			if (!string__ends_with(f, _v3_lit_2_9af75900c5c13533)) {
				continue;
			}
			if (string__ends_with(f, _v3_lit_7_622193d14801b764)) {
				continue;
			}
			__v_result_string __or_opt_1 = os__read_file(f);
			string __or_val_2 = {0};
			if (__or_opt_1.ok) {
				__or_val_2 = __or_opt_1.value;
			} else {
				IError err = __or_opt_1.err;
				continue;
			}
			string src = __or_val_2;
			Array __in_lhs_3 = string__split_into_lines(src);
			{
				i64 __for_idx_4 = 0;
				for (; __for_idx_4 < __in_lhs_3.len; __for_idx_4++) {
					string line = *(string*)(array_get(__in_lhs_3, __for_idx_4));
					string t = string__trim_space(line);
					if (!string__starts_with(t, _v3_lit_7_59848b6e4ae2099e)) {
						continue;
					}
					string rest = string__substr(					t, 					7, (					t).len);
					string head = rest;
					if (string__starts_with(rest, _v3_lit_1_44bd53d473cd3b91)) {
						__v_option_i64 __or_opt_5 = string__index(rest, _v3_lit_1_44bd52d473cd39de);
						i64 __or_val_6 = {0};
						if (__or_opt_5.ok) {
							__or_val_6 = __or_opt_5.value;
						} else {
							continue;
						}
						i64 end = __or_val_6;
						head = string__trim_space(string__substr(						rest, 						end + 1, (						rest).len));
					}
					__v_option_i64 __or_opt_7 = string__index(head, _v3_lit_1_44bd53d473cd3b91);
					i64 __or_val_8 = {0};
					if (__or_opt_7.ok) {
						__or_val_8 = __or_opt_7.value;
					} else {
						continue;
					}
					i64 v_open = __or_val_8;
					string __arr_val_9 = string__trim_space(string__substr(					head, 0, 					v_open));
					array_push(&names, &__arr_val_9);
				}
			}
		}
	}
	map seen = new_map(sizeof(string), sizeof(bool), map_hash_string, map_eq_string, map_clone_string, map_free_string);
	Array out = array_new(	sizeof(string), 0, 0);
	{
		i64 __for_idx_10 = 0;
		for (; __for_idx_10 < names.len; __for_idx_10++) {
			string n = *(string*)(array_get(names, __for_idx_10));
			if (string__eq(n, _v3_lit_0_14650fb0739d0383) || ({string __map_key_11 = n;
			map__exists(&seen, &__map_key_11);})) {
				continue;
			}
			string __map_key_12 = n;
			bool __map_val_13 = true;
			map__set(&seen, &__map_key_12, &__map_val_13);
			string __arr_val_14 = string__clone(n);
			array_push(&out, &__arr_val_14);
		}
	}
	{
		i64 __sort_n_15 = out.len;
		Array __sort_buf_16 = array_new(		sizeof(string), __sort_n_15, __sort_n_15);
		Array __sort_src_17 = out;
		Array __sort_dst_18 = __sort_buf_16;
		{
			i64 __sort_w_20 = 1;
			for (; __sort_w_20 < __sort_n_15; __sort_w_20 = (i64)(((i64)(			__sort_w_20)) << (1))) {
				{
					i64 __sort_lo_21 = 0;
					for (; __sort_lo_21 < __sort_n_15; ) {
						i64 __sort_mid_22 = __sort_lo_21 + __sort_w_20;
						if (__sort_mid_22 > __sort_n_15) {
							__sort_mid_22 = __sort_n_15;
						}
						i64 __sort_hi_23 = __sort_mid_22 + __sort_w_20;
						if (__sort_hi_23 > __sort_n_15) {
							__sort_hi_23 = __sort_n_15;
						}
						i64 __sort_i_24 = __sort_lo_21;
						i64 __sort_j_25 = __sort_mid_22;
						i64 __sort_k_26 = __sort_lo_21;
						for (; (__sort_i_24 < __sort_mid_22) && (__sort_j_25 < __sort_hi_23); __sort_k_26++) {
							if (string__lt((*((string*)((__sort_src_17).data) + (__sort_j_25))), (*((string*)((__sort_src_17).data) + (__sort_i_24))))) {
								array_sort_move(__sort_dst_18.data, __sort_k_26, __sort_src_17.data, __sort_j_25, 1, sizeof(string));
								__sort_j_25 = __sort_j_25 + 1;
							} else {
								array_sort_move(__sort_dst_18.data, __sort_k_26, __sort_src_17.data, __sort_i_24, 1, sizeof(string));
								__sort_i_24 = __sort_i_24 + 1;
							}
						}
						if (__sort_i_24 < __sort_mid_22) {
							array_sort_move(__sort_dst_18.data, __sort_k_26, __sort_src_17.data, __sort_i_24, __sort_mid_22 - __sort_i_24, sizeof(string));
						} else {
							if (__sort_j_25 < __sort_hi_23) {
								array_sort_move(__sort_dst_18.data, __sort_k_26, __sort_src_17.data, __sort_j_25, __sort_hi_23 - __sort_j_25, sizeof(string));
							}
						}
						__sort_lo_21 = __sort_hi_23;
					}
				}
				Array __sort_swap_19 = __sort_src_17;
				__sort_src_17 = __sort_dst_18;
				__sort_dst_18 = __sort_swap_19;
			}
		}
		if (__sort_src_17.data != out.data) {
			array_sort_move(out.data, 0, __sort_src_17.data, 0, __sort_n_15, sizeof(string));
		}
		array__free(&__sort_buf_16);
	}
	0;
	return out;
}

/* V3CACHE_FN_END cbcd7866e66d70f9 */
/* V3CACHE_MODULE main */
/* V3CACHE_FN_BEGIN 451ad2b69b478159 */
string read_corpus(Array paths) {
	Array sb = array_new(	sizeof(string), 0, 0);
	{
		i64 __for_idx_0 = 0;
		for (; __for_idx_0 < paths.len; __for_idx_0++) {
			string p = *(string*)(array_get(paths, __for_idx_0));
			__v_result_string __or_opt_1 = os__read_file(p);
			string __or_val_2 = {0};
			if (__or_opt_1.ok) {
				__or_val_2 = __or_opt_1.value;
			} else {
				IError err = __or_opt_1.err;
				continue;
			}
			string __arr_val_3 = __or_val_2;
			array_push(&sb, &__arr_val_3);
		}
	}
	return Array_string__join(sb, _v3_lit_1_44bd31d473cd01cb);
}

/* V3CACHE_FN_END 451ad2b69b478159 */
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
/* V3CACHE_FN_BEGIN 4851d86a46ceaae1 */
Array string__split_into_lines(string s) {
}

/* V3CACHE_FN_END 4851d86a46ceaae1 */
/* V3CACHE_MODULE builtin */
/* V3CACHE_FN_BEGIN 3d65f13d2d04ca8 */
bool string__starts_with(string s, string p) {
}

/* V3CACHE_FN_END 3d65f13d2d04ca8 */
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
/* V3CACHE_FN_BEGIN bac42a24944e71b9 */
string string__to_lower_ascii(string s) {
}

/* V3CACHE_FN_END bac42a24944e71b9 */
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
/* V3CACHE_FN_BEGIN ce3d4fd2986b025f */
inline string u64__str(u64 nn) {
}

/* V3CACHE_FN_END ce3d4fd2986b025f */
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
/* V3CACHE_FN_BEGIN 1f7938591f09bfca */
Array voidptr__vbytes(void* data, i64 len) {
}

/* V3CACHE_FN_END 1f7938591f09bfca */
// THE END.
/* V3CACHE_BODY_END */
