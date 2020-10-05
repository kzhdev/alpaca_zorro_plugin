///////////////////////////////////////////////////
// Direct3D 9.0c interface
///////////////////////////////////////////////////

#ifndef d3d9_h
#define d3d9_h

#include <windows.h>

#ifndef BEGIN_INTERFACE
#define BEGIN_INTERFACE
#define END_INTERFACE
#endif

//#define THIS_ INTERFACE * _This,
//#define THIS INTERFACE * _This 
//#ifdef CONST_VTABLE
//#undef CONST_VTBL
//#define CONST_VTBL const
//#define DECLARE_INTERFACE(iface) typedef interface iface {struct iface##Vtbl *lpVtbl;} iface; typedef struct iface##Vtbl iface##Vtbl;struct iface##Vtbl
//#else
//#undef CONST_VTBL
//#define CONST_VTBL
//#define DECLARE_INTERFACE(iface) typedef interface iface {struct iface##Vtbl * lpVtbl;} iface; typedef struct iface##Vtbl iface##Vtbl; struct iface##Vtbl
//#endif

#define MY_DECLARE_INTERFACE(iface)  typedef struct iface##Vtbl iface##Vtbl; typedef interface iface {iface##Vtbl * lpVtbl;} iface;
#define DECLARE_INTERFACE(iface) struct iface##Vtbl
#define DECLARE_INTERFACE_(iface,baseiface) DECLARE_INTERFACE(iface)
#define THIS_ void * _This ,
#define THIS  void * _This
#define CONST
#define STDMETHODCALLTYPE __stdcall
#define STDMETHOD(method) long STDMETHODCALLTYPE method
#define STDMETHOD_(type,method) type STDMETHODCALLTYPE method
#define PURE

#define D3DCOLOR_ARGB(a,r,g,b) ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define D3DCOLOR_RGBA(r,g,b,a) D3DCOLOR_ARGB(a,r,g,b)
#define D3DCOLOR_XRGB(r,g,b)   D3DCOLOR_ARGB(0xff,r,g,b)

#define D3D_SDK_VERSION 31

#define REFIID long
#define REFGUID long
#define ULONG long
#define INT int

#define SUCCEEDED(Status) ((HRESULT)(Status) >= 0)
#define FAILED(Status) ((HRESULT)(Status)<0)

#define D3DADAPTER_IDENTIFIER9 void
#define D3DDISPLAYMODE void
#define D3DCAPS9 void
#define D3DDEVICE_CREATION_PARAMETERS  void
#define RGNDATA  void
#define D3DRASTER_STATUS  void
#define D3DGAMMARAMP  void
#define D3DCLIPSTATUS9  void
#define PALETTEENTRY  void
#define D3DVERTEXELEMENT9  void
#define D3DRECTPATCH_INFO  void
#define D3DTRIPATCH_INFO  void
#define D3DVERTEXBUFFER_DESC void
#define D3DSURFACE_DESC void
#define D3DLOCKED_RECT void
#define LPDIRECT3DINDEXBUFFER9 void
#define D3DXATTRIBUTERANGE void

#define D3DFORMAT long
#define D3DDEVTYPE long
#define D3DRESOURCETYPE long
#define D3DMULTISAMPLE_TYPE long
#define HMONITOR long
#define D3DBACKBUFFER_TYPE long
#define D3DPOOL long
#define D3DTEXTUREFILTERTYPE long
#define D3DCOLOR long
#define D3DTRANSFORMSTATETYPE  long
#define D3DRENDERSTATETYPE  long
#define D3DSTATEBLOCKTYPE  long
#define D3DTEXTURESTAGESTATETYPE  long
#define D3DSAMPLERSTATETYPE  long
#define D3DPRIMITIVETYPE   long
#define D3DQUERYTYPE   long
#define D3DSWAPEFFECT   long
#define D3DLIGHTTYPE   long

#ifndef DSBUFFERDESC_DEFINED
typedef struct WAVEFORMATEX {
    WORD wFormatTag;
    WORD nChannels;
    DWORD nSamplesPerSec;
    DWORD nAvgBytesPerSec;
    WORD nBlockAlign;
    WORD wBitsPerSample;
    WORD cbSize;
} WAVEFORMATEX;

typedef struct DSBUFFERDESC {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwBufferBytes;
    DWORD dwReserved;
    WAVEFORMATEX* lpwfxFormat;
    GUID guid3DAlgorithm;
} DSBUFFERDESC;
#define DSBUFFERDESC_DEFINED
#endif

#ifndef D3DVECTOR_DEFINED
typedef struct _D3DVECTOR {
    float x;
    float y;
    float z;
} D3DVECTOR;
#define D3DVECTOR_DEFINED
#endif

#ifndef D3DCOLORVALUE_DEFINED
typedef struct _D3DCOLORVALUE {
    float r;
    float g;
    float b;
    float a;
} D3DCOLORVALUE;
#define D3DCOLORVALUE_DEFINED
#endif

#ifndef D3DRECT_DEFINED
typedef struct _D3DRECT {
    LONG x1;
    LONG y1;
    LONG x2;
    LONG y2;
} D3DRECT;
#define D3DRECT_DEFINED
#endif

#ifndef D3DMATRIX_DEFINED
typedef struct _D3DMATRIX {
        float m[4][4];
} D3DMATRIX;

#define _11 m[0][0]
#define _12 m[0][1]
#define _13 m[0][2]
#define _14 m[0][3]
#define _21 m[1][0]
#define _22 m[1][1]
#define _23 m[1][2]
#define _24 m[1][3]
#define _31 m[2][0]
#define _32 m[2][1]
#define _33 m[2][2]
#define _34 m[2][3]
#define _41 m[3][0]
#define _42 m[3][1]
#define _43 m[3][2]
#define _44 m[3][3]

#define D3DMATRIX_DEFINED
#endif

typedef struct _D3DVIEWPORT9 {
    DWORD       X;
    DWORD       Y;            /* Viewport Top left */
    DWORD       Width;
    DWORD       Height;       /* Viewport Dimensions */
    float       MinZ;         /* Min/max of clip Volume */
    float       MaxZ;
} D3DVIEWPORT9;

typedef struct _D3DMATERIAL9 {
    D3DCOLORVALUE   Diffuse;        /* Diffuse color RGBA */
    D3DCOLORVALUE   Ambient;        /* Ambient color RGB */
    D3DCOLORVALUE   Specular;       /* Specular 'shininess' */
    D3DCOLORVALUE   Emissive;       /* Emissive color RGB */
    float           Power;          /* Sharpness if specular highlight */
} D3DMATERIAL9;

typedef struct _D3DLIGHT9 {
    D3DLIGHTTYPE    Type;            /* Type of light source */
    D3DCOLORVALUE   Diffuse;         /* Diffuse color of light */
    D3DCOLORVALUE   Specular;        /* Specular color of light */
    D3DCOLORVALUE   Ambient;         /* Ambient color of light */
    D3DVECTOR       Position;         /* Position in world space */
    D3DVECTOR       Direction;        /* Direction in world space */
    float           Range;            /* Cutoff range */
    float           Falloff;          /* Falloff */
    float           Attenuation0;     /* Constant attenuation */
    float           Attenuation1;     /* Linear attenuation */
    float           Attenuation2;     /* Quadratic attenuation */
    float           Theta;            /* Inner angle of spotlight cone */
    float           Phi;              /* Outer angle of spotlight cone */
} D3DLIGHT9;

typedef long D3DXIMAGE_FILEFORMAT;

typedef struct _D3DXIMAGE_INFO
{
    UINT                    Width;
    UINT                    Height;
    UINT                    Depth;
    UINT                    MipLevels;
    D3DFORMAT               Format;
    D3DRESOURCETYPE         ResourceType;
    D3DXIMAGE_FILEFORMAT    ImageFileFormat;
} D3DXIMAGE_INFO;

typedef struct _D3DBOX
{
    UINT                Left;
    UINT                Top;
    UINT                Right;
    UINT                Bottom;
    UINT                Front;
    UINT                Back;
} D3DBOX;

#define D3DXMATRIX D3DMATRIX
#define D3DXMATRIXA16 D3DMATRIX
#define D3DXVECTOR3 D3DVECTOR
#define FLOAT float
#define LPCVOID void*

typedef struct D3DXVECTOR2 {
    float x; float y;
} D3DXVECTOR2;

typedef struct D3DXVECTOR4 {
    float x; float y; float z; float w;
} D3DXVECTOR4;

typedef struct D3DXQUATERNION {
    float x; float y; float z; float w;
} D3DXQUATERNION;

typedef struct D3DXPLANE {
    float x; float y; float z; float w;
} D3DXPLANE;

typedef struct D3DXCOLOR {
    FLOAT r, g, b, a;
} D3DXCOLOR;

    
//===========================================================================
//
// General purpose utilities
//
//===========================================================================
#define D3DX_PI    ((FLOAT)  3.141592654)
#define D3DX_1BYPI ((FLOAT)  0.318309886)

#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0))
#define D3DXToDegree( radian ) ((radian) * (180.0 / D3DX_PI))




/* Resize Optional Parameters */
typedef struct _D3DPRESENT_PARAMETERS_
{
    UINT                BackBufferWidth;
    UINT                BackBufferHeight;
    D3DFORMAT           BackBufferFormat;
    UINT                BackBufferCount;

    D3DMULTISAMPLE_TYPE MultiSampleType;
    DWORD               MultiSampleQuality;

    D3DSWAPEFFECT       SwapEffect;
    HWND                hDeviceWindow;
    BOOL                Windowed;
    BOOL                EnableAutoDepthStencil;
    D3DFORMAT           AutoDepthStencilFormat;
    DWORD               Flags;

    /* FullScreen_RefreshRateInHz must be zero for Windowed mode */
    UINT                FullScreen_RefreshRateInHz;
    UINT                PresentationInterval;
} D3DPRESENT_PARAMETERS;

#undef INTERFACE
#define INTERFACE IDirect3D9

DECLARE_INTERFACE_(IDirect3D9, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirect3D9 methods ***/
    STDMETHOD(RegisterSoftwareDevice)(THIS_ void* pInitializeFunction) PURE;
    STDMETHOD_(UINT, GetAdapterCount)(THIS) PURE;
    STDMETHOD(GetAdapterIdentifier)(THIS_ UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier) PURE;
    STDMETHOD_(UINT, GetAdapterModeCount)(THIS_ UINT Adapter,D3DFORMAT Format) PURE;
    STDMETHOD(EnumAdapterModes)(THIS_ UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(GetAdapterDisplayMode)(THIS_ UINT Adapter,D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(CheckDeviceType)(THIS_ UINT iAdapter,D3DDEVTYPE DevType,D3DFORMAT DisplayFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed) PURE;
    STDMETHOD(CheckDeviceFormat)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat) PURE;
    STDMETHOD(CheckDeviceMultiSampleType)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels) PURE;
    STDMETHOD(CheckDepthStencilMatch)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat) PURE;
    STDMETHOD(CheckDeviceFormatConversion)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat) PURE;
    STDMETHOD(GetDeviceCaps)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps) PURE;
    STDMETHOD_(HMONITOR, GetAdapterMonitor)(THIS_ UINT Adapter) PURE;
    STDMETHOD(CreateDevice)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,void** ppReturnedDeviceInterface) PURE;
};

MY_DECLARE_INTERFACE(IDirect3D9)

typedef IDirect3D9 * LPDIRECT3D9;

#undef INTERFACE
#define INTERFACE IDirect3DDevice9

DECLARE_INTERFACE_(IDirect3DDevice9, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirect3DDevice9 methods ***/
    STDMETHOD(TestCooperativeLevel)(THIS) PURE;
    STDMETHOD_(UINT, GetAvailableTextureMem)(THIS) PURE;
    STDMETHOD(EvictManagedResources)(THIS) PURE;
    STDMETHOD(GetDirect3D)(THIS_ IDirect3D9** ppD3D9) PURE;
    STDMETHOD(GetDeviceCaps)(THIS_ D3DCAPS9* pCaps) PURE;
    STDMETHOD(GetDisplayMode)(THIS_ UINT iSwapChain,D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(GetCreationParameters)(THIS_ D3DDEVICE_CREATION_PARAMETERS *pParameters) PURE;
    STDMETHOD(SetCursorProperties)(THIS_ UINT XHotSpot,UINT YHotSpot,void* pCursorBitmap) PURE;
    STDMETHOD_(void, SetCursorPosition)(THIS_ int X,int Y,DWORD Flags) PURE;
    STDMETHOD_(BOOL, ShowCursor)(THIS_ BOOL bShow) PURE;
    STDMETHOD(CreateAdditionalSwapChain)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters,void** pSwapChain) PURE;
    STDMETHOD(GetSwapChain)(THIS_ UINT iSwapChain,void** pSwapChain) PURE;
    STDMETHOD_(UINT, GetNumberOfSwapChains)(THIS) PURE;
    STDMETHOD(Reset)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters) PURE;
    STDMETHOD(Present)(THIS_ CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion) PURE;
    STDMETHOD(GetBackBuffer)(THIS_ UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,void** ppBackBuffer) PURE;
    STDMETHOD(GetRasterStatus)(THIS_ UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus) PURE;
    STDMETHOD(SetDialogBoxMode)(THIS_ BOOL bEnableDialogs) PURE;
    STDMETHOD_(void, SetGammaRamp)(THIS_ UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp) PURE;
    STDMETHOD_(void, GetGammaRamp)(THIS_ UINT iSwapChain,D3DGAMMARAMP* pRamp) PURE;
    STDMETHOD(CreateTexture)(THIS_ UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,void** ppTexture,HANDLE* pSharedHandle) PURE;
    STDMETHOD(CreateVolumeTexture)(THIS_ UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,void** ppVolumeTexture,HANDLE* pSharedHandle) PURE;
    STDMETHOD(CreateCubeTexture)(THIS_ UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,void** ppCubeTexture,HANDLE* pSharedHandle) PURE;
    STDMETHOD(CreateVertexBuffer)(THIS_ UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,void** ppVertexBuffer,HANDLE* pSharedHandle) PURE;
    STDMETHOD(CreateIndexBuffer)(THIS_ UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,void** ppIndexBuffer,HANDLE* pSharedHandle) PURE;
    STDMETHOD(CreateRenderTarget)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,void** ppSurface,HANDLE* pSharedHandle) PURE;
    STDMETHOD(CreateDepthStencilSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,void** ppSurface,HANDLE* pSharedHandle) PURE;
    STDMETHOD(UpdateSurface)(THIS_ void* pSourceSurface,CONST RECT* pSourceRect,void* pDestinationSurface,CONST POINT* pDestPoint) PURE;
    STDMETHOD(UpdateTexture)(THIS_ void* pSourceTexture,void* pDestinationTexture) PURE;
    STDMETHOD(GetRenderTargetData)(THIS_ void* pRenderTarget,void* pDestSurface) PURE;
    STDMETHOD(GetFrontBufferData)(THIS_ UINT iSwapChain,void* pDestSurface) PURE;
    STDMETHOD(StretchRect)(THIS_ void* pSourceSurface,CONST RECT* pSourceRect,void* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter) PURE;
    STDMETHOD(ColorFill)(THIS_ void* pSurface,CONST RECT* pRect,D3DCOLOR color) PURE;
    STDMETHOD(CreateOffscreenPlainSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,void** ppSurface,HANDLE* pSharedHandle) PURE;
    STDMETHOD(SetRenderTarget)(THIS_ DWORD RenderTargetIndex,void* pRenderTarget) PURE;
    STDMETHOD(GetRenderTarget)(THIS_ DWORD RenderTargetIndex,void** ppRenderTarget) PURE;
    STDMETHOD(SetDepthStencilSurface)(THIS_ void* pNewZStencil) PURE;
    STDMETHOD(GetDepthStencilSurface)(THIS_ void** ppZStencilSurface) PURE;
    STDMETHOD(BeginScene)(THIS) PURE;
    STDMETHOD(EndScene)(THIS) PURE;
    STDMETHOD(Clear)(THIS_ DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil) PURE;
    STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix) PURE;
    STDMETHOD(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix) PURE;
    STDMETHOD(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE,CONST D3DMATRIX*) PURE;
    STDMETHOD(SetViewport)(THIS_ CONST D3DVIEWPORT9* pViewport) PURE;
    STDMETHOD(GetViewport)(THIS_ D3DVIEWPORT9* pViewport) PURE;
    STDMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL9* pMaterial) PURE;
    STDMETHOD(GetMaterial)(THIS_ D3DMATERIAL9* pMaterial) PURE;
    STDMETHOD(SetLight)(THIS_ DWORD Index,CONST D3DLIGHT9*) PURE;
    STDMETHOD(GetLight)(THIS_ DWORD Index,D3DLIGHT9*) PURE;
    STDMETHOD(LightEnable)(THIS_ DWORD Index,BOOL Enable) PURE;
    STDMETHOD(GetLightEnable)(THIS_ DWORD Index,BOOL* pEnable) PURE;
    STDMETHOD(SetClipPlane)(THIS_ DWORD Index,CONST float* pPlane) PURE;
    STDMETHOD(GetClipPlane)(THIS_ DWORD Index,float* pPlane) PURE;
    STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD Value) PURE;
    STDMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD* pValue) PURE;
    STDMETHOD(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE Type,void** ppSB) PURE;
    STDMETHOD(BeginStateBlock)(THIS) PURE;
    STDMETHOD(EndStateBlock)(THIS_ void** ppSB) PURE;
    STDMETHOD(SetClipStatus)(THIS_ CONST D3DCLIPSTATUS9* pClipStatus) PURE;
    STDMETHOD(GetClipStatus)(THIS_ D3DCLIPSTATUS9* pClipStatus) PURE;
    STDMETHOD(GetTexture)(THIS_ DWORD Stage,void** ppTexture) PURE;
    STDMETHOD(SetTexture)(THIS_ DWORD Stage,void* pTexture) PURE;
    STDMETHOD(GetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue) PURE;
    STDMETHOD(SetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value) PURE;
    STDMETHOD(GetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue) PURE;
    STDMETHOD(SetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value) PURE;
    STDMETHOD(ValidateDevice)(THIS_ DWORD* pNumPasses) PURE;
    STDMETHOD(SetPaletteEntries)(THIS_ UINT PaletteNumber,CONST PALETTEENTRY* pEntries) PURE;
    STDMETHOD(GetPaletteEntries)(THIS_ UINT PaletteNumber,PALETTEENTRY* pEntries) PURE;
    STDMETHOD(SetCurrentTexturePalette)(THIS_ UINT PaletteNumber) PURE;
    STDMETHOD(GetCurrentTexturePalette)(THIS_ UINT *PaletteNumber) PURE;
    STDMETHOD(SetScissorRect)(THIS_ CONST RECT* pRect) PURE;
    STDMETHOD(GetScissorRect)(THIS_ RECT* pRect) PURE;
    STDMETHOD(SetSoftwareVertexProcessing)(THIS_ BOOL bSoftware) PURE;
    STDMETHOD_(BOOL, GetSoftwareVertexProcessing)(THIS) PURE;
    STDMETHOD(SetNPatchMode)(THIS_ float nSegments) PURE;
    STDMETHOD_(float, GetNPatchMode)(THIS) PURE;
    STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount) PURE;
    STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount) PURE;
    STDMETHOD(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) PURE;
    STDMETHOD(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) PURE;
    STDMETHOD(ProcessVertices)(THIS_ UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,void* pDestBuffer,void* pVertexDecl,DWORD Flags) PURE;
    STDMETHOD(CreateVertexDeclaration)(THIS_ CONST D3DVERTEXELEMENT9* pVertexElements,void** ppDecl) PURE;
    STDMETHOD(SetVertexDeclaration)(THIS_ void* pDecl) PURE;
    STDMETHOD(GetVertexDeclaration)(THIS_ void** ppDecl) PURE;
    STDMETHOD(SetFVF)(THIS_ DWORD FVF) PURE;
    STDMETHOD(GetFVF)(THIS_ DWORD* pFVF) PURE;
    STDMETHOD(CreateVertexShader)(THIS_ CONST DWORD* pFunction,void** ppShader) PURE;
    STDMETHOD(SetVertexShader)(THIS_ void* pShader) PURE;
    STDMETHOD(GetVertexShader)(THIS_ void** ppShader) PURE;
    STDMETHOD(SetVertexShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) PURE;
    STDMETHOD(GetVertexShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount) PURE;
    STDMETHOD(SetVertexShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) PURE;
    STDMETHOD(GetVertexShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount) PURE;
    STDMETHOD(SetVertexShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) PURE;
    STDMETHOD(GetVertexShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount) PURE;
    STDMETHOD(SetStreamSource)(THIS_ UINT StreamNumber,void* pStreamData,UINT OffsetInBytes,UINT Stride) PURE;
    STDMETHOD(GetStreamSource)(THIS_ UINT StreamNumber,void** ppStreamData,UINT* OffsetInBytes,UINT* pStride) PURE;
    STDMETHOD(SetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT Divider) PURE;
    STDMETHOD(GetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT* Divider) PURE;
    STDMETHOD(SetIndices)(THIS_ void* pIndexData) PURE;
    STDMETHOD(GetIndices)(THIS_ void** ppIndexData) PURE;
    STDMETHOD(CreatePixelShader)(THIS_ CONST DWORD* pFunction,void** ppShader) PURE;
    STDMETHOD(SetPixelShader)(THIS_ void* pShader) PURE;
    STDMETHOD(GetPixelShader)(THIS_ void** ppShader) PURE;
    STDMETHOD(SetPixelShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) PURE;
    STDMETHOD(GetPixelShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount) PURE;
    STDMETHOD(SetPixelShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) PURE;
    STDMETHOD(GetPixelShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount) PURE;
    STDMETHOD(SetPixelShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) PURE;
    STDMETHOD(GetPixelShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount) PURE;
    STDMETHOD(DrawRectPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo) PURE;
    STDMETHOD(DrawTriPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo) PURE;
    STDMETHOD(DeletePatch)(THIS_ UINT Handle) PURE;
    STDMETHOD(CreateQuery)(THIS_ D3DQUERYTYPE Type,void** ppQuery) PURE;
};

