#ifndef __DEFINES_H__
#define __DEFINES_H__

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef float f32;
typedef double f64;

typedef _Bool b8;
typedef u32 b32;

#if defined(__clang__) || defined(__gcc__) || defined(__GNUC__)
#define STATIC_ASSERT _Static_assert
#define TYPEOF(var) __typeof__(var)
#elif defined(__TINYC__)
#define STATIC_ASSERT(COND,MSG) typedef char static_assertion_##MSG[(COND)?1:-1]
#define TYPEOF(var) typeof(var)
#else
#define STATIC_ASSERT static_assert
#define TYPEOF(var) typeof(var)
#endif

#ifndef __cplusplus
#ifdef __TINYC__
STATIC_ASSERT(sizeof(u8)  == 1, u8_expected_1_byte);
STATIC_ASSERT(sizeof(u16) == 2, u16_expected_2_byte);
STATIC_ASSERT(sizeof(u32) == 4, u32_expected_4_byte);
STATIC_ASSERT(sizeof(u64) == 8, u64_expected_8_byte);

STATIC_ASSERT(sizeof(i8)  == 1, i8_expected_1_byte);
STATIC_ASSERT(sizeof(i16) == 2, i16_expected_2_byte);
STATIC_ASSERT(sizeof(i32) == 4, i32_expected_4_byte);
STATIC_ASSERT(sizeof(i64) == 8, i64_expected_8_byte);

STATIC_ASSERT(sizeof(f32) == 4, f32_expected_4_byte);
STATIC_ASSERT(sizeof(f64) == 8, f64_expected_8_byte);
#else
STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");
#endif
#endif

#define true 1
#define false 0

#define OPTIMIZE_ENABLE __pragma(optimize ("", on))
#define OPTIMIZE_DISABLE __pragma(optimize ("", off))

#ifdef LIBRARY_EXPORTS
#if defined(_MSC_VER)
#define LIB_API __declspec(dllexport)
#else
#define LIB_API __attribute__((visibility("default")))
#endif
#else
#if defined(_MSC_VER)
#define LIB_API __declspec(dllimport)
#else
#define LIB_API
#endif
#endif

#define BIT(x_) (1llu << (x_))
#define CHANGE_BIT(_num, _index, _value) (_num ^= (-(_value) ^ _num) & (_index))
#define CHECK_BIT(_num, _index) ((_num >> _index) & 1U)
#define TOGGLE_BIT(_num, _index) (_num ^= 1UL << _index)
#define SET_BIT(_num, _index) (_num |= 1UL << _index)
#define CLEAR_BIT(_num, _index) (_num &= ~(1UL << _index))

#define GiB (1024 * 1024 * 1024)
#define MiB (1024 * 1024)
#define KiB (1024)

#define CLAMP(value_, min_, max_) (value_ <= min_) ? min_ : (value_ >= max_) ? max_ : value_;
#define MIN(v1_, v2_) (v1_ < v2_ ? v1_ : v2_)
#define MAX(v1_, v2_) (v1_ > v2_ ? v1_ : v2_)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define print_bits(x)\
do\
{\
u64 a__ = (x);\
u64 bits__ = sizeof(x) * 8;\
printf(#x ": ");\
while (bits__--) putchar(a__ &(1ULL << bits__) ? '1' : '0');\
putchar('\n');\
} while (0)

#endif