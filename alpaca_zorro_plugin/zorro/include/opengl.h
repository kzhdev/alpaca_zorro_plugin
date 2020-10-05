#ifndef opengl_h
#define opengl_h

#include <windows.h>

//OpenGL.H
// AccumOp 
#define GL_ACCUM                          0x0100
#define GL_LOAD                           0x0101
#define GL_RETURN                         0x0102
#define GL_MULT                           0x0103
#define GL_ADD                            0x0104
// AlphaFunction 
#define GL_NEVER                          0x0200
#define GL_LESS                           0x0201
#define GL_EQUAL                          0x0202
#define GL_LEQUAL                         0x0203
#define GL_GREATER                        0x0204
#define GL_NOTEQUAL                       0x0205
#define GL_GEQUAL                         0x0206
#define GL_ALWAYS                         0x0207
// AttribMask 
#define GL_CURRENT_BIT                    0x00000001
#define GL_POINT_BIT                      0x00000002
#define GL_LINE_BIT                       0x00000004
#define GL_POLYGON_BIT                    0x00000008
#define GL_POLYGON_STIPPLE_BIT            0x00000010
#define GL_PIXEL_MODE_BIT                 0x00000020
#define GL_LIGHTING_BIT                   0x00000040
#define GL_FOG_BIT                        0x00000080
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_ACCUM_BUFFER_BIT               0x00000200
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_VIEWPORT_BIT                   0x00000800
#define GL_TRANSFORM_BIT                  0x00001000
#define GL_ENABLE_BIT                     0x00002000
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_HINT_BIT                       0x00008000
#define GL_EVAL_BIT                       0x00010000
#define GL_LIST_BIT                       0x00020000
#define GL_TEXTURE_BIT                    0x00040000
#define GL_SCISSOR_BIT                    0x00080000
#define GL_ALL_ATTRIB_BITS                0x000fffff
// BeginMode 
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_QUADS                          0x0007
#define GL_QUAD_STRIP                     0x0008
#define GL_POLYGON                        0x0009
// BlendingFactorDest 
#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_SRC_COLOR                      0x0300
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305
// BlendingFactorSrc 
//      GL_ZERO 
//      GL_ONE 
#define GL_DST_COLOR                      0x0306
#define GL_ONE_MINUS_DST_COLOR            0x0307
#define GL_SRC_ALPHA_SATURATE             0x0308
// Boolean 
#define GL_TRUE                           1
#define GL_FALSE                          0
// ClipPlaneName 
#define GL_CLIP_PLANE0                    0x3000
#define GL_CLIP_PLANE1                    0x3001
#define GL_CLIP_PLANE2                    0x3002
#define GL_CLIP_PLANE3                    0x3003
#define GL_CLIP_PLANE4                    0x3004
#define GL_CLIP_PLANE5                    0x3005
// DataType 
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_2_BYTES                        0x1407
#define GL_3_BYTES                        0x1408
#define GL_4_BYTES                        0x1409
#define GL_DOUBLE                         0x140A
//      GL_ALWAYS 
// DrawBufferMode 
#define GL_NONE                           0
#define GL_FRONT_LEFT                     0x0400
#define GL_FRONT_RIGHT                    0x0401
#define GL_BACK_LEFT                      0x0402
#define GL_BACK_RIGHT                     0x0403
#define GL_FRONT                          0x0404
#define GL_BACK                           0x0405
#define GL_LEFT                           0x0406
#define GL_RIGHT                          0x0407
#define GL_FRONT_AND_BACK                 0x0408
#define GL_AUX0                           0x0409
#define GL_AUX1                           0x040A
#define GL_AUX2                           0x040B
#define GL_AUX3                           0x040C

// ErrorCode 
#define GL_NO_ERROR                       0
#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_STACK_OVERFLOW                 0x0503
#define GL_STACK_UNDERFLOW                0x0504
#define GL_OUT_OF_MEMORY                  0x0505

// FeedBackMode 
#define GL_2D                             0x0600
#define GL_3D                             0x0601
#define GL_3D_COLOR                       0x0602
#define GL_3D_COLOR_TEXTURE               0x0603
#define GL_4D_COLOR_TEXTURE               0x0604

// FeedBackToken 
#define GL_PASS_THROUGH_TOKEN             0x0700
#define GL_POINT_TOKEN                    0x0701
#define GL_LINE_TOKEN                     0x0702
#define GL_POLYGON_TOKEN                  0x0703
#define GL_BITMAP_TOKEN                   0x0704
#define GL_DRAW_PIXEL_TOKEN               0x0705
#define GL_COPY_PIXEL_TOKEN               0x0706
#define GL_LINE_RESET_TOKEN               0x0707

// FogMode 
//      GL_LINEAR 
#define GL_EXP                            0x0800
#define GL_EXP2                           0x0801
// FrontFaceDirection 
#define GL_CW                             0x0900
#define GL_CCW                            0x0901