MY_DECLARE_INTERFACE(IDirect3DDevice9)
typedef IDirect3DDevice9 *LPDIRECT3DDEVICE9;
#define D3DDev ((LPDIRECT3DDEVICE9)pd3ddev)

#undef INTERFACE
#define INTERFACE IDirect3DVertexBuffer9

DECLARE_INTERFACE_(IDirect3DVertexBuffer9, IDirect3DResource9)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirect3DResource9 methods ***/
    STDMETHOD(GetDevice)(THIS_ IDirect3DDevice9** ppDevice) PURE;
    STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) PURE;
    STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid,void* pData,DWORD* pSizeOfData) PURE;
    STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid) PURE;
    STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD PriorityNew) PURE;
    STDMETHOD_(DWORD, GetPriority)(THIS) PURE;
    STDMETHOD_(void, PreLoad)(THIS) PURE;
    STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS) PURE;
    STDMETHOD(Lock)(THIS_ UINT OffsetToLock,UINT SizeToLock,void** ppbData,DWORD Flags) PURE;
    STDMETHOD(Unlock)(THIS) PURE;
    STDMETHOD(GetDesc)(THIS_ D3DVERTEXBUFFER_DESC *pDesc) PURE;
};

MY_DECLARE_INTERFACE(IDirect3DVertexBuffer9)

typedef IDirect3DVertexBuffer9 *LPDIRECT3DVERTEXBUFFER9;


#undef INTERFACE
#define INTERFACE IDirect3DTexture9

DECLARE_INTERFACE_(IDirect3DTexture9, IDirect3DBaseTexture9)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirect3DBaseTexture9 methods ***/
    STDMETHOD(GetDevice)(THIS_ IDirect3DDevice9** ppDevice) PURE;
    STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) PURE;
    STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid,void* pData,DWORD* pSizeOfData) PURE;
    STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid) PURE;
    STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD PriorityNew) PURE;
    STDMETHOD_(DWORD, GetPriority)(THIS) PURE;
    STDMETHOD_(void, PreLoad)(THIS) PURE;
    STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS) PURE;
    STDMETHOD_(DWORD, SetLOD)(THIS_ DWORD LODNew) PURE;
    STDMETHOD_(DWORD, GetLOD)(THIS) PURE;
    STDMETHOD_(DWORD, GetLevelCount)(THIS) PURE;
    STDMETHOD(SetAutoGenFilterType)(THIS_ D3DTEXTUREFILTERTYPE FilterType) PURE;
    STDMETHOD_(D3DTEXTUREFILTERTYPE, GetAutoGenFilterType)(THIS) PURE;
    STDMETHOD_(void, GenerateMipSubLevels)(THIS) PURE;
    STDMETHOD(GetLevelDesc)(THIS_ UINT Level,D3DSURFACE_DESC *pDesc) PURE;
    STDMETHOD(GetSurfaceLevel)(THIS_ UINT Level,void** ppSurfaceLevel) PURE;//IDirect3DSurface9
    STDMETHOD(LockRect)(THIS_ UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) PURE;
    STDMETHOD(UnlockRect)(THIS_ UINT Level) PURE;
    STDMETHOD(AddDirtyRect)(THIS_ CONST RECT* pDirtyRect) PURE;
};
MY_DECLARE_INTERFACE(IDirect3DTexture9)
typedef IDirect3DTexture9 *LPDIRECT3DTEXTURE9;

#undef INTERFACE
#define INTERFACE IDirect3DSurface9

DECLARE_INTERFACE_(IDirect3DSurface9, IDirect3DResource9)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirect3DResource9 methods ***/
    STDMETHOD(GetDevice)(THIS_ IDirect3DDevice9** ppDevice) PURE;
    STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) PURE;
    STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid,void* pData,DWORD* pSizeOfData) PURE;
    STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid) PURE;
    STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD PriorityNew) PURE;
    STDMETHOD_(DWORD, GetPriority)(THIS) PURE;
    STDMETHOD_(void, PreLoad)(THIS) PURE;
    STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS) PURE;
    STDMETHOD(GetContainer)(THIS_ REFIID riid,void** ppContainer) PURE;
    STDMETHOD(GetDesc)(THIS_ D3DSURFACE_DESC *pDesc) PURE;
    STDMETHOD(LockRect)(THIS_ D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) PURE;
    STDMETHOD(UnlockRect)(THIS) PURE;
    STDMETHOD(GetDC)(THIS_ HDC *phdc) PURE;
    STDMETHOD(ReleaseDC)(THIS_ HDC hdc) PURE;
};
MY_DECLARE_INTERFACE(IDirect3DSurface9)
typedef IDirect3DSurface9 *LPDIRECT3DSURFACE9;

#undef INTERFACE
#define INTERFACE ID3DXBuffer

DECLARE_INTERFACE_(ID3DXBuffer, IUnknown)
{
    // IUnknown
    STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    // ID3DXBuffer
    STDMETHOD_(LPVOID, GetBufferPointer)(THIS) PURE;
    STDMETHOD_(DWORD, GetBufferSize)(THIS) PURE;
};
MY_DECLARE_INTERFACE(ID3DXBuffer)
typedef ID3DXBuffer *LPD3DXBUFFER;

#undef INTERFACE
#define INTERFACE ID3DXBaseMesh

DECLARE_INTERFACE_(ID3DXBaseMesh, IUnknown)
{
    // IUnknown
    STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    // ID3DXBaseMesh
    STDMETHOD(DrawSubset)(THIS_ DWORD AttribId) PURE;
    STDMETHOD_(DWORD, GetNumFaces)(THIS) PURE;
    STDMETHOD_(DWORD, GetNumVertices)(THIS) PURE;
    STDMETHOD_(DWORD, GetFVF)(THIS) PURE;
    STDMETHOD(GetDeclaration)(THIS_ D3DVERTEXELEMENT9 *) PURE;//Declaration[MAX_FVF_DECL_SIZE]
    STDMETHOD_(DWORD, GetNumBytesPerVertex)(THIS) PURE;
    STDMETHOD_(DWORD, GetOptions)(THIS) PURE;
    STDMETHOD(GetDevice)(THIS_ LPDIRECT3DDEVICE9* ppDevice) PURE;
    STDMETHOD(CloneMeshFVF)(THIS_ DWORD Options, 
                DWORD FVF, LPDIRECT3DDEVICE9 pD3DDevice, void* ppCloneMesh) PURE;//LPD3DXMESH
    STDMETHOD(CloneMesh)(THIS_ DWORD Options, 
                CONST D3DVERTEXELEMENT9 *pDeclaration, LPDIRECT3DDEVICE9 pD3DDevice, void* ppCloneMesh) PURE;//LPD3DXMESH
    STDMETHOD(GetVertexBuffer)(THIS_ LPDIRECT3DVERTEXBUFFER9* ppVB) PURE;
    STDMETHOD(GetIndexBuffer)(THIS_ LPDIRECT3DINDEXBUFFER9* ppIB) PURE;
    STDMETHOD(LockVertexBuffer)(THIS_ DWORD Flags, void** ppData) PURE;
    STDMETHOD(UnlockVertexBuffer)(THIS) PURE;
    STDMETHOD(LockIndexBuffer)(THIS_ DWORD Flags, void** ppData) PURE;
    STDMETHOD(UnlockIndexBuffer)(THIS) PURE;
    STDMETHOD(GetAttributeTable)(
                THIS_ D3DXATTRIBUTERANGE *pAttribTable, DWORD* pAttribTableSize) PURE;

    STDMETHOD(ConvertPointRepsToAdjacency)(THIS_ CONST DWORD* pPRep, DWORD* pAdjacency) PURE;
    STDMETHOD(ConvertAdjacencyToPointReps)(THIS_ CONST DWORD* pAdjacency, DWORD* pPRep) PURE;
    STDMETHOD(GenerateAdjacency)(THIS_ FLOAT Epsilon, DWORD* pAdjacency) PURE;

    STDMETHOD(UpdateSemantics)(THIS_ D3DVERTEXELEMENT9 *) PURE;//Declaration[MAX_FVF_DECL_SIZE]
};
MY_DECLARE_INTERFACE(ID3DXBaseMesh)
typedef ID3DXBaseMesh *LPD3DXBASEMESH;

#undef INTERFACE
#define INTERFACE ID3DXMesh

DECLARE_INTERFACE_(ID3DXMesh, ID3DXBaseMesh)
{
    // IUnknown
    STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    // ID3DXBaseMesh
    STDMETHOD(DrawSubset)(THIS_ DWORD AttribId) PURE;
    STDMETHOD_(DWORD, GetNumFaces)(THIS) PURE;
    STDMETHOD_(DWORD, GetNumVertices)(THIS) PURE;
    STDMETHOD_(DWORD, GetFVF)(THIS) PURE;
    STDMETHOD(GetDeclaration)(THIS_ D3DVERTEXELEMENT9 *) PURE;//Declaration[MAX_FVF_DECL_SIZE]
    STDMETHOD_(DWORD, GetNumBytesPerVertex)(THIS) PURE;
    STDMETHOD_(DWORD, GetOptions)(THIS) PURE;
    STDMETHOD(GetDevice)(THIS_ LPDIRECT3DDEVICE9* ppDevice) PURE;
    STDMETHOD(CloneMeshFVF)(THIS_ DWORD Options, 
                DWORD FVF, LPDIRECT3DDEVICE9 pD3DDevice, void* ppCloneMesh) PURE;//LPD3DXMESH
    STDMETHOD(CloneMesh)(THIS_ DWORD Options, 
                CONST D3DVERTEXELEMENT9 *pDeclaration, LPDIRECT3DDEVICE9 pD3DDevice, void* ppCloneMesh) PURE;//LPD3DXMESH
    STDMETHOD(GetVertexBuffer)(THIS_ LPDIRECT3DVERTEXBUFFER9* ppVB) PURE;
    STDMETHOD(GetIndexBuffer)(THIS_ LPDIRECT3DINDEXBUFFER9* ppIB) PURE;
    STDMETHOD(LockVertexBuffer)(THIS_ DWORD Flags, void** ppData) PURE;
    STDMETHOD(UnlockVertexBuffer)(THIS) PURE;
    STDMETHOD(LockIndexBuffer)(THIS_ DWORD Flags, void** ppData) PURE;
    STDMETHOD(UnlockIndexBuffer)(THIS) PURE;
    STDMETHOD(GetAttributeTable)(
                THIS_ D3DXATTRIBUTERANGE *pAttribTable, DWORD* pAttribTableSize) PURE;

    STDMETHOD(ConvertPointRepsToAdjacency)(THIS_ CONST DWORD* pPRep, DWORD* pAdjacency) PURE;
    STDMETHOD(ConvertAdjacencyToPointReps)(THIS_ CONST DWORD* pAdjacency, DWORD* pPRep) PURE;
    STDMETHOD(GenerateAdjacency)(THIS_ FLOAT Epsilon, DWORD* pAdjacency) PURE;

    STDMETHOD(UpdateSemantics)(THIS_ D3DVERTEXELEMENT9 *) PURE;//Declaration[MAX_FVF_DECL_SIZE]

    // ID3DXMesh
    STDMETHOD(LockAttributeBuffer)(THIS_ DWORD Flags, DWORD** ppData) PURE;
    STDMETHOD(UnlockAttributeBuffer)(THIS) PURE;
    STDMETHOD(Optimize)(THIS_ DWORD Flags, CONST DWORD* pAdjacencyIn, DWORD* pAdjacencyOut, 
                     DWORD* pFaceRemap, LPD3DXBUFFER *ppVertexRemap,  
                     void* ppOptMesh) PURE;//LPD3DXMESH
    STDMETHOD(OptimizeInplace)(THIS_ DWORD Flags, CONST DWORD* pAdjacencyIn, DWORD* pAdjacencyOut, 
                     DWORD* pFaceRemap, LPD3DXBUFFER *ppVertexRemap) PURE;
    STDMETHOD(SetAttributeTable)(THIS_ CONST D3DXATTRIBUTERANGE *pAttribTable, DWORD cAttribTableSize) PURE;
};
MY_DECLARE_INTERFACE(ID3DXMesh)
typedef ID3DXMesh *LPD3DXMESH;


typedef struct _D3DXMATERIAL
{
    D3DMATERIAL9  MatD3D;
    LPSTR         pTextureFilename;
} D3DXMATERIAL;
typedef D3DXMATERIAL *LPD3DXMATERIAL;

typedef LPCSTR D3DXHANDLE;
typedef D3DXHANDLE *LPD3DXHANDLE;

typedef struct _D3DXEFFECT_DESC
{
    LPCSTR Creator;                     // Creator string
    UINT Parameters;                    // Number of parameters
    UINT Techniques;                    // Number of techniques
    UINT Functions;                     // Number of function entrypoints
} D3DXEFFECT_DESC;

typedef DWORD D3DXPARAMETER_CLASS;
typedef DWORD D3DXPARAMETER_TYPE;

typedef struct _D3DXPARAMETER_DESC
{
    LPCSTR Name;                        // Parameter name
    LPCSTR Semantic;                    // Parameter semantic
    D3DXPARAMETER_CLASS Class;          // Class
    D3DXPARAMETER_TYPE Type;            // Component type
    UINT Rows;                          // Number of rows
    UINT Columns;                       // Number of columns
    UINT Elements;                      // Number of array elements
    UINT Annotations;                   // Number of annotations
    UINT StructMembers;                 // Number of structure member sub-parameters
    DWORD Flags;                        // D3DX_PARAMETER_* flags
    UINT Bytes;                         // Parameter size, in bytes
} D3DXPARAMETER_DESC;

typedef struct _D3DXTECHNIQUE_DESC
{
    LPCSTR Name;                        // Technique name
    UINT Passes;                        // Number of passes
    UINT Annotations;                   // Number of annotations
} D3DXTECHNIQUE_DESC;

typedef struct _D3DXPASS_DESC
{
    LPCSTR Name;                        // Pass name
    UINT Annotations;                   // Number of annotations
    CONST DWORD *pVertexShaderFunction; // Vertex shader function
    CONST DWORD *pPixelShaderFunction;  // Pixel shader function
} D3DXPASS_DESC;

typedef struct _D3DXFUNCTION_DESC
{
    LPCSTR Name;                        // Function name
    UINT Annotations;                   // Number of annotations
} D3DXFUNCTION_DESC;

#undef INTERFACE
#define INTERFACE IDirect3DVertexShader9

DECLARE_INTERFACE_(IDirect3DVertexShader9, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirect3DVertexShader9 methods ***/
    STDMETHOD(GetDevice)(THIS_ IDirect3DDevice9** ppDevice) PURE;
    STDMETHOD(GetFunction)(THIS_ void*,UINT* pSizeOfData) PURE;
};
MY_DECLARE_INTERFACE(IDirect3DVertexShader9)    
typedef IDirect3DVertexShader9 *LPDIRECT3DVERTEXSHADER9;

#undef INTERFACE
#define INTERFACE IDirect3DPixelShader9

DECLARE_INTERFACE_(IDirect3DPixelShader9, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirect3DPixelShader9 methods ***/
    STDMETHOD(GetDevice)(THIS_ IDirect3DDevice9** ppDevice) PURE;
    STDMETHOD(GetFunction)(THIS_ void*,UINT* pSizeOfData) PURE;
};
MY_DECLARE_INTERFACE(IDirect3DPixelShader9)
typedef IDirect3DPixelShader9 *LPDIRECT3DPIXELSHADER9;

#undef INTERFACE
#define INTERFACE ID3DXEffect

