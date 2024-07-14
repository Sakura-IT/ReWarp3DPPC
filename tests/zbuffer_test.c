#include <Warp3D/Warp3D.h>
#include <proto/Warp3D.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <stdio.h>

struct Library *Warp3DBase = 0;
struct Warp3DIFace *IWarp3D = 0;

int main() {
  Warp3DBase = IExec->OpenLibrary("Warp3D.library", 50L);
  IWarp3D = (struct Warp3DIFace *)IExec->GetInterface(Warp3DBase, "main", 1, TAG_DONE);
  if(!IWarp3D) { printf("Failed to open Warp3D.\n"); return 0; }
  
  IWarp3D->W3D_CheckDriver();
 
// [ENTRY :] stub_CheckDriver in file warp3dppc_68k.c line 134
// [ENTRY :] stub_GetDrivers in file warp3dppc_68k.c line 867

  ULONG ModeID = IWarp3D->W3D_RequestModeTags(
    W3D_SMR_TYPE, W3D_DRIVER_3DHW,
    W3D_SMR_DESTFMT,
      W3D_FMT_R5G5B5|
      W3D_FMT_B5G5R5|
      W3D_FMT_R5G6B5|
      W3D_FMT_B5G6R5|
      W3D_FMT_A8R8G8B8, //!!
    TAG_DONE);

// ENTRY :] stub_RequestMode in file warp3dppc_68k.c line 934
// [PARAM] ti_Tag == 0x80202002  type == W3D_DRIVER_3DHW
// [PARAM] ti_Data == 0x4
// [PARAM] ti_Tag == 0x80202001 destfmt ==

// [PARAM] ti_Data == 0x66
// [RETURN : 0x50001300] stub_RequestMode in file warp3dppc_68k.c line 947

  APTR driver = IWarp3D->W3D_TestMode(ModeID);
  if(!driver) { printf("Failed to aquire driver.\n"); return 0; }

// [ENTRY :] stub_TestMode in file warp3dppc_68k.c line 953
// [PARAM] ModeID == 0x500f1300

  // struct BitMap *bm = IGraphics->AllocBitMapTags(
  struct Screen *wb = IIntuition->LockPubScreen(0); 
  struct BitMap *bm = wb ? IGraphics->AllocBitMap(800, 600, 32, BMF_CLEAR|BMF_DISPLAYABLE, wb->RastPort.BitMap) : 0;
  IIntuition->UnlockPubScreen(0, wb);
  
  if(!bm) { printf("Failed to allocate bitmap.\n"); return 0; }
  
  ULONG error;
  W3D_Context *context = IWarp3D->W3D_CreateContextTags(&error,
    W3D_CC_MODEID, ModeID,
    W3D_CC_BITMAP, bm,
    W3D_CC_YOFFSET, 0,
    W3D_CC_DRIVERTYPE, W3D_DRIVER_3DHW,
    // W3D_CC_GLOBALTEXENV, TRUE,
    // W3D_CC_INDIRECT, FALSE,
    // W3D_CC_FAST, TRUE,
    TAG_DONE);
  if(!context) { printf("Failed to create context.\n"); return 0; }
    
// ENTRY :] stub_CreateContext in file warp3dppc_68k.c line 86
// ENTRY :] stub_CreateContext in file warp3dppc_68k.c line 86
// [PARAM] [TAG  :] == 0x80200008 ModeID
// [PARAM] [DATA :] == 0x50001300
// [PARAM] [TAG  :] == 0x80200000 BitMap
// [PARAM] [DATA :] == 0x6aa8bc38
// [PARAM] [TAG  :] == 0x80200001 yOffset
// [PARAM] [DATA :] == 0x0
// [PARAM] [TAG  :] == 0x80200002  DriverType == 3DHW
// [PARAM] [DATA :] == 0x4
// [PARAM] [TAG  :] == 0x80200005 GlobalTexEnv == TRUE
// [PARAM] [DATA :] == 0x1
// [PARAM] [TAG  :] == 0x80200004  Indirect == FALSE
// [PARAM] [DATA :] == 0x0
// [PARAM] [TAG  :] == 0x80200007 Fast == TRUE
// [PARAM] [DATA :] == 0x1
// [RETURN : 0x6e4ed000] stub_CreateContext in file warp3dppc_68k.c line 99

  ULONG result = IWarp3D->W3D_AllocZBuffer(context);
  if(result != W3D_SUCCESS) { printf("Failed to allocate z-buffer.\n"); return 0; }

// [ENTRY :] stub_AllocZBuffer in file warp3dppc_68k.c line 514
// [PARAM] context == 0x69f7e5e0
// [RETURN : 0x0] stub_AllocZBuffer in file warp3dppc_68k.c line 519

  result = IWarp3D->W3D_SetDrawRegion(context, bm, 0, /*scissor ??*/0);
  if(result != W3D_SUCCESS) { printf("Failed to set draw region.\n"); return 0; }
  
// [ENTRY :] stub_SetDrawRegion in file warp3dppc_68k.c line 467
// [RETURN : 0x0] stub_SetDrawRegion in file warp3dppc_68k.c line 473

  result = IWarp3D->W3D_SetState(context, W3D_GOURAUD, TRUE);
  if(result != W3D_SUCCESS) { printf("Failed to set Gouroud shading.\n"); }

  result = IWarp3D->W3D_SetState(context, W3D_DITHERING, TRUE);
  if(result != W3D_SUCCESS) { printf("Failed to set dithering.\n"); }
  
// [ENTRY :] stub_SetState in file warp3dppc_68k.c line 120
// [PARAM] context == 0x69f7e5e0 
// [PARAM] state == 0x400
// [PARAM] action == 0x1
// [RETURN : 0x0] stub_SetState in file warp3dppc_68k.c line 126
// [ENTRY :] stub_SetState in file warp3dppc_68k.c line 120
// [PARAM] context == 0x69f7e5e0
// [PARAM] state == 0x80000
// [PARAM] action == 0x1
// [RETURN : 0xffffffef] stub_SetState in file warp3dppc_68k.c line 126

  result = IWarp3D->W3D_SetZCompareMode(context, W3D_Z_LESS);
  if(result != W3D_SUCCESS) { printf("Failed to set z buffer compare mode.\n"); }
  
// [ENTRY :] stub_SetZCompareMode in file warp3dppc_68k.c line 580
// [PARAM] mode == 0x2
// [RETURN : 0x0] stub_SetZCompareMode in file warp3dppc_68k.c line 586


  result = IWarp3D->W3D_SetBlendMode(context, 0, 0); //??
  
// [ENTRY :] stub_SetBlendMode in file warp3dppc_68k.c line 456
// [RETURN : 0x0] stub_SetBlendMode in file warp3dppc_68k.c line 459

  result = IWarp3D->W3D_LockHardware(context);
  if(result != W3D_SUCCESS) { printf("Failed to lock hardware.\n"); }

// [ENTRY :] stub_LockHardware in file warp3dppc_68k.c line 150
// [RETURN : 0x0] stub_LockHardware in file warp3dppc_68k.c line 153

  // static W3D_Double clearValue = 0;
  // result = IWarp3D->W3D_ClearZBuffer(context, &clearValue);
  // if(result != W3D_SUCCESS) { printf("Failed to clear z-buffer.\n"); }

  // W3D_Color color = { 0.0, 0.0, 0.0 };
  W3D_Double depth = 0.0;
  result = IWarp3D->W3D_ClearBuffers(context, 0/*&color*/, &depth, 0);
  if(result != W3D_SUCCESS) { printf("Failed to clear z-buffer.\n"); }

// [ENTRY :] stub_ClearZBuffer in file warp3dppc_68k.c line 539
// [PARAM] context == 0x69f7e5e0
// [PARAM] clearvalue == 0x6a009784
// [RETURN : 0xffffffe9] stub_ClearZBuffer in file warp3dppc_68k.c line 546

  IWarp3D->W3D_FlushFrame(context);
  IWarp3D->W3D_UnLockHardware(context);
  
// [ENTRY :] stub_FlushFrame in file warp3dppc_68k.c line 943
// [EXIT :] stub_FlushFrame in file warp3dppc_68k.c line 945
// [ENTRY :] stub_UnLockHardware in file warp3dppc_68k.c line 161
// [EXIT :] stub_UnLockHardware in file warp3dppc_68k.c line 163

  IWarp3D->W3D_DestroyContext(context);

  IExec->DropInterface((struct Interface *)IWarp3D);
  IExec->CloseLibrary(Warp3DBase);
  
  return 0;
}
