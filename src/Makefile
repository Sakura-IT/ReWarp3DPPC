HOST	= aos4	# can be overriden when calling make

CC	= vc +aosppc
CFLAGS	= -O3 -speed -I. -Iinclude
LDFLAGS	= -P_start -P__amigaos4__ -s -nostdlib
LIBS	= -lm -lvc
LHA	= lha

DISTRIBUTION = ReWarp3DPPC.lha
OBJS	= warp3dppcinit.o warp3dppc_68k.o
W3DPPC_LIB = Warp3DPPC.library

$(W3DPPC_LIB): $(OBJS) 
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
	
%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	$(RM) $(OBJS) $(W3DPPC_LIB) $(DISTRIBUTION)

distribution: $(W3DPPC_LIB)
	$(LHA) a $(DISTRIBUTION) $(W3DPPC_LIB)

include Makefile.inc.$(HOST)