// GetMapTarget 
#define GL_COEFF                          0x0A00
#define GL_ORDER                          0x0A01
#define GL_DOMAIN                         0x0A02
// GetTarget 
#define GL_CURRENT_COLOR                  0x0B00
#define GL_CURRENT_INDEX                  0x0B01
#define GL_CURRENT_NORMAL                 0x0B02
#define GL_CURRENT_TEXTURE_COORDS         0x0B03
#define GL_CURRENT_RASTER_COLOR           0x0B04
#define GL_CURRENT_RASTER_INDEX           0x0B05
#define GL_CURRENT_RASTER_TEXTURE_COORDS  0x0B06
#define GL_CURRENT_RASTER_POSITION        0x0B07
#define GL_CURRENT_RASTER_POSITION_VALID  0x0B08
#define GL_CURRENT_RASTER_DISTANCE        0x0B09
#define GL_POINT_SMOOTH                   0x0B10
#define GL_POINT_SIZE                     0x0B11
#define GL_POINT_SIZE_RANGE               0x0B12
#define GL_POINT_SIZE_GRANULARITY         0x0B13
#define GL_LINE_SMOOTH                    0x0B20
#define GL_LINE_WIDTH                     0x0B21
#define GL_LINE_WIDTH_RANGE               0x0B22
#define GL_LINE_WIDTH_GRANULARITY         0x0B23
#define GL_LINE_STIPPLE                   0x0B24
#define GL_LINE_STIPPLE_PATTERN           0x0B25
#define GL_LINE_STIPPLE_REPEAT            0x0B26
#define GL_LIST_MODE                      0x0B30
#define GL_MAX_LIST_NESTING               0x0B31
#define GL_LIST_BASE                      0x0B32
#define GL_LIST_INDEX                     0x0B33
#define GL_POLYGON_MODE                   0x0B40
#define GL_POLYGON_SMOOTH                 0x0B41
#define GL_POLYGON_STIPPLE                0x0B42
#define GL_EDGE_FLAG                      0x0B43
#define GL_CULL_FACE                      0x0B44
#define GL_CULL_FACE_MODE                 0x0B45
#define GL_FRONT_FACE                     0x0B46
#define GL_LIGHTING                       0x0B50
#define GL_LIGHT_MODEL_LOCAL_VIEWER       0x0B51
#define GL_LIGHT_MODEL_TWO_SIDE           0x0B52
#define GL_LIGHT_MODEL_AMBIENT            0x0B53
#define GL_SHADE_MODEL                    0x0B54
#define GL_COLOR_MATERIAL_FACE            0x0B55
#define GL_COLOR_MATERIAL_PARAMETER       0x0B56
#define GL_COLOR_MATERIAL                 0x0B57
#define GL_FOG                            0x0B60
#define GL_FOG_INDEX                      0x0B61
#define GL_FOG_DENSITY                    0x0B62
#define GL_FOG_START                      0x0B63
#define GL_FOG_END                        0x0B64
#define GL_FOG_MODE                       0x0B65
#define GL_FOG_COLOR                      0x0B66
#define GL_DEPTH_RANGE                    0x0B70
#define GL_DEPTH_TEST                     0x0B71
#define GL_DEPTH_WRITEMASK                0x0B72
#define GL_DEPTH_CLEAR_VALUE              0x0B73
#define GL_DEPTH_FUNC                     0x0B74
#define GL_ACCUM_CLEAR_VALUE              0x0B80
#define GL_STENCIL_TEST                   0x0B90
#define GL_STENCIL_CLEAR_VALUE            0x0B91
#define GL_STENCIL_FUNC                   0x0B92
#define GL_STENCIL_VALUE_MASK             0x0B93
#define GL_STENCIL_FAIL                   0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL        0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS        0x0B96
#define GL_STENCIL_REF                    0x0B97
#define GL_STENCIL_WRITEMASK              0x0B98
#define GL_MATRIX_MODE                    0x0BA0
#define GL_NORMALIZE                      0x0BA1
#define GL_VIEWPORT                       0x0BA2
#define GL_MODELVIEW_STACK_DEPTH          0x0BA3
#define GL_PROJECTION_STACK_DEPTH         0x0BA4
#define GL_TEXTURE_STACK_DEPTH            0x0BA5
#define GL_MODELVIEW_MATRIX               0x0BA6
#define GL_PROJECTION_MATRIX              0x0BA7
#define GL_TEXTURE_MATRIX                 0x0BA8
#define GL_ATTRIB_STACK_DEPTH             0x0BB0
#define GL_CLIENT_ATTRIB_STACK_DEPTH      0x0BB1
#define GL_ALPHA_TEST                     0x0BC0
#define GL_ALPHA_TEST_FUNC                0x0BC1
#define GL_ALPHA_TEST_REF                 0x0BC2
#define GL_DITHER                         0x0BD0
#define GL_BLEND_DST                      0x0BE0
#define GL_BLEND_SRC                      0x0BE1
#define GL_BLEND                          0x0BE2
#define GL_LOGIC_OP_MODE                  0x0BF0
#define GL_INDEX_LOGIC_OP                 0x0BF1
#define GL_COLOR_LOGIC_OP                 0x0BF2
#define GL_AUX_BUFFERS                    0x0C00
#define GL_DRAW_BUFFER                    0x0C01
#define GL_READ_BUFFER                    0x0C02
#define GL_SCISSOR_BOX                    0x0C10
#define GL_SCISSOR_TEST                   0x0C11
#define GL_INDEX_CLEAR_VALUE              0x0C20
#define GL_INDEX_WRITEMASK                0x0C21
#define GL_COLOR_CLEAR_VALUE              0x0C22
#define GL_COLOR_WRITEMASK                0x0C23
#define GL_INDEX_MODE                     0x0C30
#define GL_RGBA_MODE                      0x0C31
#define GL_DOUBLEBUFFER                   0x0C32
#define GL_STEREO                         0x0C33
#define GL_RENDER_MODE                    0x0C40
#define GL_PERSPECTIVE_CORRECTION_HINT    0x0C50
#define GL_POINT_SMOOTH_HINT              0x0C51
#define GL_LINE_SMOOTH_HINT               0x0C52
#define GL_POLYGON_SMOOTH_HINT            0x0C53
#define GL_FOG_HINT                       0x0C54
#define GL_TEXTURE_GEN_S                  0x0C60
#define GL_TEXTURE_GEN_T                  0x0C61
#define GL_TEXTURE_GEN_R                  0x0C62
#define GL_TEXTURE_GEN_Q                  0x0C63
#define GL_PIXEL_MAP_I_TO_I               0x0C70
#define GL_PIXEL_MAP_S_TO_S               0x0C71
#define GL_PIXEL_MAP_I_TO_R               0x0C72
#define GL_PIXEL_MAP_I_TO_G               0x0C73
#define GL_PIXEL_MAP_I_TO_B               0x0C74
#define GL_PIXEL_MAP_I_TO_A               0x0C75
#define GL_PIXEL_MAP_R_TO_R               0x0C76
#define GL_PIXEL_MAP_G_TO_G               0x0C77
#define GL_PIXEL_MAP_B_TO_B               0x0C78
#define GL_PIXEL_MAP_A_TO_A               0x0C79
#define GL_PIXEL_MAP_I_TO_I_SIZE          0x0CB0
#define GL_PIXEL_MAP_S_TO_S_SIZE          0x0CB1
#define GL_PIXEL_MAP_I_TO_R_SIZE          0x0CB2
#define GL_PIXEL_MAP_I_TO_G_SIZE          0x0CB3
#define GL_PIXEL_MAP_I_TO_B_SIZE          0x0CB4
#define GL_PIXEL_MAP_I_TO_A_SIZE          0x0CB5
#define GL_PIXEL_MAP_R_TO_R_SIZE          0x0CB6
#define GL_PIXEL_MAP_G_TO_G_SIZE          0x0CB7
#define GL_PIXEL_MAP_B_TO_B_SIZE          0x0CB8
#define GL_PIXEL_MAP_A_TO_A_SIZE          0x0CB9
#define GL_UNPACK_SWAP_BYTES              0x0CF0
#define GL_UNPACK_LSB_FIRST               0x0CF1
#define GL_UNPACK_ROW_LENGTH              0x0CF2
#define GL_UNPACK_SKIP_ROWS               0x0CF3
#define GL_UNPACK_SKIP_PIXELS             0x0CF4
#define GL_UNPACK_ALIGNMENT               0x0CF5
#define GL_PACK_SWAP_BYTES                0x0D00
#define GL_PACK_LSB_FIRST                 0x0D01
#define GL_PACK_ROW_LENGTH                0x0D02
#define GL_PACK_SKIP_ROWS                 0x0D03
#define GL_PACK_SKIP_PIXELS               0x0D04
#define GL_PACK_ALIGNMENT                 0x0D05
#define GL_MAP_COLOR                      0x0D10
#define GL_MAP_STENCIL                    0x0D11
#define GL_INDEX_SHIFT                    0x0D12
#define GL_INDEX_OFFSET                   0x0D13
#define GL_RED_SCALE                      0x0D14
#define GL_RED_BIAS                       0x0D15
#define GL_ZOOM_X                         0x0D16
#define GL_ZOOM_Y                         0x0D17
#define GL_GREEN_SCALE                    0x0D18
#define GL_GREEN_BIAS                     0x0D19
#define GL_BLUE_SCALE                     0x0D1A
#define GL_BLUE_BIAS                      0x0D1B
#define GL_ALPHA_SCALE                    0x0D1C
#define GL_ALPHA_BIAS                     0x0D1D
#define GL_DEPTH_SCALE                    0x0D1E
#define GL_DEPTH_BIAS                     0x0D1F
#define GL_MAX_EVAL_ORDER                 0x0D30
#define GL_MAX_LIGHTS                     0x0D31
#define GL_MAX_CLIP_PLANES                0x0D32
#define GL_MAX_TEXTURE_SIZE               0x0D33
#define GL_MAX_PIXEL_MAP_TABLE            0x0D34
#define GL_MAX_ATTRIB_STACK_DEPTH         0x0D35
#define GL_MAX_MODELVIEW_STACK_DEPTH      0x0D36
#define GL_MAX_NAME_STACK_DEPTH           0x0D37
#define GL_MAX_PROJECTION_STACK_DEPTH     0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH        0x0D39
#define GL_MAX_VIEWPORT_DIMS              0x0D3A
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH  0x0D3B
#define GL_SUBPIXEL_BITS                  0x0D50
#define GL_INDEX_BITS                     0x0D51
#define GL_RED_BITS                       0x0D52
#define GL_GREEN_BITS                     0x0D53
#define GL_BLUE_BITS                      0x0D54
#define GL_ALPHA_BITS                     0x0D55
#define GL_DEPTH_BITS                     0x0D56
#define GL_STENCIL_BITS                   0x0D57
#define GL_ACCUM_RED_BITS                 0x0D58
#define GL_ACCUM_GREEN_BITS               0x0D59
#define GL_ACCUM_BLUE_BITS                0x0D5A
#define GL_ACCUM_ALPHA_BITS               0x0D5B
#define GL_NAME_STACK_DEPTH               0x0D70
#define GL_AUTO_NORMAL                    0x0D80
#define GL_MAP1_COLOR_4                   0x0D90
#define GL_MAP1_INDEX                     0x0D91
#define GL_MAP1_NORMAL                    0x0D92
#define GL_MAP1_TEXTURE_COORD_1           0x0D93
#define GL_MAP1_TEXTURE_COORD_2           0x0D94
#define GL_MAP1_TEXTURE_COORD_3           0x0D95
#define GL_MAP1_TEXTURE_COORD_4           0x0D96
#define GL_MAP1_VERTEX_3                  0x0D97
#define GL_MAP1_VERTEX_4                  0x0D98
#define GL_MAP2_COLOR_4                   0x0DB0
#define GL_MAP2_INDEX                     0x0DB1
#define GL_MAP2_NORMAL                    0x0DB2
#define GL_MAP2_TEXTURE_COORD_1           0x0DB3
#define GL_MAP2_TEXTURE_COORD_2           0x0DB4
#define GL_MAP2_TEXTURE_COORD_3           0x0DB5
#define GL_MAP2_TEXTURE_COORD_4           0x0DB6
#define GL_MAP2_VERTEX_3                  0x0DB7
#define GL_MAP2_VERTEX_4                  0x0DB8
#define GL_MAP1_GRID_DOMAIN               0x0DD0
#define GL_MAP1_GRID_SEGMENTS             0x0DD1
#define GL_MAP2_GRID_DOMAIN               0x0DD2
#define GL_MAP2_GRID_SEGMENTS             0x0DD3
#define GL_TEXTURE_1D                     0x0DE0
#define GL_TEXTURE_2D                     0x0DE1
#define GL_FEEDBACK_BUFFER_POINTER        0x0DF0
#define GL_FEEDBACK_BUFFER_SIZE           0x0DF1
#define GL_FEEDBACK_BUFFER_TYPE           0x0DF2
#define GL_SELECTION_BUFFER_POINTER       0x0DF3
#define GL_SELECTION_BUFFER_SIZE          0x0DF4
#define GL_TEXTURE_WIDTH                  0x1000
#define GL_TEXTURE_HEIGHT                 0x1001
#define GL_TEXTURE_INTERNAL_FORMAT        0x1003
#define GL_TEXTURE_BORDER_COLOR           0x1004
#define GL_TEXTURE_BORDER                 0x1005
// HintMode 
#define GL_DONT_CARE                      0x1100
#define GL_FASTEST                        0x1101
#define GL_NICEST                         0x1102
// LightName 
#define GL_LIGHT0                         0x4000
#define GL_LIGHT1                         0x4001
#define GL_LIGHT2                         0x4002
#define GL_LIGHT3                         0x4003
#define GL_LIGHT4                         0x4004
#define GL_LIGHT5                         0x4005
#define GL_LIGHT6                         0x4006
#define GL_LIGHT7                         0x4007
// LightParameter 
#define GL_AMBIENT                        0x1200
#define GL_DIFFUSE                        0x1201
#define GL_SPECULAR                       0x1202
#define GL_POSITION                       0x1203
#define GL_SPOT_DIRECTION                 0x1204
#define GL_SPOT_EXPONENT                  0x1205
#define GL_SPOT_CUTOFF                    0x1206
#define GL_CONSTANT_ATTENUATION           0x1207
#define GL_LINEAR_ATTENUATION             0x1208
#define GL_QUADRATIC_ATTENUATION          0x1209
// ListMode 
#define GL_COMPILE                        0x1300
#define GL_COMPILE_AND_EXECUTE            0x1301
// LogicOp 
#define GL_CLEAR                          0x1500
#define GL_AND                            0x1501
#define GL_AND_REVERSE                    0x1502
#define GL_COPY                           0x1503
#define GL_AND_INVERTED                   0x1504
#define GL_NOOP                           0x1505
#define GL_XOR                            0x1506
#define GL_OR                             0x1507
#define GL_NOR                            0x1508
#define GL_EQUIV                          0x1509
#define GL_INVERT                         0x150A
#define GL_OR_REVERSE                     0x150B
#define GL_COPY_INVERTED                  0x150C
#define GL_OR_INVERTED                    0x150D
#define GL_NAND                           0x150E
#define GL_SET                            0x150F
// MaterialParameter 
#define GL_EMISSION                       0x1600
#define GL_SHININESS                      0x1601
#define GL_AMBIENT_AND_DIFFUSE            0x1602
#define GL_COLOR_INDEXES                  0x1603
// MatrixMode 
#define GL_MODELVIEW                      0x1700
#define GL_PROJECTION                     0x1701
#define GL_TEXTURE                        0x1702
// PixelCopyType 
#define GL_COLOR                          0x1800
#define GL_DEPTH                          0x1801
#define GL_STENCIL                        0x1802
// PixelFormat 
#define GL_COLOR_INDEX                    0x1900
#define GL_STENCIL_INDEX                  0x1901
#define GL_DEPTH_COMPONENT                0x1902
#define GL_RED                            0x1903
#define GL_GREEN                          0x1904
#define GL_BLUE                           0x1905
#define GL_ALPHA                          0x1906
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_LUMINANCE                      0x1909
#define GL_LUMINANCE_ALPHA                0x190A
// PixelType 
#define GL_BITMAP                         0x1A00
// PolygonMode 
#define GL_POINT                          0x1B00
#define GL_LINE                           0x1B01
#define GL_FILL                           0x1B02
// RenderingMode 
#define GL_RENDER                         0x1C00
#define GL_FEEDBACK                       0x1C01
#define GL_SELECT                         0x1C02
// ShadingModel 
#define GL_FLAT                           0x1D00
#define GL_SMOOTH                         0x1D01
// StencilOp 
//      GL_ZERO 
#define GL_KEEP                           0x1E00
#define GL_REPLACE                        0x1E01
#define GL_INCR                           0x1E02
#define GL_DECR                           0x1E03
//      GL_INVERT 
// StringName 
#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03
// TextureCoordName 
#define GL_S                              0x2000
#define GL_T                              0x2001
#define GL_R                              0x2002
#define GL_Q                              0x2003
// TextureEnvMode 
#define GL_MODULATE                       0x2100
#define GL_DECAL                          0x2101
// TextureEnvParameter 
#define GL_TEXTURE_ENV_MODE               0x2200
#define GL_TEXTURE_ENV_COLOR              0x2201
// TextureEnvTarget 
#define GL_TEXTURE_ENV                    0x2300
// TextureGenMode 
#define GL_EYE_LINEAR                     0x2400
#define GL_OBJECT_LINEAR                  0x2401
#define GL_SPHERE_MAP                     0x2402
// TextureGenParameter 
#define GL_TEXTURE_GEN_MODE               0x2500
#define GL_OBJECT_PLANE                   0x2501
#define GL_EYE_PLANE                      0x2502
// TextureMagFilter 
#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601
// TextureMinFilter 
//      GL_NEAREST 
//      GL_LINEAR 
#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_LINEAR_MIPMAP_NEAREST          0x2701
#define GL_NEAREST_MIPMAP_LINEAR          0x2702
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
// TextureParameterName 
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
// TextureWrapMode 
#define GL_CLAMP                          0x2900
#define GL_REPEAT                         0x2901
// ClientAttribMask 
#define GL_CLIENT_PIXEL_STORE_BIT         0x00000001
#define GL_CLIENT_VERTEX_ARRAY_BIT        0x00000002
#define GL_CLIENT_ALL_ATTRIB_BITS         0xffffffff
// polygon_offset 
#define GL_POLYGON_OFFSET_FACTOR          0x8038
#define GL_POLYGON_OFFSET_UNITS           0x2A00
#define GL_POLYGON_OFFSET_POINT           0x2A01
#define GL_POLYGON_OFFSET_LINE            0x2A02
#define GL_POLYGON_OFFSET_FILL            0x8037
// texture 
#define GL_ALPHA4                         0x803B
#define GL_ALPHA8                         0x803C
#define GL_ALPHA12                        0x803D
#define GL_ALPHA16                        0x803E
#define GL_LUMINANCE4                     0x803F
#define GL_LUMINANCE8                     0x8040
#define GL_LUMINANCE12                    0x8041
#define GL_LUMINANCE16                    0x8042
#define GL_LUMINANCE4_ALPHA4              0x8043
#define GL_LUMINANCE6_ALPHA2              0x8044
#define GL_LUMINANCE8_ALPHA8              0x8045
#define GL_LUMINANCE12_ALPHA4             0x8046
#define GL_LUMINANCE12_ALPHA12            0x8047
#define GL_LUMINANCE16_ALPHA16            0x8048
#define GL_INTENSITY                      0x8049
#define GL_INTENSITY4                     0x804A
#define GL_INTENSITY8                     0x804B
#define GL_INTENSITY12                    0x804C
#define GL_INTENSITY16                    0x804D
#define GL_R3_G3_B2                       0x2A10
#define GL_RGB4                           0x804F
#define GL_RGB5                           0x8050
#define GL_RGB8                           0x8051
#define GL_RGB10                          0x8052
#define GL_RGB12                          0x8053
#define GL_RGB16                          0x8054
#define GL_RGBA2                          0x8055
#define GL_RGBA4                          0x8056
#define GL_RGB5_A1                        0x8057
#define GL_RGBA8                          0x8058
#define GL_RGB10_A2                       0x8059
#define GL_RGBA12                         0x805A
#define GL_RGBA16                         0x805B
#define GL_TEXTURE_RED_SIZE               0x805C
#define GL_TEXTURE_GREEN_SIZE             0x805D
#define GL_TEXTURE_BLUE_SIZE              0x805E
#define GL_TEXTURE_ALPHA_SIZE             0x805F
#define GL_TEXTURE_LUMINANCE_SIZE         0x8060
#define GL_TEXTURE_INTENSITY_SIZE         0x8061
#define GL_PROXY_TEXTURE_1D               0x8063
#define GL_PROXY_TEXTURE_2D               0x8064