DECLARE_INTERFACE_(ID3DXEffect, ID3DXBaseEffect)
{
    // ID3DXBaseEffect
    STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    // Descs
    STDMETHOD(GetDesc)(THIS_ D3DXEFFECT_DESC* pDesc) PURE;
    STDMETHOD(GetParameterDesc)(THIS_ D3DXHANDLE hParameter, D3DXPARAMETER_DESC* pDesc) PURE;
    STDMETHOD(GetTechniqueDesc)(THIS_ D3DXHANDLE hTechnique, D3DXTECHNIQUE_DESC* pDesc) PURE;
    STDMETHOD(GetPassDesc)(THIS_ D3DXHANDLE hPass, D3DXPASS_DESC* pDesc) PURE;
    STDMETHOD(GetFunctionDesc)(THIS_ D3DXHANDLE hShader, D3DXFUNCTION_DESC* pDesc) PURE;

    // Handle operations
    STDMETHOD_(D3DXHANDLE, GetParameter)(THIS_ D3DXHANDLE hParameter, UINT Index) PURE;
    STDMETHOD_(D3DXHANDLE, GetParameterByName)(THIS_ D3DXHANDLE hParameter, LPCSTR pName) PURE;
    STDMETHOD_(D3DXHANDLE, GetParameterBySemantic)(THIS_ D3DXHANDLE hParameter, LPCSTR pSemantic) PURE;
    STDMETHOD_(D3DXHANDLE, GetParameterElement)(THIS_ D3DXHANDLE hParameter, UINT Index) PURE;
    STDMETHOD_(D3DXHANDLE, GetTechnique)(THIS_ UINT Index) PURE;
    STDMETHOD_(D3DXHANDLE, GetTechniqueByName)(THIS_ LPCSTR pName) PURE;
    STDMETHOD_(D3DXHANDLE, GetPass)(THIS_ D3DXHANDLE hTechnique, UINT Index) PURE;
    STDMETHOD_(D3DXHANDLE, GetPassByName)(THIS_ D3DXHANDLE hTechnique, LPCSTR pName) PURE;
    STDMETHOD_(D3DXHANDLE, GetFunction)(THIS_ UINT Index) PURE;
    STDMETHOD_(D3DXHANDLE, GetFunctionByName)(THIS_ LPCSTR pName) PURE;
    STDMETHOD_(D3DXHANDLE, GetAnnotation)(THIS_ D3DXHANDLE hObject, UINT Index) PURE;
    STDMETHOD_(D3DXHANDLE, GetAnnotationByName)(THIS_ D3DXHANDLE hObject, LPCSTR pName) PURE;

    // Get/Set Parameters
    STDMETHOD(SetValue)(THIS_ D3DXHANDLE hParameter, LPCVOID pData, UINT Bytes) PURE;
    STDMETHOD(GetValue)(THIS_ D3DXHANDLE hParameter, LPVOID pData, UINT Bytes) PURE;
    STDMETHOD(SetBool)(THIS_ D3DXHANDLE hParameter, BOOL b) PURE;
    STDMETHOD(GetBool)(THIS_ D3DXHANDLE hParameter, BOOL* pb) PURE;
    STDMETHOD(SetBoolArray)(THIS_ D3DXHANDLE hParameter, CONST BOOL* pb, UINT Count) PURE;
    STDMETHOD(GetBoolArray)(THIS_ D3DXHANDLE hParameter, BOOL* pb, UINT Count) PURE;
    STDMETHOD(SetInt)(THIS_ D3DXHANDLE hParameter, INT n) PURE;
    STDMETHOD(GetInt)(THIS_ D3DXHANDLE hParameter, INT* pn) PURE;
    STDMETHOD(SetIntArray)(THIS_ D3DXHANDLE hParameter, CONST INT* pn, UINT Count) PURE;
    STDMETHOD(GetIntArray)(THIS_ D3DXHANDLE hParameter, INT* pn, UINT Count) PURE;
    STDMETHOD(SetFloat)(THIS_ D3DXHANDLE hParameter, FLOAT f) PURE;
    STDMETHOD(GetFloat)(THIS_ D3DXHANDLE hParameter, FLOAT* pf) PURE;
    STDMETHOD(SetFloatArray)(THIS_ D3DXHANDLE hParameter, CONST FLOAT* pf, UINT Count) PURE;
    STDMETHOD(GetFloatArray)(THIS_ D3DXHANDLE hParameter, FLOAT* pf, UINT Count) PURE;
    STDMETHOD(SetVector)(THIS_ D3DXHANDLE hParameter, CONST D3DXVECTOR4* pVector) PURE;
    STDMETHOD(GetVector)(THIS_ D3DXHANDLE hParameter, D3DXVECTOR4* pVector) PURE;
    STDMETHOD(SetVectorArray)(THIS_ D3DXHANDLE hParameter, CONST D3DXVECTOR4* pVector, UINT Count) PURE;
    STDMETHOD(GetVectorArray)(THIS_ D3DXHANDLE hParameter, D3DXVECTOR4* pVector, UINT Count) PURE;
    STDMETHOD(SetMatrix)(THIS_ D3DXHANDLE hParameter, CONST D3DXMATRIX* pMatrix) PURE;
    STDMETHOD(GetMatrix)(THIS_ D3DXHANDLE hParameter, D3DXMATRIX* pMatrix) PURE;
    STDMETHOD(SetMatrixArray)(THIS_ D3DXHANDLE hParameter, CONST D3DXMATRIX* pMatrix, UINT Count) PURE;
    STDMETHOD(GetMatrixArray)(THIS_ D3DXHANDLE hParameter, D3DXMATRIX* pMatrix, UINT Count) PURE;
    STDMETHOD(SetMatrixPointerArray)(THIS_ D3DXHANDLE hParameter, CONST D3DXMATRIX** ppMatrix, UINT Count) PURE;
    STDMETHOD(GetMatrixPointerArray)(THIS_ D3DXHANDLE hParameter, D3DXMATRIX** ppMatrix, UINT Count) PURE;
    STDMETHOD(SetMatrixTranspose)(THIS_ D3DXHANDLE hParameter, CONST D3DXMATRIX* pMatrix) PURE;
    STDMETHOD(GetMatrixTranspose)(THIS_ D3DXHANDLE hParameter, D3DXMATRIX* pMatrix) PURE;
    STDMETHOD(SetMatrixTransposeArray)(THIS_ D3DXHANDLE hParameter, CONST D3DXMATRIX* pMatrix, UINT Count) PURE;
    STDMETHOD(GetMatrixTransposeArray)(THIS_ D3DXHANDLE hParameter, D3DXMATRIX* pMatrix, UINT Count) PURE;
    STDMETHOD(SetMatrixTransposePointerArray)(THIS_ D3DXHANDLE hParameter, CONST D3DXMATRIX** ppMatrix, UINT Count) PURE;
    STDMETHOD(GetMatrixTransposePointerArray)(THIS_ D3DXHANDLE hParameter, D3DXMATRIX** ppMatrix, UINT Count) PURE;
    STDMETHOD(SetString)(THIS_ D3DXHANDLE hParameter, LPCSTR pString) PURE;
    STDMETHOD(GetString)(THIS_ D3DXHANDLE hParameter, LPCSTR* ppString) PURE;
    STDMETHOD(SetTexture)(THIS_ D3DXHANDLE hParameter, LPVOID /*LPDIRECT3DBASETEXTURE9*/ pTexture) PURE;
    STDMETHOD(GetTexture)(THIS_ D3DXHANDLE hParameter, LPVOID /*LPDIRECT3DBASETEXTURE9*/ *ppTexture) PURE;
    STDMETHOD(GetPixelShader)(THIS_ D3DXHANDLE hParameter, LPDIRECT3DPIXELSHADER9 *ppPShader) PURE;
    STDMETHOD(GetVertexShader)(THIS_ D3DXHANDLE hParameter, LPDIRECT3DVERTEXSHADER9 *ppVShader) PURE;

	//Set Range of an Array to pass to device
	//Usefull for sending only a subrange of an array down to the device
	STDMETHOD(SetArrayRange)(THIS_ D3DXHANDLE hParameter, UINT uStart, UINT uEnd) PURE; 
	// ID3DXBaseEffect
    
    
    // Pool
    STDMETHOD(GetPool)(THIS_ LPVOID /*LPD3DXEFFECTPOOL*/ *ppPool) PURE;

    // Selecting and setting a technique
    STDMETHOD(SetTechnique)(THIS_ D3DXHANDLE hTechnique) PURE;
    STDMETHOD_(D3DXHANDLE, GetCurrentTechnique)(THIS) PURE;
    STDMETHOD(ValidateTechnique)(THIS_ D3DXHANDLE hTechnique) PURE;
    STDMETHOD(FindNextValidTechnique)(THIS_ D3DXHANDLE hTechnique, D3DXHANDLE *pTechnique) PURE;
    STDMETHOD_(BOOL, IsParameterUsed)(THIS_ D3DXHANDLE hParameter, D3DXHANDLE hTechnique) PURE;

    // Using current technique
    // Begin           starts active technique
    // BeginPass       begins a pass
    // CommitChanges   updates changes to any set calls in the pass. This should be called before
    //                 any DrawPrimitive call to d3d
    // EndPass         ends a pass
    // End             ends active technique
    STDMETHOD(Begin)(THIS_ UINT *pPasses, DWORD Flags) PURE;
    STDMETHOD(BeginPass)(THIS_ UINT Pass) PURE;
    STDMETHOD(CommitChanges)(THIS) PURE;
    STDMETHOD(EndPass)(THIS) PURE;
    STDMETHOD(End)(THIS) PURE;

    // Managing D3D Device
    STDMETHOD(GetDevice)(THIS_ LPDIRECT3DDEVICE9* ppDevice) PURE;
    STDMETHOD(OnLostDevice)(THIS) PURE;
    STDMETHOD(OnResetDevice)(THIS) PURE;

    // Logging device calls
    STDMETHOD(SetStateManager)(THIS_ LPVOID /*LPD3DXEFFECTSTATEMANAGER*/ pManager) PURE;
    STDMETHOD(GetStateManager)(THIS_ LPVOID /*LPD3DXEFFECTSTATEMANAGER*/ *ppManager) PURE;

    // Parameter blocks
    STDMETHOD(BeginParameterBlock)(THIS) PURE;
    STDMETHOD_(D3DXHANDLE, EndParameterBlock)(THIS) PURE;
    STDMETHOD(ApplyParameterBlock)(THIS_ D3DXHANDLE hParameterBlock) PURE;
    STDMETHOD(DeleteParameterBlock)(THIS_ D3DXHANDLE hParameterBlock) PURE;

    // Cloning
    STDMETHOD(CloneEffect)(THIS_ LPDIRECT3DDEVICE9 pDevice, LPVOID /*LPD3DXEFFECT*/ *ppEffect) PURE;
    
    // Fast path for setting variables directly in ID3DXEffect
    STDMETHOD(SetRawValue)(THIS_ D3DXHANDLE hParameter, LPCVOID pData, UINT ByteOffset, UINT Bytes) PURE;
};
MY_DECLARE_INTERFACE(ID3DXEffect)
typedef ID3DXEffect *LPD3DXEFFECT;


typedef struct _D3DXATTRIBUTEWEIGHTS
{
    FLOAT Position;
    FLOAT Boundary;
    FLOAT Normal;
    FLOAT Diffuse;
    FLOAT Specular;
    FLOAT Texcoord[8];
    FLOAT Tangent;
    FLOAT Binormal;
} D3DXATTRIBUTEWEIGHTS;
typedef D3DXATTRIBUTEWEIGHTS *LPD3DXATTRIBUTEWEIGHTS;

#define D3DSWAPEFFECT_DISCARD 1
#define D3DFMT_UNKNOWN 0
#define D3DADAPTER_DEFAULT 0
#define D3DDEVTYPE_HAL 1
#define D3DCREATE_SOFTWARE_VERTEXPROCESSING 0x20
#define D3DCLEAR_TARGET 1

#define D3DPOOL_DEFAULT         0

#define D3DPT_TRIANGLELIST      4
#define D3DPT_TRIANGLESTRIP     5
#define D3DPT_TRIANGLEFAN       6

#define D3DCULL_NONE  1
#define D3DCULL_CW    2
#define D3DCULL_CCW   3

#define D3DRS_ZENABLE             7    /* D3DZBUFFERTYPE (or TRUE/FALSE for legacy) */
#define D3DRS_FILLMODE            8    /* D3DFILLMODE */
#define D3DRS_SHADEMODE           9    /* D3DSHADEMODE */
#define D3DRS_ZWRITEENABLE        14   /* TRUE to enable z writes */
#define D3DRS_ALPHATESTENABLE     15   /* TRUE to enable alpha tests */
#define D3DRS_LASTPIXEL           16   /* TRUE for last-pixel on lines */
#define D3DRS_SRCBLEND            19   /* D3DBLEND */
#define D3DRS_DESTBLEND           20   /* D3DBLEND */
#define D3DRS_CULLMODE            22   /* D3DCULL */
#define D3DRS_ZFUNC               23   /* D3DCMPFUNC */
#define D3DRS_ALPHAREF            24   /* D3DFIXED */
#define D3DRS_ALPHAFUNC           25   /* D3DCMPFUNC */
#define D3DRS_DITHERENABLE        26   /* TRUE to enable dithering */
#define D3DRS_ALPHABLENDENABLE    27   /* TRUE to enable alpha blending */
#define D3DRS_FOGENABLE           28   /* TRUE to enable fog blending */
#define D3DRS_SPECULARENABLE      29   /* TRUE to enable specular */
#define D3DRS_FOGCOLOR            34   /* D3DCOLOR */
#define D3DRS_FOGTABLEMODE        35   /* D3DFOGMODE */
#define D3DRS_FOGSTART            36   /* Fog start (for both vertex and pixel fog) */
#define D3DRS_FOGEND              37   /* Fog end      */
#define D3DRS_FOGDENSITY          38   /* Fog density  */
#define D3DRS_RANGEFOGENABLE      48   /* Enables range-based fog */
#define D3DRS_STENCILENABLE       52   /* BOOL enable/disable stenciling */
#define D3DRS_STENCILFAIL         53   /* D3DSTENCILOP to do if stencil test fails */
#define D3DRS_STENCILZFAIL        54   /* D3DSTENCILOP to do if stencil test passes and Z test fails */
#define D3DRS_STENCILPASS         55   /* D3DSTENCILOP to do if both stencil and Z tests pass */
#define D3DRS_STENCILFUNC         56   /* D3DCMPFUNC fn.  Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
#define D3DRS_STENCILREF          57   /* Reference value used in stencil test */
#define D3DRS_STENCILMASK         58   /* Mask value used in stencil test */
#define D3DRS_STENCILWRITEMASK    59   /* Write mask applied to values written to stencil buffer */
#define D3DRS_TEXTUREFACTOR       60   /* D3DCOLOR used for multi-texture blend */
#define D3DRS_WRAP0               128  /* wrap for 1st texture coord. set */
#define D3DRS_WRAP1               129  /* wrap for 2nd texture coord. set */
#define D3DRS_WRAP2               130  /* wrap for 3rd texture coord. set */
#define D3DRS_WRAP3               131  /* wrap for 4th texture coord. set */
#define D3DRS_WRAP4               132  /* wrap for 5th texture coord. set */
#define D3DRS_WRAP5               133  /* wrap for 6th texture coord. set */
#define D3DRS_WRAP6               134  /* wrap for 7th texture coord. set */
#define D3DRS_WRAP7               135  /* wrap for 8th texture coord. set */
#define D3DRS_CLIPPING            136
#define D3DRS_LIGHTING            137
#define D3DRS_AMBIENT             139
#define D3DRS_FOGVERTEXMODE       140
#define D3DRS_COLORVERTEX         141
#define D3DRS_LOCALVIEWER         142
#define D3DRS_NORMALIZENORMALS    143
#define D3DRS_DIFFUSEMATERIALSOURCE     145
#define D3DRS_SPECULARMATERIALSOURCE    146
#define D3DRS_AMBIENTMATERIALSOURCE     147
#define D3DRS_EMISSIVEMATERIALSOURCE    148
#define D3DRS_VERTEXBLEND         151
#define D3DRS_CLIPPLANEENABLE     152
#define D3DRS_POINTSIZE           154   /* float point size */
#define D3DRS_POINTSIZE_MIN       155   /* float point size min threshold */
#define D3DRS_POINTSPRITEENABLE   156   /* BOOL point texture coord control */
#define D3DRS_POINTSCALEENABLE    157   /* BOOL point size scale enable */
#define D3DRS_POINTSCALE_A        158   /* float point attenuation A value */
#define D3DRS_POINTSCALE_B        159   /* float point attenuation B value */
#define D3DRS_POINTSCALE_C        160   /* float point attenuation C value */
#define D3DRS_MULTISAMPLEANTIALIAS161  // BOOL - set to do FSAA with multisample buffer
#define D3DRS_MULTISAMPLEMASK     162  // DWORD - per-sample enable/disable
#define D3DRS_PATCHEDGESTYLE      163  // Sets whether patch edges will use float style tessellation
#define D3DRS_DEBUGMONITORTOKEN   165  // DEBUG ONLY - token to debug monitor
#define D3DRS_POINTSIZE_MAX       166   /* float point size max threshold */
#define D3DRS_INDEXEDVERTEXBLENDENABLE  167
#define D3DRS_COLORWRITEENABLE    168  // per-channel write enable
#define D3DRS_TWEENFACTOR         170   // float tween factor
#define D3DRS_BLENDOP             171   // D3DBLENDOP setting
#define D3DRS_POSITIONDEGREE      172   // NPatch position interpolation degree. D3DDEGREE_LINEAR or D3DDEGREE_CUBIC (default)
#define D3DRS_NORMALDEGREE        173   // NPatch normal interpolation degree. D3DDEGREE_LINEAR (default) or D3DDEGREE_QUADRATIC
#define D3DRS_SCISSORTESTENABLE   174
#define D3DRS_SLOPESCALEDEPTHBIAS 175
#define D3DRS_ANTIALIASEDLINEENABLE     176
#define D3DRS_MINTESSELLATIONLEVEL178
#define D3DRS_MAXTESSELLATIONLEVEL179
#define D3DRS_ADAPTIVETESS_X      180
#define D3DRS_ADAPTIVETESS_Y      181
#define D3DRS_ADAPTIVETESS_Z      182
#define D3DRS_ADAPTIVETESS_W      183
#define D3DRS_ENABLEADAPTIVETESSELLATION 184
#define D3DRS_TWOSIDEDSTENCILMODE 185   /* BOOL enable/disable 2 sided stenciling */
#define D3DRS_CCW_STENCILFAIL     186   /* D3DSTENCILOP to do if ccw stencil test fails */
#define D3DRS_CCW_STENCILZFAIL    187   /* D3DSTENCILOP to do if ccw stencil test passes and Z test fails */
#define D3DRS_CCW_STENCILPASS     188   /* D3DSTENCILOP to do if both ccw stencil and Z tests pass */
#define D3DRS_CCW_STENCILFUNC     189   /* D3DCMPFUNC fn.  ccw Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
#define D3DRS_COLORWRITEENABLE1   190   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
#define D3DRS_COLORWRITEENABLE2   191   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
#define D3DRS_COLORWRITEENABLE3   192   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
#define D3DRS_BLENDFACTOR         193   /* D3DCOLOR used for a constant blend factor during alpha blending for devices that support D3DPBLENDCAPS_BLENDFACTOR */
#define D3DRS_SRGBWRITEENABLE     194   /* Enable rendertarget writes to be DE-linearized to SRGB (for formats that expose D3DUSAGE_QUERY_SRGBWRITE) */
#define D3DRS_DEPTHBIAS           195
#define D3DRS_WRAP8               198   /* Additional wrap states for vs_3_0+ attributes with D3DDECLUSAGE_TEXCOORD */
#define D3DRS_WRAP9               199
#define D3DRS_WRAP10              200
#define D3DRS_WRAP11              201
#define D3DRS_WRAP12              202
#define D3DRS_WRAP13              203
#define D3DRS_WRAP14              204
#define D3DRS_WRAP15              205
#define D3DRS_SEPARATEALPHABLENDENABLE  206  /* TRUE to enable a separate blending function for the alpha channel */
#define D3DRS_SRCBLENDALPHA       207  /* SRC blend factor for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
#define D3DRS_DESTBLENDALPHA      208  /* DST blend factor for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
#define D3DRS_BLENDOPALPHA        209  /* Blending operation for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */

// Flexible vertex format bits
#define D3DFVF_POSITION_MASK    0x400E
#define D3DFVF_XYZ              0x002
#define D3DFVF_XYZRHW           0x004
#define D3DFVF_XYZB1            0x006
#define D3DFVF_XYZB2            0x008
#define D3DFVF_XYZB3            0x00a
#define D3DFVF_XYZB4            0x00c
#define D3DFVF_XYZB5            0x00e
#define D3DFVF_XYZW             0x4002

#define D3DFVF_NORMAL           0x010
#define D3DFVF_PSIZE            0x020
#define D3DFVF_DIFFUSE          0x040
#define D3DFVF_SPECULAR         0x080

#define D3DFVF_TEXCOUNT_MASK    0xf00
#define D3DFVF_TEXCOUNT_SHIFT   8
#define D3DFVF_TEX0             0x000
#define D3DFVF_TEX1             0x100
#define D3DFVF_TEX2             0x200
#define D3DFVF_TEX3             0x300
#define D3DFVF_TEX4             0x400
#define D3DFVF_TEX5             0x500
#define D3DFVF_TEX6             0x600
#define D3DFVF_TEX7             0x700
#define D3DFVF_TEX8             0x800

#define D3DFVF_TEXTUREFORMAT1 3 // one floating point value
#define D3DFVF_TEXTUREFORMAT2 0 // two floating point values
#define D3DFVF_TEXTUREFORMAT3 1 // three floating point values
#define D3DFVF_TEXTUREFORMAT4 2 // four floating point values

#define D3DFVF_TEXCOORDSIZE1(CoordIndex) (D3DFVF_TEXTUREFORMAT1 << (CoordIndex*2 + 16))
#define D3DFVF_TEXCOORDSIZE2(CoordIndex) (D3DFVF_TEXTUREFORMAT2)
#define D3DFVF_TEXCOORDSIZE3(CoordIndex) (D3DFVF_TEXTUREFORMAT3 << (CoordIndex*2 + 16))
#define D3DFVF_TEXCOORDSIZE4(CoordIndex) (D3DFVF_TEXTUREFORMAT4 << (CoordIndex*2 + 16)) 

#define D3DTSS_COLOROP        1
#define D3DTSS_COLORARG1      2
#define D3DTSS_COLORARG2      3
#define D3DTSS_ALPHAOP        4
#define D3DTSS_ALPHAARG1      5 /* D3DTA_* (texture arg) */
#define D3DTSS_ALPHAARG2      6 /* D3DTA_* (texture arg) */
#define D3DTSS_BUMPENVMAT00   7 /* float (bump mapping matrix) */
#define D3DTSS_BUMPENVMAT01   8 /* float (bump mapping matrix) */
#define D3DTSS_BUMPENVMAT10   9 /* float (bump mapping matrix) */
#define D3DTSS_BUMPENVMAT11  10 /* float (bump mapping matrix) */
#define D3DTSS_TEXCOORDINDEX 11 /* identifies which set of texture coordinates index this texture */
#define D3DTSS_BUMPENVLSCALE 22 /* float scale for bump map luminance */
#define D3DTSS_BUMPENVLOFFSET23 /* float offset for bump map luminance */
#define D3DTSS_TEXTURETRANSFORMFLAGS24 /* D3DTEXTURETRANSFORMFLAGS controls texture transform */
#define D3DTSS_COLORARG0     26 /* D3DTA_* third arg for triadic ops */
#define D3DTSS_ALPHAARG0     27 /* D3DTA_* third arg for triadic ops */
#define D3DTSS_RESULTARG     28 /* D3DTA_* arg for result (CURRENT or TEMP) */
#define D3DTSS_CONSTANT      32 /* Per-stage constant D3DTA_CONSTANT */

#define D3DTOP_DISABLE        1
#define D3DTOP_SELECTARG1          2      // the default
#define D3DTOP_SELECTARG2          3
#define D3DTOP_MODULATE            4      // multiply args together
#define D3DTOP_MODULATE2X          5      // multiply and  1 bit
#define D3DTOP_MODULATE4X          6      // multiply and  2 bits
#define D3DTOP_ADD                  7   // add arguments together
#define D3DTOP_ADDSIGNED            8   // add with -0.5 bias
#define D3DTOP_ADDSIGNED2X          9   // as above but left  1 bit
#define D3DTOP_SUBTRACT            10,   // Arg1 - Arg2 with no saturation
#define D3DTOP_ADDSMOOTH           11,   // add 2 args subtract product
#define D3DTOP_BLENDDIFFUSEALPHA   12 // iterated alpha
#define D3DTOP_BLENDTEXTUREALPHA   13 // texture alpha
#define D3DTOP_BLENDFACTORALPHA    14 // alpha from D3DRS_TEXTUREFACTOR
#define D3DTOP_BLENDTEXTUREALPHAPM 15 // texture alpha
#define D3DTOP_BLENDCURRENTALPHA   16 // by alpha of current color
#define D3DTOP_PREMODULATE           17     // modulate with next texture before use
#define D3DTOP_MODULATEALPHA_ADDCOLOR18     // Arg1.RGB + Arg1.A*Arg2.RGB
#define D3DTOP_MODULATECOLOR_ADDALPHA19     // Arg1.RGB*Arg2.RGB + Arg1.A
#define D3DTOP_MODULATEINVALPHA_ADDCOLOR20  // (1-Arg1.A)*Arg2.RGB + Arg1.RGB
#define D3DTOP_MODULATEINVCOLOR_ADDALPHA21  // (1-Arg1.RGB)*Arg2.RGB + Arg1.A
#define D3DTOP_BUMPENVMAP          22 // per pixel env map perturbation
#define D3DTOP_BUMPENVMAPLUMINANCE 23 // with luminance channel
#define D3DTOP_DOTPRODUCT3         24
#define D3DTOP_MULTIPLYADD         25 // Arg0 + Arg1*Arg2
#define D3DTOP_LERP                26 // (Arg0)*Arg1 + (1-Arg0)*Arg2

