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

/***************************************************************************************************/

APTR stub_CreateContext(ULONG * error, struct TagItem * CCTags)
{
	W3D_Context *context;
	context = W3D_CreateContext(error, CCTags);
	return (context);
}

/***************************************************************************************************/

void stub_DestroyContext(W3D_Context * context)
{
    W3D_DestroyContext(context);
    return;
}

/***************************************************************************************************/

ULONG stub_GetState (W3D_Context * context, ULONG state) 
{
	ULONG result;
	result = W3D_GetState(context, state);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetState(W3D_Context * context, ULONG state, ULONG action)
{
	ULONG result;
	result = W3D_SetState(context, state, action);
	return (result);
}

/***************************************************************************************************/

ULONG stub_CheckDriver(void)
{
	ULONG result;
	result = W3D_CheckDriver();
	return (result);
}

/***************************************************************************************************/

ULONG stub_LockHardware(W3D_Context * context)
{
	ULONG result;
	result = W3D_LockHardware(context);
	return (result);
}

/***************************************************************************************************/

void stub_UnLockHardware(W3D_Context * context)
{
	W3D_UnLockHardware(context);
	return;
}

/***************************************************************************************************/

void stub_WaitIdle(W3D_Context * context)
{
	W3D_WaitIdle(context);	
	return;
}

/***************************************************************************************************/

ULONG stub_CheckIdle(W3D_Context * context)
{
	ULONG result;
	result = W3D_CheckIdle(context);
	return (result);
}

/***************************************************************************************************/

ULONG stub_Query(W3D_Context * context, ULONG query, ULONG destfmt)
{
	ULONG result;
	result = W3D_Query(context, query, destfmt);
	return (result);
}

/***************************************************************************************************/

ULONG stub_GetTexFmtInfo(W3D_Context * context, ULONG format, ULONG destfmt)
{
	ULONG result;
	result = W3D_GetTexFmtInfo(context, format, destfmt);
	return (result);
}

/***************************************************************************************************/

APTR stub_AllocTexObj(W3D_Context * context, ULONG * error, struct TagItem * ATOTags)
{
	W3D_Texture *texture;
	texture = W3D_AllocTexObj(context, error, ATOTags);
	return (texture);
}

/***************************************************************************************************/

void stub_FreeTexObj(W3D_Context * context, W3D_Texture * texture)
{
	W3D_FreeTexObj(context, texture);
	return;
}

/***************************************************************************************************/

void stub_ReleaseTexture(W3D_Context * context, W3D_Texture * texture)
{
	W3D_ReleaseTexture(context, texture);
	return;
}

/***************************************************************************************************/

void stub_FlushTextures(W3D_Context * context)
{
	W3D_FlushTextures(context);
	return;
}

/***************************************************************************************************/

ULONG stub_SetFilter(W3D_Context * context, W3D_Texture * texture, ULONG min, ULONG mag)
{
	ULONG result;
	result = W3D_SetFilter(context, texture, min, mag);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetTexEnv(W3D_Context * context, W3D_Texture * texture, ULONG envparam, W3D_Color * envcolor)
{
	ULONG result;
	result = W3D_SetTexEnv(context, texture, envparam, envcolor);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetWrapMode(W3D_Context * context, W3D_Texture * texture, ULONG mode_s, ULONG mode_t, W3D_Color * bordercolor)
{
	ULONG result;
	result = W3D_SetWrapMode(context, texture, mode_s, mode_t, bordercolor);
	return (result);
}

/***************************************************************************************************/

ULONG stub_UpdateTexImage(W3D_Context * context, W3D_Texture * texture, void * teximage, int level, ULONG * palette)
{
	ULONG result;
	result = W3D_UpdateTexImage(context, texture, teximage, level, palette);
	return (result);
}

/***************************************************************************************************/

ULONG stub_UploadTexture(W3D_Context * context, W3D_Texture * texture)
{
	ULONG result;
	result = W3D_UploadTexture(context, texture);
	return (result);
}

/***************************************************************************************************/

ULONG DrawLine(W3D_Context *context,W3D_Vertex *v1,W3D_Vertex *v2,W3D_Texture *tex,float size)
{
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
	return(result);
}

/***************************************************************************************************/

ULONG stub_DrawLine(W3D_Context *context, W3D_Line *line)
{
	if (PatchFlag & PATCH_DRAWLINE) {
		register ULONG result;
		result = DrawLine(context, &line->v1, &line->v2, line->tex, line->linewidth);
		return(result);
	} else {
		ULONG result;
		result = W3D_DrawLine(context, line);
		return (result);
	}
}
/***************************************************************************************************/

ULONG stub_DrawPoint(W3D_Context * context, W3D_Point * point)
{
	ULONG result;
	result = W3D_DrawPoint(context, point);
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawTriangle(W3D_Context * context, W3D_Triangle * triangle)
{
	ULONG result;
	result = W3D_DrawTriangle(context, triangle);
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawTriFan(W3D_Context * context, W3D_Triangles * triangles)
{
	ULONG result;
	result = W3D_DrawTriFan(context, triangles);
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawTriStrip(W3D_Context * context, W3D_Triangles * triangles)
{
	ULONG result;
	result = W3D_DrawTriStrip(context, triangles);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetAlphaMode(W3D_Context * context, ULONG mode, W3D_Float * refval)
{
	ULONG result;
	result = W3D_SetAlphaMode(context, mode, refval);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetBlendMode(W3D_Context * context, ULONG srcfunc, ULONG dstfunc)
{
	ULONG result;
	result = W3D_SetBlendMode(context, srcfunc, dstfunc);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetDrawRegion(W3D_Context * context, struct BitMap * bm, int yoffset, W3D_Scissor * scissor)
{
	ULONG result;
	result = W3D_SetDrawRegion(context, bm, yoffset, scissor);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetFogParams(W3D_Context * context, W3D_Fog * fogparams, ULONG fogmode)
{
	ULONG result;
	result = W3D_SetFogParams(context, fogparams, fogmode);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetColorMask(W3D_Context * context, W3D_Bool red, W3D_Bool green, W3D_Bool blue, W3D_Bool alpha)
{
	ULONG result;
	result = W3D_SetColorMask(context, red, green, blue, alpha);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetStencilFunc(W3D_Context * context, ULONG func, ULONG refvalue, ULONG mask)
{
	ULONG result;
	result = W3D_SetStencilFunc(context, func, refvalue, mask);
	return (result);
}

/***************************************************************************************************/

ULONG stub_AllocZBuffer(W3D_Context * context)
{
	ULONG result;
	result = W3D_AllocZBuffer(context);
	return (result);
}

/***************************************************************************************************/

ULONG stub_FreeZBuffer(W3D_Context * context)
{
	ULONG result;
	result = W3D_FreeZBuffer(context);
	return (result);
}

/***************************************************************************************************/

ULONG stub_ClearZBuffer(W3D_Context * context, W3D_Double * clearvalue)
{
	ULONG result;
	result = W3D_ClearZBuffer(context, clearvalue);
	return (result);
}

/***************************************************************************************************/

ULONG stub_ReadZPixel(W3D_Context * context, ULONG x, ULONG y, W3D_Double * z)
{
	ULONG result;
	result = W3D_ReadZPixel(context, x, y, z);
	return (result);
}

/***************************************************************************************************/

ULONG stub_ReadZSpan(W3D_Context * context, ULONG x, ULONG y, ULONG n, W3D_Double * z)
{
	ULONG result;
	result = W3D_ReadZSpan(context, x, y, n, z);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetZCompareMode(W3D_Context * context, ULONG mode)
{
	ULONG result;
	result = W3D_SetZCompareMode(context, mode);
	return (result);
}

/***************************************************************************************************/

ULONG stub_AllocStencilBuffer(W3D_Context * context)
{
	ULONG result;
	result = W3D_AllocStencilBuffer(context);
	return (result);
}

/***************************************************************************************************/

ULONG stub_ClearStencilBuffer(W3D_Context * context, ULONG * clearval)
{
	ULONG result;
	result = W3D_ClearStencilBuffer(context, clearval);
	return (result);
}

/***************************************************************************************************/

ULONG stub_FillStencilBuffer(W3D_Context * context, ULONG x, ULONG y, ULONG width, ULONG height, ULONG depth, void * data)
{
	ULONG result;
	result = W3D_FillStencilBuffer(context, x, y, width, height, depth, data);
	return (result);
}

/***************************************************************************************************/

ULONG stub_FreeStencilBuffer(W3D_Context * context)
{
	ULONG result;
	result = W3D_FreeStencilBuffer(context);
	return (result);
}

/***************************************************************************************************/

ULONG stub_ReadStencilPixel(W3D_Context * context, ULONG x, ULONG y, ULONG * st)
{
	ULONG result;
	result = W3D_ReadStencilPixel(context, x, y, st);
	return (result);
}

/***************************************************************************************************/

ULONG stub_ReadStencilSpan(W3D_Context * context, ULONG x, ULONG y, ULONG n, ULONG * st)
{
	ULONG result;
	result = W3D_ReadStencilSpan(context, x, y, n, st);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetLogicOp(W3D_Context * context, ULONG operation)
{
	ULONG result;
	result = W3D_SetLogicOp(context, operation);
	return (result);
}

/***************************************************************************************************/

ULONG stub_Hint(W3D_Context * context, ULONG mode, ULONG quality)
{
	ULONG result;
	result = W3D_Hint(context, mode, quality);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetDrawRegionWBM(W3D_Context * context, W3D_Bitmap * bitmap, W3D_Scissor * scissor)
{
	ULONG result;
	result = W3D_SetDrawRegionWBM(context, bitmap, scissor);
	return (result);
}

/***************************************************************************************************/

ULONG stub_GetDriverState(W3D_Context * context)
{
	ULONG result;
	result = W3D_GetDriverState(context);
	return (result);
}

/***************************************************************************************************/

ULONG stub_Flush(W3D_Context * context)
{
	ULONG result;
	result = W3D_Flush(context);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetPenMask(W3D_Context * context, ULONG pen)
{
	ULONG result;
	result = W3D_SetPenMask(context, pen);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetStencilOp(W3D_Context * context, ULONG sfail, ULONG dpfail, ULONG dppass)
{
	ULONG result;
	result = W3D_SetStencilOp(context, sfail, dpfail, dppass);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetWriteMask(W3D_Context * context, ULONG mask)
{
	ULONG result;
	result = W3D_SetWriteMask(context, mask);
	return (result);
}

/***************************************************************************************************/

ULONG stub_WriteStencilPixel(W3D_Context * context, ULONG x, ULONG y, ULONG st)
{
	ULONG result;
	result = W3D_WriteStencilPixel(context, x, y, st);
	return (result);
}

/***************************************************************************************************/

ULONG stub_WriteStencilSpan(W3D_Context * context, ULONG x, ULONG y, ULONG n, ULONG * st, UBYTE * mask)
{
	ULONG result;
	result = W3D_WriteStencilSpan(context, x, y, n, st, mask);
	return (result);
}

/***************************************************************************************************/

void stub_WriteZPixel(W3D_Context * context, ULONG x, ULONG y, W3D_Double * z)
{
	W3D_WriteZPixel(context, x, y, z);
	return;
}

/***************************************************************************************************/

void stub_WriteZSpan(W3D_Context * context, ULONG x, ULONG y, ULONG n, W3D_Double * z, UBYTE * mask)
{
	W3D_WriteZSpan(context, x, y, n, z, mask);
	return;
}

/***************************************************************************************************/

ULONG stub_SetCurrentColor(W3D_Context * context, W3D_Color * color)
{
	ULONG result;
	result = W3D_SetCurrentColor(context, color);
	return (result);
}

/***************************************************************************************************/

ULONG stub_SetCurrentPen(W3D_Context * context, ULONG pen)
{
	ULONG result;
	result = W3D_SetCurrentPen(context, pen);
	return (result);
}

/***************************************************************************************************/

ULONG stub_UpdateTexSubImage(W3D_Context * context, W3D_Texture * texture, void * teximage, ULONG lev, ULONG * palette, W3D_Scissor * scissor, ULONG srcbpr)
{
	ULONG result;
	result = W3D_UpdateTexSubImage(context, texture, teximage, lev, palette, scissor, srcbpr);
	return (result);
}

/***************************************************************************************************/

ULONG stub_FreeAllTexObj(W3D_Context * context)
{
	ULONG result;
	result = W3D_FreeAllTexObj(context);
	return (result);
}

/***************************************************************************************************/

ULONG stub_GetDestFmt(void)
{
	ULONG result;
	result = W3D_GetDestFmt();
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawLineStrip(W3D_Context * context, W3D_Lines * lines)
{
	ULONG result;
	result = W3D_DrawLineStrip(context, lines);
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawLineLoop(W3D_Context * context, W3D_Lines * lines)
{
	ULONG result;
	result = W3D_DrawLineLoop(context, lines);
	return (result);
}

/***************************************************************************************************/

APTR stub_GetDrivers(void)
{
	W3D_Driver ** driver;
	driver = W3D_GetDrivers();
	return (driver);
}

/***************************************************************************************************/

ULONG stub_QueryDriver(W3D_Driver * driver, ULONG query, ULONG destfmt)
{
	ULONG result;
	result = W3D_QueryDriver(driver, query, destfmt);
	return (result);
}

/***************************************************************************************************/

ULONG stub_GetDriverTexFmtInfo(W3D_Driver * driver, ULONG format, ULONG destfmt)
{
	ULONG result;
	result = W3D_GetDriverTexFmtInfo(driver, format, destfmt);
	return (result);
}

/***************************************************************************************************/

ULONG stub_RequestMode(struct TagItem * taglist)
{
	ULONG result;
	result = W3D_RequestMode(taglist);
	return (result);
}

/***************************************************************************************************/

void stub_SetScissor(W3D_Context * context, W3D_Scissor * scissor)
{
	W3D_SetScissor(context, scissor);
	return;
}

/***************************************************************************************************/

void stub_FlushFrame(W3D_Context * context)
{
	W3D_FlushFrame(context);	
	return;
}

/***************************************************************************************************/

APTR stub_TestMode(ULONG ModeID)
{
	W3D_Driver * driver;
	driver = W3D_TestMode(ModeID);
	return (driver);
}

/***************************************************************************************************/

ULONG stub_SetChromaTestBounds(W3D_Context * context, W3D_Texture * texture, ULONG rgba_lower, ULONG rgba_upper, ULONG mode)
{
	ULONG result;
	result = W3D_SetChromaTestBounds(context, texture, rgba_lower, rgba_upper, mode);
	return (result);
}

/***************************************************************************************************/

ULONG stub_ClearDrawRegion(W3D_Context * context, ULONG color)
{
	ULONG result;
	result = W3D_ClearDrawRegion(context, color);
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawTriangleV(W3D_Context * context, W3D_TriangleV * triangle)
{
	ULONG result;
	result = W3D_DrawTriangleV(context, triangle);
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawTriFanV(W3D_Context * context, W3D_TrianglesV * triangles)
{
	ULONG result;
	result = W3D_DrawTriFanV(context, triangles);
	return (result);
}

/***************************************************************************************************/

ULONG stub_DrawTriStripV(W3D_Context * context, W3D_TrianglesV * triangles)
{
	ULONG result;
	result = W3D_DrawTriStripV(context, triangles);
	return (result);
}

/***************************************************************************************************/

APTR stub_GetScreenmodeList(void)
{
	W3D_ScreenMode * list;
	list = W3D_GetScreenmodeList();
	return (list);
}

/***************************************************************************************************/

void stub_FreeScreenmodeList(W3D_ScreenMode * list)
{
	W3D_FreeScreenmodeList(list);
	return;
}

/***************************************************************************************************/

ULONG stub_BestModeID(struct TagItem * tags)
{
	ULONG result;
	result = W3D_BestModeID(tags);
	return (result);
}

/***************************************************************************************************/

ULONG stub_VertexPointer(W3D_Context * context, void * pointer, int stride, ULONG mode, ULONG flags)
{
	ULONG result;
	result = W3D_VertexPointer(context, pointer, stride, mode, flags);
	return (result);
}

/***************************************************************************************************/

ULONG stub_TexCoordPointer(W3D_Context * context, void * pointer, int stride, int unit, int off_v, int off_w, ULONG flags)
{
	ULONG result;
	result = W3D_TexCoordPointer(context, pointer, stride, unit, off_v, off_w, flags);
	return (result);
}

/***************************************************************************************************/

ULONG stub_ColorPointer(W3D_Context * context, void * pointer, int stride, ULONG format, ULONG mode, ULONG flags)
{
	ULONG result;
	result = W3D_ColorPointer(context, pointer, stride, format, mode, flags);
	return (result);
}

/***************************************************************************************************/

ULONG stub_BindTexture(W3D_Context * context, ULONG tmu, W3D_Texture * texture)
{
	ULONG result;
	result = W3D_BindTexture(context, tmu, texture);
	return (result);
}

/***************************************************************************************************/

void ColorToRGBA(UBYTE *RGBA,float r,float g,float b,float a)
{
	RGBA[0] = (UBYTE)(r*256.0);

	if(1.0 <= r)
		RGBA[0] =2 55;
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
	if(V!=NULL)
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
	struct drawcontext C;
	C.context	=	context;
	C.primitive	=	primitive;
	C.type		=	W3D_INDEX_NONE;
	C.count		=	count;
	C.indices	=	NULL;
	C.base		=	base;

	if (PatchFlag & PATCH_DRAWARRAY) {
		DrawPrimitive(&C);
		return(W3D_SUCCESS);
	} else {
		ULONG result;
		result = W3D_DrawArray(context, primitive, base, count);
		return (result);
	}
}

/***************************************************************************************************/

ULONG stub_DrawElements(W3D_Context* context, ULONG primitive, ULONG type, ULONG count,void *indices)
{

	struct drawcontext C;

	C.context = context;
	C.primitive = primitive;
	C.type = type;
	C.count	= count;
	C.indices = indices;
	C.base = 0;

	if (PatchFlag & PATCH_DRAWELEMENTS) {
		DrawPrimitive(&C);
		return(W3D_SUCCESS);
	} else {
		ULONG result;	
		result = W3D_DrawElements(context, primitive, type, count, indices);
		return (result);
	}
}

/***************************************************************************************************/

void stub_SetFrontFace(W3D_Context * context, ULONG direction)
{
	W3D_SetFrontFace(context, direction);
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
	(CONST_APTR)-1
};
