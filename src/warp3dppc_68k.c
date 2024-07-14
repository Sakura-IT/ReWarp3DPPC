/*
**
**	Thanks to Alain Thellier for providing the patches to DrawElements, DrawArray and DrawLine!
**
*/

#define __USE_BASETYPE__
#define __USE_INLINE__
#include <string.h>
#include <math.h>
#include <exec/interfaces.h>
#include <exec/libraries.h>
#include <exec/emulation.h>
#include <interfaces/exec.h>
#include <interfaces/warp3dppc.h>
#include <proto/warp3d.h>
#include <interfaces/warp3d.h>
#include <proto/graphics.h>

#include <stdio.h>

#if defined(DEBUG) || defined(PATCH_RADEONHD)
extern struct ExecIFace *iexec;
#endif

#include "warp3dppc_patch.h"

/***************************************************************************************************/

#define NLOOP(nbre) for(n=0;n<nbre;n++)
#define AND &
#define COPYRGBA(x,y)	*((ULONG *)(x))=*((ULONG *)(y));
#define W3D_INDEX_NONE 9999

extern ULONG PatchFlag;

struct drawcontext {
	W3D_Context *context;
	ULONG primitive;
	ULONG type;
	ULONG count;
	void *indices;
	ULONG base;
};

struct point3D {
	float x,y,z;
	float u,v;
	float w;
	UBYTE RGBA[4];
};

struct VertexFFF {
	float x,y,z;
};

struct VertexFFD {
	float x,y;
	double z;
};

struct VertexDDD {
	double x,y,z;
};

static inline int8  convert_int8 (uint32 x) { return x; }
static inline int16 convert_int16(uint32 x) { return x; }

#ifdef DEBUG
#define ENTRY iexec->DebugPrintF( "[ENTRY :] %s in file %s line %ld\n", __func__ , __FILE__, __LINE__ );
#define EXIT iexec->DebugPrintF( "[EXIT :] %s in file %s line %ld\n", __func__ , __FILE__, __LINE__ );
#define RETURN(r) iexec->DebugPrintF( "[RETURN : 0x%lx] %s in file %s line %ld\n", r, __func__ , __FILE__, __LINE__ );
#define PARAMD(n,d)  iexec->DebugPrintF( "[PARAM] %s == %d\n", n, d);
#define PARAMS(n,s)  iexec->DebugPrintF( "[PARAM] %s == %s\n", n, s);
#define PARAMX(n,x)  iexec->DebugPrintF( "[PARAM] %s == 0x%lx\n", n, x);
#else
#define ENTRY
#define EXIT
#define RETURN(r)
#define PARAMD(n,d)
#define PARAMS(n,s)
#define PARAMX(n,x)
#endif

/***************************************************************************************************/

APTR stub_CreateContext(ULONG * error, struct TagItem * CCTags)
{
ENTRY

#ifdef DEBUG
	struct TagItem *ti = CCTags;
	for(; ti->ti_Tag != TAG_DONE; ti++) {
		PARAMX("[TAG  :]", ti->ti_Tag);
		PARAMX("[DATA :]", ti->ti_Data);
	}
#endif

	W3D_Context *context;
	context = W3D_CreateContext(error, CCTags);
	
RETURN(context)
	return (context);
}

/***************************************************************************************************/

void stub_DestroyContext(W3D_Context * context)
{
ENTRY
    W3D_DestroyContext(context);
EXIT
    return;
}

/***************************************************************************************************/

