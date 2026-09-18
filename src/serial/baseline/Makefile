CC        ?= cc
STD       ?= -std=c11
CPPFLAGS  ?=
CFLAGS    ?= -O3 -march=native -flto -ffast-math -Wall -Wextra -Wpedantic
CFLAGS    += -D_POSIX_C_SOURCE=199309L
LDLIBS    ?= -lm
PRECISION ?= double

COMMON_DIR := ../../common

PROGRAMS := nbody_direct_serial generate_ic
HEADERS  := $(COMMON_DIR)/nbody_common.h
COMMON_INCLUDES := -I$(COMMON_DIR)
COMMON_SRCS      := $(COMMON_DIR)/profiling.c $(COMMON_DIR)/particles.c $(COMMON_DIR)/utils.c
COMMON_HEADERS   := $(COMMON_DIR)/profiling.h $(COMMON_DIR)/particles.h $(COMMON_DIR)/utils.h
UTILS_SRCS       := $(COMMON_DIR)/utils.c
UTILS_HEADERS    := $(COMMON_DIR)/utils.h

ifeq ($(PRECISION),float)
PRECISION_CPPFLAGS := -DNBODY_USE_FLOAT
else ifeq ($(PRECISION),double)
PRECISION_CPPFLAGS := -DNBODY_USE_DOUBLE
else
$(error PRECISION must be either double or float)
endif

.PHONY: all clean run-smoke

all: $(PROGRAMS)

nbody_direct_serial: nbody_direct_serial.c $(HEADERS) $(COMMON_SRCS) $(COMMON_HEADERS)
	$(CC) $(STD) $(CPPFLAGS) $(COMMON_INCLUDES) $(PRECISION_CPPFLAGS) $(CFLAGS) -o $@ nbody_direct_serial.c $(COMMON_SRCS) $(LDLIBS)

generate_ic: $(COMMON_DIR)/generate_ic.c $(HEADERS) $(UTILS_SRCS) $(UTILS_HEADERS)
	$(CC) $(STD) $(CPPFLAGS) $(COMMON_INCLUDES) $(PRECISION_CPPFLAGS) $(CFLAGS) -o $@ $(COMMON_DIR)/generate_ic.c $(UTILS_SRCS) $(LDLIBS)


run-smoke: all
	./generate_ic --model 0 --n 128 --seed 42 --output plummer_128.bin
	./nbody_direct_serial --input plummer_128.bin --nsteps 5 --dt 1e-4 --eps 0.05 --energy-every 1 --output plummer_128_final.bin --quiet
	./generate_ic --model 1 --n 128 --seed 43 --output ball_128.bin
	./nbody_direct_serial --input ball_128.bin --nsteps 5 --dt 1e-4 --eps 0.05 --energy-every 1 --output ball_128_final.bin --quiet

clean:
	rm -f $(PROGRAMS) *.o *.bin