#define D3DTA_TEXTURE        2
#define D3DTA_DIFFUSE        0

#define D3DTS_WORLD 256
#define D3DTS_VIEW 2
#define D3DTS_PROJECTION 3

#define D3DFMT_D16 80
#define D3DLIGHT_DIRECTIONAL    3

#define D3DCLEAR_TARGET  1
#define D3DCLEAR_ZBUFFER 2 
#define D3DCLEAR_STENCIL 4 

//#define D3DXMESH_SYSTEMMEM  272 
#define D3DXMESH_32BIT   0x001
#define D3DXMESH_DONOTCLIP   0x002
#define D3DXMESH_POINTS   0x004
#define D3DXMESH_RTPATCHES   0x008
#define D3DXMESH_NPATCHES   0x4000
#define D3DXMESH_VB_SYSTEMMEM   0x010
#define D3DXMESH_VB_MANAGED   0x020
#define D3DXMESH_VB_WRITEONLY   0x040
#define D3DXMESH_VB_DYNAMIC   0x080
#define D3DXMESH_VB_SOFTWAREPROCESSING   0x8000
#define D3DXMESH_IB_SYSTEMMEM   0x100
#define D3DXMESH_IB_MANAGED   0x200
#define D3DXMESH_IB_WRITEONLY   0x400
#define D3DXMESH_IB_DYNAMIC   0x800
#define D3DXMESH_IB_SOFTWAREPROCESSING   0x10000
#define D3DXMESH_VB_SHARE   0x1000
#define D3DXMESH_USEHWONLY   0x2000
#define D3DXMESH_SYSTEMMEM   0x110
#define D3DXMESH_MANAGED   0x220
#define D3DXMESH_WRITEONLY   0x440
#define D3DXMESH_DYNAMIC   0x880
#define D3DXMESH_SOFTWAREPROCESSING   0x18000

// function prototypes

// To do: all here the function prototypes from
//#include <d3dx9math.h>
//#include <d3dx9core.h>
// "d3dx9xof.h"
//#include <d3dx9mesh.h>
// "d3dx9shader.h"
// "d3dx9effect.h"
//#include <d3dx9tex.h>
// "d3dx9shape.h"
// "d3dx9anim.h"

// D3DX API functions ////////////////////////////////////////////////////////

IDirect3D9 * WINAPI Direct3DCreate9(UINT SDKVersion);

#define PRAGMA_API Direct3DCreate9;D3D9!Direct3DCreate9

BOOL WINAPI D3DXCheckVersion(UINT D3DSdkVersion, UINT D3DXSdkVersion);

BOOL WINAPI D3DXDebugMute(BOOL Mute);  

UINT WINAPI D3DXGetDriverLevel(LPDIRECT3DDEVICE9 pDevice);

#define PRAGMA_API D3DXCheckVersion;D3DX9_30!D3DXCheckVersion
#define PRAGMA_API D3DXDebugMute;D3DX9_30!D3DXDebugMute
#define PRAGMA_API D3DXGetDriverLevel;D3DX9_30!D3DXGetDriverLevel

#define PRAGMA_API D3DXBoxBoundProbe;D3DX9_30!D3DXBoxBoundProbe
#define PRAGMA_API D3DXCheckCubeTextureRequirements;D3DX9_30!D3DXCheckCubeTextureRequirements
#define PRAGMA_API D3DXCheckTextureRequirements;D3DX9_30!D3DXCheckTextureRequirements

#define PRAGMA_API D3DXCheckVolumeTextureRequirements;D3DX9_30!D3DXCheckVolumeTextureRequirements
#define PRAGMA_API D3DXCleanMesh;D3DX9_30!D3DXCleanMesh

// Color ///////////////////////////////////////////////////////////////////////

// Interpolate r,g,b between desaturated color and color.
// DesaturatedColor + s(Color - DesaturatedColor)
D3DXCOLOR* WINAPI D3DXColorAdjustSaturation
    (D3DXCOLOR *pOut, CONST D3DXCOLOR *pC, FLOAT s);

// Interpolate r,g,b between 50% grey and color.  Grey + s(Color - Grey)
D3DXCOLOR* WINAPI D3DXColorAdjustContrast
    (D3DXCOLOR *pOut, CONST D3DXCOLOR *pC, FLOAT c);

#define PRAGMA_API D3DXColorAdjustContrast;D3DX9_30!D3DXColorAdjustContrast
#define PRAGMA_API D3DXColorAdjustSaturation;D3DX9_30!D3DXColorAdjustSaturation

#define PRAGMA_API D3DXCompileShader;D3DX9_30!D3DXCompileShader
#define PRAGMA_API D3DXCompileShaderFromFileA;D3DX9_30!D3DXCompileShaderFromFileA
#define PRAGMA_API D3DXCompileShaderFromFileW;D3DX9_30!D3DXCompileShaderFromFileW
#define PRAGMA_API D3DXCompileShaderFromResourceA;D3DX9_30!D3DXCompileShaderFromResourceA
#define PRAGMA_API D3DXCompileShaderFromResourceW;D3DX9_30!D3DXCompileShaderFromResource
#define PRAGMA_API D3DXComputeBoundingBox;D3DX9_30!D3DXComputeBoundingBox
#define PRAGMA_API D3DXComputeBoundingSphere;D3DX9_30!D3DXComputeBoundingSphere
#define PRAGMA_API D3DXComputeIMTFromPerTexelSignal;D3DX9_30!D3DXComputeIMTFromPerTexelSignal
#define PRAGMA_API D3DXComputeIMTFromPerVertexSignal;D3DX9_30!D3DXComputeIMTFromPerVertexSignal
#define PRAGMA_API D3DXComputeIMTFromSignal;D3DX9_30!D3DXComputeIMTFromSignal
#define PRAGMA_API D3DXComputeIMTFromTexture;D3DX9_30!D3DXComputeIMTFromTexture
#define PRAGMA_API D3DXComputeNormalMap;D3DX9_30!D3DXComputeNormalMap
#define PRAGMA_API D3DXComputeNormals;D3DX9_30!D3DXComputeNormals
#define PRAGMA_API D3DXComputeTangent;D3DX9_30!D3DXComputeTangent
#define PRAGMA_API D3DXComputeTangentFrame;D3DX9_30!D3DXComputeTangentFrame
#define PRAGMA_API D3DXComputeTangentFrameEx;D3DX9_30!D3DXComputeTangentFrameEx
#define PRAGMA_API D3DXConcatenateMeshes;D3DX9_30!D3DXConcatenateMeshes
#define PRAGMA_API D3DXConvertMeshSubsetToSingleStrip;D3DX9_30!D3DXConvertMeshSubsetToSingleStrip
#define PRAGMA_API D3DXConvertMeshSubsetToStrips;D3DX9_30!D3DXConvertMeshSubsetToStrips
#define PRAGMA_API D3DXCpuOptimizations;D3DX9_30!D3DXCpuOptimizations
#define PRAGMA_API D3DXCreateAnimationController;D3DX9_30!D3DXCreateAnimationController
#define PRAGMA_API D3DXCreateBox;D3DX9_30!D3DXCreateBox
#define PRAGMA_API D3DXCreateBuffer;D3DX9_30!D3DXCreateBuffer
#define PRAGMA_API D3DXCreateCompressedAnimationSet;D3DX9_30!D3DXCreateCompressedAnimationSet
#define PRAGMA_API D3DXCreateCubeTexture;D3DX9_30!D3DXCreateCubeTexture
#define PRAGMA_API D3DXCreateCubeTextureFromFileA;D3DX9_30!D3DXCreateCubeTextureFromFileA
#define PRAGMA_API D3DXCreateCubeTextureFromFileExA;D3DX9_30!D3DXCreateCubeTextureFromFileExA
#define PRAGMA_API D3DXCreateCubeTextureFromFileExW;D3DX9_30!D3DXCreateCubeTextureFromFileExW
#define PRAGMA_API D3DXCreateCubeTextureFromFileInMemory;D3DX9_30!D3DXCreateCubeTextureFromFileInMemory
#define PRAGMA_API D3DXCreateCubeTextureFromFileInMemoryEx;D3DX9_30!D3DXCreateCubeTextureFromFileInMemoryEx
#define PRAGMA_API D3DXCreateCubeTextureFromFileW;D3DX9_30!D3DXCreateCubeTextureFromFileW
#define PRAGMA_API D3DXCreateCubeTextureFromResourceA;D3DX9_30!D3DXCreateCubeTextureFromResourceA
#define PRAGMA_API D3DXCreateCubeTextureFromResourceExA;D3DX9_30!D3DXCreateCubeTextureFromResourceExA
#define PRAGMA_API D3DXCreateCubeTextureFromResourceExW;D3DX9_30!D3DXCreateCubeTextureFromResourceExW
#define PRAGMA_API D3DXCreateCubeTextureFromResourceW;D3DX9_30!D3DXCreateCubeTextureFromResourceW
#define PRAGMA_API D3DXCreateCylinder;D3DX9_30!D3DXCreateCylinder

// Textures /////////////////////////////////////////////////////

HRESULT WINAPI D3DXCreateTextureFromFileA(
        LPDIRECT3DDEVICE9         pDevice,
        LPCSTR                    pSrcFile,
        LPDIRECT3DTEXTURE9*       ppTexture);

#define D3DXCreateTextureFromFile D3DXCreateTextureFromFileA

HRESULT WINAPI
    D3DXGetImageInfoFromFileA(
        LPCSTR                    pSrcFile,
        D3DXIMAGE_INFO*           pSrcInfo);

#define D3DXGetImageInfoFromFile D3DXGetImageInfoFromFileA

HRESULT WINAPI
    D3DXGetImageInfoFromResourceA(
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        D3DXIMAGE_INFO*           pSrcInfo);

#define D3DXGetImageInfoFromResource D3DXGetImageInfoFromResourceA

HRESULT WINAPI
    D3DXGetImageInfoFromFileInMemory(
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        D3DXIMAGE_INFO*           pSrcInfo);

