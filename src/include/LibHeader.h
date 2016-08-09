#ifndef	WARP3DPPC_LIBHEADER_H
#define	WARP3DPPC_LIBHEADER_H

#include	<dos/dos.h>
#include	<exec/libraries.h>
#include	<utility/utility.h>

#ifndef	__GNUC__
#undef __attribute__
#define	__attribute__(x)
#endif

/* #include	"warp3dppc_priv.h"*/

/*********************************************************************
 * @Structures																			*
 ********************************************************************/

#ifdef __GNUC__
   #ifdef __PPC__
    #pragma pack(2)
   #endif
#elif defined(__VBCC__)
   #pragma amiga-align
#endif

struct MyInitData
{
	UBYTE ln_Type_Init[4];
	UBYTE ln_Pri_Init[4];
	UBYTE ln_Name_Init[2];
	ULONG __attribute__((aligned(2)))	ln_Name_Content;
	UBYTE lib_Flags_Init[4];
	UBYTE lib_Version_Init[2]; UWORD lib_Version_Content;
	UBYTE lib_Revision_Init[2]; UWORD lib_Revision_Content;
	UBYTE lib_IdString_Init[2];
	ULONG  __attribute__((aligned(2)))	lib_IdString_Content;
	UWORD EndMark;
} __attribute__((packed));

struct warp3dppcLibrary
{
	struct Library		libNode;
	UWORD			pad;
	BPTR			segList;

	struct ExecIFace	*IExec;
	struct Warp3DIFace	*IWarp3D;
	struct Library		*Warp3DBase;
};

#define __NOLIBBASE__ 1
#define __NOGLOBALIFACE__ 1

#define IExec LibBase->IExec

#ifdef __GNUC__
   #ifdef __PPC__
    #pragma pack()
   #endif
#elif defined(__VBCC__)
   #pragma default-align
#endif

/*********************************************************************
 * @Prototypes																			*
 ********************************************************************/

#ifndef WARP3DPPC_INTERFACE_DEF_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef EXEC_EXEC_H
#include <exec/exec.h>
#endif
#ifndef EXEC_INTERFACES_H
#include <exec/interfaces.h>
#endif

#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif

struct warp3dppcIFace
{
	struct InterfaceData Data;

	ULONG APICALL (*Obtain)(struct warp3dppcIFace *Self);
	ULONG APICALL (*Release)(struct warp3dppcIFace *Self);
	void APICALL (*Expunge)(struct warp3dppcIFace *Self);
	struct Interface * APICALL (*Clone)(struct warp3dppcIFace *Self);
};

#endif /* WARP3DPPC_INTERFACE_DEF_H */

/*extern pt_mod_s *Init(struct warp3dppcIFace *Self, UBYTE * buf, LONG bufsize, LONG frequency, ULONG modtype);
extern ULONG PtSeek(struct warp3dppcIFace *Self, pt_mod_s *mod, ULONG time);*/

#endif	/* WARP3DPPC_LIBHEADER_H */
