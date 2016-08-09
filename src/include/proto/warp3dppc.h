#ifndef PROTO_WARP3DPPC_H
#define PROTO_WARP3DPPC_H

/*
**	$Id$
**	Includes Release 50.1
**
**	Prototype/inline/pragma header file combo
**
**	(C) Copyright 2003-2005 Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif

/****************************************************************************/

#ifndef __NOLIBBASE__
 #ifndef __USE_BASETYPE__
  extern struct Library * warp3dppcBase;
 #else
  extern struct warp3dppcLibrary * warp3dppcBase;
 #endif /* __USE_BASETYPE__ */
#endif /* __NOLIBBASE__ */

/****************************************************************************/

#ifdef __amigaos4__
 #include <interfaces/warp3dppc.h>
 #ifdef __USE_INLINE__
  #include <inline4/warp3dppc.h>
 #endif /* __USE_INLINE__ */
 #ifndef CLIB_WARP3DPPC_PROTOS_H
  #define CLIB_WARP3DPPC_PROTOS_H 1
 #endif /* CLIB_WARP3DPPC_PROTOS_H */
 #ifndef __NOGLOBALIFACE__
  extern struct warp3dppcIFace *Iwarp3dppc;
 #endif /* __NOGLOBALIFACE__ */
#else /* __amigaos4__ */
 #ifndef CLIB_WARP3DPPC_PROTOS_H
  #include <clib/warp3dppc_protos.h>
 #endif /* CLIB_WARP3DPPC_PROTOS_H */
 #if defined(__GNUC__)
  #ifndef __PPC__
   #include <inline/warp3dppc.h>
  #else
   #include <ppcinline/warp3dppc.h>
  #endif /* __PPC__ */
 #elif defined(__VBCC__)
  #ifndef __PPC__
   #include <inline/warp3dppc_protos.h>
  #endif /* __PPC__ */
 #else
  #include <pragmas/warp3dppc_pragmas.h>
 #endif /* __GNUC__ */
#endif /* __amigaos4__ */

/****************************************************************************/

#endif /* PROTO_WARP3DPPC_H */