HRESULT WINAPI
    D3DXLoadSurfaceFromFileA(
        LPDIRECT3DSURFACE9        pDestSurface,
        CONST PALETTEENTRY*       pDestPalette,
        CONST RECT*               pDestRect,
        LPCSTR                    pSrcFile,
        CONST RECT*               pSrcRect,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

#define D3DXLoadSurfaceFromFile D3DXLoadSurfaceFromFileA

HRESULT WINAPI
    D3DXLoadSurfaceFromResourceA(
        LPDIRECT3DSURFACE9        pDestSurface,
        CONST PALETTEENTRY*       pDestPalette,
        CONST RECT*               pDestRect,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        CONST RECT*               pSrcRect,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

#define D3DXLoadSurfaceFromResource D3DXLoadSurfaceFromResourceA

HRESULT WINAPI
    D3DXLoadSurfaceFromFileInMemory(
        LPDIRECT3DSURFACE9        pDestSurface,
        CONST PALETTEENTRY*       pDestPalette,
        CONST RECT*               pDestRect,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        CONST RECT*               pSrcRect,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

HRESULT WINAPI
    D3DXLoadSurfaceFromSurface(
        LPDIRECT3DSURFACE9        pDestSurface,
        CONST PALETTEENTRY*       pDestPalette,
        CONST RECT*               pDestRect,
        LPDIRECT3DSURFACE9        pSrcSurface,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST RECT*               pSrcRect,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey);

HRESULT WINAPI
    D3DXLoadSurfaceFromMemory(
        LPDIRECT3DSURFACE9        pDestSurface,
        CONST PALETTEENTRY*       pDestPalette,
        CONST RECT*               pDestRect,
        LPCVOID                   pSrcMemory,
        D3DFORMAT                 SrcFormat,
        UINT                      SrcPitch,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST RECT*               pSrcRect,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey);

HRESULT WINAPI
    D3DXSaveSurfaceToFileA(
        LPCSTR                    pDestFile,
        D3DXIMAGE_FILEFORMAT      DestFormat,
        LPDIRECT3DSURFACE9        pSrcSurface,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST RECT*               pSrcRect);

#define D3DXSaveSurfaceToFile D3DXSaveSurfaceToFileA

HRESULT WINAPI
    D3DXSaveSurfaceToFileInMemory(
        LPD3DXBUFFER*             ppDestBuf,
        D3DXIMAGE_FILEFORMAT      DestFormat,
        LPDIRECT3DSURFACE9        pSrcSurface,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST RECT*               pSrcRect);

/*HRESULT WINAPI
    D3DXLoadVolumeFromFileA(
        LPDIRECT3DVOLUME9         pDestVolume,
        CONST PALETTEENTRY*       pDestPalette,
        CONST D3DBOX*             pDestBox,
        LPCSTR                    pSrcFile,
        CONST D3DBOX*             pSrcBox,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

#define D3DXLoadVolumeFromFile D3DXLoadVolumeFromFileA

HRESULT WINAPI
    D3DXLoadVolumeFromResourceA(
        LPDIRECT3DVOLUME9         pDestVolume,
        CONST PALETTEENTRY*       pDestPalette,
        CONST D3DBOX*             pDestBox,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        CONST D3DBOX*             pSrcBox,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

#define D3DXLoadVolumeFromResource D3DXLoadVolumeFromResourceA


HRESULT WINAPI
    D3DXLoadVolumeFromFileInMemory(
        LPDIRECT3DVOLUME9         pDestVolume,
        CONST PALETTEENTRY*       pDestPalette,
        CONST D3DBOX*             pDestBox,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        CONST D3DBOX*             pSrcBox,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

HRESULT WINAPI
    D3DXLoadVolumeFromVolume(
        LPDIRECT3DVOLUME9         pDestVolume,
        CONST PALETTEENTRY*       pDestPalette,
        CONST D3DBOX*             pDestBox,
        LPDIRECT3DVOLUME9         pSrcVolume,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST D3DBOX*             pSrcBox,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey);


HRESULT WINAPI
    D3DXLoadVolumeFromMemory(
        LPDIRECT3DVOLUME9         pDestVolume,
        CONST PALETTEENTRY*       pDestPalette,
        CONST D3DBOX*             pDestBox,
        LPCVOID                   pSrcMemory,
        D3DFORMAT                 SrcFormat,
        UINT                      SrcRowPitch,
        UINT                      SrcSlicePitch,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST D3DBOX*             pSrcBox,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey);

HRESULT WINAPI
    D3DXSaveVolumeToFileA(
        LPCSTR                    pDestFile,
        D3DXIMAGE_FILEFORMAT      DestFormat,
        LPDIRECT3DVOLUME9         pSrcVolume,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST D3DBOX*             pSrcBox);

#define D3DXSaveVolumeToFile D3DXSaveVolumeToFileA

HRESULT WINAPI
    D3DXSaveVolumeToFileInMemory(
        LPD3DXBUFFER*             ppDestBuf,
        D3DXIMAGE_FILEFORMAT      DestFormat,
        LPDIRECT3DVOLUME9         pSrcVolume,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST D3DBOX*             pSrcBox);*/

HRESULT WINAPI
    D3DXCheckTextureRequirements(
        LPDIRECT3DDEVICE9         pDevice,
        UINT*                     pWidth,
        UINT*                     pHeight,
        UINT*                     pNumMipLevels,
        DWORD                     Usage,
        D3DFORMAT*                pFormat,
        D3DPOOL                   Pool);

HRESULT WINAPI
    D3DXCheckCubeTextureRequirements(
        LPDIRECT3DDEVICE9         pDevice,
        UINT*                     pSize,
        UINT*                     pNumMipLevels,
        DWORD                     Usage,
        D3DFORMAT*                pFormat,
        D3DPOOL                   Pool);

HRESULT WINAPI
    D3DXCheckVolumeTextureRequirements(
        LPDIRECT3DDEVICE9         pDevice,
        UINT*                     pWidth,
        UINT*                     pHeight,
        UINT*                     pDepth,
        UINT*                     pNumMipLevels,
        DWORD                     Usage,
        D3DFORMAT*                pFormat,
        D3DPOOL                   Pool);

HRESULT WINAPI
    D3DXCreateTexture(
        LPDIRECT3DDEVICE9         pDevice,
        UINT                      Width,
        UINT                      Height,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        LPDIRECT3DTEXTURE9*       ppTexture);

/*HRESULT WINAPI
    D3DXCreateCubeTexture(
        LPDIRECT3DDEVICE9         pDevice,
        UINT                      Size,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

HRESULT WINAPI
    D3DXCreateVolumeTexture(
        LPDIRECT3DDEVICE9         pDevice,
        UINT                      Width,
        UINT                      Height,
        UINT                      Depth,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);*/

HRESULT WINAPI
    D3DXCreateTextureFromFileA(
        LPDIRECT3DDEVICE9         pDevice,
        LPCSTR                    pSrcFile,
        LPDIRECT3DTEXTURE9*       ppTexture);

#define D3DXCreateTextureFromFile D3DXCreateTextureFromFileA

/*HRESULT WINAPI
    D3DXCreateCubeTextureFromFileA(
        LPDIRECT3DDEVICE9         pDevice,
        LPCSTR                    pSrcFile,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

#define D3DXCreateCubeTextureFromFile D3DXCreateCubeTextureFromFileA

HRESULT WINAPI
    D3DXCreateVolumeTextureFromFileA(
        LPDIRECT3DDEVICE9         pDevice,
        LPCSTR                    pSrcFile,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);

#define D3DXCreateVolumeTextureFromFile D3DXCreateVolumeTextureFromFileA*/

HRESULT WINAPI
    D3DXCreateTextureFromResourceA(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        LPDIRECT3DTEXTURE9*       ppTexture);

#define D3DXCreateTextureFromResource D3DXCreateTextureFromResourceA

/*HRESULT WINAPI
    D3DXCreateCubeTextureFromResourceA(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

#define D3DXCreateCubeTextureFromResource D3DXCreateCubeTextureFromResourceA

HRESULT WINAPI
    D3DXCreateVolumeTextureFromResourceA(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);

#define D3DXCreateVolumeTextureFromResource D3DXCreateVolumeTextureFromResourceA*/

HRESULT WINAPI
    D3DXCreateTextureFromFileExA(
        LPDIRECT3DDEVICE9         pDevice,
        LPCSTR                    pSrcFile,
        UINT                      Width,
        UINT                      Height,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DTEXTURE9*       ppTexture);

#define D3DXCreateTextureFromFileEx D3DXCreateTextureFromFileExA

/*HRESULT WINAPI
    D3DXCreateCubeTextureFromFileExA(
        LPDIRECT3DDEVICE9         pDevice,
        LPCSTR                    pSrcFile,
        UINT                      Size,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

#define D3DXCreateCubeTextureFromFileEx D3DXCreateCubeTextureFromFileExA

HRESULT WINAPI
    D3DXCreateVolumeTextureFromFileExA(
        LPDIRECT3DDEVICE9         pDevice,
        LPCSTR                    pSrcFile,
        UINT                      Width,
        UINT                      Height,
        UINT                      Depth,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);

#define D3DXCreateVolumeTextureFromFileEx D3DXCreateVolumeTextureFromFileExA*/

HRESULT WINAPI
    D3DXCreateTextureFromResourceExA(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        UINT                      Width,
        UINT                      Height,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DTEXTURE9*       ppTexture);

#define D3DXCreateTextureFromResourceEx D3DXCreateTextureFromResourceExA

/*HRESULT WINAPI
    D3DXCreateCubeTextureFromResourceExA(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        UINT                      Size,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

#define D3DXCreateCubeTextureFromResourceEx D3DXCreateCubeTextureFromResourceExA

HRESULT WINAPI
    D3DXCreateVolumeTextureFromResourceExA(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        UINT                      Width,
        UINT                      Height,
        UINT                      Depth,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);

#define D3DXCreateVolumeTextureFromResourceEx D3DXCreateVolumeTextureFromResourceExA*/

HRESULT WINAPI
    D3DXCreateTextureFromFileInMemory(
        LPDIRECT3DDEVICE9         pDevice,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        LPDIRECT3DTEXTURE9*       ppTexture);

/*HRESULT WINAPI
    D3DXCreateCubeTextureFromFileInMemory(
        LPDIRECT3DDEVICE9         pDevice,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

HRESULT WINAPI
    D3DXCreateVolumeTextureFromFileInMemory(
        LPDIRECT3DDEVICE9         pDevice,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);*/

HRESULT WINAPI
    D3DXCreateTextureFromFileInMemoryEx(
        LPDIRECT3DDEVICE9         pDevice,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        UINT                      Width,
        UINT                      Height,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DTEXTURE9*       ppTexture);

/*HRESULT WINAPI
    D3DXCreateCubeTextureFromFileInMemoryEx(
        LPDIRECT3DDEVICE9         pDevice,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        UINT                      Size,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

HRESULT WINAPI
    D3DXCreateVolumeTextureFromFileInMemoryEx(
        LPDIRECT3DDEVICE9         pDevice,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        UINT                      Width,
        UINT                      Height,
        UINT                      Depth,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);*/

/*HRESULT WINAPI
    D3DXSaveTextureToFileA(
        LPCSTR                    pDestFile,
        D3DXIMAGE_FILEFORMAT      DestFormat,
        LPDIRECT3DBASETEXTURE9    pSrcTexture,
        CONST PALETTEENTRY*       pSrcPalette);

#define D3DXSaveTextureToFile D3DXSaveTextureToFileA

HRESULT WINAPI
    D3DXSaveTextureToFileInMemory(
        LPD3DXBUFFER*             ppDestBuf,
        D3DXIMAGE_FILEFORMAT      DestFormat,
        LPDIRECT3DBASETEXTURE9    pSrcTexture,
        CONST PALETTEENTRY*       pSrcPalette);

HRESULT WINAPI
    D3DXFilterTexture(
        LPDIRECT3DBASETEXTURE9    pBaseTexture,
        CONST PALETTEENTRY*       pPalette,
        UINT                      SrcLevel,
        DWORD                     Filter);

#define D3DXFilterCubeTexture D3DXFilterTexture
#define D3DXFilterVolumeTexture D3DXFilterTexture*/

/*HRESULT WINAPI
    D3DXFillTexture(
        LPDIRECT3DTEXTURE9        pTexture,
        LPD3DXFILL2D              pFunction,
        LPVOID                    pData);

HRESULT WINAPI
    D3DXFillCubeTexture(
        LPDIRECT3DCUBETEXTURE9    pCubeTexture,
        LPD3DXFILL3D              pFunction,
        LPVOID                    pData);

HRESULT WINAPI
    D3DXFillVolumeTexture(
        LPDIRECT3DVOLUMETEXTURE9  pVolumeTexture,
        LPD3DXFILL3D              pFunction,
        LPVOID                    pData);

HRESULT WINAPI 
    D3DXFillTextureTX(
        LPDIRECT3DTEXTURE9        pTexture,
        LPD3DXTEXTURESHADER       pTextureShader);

HRESULT WINAPI
    D3DXFillCubeTextureTX(
        LPDIRECT3DCUBETEXTURE9    pCubeTexture,
        LPD3DXTEXTURESHADER       pTextureShader);
                                                        
HRESULT WINAPI 
    D3DXFillVolumeTextureTX(
        LPDIRECT3DVOLUMETEXTURE9  pVolumeTexture,
        LPD3DXTEXTURESHADER       pTextureShader);*/

HRESULT WINAPI
    D3DXComputeNormalMap(
        LPDIRECT3DTEXTURE9        pTexture,
        LPDIRECT3DTEXTURE9        pSrcTexture,
        CONST PALETTEENTRY*       pSrcPalette,
        DWORD                     Flags,
        DWORD                     Channel,
        FLOAT                     Amplitude);

#define PRAGMA_API D3DXCreateTexture;D3DX9_30!D3DXCreateTexture
#define PRAGMA_API D3DXCreateTextureFromFileA;D3DX9_30!D3DXCreateTextureFromFileA
#define PRAGMA_API D3DXCreateTextureFromFileExA;D3DX9_30!D3DXCreateTextureFromFileExA
#define PRAGMA_API D3DXCreateTextureFromFileExW;D3DX9_30!D3DXCreateTextureFromFileExW
#define PRAGMA_API D3DXCreateTextureFromFileInMemory;D3DX9_30!D3DXCreateTextureFromFileInMemory
#define PRAGMA_API D3DXCreateTextureFromFileInMemoryEx;D3DX9_30!D3DXCreateTextureFromFileInMemoryEx
#define PRAGMA_API D3DXCreateTextureFromFileW;D3DX9_30!D3DXCreateTextureFromFileW
#define PRAGMA_API D3DXCreateTextureFromResourceA;D3DX9_30!D3DXCreateTextureFromResourceA
#define PRAGMA_API D3DXCreateTextureFromResourceExA;D3DX9_30!D3DXCreateTextureFromResourceExA
#define PRAGMA_API D3DXCreateTextureFromResourceExW;D3DX9_30!D3DXCreateTextureFromResourceExW
#define PRAGMA_API D3DXCreateTextureFromResourceW;D3DX9_30!D3DXCreateTextureFromResourceW
#define PRAGMA_API D3DXCreateTextureGutterHelper;D3DX9_30!D3DXCreateTextureGutterHelper
#define PRAGMA_API D3DXCreateTextureShader;D3DX9_30!D3DXCreateTextureShader
#define PRAGMA_API D3DXCreateTextW;D3DX9_30!D3DXCreateTextW
#define PRAGMA_API D3DXCreateTorus;D3DX9_30!D3DXCreateTorus
#define PRAGMA_API D3DXCreateVolumeTexture;D3DX9_30!D3DXCreateVolumeTexture
#define PRAGMA_API D3DXCreateVolumeTextureFromFileA;D3DX9_30!D3DXCreateVolumeTextureFromFileA
#define PRAGMA_API D3DXCreateVolumeTextureFromFileExA;D3DX9_30!D3DXCreateVolumeTextureFromFileExA
#define PRAGMA_API D3DXCreateVolumeTextureFromFileExW;D3DX9_30!D3DXCreateVolumeTextureFromFileExW
#define PRAGMA_API D3DXCreateVolumeTextureFromFileInMemory;D3DX9_30!D3DXCreateVolumeTextureFromFileInMemory
#define PRAGMA_API D3DXCreateVolumeTextureFromFileInMemoryEx;D3DX9_30!D3DXCreateVolumeTextureFromFileInMemoryEx
#define PRAGMA_API D3DXCreateVolumeTextureFromFileW;D3DX9_30!D3DXCreateVolumeTextureFromFileW
#define PRAGMA_API D3DXCreateVolumeTextureFromResourceA;D3DX9_30!D3DXCreateVolumeTextureFromResourceA
#define PRAGMA_API D3DXCreateVolumeTextureFromResourceExA;D3DX9_30!D3DXCreateVolumeTextureFromResourceExA
#define PRAGMA_API D3DXCreateVolumeTextureFromResourceExW;D3DX9_30!D3DXCreateVolumeTextureFromResourceExW
#define PRAGMA_API D3DXCreateVolumeTextureFromResourceW;D3DX9_30!D3DXCreateVolumeTextureFromResourceW
#define PRAGMA_API D3DXFileCreate;D3DX9_30!D3DXFileCreate
#define PRAGMA_API D3DXFillCubeTexture;D3DX9_30!D3DXFillCubeTexture
#define PRAGMA_API D3DXFillCubeTextureTX;D3DX9_30!D3DXFillCubeTextureTX
#define PRAGMA_API D3DXFillTexture;D3DX9_30!D3DXFillTexture
#define PRAGMA_API D3DXFillTextureTX;D3DX9_30!D3DXFillTextureTX
#define PRAGMA_API D3DXFillVolumeTexture;D3DX9_30!D3DXFillVolumeTexture
#define PRAGMA_API D3DXFillVolumeTextureTX;D3DX9_30!D3DXFillVolumeTextureTX
#define PRAGMA_API D3DXFilterTexture;D3DX9_30!D3DXFilterTexture
#define PRAGMA_API D3DXFindShaderComment;D3DX9_30!D3DXFindShaderComment
#define PRAGMA_API D3DXFloat16To32Array;D3DX9_30!D3DXFloat16To32Array
#define PRAGMA_API D3DXFloat32To16Array;D3DX9_30!D3DXFloat32To16Array
#define PRAGMA_API D3DXFrameAppendChild;D3DX9_30!D3DXFrameAppendChild
#define PRAGMA_API D3DXFrameCalculateBoundingSphere;D3DX9_30!D3DXFrameCalculateBoundingSphere
#define PRAGMA_API D3DXFrameDestroy;D3DX9_30!D3DXFrameDestroy
#define PRAGMA_API D3DXFrameFind;D3DX9_30!D3DXFrameFind
#define PRAGMA_API D3DXFrameNumNamedMatrices;D3DX9_30!D3DXFrameNumNamedMatrices
#define PRAGMA_API D3DXFrameRegisterNamedMatrices;D3DX9_30!D3DXFrameRegisterNamedMatrices
#define PRAGMA_API D3DXFresnelTerm;D3DX9_30!D3DXFresnelTerm
#define PRAGMA_API D3DXFVFFromDeclarator;D3DX9_30!D3DXFVFFromDeclarator
#define PRAGMA_API D3DXGatherFragments;D3DX9_30!D3DXGatherFragments
#define PRAGMA_API D3DXGatherFragmentsFromFileA;D3DX9_30!D3DXGatherFragmentsFromFileA
#define PRAGMA_API D3DXGatherFragmentsFromFileW;D3DX9_30!D3DXGatherFragmentsFromFileW
#define PRAGMA_API D3DXGatherFragmentsFromResourceA;D3DX9_30!D3DXGatherFragmentsFromResourceA
#define PRAGMA_API D3DXGatherFragmentsFromResourceW;D3DX9_30!D3DXGatherFragmentsFromResourceW
#define PRAGMA_API D3DXGenerateOutputDecl;D3DX9_30!D3DXGenerateOutputDecl
#define PRAGMA_API D3DXGeneratePMesh;D3DX9_30!D3DXGeneratePMesh
#define PRAGMA_API D3DXGetDeclLength;D3DX9_30!D3DXGetDeclLength
#define PRAGMA_API D3DXGetDeclVertexSize;D3DX9_30!D3DXGetDeclVertexSize

#define PRAGMA_API D3DXGetFVFVertexSize;D3DX9_30!D3DXGetFVFVertexSize
#define PRAGMA_API D3DXGetImageInfoFromFileA;D3DX9_30!D3DXGetImageInfoFromFileA
#define PRAGMA_API D3DXGetImageInfoFromFileInMemory;D3DX9_30!D3DXGetImageInfoFromFileInMemory
#define PRAGMA_API D3DXGetImageInfoFromFileW;D3DX9_30!D3DXGetImageInfoFromFileW
#define PRAGMA_API D3DXGetImageInfoFromResourceA;D3DX9_30!D3DXGetImageInfoFromResourceA
#define PRAGMA_API D3DXGetImageInfoFromResourceW;D3DX9_30!D3DXGetImageInfoFromResourceW
#define PRAGMA_API D3DXGetPixelShaderProfile;D3DX9_30!D3DXGetPixelShaderProfile
#define PRAGMA_API D3DXGetShaderConstantTable;D3DX9_30!D3DXGetShaderConstantTable
#define PRAGMA_API D3DXGetShaderInputSemantics;D3DX9_30!D3DXGetShaderInputSemantics
#define PRAGMA_API D3DXGetShaderOutputSemantics;D3DX9_30!D3DXGetShaderOutputSemantics
#define PRAGMA_API D3DXGetShaderSamplers;D3DX9_30!D3DXGetShaderSamplers
#define PRAGMA_API D3DXGetShaderSize;D3DX9_30!D3DXGetShaderSize
#define PRAGMA_API D3DXGetShaderVersion;D3DX9_30!D3DXGetShaderVersion
#define PRAGMA_API D3DXGetTargetDescByName;D3DX9_30!D3DXGetTargetDescByName
#define PRAGMA_API D3DXGetTargetDescByVersion;D3DX9_30!D3DXGetTargetDescByVersion
#define PRAGMA_API D3DXGetVertexShaderProfile;D3DX9_30!D3DXGetVertexShaderProfile
#define PRAGMA_API D3DXIntersect;D3DX9_30!D3DXIntersect
#define PRAGMA_API D3DXIntersectSubset;D3DX9_30!D3DXIntersectSubset
#define PRAGMA_API D3DXIntersectTri;D3DX9_30!D3DXIntersectTri

// Mesh /////////////////////////////////////////////////////////////////////////

HRESULT WINAPI 
    D3DXCreateMesh(
        DWORD NumFaces, 
        DWORD NumVertices, 
        DWORD Options, 
        CONST D3DVERTEXELEMENT9 *pDeclaration, 
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXMESH* ppMesh);

HRESULT WINAPI 
    D3DXCreateMeshFVF(
        DWORD NumFaces, 
        DWORD NumVertices, 
        DWORD Options, 
        DWORD FVF, 
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXMESH* ppMesh);

/*HRESULT WINAPI 
    D3DXCreateSPMesh(
        LPD3DXMESH pMesh, 
        CONST DWORD* pAdjacency, 
        CONST D3DXATTRIBUTEWEIGHTS *pVertexAttributeWeights,
        CONST FLOAT *pVertexWeights,
        LPD3DXSPMESH* ppSMesh);*/

// clean a mesh up for simplification, try to make manifold
/*HRESULT WINAPI
    D3DXCleanMesh(
    D3DXCLEANTYPE CleanType,
    LPD3DXMESH pMeshIn,
    CONST DWORD* pAdjacencyIn,
    LPD3DXMESH* ppMeshOut,
    DWORD* pAdjacencyOut,
    LPD3DXBUFFER* ppErrorsAndWarnings);*/

HRESULT WINAPI
    D3DXValidMesh(
    LPD3DXMESH pMeshIn,
    CONST DWORD* pAdjacency,
    LPD3DXBUFFER* ppErrorsAndWarnings);

/*HRESULT WINAPI 
    D3DXGeneratePMesh(
        LPD3DXMESH pMesh, 
        CONST DWORD* pAdjacency, 
        CONST D3DXATTRIBUTEWEIGHTS *pVertexAttributeWeights,
        CONST FLOAT *pVertexWeights,
        DWORD MinValue, 
        DWORD Options, 
        LPD3DXPMESH* ppPMesh);*/

HRESULT WINAPI 
    D3DXSimplifyMesh(
        LPD3DXMESH pMesh, 
        CONST DWORD* pAdjacency, 
        CONST D3DXATTRIBUTEWEIGHTS *pVertexAttributeWeights,
        CONST FLOAT *pVertexWeights,
        DWORD MinValue, 
        DWORD Options, 
        LPD3DXMESH* ppMesh);

HRESULT WINAPI 
    D3DXComputeBoundingSphere(
        CONST D3DXVECTOR3 *pFirstPosition,  // pointer to first position
        DWORD NumVertices, 
        DWORD dwStride,                     // count in bytes to subsequent position vectors
        D3DXVECTOR3 *pCenter, 
        FLOAT *pRadius);

HRESULT WINAPI 
    D3DXComputeBoundingBox(
        CONST D3DXVECTOR3 *pFirstPosition,  // pointer to first position
        DWORD NumVertices, 
        DWORD dwStride,                     // count in bytes to subsequent position vectors
        D3DXVECTOR3 *pMin, 
        D3DXVECTOR3 *pMax);

HRESULT WINAPI 
    D3DXComputeNormals(
        LPD3DXBASEMESH pMesh,
        CONST DWORD *pAdjacency);

HRESULT WINAPI 
    D3DXCreateBuffer(
        DWORD NumBytes, 
        LPD3DXBUFFER *ppBuffer);


HRESULT WINAPI
    D3DXLoadMeshFromXA(
        LPCSTR pFilename, 
        DWORD Options, 
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXBUFFER *ppAdjacency,
        LPD3DXBUFFER *ppMaterials, 
        LPD3DXBUFFER *ppEffectInstances, 
        DWORD *pNumMaterials,
        LPD3DXMESH *ppMesh);

#define D3DXLoadMeshFromX D3DXLoadMeshFromXA

HRESULT WINAPI 
    D3DXLoadMeshFromXInMemory(
        LPCVOID Memory,
        DWORD SizeOfMemory,
        DWORD Options, 
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXBUFFER *ppAdjacency,
        LPD3DXBUFFER *ppMaterials, 
        LPD3DXBUFFER *ppEffectInstances, 
        DWORD *pNumMaterials,
        LPD3DXMESH *ppMesh);

HRESULT WINAPI 
    D3DXLoadMeshFromXResource(
        HMODULE Module,
        LPCSTR Name,
        LPCSTR Type,
        DWORD Options, 
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXBUFFER *ppAdjacency,
        LPD3DXBUFFER *ppMaterials, 
        LPD3DXBUFFER *ppEffectInstances, 
        DWORD *pNumMaterials,
        LPD3DXMESH *ppMesh);

/*HRESULT WINAPI 
    D3DXSaveMeshToXA(
        LPCSTR pFilename,
        LPD3DXMESH pMesh,
        CONST DWORD* pAdjacency,
        CONST D3DXMATERIAL* pMaterials,
        CONST D3DXEFFECTINSTANCE* pEffectInstances, 
        DWORD NumMaterials,
        DWORD Format
        );*/

#define D3DXSaveMeshToX D3DXSaveMeshToXA
        

/*HRESULT WINAPI 
    D3DXCreatePMeshFromStream(
        IStream *pStream, 
        DWORD Options,
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXBUFFER *ppMaterials,
        LPD3DXBUFFER *ppEffectInstances, 
        DWORD* pNumMaterials,
        LPD3DXPMESH *ppPMesh);*/

// Creates a skin info object based on the number of vertices, number of bones, and a declaration describing the vertex layout of the target vertices
// The bone names and initial bone transforms are not filled in the skin info object by this method.
/*HRESULT WINAPI
    D3DXCreateSkinInfo(
        DWORD NumVertices,
        CONST D3DVERTEXELEMENT9 *pDeclaration, 
        DWORD NumBones,
        LPD3DXSKININFO* ppSkinInfo);
        
// Creates a skin info object based on the number of vertices, number of bones, and a FVF describing the vertex layout of the target vertices
// The bone names and initial bone transforms are not filled in the skin info object by this method.
HRESULT WINAPI
    D3DXCreateSkinInfoFVF(
        DWORD NumVertices,
        DWORD FVF,
        DWORD NumBones,
        LPD3DXSKININFO* ppSkinInfo);
        
HRESULT WINAPI 
    D3DXLoadMeshFromXof(
        LPD3DXFILEDATA pxofMesh, 
        DWORD Options, 
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXBUFFER *ppAdjacency,
        LPD3DXBUFFER *ppMaterials, 
        LPD3DXBUFFER *ppEffectInstances, 
        DWORD *pNumMaterials,
        LPD3DXMESH *ppMesh);

// This similar to D3DXLoadMeshFromXof, except also returns skinning info if present in the file
// If skinning info is not present, ppSkinInfo will be NULL     
HRESULT WINAPI
    D3DXLoadSkinMeshFromXof(
        LPD3DXFILEDATA pxofMesh, 
        DWORD Options,
        LPDIRECT3DDEVICE9 pD3DDevice,
        LPD3DXBUFFER* ppAdjacency,
        LPD3DXBUFFER* ppMaterials,
        LPD3DXBUFFER *ppEffectInstances, 
        DWORD *pMatOut,
        LPD3DXSKININFO* ppSkinInfo,
        LPD3DXMESH* ppMesh);


// The inverse of D3DXConvertTo{Indexed}BlendedMesh() functions. It figures out the skinning info from
// the mesh and the bone combination table and populates a skin info object with that data. The bone
// names and initial bone transforms are not filled in the skin info object by this method. This works
// with either a non-indexed or indexed blended mesh. It examines the FVF or declarator of the mesh to
// determine what type it is.
HRESULT WINAPI
    D3DXCreateSkinInfoFromBlendedMesh(
        LPD3DXBASEMESH pMesh,
        DWORD NumBones,
        CONST D3DXBONECOMBINATION *pBoneCombinationTable,
        LPD3DXSKININFO* ppSkinInfo);*/
        
HRESULT WINAPI
    D3DXTessellateNPatches(
        LPD3DXMESH pMeshIn,             
        CONST DWORD* pAdjacencyIn,             
        FLOAT NumSegs,                    
        BOOL  QuadraticInterpNormals,     // if false use linear intrep for normals, if true use quadratic
        LPD3DXMESH *ppMeshOut,
        LPD3DXBUFFER *ppAdjacencyOut);


//generates implied outputdecl from input decl
//the decl generated from this should be used to generate the output decl for
//the tessellator subroutines. 

HRESULT WINAPI
    D3DXGenerateOutputDecl(
        D3DVERTEXELEMENT9 *pOutput,
        CONST D3DVERTEXELEMENT9 *pInput);

//loads patches from an XFileData
//since an X file can have up to 6 different patch meshes in it,
//returns them in an array - pNumPatches will contain the number of
//meshes in the actual file. 
/*HRESULT WINAPI
    D3DXLoadPatchMeshFromXof(
        LPD3DXFILEDATA pXofObjMesh,
        DWORD Options,
        LPDIRECT3DDEVICE9 pD3DDevice,
        LPD3DXBUFFER *ppMaterials,
        LPD3DXBUFFER *ppEffectInstances, 
        PDWORD pNumMaterials,
        LPD3DXPATCHMESH *ppMesh);*/

//computes the size a single rect patch.
HRESULT WINAPI
    D3DXRectPatchSize(
        CONST FLOAT *pfNumSegs, //segments for each edge (4)
        DWORD *pdwTriangles,    //output number of triangles
        DWORD *pdwVertices);    //output number of vertices

//computes the size of a single triangle patch      
HRESULT WINAPI
    D3DXTriPatchSize(
        CONST FLOAT *pfNumSegs, //segments for each edge (3)    
        DWORD *pdwTriangles,    //output number of triangles
        DWORD *pdwVertices);    //output number of vertices


//tessellates a patch into a created mesh
//similar to D3D RT patch
HRESULT WINAPI
    D3DXTessellateRectPatch(
        LPDIRECT3DVERTEXBUFFER9 pVB,
        CONST FLOAT *pNumSegs,
        CONST D3DVERTEXELEMENT9 *pdwInDecl,
        CONST D3DRECTPATCH_INFO *pRectPatchInfo,
        LPD3DXMESH pMesh);


HRESULT WINAPI
    D3DXTessellateTriPatch(
      LPDIRECT3DVERTEXBUFFER9 pVB,
      CONST FLOAT *pNumSegs,
      CONST D3DVERTEXELEMENT9 *pInDecl,
      CONST D3DTRIPATCH_INFO *pTriPatchInfo,
      LPD3DXMESH pMesh);



//creates an NPatch PatchMesh from a D3DXMESH 
/*HRESULT WINAPI
    D3DXCreateNPatchMesh(
        LPD3DXMESH pMeshSysMem,
        LPD3DXPATCHMESH *pPatchMesh);

      
//creates a patch mesh
HRESULT WINAPI
    D3DXCreatePatchMesh(
        CONST D3DXPATCHINFO *pInfo,     //patch type
        DWORD dwNumPatches,             //number of patches
        DWORD dwNumVertices,            //number of control vertices
        DWORD dwOptions,                //options 
        CONST D3DVERTEXELEMENT9 *pDecl, //format of control vertices
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXPATCHMESH *pPatchMesh);

        
//returns the number of degenerates in a patch mesh -
//text output put in string.
HRESULT WINAPI
    D3DXValidPatchMesh(LPD3DXPATCHMESH pMesh,
                        DWORD *dwcDegenerateVertices,
                        DWORD *dwcDegeneratePatches,
                        LPD3DXBUFFER *ppErrorsAndWarnings);*/

UINT WINAPI
    D3DXGetFVFVertexSize(DWORD FVF);

UINT WINAPI 
    D3DXGetDeclVertexSize(CONST D3DVERTEXELEMENT9 *pDecl,DWORD Stream);

UINT WINAPI 
    D3DXGetDeclLength(CONST D3DVERTEXELEMENT9 *pDecl);

HRESULT WINAPI
    D3DXDeclaratorFromFVF(
        DWORD FVF,
        D3DVERTEXELEMENT9 *pDeclarator);

HRESULT WINAPI
    D3DXFVFFromDeclarator(
        CONST D3DVERTEXELEMENT9 *pDeclarator,
        DWORD *pFVF);

/*HRESULT WINAPI 
    D3DXWeldVertices(
        LPD3DXMESH pMesh,         
        DWORD Flags,
        CONST D3DXWELDEPSILONS *pEpsilons,                 
        CONST DWORD *pAdjacencyIn, 
        DWORD *pAdjacencyOut,
        DWORD *pFaceRemap, 
        LPD3DXBUFFER *ppVertexRemap);*/

HRESULT WINAPI
    D3DXIntersect(
        LPD3DXBASEMESH pMesh,
        CONST D3DXVECTOR3 *pRayPos,
        CONST D3DXVECTOR3 *pRayDir, 
        BOOL    *pHit,              // True if any faces were intersected
        DWORD   *pFaceIndex,        // index of closest face intersected
        FLOAT   *pU,                // Barycentric Hit Coordinates    
        FLOAT   *pV,                // Barycentric Hit Coordinates
        FLOAT   *pDist,             // Ray-Intersection Parameter Distance
        LPD3DXBUFFER *ppAllHits,    // Array of D3DXINTERSECTINFOs for all hits (not just closest) 
        DWORD   *pCountOfHits);     // Number of entries in AllHits array

HRESULT WINAPI
    D3DXIntersectSubset(
        LPD3DXBASEMESH pMesh,
        DWORD AttribId,
        CONST D3DXVECTOR3 *pRayPos,
        CONST D3DXVECTOR3 *pRayDir, 
        BOOL    *pHit,              // True if any faces were intersected
        DWORD   *pFaceIndex,        // index of closest face intersected
        FLOAT   *pU,                // Barycentric Hit Coordinates    
        FLOAT   *pV,                // Barycentric Hit Coordinates
        FLOAT   *pDist,             // Ray-Intersection Parameter Distance
        LPD3DXBUFFER *ppAllHits,    // Array of D3DXINTERSECTINFOs for all hits (not just closest) 
        DWORD   *pCountOfHits);     // Number of entries in AllHits array


HRESULT WINAPI D3DXSplitMesh
    (
    LPD3DXMESH pMeshIn,         
    CONST DWORD *pAdjacencyIn, 
    CONST DWORD MaxSize,
    CONST DWORD Options,
    DWORD *pMeshesOut,
    LPD3DXBUFFER *ppMeshArrayOut,
    LPD3DXBUFFER *ppAdjacencyArrayOut,
    LPD3DXBUFFER *ppFaceRemapArrayOut,
    LPD3DXBUFFER *ppVertRemapArrayOut
    );

BOOL WINAPI D3DXIntersectTri 
(
    CONST D3DXVECTOR3 *p0,           // Triangle vertex 0 position
    CONST D3DXVECTOR3 *p1,           // Triangle vertex 1 position
    CONST D3DXVECTOR3 *p2,           // Triangle vertex 2 position
    CONST D3DXVECTOR3 *pRayPos,      // Ray origin
    CONST D3DXVECTOR3 *pRayDir,      // Ray direction
    FLOAT *pU,                       // Barycentric Hit Coordinates
    FLOAT *pV,                       // Barycentric Hit Coordinates
    FLOAT *pDist);                   // Ray-Intersection Parameter Distance

BOOL WINAPI
    D3DXSphereBoundProbe(
        CONST D3DXVECTOR3 *pCenter,
        FLOAT Radius,
        CONST D3DXVECTOR3 *pRayPosition,
        CONST D3DXVECTOR3 *pRayDirection);

BOOL WINAPI 
    D3DXBoxBoundProbe(
        CONST D3DXVECTOR3 *pMin, 
        CONST D3DXVECTOR3 *pMax,
        CONST D3DXVECTOR3 *pRayPosition,
        CONST D3DXVECTOR3 *pRayDirection);


HRESULT WINAPI D3DXComputeTangentFrame(ID3DXMesh *pMesh,
                                       DWORD dwOptions);

HRESULT WINAPI D3DXComputeTangentFrameEx(ID3DXMesh *pMesh,
                                         DWORD dwTextureInSemantic,
                                         DWORD dwTextureInIndex,
                                         DWORD dwUPartialOutSemantic,
                                         DWORD dwUPartialOutIndex,
                                         DWORD dwVPartialOutSemantic,
                                         DWORD dwVPartialOutIndex,
                                         DWORD dwNormalOutSemantic,
                                         DWORD dwNormalOutIndex,
                                         DWORD dwOptions,
                                         CONST DWORD *pdwAdjacency,
                                         FLOAT fPartialEdgeThreshold,
                                         FLOAT fSingularPointThreshold,
                                         FLOAT fNormalEdgeThreshold,
                                         ID3DXMesh **ppMeshOut,
                                         ID3DXBuffer **ppVertexMapping);


HRESULT WINAPI D3DXComputeTangent(LPD3DXMESH Mesh,
                                 DWORD TexStage,
                                 DWORD TangentIndex,
                                 DWORD BinormIndex,
                                 DWORD Wrap,
                                 CONST DWORD *pAdjacency);

#define PRAGMA_API D3DXLoadMeshFromXA;D3DX9_30!D3DXLoadMeshFromXA
#define PRAGMA_API D3DXLoadMeshFromXInMemory;D3DX9_30!D3DXLoadMeshFromXInMemory
#define PRAGMA_API D3DXLoadMeshFromXof;D3DX9_30!D3DXLoadMeshFromXof
#define PRAGMA_API D3DXLoadMeshFromXResource;D3DX9_30!D3DXLoadMeshFromXResource
#define PRAGMA_API D3DXLoadMeshFromXW;D3DX9_30!D3DXLoadMeshFromXW
#define PRAGMA_API D3DXLoadMeshHierarchyFromXA;D3DX9_30!D3DXLoadMeshHierarchyFromXA
#define PRAGMA_API D3DXLoadMeshHierarchyFromXInMemory;D3DX9_30!D3DXLoadMeshHierarchyFromXInMemory
#define PRAGMA_API D3DXLoadMeshHierarchyFromXW;D3DX9_30!D3DXLoadMeshHierarchyFromXW
#define PRAGMA_API D3DXLoadPatchMeshFromXof;D3DX9_30!D3DXLoadPatchMeshFromXof
#define PRAGMA_API D3DXLoadPRTBufferFromFileA;D3DX9_30!D3DXLoadPRTBufferFromFileA
#define PRAGMA_API D3DXLoadPRTBufferFromFileW;D3DX9_30!D3DXLoadPRTBufferFromFileW
#define PRAGMA_API D3DXLoadPRTCompBufferFromFileA;D3DX9_30!D3DXLoadPRTCompBufferFromFileA
#define PRAGMA_API D3DXLoadPRTCompBufferFromFileW;D3DX9_30!D3DXLoadPRTCompBufferFromFileW
#define PRAGMA_API D3DXLoadSkinMeshFromXof;D3DX9_30!D3DXLoadSkinMeshFromXof
#define PRAGMA_API D3DXLoadSurfaceFromFileA;D3DX9_30!D3DXLoadSurfaceFromFileA
#define PRAGMA_API D3DXLoadSurfaceFromFileInMemory;D3DX9_30!D3DXLoadSurfaceFromFileInMemory
#define PRAGMA_API D3DXLoadSurfaceFromFileW;D3DX9_30!D3DXLoadSurfaceFromFileW
#define PRAGMA_API D3DXLoadSurfaceFromMemory;D3DX9_30!D3DXLoadSurfaceFromMemory
#define PRAGMA_API D3DXLoadSurfaceFromResourceA;D3DX9_30!D3DXLoadSurfaceFromResourceA
#define PRAGMA_API D3DXLoadSurfaceFromResourceW;D3DX9_30!D3DXLoadSurfaceFromResourceW
#define PRAGMA_API D3DXLoadSurfaceFromSurface;D3DX9_30!D3DXLoadSurfaceFromSurface
#define PRAGMA_API D3DXLoadVolumeFromFileA;D3DX9_30!D3DXLoadVolumeFromFileA
#define PRAGMA_API D3DXLoadVolumeFromFileInMemory;D3DX9_30!D3DXLoadVolumeFromFileInMemory
#define PRAGMA_API D3DXLoadVolumeFromFileW;D3DX9_30!D3DXLoadVolumeFromFileW
#define PRAGMA_API D3DXLoadVolumeFromMemory;D3DX9_30!D3DXLoadVolumeFromMemory
#define PRAGMA_API D3DXLoadVolumeFromResourceA;D3DX9_30!D3DXLoadVolumeFromResourceA
#define PRAGMA_API D3DXLoadVolumeFromResourceW;D3DX9_30!D3DXLoadVolumeFromResourceW
#define PRAGMA_API D3DXLoadVolumeFromVolume;D3DX9_30!D3DXLoadVolumeFromVolume

// Matrix //////////////////////////////////////////////////////////////

D3DXMATRIX* D3DXMatrixIdentity
    ( D3DXMATRIX *pOut )
{
    float* m = (float*)pOut; // in case the matrix is defined as a struct (_11..) 
    m[0+0] = 1.0; m[0+1] = 0.0; m[0+2] = 0.0; m[0+3] = 0.0;
    m[4+0] = 0.0; m[4+1] = 1.0; m[4+2] = 0.0; m[4+3] = 0.0;
    m[8+0] = 0.0; m[8+1] = 0.0; m[8+2] = 1.0; m[8+3] = 0.0;
    m[12+0] = 0.0; m[12+1] = 0.0; m[12+2] = 0.0; m[12+3] = 1.0;
    return pOut;
}


BOOL D3DXMatrixIsIdentity
    ( CONST D3DXMATRIX *pM )
{
    float* m = (float*)pM; // in case the matrix is defined as a struct (_11..) 
    return 
        m[0+0] == 1.0 && m[0+1] == 0.0 && m[0+2] == 0.0 && m[0+3] == 0.0 &&
        m[4+0] == 0.0 && m[4+1] == 1.0 && m[4+2] == 0.0 && m[4+3] == 0.0 &&
        m[8+0] == 0.0 && m[8+1] == 0.0 && m[8+2] == 1.0 && m[8+3] == 0.0 &&
        m[12+0] == 0.0 && m[12+1] == 0.0 && m[12+2] == 0.0 && m[12+3] == 1.0;
}


FLOAT WINAPI D3DXMatrixDeterminant
    ( CONST D3DXMATRIX *pM );

HRESULT WINAPI D3DXMatrixDecompose
    ( D3DXVECTOR3 *pOutScale, D3DXQUATERNION *pOutRotation, 
	  D3DXVECTOR3 *pOutTranslation, CONST D3DXMATRIX *pM );

D3DXMATRIX* WINAPI D3DXMatrixTranspose
    ( D3DXMATRIX *pOut, CONST D3DXMATRIX *pM );

// Matrix multiplication.  The result represents the transformation M2
// followed by the transformation M1.  (Out = M1 * M2)
D3DXMATRIX* WINAPI D3DXMatrixMultiply
    ( D3DXMATRIX *pOut, CONST D3DXMATRIX *pM1, CONST D3DXMATRIX *pM2 );

// Matrix multiplication, followed by a transpose. (Out = T(M1 * M2))
D3DXMATRIX* WINAPI D3DXMatrixMultiplyTranspose
    ( D3DXMATRIX *pOut, CONST D3DXMATRIX *pM1, CONST D3DXMATRIX *pM2 );

// Calculate inverse of matrix.  Inversion my fail, in which case NULL will
// be returned.  The determinant of pM is also returned it pfDeterminant
// is non-NULL.
D3DXMATRIX* WINAPI D3DXMatrixInverse
    ( D3DXMATRIX *pOut, FLOAT *pDeterminant, CONST D3DXMATRIX *pM );

// Build a matrix which scales by (sx, sy, sz)
D3DXMATRIX* WINAPI D3DXMatrixScaling
    ( D3DXMATRIX *pOut, FLOAT sx, FLOAT sy, FLOAT sz );

// Build a matrix which translates by (x, y, z)
D3DXMATRIX* WINAPI D3DXMatrixTranslation
    ( D3DXMATRIX *pOut, FLOAT x, FLOAT y, FLOAT z );

// Build a matrix which rotates around the X axis
D3DXMATRIX* WINAPI D3DXMatrixRotationX
    ( D3DXMATRIX *pOut, FLOAT Angle );

// Build a matrix which rotates around the Y axis
D3DXMATRIX* WINAPI D3DXMatrixRotationY
    ( D3DXMATRIX *pOut, FLOAT Angle );

// Build a matrix which rotates around the Z axis
D3DXMATRIX* WINAPI D3DXMatrixRotationZ
    ( D3DXMATRIX *pOut, FLOAT Angle );

// Build a matrix which rotates around an arbitrary axis
D3DXMATRIX* WINAPI D3DXMatrixRotationAxis
    ( D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pV, FLOAT Angle );

// Build a matrix from a quaternion
D3DXMATRIX* WINAPI D3DXMatrixRotationQuaternion
    ( D3DXMATRIX *pOut, CONST D3DXQUATERNION *pQ);

// Yaw around the Y axis, a pitch around the X axis,
// and a roll around the Z axis.
D3DXMATRIX* WINAPI D3DXMatrixRotationYawPitchRoll
    ( D3DXMATRIX *pOut, FLOAT Yaw, FLOAT Pitch, FLOAT Roll );

// Build transformation matrix.  NULL arguments are treated as identity.
// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
D3DXMATRIX* WINAPI D3DXMatrixTransformation
    ( D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pScalingCenter,
      CONST D3DXQUATERNION *pScalingRotation, CONST D3DXVECTOR3 *pScaling,
      CONST D3DXVECTOR3 *pRotationCenter, CONST D3DXQUATERNION *pRotation,
      CONST D3DXVECTOR3 *pTranslation);

// Build 2D transformation matrix in XY plane.  NULL arguments are treated as identity.
// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
D3DXMATRIX* WINAPI D3DXMatrixTransformation2D
    ( D3DXMATRIX *pOut, CONST D3DXVECTOR2* pScalingCenter, 
      FLOAT ScalingRotation, CONST D3DXVECTOR2* pScaling, 
      CONST D3DXVECTOR2* pRotationCenter, FLOAT Rotation, 
      CONST D3DXVECTOR2* pTranslation);

// Build affine transformation matrix.  NULL arguments are treated as identity.
// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
D3DXMATRIX* WINAPI D3DXMatrixAffineTransformation
    ( D3DXMATRIX *pOut, FLOAT Scaling, CONST D3DXVECTOR3 *pRotationCenter,
      CONST D3DXQUATERNION *pRotation, CONST D3DXVECTOR3 *pTranslation);

// Build 2D affine transformation matrix in XY plane.  NULL arguments are treated as identity.
// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
D3DXMATRIX* WINAPI D3DXMatrixAffineTransformation2D
    ( D3DXMATRIX *pOut, FLOAT Scaling, CONST D3DXVECTOR2* pRotationCenter, 
      FLOAT Rotation, CONST D3DXVECTOR2* pTranslation);

// Build a lookat matrix. (right-handed)
D3DXMATRIX* WINAPI D3DXMatrixLookAtRH
    ( D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye, CONST D3DXVECTOR3 *pAt,
      CONST D3DXVECTOR3 *pUp );

// Build a lookat matrix. (left-handed)
D3DXMATRIX* WINAPI D3DXMatrixLookAtLH
    ( D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye, CONST D3DXVECTOR3 *pAt,
      CONST D3DXVECTOR3 *pUp );

// Build a perspective projection matrix. (right-handed)
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveRH
    ( D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );

// Build a perspective projection matrix. (left-handed)
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveLH
    ( D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );

// Build a perspective projection matrix. (right-handed)
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveFovRH
    ( D3DXMATRIX *pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf );

// Build a perspective projection matrix. (left-handed)
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveFovLH
    ( D3DXMATRIX *pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf );

// Build a perspective projection matrix. (right-handed)
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveOffCenterRH
    ( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
      FLOAT zf );

// Build a perspective projection matrix. (left-handed)
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveOffCenterLH
    ( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
      FLOAT zf );

// Build an ortho projection matrix. (right-handed)
D3DXMATRIX* WINAPI D3DXMatrixOrthoRH
    ( D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );

// Build an ortho projection matrix. (left-handed)
D3DXMATRIX* WINAPI D3DXMatrixOrthoLH
    ( D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );

// Build an ortho projection matrix. (right-handed)
D3DXMATRIX* WINAPI D3DXMatrixOrthoOffCenterRH
    ( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
      FLOAT zf );

// Build an ortho projection matrix. (left-handed)
D3DXMATRIX* WINAPI D3DXMatrixOrthoOffCenterLH
    ( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
      FLOAT zf );

// Build a matrix which flattens geometry into a plane, as if casting
// a shadow from a light.
D3DXMATRIX* WINAPI D3DXMatrixShadow
    ( D3DXMATRIX *pOut, CONST D3DXVECTOR4 *pLight,
      CONST D3DXPLANE *pPlane );

// Build a matrix which reflects the coordinate system about a plane
D3DXMATRIX* WINAPI D3DXMatrixReflect
    ( D3DXMATRIX *pOut, CONST D3DXPLANE *pPlane );

#define PRAGMA_API D3DXMatrixAffineTransformation;D3DX9_30!D3DXMatrixAffineTransformation
#define PRAGMA_API D3DXMatrixAffineTransformation2D;D3DX9_30!D3DXMatrixAffineTransformation2D
#define PRAGMA_API D3DXMatrixDecompose;D3DX9_30!D3DXMatrixDecompose
#define PRAGMA_API D3DXMatrixDeterminant;D3DX9_30!D3DXMatrixDeterminant
#define PRAGMA_API D3DXMatrixInverse;D3DX9_30!D3DXMatrixInverse
#define PRAGMA_API D3DXMatrixLookAtLH;D3DX9_30!D3DXMatrixLookAtLH
#define PRAGMA_API D3DXMatrixLookAtRH;D3DX9_30!D3DXMatrixLookAtRH
#define PRAGMA_API D3DXMatrixMultiply;D3DX9_30!D3DXMatrixMultiply
#define PRAGMA_API D3DXMatrixMultiplyTranspose;D3DX9_30!D3DXMatrixMultiplyTranspose
#define PRAGMA_API D3DXMatrixOrthoLH;D3DX9_30!D3DXMatrixOrthoLH
#define PRAGMA_API D3DXMatrixOrthoOffCenterLH;D3DX9_30!D3DXMatrixOrthoOffCenterLH
#define PRAGMA_API D3DXMatrixOrthoOffCenterRH;D3DX9_30!D3DXMatrixOrthoOffCenterRH
#define PRAGMA_API D3DXMatrixOrthoRH;D3DX9_30!D3DXMatrixOrthoRH
#define PRAGMA_API D3DXMatrixPerspectiveFovLH;D3DX9_30!D3DXMatrixPerspectiveFovLH
#define PRAGMA_API D3DXMatrixPerspectiveFovRH;D3DX9_30!D3DXMatrixPerspectiveFovRH
#define PRAGMA_API D3DXMatrixPerspectiveLH;D3DX9_30!D3DXMatrixPerspectiveLH
#define PRAGMA_API D3DXMatrixPerspectiveOffCenterLH;D3DX9_30!D3DXMatrixPerspectiveOffCenterLH
#define PRAGMA_API D3DXMatrixPerspectiveOffCenterRH;D3DX9_30!D3DXMatrixPerspectiveOffCenterRH
#define PRAGMA_API D3DXMatrixPerspectiveRH;D3DX9_30!D3DXMatrixPerspectiveRH
#define PRAGMA_API D3DXMatrixReflect;D3DX9_30!D3DXMatrixReflect
#define PRAGMA_API D3DXMatrixRotationAxis;D3DX9_30!D3DXMatrixRotationAxis
#define PRAGMA_API D3DXMatrixRotationQuaternion;D3DX9_30!D3DXMatrixRotationQuaternion
#define PRAGMA_API D3DXMatrixRotationX;D3DX9_30!D3DXMatrixRotationX
#define PRAGMA_API D3DXMatrixRotationY;D3DX9_30!D3DXMatrixRotationY
#define PRAGMA_API D3DXMatrixRotationYawPitchRoll;D3DX9_30!D3DXMatrixRotationYawPitchRoll
#define PRAGMA_API D3DXMatrixRotationZ;D3DX9_30!D3DXMatrixRotationZ
#define PRAGMA_API D3DXMatrixScaling;D3DX9_30!D3DXMatrixScaling
#define PRAGMA_API D3DXMatrixShadow;D3DX9_30!D3DXMatrixShadow
#define PRAGMA_API D3DXMatrixTransformation;D3DX9_30!D3DXMatrixTransformation
#define PRAGMA_API D3DXMatrixTransformation2D;D3DX9_30!D3DXMatrixTransformation2D
#define PRAGMA_API D3DXMatrixTranslation;D3DX9_30!D3DXMatrixTranslation
#define PRAGMA_API D3DXMatrixTranspose;D3DX9_30!D3DXMatrixTranspose

#define PRAGMA_API D3DXOptimizeFaces;D3DX9_30!D3DXOptimizeFaces
#define PRAGMA_API D3DXOptimizeVertices;D3DX9_30!D3DXOptimizeVertices

// Plane ///////////////////////////////////////////////////////////////////

// Normalize plane (so that |a,b,c| == 1)
D3DXPLANE* WINAPI D3DXPlaneNormalize
    ( D3DXPLANE *pOut, CONST D3DXPLANE *pP);

// Find the intersection between a plane and a line.  If the line is
// parallel to the plane, NULL is returned.
D3DXVECTOR3* WINAPI D3DXPlaneIntersectLine
    ( D3DXVECTOR3 *pOut, CONST D3DXPLANE *pP, CONST D3DXVECTOR3 *pV1,
      CONST D3DXVECTOR3 *pV2);

// Construct a plane from a point and a normal
D3DXPLANE* WINAPI D3DXPlaneFromPointNormal
    ( D3DXPLANE *pOut, CONST D3DXVECTOR3 *pPoint, CONST D3DXVECTOR3 *pNormal);

// Construct a plane from 3 points
D3DXPLANE* WINAPI D3DXPlaneFromPoints
    ( D3DXPLANE *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2,
      CONST D3DXVECTOR3 *pV3);

// Transform a plane by a matrix.  The vector (a,b,c) must be normal.
// M should be the inverse transpose of the transformation desired.
D3DXPLANE* WINAPI D3DXPlaneTransform
    ( D3DXPLANE *pOut, CONST D3DXPLANE *pP, CONST D3DXMATRIX *pM );
    
// Transform an array of planes by a matrix.  The vectors (a,b,c) must be normal.
// M should be the inverse transpose of the transformation desired.
D3DXPLANE* WINAPI D3DXPlaneTransformArray
    ( D3DXPLANE *pOut, UINT OutStride, CONST D3DXPLANE *pP, UINT PStride, CONST D3DXMATRIX *pM, UINT n );

#define PRAGMA_API D3DXPlaneFromPointNormal;D3DX9_30!D3DXPlaneFromPointNormal
#define PRAGMA_API D3DXPlaneFromPoints;D3DX9_30!D3DXPlaneFromPoints
#define PRAGMA_API D3DXPlaneIntersectLine;D3DX9_30!D3DXPlaneIntersectLine
#define PRAGMA_API D3DXPlaneNormalize;D3DX9_30!D3DXPlaneNormalize
#define PRAGMA_API D3DXPlaneTransform;D3DX9_30!D3DXPlaneTransform
#define PRAGMA_API D3DXPlaneTransformArray;D3DX9_30!D3DXPlaneTransformArray

// Shader /////////////////////////////////////////////////////////////////

#define PRAGMA_API D3DXAssembleShader;D3DX9_30!D3DXAssembleShader
#define PRAGMA_API D3DXAssembleShaderFromFileA;D3DX9_30!D3DXAssembleShaderFromFileA
#define PRAGMA_API D3DXAssembleShaderFromFileW;D3DX9_30!D3DXAssembleShaderFromFileW
#define PRAGMA_API D3DXAssembleShaderFromResourceA;D3DX9_30!D3DXAssembleShaderFromResourceA
#define PRAGMA_API D3DXAssembleShaderFromResourceW;D3DX9_30!D3DXAssembleShaderFromResourceW
#define PRAGMA_API D3DXDeclaratorFromFVF;D3DX9_30!D3DXDeclaratorFromFVF
#define PRAGMA_API D3DXDisassembleEffect;D3DX9_30!D3DXDisassembleEffect
#define PRAGMA_API D3DXDisassembleShader;D3DX9_30!D3DXDisassembleShader
#define PRAGMA_API D3DXPreprocessShader;D3DX9_30!D3DXPreprocessShader
#define PRAGMA_API D3DXPreprocessShaderFromFileA;D3DX9_30!D3DXPreprocessShaderFromFileA
#define PRAGMA_API D3DXPreprocessShaderFromFileW;D3DX9_30!D3DXPreprocessShaderFromFileW
#define PRAGMA_API D3DXPreprocessShaderFromResourceA;D3DX9_30!D3DXPreprocessShaderFromResourceA
#define PRAGMA_API D3DXPreprocessShaderFromResourceW;D3DX9_30!D3DXPreprocessShaderFromResourceW
#define PRAGMA_API D3DXCreateEffect;D3DX9_30!D3DXCreateEffect
#define PRAGMA_API D3DXCreateEffectCompiler;D3DX9_30!D3DXCreateEffectCompiler
#define PRAGMA_API D3DXCreateEffectCompilerFromFileA;D3DX9_30!D3DXCreateEffectCompilerFromFileA
#define PRAGMA_API D3DXCreateEffectCompilerFromFileW;D3DX9_30!D3DXCreateEffectCompilerFromFileW
#define PRAGMA_API D3DXCreateEffectCompilerFromResourceA;D3DX9_30!D3DXCreateEffectCompilerFromResourceA
#define PRAGMA_API D3DXCreateEffectCompilerFromResourceW;D3DX9_30!D3DXCreateEffectCompilerFromResourceW
#define PRAGMA_API D3DXCreateEffectEx;D3DX9_30!D3DXCreateEffectEx
#define PRAGMA_API D3DXCreateEffectFromFileA;D3DX9_30!D3DXCreateEffectFromFileA
#define PRAGMA_API D3DXCreateEffectFromFileExA;D3DX9_30!D3DXCreateEffectFromFileExA
#define PRAGMA_API D3DXCreateEffectFromFileExW;D3DX9_30!D3DXCreateEffectFromFileExW
#define PRAGMA_API D3DXCreateEffectFromFileW;D3DX9_30!D3DXCreateEffectFromFileW
#define PRAGMA_API D3DXCreateEffectFromResourceA;D3DX9_30!D3DXCreateEffectFromResourceA
#define PRAGMA_API D3DXCreateEffectFromResourceExA;D3DX9_30!D3DXCreateEffectFromResourceExA
#define PRAGMA_API D3DXCreateEffectFromResourceExW;D3DX9_30!D3DXCreateEffectFromResourceExW
#define PRAGMA_API D3DXCreateEffectFromResourceW;D3DX9_30!D3DXCreateEffectFromResourceW
#define PRAGMA_API D3DXCreateEffectPool;D3DX9_30!D3DXCreateEffectPool
#define PRAGMA_API D3DXCreateFontA;D3DX9_30!D3DXCreateFontA
#define PRAGMA_API D3DXCreateFontIndirectA;D3DX9_30!D3DXCreateFontIndirectA
#define PRAGMA_API D3DXCreateFontIndirectW;D3DX9_30!D3DXCreateFontIndirectW
#define PRAGMA_API D3DXCreateFontW;D3DX9_30!D3DXCreateFontW
#define PRAGMA_API D3DXCreateFragmentLinker;D3DX9_30!D3DXCreateFragmentLinker
#define PRAGMA_API D3DXCreateKeyframedAnimationSet;D3DX9_30!D3DXCreateKeyframedAnimationSet
#define PRAGMA_API D3DXCreateLine;D3DX9_30!D3DXCreateLine
#define PRAGMA_API D3DXCreateMatrixStack;D3DX9_30!D3DXCreateMatrixStack
#define PRAGMA_API D3DXCreateMesh;D3DX9_30!D3DXCreateMesh
#define PRAGMA_API D3DXCreateMeshFVF;D3DX9_30!D3DXCreateMeshFVF
#define PRAGMA_API D3DXCreateNPatchMesh;D3DX9_30!D3DXCreateNPatchMesh
#define PRAGMA_API D3DXCreatePatchMesh;D3DX9_30!D3DXCreatePatchMesh
#define PRAGMA_API D3DXCreatePMeshFromStream;D3DX9_30!D3DXCreatePMeshFromStream
#define PRAGMA_API D3DXCreatePolygon;D3DX9_30!D3DXCreatePolygon
#define PRAGMA_API D3DXCreatePRTBuffer;D3DX9_30!D3DXCreatePRTBuffer
#define PRAGMA_API D3DXCreatePRTBufferTex;D3DX9_30!D3DXCreatePRTBufferTex
#define PRAGMA_API D3DXCreatePRTCompBuffer;D3DX9_30!D3DXCreatePRTCompBuffer
#define PRAGMA_API D3DXCreatePRTEngine;D3DX9_30!D3DXCreatePRTEngine
#define PRAGMA_API D3DXCreateRenderToEnvMap;D3DX9_30!D3DXCreateRenderToEnvMap
#define PRAGMA_API D3DXCreateRenderToSurface;D3DX9_30!D3DXCreateRenderToSurface
#define PRAGMA_API D3DXCreateSkinInfo;D3DX9_30!D3DXCreateSkinInfo
#define PRAGMA_API D3DXCreateSkinInfoFromBlendedMesh;D3DX9_30!D3DXCreateSkinInfoFromBlendedMesh
#define PRAGMA_API D3DXCreateSkinInfoFVF;D3DX9_30!D3DXCreateSkinInfoFVF
#define PRAGMA_API D3DXCreateSphere;D3DX9_30!D3DXCreateSphere
#define PRAGMA_API D3DXCreateSPMesh;D3DX9_30!D3DXCreateSPMesh
#define PRAGMA_API D3DXCreateSprite;D3DX9_30!D3DXCreateSprite
#define PRAGMA_API D3DXCreateTeapot;D3DX9_30!D3DXCreateTeapot
#define PRAGMA_API D3DXCreateTextA;D3DX9_30!D3DXCreateTextA

// Quaternion ////////////////////////////////////////////////////////////

// Compute a quaternin's axis and angle of rotation. Expects unit quaternions.
void WINAPI D3DXQuaternionToAxisAngle
    ( CONST D3DXQUATERNION *pQ, D3DXVECTOR3 *pAxis, FLOAT *pAngle );

// Build a quaternion from a rotation matrix.
D3DXQUATERNION* WINAPI D3DXQuaternionRotationMatrix
    ( D3DXQUATERNION *pOut, CONST D3DXMATRIX *pM);

// Rotation about arbitrary axis.
D3DXQUATERNION* WINAPI D3DXQuaternionRotationAxis
    ( D3DXQUATERNION *pOut, CONST D3DXVECTOR3 *pV, FLOAT Angle );

// Yaw around the Y axis, a pitch around the X axis,
// and a roll around the Z axis.
D3DXQUATERNION* WINAPI D3DXQuaternionRotationYawPitchRoll
    ( D3DXQUATERNION *pOut, FLOAT Yaw, FLOAT Pitch, FLOAT Roll );

// Quaternion multiplication.  The result represents the rotation Q2
// followed by the rotation Q1.  (Out = Q2 * Q1)
D3DXQUATERNION* WINAPI D3DXQuaternionMultiply
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ1,
      CONST D3DXQUATERNION *pQ2 );

D3DXQUATERNION* WINAPI D3DXQuaternionNormalize
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ );

// Conjugate and re-norm
D3DXQUATERNION* WINAPI D3DXQuaternionInverse
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ );

