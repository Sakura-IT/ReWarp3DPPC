
#ifndef EXEC_TYPES_H
#include <exec/types.h>

#endif
#ifndef EXEC_EXEC_H
#include <exec/exec.h>
#endif
#ifndef EXEC_INTERFACES_H
#include <exec/interfaces.h>
#endif

ULONG warp3dppc_Obtain(struct warp3dppcIFace *Self);
ULONG warp3dppc_Release(struct warp3dppcIFace *Self);

STATIC CONST APTR main_vectors[] =
{
    (APTR)warp3dppc_Obtain,
    (APTR)warp3dppc_Release,
    NULL,
    NULL,
    (APTR)-1
};

ULONG warp3dppc_Obtain (struct warp3dppcIFace *Self) {
	return(Self->Data.RefCount++);
}

ULONG warp3dppc_Release (struct warp3dppcIFace *Self) {
	return(Self->Data.RefCount--);
}
