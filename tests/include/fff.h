#pragma once

/**
 * @file fff.h
 * @brief Simple Fake Function Framework implementation
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FFF_MAX_HISTORY
#define FFF_MAX_HISTORY 50
#endif

// Global FFF state
typedef struct {
    unsigned int call_count;
} fff_globals_t;

extern fff_globals_t fff;

#define DEFINE_FFF_GLOBALS \
    fff_globals_t fff = {0}

#define FFF_RESET_HISTORY() do { \
    fff.call_count = 0; \
} while(0)

#define RESET_FAKE(FUNCTION) FUNCTION##_reset()

// For functions with return values and sequences
#define SET_RETURN_SEQ(FUNCTION, VALUES, LEN) do { \
    for(unsigned int i = 0; i < LEN && i < FFF_MAX_HISTORY; i++) { \
        FUNCTION##_fake.return_val_seq[i] = VALUES[i]; \
    } \
    FUNCTION##_fake.return_val_seq_len = LEN; \
    FUNCTION##_fake.return_val_seq_idx = 0; \
} while(0)

// DECLARE macros (in header files)
#define DECLARE_FAKE_VOID_FUNC(FUNCTION) \
    typedef struct { \
        unsigned int call_count; \
    } FUNCTION##_fake_t; \
    extern FUNCTION##_fake_t FUNCTION##_fake; \
    extern unsigned int FUNCTION##_call_count; \
    void FUNCTION(void); \
    void FUNCTION##_reset(void)

#define DECLARE_FAKE_VOID_FUNC1(FUNCTION, ARG0_TYPE) \
    typedef struct { \
        unsigned int call_count; \
        ARG0_TYPE arg0_history[FFF_MAX_HISTORY]; \
    } FUNCTION##_fake_t; \
    extern FUNCTION##_fake_t FUNCTION##_fake; \
    extern unsigned int FUNCTION##_call_count; \
    extern ARG0_TYPE FUNCTION##_arg0_history[FFF_MAX_HISTORY]; \
    void FUNCTION(ARG0_TYPE arg0); \
    void FUNCTION##_reset(void)

#define DECLARE_FAKE_VALUE_FUNC(RETURN_TYPE, FUNCTION) \
    typedef struct { \
        unsigned int call_count; \
        RETURN_TYPE return_val; \
        RETURN_TYPE return_val_seq[FFF_MAX_HISTORY]; \
        unsigned int return_val_seq_len; \
        unsigned int return_val_seq_idx; \
    } FUNCTION##_fake_t; \
    extern FUNCTION##_fake_t FUNCTION##_fake; \
    extern RETURN_TYPE FUNCTION##_return_val; \
    extern unsigned int FUNCTION##_call_count; \
    RETURN_TYPE FUNCTION(void); \
    void FUNCTION##_reset(void)

// DEFINE macros (in implementation files)
#define DEFINE_FAKE_VOID_FUNC(FUNCTION) \
    FUNCTION##_fake_t FUNCTION##_fake = {0}; \
    unsigned int FUNCTION##_call_count = 0; \
    void FUNCTION(void) { \
        FUNCTION##_call_count++; \
        FUNCTION##_fake.call_count++; \
    } \
    void FUNCTION##_reset(void) { \
        FUNCTION##_call_count = 0; \
        FUNCTION##_fake.call_count = 0; \
    }

#define DEFINE_FAKE_VOID_FUNC1(FUNCTION, ARG0_TYPE) \
    FUNCTION##_fake_t FUNCTION##_fake = {0}; \
    unsigned int FUNCTION##_call_count = 0; \
    ARG0_TYPE FUNCTION##_arg0_history[FFF_MAX_HISTORY]; \
    void FUNCTION(ARG0_TYPE arg0) { \
        if(FUNCTION##_call_count < FFF_MAX_HISTORY) { \
            FUNCTION##_arg0_history[FUNCTION##_call_count] = arg0; \
            FUNCTION##_fake.arg0_history[FUNCTION##_call_count] = arg0; \
        } \
        FUNCTION##_call_count++; \
        FUNCTION##_fake.call_count++; \
    } \
    void FUNCTION##_reset(void) { \
        FUNCTION##_call_count = 0; \
        FUNCTION##_fake.call_count = 0; \
    }

#define DEFINE_FAKE_VALUE_FUNC(RETURN_TYPE, FUNCTION) \
    FUNCTION##_fake_t FUNCTION##_fake = {0}; \
    RETURN_TYPE FUNCTION##_return_val = (RETURN_TYPE)0; \
    unsigned int FUNCTION##_call_count = 0; \
    RETURN_TYPE FUNCTION(void) { \
        FUNCTION##_call_count++; \
        FUNCTION##_fake.call_count++; \
        if(FUNCTION##_fake.return_val_seq_len > 0) { \
            if(FUNCTION##_fake.return_val_seq_idx < FUNCTION##_fake.return_val_seq_len) { \
                return FUNCTION##_fake.return_val_seq[FUNCTION##_fake.return_val_seq_idx++]; \
            } else { \
                return FUNCTION##_fake.return_val_seq[FUNCTION##_fake.return_val_seq_len - 1]; \
            } \
        } \
        return FUNCTION##_fake.return_val; \
    } \
    void FUNCTION##_reset(void) { \
        FUNCTION##_call_count = 0; \
        FUNCTION##_fake.call_count = 0; \
        FUNCTION##_fake.return_val = (RETURN_TYPE)0; \
        FUNCTION##_fake.return_val_seq_len = 0; \
        FUNCTION##_fake.return_val_seq_idx = 0; \
        FUNCTION##_return_val = (RETURN_TYPE)0; \
    }

#ifdef __cplusplus
}
#endif
