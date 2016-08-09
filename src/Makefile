CC = vc +aosppc
CP = copy
RM = delete

# Change these as required
OPTIMIZE = -O3 -speed
DEBUG =
CFLAGS = $(OPTIMIZE) $(DEBUG) -I. -Iinclude
LFLAGS = -P_start -P__amigaos4__ -s

# Flags passed to gcc during linking
LINK = $(OPTIMIZE) $(DEBUG) $(LFLAGS)

# Name of the "thing" to build
TARGET = Warp3DPPC.library

# Additional linker libraries
LIBS = -lm -lvc

# Version of the library to build
VERSION = 4

# Source code files used in this project
# Add any additional files to this line

SRCS = warp3dppcinit.c warp3dppc_68k.c

# -------------------------------------------------------------
# Nothing should need changing below this line

OBJS = $(SRCS:.c=.o)
# Rules for building
../$(TARGET): $(OBJS)
	$(CC) $(LINK) -nostdlib -o /$(TARGET) $(OBJS) $(LIBS)

warp3dppcinit.o: warp3dppc.library_rev.h

.PHONY: install
install: ../$(TARGET)
	$(CP) /$(TARGET) LIBS:

.PHONY: clean
clean:
	$(RM) $(OBJS)

.PHONY: revision
revision:
	bumprev $(VERSION) $(TARGET)
