CC        ?= cc
STD       ?= -std=c11
CPPFLAGS  ?=
CFLAGS    ?= -O3 -march=native -flto -ffast-math -Wall -Wextra -Wpedantic
CFLAGS    += -D_POSIX_C_SOURCE=199309L
LDLIBS    ?= -lm
PRECISION ?= double

USE_PAPI  ?= 0
ifeq ($(USE_PAPI),1)
CFLAGS    += -DUSE_PAPI -I$(HOME)/local/papi/include
LDLIBS    += -L$(HOME)/local/papi/lib -lpapi
endif

PROGRAMS := nbody_direct_serial generate_ic
HEADERS  := nbody_common.h profiling.h
PROFILING_SRCS := profiling.c

ifeq ($(PRECISION),float)
PRECISION_CPPFLAGS := -DNBODY_USE_FLOAT
else ifeq ($(PRECISION),double)
PRECISION_CPPFLAGS := -DNBODY_USE_DOUBLE
else
$(error PRECISION must be either double or float)
endif

.PHONY: all clean run-smoke

all: $(PROGRAMS)

nbody_direct_serial: nbody_direct_serial.c $(HEADERS) $(PROFILING_SRCS)
	$(CC) $(STD) $(CPPFLAGS) $(PRECISION_CPPFLAGS) $(CFLAGS) -o $@ nbody_direct_serial.c $(PROFILING_SRCS) $(LDLIBS)

generate_ic: generate_ic.c $(HEADERS)
	$(CC) $(STD) $(CPPFLAGS) $(PRECISION_CPPFLAGS) $(CFLAGS) -o $@ $< $(LDLIBS)


run-smoke: all
	./generate_ic --model 0 --n 128 --seed 42 --output plummer_128.bin
	./nbody_direct_serial --input plummer_128.bin --nsteps 5 --dt 1e-4 --eps 0.05 --energy-every 1 --output plummer_128_final.bin --quiet
	./generate_ic --model 1 --n 128 --seed 43 --output ball_128.bin
	./nbody_direct_serial --input ball_128.bin --nsteps 5 --dt 1e-4 --eps 0.05 --energy-every 1 --output ball_128_final.bin --quiet

clean:
	rm -f $(PROGRAMS) *.o *.bin