ULONG stub_GetState(W3D_Context * context, ULONG state) 
{
ENTRY
	ULONG result;
	result = W3D_GetState(context, state);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetState(W3D_Context * context, ULONG state, ULONG action)
{
ENTRY
PARAMX("context",context)
PARAMX("state",state)
PARAMX("action",action)
	ULONG result;
	result = W3D_SetState(context, state, action);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_CheckDriver(void)
{
ENTRY

	ULONG result;
	result = W3D_CheckDriver();
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_LockHardware(W3D_Context * context)
{
ENTRY
	ULONG result;
	result = W3D_LockHardware(context);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

void stub_UnLockHardware(W3D_Context * context)
{
ENTRY
	W3D_UnLockHardware(context);
EXIT
	return;
}

/***************************************************************************************************/

void stub_WaitIdle(W3D_Context * context)
{
ENTRY
	W3D_WaitIdle(context);	
EXIT
	return;
}

/***************************************************************************************************/

ULONG stub_CheckIdle(W3D_Context * context)
{
ENTRY
	ULONG result;
	result = W3D_CheckIdle(context);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_Query(W3D_Context * context, ULONG query, ULONG destfmt)
{
ENTRY
PARAMX("context",context)
PARAMX("query",query)
PARAMX("destfmt",destfmt)
	ULONG result;
	result = W3D_Query(context, query, destfmt);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_GetTexFmtInfo(W3D_Context * context, ULONG format, ULONG destfmt)
{
ENTRY
PARAMX("context", context)
PARAMX("format", format)
PARAMX("destfmt",destfmt)
	ULONG result;
	result = W3D_GetTexFmtInfo(context, format, destfmt);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

APTR stub_AllocTexObj(W3D_Context * context, ULONG * error, struct TagItem * ATOTags)
{
ENTRY

#ifdef DEBUG
	struct TagItem *ti = ATOTags;
	for(; ti->ti_Tag != TAG_DONE; ti++) {
		PARAMX("[TAG  :]", ti->ti_Tag);
		PARAMX("[DATA :]", ti->ti_Data);
	}
#endif

	W3D_Texture *texture;
	texture = W3D_AllocTexObj(context, error, ATOTags);
RETURN(texture)
	return (texture);
}

/***************************************************************************************************/

void stub_FreeTexObj(W3D_Context * context, W3D_Texture * texture)
{
ENTRY
	W3D_FreeTexObj(context, texture);
EXIT
	return;
}

/***************************************************************************************************/

void stub_ReleaseTexture(W3D_Context * context, W3D_Texture * texture)
{
ENTRY
	W3D_ReleaseTexture(context, texture);
EXIT
	return;
}

/***************************************************************************************************/

void stub_FlushTextures(W3D_Context * context)
{
ENTRY
	W3D_FlushTextures(context);
EXIT
	return;
}

/***************************************************************************************************/

ULONG stub_SetFilter(W3D_Context * context, W3D_Texture * texture, ULONG min, ULONG mag)
{
ENTRY
	ULONG result;
	result = W3D_SetFilter(context, texture, min, mag);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetTexEnv(W3D_Context * context, W3D_Texture * texture, ULONG envparam, W3D_Color * envcolor)
{
ENTRY
	ULONG result;
	result = W3D_SetTexEnv(context, texture, envparam, envcolor);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetWrapMode(W3D_Context * context, W3D_Texture * texture, ULONG mode_s, ULONG mode_t, W3D_Color * bordercolor)
{
ENTRY
	ULONG result;
	result = W3D_SetWrapMode(context, texture, mode_s, mode_t, bordercolor);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_UpdateTexImage(W3D_Context * context, W3D_Texture * texture, void * teximage, int level, ULONG * palette)
{
ENTRY
	ULONG result;
	result = W3D_UpdateTexImage(context, texture, teximage, level, palette);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_UploadTexture(W3D_Context * context, W3D_Texture * texture)
{
ENTRY
	ULONG result;
	result = W3D_UploadTexture(context, texture);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG DrawLine(W3D_Context *context,W3D_Vertex *v1,W3D_Vertex *v2,W3D_Texture *tex,float size)
{
ENTRY
	W3D_Vertex quad[4];
	W3D_Triangles triangles;
	register ULONG result;
	float dim,x,y;
	ULONG currentstate;

	if (size < 1.0)
		size = 1.0;

	currentstate = W3D_GetState(context, W3D_CULLFACE);
	W3D_SetState(context, W3D_CULLFACE, W3D_DISABLE);		/* a line is never an hidden-face */

	triangles.v = quad;
	triangles.tex = tex;
	triangles.vertexcount = 4;

	memcpy(&quad[0], v1, sizeof(W3D_Vertex));
	memcpy(&quad[1], v2, sizeof(W3D_Vertex));
	memcpy(&quad[2], v2, sizeof(W3D_Vertex));
	memcpy(&quad[3], v1, sizeof(W3D_Vertex));
	
	x = (v2->x - v1->x);
	y = (v2->y - v1->y);
	dim = sqrt(x*x + y*y);

	if(dim != 0.0) {
		size = size/(dim*2.0);
		x = size*x;
		y = size*y;
		quad[0].x += y;
		quad[0].y -= x;
	 	quad[1].x += y; 
		quad[1].y -= x;
		quad[2].x -= y;
		quad[2].y += x;
		quad[3].x -= y;
		quad[3].y += x;
	} else {
		quad[0].x += 0.0; 
		quad[0].y += 0.0;
 		quad[1].x += size; 
		quad[1].y += 0.0;
	 	quad[2].x += size; 
		quad[2].y += size;
	 	quad[3].x += 0.0; 
		quad[3].y += size;
	}

	result = W3D_DrawTriFan(context, &triangles);
	W3D_SetState(context, W3D_CULLFACE, currentstate);
RETURN(result)
	return(result);
}

/***************************************************************************************************/

ULONG stub_DrawLine(W3D_Context *context, W3D_Line *line)
{
ENTRY
	if (PatchFlag & PATCH_DRAWLINE) {
		register ULONG result;
		result = DrawLine(context, &line->v1, &line->v2, line->tex, line->linewidth);
RETURN(result)
		return(result);
	} else {
		ULONG result;
		result = W3D_DrawLine(context, line);
RETURN(result)
		return (result);
	}
}
/***************************************************************************************************/

ULONG stub_DrawPoint(W3D_Context * context, W3D_Point * point)
{
ENTRY
	ULONG result;
	result = W3D_DrawPoint(context, point);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawTriangle(W3D_Context * context, W3D_Triangle * triangle)
{
ENTRY

#ifdef DEBUG
printf("z[1] : %f\n", triangle->v1.z);
printf("z[2] : %f\n", triangle->v2.z);
printf("z[3] : %f\n", triangle->v3.z);
#endif

#ifdef BEEFDEBUG
	if(triangle->v1.z < 0.0 || triangle->v1.z > 1.0) printf("z : %f\n", triangle->v1.z);
	if(triangle->v2.z < 0.0 || triangle->v2.z > 1.0) printf("z : %f\n", triangle->v2.z);
	if(triangle->v3.z < 0.0 || triangle->v3.z > 1.0) printf("z : %f\n", triangle->v3.z);
#endif

	ULONG result;
	result = W3D_DrawTriangle(context, triangle);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawTriFan(W3D_Context * context, W3D_Triangles * triangles)
{
ENTRY

#ifdef DEBUG
	for(int i = 0; i < triangles->vertexcount; i++)
		printf("z[%d] : %f\n", i, triangles->v[i].z);
#endif

#ifdef BEEFDEBUG
	for(int i = 0; i < triangles->vertexcount; i++)
		if(triangles->v[i].z < 0.0 || triangles->v[i].z > 1.0) printf("z : %f\n", triangles->v[i].z);
#endif

	ULONG result;
	result = W3D_DrawTriFan(context, triangles);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawTriStrip(W3D_Context * context, W3D_Triangles * triangles)
{
ENTRY

#ifdef DEBUG
	for(int i = 0; i < triangles->vertexcount; i++)
		printf("z[%d] : %f\n", i, triangles->v[i].z);
#endif

#ifdef BEEFDEBUG
	for(int i = 0; i < triangles->vertexcount; i++)
		if(triangles->v[i].z < 0.0 || triangles->v[i].z > 1.0) printf("z : %f\n", triangles->v[i].z);
#endif

	ULONG result;
	result = W3D_DrawTriStrip(context, triangles);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetAlphaMode(W3D_Context * context, ULONG mode, W3D_Float * refval)
{
ENTRY
	ULONG result;
	result = W3D_SetAlphaMode(context, mode, refval);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetBlendMode(W3D_Context * context, ULONG srcfunc, ULONG dstfunc)
{
ENTRY
	PARAMX("srcfunc", srcfunc)
	PARAMX("dstfunc", dstfunc)
	
	ULONG result;
	result = W3D_SetBlendMode(context, srcfunc, dstfunc);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetDrawRegion(W3D_Context * context, struct BitMap * bm, int yoffset, W3D_Scissor * _scissor)
{
ENTRY
	ULONG result;
	PARAMX("context", context)
	PARAMX("bm", bm)
	PARAMX("yoffset", yoffset)
	PARAMX("scissor", _scissor)

#ifdef PATCH_RADEONHD

	if(!_scissor) {
		const int width = GetBitMapAttr(bm, BMA_WIDTH);
		const int height = GetBitMapAttr(bm, BMA_HEIGHT);
		static W3D_Scissor fallbackScissor;
		fallbackScissor.left = 0;
		fallbackScissor.top = 0;
		fallbackScissor.width = width;
		fallbackScissor.height = height;
		result = W3D_SetDrawRegion(context, bm, yoffset, _scissor ? _scissor : &fallbackScissor);

	RETURN(result)
		return (result);	
	}
	
#endif

	result = W3D_SetDrawRegion(context, bm, yoffset, _scissor);

RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetFogParams(W3D_Context * context, W3D_Fog * fogparams, ULONG fogmode)
{
ENTRY
	ULONG result;
	result = W3D_SetFogParams(context, fogparams, fogmode);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetColorMask(W3D_Context * context, W3D_Bool red, W3D_Bool green, W3D_Bool blue, W3D_Bool alpha)
{
ENTRY
	ULONG result;
	result = W3D_SetColorMask(context, red, green, blue, alpha);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetStencilFunc(W3D_Context * context, ULONG func, ULONG refvalue, ULONG mask)
{
ENTRY
	ULONG result;
	result = W3D_SetStencilFunc(context, func, refvalue, mask);
RETURN(result)
	return (result);
}

/***************************************************************************************************/
BOOL w3dpatch=TRUE;
/*==========================================================================*/
#define ZDOUBLE	1
#define ZDOUBLEPC	2
#define ZFLOAT	3
#define ZFLOATPC	4
#define ZWORD	5
#define ZWORDPC	6
#define ZUWORD	7
#define ZUWORDPC	8
#define Z24S8	9
#define Z24S8PC	10
#define S8Z24	11
#define S8Z24PC	12

static ULONG zsizes[13]={0,64,64,32,32,16,16,16,16,32,32,32,32};
static LONG zformat=0;
#define ILOOP(nbre) for(i=0;i<nbre;i++)
#define NLOOP(nbre) for(n=0;n<nbre;n++)
void findzformat(W3D_Context *context);
/*==========================================================================*/
ULONG PatchW3D_ReadZSpan(W3D_Context *context, ULONG x, ULONG y,ULONG n, W3D_Double *z)
{
register UBYTE *zbuffer;
register ULONG i;
register float  *fz;
register double *dz;
register  WORD  *wz;
register UWORD  *uwz;
register ULONG  *ulz;
UBYTE t[8];
ULONG zsize;
ULONG result;

	if(!w3dpatch)	return( W3D_ReadZSpan(context,x,y,n,z) );

	if(context->zbuffer==NULL) return(W3D_NOZBUFFER);

	if(zformat== 0) findzformat(context);
	if(zformat==-1) return(W3D_NOZBUFFER);
	result=W3D_SUCCESS;
	zsize=zsizes[zformat]/8;

	zbuffer=context->zbuffer+((y*context->width+x)*zsize);	/* offset from start */

	if(zformat==ZFLOAT)
	ILOOP(n)
		{
		fz=(float*)zbuffer;
		z[i]=(double)fz[0]; zbuffer+=zsize;
		}

	if(zformat==ZFLOATPC)
	ILOOP(n)
		{
		fz=(float*)t;
		t[3]=zbuffer[0]; t[2]=zbuffer[1]; t[1]=zbuffer[2]; t[0]=zbuffer[3];
		z[i]=(double)fz[0]; zbuffer+=zsize;
		}

	if(zformat==ZDOUBLE)
	ILOOP(n)
		{
		dz=(double*)zbuffer;
		z[i]=(double)dz[0]; zbuffer+=zsize;
		}

	if(zformat==ZDOUBLEPC)
	ILOOP(n)
		{
		dz=(double*)t;
		t[7]=zbuffer[0]; t[6]=zbuffer[1]; t[5]=zbuffer[2]; t[4]=zbuffer[3]; t[3]=zbuffer[4]; t[2]=zbuffer[5]; t[1]=zbuffer[6]; t[0]=zbuffer[7];
		z[i]=dz[0]; zbuffer+=zsize;
		}

	if(zformat==ZWORD)
	ILOOP(n)
		{
		wz=(WORD*)zbuffer;
		z[i]=((double)wz[0])/32767.0; zbuffer+=zsize;
		}

	if(zformat==ZWORDPC)
	ILOOP(n)
		{
		wz=(WORD*)t;
		t[1]=zbuffer[0]; t[0]=zbuffer[1];
		z[i]=((double)wz[0])/32767.0; zbuffer+=zsize;
		}

	if(zformat==ZUWORD)
	ILOOP(n)
		{
		uwz=(UWORD*)zbuffer;
		z[i]=((double)uwz[0])/65535.0;  zbuffer+=zsize;
		}

	if(zformat==ZUWORDPC)
	ILOOP(n)
		{
		uwz=(UWORD*)t;
		t[1]=zbuffer[0]; t[0]=zbuffer[1];
		z[i]=((double)uwz[0])/65535.0; zbuffer+=zsize;
		}

	if(zformat==Z24S8)
	ILOOP(n)
		{
		ulz=(ULONG*)t;
		t[0]=0; t[1]=zbuffer[0]; t[2]=zbuffer[1]; t[3]=zbuffer[2];
		z[i]=((double)ulz[0])/16777215.0; zbuffer+=zsize;
		}

	if(zformat==Z24S8PC)
	ILOOP(n)
		{
		ulz=(ULONG*)t;
		t[0]=0; t[1]=zbuffer[3]; t[2]=zbuffer[2]; t[3]=zbuffer[1];
		z[i]=((double)ulz[0])/16777215.0; zbuffer+=zsize;
		}

	if(zformat==S8Z24)
	ILOOP(n)
		{
		ulz=(ULONG*)t;
		t[0]=0; t[1]=zbuffer[1]; t[2]=zbuffer[2]; t[3]=zbuffer[3];
		z[i]=((double)ulz[0])/16777215.0; zbuffer+=zsize;
		}

	if(zformat==S8Z24PC)
	ILOOP(n)
		{
		ulz=(ULONG*)t;
		t[0]=0; t[1]=zbuffer[0]; t[2]=zbuffer[1]; t[3]=zbuffer[2];
		z[i]=((double)ulz[0])/16777215.0; zbuffer+=zsize;
		}

	return(result);
}
/*==========================================================================*/
ULONG PatchW3D_ReadZPixel(W3D_Context *context, ULONG x, ULONG y,W3D_Double *dz)
{
	if(!w3dpatch)	return( W3D_ReadZPixel(context,x,y,dz) );

	return(PatchW3D_ReadZSpan(context,x,y,1,dz));
}
/*==========================================================================*/
ULONG stub_ClearZBuffer(W3D_Context * context, W3D_Double * clearvalue);
void findzformat(W3D_Context *context)
{
#define TESTZ 0.1
W3D_Double dz=TESTZ;
ULONG i;

	zformat=-1;
	if(stub_ClearZBuffer(context,&dz)!=W3D_SUCCESS)	/* put a given value in zbuffer */
		{printf("no zbuffer so cant find z format\n");return;}
	ILOOP(12)
		{
		zformat=i+1; 						/* try all z formats */
		PatchW3D_ReadZPixel(context,0,0,&dz);		/* try to re-read the zbuffer with this format */
		if((TESTZ-0.001) < dz )					/* is it aproximately the same value  ? */
		if(dz < (TESTZ+0.001) )
			{printf("found z format %d\n",zformat); return;}
		}
	zformat=-1;
	printf("cant find z format\n");
}
/*==========================================================================*/

ULONG stub_AllocZBuffer(W3D_Context * context)
{
ENTRY
PARAMX("context",context)

// #ifdef PATCH_RADEONHD
// 		return W3D_SUCCESS;
// #endif

	ULONG result;
	result = W3D_AllocZBuffer(context);

#ifdef DEBUG
	findzformat(context);
#endif

RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_FreeZBuffer(W3D_Context * context)
{
ENTRY
PARAMX("context",context)
	ULONG result;
	result = W3D_FreeZBuffer(context);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_ClearZBuffer(W3D_Context * context, W3D_Double * clearvalue)
{
ENTRY

PARAMX("context",context)
PARAMX("clearvalue", clearvalue)

// #ifdef PATCH_RADEONHD
// 	return W3D_SUCCESS;
// #endif

	ULONG result;

#ifdef PATCH_RADEONHD
W3D_Double depth = clearvalue ? *clearvalue : 1.0;
result = W3D_ClearBuffers(context, 0, &depth, 0);
RETURN(result)
	return (result);
#endif

// printf("[ClearZBuffer :] clearvalue == %f\n", *clearvalue);

	result = W3D_ClearZBuffer(context, clearvalue);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_ReadZPixel(W3D_Context * context, ULONG x, ULONG y, W3D_Double * z)
{
ENTRY
PARAMX("context",context)
PARAMX("x", x)
PARAMX("y", y)
PARAMX("z", z)
	ULONG result;
	result = W3D_ReadZPixel(context, x, y, z);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_ReadZSpan(W3D_Context * context, ULONG x, ULONG y, ULONG n, W3D_Double * z)
{
ENTRY
	ULONG result;
	result = W3D_ReadZSpan(context, x, y, n, z);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetZCompareMode(W3D_Context * context, ULONG mode)
{
ENTRY

PARAMX("mode", mode)

	// W3D_Z_NEVER              =  1,  /* discard incoming pixel */
	// W3D_Z_LESS               =  2,  /* draw, if value < Z(Z_Buffer) */
	// W3D_Z_GEQUAL             =  3,  /* draw, if value >= Z(Z_Buffer) */
	// W3D_Z_LEQUAL             =  4,  /* draw, if value <= Z(Z_Buffer) */
	// W3D_Z_GREATER            =  5,  /* draw, if value > Z(Z_Buffer) */
	// W3D_Z_NOTEQUAL           =  6,  /* draw, if value != Z(Z_Buffer) */
	// W3D_Z_EQUAL              =  7,  /* draw, if value == Z(Z_Buffer) */
	// W3D_Z_ALWAYS             =  8   /* always draw */

#ifdef PATCH_RADEONHD
	if(mode == W3D_Z_LESS) mode = W3D_Z_LEQUAL;
#endif

	ULONG result;
	result = W3D_SetZCompareMode(context, mode);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_AllocStencilBuffer(W3D_Context * context)
{
ENTRY
	ULONG result;
	result = W3D_AllocStencilBuffer(context);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_ClearStencilBuffer(W3D_Context * context, ULONG * clearval)
{
ENTRY
	ULONG result;
	result = W3D_ClearStencilBuffer(context, clearval);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_FillStencilBuffer(W3D_Context * context, ULONG x, ULONG y, ULONG width, ULONG height, ULONG depth, void * data)
{
ENTRY
	ULONG result;
	result = W3D_FillStencilBuffer(context, x, y, width, height, depth, data);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_FreeStencilBuffer(W3D_Context * context)
{
ENTRY
	ULONG result;
	result = W3D_FreeStencilBuffer(context);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_ReadStencilPixel(W3D_Context * context, ULONG x, ULONG y, ULONG * st)
{
ENTRY
	ULONG result;
	result = W3D_ReadStencilPixel(context, x, y, st);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_ReadStencilSpan(W3D_Context * context, ULONG x, ULONG y, ULONG n, ULONG * st)
{
ENTRY
	ULONG result;
	result = W3D_ReadStencilSpan(context, x, y, n, st);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetLogicOp(W3D_Context * context, ULONG operation)
{
ENTRY
	ULONG result;
	result = W3D_SetLogicOp(context, operation);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_Hint(W3D_Context * context, ULONG mode, ULONG quality)
{
ENTRY
	ULONG result;
	result = W3D_Hint(context, mode, quality);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetDrawRegionWBM(W3D_Context * context, W3D_Bitmap * bitmap, W3D_Scissor * scissor)
{
ENTRY
	ULONG result;
	result = W3D_SetDrawRegionWBM(context, bitmap, scissor);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_GetDriverState(W3D_Context * context)
{
ENTRY
	ULONG result;
	result = W3D_GetDriverState(context);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_Flush(W3D_Context * context)
{
ENTRY
	ULONG result;
	result = W3D_Flush(context);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetPenMask(W3D_Context * context, ULONG pen)
{
ENTRY
	ULONG result;
	result = W3D_SetPenMask(context, pen);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetStencilOp(W3D_Context * context, ULONG sfail, ULONG dpfail, ULONG dppass)
{
ENTRY
	ULONG result;
	result = W3D_SetStencilOp(context, sfail, dpfail, dppass);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetWriteMask(W3D_Context * context, ULONG mask)
{
ENTRY
	ULONG result;
	result = W3D_SetWriteMask(context, mask);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_WriteStencilPixel(W3D_Context * context, ULONG x, ULONG y, ULONG st)
{
ENTRY
	ULONG result;
	result = W3D_WriteStencilPixel(context, x, y, st);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_WriteStencilSpan(W3D_Context * context, ULONG x, ULONG y, ULONG n, ULONG * st, UBYTE * mask)
{
ENTRY
	ULONG result;
	result = W3D_WriteStencilSpan(context, x, y, n, st, mask);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

void stub_WriteZPixel(W3D_Context * context, ULONG x, ULONG y, W3D_Double * z)
{
ENTRY
	W3D_WriteZPixel(context, x, y, z);
EXIT
	return;
}

/***************************************************************************************************/

void stub_WriteZSpan(W3D_Context * context, ULONG x, ULONG y, ULONG n, W3D_Double * z, UBYTE * mask)
{
ENTRY
	W3D_WriteZSpan(context, x, y, n, z, mask);
EXIT
	return;
}

/***************************************************************************************************/

ULONG stub_SetCurrentColor(W3D_Context * context, W3D_Color * color)
{
ENTRY
	ULONG result;
	result = W3D_SetCurrentColor(context, color);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetCurrentPen(W3D_Context * context, ULONG pen)
{
ENTRY
	ULONG result;
	result = W3D_SetCurrentPen(context, pen);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_UpdateTexSubImage(W3D_Context * context, W3D_Texture * texture, void * teximage, ULONG lev, ULONG * palette, W3D_Scissor * scissor, ULONG srcbpr)
{
ENTRY
	ULONG result;
	result = W3D_UpdateTexSubImage(context, texture, teximage, lev, palette, scissor, srcbpr);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_FreeAllTexObj(W3D_Context * context)
{
ENTRY
	ULONG result;
	result = W3D_FreeAllTexObj(context);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_GetDestFmt(void)
{
ENTRY
	ULONG result;
	result = W3D_GetDestFmt();
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawLineStrip(W3D_Context * context, W3D_Lines * lines)
{
ENTRY
	ULONG result;
	result = W3D_DrawLineStrip(context, lines);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawLineLoop(W3D_Context * context, W3D_Lines * lines)
{
ENTRY
	ULONG result;
	result = W3D_DrawLineLoop(context, lines);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

APTR stub_GetDrivers(void)
{
ENTRY
	W3D_Driver ** driver;
	driver = W3D_GetDrivers();

// #ifdef PATH_RADEONHD
// 	for(int i = 0; driver[i]; i++) {
// 		PARAMX("** driver", driver[i])
// 		PARAMX("ChipID", driver[i]->ChipID)
// 		PARAMX("formats", driver[i]->formats)
// 		PARAMS("name", driver[i]->name)
// 		PARAMX("swdriver", driver[i]->swdriver)
// 		//driver[i]->formats |= W3D_FMT_R5G5B5;
// 	}
// #endif

RETURN(driver)
	return (driver);
}

/***************************************************************************************************/

ULONG stub_QueryDriver(W3D_Driver * driver, ULONG query, ULONG destfmt)
{
ENTRY
	ULONG result;
	result = W3D_QueryDriver(driver, query, destfmt);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_GetDriverTexFmtInfo(W3D_Driver * driver, ULONG format, ULONG destfmt)
{
ENTRY
	ULONG result;
	result = W3D_GetDriverTexFmtInfo(driver, format, destfmt);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_RequestMode(struct TagItem * taglist)
{
ENTRY

	ULONG result;

#ifdef PATCH_RADEONHD
	struct TagItem *ptr = taglist;
	while(ptr->ti_Tag != TAG_DONE) {
		PARAMX("ti_Tag", ptr->ti_Tag)
		PARAMX("ti_Data", ptr->ti_Data)
		// if(ptr->ti_Tag == W3D_SMR_TYPE)
		// 	ptr->ti_Data = W3D_DRIVER_3DHW;
		if(ptr->ti_Tag == W3D_SMR_DESTFMT)
			ptr->ti_Data |= W3D_FMT_A8R8G8B8;
		ptr++;
	}
	// struct TagItem newTags[] = {
	// 	{ W3D_SMR_TYPE, W3D_DRIVER_3DHW },
	// 	{ W3D_SMR_DESTFMT,
	// 		W3D_FMT_R5G5B5|
	// 		W3D_FMT_B5G5R5|
	// 		W3D_FMT_R5G6B5|
	// 		W3D_FMT_B5G6R5|
	// 		W3D_FMT_A8R8G8B8 },
	// 	{ TAG_DONE, 0 }
	// };
	// result = W3D_RequestMode(newTags);
#endif
	result = W3D_RequestMode(taglist);

RETURN(result)
	return (result);
}

/***************************************************************************************************/

void stub_SetScissor(W3D_Context * context, W3D_Scissor * scissor)
{
ENTRY
	W3D_SetScissor(context, scissor);
EXIT
	return;
}

/***************************************************************************************************/

void stub_FlushFrame(W3D_Context * context)
{
ENTRY
	W3D_FlushFrame(context);	
EXIT
	return;
}

/***************************************************************************************************/

APTR stub_TestMode(ULONG ModeID)
{
ENTRY
PARAMX("ModeID", ModeID)

	W3D_Driver * driver;
	driver = W3D_TestMode(ModeID);
RETURN(driver)
	return (driver);
}

/***************************************************************************************************/

ULONG stub_SetChromaTestBounds(W3D_Context * context, W3D_Texture * texture, ULONG rgba_lower, ULONG rgba_upper, ULONG mode)
{
ENTRY
	ULONG result;
	result = W3D_SetChromaTestBounds(context, texture, rgba_lower, rgba_upper, mode);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_ClearDrawRegion(W3D_Context * context, ULONG color)
{
ENTRY
	ULONG result;
	result = W3D_ClearDrawRegion(context, color);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawTriangleV(W3D_Context * context, W3D_TriangleV * triangle)
{
ENTRY
	ULONG result;
	result = W3D_DrawTriangleV(context, triangle);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawTriFanV(W3D_Context * context, W3D_TrianglesV * triangles)
{
ENTRY
	ULONG result;
	result = W3D_DrawTriFanV(context, triangles);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawTriStripV(W3D_Context * context, W3D_TrianglesV * triangles)
{
ENTRY
	ULONG result;
	result = W3D_DrawTriStripV(context, triangles);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

APTR stub_GetScreenmodeList(void)
{
ENTRY
	W3D_ScreenMode * list;
	list = W3D_GetScreenmodeList();
RETURN(list)
	return (list);
}

/***************************************************************************************************/

void stub_FreeScreenmodeList(W3D_ScreenMode * list)
{
ENTRY
	W3D_FreeScreenmodeList(list);
EXIT
	return;
}

/***************************************************************************************************/

ULONG stub_BestModeID(struct TagItem * tags)
{
ENTRY
	ULONG result;
	result = W3D_BestModeID(tags);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_VertexPointer(W3D_Context * context, void * pointer, int stride, ULONG mode, ULONG flags)
{
ENTRY
	ULONG result;
	result = W3D_VertexPointer(context, pointer, stride, mode, flags);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_TexCoordPointer(W3D_Context * context, void * pointer, int stride, int unit, int off_v, int off_w, ULONG flags)
{
ENTRY
	ULONG result;
	result = W3D_TexCoordPointer(context, pointer, stride, unit, off_v, off_w, flags);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_ColorPointer(W3D_Context * context, void * pointer, int stride, ULONG format, ULONG mode, ULONG flags)
{
ENTRY
	ULONG result;
	result = W3D_ColorPointer(context, pointer, stride, format, mode, flags);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

ULONG stub_BindTexture(W3D_Context * context, ULONG tmu, W3D_Texture * texture)
{
ENTRY
	ULONG result;
	result = W3D_BindTexture(context, tmu, texture);
RETURN(result)
	return (result);
}

/***************************************************************************************************/

void ColorToRGBA(UBYTE *RGBA,float r,float g,float b,float a)
{
ENTRY
	RGBA[0] = (UBYTE)(r*256.0);

	if(1.0 <= r)
		RGBA[0] =255;
	if(r <= 0.0)
		RGBA[0] = 0;

	RGBA[1] = (UBYTE)(g*256.0);

	if(1.0 <= g)
		RGBA[1]=255;
	if(g <= 0.0)
		RGBA[1]=0;

	RGBA[2] = (UBYTE)(b*256.0);

	if(1.0 <= b)
		RGBA[2]=255;
	if(b <= 0.0)
		RGBA[2]=0;

	RGBA[3] = (UBYTE)(a*256.0);

	if(1.0 <= a)
		RGBA[3]=255;
	if(a <= 0.0)
		RGBA[3]=0;
EXIT
}

/***************************************************************************************************/

void GetPoint(W3D_Context *context,struct point3D *P,ULONG i)
{
	UBYTE *V = context->VertexPointer;
	ULONG Vformat = context->VPMode;
	ULONG Vstride = context->VPStride;

	WORD unit = 0;	/* default use tex0 TODO: manage multi texturing */
	W3D_Texture *texture = context->CurrentTex[unit];
	UBYTE *UV = context->TexCoordPointer[unit];
	ULONG UVformat = context->TPFlags[unit];
	ULONG UVstride = context->TPStride[unit];
	ULONG UVoffsetv	= context->TPVOffs[unit];
	ULONG UVoffsetw	= context->TPWOffs[unit];

	UBYTE *C = context->ColorPointer;
	ULONG Cformat = context->CPMode;
	ULONG Cstride = context->CPStride;

	UBYTE *pt;
	struct VertexFFF *fff;
	struct VertexFFD *ffd;
	struct VertexDDD *ddd;
	float *u;
	float *v;
	float *w;
	float *rgbaF;
	UBYTE *rgbaB;
	UBYTE  RGBA[4];

	P->x=P->y=P->z=P->u=P->v=0.0;
	P->RGBA[0]=P->RGBA[1]=P->RGBA[2]=P->RGBA[3]=255; 	/* default: use white. TODO: use CurrentColor */

	/* recover XYZ values */
	if(V!=NULL)
	{
		pt=&(V[i*Vstride]);

		if(Vformat==W3D_VERTEX_F_F_F)
		{
			fff=(struct VertexFFF *)pt;
			P->x=fff->x;
			P->y=fff->y;
			P->z=fff->z;
		}

		if(Vformat==W3D_VERTEX_F_F_D)
		{
			ffd=(struct VertexFFD *)pt;
			P->x=ffd->x;
			P->y=ffd->y;
			P->z=(float)ffd->z;
		}

		if(Vformat==W3D_VERTEX_D_D_D)
		{
			ddd=(struct VertexDDD *)pt;
			P->x=(float)ddd->x;
			P->y=(float)ddd->y;
			P->z=(float)ddd->z;
		}

		P->w=1.0/P->z;
	}


/* recover UV values */
	if(UV!=NULL)
	{
		pt=&(UV[i*UVstride]);
		u=(float *)pt;
		v=(float *)&pt[UVoffsetv];
		w=(float *)&pt[UVoffsetw];

		if(UVformat==W3D_TEXCOORD_NORMALIZED)
		{
			P->u=u[0] ;
		P->v=v[0] ;
		P->w=w[0] ;
		}
		else
		{
			P->u=u[0] /texture->texwidth;
			P->v=v[0] /texture->texheight;
			P->w=w[0] ;
		}
	}

/* recover Color RGBA values */
	if(C!=NULL)
	{
	pt=&(C[i*Cstride]);
	if(Cformat AND W3D_COLOR_FLOAT)
		{
		rgbaF=(float *)pt;
		ColorToRGBA(RGBA,rgbaF[0],rgbaF[1],rgbaF[2],rgbaF[3]);
		}
	if(Cformat AND W3D_COLOR_UBYTE)
		{
		rgbaB=(UBYTE *)pt;
		COPYRGBA(RGBA,rgbaB);
		}
	if(Cformat AND W3D_CMODE_RGB)
		{P->RGBA[0]=RGBA[0]; P->RGBA[1]=RGBA[1]; P->RGBA[2]=RGBA[2]; P->RGBA[3]=255;		}
	if(Cformat AND W3D_CMODE_BGR)
		{P->RGBA[0]=RGBA[2]; P->RGBA[1]=RGBA[1]; P->RGBA[2]=RGBA[0]; P->RGBA[3]=255;		}
	if(Cformat AND W3D_CMODE_RGBA)
		{P->RGBA[0]=RGBA[0]; P->RGBA[1]=RGBA[1]; P->RGBA[2]=RGBA[2]; P->RGBA[3]=RGBA[3];	}
	if(Cformat AND W3D_CMODE_ARGB)
		{P->RGBA[0]=RGBA[1]; P->RGBA[1]=RGBA[2]; P->RGBA[2]=RGBA[3]; P->RGBA[3]=RGBA[0];	}
	if(Cformat AND W3D_CMODE_BGRA)
		{P->RGBA[0]=RGBA[2]; P->RGBA[1]=RGBA[1]; P->RGBA[2]=RGBA[0]; P->RGBA[3]=RGBA[3];	}
	}
}

/***************************************************************************************************/

void PtoV(W3D_Context *context,struct point3D *P,W3D_Vertex *v)
{
WORD   unit		=0;	/* default use tex0 TODO: manage multi texturing */
W3D_Texture *texture=context->CurrentTex[unit];

	v->x=P->x; v->y=P->y; v->z=P->z; v->w=P->w;
	v->u=P->u*texture->texwidth; v->v=P->v*texture->texheight;
	v->color.r=(float)P->RGBA[0]/255.0;
	v->color.g=(float)P->RGBA[1]/255.0;
	v->color.b=(float)P->RGBA[2]/255.0;
	v->color.a=(float)P->RGBA[3]/255.0;
}

/***************************************************************************************************/
	
void DrawTri(struct drawcontext *C,ULONG a,ULONG b,ULONG c)
{
W3D_Context *context=C->context;
WORD   unit		=0;	/* default use tex0 TODO: manage multi texturing */
W3D_Texture *texture=context->CurrentTex[unit];
W3D_Triangle tri;
W3D_Vertex *v;
UBYTE *I8 =C->indices;
UWORD *I16=C->indices;
ULONG *I32=C->indices;
struct point3D  PA;
struct point3D  PB;
struct point3D  PC;

	if(C->type==W3D_INDEX_NONE)
		{ a=a+C->base; b=b+C->base; c=c+C->base; }

	if(C->type==W3D_INDEX_UBYTE)
		{ a=I8[a];  b=I8[b];  c=I8[c];  }

	if(C->type==W3D_INDEX_UWORD)
		{ a=I16[a]; b=I16[b]; c=I16[c]; }

	if(C->type==W3D_INDEX_ULONG)
		{ a=I32[a]; b=I32[b]; c=I32[c]; }

	GetPoint(context,&PA,a);
	GetPoint(context,&PB,b);
	GetPoint(context,&PC,c);

	PtoV(context,&PA,&tri.v1);
	PtoV(context,&PB,&tri.v2);
	PtoV(context,&PC,&tri.v3);

	tri.tex=texture;

 	W3D_DrawTriangle(context, &tri);
}

/***************************************************************************************************/

void DrawPrimitive(struct drawcontext *C)
{
ULONG n;
ULONG primitive=C->primitive;
ULONG nb=C->count;

	if(primitive==W3D_PRIMITIVE_TRIANGLES)
	{
	nb=nb/3;
	NLOOP(nb)
		DrawTri(C,3*n+0,3*n+1,3*n+2);
	}

	if(primitive==W3D_PRIMITIVE_TRIFAN)
	{
	for (n=2;n<nb;n++)
		DrawTri(C,0,n-1,n);
	return;
	}

	if(primitive==W3D_PRIMITIVE_TRISTRIP)
	{
	for (n=2;n<nb;n++)
	if (n&1)	  /* reverse vertex order */
		DrawTri(C,n-1,n-2,n-0);
	else
		DrawTri(C,n-2,n-1,n-0);
	return;
	}

	if(primitive==W3D_PRIMITIVE_POINTS)
	{
	NLOOP(nb)
		{
		DrawTri(C,n,n,n);
		}
	}

	if(primitive==W3D_PRIMITIVE_LINES)
	{
	nb=nb/2;
	NLOOP(nb)
		{
		DrawTri(C,2*n,2*n+1,2*n+1);
		}
	return;
	}

	if(primitive==W3D_PRIMITIVE_LINELOOP)
	{
	nb=nb-1;
	NLOOP(nb)
		{
		DrawTri(C,n,n+1,n+1);
		}

	DrawTri(C,nb,0,0);
	return;
	}

	if(primitive==W3D_PRIMITIVE_LINESTRIP)
	{
	nb=nb-1;
	NLOOP(nb)
		{
		DrawTri(C,n,n+1,n+1);
		}
	return;
	}
}

/***************************************************************************************************/

ULONG stub_DrawArray(W3D_Context* context, ULONG primitive, ULONG base, ULONG count)
{
ENTRY
	struct drawcontext C;
	C.context		=	context;
	C.primitive		=	primitive;
	C.type		=	W3D_INDEX_NONE;
	C.count		=	count;
	C.indices		=	NULL;
	C.base		=	base;

	if (PatchFlag & PATCH_DRAWARRAY) {
		DrawPrimitive(&C);
RETURN(W3D_SUCCESS)
		return(W3D_SUCCESS);
	} else {
		ULONG result;
		result = W3D_DrawArray(context, primitive, base, count);
RETURN(result)
		return (result);
	}
}

/***************************************************************************************************/

ULONG stub_DrawElements(W3D_Context* context, ULONG primitive, ULONG type, ULONG count,void *indices)
{
ENTRY

	struct drawcontext C;

	C.context	= context;
	C.primitive	= primitive;
	C.type 	= type;
	C.count	= count;
	C.indices	= indices;
	C.base 	= 0;

	if (PatchFlag & PATCH_DRAWELEMENTS) {
		DrawPrimitive(&C);
RETURN(W3D_SUCCESS)
		return(W3D_SUCCESS);
	} else {
		ULONG result;	
		result = W3D_DrawElements(context, primitive, type, count, indices);
RETURN(result)
		return (result);
	}
}

/***************************************************************************************************/

void stub_SetFrontFace(W3D_Context * context, ULONG direction)
{
ENTRY
	W3D_SetFrontFace(context, direction);
EXIT
	return;
}

/***************************************************************************************************/

CONST APTR VecTable68K[] =
{
	NULL,						/*open*/
	NULL,						/*close*/
	NULL,
	NULL,
	(APTR)stub_CreateContext,
	(APTR)stub_DestroyContext,
	(APTR)stub_GetState,
	(APTR)stub_SetState,
	(APTR)stub_CheckDriver,
	(APTR)stub_LockHardware,
	(APTR)stub_UnLockHardware,
	(APTR)stub_WaitIdle,
	(APTR)stub_CheckIdle,
	(APTR)stub_Query,
	(APTR)stub_GetTexFmtInfo,
	(APTR)stub_AllocTexObj,
	(APTR)stub_FreeTexObj,
	(APTR)stub_ReleaseTexture,
	(APTR)stub_FlushTextures,
	(APTR)stub_SetFilter,
	(APTR)stub_SetTexEnv,
	(APTR)stub_SetWrapMode,
	(APTR)stub_UpdateTexImage,
	(APTR)stub_UploadTexture,
	(APTR)stub_DrawLine,
	(APTR)stub_DrawPoint,
	(APTR)stub_DrawTriangle,
	(APTR)stub_DrawTriFan,
	(APTR)stub_DrawTriStrip,
	(APTR)stub_SetAlphaMode,
	(APTR)stub_SetBlendMode,
	(APTR)stub_SetDrawRegion,
	(APTR)stub_SetFogParams,
	(APTR)stub_SetColorMask,
	(APTR)stub_SetStencilFunc,
	(APTR)stub_AllocZBuffer,
	(APTR)stub_FreeZBuffer,
	(APTR)stub_ClearZBuffer,
	(APTR)stub_ReadZPixel,
	(APTR)stub_ReadZSpan,
	(APTR)stub_SetZCompareMode,
	(APTR)stub_AllocStencilBuffer,
	(APTR)stub_ClearStencilBuffer,
	(APTR)stub_FillStencilBuffer,
	(APTR)stub_FreeStencilBuffer,
	(APTR)stub_ReadStencilPixel,
	(APTR)stub_ReadStencilSpan,
	(APTR)stub_SetLogicOp,
	(APTR)stub_Hint,
	(APTR)stub_SetDrawRegionWBM,
	(APTR)stub_GetDriverState,
	(APTR)stub_Flush,
	(APTR)stub_SetPenMask,
	(APTR)stub_SetStencilOp,
	(APTR)stub_SetWriteMask,
	(APTR)stub_WriteStencilPixel,
	(APTR)stub_WriteStencilSpan,
	(APTR)stub_WriteZPixel,
	(APTR)stub_WriteZSpan,
	(APTR)stub_SetCurrentColor,
	(APTR)stub_SetCurrentPen,
	(APTR)stub_UpdateTexSubImage,
	(APTR)stub_FreeAllTexObj,
	(APTR)stub_GetDestFmt,
	(APTR)stub_DrawLineStrip,
	(APTR)stub_DrawLineLoop,
	(APTR)stub_GetDrivers,
	(APTR)stub_QueryDriver,
	(APTR)stub_GetDriverTexFmtInfo,
	(APTR)stub_RequestMode,
	(APTR)stub_SetScissor,
	(APTR)stub_FlushFrame,
	(APTR)stub_TestMode,
	(APTR)stub_SetChromaTestBounds,
	(APTR)stub_ClearDrawRegion,
	(APTR)stub_DrawTriangleV,
	(APTR)stub_DrawTriFanV,
	(APTR)stub_DrawTriStripV,
	(APTR)stub_GetScreenmodeList,
	(APTR)stub_FreeScreenmodeList,
	(APTR)stub_BestModeID,
	(APTR)stub_VertexPointer,
	(APTR)stub_TexCoordPointer,
	(APTR)stub_ColorPointer,
	(APTR)stub_BindTexture,
	(APTR)stub_DrawArray,
	(APTR)stub_DrawElements,
	(APTR)stub_SetFrontFace,
	(APTR)-1
};