// texture_object 
#define GL_TEXTURE_PRIORITY               0x8066
#define GL_TEXTURE_RESIDENT               0x8067
#define GL_TEXTURE_BINDING_1D             0x8068
#define GL_TEXTURE_BINDING_2D             0x8069

// vertex_array 
#define GL_VERTEX_ARRAY                   0x8074
#define GL_NORMAL_ARRAY                   0x8075
#define GL_COLOR_ARRAY                    0x8076
#define GL_INDEX_ARRAY                    0x8077
#define GL_TEXTURE_COORD_ARRAY            0x8078
#define GL_EDGE_FLAG_ARRAY                0x8079
#define GL_VERTEX_ARRAY_SIZE              0x807A
#define GL_VERTEX_ARRAY_TYPE              0x807B
#define GL_VERTEX_ARRAY_STRIDE            0x807C
#define GL_NORMAL_ARRAY_TYPE              0x807E
#define GL_NORMAL_ARRAY_STRIDE            0x807F
#define GL_COLOR_ARRAY_SIZE               0x8081
#define GL_COLOR_ARRAY_TYPE               0x8082
#define GL_COLOR_ARRAY_STRIDE             0x8083
#define GL_INDEX_ARRAY_TYPE               0x8085
#define GL_INDEX_ARRAY_STRIDE             0x8086
#define GL_TEXTURE_COORD_ARRAY_SIZE       0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE       0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE     0x808A
#define GL_EDGE_FLAG_ARRAY_STRIDE         0x808C
#define GL_VERTEX_ARRAY_POINTER           0x808E
#define GL_NORMAL_ARRAY_POINTER           0x808F
#define GL_COLOR_ARRAY_POINTER            0x8090
#define GL_INDEX_ARRAY_POINTER            0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER    0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER        0x8093
#define GL_V2F                            0x2A20
#define GL_V3F                            0x2A21
#define GL_C4UB_V2F                       0x2A22
#define GL_C4UB_V3F                       0x2A23
#define GL_C3F_V3F                        0x2A24
#define GL_N3F_V3F                        0x2A25
#define GL_C4F_N3F_V3F                    0x2A26
#define GL_T2F_V3F                        0x2A27
#define GL_T4F_V4F                        0x2A28
#define GL_T2F_C4UB_V3F                   0x2A29
#define GL_T2F_C3F_V3F                    0x2A2A
#define GL_T2F_N3F_V3F                    0x2A2B
#define GL_T2F_C4F_N3F_V3F                0x2A2C
#define GL_T4F_C4F_N3F_V4F                0x2A2D

// Extensions 
#define GL_EXT_vertex_array               1
#define GL_EXT_bgra                       1
#define GL_EXT_paletted_texture           1
#define GL_WIN_swap_hint                  1
#define GL_WIN_draw_range_elements        1
// #define GL_WIN_phong_shading              1
// #define GL_WIN_specular_fog               1

// EXT_vertex_array 
#define GL_VERTEX_ARRAY_EXT               0x8074
#define GL_NORMAL_ARRAY_EXT               0x8075
#define GL_COLOR_ARRAY_EXT                0x8076
#define GL_INDEX_ARRAY_EXT                0x8077
#define GL_TEXTURE_COORD_ARRAY_EXT        0x8078
#define GL_EDGE_FLAG_ARRAY_EXT            0x8079
#define GL_VERTEX_ARRAY_SIZE_EXT          0x807A
#define GL_VERTEX_ARRAY_TYPE_EXT          0x807B
#define GL_VERTEX_ARRAY_STRIDE_EXT        0x807C
#define GL_VERTEX_ARRAY_COUNT_EXT         0x807D
#define GL_NORMAL_ARRAY_TYPE_EXT          0x807E
#define GL_NORMAL_ARRAY_STRIDE_EXT        0x807F
#define GL_NORMAL_ARRAY_COUNT_EXT         0x8080
#define GL_COLOR_ARRAY_SIZE_EXT           0x8081
#define GL_COLOR_ARRAY_TYPE_EXT           0x8082
#define GL_COLOR_ARRAY_STRIDE_EXT         0x8083
#define GL_COLOR_ARRAY_COUNT_EXT          0x8084
#define GL_INDEX_ARRAY_TYPE_EXT           0x8085
#define GL_INDEX_ARRAY_STRIDE_EXT         0x8086
#define GL_INDEX_ARRAY_COUNT_EXT          0x8087
#define GL_TEXTURE_COORD_ARRAY_SIZE_EXT   0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE_EXT   0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE_EXT 0x808A
#define GL_TEXTURE_COORD_ARRAY_COUNT_EXT  0x808B
#define GL_EDGE_FLAG_ARRAY_STRIDE_EXT     0x808C
#define GL_EDGE_FLAG_ARRAY_COUNT_EXT      0x808D
#define GL_VERTEX_ARRAY_POINTER_EXT       0x808E
#define GL_NORMAL_ARRAY_POINTER_EXT       0x808F
#define GL_COLOR_ARRAY_POINTER_EXT        0x8090
#define GL_INDEX_ARRAY_POINTER_EXT        0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER_EXT 0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER_EXT    0x8093
#define GL_DOUBLE_EXT                     GL_DOUBLE

// EXT_bgra 
#define GL_BGR_EXT                        0x80E0
#define GL_BGRA_EXT                       0x80E1

// EXT_paletted_texture 

// These must match the GL_COLOR_TABLE_*_SGI enumerants 
#define GL_COLOR_TABLE_FORMAT_EXT         0x80D8
#define GL_COLOR_TABLE_WIDTH_EXT          0x80D9
#define GL_COLOR_TABLE_RED_SIZE_EXT       0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE_EXT     0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE_EXT      0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE_EXT     0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE_EXT 0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE_EXT 0x80DF

#define GL_COLOR_INDEX1_EXT               0x80E2
#define GL_COLOR_INDEX2_EXT               0x80E3
#define GL_COLOR_INDEX4_EXT               0x80E4
#define GL_COLOR_INDEX8_EXT               0x80E5
#define GL_COLOR_INDEX12_EXT              0x80E6
#define GL_COLOR_INDEX16_EXT              0x80E7

// WIN_draw_range_elements 
#define GL_MAX_ELEMENTS_VERTICES_WIN      0x80E8
#define GL_MAX_ELEMENTS_INDICES_WIN       0x80E9

// WIN_phong_shading 
#define GL_PHONG_WIN                      0x80EA 
#define GL_PHONG_HINT_WIN                 0x80EB 

// WIN_specular_fog 
#define GL_FOG_SPECULAR_TEXTURE_WIN       0x80EC



typedef int GLenum;
typedef long GLboolean;
typedef int GLbitfield;
typedef long GLbyte;
typedef long GLshort;
typedef int GLint;
typedef int GLsizei;
typedef long GLubyte;
typedef long GLushort;
typedef int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