// Expects unit quaternions.
// if q = (cos(theta), sin(theta) * v); ln(q) = (0, theta * v)
D3DXQUATERNION* WINAPI D3DXQuaternionLn
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ );

// Expects pure quaternions. (w == 0)  w is ignored in calculation.
// if q = (0, theta * v); exp(q) = (cos(theta), sin(theta) * v)
D3DXQUATERNION* WINAPI D3DXQuaternionExp
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ );
      
// Spherical linear interpolation between Q1 (t == 0) and Q2 (t == 1).
// Expects unit quaternions.
D3DXQUATERNION* WINAPI D3DXQuaternionSlerp
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ1,
      CONST D3DXQUATERNION *pQ2, FLOAT t );

// Spherical quadrangle interpolation.
// Slerp(Slerp(Q1, C, t), Slerp(A, B, t), 2t(1-t))
D3DXQUATERNION* WINAPI D3DXQuaternionSquad
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ1,
      CONST D3DXQUATERNION *pA, CONST D3DXQUATERNION *pB,
      CONST D3DXQUATERNION *pC, FLOAT t );

// Setup control points for spherical quadrangle interpolation
// from Q1 to Q2.  The control points are chosen in such a way 
// to ensure the continuity of tangents with adjacent segments.
void WINAPI D3DXQuaternionSquadSetup
    ( D3DXQUATERNION *pAOut, D3DXQUATERNION *pBOut, D3DXQUATERNION *pCOut,
      CONST D3DXQUATERNION *pQ0, CONST D3DXQUATERNION *pQ1, 
      CONST D3DXQUATERNION *pQ2, CONST D3DXQUATERNION *pQ3 );

