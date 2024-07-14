HOST	= aos4	# can be overriden when calling make

CC	= gcc
CFLAGS	= -nostartfiles -O3 -I. -Iinclude -DPATCH_RADEONHD #-DBEEFDEBUG -DDEBUG 
LDFLAGS	= -nostartfiles #-nostdlib
LIBS	= 
LHA	= lha

DISTRIBUTION = ReWarp3DPPC.lha
OBJS	= warp3dppc_68k.o warp3dppcinit.o
W3DPPC_LIB = Warp3DPPC.library

$(W3DPPC_LIB): $(OBJS) makefile
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

warp3dppc_68k.o: warp3dppc_68k.c warp3dppc_patch.h makefile
	$(CC) -c $(CFLAGS) -o  warp3dppc_68k.o warp3dppc_68k.c

warp3dppcinit.o: warp3dppcinit.c include/LibHeader.h warp3dppc.library_rev.h warp3dppc_patch.h makefile
	$(CC) -c $(CFLAGS) -o warp3dppcinit.o warp3dppcinit.c

clean:
	$(RM) $(OBJS) $(W3DPPC_LIB) $(DISTRIBUTION)

distribution: $(W3DPPC_LIB)
	$(LHA) a $(DISTRIBUTION) $(W3DPPC_LIB)

install: $(W3DPPC_LIB)
	delete LIBS:Warp3DPPC.library
	copy Warp3DPPC.library LIBS:

include Makefile.inc.$(HOST)