void __stdcall glAccum (GLenum op, GLfloat value);
void __stdcall glAlphaFunc (GLenum func, GLclampf ref);
GLboolean __stdcall glAreTexturesResident (GLsizei n, GLuint *textures, GLboolean *residences);
void __stdcall glArrayElement (GLint i);
void __stdcall glBegin (GLenum mode);
void __stdcall glBindTexture (GLenum target, GLuint texture);
void __stdcall glBitmap (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, GLubyte *bitmap);
void __stdcall glBlendFunc (GLenum sfactor, GLenum dfactor);
void __stdcall glCallList (GLuint list);
void __stdcall glCallLists (GLsizei n, GLenum type, GLvoid *lists);
void __stdcall glClear (GLbitfield mask);
void __stdcall glClearAccum (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void __stdcall glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void __stdcall glClearDepth (GLclampd depth);
void __stdcall glClearIndex (GLfloat c);
void __stdcall glClearStencil (GLint s);
void __stdcall glClipPlane (GLenum plane, GLdouble *equation);
void __stdcall glColor3b (GLbyte red, GLbyte green, GLbyte blue);
void __stdcall glColor3bv (GLbyte *v);
void __stdcall glColor3d (GLdouble red, GLdouble green, GLdouble blue);
void __stdcall glColor3dv (GLdouble *v);
void __stdcall glColor3f (GLfloat red, GLfloat green, GLfloat blue);
void __stdcall glColor3fv (GLfloat *v);
void __stdcall glColor3i (GLint red, GLint green, GLint blue);
void __stdcall glColor3iv (GLint *v);
void __stdcall glColor3s (GLshort red, GLshort green, GLshort blue);
void __stdcall glColor3sv (GLshort *v);
void __stdcall glColor3ub (GLubyte red, GLubyte green, GLubyte blue);
void __stdcall glColor3ubv (GLubyte *v);
void __stdcall glColor3ui (GLuint red, GLuint green, GLuint blue);
void __stdcall glColor3uiv (GLuint *v);
void __stdcall glColor3us (GLushort red, GLushort green, GLushort blue);
void __stdcall glColor3usv (GLushort *v);
void __stdcall glColor4b (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
void __stdcall glColor4bv (GLbyte *v);
void __stdcall glColor4d (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
void __stdcall glColor4dv (GLdouble *v);
void __stdcall glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void __stdcall glColor4fv (GLfloat *v);
void __stdcall glColor4i (GLint red, GLint green, GLint blue, GLint alpha);
void __stdcall glColor4iv (GLint *v);
void __stdcall glColor4s (GLshort red, GLshort green, GLshort blue, GLshort alpha);
void __stdcall glColor4sv (GLshort *v);
void __stdcall glColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
void __stdcall glColor4ubv (GLubyte *v);
void __stdcall glColor4ui (GLuint red, GLuint green, GLuint blue, GLuint alpha);
void __stdcall glColor4uiv (GLuint *v);
void __stdcall glColor4us (GLushort red, GLushort green, GLushort blue, GLushort alpha);
void __stdcall glColor4usv (GLushort *v);
void __stdcall glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void __stdcall glColorMaterial (GLenum face, GLenum mode);
void __stdcall glColorPointer (GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
void __stdcall glCopyPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
void __stdcall glCopyTexImage1D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
void __stdcall glCopyTexImage2D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void __stdcall glCopyTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void __stdcall glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void __stdcall glCullFace (GLenum mode);
void __stdcall glDeleteLists (GLuint list, GLsizei range);
void __stdcall glDeleteTextures (GLsizei n, GLuint *textures);
void __stdcall glDepthFunc (GLenum func);
void __stdcall glDepthMask (GLboolean flag);
void __stdcall glDepthRange (GLclampd zNear, GLclampd zFar);
void __stdcall glDisable (GLenum cap);
void __stdcall glDisableClientState (GLenum array);
void __stdcall glDrawArrays (GLenum mode, GLint first, GLsizei count);
void __stdcall glDrawBuffer (GLenum mode);
void __stdcall glDrawElements (GLenum mode, GLsizei count, GLenum type, GLvoid *indices);
void __stdcall glDrawPixels (GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void __stdcall glEdgeFlag (GLboolean flag);
void __stdcall glEdgeFlagPointer (GLsizei stride, GLvoid *pointer);
void __stdcall glEdgeFlagv (GLboolean *flag);
void __stdcall glEnable (GLenum cap);
void __stdcall glEnableClientState (GLenum array);
void __stdcall glEnd ();
void __stdcall glEndList ();
void __stdcall glEvalCoord1d (GLdouble u);
void __stdcall glEvalCoord1dv (GLdouble *u);
void __stdcall glEvalCoord1f (GLfloat u);
void __stdcall glEvalCoord1fv (GLfloat *u);
void __stdcall glEvalCoord2d (GLdouble u, GLdouble v);
void __stdcall glEvalCoord2dv (GLdouble *u);
void __stdcall glEvalCoord2f (GLfloat u, GLfloat v);
void __stdcall glEvalCoord2fv (GLfloat *u);
void __stdcall glEvalMesh1 (GLenum mode, GLint i1, GLint i2);
void __stdcall glEvalMesh2 (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
void __stdcall glEvalPoint1 (GLint i);
void __stdcall glEvalPoint2 (GLint i, GLint j);
void __stdcall glFeedbackBuffer (GLsizei size, GLenum type, GLfloat *buffer);
void __stdcall glFinish ();
void __stdcall glFlush ();
void __stdcall glFogf (GLenum pname, GLfloat param);
void __stdcall glFogfv (GLenum pname, GLfloat *params);
void __stdcall glFogi (GLenum pname, GLint param);
void __stdcall glFogiv (GLenum pname, GLint *params);
void __stdcall glFrontFace (GLenum mode);
void __stdcall glFrustum (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLuint __stdcall glGenLists (GLsizei range);
void __stdcall glGenTextures (GLsizei n, GLuint *textures);
void __stdcall glGetBooleanv (GLenum pname, GLboolean *params);
void __stdcall glGetClipPlane (GLenum plane, GLdouble *equation);
void __stdcall glGetDoublev (GLenum pname, GLdouble *params);
GLenum __stdcall glGetError ();
void __stdcall glGetFloatv (GLenum pname, GLfloat *params);
void __stdcall glGetIntegerv (GLenum pname, GLint *params);
void __stdcall glGetLightfv (GLenum light, GLenum pname, GLfloat *params);
void __stdcall glGetLightiv (GLenum light, GLenum pname, GLint *params);
void __stdcall glGetMapdv (GLenum target, GLenum query, GLdouble *v);
void __stdcall glGetMapfv (GLenum target, GLenum query, GLfloat *v);
void __stdcall glGetMapiv (GLenum target, GLenum query, GLint *v);
void __stdcall glGetMaterialfv (GLenum face, GLenum pname, GLfloat *params);
void __stdcall glGetMaterialiv (GLenum face, GLenum pname, GLint *params);
void __stdcall glGetPixelMapfv (GLenum map, GLfloat *values);
void __stdcall glGetPixelMapuiv (GLenum map, GLuint *values);
void __stdcall glGetPixelMapusv (GLenum map, GLushort *values);
void __stdcall glGetPointerv (GLenum pname, GLvoid* *params);
void __stdcall glGetPolygonStipple (GLubyte *mask);
GLubyte * __stdcall glGetString (GLenum name);
void __stdcall glGetTexEnvfv (GLenum target, GLenum pname, GLfloat *params);
void __stdcall glGetTexEnviv (GLenum target, GLenum pname, GLint *params);
void __stdcall glGetTexGendv (GLenum coord, GLenum pname, GLdouble *params);
void __stdcall glGetTexGenfv (GLenum coord, GLenum pname, GLfloat *params);
void __stdcall glGetTexGeniv (GLenum coord, GLenum pname, GLint *params);
void __stdcall glGetTexImage (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
void __stdcall glGetTexLevelParameterfv (GLenum target, GLint level, GLenum pname, GLfloat *params);
void __stdcall glGetTexLevelParameteriv (GLenum target, GLint level, GLenum pname, GLint *params);
void __stdcall glGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params);
void __stdcall glGetTexParameteriv (GLenum target, GLenum pname, GLint *params);
void __stdcall glHint (GLenum target, GLenum mode);
void __stdcall glIndexMask (GLuint mask);
void __stdcall glIndexPointer (GLenum type, GLsizei stride, GLvoid *pointer);
void __stdcall glIndexd (GLdouble c);
void __stdcall glIndexdv (GLdouble *c);
void __stdcall glIndexf (GLfloat c);
void __stdcall glIndexfv (GLfloat *c);
void __stdcall glIndexi (GLint c);
void __stdcall glIndexiv (GLint *c);
void __stdcall glIndexs (GLshort c);
void __stdcall glIndexsv (GLshort *c);
void __stdcall glIndexub (GLubyte c);
void __stdcall glIndexubv (GLubyte *c);
void __stdcall glInitNames ();
void __stdcall glInterleavedArrays (GLenum format, GLsizei stride, GLvoid *pointer);
GLboolean __stdcall glIsEnabled (GLenum cap);
GLboolean __stdcall glIsList (GLuint list);
GLboolean __stdcall glIsTexture (GLuint texture);
void __stdcall glLightModelf (GLenum pname, GLfloat param);
void __stdcall glLightModelfv (GLenum pname, GLfloat *params);
void __stdcall glLightModeli (GLenum pname, GLint param);
void __stdcall glLightModeliv (GLenum pname, GLint *params);
void __stdcall glLightf (GLenum light, GLenum pname, GLfloat param);
void __stdcall glLightfv (GLenum light, GLenum pname, GLfloat *params);
void __stdcall glLighti (GLenum light, GLenum pname, GLint param);
void __stdcall glLightiv (GLenum light, GLenum pname, GLint *params);
void __stdcall glLineStipple (GLint factor, GLushort pattern);
void __stdcall glLineWidth (GLfloat width);
void __stdcall glListBase (GLuint base);
void __stdcall glLoadIdentity ();
void __stdcall glLoadMatrixd (GLdouble *m);
void __stdcall glLoadMatrixf (GLfloat *m);
void __stdcall glLoadName (GLuint name);
void __stdcall glLogicOp (GLenum opcode);
void __stdcall glMap1d (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, GLdouble *points);
void __stdcall glMap1f (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, GLfloat *points);
void __stdcall glMap2d (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, GLdouble *points);
void __stdcall glMap2f (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat *points);
void __stdcall glMapGrid1d (GLint un, GLdouble u1, GLdouble u2);
void __stdcall glMapGrid1f (GLint un, GLfloat u1, GLfloat u2);
void __stdcall glMapGrid2d (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
void __stdcall glMapGrid2f (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
void __stdcall glMaterialf (GLenum face, GLenum pname, GLfloat param);
void __stdcall glMaterialfv (GLenum face, GLenum pname, GLfloat *params);
void __stdcall glMateriali (GLenum face, GLenum pname, GLint param);
void __stdcall glMaterialiv (GLenum face, GLenum pname, GLint *params);
void __stdcall glMatrixMode (GLenum mode);
void __stdcall glMultMatrixd (GLdouble *m);
void __stdcall glMultMatrixf (GLfloat *m);
void __stdcall glNewList (GLuint list, GLenum mode);
void __stdcall glNormal3b (GLbyte nx, GLbyte ny, GLbyte nz);
void __stdcall glNormal3bv (GLbyte *v);
void __stdcall glNormal3d (GLdouble nx, GLdouble ny, GLdouble nz);
void __stdcall glNormal3dv (GLdouble *v);
void __stdcall glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz);
void __stdcall glNormal3fv (GLfloat *v);
void __stdcall glNormal3i (GLint nx, GLint ny, GLint nz);
void __stdcall glNormal3iv (GLint *v);
void __stdcall glNormal3s (GLshort nx, GLshort ny, GLshort nz);
void __stdcall glNormal3sv (GLshort *v);
void __stdcall glNormalPointer (GLenum type, GLsizei stride, GLvoid *pointer);
void __stdcall glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void __stdcall glPassThrough (GLfloat token);
void __stdcall glPixelMapfv (GLenum map, GLsizei mapsize, GLfloat *values);
void __stdcall glPixelMapuiv (GLenum map, GLsizei mapsize, GLuint *values);
void __stdcall glPixelMapusv (GLenum map, GLsizei mapsize, GLushort *values);
void __stdcall glPixelStoref (GLenum pname, GLfloat param);
void __stdcall glPixelStorei (GLenum pname, GLint param);
void __stdcall glPixelTransferf (GLenum pname, GLfloat param);
void __stdcall glPixelTransferi (GLenum pname, GLint param);
void __stdcall glPixelZoom (GLfloat xfactor, GLfloat yfactor);
void __stdcall glPointSize (GLfloat size);
void __stdcall glPolygonMode (GLenum face, GLenum mode);
void __stdcall glPolygonOffset (GLfloat factor, GLfloat units);
void __stdcall glPolygonStipple (GLubyte *mask);
void __stdcall glPopAttrib ();
void __stdcall glPopClientAttrib ();
void __stdcall glPopMatrix ();
void __stdcall glPopName ();
void __stdcall glPrioritizeTextures (GLsizei n, GLuint *textures, GLclampf *priorities);
void __stdcall glPushAttrib (GLbitfield mask);
void __stdcall glPushClientAttrib (GLbitfield mask);
void __stdcall glPushMatrix ();
void __stdcall glPushName (GLuint name);
void __stdcall glRasterPos2d (GLdouble x, GLdouble y);
void __stdcall glRasterPos2dv (GLdouble *v);
void __stdcall glRasterPos2f (GLfloat x, GLfloat y);
void __stdcall glRasterPos2fv (GLfloat *v);
void __stdcall glRasterPos2i (GLint x, GLint y);
void __stdcall glRasterPos2iv (GLint *v);
void __stdcall glRasterPos2s (GLshort x, GLshort y);
void __stdcall glRasterPos2sv (GLshort *v);
void __stdcall glRasterPos3d (GLdouble x, GLdouble y, GLdouble z);
void __stdcall glRasterPos3dv (GLdouble *v);
void __stdcall glRasterPos3f (GLfloat x, GLfloat y, GLfloat z);
void __stdcall glRasterPos3fv (GLfloat *v);
void __stdcall glRasterPos3i (GLint x, GLint y, GLint z);
void __stdcall glRasterPos3iv (GLint *v);
void __stdcall glRasterPos3s (GLshort x, GLshort y, GLshort z);
void __stdcall glRasterPos3sv (GLshort *v);
void __stdcall glRasterPos4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void __stdcall glRasterPos4dv (GLdouble *v);
void __stdcall glRasterPos4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void __stdcall glRasterPos4fv (GLfloat *v);
void __stdcall glRasterPos4i (GLint x, GLint y, GLint z, GLint w);
void __stdcall glRasterPos4iv (GLint *v);
void __stdcall glRasterPos4s (GLshort x, GLshort y, GLshort z, GLshort w);
void __stdcall glRasterPos4sv (GLshort *v);
void __stdcall glReadBuffer (GLenum mode);
void __stdcall glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void __stdcall glRectd (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
void __stdcall glRectdv (GLdouble *v1, GLdouble *v2);
void __stdcall glRectf (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
void __stdcall glRectfv (GLfloat *v1, GLfloat *v2);
void __stdcall glRecti (GLint x1, GLint y1, GLint x2, GLint y2);
void __stdcall glRectiv (GLint *v1, GLint *v2);
void __stdcall glRects (GLshort x1, GLshort y1, GLshort x2, GLshort y2);
void __stdcall glRectsv (GLshort *v1, GLshort *v2);
GLint __stdcall glRenderMode (GLenum mode);
void __stdcall glRotated (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
void __stdcall glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void __stdcall glScaled (GLdouble x, GLdouble y, GLdouble z);
void __stdcall glScalef (GLfloat x, GLfloat y, GLfloat z);
void __stdcall glScissor (GLint x, GLint y, GLsizei width, GLsizei height);
void __stdcall glSelectBuffer (GLsizei size, GLuint *buffer);
void __stdcall glShadeModel (GLenum mode);
void __stdcall glStencilFunc (GLenum func, GLint ref, GLuint mask);
void __stdcall glStencilMask (GLuint mask);
void __stdcall glStencilOp (GLenum fail, GLenum zfail, GLenum zpass);
void __stdcall glTexCoord1d (GLdouble s);
void __stdcall glTexCoord1dv (GLdouble *v);
void __stdcall glTexCoord1f (GLfloat s);
void __stdcall glTexCoord1fv (GLfloat *v);
void __stdcall glTexCoord1i (GLint s);
void __stdcall glTexCoord1iv (GLint *v);
void __stdcall glTexCoord1s (GLshort s);
void __stdcall glTexCoord1sv (GLshort *v);
void __stdcall glTexCoord2d (GLdouble s, GLdouble t);
void __stdcall glTexCoord2dv (GLdouble *v);
void __stdcall glTexCoord2f (GLfloat s, GLfloat t);
void __stdcall glTexCoord2fv (GLfloat *v);
void __stdcall glTexCoord2i (GLint s, GLint t);
void __stdcall glTexCoord2iv (GLint *v);
void __stdcall glTexCoord2s (GLshort s, GLshort t);
void __stdcall glTexCoord2sv (GLshort *v);
void __stdcall glTexCoord3d (GLdouble s, GLdouble t, GLdouble r);
void __stdcall glTexCoord3dv (GLdouble *v);
void __stdcall glTexCoord3f (GLfloat s, GLfloat t, GLfloat r);
void __stdcall glTexCoord3fv (GLfloat *v);
void __stdcall glTexCoord3i (GLint s, GLint t, GLint r);
void __stdcall glTexCoord3iv (GLint *v);
void __stdcall glTexCoord3s (GLshort s, GLshort t, GLshort r);
void __stdcall glTexCoord3sv (GLshort *v);
void __stdcall glTexCoord4d (GLdouble s, GLdouble t, GLdouble r, GLdouble q);
void __stdcall glTexCoord4dv (GLdouble *v);
void __stdcall glTexCoord4f (GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void __stdcall glTexCoord4fv (GLfloat *v);
void __stdcall glTexCoord4i (GLint s, GLint t, GLint r, GLint q);
void __stdcall glTexCoord4iv (GLint *v);
void __stdcall glTexCoord4s (GLshort s, GLshort t, GLshort r, GLshort q);
void __stdcall glTexCoord4sv (GLshort *v);
void __stdcall glTexCoordPointer (GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
void __stdcall glTexEnvf (GLenum target, GLenum pname, GLfloat param);
void __stdcall glTexEnvfv (GLenum target, GLenum pname, GLfloat *params);
void __stdcall glTexEnvi (GLenum target, GLenum pname, GLint param);
void __stdcall glTexEnviv (GLenum target, GLenum pname, GLint *params);
void __stdcall glTexGend (GLenum coord, GLenum pname, GLdouble param);
void __stdcall glTexGendv (GLenum coord, GLenum pname, GLdouble *params);
void __stdcall glTexGenf (GLenum coord, GLenum pname, GLfloat param);
void __stdcall glTexGenfv (GLenum coord, GLenum pname, GLfloat *params);
void __stdcall glTexGeni (GLenum coord, GLenum pname, GLint param);
void __stdcall glTexGeniv (GLenum coord, GLenum pname, GLint *params);
void __stdcall glTexImage1D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, GLvoid *pixels);
void __stdcall glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, GLvoid *pixels);
void __stdcall glTexParameterf (GLenum target, GLenum pname, GLfloat param);
void __stdcall glTexParameterfv (GLenum target, GLenum pname, GLfloat *params);
void __stdcall glTexParameteri (GLenum target, GLenum pname, GLint param);
void __stdcall glTexParameteriv (GLenum target, GLenum pname, GLint *params);
void __stdcall glTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, GLvoid *pixels);
void __stdcall glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void __stdcall glTranslated (GLdouble x, GLdouble y, GLdouble z);
void __stdcall glTranslatef (GLfloat x, GLfloat y, GLfloat z);
void __stdcall glVertex2d (GLdouble x, GLdouble y);
void __stdcall glVertex2dv (GLdouble *v);
void __stdcall glVertex2f (GLfloat x, GLfloat y);
void __stdcall glVertex2fv (GLfloat *v);
void __stdcall glVertex2i (GLint x, GLint y);
void __stdcall glVertex2iv (GLint *v);
void __stdcall glVertex2s (GLshort x, GLshort y);
void __stdcall glVertex2sv (GLshort *v);
void __stdcall glVertex3d (GLdouble x, GLdouble y, GLdouble z);
void __stdcall glVertex3dv (GLdouble *v);
void __stdcall glVertex3f (GLfloat x, GLfloat y, GLfloat z);
void __stdcall glVertex3fv (GLfloat *v);
void __stdcall glVertex3i (GLint x, GLint y, GLint z);
void __stdcall glVertex3iv (GLint *v);
void __stdcall glVertex3s (GLshort x, GLshort y, GLshort z);
void __stdcall glVertex3sv (GLshort *v);
void __stdcall glVertex4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void __stdcall glVertex4dv (GLdouble *v);
void __stdcall glVertex4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void __stdcall glVertex4fv (GLfloat *v);
void __stdcall glVertex4i (GLint x, GLint y, GLint z, GLint w);
void __stdcall glVertex4iv (GLint *v);
void __stdcall glVertex4s (GLshort x, GLshort y, GLshort z, GLshort w);
void __stdcall glVertex4sv (GLshort *v);
void __stdcall glVertexPointer (GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
void __stdcall glViewport (GLint x, GLint y, GLsizei width, GLsizei height);

BOOL  __stdcall wglCopyContext(long,long, UINT);
long  __stdcall wglCreateContext(HDC);
long  __stdcall wglCreateLayerContext(HDC, int);
BOOL  __stdcall wglDeleteContext(long);
long  __stdcall wglGetCurrentContext();
HDC   __stdcall wglGetCurrentDC();
long  __stdcall wglGetProcAddress(long);
BOOL  __stdcall wglMakeCurrent(HDC,long);
BOOL  __stdcall wglShareLists(long,long);
BOOL  __stdcall wglUseFontBitmapsA(HDC, DWORD, DWORD, DWORD);
BOOL  __stdcall wglUseFontBitmapsW(HDC, DWORD, DWORD, DWORD);

#ifdef GLAUX
void __stdcall auxWireSphere(GLdouble);
void __stdcall auxSolidSphere(GLdouble);
void __stdcall auxWireCube(GLdouble);
void __stdcall auxSolidCube(GLdouble);
void __stdcall auxWireBox(GLdouble, GLdouble, GLdouble);
void __stdcall auxSolidBox(GLdouble, GLdouble, GLdouble);
void __stdcall auxWireTorus(GLdouble, GLdouble);
void __stdcall auxSolidTorus(GLdouble, GLdouble);
void __stdcall auxWireCylinder(GLdouble, GLdouble);
void __stdcall auxSolidCylinder(GLdouble, GLdouble);
void __stdcall auxWireIcosahedron(GLdouble);
void __stdcall auxSolidIcosahedron(GLdouble);
void __stdcall auxWireOctahedron(GLdouble);
void __stdcall auxSolidOctahedron(GLdouble);
void __stdcall auxWireTetrahedron(GLdouble);
void __stdcall auxSolidTetrahedron(GLdouble);
void __stdcall auxWireDodecahedron(GLdouble);
void __stdcall auxSolidDodecahedron(GLdouble);
void __stdcall auxWireCone(GLdouble, GLdouble);
void __stdcall auxSolidCone(GLdouble, GLdouble);
void __stdcall auxWireTeapot(GLdouble);
void __stdcall auxSolidTeapot(GLdouble);
#endif

void __stdcall gluPerspective (GLdouble fovy,GLdouble aspect,GLdouble zNear,GLdouble zFar);
void __stdcall gluLookAt (GLdouble eyex,GLdouble eyey,GLdouble eyez,GLdouble centerx,GLdouble centery,GLdouble centerz,GLdouble upx,GLdouble upy,GLdouble upz);

// API functions

#define PRAGMA_API GlmfBeginGlsBlock;opengl32!GlmfBeginGlsBlock
#define PRAGMA_API GlmfCloseMetaFile;opengl32!GlmfCloseMetaFile
#define PRAGMA_API GlmfEndGlsBlock;opengl32!GlmfEndGlsBlock
#define PRAGMA_API GlmfEndPlayback;opengl32!GlmfEndPlayback
#define PRAGMA_API GlmfInitPlayback;opengl32!GlmfInitPlayback
#define PRAGMA_API GlmfPlayGlsRecord;opengl32!GlmfPlayGlsRecord
#define PRAGMA_API glAccum;opengl32!glAccum
#define PRAGMA_API glAlphaFunc;opengl32!glAlphaFunc
#define PRAGMA_API glAreTexturesResident;opengl32!glAreTexturesResident
#define PRAGMA_API glArrayElement;opengl32!glArrayElement
#define PRAGMA_API glBegin;opengl32!glBegin
#define PRAGMA_API glBindTexture;opengl32!glBindTexture
#define PRAGMA_API glBitmap;opengl32!glBitmap
#define PRAGMA_API glBlendFunc;opengl32!glBlendFunc
#define PRAGMA_API glCallList;opengl32!glCallList
#define PRAGMA_API glCallLists;opengl32!glCallLists
#define PRAGMA_API glClear;opengl32!glClear
#define PRAGMA_API glClearAccum;opengl32!glClearAccum
#define PRAGMA_API glClearColor;opengl32!glClearColor
#define PRAGMA_API glClearDepth;opengl32!glClearDepth
#define PRAGMA_API glClearIndex;opengl32!glClearIndex
#define PRAGMA_API glClearStencil;opengl32!glClearStencil
#define PRAGMA_API glClipPlane;opengl32!glClipPlane
#define PRAGMA_API glColor3b;opengl32!glColor3b
#define PRAGMA_API glColor3bv;opengl32!glColor3bv
#define PRAGMA_API glColor3d;opengl32!glColor3d
#define PRAGMA_API glColor3dv;opengl32!glColor3dv
#define PRAGMA_API glColor3f;opengl32!glColor3f
#define PRAGMA_API glColor3fv;opengl32!glColor3fv
#define PRAGMA_API glColor3i;opengl32!glColor3i
#define PRAGMA_API glColor3iv;opengl32!glColor3iv
#define PRAGMA_API glColor3s;opengl32!glColor3s
#define PRAGMA_API glColor3sv;opengl32!glColor3sv
#define PRAGMA_API glColor3ub;opengl32!glColor3ub
#define PRAGMA_API glColor3ubv;opengl32!glColor3ubv
#define PRAGMA_API glColor3ui;opengl32!glColor3ui
#define PRAGMA_API glColor3uiv;opengl32!glColor3uiv
#define PRAGMA_API glColor3us;opengl32!glColor3us
#define PRAGMA_API glColor3usv;opengl32!glColor3usv
#define PRAGMA_API glColor4b;opengl32!glColor4b
#define PRAGMA_API glColor4bv;opengl32!glColor4bv
#define PRAGMA_API glColor4d;opengl32!glColor4d
#define PRAGMA_API glColor4dv;opengl32!glColor4dv
#define PRAGMA_API glColor4f;opengl32!glColor4f
#define PRAGMA_API glColor4fv;opengl32!glColor4fv
#define PRAGMA_API glColor4i;opengl32!glColor4i
#define PRAGMA_API glColor4iv;opengl32!glColor4iv
#define PRAGMA_API glColor4s;opengl32!glColor4s
#define PRAGMA_API glColor4sv;opengl32!glColor4sv
#define PRAGMA_API glColor4ub;opengl32!glColor4ub
#define PRAGMA_API glColor4ubv;opengl32!glColor4ubv
#define PRAGMA_API glColor4ui;opengl32!glColor4ui
#define PRAGMA_API glColor4uiv;opengl32!glColor4uiv
#define PRAGMA_API glColor4us;opengl32!glColor4us
#define PRAGMA_API glColor4usv;opengl32!glColor4usv
#define PRAGMA_API glColorMask;opengl32!glColorMask
#define PRAGMA_API glColorMaterial;opengl32!glColorMaterial
#define PRAGMA_API glColorPointer;opengl32!glColorPointer
#define PRAGMA_API glCopyPixels;opengl32!glCopyPixels
#define PRAGMA_API glCopyTexImage1D;opengl32!glCopyTexImage1D
#define PRAGMA_API glCopyTexImage2D;opengl32!glCopyTexImage2D
#define PRAGMA_API glCopyTexSubImage1D;opengl32!glCopyTexSubImage1D
#define PRAGMA_API glCopyTexSubImage2D;opengl32!glCopyTexSubImage2D
#define PRAGMA_API glCullFace;opengl32!glCullFace
#define PRAGMA_API glDebugEntry;opengl32!glDebugEntry
#define PRAGMA_API glDeleteLists;opengl32!glDeleteLists
#define PRAGMA_API glDeleteTextures;opengl32!glDeleteTextures
#define PRAGMA_API glDepthFunc;opengl32!glDepthFunc
#define PRAGMA_API glDepthMask;opengl32!glDepthMask
#define PRAGMA_API glDepthRange;opengl32!glDepthRange
#define PRAGMA_API glDisable;opengl32!glDisable
#define PRAGMA_API glDisableClientState;opengl32!glDisableClientState
#define PRAGMA_API glDrawArrays;opengl32!glDrawArrays
#define PRAGMA_API glDrawBuffer;opengl32!glDrawBuffer
#define PRAGMA_API glDrawElements;opengl32!glDrawElements
#define PRAGMA_API glDrawPixels;opengl32!glDrawPixels
#define PRAGMA_API glEdgeFlag;opengl32!glEdgeFlag
#define PRAGMA_API glEdgeFlagPointer;opengl32!glEdgeFlagPointer
#define PRAGMA_API glEdgeFlagv;opengl32!glEdgeFlagv
#define PRAGMA_API glEnable;opengl32!glEnable
#define PRAGMA_API glEnableClientState;opengl32!glEnableClientState
#define PRAGMA_API glEnd;opengl32!glEnd
#define PRAGMA_API glEndList;opengl32!glEndList
#define PRAGMA_API glEvalCoord1d;opengl32!glEvalCoord1d
#define PRAGMA_API glEvalCoord1dv;opengl32!glEvalCoord1dv
#define PRAGMA_API glEvalCoord1f;opengl32!glEvalCoord1f
#define PRAGMA_API glEvalCoord1fv;opengl32!glEvalCoord1fv
#define PRAGMA_API glEvalCoord2d;opengl32!glEvalCoord2d
#define PRAGMA_API glEvalCoord2dv;opengl32!glEvalCoord2dv
#define PRAGMA_API glEvalCoord2f;opengl32!glEvalCoord2f
#define PRAGMA_API glEvalCoord2fv;opengl32!glEvalCoord2fv
#define PRAGMA_API glEvalMesh1;opengl32!glEvalMesh1
#define PRAGMA_API glEvalMesh2;opengl32!glEvalMesh2
#define PRAGMA_API glEvalPoint1;opengl32!glEvalPoint1
#define PRAGMA_API glEvalPoint2;opengl32!glEvalPoint2
#define PRAGMA_API glFeedbackBuffer;opengl32!glFeedbackBuffer
#define PRAGMA_API glFinish;opengl32!glFinish
#define PRAGMA_API glFlush;opengl32!glFlush
#define PRAGMA_API glFogf;opengl32!glFogf
#define PRAGMA_API glFogfv;opengl32!glFogfv
#define PRAGMA_API glFogi;opengl32!glFogi
#define PRAGMA_API glFogiv;opengl32!glFogiv
#define PRAGMA_API glFrontFace;opengl32!glFrontFace
#define PRAGMA_API glFrustum;opengl32!glFrustum
#define PRAGMA_API glGenLists;opengl32!glGenLists
#define PRAGMA_API glGenTextures;opengl32!glGenTextures
#define PRAGMA_API glGetBooleanv;opengl32!glGetBooleanv
#define PRAGMA_API glGetClipPlane;opengl32!glGetClipPlane
#define PRAGMA_API glGetDoublev;opengl32!glGetDoublev
#define PRAGMA_API glGetError;opengl32!glGetError
#define PRAGMA_API glGetFloatv;opengl32!glGetFloatv
#define PRAGMA_API glGetIntegerv;opengl32!glGetIntegerv
#define PRAGMA_API glGetLightfv;opengl32!glGetLightfv
#define PRAGMA_API glGetLightiv;opengl32!glGetLightiv
#define PRAGMA_API glGetMapdv;opengl32!glGetMapdv
#define PRAGMA_API glGetMapfv;opengl32!glGetMapfv
#define PRAGMA_API glGetMapiv;opengl32!glGetMapiv
#define PRAGMA_API glGetMaterialfv;opengl32!glGetMaterialfv
#define PRAGMA_API glGetMaterialiv;opengl32!glGetMaterialiv
#define PRAGMA_API glGetPixelMapfv;opengl32!glGetPixelMapfv
#define PRAGMA_API glGetPixelMapuiv;opengl32!glGetPixelMapuiv
#define PRAGMA_API glGetPixelMapusv;opengl32!glGetPixelMapusv
#define PRAGMA_API glGetPointerv;opengl32!glGetPointerv
#define PRAGMA_API glGetPolygonStipple;opengl32!glGetPolygonStipple
#define PRAGMA_API glGetString;opengl32!glGetString
#define PRAGMA_API glGetTexEnvfv;opengl32!glGetTexEnvfv
#define PRAGMA_API glGetTexEnviv;opengl32!glGetTexEnviv
#define PRAGMA_API glGetTexGendv;opengl32!glGetTexGendv
#define PRAGMA_API glGetTexGenfv;opengl32!glGetTexGenfv
#define PRAGMA_API glGetTexGeniv;opengl32!glGetTexGeniv
#define PRAGMA_API glGetTexImage;opengl32!glGetTexImage
#define PRAGMA_API glGetTexLevelParameterfv;opengl32!glGetTexLevelParameterfv
#define PRAGMA_API glGetTexLevelParameteriv;opengl32!glGetTexLevelParameteriv
#define PRAGMA_API glGetTexParameterfv;opengl32!glGetTexParameterfv
#define PRAGMA_API glGetTexParameteriv;opengl32!glGetTexParameteriv
#define PRAGMA_API glHint;opengl32!glHint
#define PRAGMA_API glIndexMask;opengl32!glIndexMask
#define PRAGMA_API glIndexPointer;opengl32!glIndexPointer
#define PRAGMA_API glIndexd;opengl32!glIndexd
#define PRAGMA_API glIndexdv;opengl32!glIndexdv
#define PRAGMA_API glIndexf;opengl32!glIndexf
#define PRAGMA_API glIndexfv;opengl32!glIndexfv
#define PRAGMA_API glIndexi;opengl32!glIndexi
#define PRAGMA_API glIndexiv;opengl32!glIndexiv
#define PRAGMA_API glIndexs;opengl32!glIndexs
#define PRAGMA_API glIndexsv;opengl32!glIndexsv
#define PRAGMA_API glIndexub;opengl32!glIndexub
#define PRAGMA_API glIndexubv;opengl32!glIndexubv
#define PRAGMA_API glInitNames;opengl32!glInitNames
#define PRAGMA_API glInterleavedArrays;opengl32!glInterleavedArrays
#define PRAGMA_API glIsEnabled;opengl32!glIsEnabled
#define PRAGMA_API glIsList;opengl32!glIsList
#define PRAGMA_API glIsTexture;opengl32!glIsTexture
#define PRAGMA_API glLightModelf;opengl32!glLightModelf
#define PRAGMA_API glLightModelfv;opengl32!glLightModelfv
#define PRAGMA_API glLightModeli;opengl32!glLightModeli
#define PRAGMA_API glLightModeliv;opengl32!glLightModeliv
#define PRAGMA_API glLightf;opengl32!glLightf
#define PRAGMA_API glLightfv;opengl32!glLightfv
#define PRAGMA_API glLighti;opengl32!glLighti
#define PRAGMA_API glLightiv;opengl32!glLightiv
#define PRAGMA_API glLineStipple;opengl32!glLineStipple
#define PRAGMA_API glLineWidth;opengl32!glLineWidth
#define PRAGMA_API glListBase;opengl32!glListBase
#define PRAGMA_API glLoadIdentity;opengl32!glLoadIdentity
#define PRAGMA_API glLoadMatrixd;opengl32!glLoadMatrixd
#define PRAGMA_API glLoadMatrixf;opengl32!glLoadMatrixf
#define PRAGMA_API glLoadName;opengl32!glLoadName
#define PRAGMA_API glLogicOp;opengl32!glLogicOp
#define PRAGMA_API glMap1d;opengl32!glMap1d
#define PRAGMA_API glMap1f;opengl32!glMap1f
#define PRAGMA_API glMap2d;opengl32!glMap2d
#define PRAGMA_API glMap2f;opengl32!glMap2f
#define PRAGMA_API glMapGrid1d;opengl32!glMapGrid1d
#define PRAGMA_API glMapGrid1f;opengl32!glMapGrid1f
#define PRAGMA_API glMapGrid2d;opengl32!glMapGrid2d
#define PRAGMA_API glMapGrid2f;opengl32!glMapGrid2f
#define PRAGMA_API glMaterialf;opengl32!glMaterialf
#define PRAGMA_API glMaterialfv;opengl32!glMaterialfv
#define PRAGMA_API glMateriali;opengl32!glMateriali
#define PRAGMA_API glMaterialiv;opengl32!glMaterialiv
#define PRAGMA_API glMatrixMode;opengl32!glMatrixMode
#define PRAGMA_API glMultMatrixd;opengl32!glMultMatrixd
#define PRAGMA_API glMultMatrixf;opengl32!glMultMatrixf
#define PRAGMA_API glNewList;opengl32!glNewList
#define PRAGMA_API glNormal3b;opengl32!glNormal3b
#define PRAGMA_API glNormal3bv;opengl32!glNormal3bv
#define PRAGMA_API glNormal3d;opengl32!glNormal3d
#define PRAGMA_API glNormal3dv;opengl32!glNormal3dv
#define PRAGMA_API glNormal3f;opengl32!glNormal3f
#define PRAGMA_API glNormal3fv;opengl32!glNormal3fv
#define PRAGMA_API glNormal3i;opengl32!glNormal3i
#define PRAGMA_API glNormal3iv;opengl32!glNormal3iv
#define PRAGMA_API glNormal3s;opengl32!glNormal3s
#define PRAGMA_API glNormal3sv;opengl32!glNormal3sv
#define PRAGMA_API glNormalPointer;opengl32!glNormalPointer
#define PRAGMA_API glOrtho;opengl32!glOrtho
#define PRAGMA_API glPassThrough;opengl32!glPassThrough
#define PRAGMA_API glPixelMapfv;opengl32!glPixelMapfv
#define PRAGMA_API glPixelMapuiv;opengl32!glPixelMapuiv
#define PRAGMA_API glPixelMapusv;opengl32!glPixelMapusv
#define PRAGMA_API glPixelStoref;opengl32!glPixelStoref
#define PRAGMA_API glPixelStorei;opengl32!glPixelStorei
#define PRAGMA_API glPixelTransferf;opengl32!glPixelTransferf
#define PRAGMA_API glPixelTransferi;opengl32!glPixelTransferi
#define PRAGMA_API glPixelZoom;opengl32!glPixelZoom
#define PRAGMA_API glPointSize;opengl32!glPointSize
#define PRAGMA_API glPolygonMode;opengl32!glPolygonMode
#define PRAGMA_API glPolygonOffset;opengl32!glPolygonOffset
#define PRAGMA_API glPolygonStipple;opengl32!glPolygonStipple
#define PRAGMA_API glPopAttrib;opengl32!glPopAttrib
#define PRAGMA_API glPopClientAttrib;opengl32!glPopClientAttrib
#define PRAGMA_API glPopMatrix;opengl32!glPopMatrix
#define PRAGMA_API glPopName;opengl32!glPopName
#define PRAGMA_API glPrioritizeTextures;opengl32!glPrioritizeTextures
#define PRAGMA_API glPushAttrib;opengl32!glPushAttrib
#define PRAGMA_API glPushClientAttrib;opengl32!glPushClientAttrib
#define PRAGMA_API glPushMatrix;opengl32!glPushMatrix
#define PRAGMA_API glPushName;opengl32!glPushName
#define PRAGMA_API glRasterPos2d;opengl32!glRasterPos2d
#define PRAGMA_API glRasterPos2dv;opengl32!glRasterPos2dv
#define PRAGMA_API glRasterPos2f;opengl32!glRasterPos2f
#define PRAGMA_API glRasterPos2fv;opengl32!glRasterPos2fv
#define PRAGMA_API glRasterPos2i;opengl32!glRasterPos2i
#define PRAGMA_API glRasterPos2iv;opengl32!glRasterPos2iv
#define PRAGMA_API glRasterPos2s;opengl32!glRasterPos2s
#define PRAGMA_API glRasterPos2sv;opengl32!glRasterPos2sv
#define PRAGMA_API glRasterPos3d;opengl32!glRasterPos3d
#define PRAGMA_API glRasterPos3dv;opengl32!glRasterPos3dv
#define PRAGMA_API glRasterPos3f;opengl32!glRasterPos3f
#define PRAGMA_API glRasterPos3fv;opengl32!glRasterPos3fv
#define PRAGMA_API glRasterPos3i;opengl32!glRasterPos3i
#define PRAGMA_API glRasterPos3iv;opengl32!glRasterPos3iv
#define PRAGMA_API glRasterPos3s;opengl32!glRasterPos3s
#define PRAGMA_API glRasterPos3sv;opengl32!glRasterPos3sv
#define PRAGMA_API glRasterPos4d;opengl32!glRasterPos4d
#define PRAGMA_API glRasterPos4dv;opengl32!glRasterPos4dv
#define PRAGMA_API glRasterPos4f;opengl32!glRasterPos4f
#define PRAGMA_API glRasterPos4fv;opengl32!glRasterPos4fv
#define PRAGMA_API glRasterPos4i;opengl32!glRasterPos4i
#define PRAGMA_API glRasterPos4iv;opengl32!glRasterPos4iv
#define PRAGMA_API glRasterPos4s;opengl32!glRasterPos4s
#define PRAGMA_API glRasterPos4sv;opengl32!glRasterPos4sv
#define PRAGMA_API glReadBuffer;opengl32!glReadBuffer
#define PRAGMA_API glReadPixels;opengl32!glReadPixels
#define PRAGMA_API glRectd;opengl32!glRectd
#define PRAGMA_API glRectdv;opengl32!glRectdv
#define PRAGMA_API glRectf;opengl32!glRectf
#define PRAGMA_API glRectfv;opengl32!glRectfv
#define PRAGMA_API glRecti;opengl32!glRecti
#define PRAGMA_API glRectiv;opengl32!glRectiv
#define PRAGMA_API glRects;opengl32!glRects
#define PRAGMA_API glRectsv;opengl32!glRectsv
#define PRAGMA_API glRenderMode;opengl32!glRenderMode
#define PRAGMA_API glRotated;opengl32!glRotated
#define PRAGMA_API glRotatef;opengl32!glRotatef
#define PRAGMA_API glScaled;opengl32!glScaled
#define PRAGMA_API glScalef;opengl32!glScalef
#define PRAGMA_API glScissor;opengl32!glScissor
#define PRAGMA_API glSelectBuffer;opengl32!glSelectBuffer
#define PRAGMA_API glShadeModel;opengl32!glShadeModel
#define PRAGMA_API glStencilFunc;opengl32!glStencilFunc
#define PRAGMA_API glStencilMask;opengl32!glStencilMask
#define PRAGMA_API glStencilOp;opengl32!glStencilOp
#define PRAGMA_API glTexCoord1d;opengl32!glTexCoord1d
#define PRAGMA_API glTexCoord1dv;opengl32!glTexCoord1dv
#define PRAGMA_API glTexCoord1f;opengl32!glTexCoord1f
#define PRAGMA_API glTexCoord1fv;opengl32!glTexCoord1fv
#define PRAGMA_API glTexCoord1i;opengl32!glTexCoord1i
#define PRAGMA_API glTexCoord1iv;opengl32!glTexCoord1iv
#define PRAGMA_API glTexCoord1s;opengl32!glTexCoord1s
#define PRAGMA_API glTexCoord1sv;opengl32!glTexCoord1sv
#define PRAGMA_API glTexCoord2d;opengl32!glTexCoord2d
#define PRAGMA_API glTexCoord2dv;opengl32!glTexCoord2dv
#define PRAGMA_API glTexCoord2f;opengl32!glTexCoord2f
#define PRAGMA_API glTexCoord2fv;opengl32!glTexCoord2fv
#define PRAGMA_API glTexCoord2i;opengl32!glTexCoord2i
#define PRAGMA_API glTexCoord2iv;opengl32!glTexCoord2iv
#define PRAGMA_API glTexCoord2s;opengl32!glTexCoord2s
#define PRAGMA_API glTexCoord2sv;opengl32!glTexCoord2sv
#define PRAGMA_API glTexCoord3d;opengl32!glTexCoord3d
#define PRAGMA_API glTexCoord3dv;opengl32!glTexCoord3dv
#define PRAGMA_API glTexCoord3f;opengl32!glTexCoord3f
#define PRAGMA_API glTexCoord3fv;opengl32!glTexCoord3fv
#define PRAGMA_API glTexCoord3i;opengl32!glTexCoord3i
#define PRAGMA_API glTexCoord3iv;opengl32!glTexCoord3iv
#define PRAGMA_API glTexCoord3s;opengl32!glTexCoord3s
#define PRAGMA_API glTexCoord3sv;opengl32!glTexCoord3sv
#define PRAGMA_API glTexCoord4d;opengl32!glTexCoord4d
#define PRAGMA_API glTexCoord4dv;opengl32!glTexCoord4dv
#define PRAGMA_API glTexCoord4f;opengl32!glTexCoord4f
#define PRAGMA_API glTexCoord4fv;opengl32!glTexCoord4fv
#define PRAGMA_API glTexCoord4i;opengl32!glTexCoord4i
#define PRAGMA_API glTexCoord4iv;opengl32!glTexCoord4iv
#define PRAGMA_API glTexCoord4s;opengl32!glTexCoord4s
#define PRAGMA_API glTexCoord4sv;opengl32!glTexCoord4sv
#define PRAGMA_API glTexCoordPointer;opengl32!glTexCoordPointer
#define PRAGMA_API glTexEnvf;opengl32!glTexEnvf
#define PRAGMA_API glTexEnvfv;opengl32!glTexEnvfv
#define PRAGMA_API glTexEnvi;opengl32!glTexEnvi
#define PRAGMA_API glTexEnviv;opengl32!glTexEnviv
#define PRAGMA_API glTexGend;opengl32!glTexGend
#define PRAGMA_API glTexGendv;opengl32!glTexGendv
#define PRAGMA_API glTexGenf;opengl32!glTexGenf
#define PRAGMA_API glTexGenfv;opengl32!glTexGenfv
#define PRAGMA_API glTexGeni;opengl32!glTexGeni
#define PRAGMA_API glTexGeniv;opengl32!glTexGeniv
#define PRAGMA_API glTexImage1D;opengl32!glTexImage1D
#define PRAGMA_API glTexImage2D;opengl32!glTexImage2D
#define PRAGMA_API glTexParameterf;opengl32!glTexParameterf
#define PRAGMA_API glTexParameterfv;opengl32!glTexParameterfv
#define PRAGMA_API glTexParameteri;opengl32!glTexParameteri
#define PRAGMA_API glTexParameteriv;opengl32!glTexParameteriv
#define PRAGMA_API glTexSubImage1D;opengl32!glTexSubImage1D
#define PRAGMA_API glTexSubImage2D;opengl32!glTexSubImage2D
#define PRAGMA_API glTranslated;opengl32!glTranslated
#define PRAGMA_API glTranslatef;opengl32!glTranslatef
#define PRAGMA_API glVertex2d;opengl32!glVertex2d
#define PRAGMA_API glVertex2dv;opengl32!glVertex2dv
#define PRAGMA_API glVertex2f;opengl32!glVertex2f
#define PRAGMA_API glVertex2fv;opengl32!glVertex2fv
#define PRAGMA_API glVertex2i;opengl32!glVertex2i
#define PRAGMA_API glVertex2iv;opengl32!glVertex2iv
#define PRAGMA_API glVertex2s;opengl32!glVertex2s
#define PRAGMA_API glVertex2sv;opengl32!glVertex2sv
#define PRAGMA_API glVertex3d;opengl32!glVertex3d
#define PRAGMA_API glVertex3dv;opengl32!glVertex3dv
#define PRAGMA_API glVertex3f;opengl32!glVertex3f
#define PRAGMA_API glVertex3fv;opengl32!glVertex3fv
#define PRAGMA_API glVertex3i;opengl32!glVertex3i
#define PRAGMA_API glVertex3iv;opengl32!glVertex3iv
#define PRAGMA_API glVertex3s;opengl32!glVertex3s
#define PRAGMA_API glVertex3sv;opengl32!glVertex3sv
#define PRAGMA_API glVertex4d;opengl32!glVertex4d
#define PRAGMA_API glVertex4dv;opengl32!glVertex4dv
#define PRAGMA_API glVertex4f;opengl32!glVertex4f
#define PRAGMA_API glVertex4fv;opengl32!glVertex4fv
#define PRAGMA_API glVertex4i;opengl32!glVertex4i
#define PRAGMA_API glVertex4iv;opengl32!glVertex4iv
#define PRAGMA_API glVertex4s;opengl32!glVertex4s
#define PRAGMA_API glVertex4sv;opengl32!glVertex4sv
#define PRAGMA_API glVertexPointer;opengl32!glVertexPointer
#define PRAGMA_API glViewport;opengl32!glViewport
#define PRAGMA_API wglChoosePixelFormat;opengl32!wglChoosePixelFormat
#define PRAGMA_API wglCopyContext;opengl32!wglCopyContext
#define PRAGMA_API wglCreateContext;opengl32!wglCreateContext
#define PRAGMA_API wglCreateLayerContext;opengl32!wglCreateLayerContext
#define PRAGMA_API wglDeleteContext;opengl32!wglDeleteContext
#define PRAGMA_API wglDescribeLayerPlane;opengl32!wglDescribeLayerPlane
#define PRAGMA_API wglDescribePixelFormat;opengl32!wglDescribePixelFormat
#define PRAGMA_API wglGetCurrentContext;opengl32!wglGetCurrentContext
#define PRAGMA_API wglGetCurrentDC;opengl32!wglGetCurrentDC
#define PRAGMA_API wglGetDefaultProcAddress;opengl32!wglGetDefaultProcAddress
#define PRAGMA_API wglGetLayerPaletteEntries;opengl32!wglGetLayerPaletteEntries
#define PRAGMA_API wglGetPixelFormat;opengl32!wglGetPixelFormat
#define PRAGMA_API wglGetProcAddress;opengl32!wglGetProcAddress
#define PRAGMA_API wglMakeCurrent;opengl32!wglMakeCurrent
#define PRAGMA_API wglRealizeLayerPalette;opengl32!wglRealizeLayerPalette
#define PRAGMA_API wglSetLayerPaletteEntries;opengl32!wglSetLayerPaletteEntries
#define PRAGMA_API wglSetPixelFormat;opengl32!wglSetPixelFormat
#define PRAGMA_API wglShareLists;opengl32!wglShareLists
#define PRAGMA_API wglSwapBuffers;opengl32!wglSwapBuffers
#define PRAGMA_API wglSwapLayerBuffers;opengl32!wglSwapLayerBuffers
#define PRAGMA_API wglUseFontBitmapsA;opengl32!wglUseFontBitmapsA
#define PRAGMA_API wglUseFontBitmapsW;opengl32!wglUseFontBitmapsW
#define PRAGMA_API wglUseFontOutlinesA;opengl32!wglUseFontOutlinesA
#define PRAGMA_API wglUseFontOutlinesW;opengl32!wglUseFontOutlinesW

#define PRAGMA_API gluBeginCurve;glu32!gluBeginCurve
#define PRAGMA_API gluBeginPolygon;glu32!gluBeginPolygon
#define PRAGMA_API gluBeginSurface;glu32!gluBeginSurface
#define PRAGMA_API gluBeginTrim;glu32!gluBeginTrim
#define PRAGMA_API gluBuild1DMipmaps;glu32!gluBuild1DMipmaps
#define PRAGMA_API gluBuild2DMipmaps;glu32!gluBuild2DMipmaps
#define PRAGMA_API gluCylinder;glu32!gluCylinder
#define PRAGMA_API gluDeleteNurbsRenderer;glu32!gluDeleteNurbsRenderer
#define PRAGMA_API gluDeleteQuadric;glu32!gluDeleteQuadric
#define PRAGMA_API gluDeleteTess;glu32!gluDeleteTess
#define PRAGMA_API gluDisk;glu32!gluDisk
#define PRAGMA_API gluEndCurve;glu32!gluEndCurve
#define PRAGMA_API gluEndPolygon;glu32!gluEndPolygon
#define PRAGMA_API gluEndSurface;glu32!gluEndSurface
#define PRAGMA_API gluEndTrim;glu32!gluEndTrim
#define PRAGMA_API gluErrorString;glu32!gluErrorString
#define PRAGMA_API gluErrorUnicodeStringEXT;glu32!gluErrorUnicodeStringEXT
#define PRAGMA_API gluGetNurbsProperty;glu32!gluGetNurbsProperty
#define PRAGMA_API gluGetString;glu32!gluGetString
#define PRAGMA_API gluGetTessProperty;glu32!gluGetTessProperty
#define PRAGMA_API gluLoadSamplingMatrices;glu32!gluLoadSamplingMatrices
#define PRAGMA_API gluLookAt;glu32!gluLookAt
#define PRAGMA_API gluNewNurbsRenderer;glu32!gluNewNurbsRenderer
#define PRAGMA_API gluNewQuadric;glu32!gluNewQuadric
#define PRAGMA_API gluNewTess;glu32!gluNewTess
#define PRAGMA_API gluNextContour;glu32!gluNextContour
#define PRAGMA_API gluNurbsCallback;glu32!gluNurbsCallback
#define PRAGMA_API gluNurbsCurve;glu32!gluNurbsCurve
#define PRAGMA_API gluNurbsProperty;glu32!gluNurbsProperty
#define PRAGMA_API gluNurbsSurface;glu32!gluNurbsSurface
#define PRAGMA_API gluOrtho2D;glu32!gluOrtho2D
#define PRAGMA_API gluPartialDisk;glu32!gluPartialDisk
#define PRAGMA_API gluPerspective;glu32!gluPerspective
#define PRAGMA_API gluPickMatrix;glu32!gluPickMatrix
#define PRAGMA_API gluProject;glu32!gluProject
#define PRAGMA_API gluPwlCurve;glu32!gluPwlCurve
#define PRAGMA_API gluQuadricCallback;glu32!gluQuadricCallback
#define PRAGMA_API gluQuadricDrawStyle;glu32!gluQuadricDrawStyle
#define PRAGMA_API gluQuadricNormals;glu32!gluQuadricNormals
#define PRAGMA_API gluQuadricOrientation;glu32!gluQuadricOrientation
#define PRAGMA_API gluQuadricTexture;glu32!gluQuadricTexture
#define PRAGMA_API gluScaleImage;glu32!gluScaleImage
#define PRAGMA_API gluSphere;glu32!gluSphere
#define PRAGMA_API gluTessBeginContour;glu32!gluTessBeginContour
#define PRAGMA_API gluTessBeginPolygon;glu32!gluTessBeginPolygon
#define PRAGMA_API gluTessCallback;glu32!gluTessCallback
#define PRAGMA_API gluTessEndContour;glu32!gluTessEndContour
#define PRAGMA_API gluTessEndPolygon;glu32!gluTessEndPolygon
#define PRAGMA_API gluTessNormal;glu32!gluTessNormal
#define PRAGMA_API gluTessProperty;glu32!gluTessProperty
#define PRAGMA_API gluTessVertex;glu32!gluTessVertex
#define PRAGMA_API gluUnProject;glu32!gluUnProject

#ifdef GLAUX
#define PRAGMA_API auxSolidBox;glaux!auxSolidBox
#define PRAGMA_API auxSolidCone;glaux!auxSolidCone
#define PRAGMA_API auxSolidCube;glaux!auxSolidCube
#define PRAGMA_API auxSolidCylinder;glaux!auxSolidCylinder
#define PRAGMA_API auxSolidDodecahedr;glaux!auxSolidDodecahedron
#define PRAGMA_API auxSolidIcosahedro;glaux!auxSolidIcosahedron
#define PRAGMA_API auxSolidOctahedron;glaux!auxSolidOctahedron
#define PRAGMA_API auxSolidSphere;glaux!auxSolidSphere
#define PRAGMA_API auxSolidTetrahedro;glaux!auxSolidTetrahedron
#define PRAGMA_API auxSolidTorus;glaux!auxSolidTorus
#define PRAGMA_API auxWireBox;glaux!auxWireBox
#define PRAGMA_API auxWireCone;glaux!auxWireCone
#define PRAGMA_API auxWireCube;glaux!auxWireCube
#define PRAGMA_API auxWireCylinder;glaux!auxWireCylinder
#define PRAGMA_API auxWireDodecahedro;glaux!auxWireDodecahedron
#define PRAGMA_API auxWireIcosahedron;glaux!auxWireIcosahedron
#define PRAGMA_API auxWireOctahedron;glaux!auxWireOctahedron
#define PRAGMA_API auxWireSphere;glaux!auxWireSphere
#define PRAGMA_API auxWireTeapot;glaux!auxWireTeapot
#define PRAGMA_API auxWireTetrahedron;glaux!auxWireTetrahedron
#define PRAGMA_API auxWireTorus;glaux!auxWireTorus
#define PRAGMA_API auxSolidTeapot;glaux!auxSolidTeapot
#endif

#endif	//opengl_h