// Barycentric interpolation.
// Slerp(Slerp(Q1, Q2, f+g), Slerp(Q1, Q3, f+g), g/(f+g))
D3DXQUATERNION* WINAPI D3DXQuaternionBaryCentric
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ1,
      CONST D3DXQUATERNION *pQ2, CONST D3DXQUATERNION *pQ3,
      FLOAT f, FLOAT g );

#define PRAGMA_API D3DXQuaternionBaryCentric;D3DX9_30!D3DXQuaternionBaryCentric
#define PRAGMA_API D3DXQuaternionExp;D3DX9_30!D3DXQuaternionExp
#define PRAGMA_API D3DXQuaternionInverse;D3DX9_30!D3DXQuaternionInverse
#define PRAGMA_API D3DXQuaternionLn;D3DX9_30!D3DXQuaternionLn
#define PRAGMA_API D3DXQuaternionMultiply;D3DX9_30!D3DXQuaternionMultiply
#define PRAGMA_API D3DXQuaternionNormalize;D3DX9_30!D3DXQuaternionNormalize
#define PRAGMA_API D3DXQuaternionRotationAxis;D3DX9_30!D3DXQuaternionRotationAxis
#define PRAGMA_API D3DXQuaternionRotationMatrix;D3DX9_30!D3DXQuaternionRotationMatrix
#define PRAGMA_API D3DXQuaternionRotationYawPitchRoll;D3DX9_30!D3DXQuaternionRotationYawPitchRoll
#define PRAGMA_API D3DXQuaternionSlerp;D3DX9_30!D3DXQuaternionSlerp
#define PRAGMA_API D3DXQuaternionSquad;D3DX9_30!D3DXQuaternionSquad
#define PRAGMA_API D3DXQuaternionSquadSetup;D3DX9_30!D3DXQuaternionSquadSetup
#define PRAGMA_API D3DXQuaternionToAxisAngle;D3DX9_30!D3DXQuaternionToAxisAngle
#define PRAGMA_API D3DXRectPatchSize;D3DX9_30!D3DXRectPatchSize
#define PRAGMA_API D3DXSaveMeshHierarchyToFileA;D3DX9_30!D3DXSaveMeshHierarchyToFileA
#define PRAGMA_API D3DXSaveMeshHierarchyToFileW;D3DX9_30!D3DXSaveMeshHierarchyToFileW
#define PRAGMA_API D3DXSaveMeshToXA;D3DX9_30!D3DXSaveMeshToXA
#define PRAGMA_API D3DXSaveMeshToXW;D3DX9_30!D3DXSaveMeshToXW
#define PRAGMA_API D3DXSavePRTBufferToFileA;D3DX9_30!D3DXSavePRTBufferToFileA
#define PRAGMA_API D3DXSavePRTBufferToFileW;D3DX9_30!D3DXSavePRTBufferToFileW
#define PRAGMA_API D3DXSavePRTCompBufferToFileA;D3DX9_30!D3DXSavePRTCompBufferToFileA
#define PRAGMA_API D3DXSavePRTCompBufferToFileW;D3DX9_30!D3DXSavePRTCompBufferToFileW
#define PRAGMA_API D3DXSaveSurfaceToFileA;D3DX9_30!D3DXSaveSurfaceToFileA
#define PRAGMA_API D3DXSaveSurfaceToFileInMemory;D3DX9_30!D3DXSaveSurfaceToFileInMemory
#define PRAGMA_API D3DXSaveSurfaceToFileW;D3DX9_30!D3DXSaveSurfaceToFileW
#define PRAGMA_API D3DXSaveTextureToFileA;D3DX9_30!D3DXSaveTextureToFileA
#define PRAGMA_API D3DXSaveTextureToFileInMemory;D3DX9_30!D3DXSaveTextureToFileInMemory
#define PRAGMA_API D3DXSaveTextureToFileW;D3DX9_30!D3DXSaveTextureToFileW
#define PRAGMA_API D3DXSaveVolumeToFileA;D3DX9_30!D3DXSaveVolumeToFileA
#define PRAGMA_API D3DXSaveVolumeToFileInMemory;D3DX9_30!D3DXSaveVolumeToFileInMemory
#define PRAGMA_API D3DXSaveVolumeToFileW;D3DX9_30!D3DXSaveVolumeToFileW
#define PRAGMA_API D3DXSHAdd;D3DX9_30!D3DXSHAdd
#define PRAGMA_API D3DXSHDot;D3DX9_30!D3DXSHDot
#define PRAGMA_API D3DXSHEvalConeLight;D3DX9_30!D3DXSHEvalConeLight
#define PRAGMA_API D3DXSHEvalDirection;D3DX9_30!D3DXSHEvalDirection
#define PRAGMA_API D3DXSHEvalDirectionalLight;D3DX9_30!D3DXSHEvalDirectionalLight
#define PRAGMA_API D3DXSHEvalHemisphereLight;D3DX9_30!D3DXSHEvalHemisphereLight
#define PRAGMA_API D3DXSHEvalSphericalLight;D3DX9_30!D3DXSHEvalSphericalLight
#define PRAGMA_API D3DXSHProjectCubeMap;D3DX9_30!D3DXSHProjectCubeMap
#define PRAGMA_API D3DXSHPRTCompSplitMeshSC;D3DX9_30!D3DXSHPRTCompSplitMeshSC
#define PRAGMA_API D3DXSHPRTCompSuperCluster;D3DX9_30!D3DXSHPRTCompSuperCluster
#define PRAGMA_API D3DXSHRotate;D3DX9_30!D3DXSHRotate
#define PRAGMA_API D3DXSHRotateZ;D3DX9_30!D3DXSHRotateZ
#define PRAGMA_API D3DXSHScale;D3DX9_30!D3DXSHScale
#define PRAGMA_API D3DXSimplifyMesh;D3DX9_30!D3DXSimplifyMesh
#define PRAGMA_API D3DXSphereBoundProbe;D3DX9_30!D3DXSphereBoundProbe
#define PRAGMA_API D3DXSplitMesh;D3DX9_30!D3DXSplitMesh
#define PRAGMA_API D3DXTessellateNPatches;D3DX9_30!D3DXTessellateNPatches
#define PRAGMA_API D3DXTessellateRectPatch;D3DX9_30!D3DXTessellateRectPatch
#define PRAGMA_API D3DXTessellateTriPatch;D3DX9_30!D3DXTessellateTriPatch
#define PRAGMA_API D3DXTriPatchSize;D3DX9_30!D3DXTriPatchSize
#define PRAGMA_API D3DXUVAtlasCreate;D3DX9_30!D3DXUVAtlasCreate
#define PRAGMA_API D3DXUVAtlasPack;D3DX9_30!D3DXUVAtlasPack
#define PRAGMA_API D3DXUVAtlasPartition;D3DX9_30!D3DXUVAtlasPartition
#define PRAGMA_API D3DXValidMesh;D3DX9_30!D3DXValidMesh
#define PRAGMA_API D3DXValidPatchMesh;D3DX9_30!D3DXValidPatchMesh

// 2D Vector ////////////////////////////////////////////////////////////

D3DXVECTOR2* WINAPI D3DXVec2Normalize
    ( D3DXVECTOR2 *pOut, CONST D3DXVECTOR2 *pV );

// Hermite interpolation between position V1, tangent T1 (when s == 0)
// and position V2, tangent T2 (when s == 1).
D3DXVECTOR2* WINAPI D3DXVec2Hermite
    ( D3DXVECTOR2 *pOut, CONST D3DXVECTOR2 *pV1, CONST D3DXVECTOR2 *pT1,
      CONST D3DXVECTOR2 *pV2, CONST D3DXVECTOR2 *pT2, FLOAT s );

// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
D3DXVECTOR2* WINAPI D3DXVec2CatmullRom
    ( D3DXVECTOR2 *pOut, CONST D3DXVECTOR2 *pV0, CONST D3DXVECTOR2 *pV1,
      CONST D3DXVECTOR2 *pV2, CONST D3DXVECTOR2 *pV3, FLOAT s );

// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
D3DXVECTOR2* WINAPI D3DXVec2BaryCentric
    ( D3DXVECTOR2 *pOut, CONST D3DXVECTOR2 *pV1, CONST D3DXVECTOR2 *pV2,
      CONST D3DXVECTOR2 *pV3, FLOAT f, FLOAT g);

// Transform (x, y, 0, 1) by matrix.
D3DXVECTOR4* WINAPI D3DXVec2Transform
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR2 *pV, CONST D3DXMATRIX *pM );

// Transform (x, y, 0, 1) by matrix, project result back into w=1.
D3DXVECTOR2* WINAPI D3DXVec2TransformCoord
    ( D3DXVECTOR2 *pOut, CONST D3DXVECTOR2 *pV, CONST D3DXMATRIX *pM );

// Transform (x, y, 0, 0) by matrix.
D3DXVECTOR2* WINAPI D3DXVec2TransformNormal
    ( D3DXVECTOR2 *pOut, CONST D3DXVECTOR2 *pV, CONST D3DXMATRIX *pM );
     
// Transform Array (x, y, 0, 1) by matrix.
D3DXVECTOR4* WINAPI D3DXVec2TransformArray
    ( D3DXVECTOR4 *pOut, UINT OutStride, CONST D3DXVECTOR2 *pV, UINT VStride, CONST D3DXMATRIX *pM, UINT n);

// Transform Array (x, y, 0, 1) by matrix, project result back into w=1.
D3DXVECTOR2* WINAPI D3DXVec2TransformCoordArray
    ( D3DXVECTOR2 *pOut, UINT OutStride, CONST D3DXVECTOR2 *pV, UINT VStride, CONST D3DXMATRIX *pM, UINT n );

// Transform Array (x, y, 0, 0) by matrix.
D3DXVECTOR2* WINAPI D3DXVec2TransformNormalArray
    ( D3DXVECTOR2 *pOut, UINT OutStride, CONST D3DXVECTOR2 *pV, UINT VStride, CONST D3DXMATRIX *pM, UINT n );

#define PRAGMA_API D3DXVec2BaryCentric;D3DX9_30!D3DXVec2BaryCentric
#define PRAGMA_API D3DXVec2CatmullRom;D3DX9_30!D3DXVec2CatmullRom
#define PRAGMA_API D3DXVec2Hermite;D3DX9_30!D3DXVec2Hermite
#define PRAGMA_API D3DXVec2Normalize;D3DX9_30!D3DXVec2Normalize
#define PRAGMA_API D3DXVec2Transform;D3DX9_30!D3DXVec2Transform
#define PRAGMA_API D3DXVec2TransformArray;D3DX9_30!D3DXVec2TransformArray
#define PRAGMA_API D3DXVec2TransformCoord;D3DX9_30!D3DXVec2TransformCoord
#define PRAGMA_API D3DXVec2TransformCoordArray;D3DX9_30!D3DXVec2TransformCoordArray
#define PRAGMA_API D3DXVec2TransformNormal;D3DX9_30!D3DXVec2TransformNormal
#define PRAGMA_API D3DXVec2TransformNormalArray;D3DX9_30!D3DXVec2TransformNormalArray

// 3D Vector ////////////////////////////////////////////////////////////

// inline functions

FLOAT D3DXVec3Length
    ( CONST D3DXVECTOR3 *pV )
{
    return (FLOAT) sqrt(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);
}

FLOAT D3DXVec3LengthSq
    ( CONST D3DXVECTOR3 *pV )
{
    return pV->x * pV->x + pV->y * pV->y + pV->z * pV->z;
}

FLOAT D3DXVec3Dot
    ( CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2 )
{
    return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}

D3DXVECTOR3* D3DXVec3Cross
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2 )
{
    D3DXVECTOR3 v;
    v.x = pV1->y * pV2->z - pV1->z * pV2->y;
    v.y = pV1->z * pV2->x - pV1->x * pV2->z;
    v.z = pV1->x * pV2->y - pV1->y * pV2->x;

    memcpy(pOut,&v,sizeof(D3DXVECTOR3));
    return pOut;
}

D3DXVECTOR3* D3DXVec3Add
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2 )
{
    pOut->x = pV1->x + pV2->x;
    pOut->y = pV1->y + pV2->y;
    pOut->z = pV1->z + pV2->z;
    return pOut;
}

D3DXVECTOR3* D3DXVec3Subtract
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2 )
{
    pOut->x = pV1->x - pV2->x;
    pOut->y = pV1->y - pV2->y;
    pOut->z = pV1->z - pV2->z;
    return pOut;
}

/*D3DXVECTOR3* D3DXVec3Minimize
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2 )
{
    pOut->x = pV1->x < pV2->x ? pV1->x : pV2->x;
    pOut->y = pV1->y < pV2->y ? pV1->y : pV2->y;
    pOut->z = pV1->z < pV2->z ? pV1->z : pV2->z;
    return pOut;
}

D3DXVECTOR3* D3DXVec3Maximize
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2 )
{
    pOut->x = pV1->x > pV2->x ? pV1->x : pV2->x;
    pOut->y = pV1->y > pV2->y ? pV1->y : pV2->y;
    pOut->z = pV1->z > pV2->z ? pV1->z : pV2->z;
    return pOut;
}*/

D3DXVECTOR3* D3DXVec3Scale
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV, FLOAT s)
{
    pOut->x = pV->x * s;
    pOut->y = pV->y * s;
    pOut->z = pV->z * s;
    return pOut;
}

D3DXVECTOR3* D3DXVec3Lerp
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2,
      FLOAT s )
{
    pOut->x = pV1->x + s * (pV2->x - pV1->x);
    pOut->y = pV1->y + s * (pV2->y - pV1->y);
    pOut->z = pV1->z + s * (pV2->z - pV1->z);
    return pOut;
}

// added convenience functions

D3DXVECTOR3* D3DXVec3Set(D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pIn)
{
    pOut->x = pIn->x;
    pOut->y = pIn->y;
    pOut->z = pIn->z;
    return pOut;
}

D3DXVECTOR3* D3DXVec3Set(D3DXVECTOR3 *pOut, CONST D3DXVECTOR4 *pIn)
{
    pOut->x = pIn->x;
    pOut->y = pIn->y;
    pOut->z = pIn->z;
    return pOut;
}

D3DXVECTOR3* D3DXVec3Set(D3DXVECTOR3 *pOut,float x,float y,float z)
{
    pOut->x = x;
    pOut->y = y;
    pOut->z = z;
    return pOut;
}

// library functions

D3DXVECTOR3* WINAPI D3DXVec3Normalize
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV );

// Hermite interpolation between position V1, tangent T1 (when s == 0)
// and position V2, tangent T2 (when s == 1).
D3DXVECTOR3* WINAPI D3DXVec3Hermite
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pT1,
      CONST D3DXVECTOR3 *pV2, CONST D3DXVECTOR3 *pT2, FLOAT s );

// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
D3DXVECTOR3* WINAPI D3DXVec3CatmullRom
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV0, CONST D3DXVECTOR3 *pV1,
      CONST D3DXVECTOR3 *pV2, CONST D3DXVECTOR3 *pV3, FLOAT s );

// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
D3DXVECTOR3* WINAPI D3DXVec3BaryCentric
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2,
      CONST D3DXVECTOR3 *pV3, FLOAT f, FLOAT g);

// Transform (x, y, z, 1) by matrix.
D3DXVECTOR4* WINAPI D3DXVec3Transform
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR3 *pV, CONST D3DXMATRIX *pM );

// Transform (x, y, z, 1) by matrix, project result back into w=1.
D3DXVECTOR3* WINAPI D3DXVec3TransformCoord
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV, CONST D3DXMATRIX *pM );

// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
// non-affine matrix, the matrix you pass to this function should be the 
// transpose of the inverse of the matrix you would use to transform a coord.
D3DXVECTOR3* WINAPI D3DXVec3TransformNormal
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV, CONST D3DXMATRIX *pM );
    
// Transform Array (x, y, z, 1) by matrix. 
D3DXVECTOR4* WINAPI D3DXVec3TransformArray
    ( D3DXVECTOR4 *pOut, UINT OutStride, CONST D3DXVECTOR3 *pV, UINT VStride, CONST D3DXMATRIX *pM, UINT n );

// Transform Array (x, y, z, 1) by matrix, project result back into w=1.
D3DXVECTOR3* WINAPI D3DXVec3TransformCoordArray
    ( D3DXVECTOR3 *pOut, UINT OutStride, CONST D3DXVECTOR3 *pV, UINT VStride, CONST D3DXMATRIX *pM, UINT n );

// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
// non-affine matrix, the matrix you pass to this function should be the 
// transpose of the inverse of the matrix you would use to transform a coord.
D3DXVECTOR3* WINAPI D3DXVec3TransformNormalArray
    ( D3DXVECTOR3 *pOut, UINT OutStride, CONST D3DXVECTOR3 *pV, UINT VStride, CONST D3DXMATRIX *pM, UINT n );

// Project vector from object space into screen space
D3DXVECTOR3* WINAPI D3DXVec3Project
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV, CONST D3DVIEWPORT9 *pViewport,
      CONST D3DXMATRIX *pProjection, CONST D3DXMATRIX *pView, CONST D3DXMATRIX *pWorld);

// Project vector from screen space into object space
D3DXVECTOR3* WINAPI D3DXVec3Unproject
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV, CONST D3DVIEWPORT9 *pViewport,
      CONST D3DXMATRIX *pProjection, CONST D3DXMATRIX *pView, CONST D3DXMATRIX *pWorld);
      
// Project vector Array from object space into screen space
D3DXVECTOR3* WINAPI D3DXVec3ProjectArray
    ( D3DXVECTOR3 *pOut, UINT OutStride,CONST D3DXVECTOR3 *pV, UINT VStride,CONST D3DVIEWPORT9 *pViewport,
      CONST D3DXMATRIX *pProjection, CONST D3DXMATRIX *pView, CONST D3DXMATRIX *pWorld, UINT n);

// Project vector Array from screen space into object space
D3DXVECTOR3* WINAPI D3DXVec3UnprojectArray
    ( D3DXVECTOR3 *pOut, UINT OutStride, CONST D3DXVECTOR3 *pV, UINT VStride, CONST D3DVIEWPORT9 *pViewport,
      CONST D3DXMATRIX *pProjection, CONST D3DXMATRIX *pView, CONST D3DXMATRIX *pWorld, UINT n);

#define PRAGMA_API D3DXVec3BaryCentric;D3DX9_30!D3DXVec3BaryCentric
#define PRAGMA_API D3DXVec3CatmullRom;D3DX9_30!D3DXVec3CatmullRom
#define PRAGMA_API D3DXVec3Hermite;D3DX9_30!D3DXVec3Hermite
#define PRAGMA_API D3DXVec3Normalize;D3DX9_30!D3DXVec3Normalize
#define PRAGMA_API D3DXVec3Project;D3DX9_30!D3DXVec3Project
#define PRAGMA_API D3DXVec3ProjectArray;D3DX9_30!D3DXVec3ProjectArray
#define PRAGMA_API D3DXVec3Transform;D3DX9_30!D3DXVec3Transform
#define PRAGMA_API D3DXVec3TransformArray;D3DX9_30!D3DXVec3TransformArray
#define PRAGMA_API D3DXVec3TransformCoord;D3DX9_30!D3DXVec3TransformCoord
#define PRAGMA_API D3DXVec3TransformCoordArray;D3DX9_30!D3DXVec3TransformCoordArray
#define PRAGMA_API D3DXVec3TransformNormal;D3DX9_30!D3DXVec3TransformNormal
#define PRAGMA_API D3DXVec3TransformNormalArray;D3DX9_30!D3DXVec3TransformNormalArray
#define PRAGMA_API D3DXVec3Unproject;D3DX9_30!D3DXVec3Unproject
#define PRAGMA_API D3DXVec3UnprojectArray;D3DX9_30!D3DXVec3UnprojectArray

// 4D Vector ////////////////////////////////////////////////////////////

FLOAT D3DXVec4Length
    ( CONST D3DXVECTOR4 *pV )
{
    return (FLOAT) sqrt(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z + pV->w * pV->w);
}

FLOAT D3DXVec4LengthSq
    ( CONST D3DXVECTOR4 *pV )
{
    return pV->x * pV->x + pV->y * pV->y + pV->z * pV->z + pV->w * pV->w;
}

FLOAT D3DXVec4Dot
    ( CONST D3DXVECTOR4 *pV1, CONST D3DXVECTOR4 *pV2 )
{
    return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z + pV1->w * pV2->w;
}

D3DXVECTOR4* D3DXVec4Add
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV1, CONST D3DXVECTOR4 *pV2)
{
    pOut->x = pV1->x + pV2->x;
    pOut->y = pV1->y + pV2->y;
    pOut->z = pV1->z + pV2->z;
    pOut->w = pV1->w + pV2->w;
    return pOut;
}

D3DXVECTOR4* D3DXVec4Subtract
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV1, CONST D3DXVECTOR4 *pV2)
{
    pOut->x = pV1->x - pV2->x;
    pOut->y = pV1->y - pV2->y;
    pOut->z = pV1->z - pV2->z;
    pOut->w = pV1->w - pV2->w;
    return pOut;
}

/*D3DXVECTOR4* D3DXVec4Minimize
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV1, CONST D3DXVECTOR4 *pV2)
{
    pOut->x = pV1->x < pV2->x ? pV1->x : pV2->x;
    pOut->y = pV1->y < pV2->y ? pV1->y : pV2->y;
    pOut->z = pV1->z < pV2->z ? pV1->z : pV2->z;
    pOut->w = pV1->w < pV2->w ? pV1->w : pV2->w;
    return pOut;
}

D3DXVECTOR4* D3DXVec4Maximize
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV1, CONST D3DXVECTOR4 *pV2)
{
    pOut->x = pV1->x > pV2->x ? pV1->x : pV2->x;
    pOut->y = pV1->y > pV2->y ? pV1->y : pV2->y;
    pOut->z = pV1->z > pV2->z ? pV1->z : pV2->z;
    pOut->w = pV1->w > pV2->w ? pV1->w : pV2->w;
    return pOut;
}*/

D3DXVECTOR4* D3DXVec4Scale
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV, FLOAT s)
{
    pOut->x = pV->x * s;
    pOut->y = pV->y * s;
    pOut->z = pV->z * s;
    pOut->w = pV->w * s;
    return pOut;
}

D3DXVECTOR4* D3DXVec4Lerp
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV1, CONST D3DXVECTOR4 *pV2,
      FLOAT s )
{
    pOut->x = pV1->x + s * (pV2->x - pV1->x);
    pOut->y = pV1->y + s * (pV2->y - pV1->y);
    pOut->z = pV1->z + s * (pV2->z - pV1->z);
    pOut->w = pV1->w + s * (pV2->w - pV1->w);
    return pOut;
}

// Convenience functions
D3DXVECTOR4* D3DXVec4Set(D3DXVECTOR4 *pOut, CONST D3DXVECTOR3 *pIn)
{
    pOut->x = pIn->x;
    pOut->y = pIn->y;
    pOut->z = pIn->z;
    pOut->w = 1.0;
    return pOut;
}

D3DXVECTOR4* D3DXVec4Set(D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pIn)
{
    pOut->x = pIn->x;
    pOut->y = pIn->y;
    pOut->z = pIn->z;
    pOut->w = pIn->w;
    return pOut;
}

D3DXVECTOR4* D3DXVec4Set(D3DXVECTOR4 *pOut,float x,float y,float z,float w)
{
    pOut->x = x;
    pOut->y = y;
    pOut->z = z;
    pOut->w = w;
    return pOut;
}

// Cross-product in 4 dimensions.
D3DXVECTOR4* WINAPI D3DXVec4Cross
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV1, CONST D3DXVECTOR4 *pV2,
      CONST D3DXVECTOR4 *pV3);

D3DXVECTOR4* WINAPI D3DXVec4Normalize
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV );

// Hermite interpolation between position V1, tangent T1 (when s == 0)
// and position V2, tangent T2 (when s == 1).
D3DXVECTOR4* WINAPI D3DXVec4Hermite
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV1, CONST D3DXVECTOR4 *pT1,
      CONST D3DXVECTOR4 *pV2, CONST D3DXVECTOR4 *pT2, FLOAT s );

// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
D3DXVECTOR4* WINAPI D3DXVec4CatmullRom
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV0, CONST D3DXVECTOR4 *pV1,
      CONST D3DXVECTOR4 *pV2, CONST D3DXVECTOR4 *pV3, FLOAT s );

// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
D3DXVECTOR4* WINAPI D3DXVec4BaryCentric
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV1, CONST D3DXVECTOR4 *pV2,
      CONST D3DXVECTOR4 *pV3, FLOAT f, FLOAT g);

// Transform vector by matrix.
D3DXVECTOR4* WINAPI D3DXVec4Transform
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV, CONST D3DXMATRIX *pM );
    
// Transform vector array by matrix.
D3DXVECTOR4* WINAPI D3DXVec4TransformArray
    ( D3DXVECTOR4 *pOut, UINT OutStride, CONST D3DXVECTOR4 *pV, UINT VStride, CONST D3DXMATRIX *pM, UINT n );

#define PRAGMA_API D3DXVec4BaryCentric;D3DX9_30!D3DXVec4BaryCentric
#define PRAGMA_API D3DXVec4CatmullRom;D3DX9_30!D3DXVec4CatmullRom
#define PRAGMA_API D3DXVec4Cross;D3DX9_30!D3DXVec4Cross
#define PRAGMA_API D3DXVec4Hermite;D3DX9_30!D3DXVec4Hermite
#define PRAGMA_API D3DXVec4Normalize;D3DX9_30!D3DXVec4Normalize
#define PRAGMA_API D3DXVec4Transform;D3DX9_30!D3DXVec4Transform
#define PRAGMA_API D3DXVec4TransformArray;D3DX9_30!D3DXVec4TransformArray

#define PRAGMA_API D3DXWeldVertices;D3DX9_30!D3DXWeldVertices

#endif //d3d9_h
