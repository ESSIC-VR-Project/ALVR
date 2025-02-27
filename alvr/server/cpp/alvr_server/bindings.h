#pragma once

struct FfiFov {
    float left;
    float right;
    float up;
    float down;
};

struct FfiQuat {
    float x;
    float y;
    float z;
    float w;
};

struct FfiHandSkeleton {
    float jointPositions[26][3];
    FfiQuat jointRotations[26];
};

struct FfiDeviceMotion {
    unsigned long long deviceID;
    FfiQuat orientation;
    float position[3];
    float linearVelocity[3];
    float angularVelocity[3];
};

enum FfiOpenvrPropertyType {
    Bool,
    Float,
    Int32,
    Uint64,
    Vector3,
    Double,
    String,
};

union FfiOpenvrPropertyValue {
    bool bool_;
    float float_;
    int int32;
    unsigned long long uint64;
    float vector3[3];
    double double_;
    char string[64];
};

struct FfiOpenvrProperty {
    unsigned int key;
    FfiOpenvrPropertyType type;
    FfiOpenvrPropertyValue value;
};

struct FfiViewsConfig {
    FfiFov fov[2];
    float ipd_m;
};

enum FfiButtonType {
    BUTTON_TYPE_BINARY,
    BUTTON_TYPE_SCALAR,
};

struct FfiButtonValue {
    FfiButtonType type;
    union {
        bool binary;
        float scalar;
    };
};

extern "C" const unsigned char *FRAME_RENDER_VS_CSO_PTR;
extern "C" unsigned int FRAME_RENDER_VS_CSO_LEN;
extern "C" const unsigned char *FRAME_RENDER_PS_CSO_PTR;
extern "C" unsigned int FRAME_RENDER_PS_CSO_LEN;
extern "C" const unsigned char *QUAD_SHADER_CSO_PTR;
extern "C" unsigned int QUAD_SHADER_CSO_LEN;
extern "C" const unsigned char *COMPRESS_AXIS_ALIGNED_CSO_PTR;
extern "C" unsigned int COMPRESS_AXIS_ALIGNED_CSO_LEN;
extern "C" const unsigned char *COLOR_CORRECTION_CSO_PTR;
extern "C" unsigned int COLOR_CORRECTION_CSO_LEN;

extern "C" const unsigned char *QUAD_SHADER_VERT_SPV_PTR;
extern "C" unsigned int QUAD_SHADER_VERT_SPV_LEN;
extern "C" const unsigned char *QUAD_SHADER_FRAG_SPV_PTR;
extern "C" unsigned int QUAD_SHADER_FRAG_SPV_LEN;
extern "C" const unsigned char *COLOR_SHADER_FRAG_SPV_PTR;
extern "C" unsigned int COLOR_SHADER_FRAG_SPV_LEN;
extern "C" const unsigned char *FFR_SHADER_FRAG_SPV_PTR;
extern "C" unsigned int FFR_SHADER_FRAG_SPV_LEN;
extern "C" const unsigned char *RGBTOYUV420_SHADER_COMP_SPV_PTR;
extern "C" unsigned int RGBTOYUV420_SHADER_COMP_SPV_LEN;

extern "C" const char *g_sessionPath;
extern "C" const char *g_driverRootDir;

extern "C" void (*LogError)(const char *stringPtr);
extern "C" void (*LogWarn)(const char *stringPtr);
extern "C" void (*LogInfo)(const char *stringPtr);
extern "C" void (*LogDebug)(const char *stringPtr);
extern "C" void (*LogPeriodically)(const char *tag, const char *stringPtr);
extern "C" void (*DriverReadyIdle)(bool setDefaultChaprone);
extern "C" void (*InitializeDecoder)(const unsigned char *configBuffer, int len);
extern "C" void (*VideoSend)(unsigned long long targetTimestampNs, unsigned char *buf, int len);
extern "C" void (*HapticsSend)(unsigned long long path,
                               float duration_s,
                               float frequency,
                               float amplitude);
extern "C" void (*ShutdownRuntime)();
extern "C" unsigned long long (*PathStringToHash)(const char *path);
extern "C" void (*ReportPresent)(unsigned long long timestamp_ns, unsigned long long offset_ns);
extern "C" void (*ReportComposed)(unsigned long long timestamp_ns, unsigned long long offset_ns);
extern "C" void (*ReportEncoded)(unsigned long long timestamp_ns);

extern "C" void *CppEntryPoint(const char *pInterfaceName, int *pReturnCode);
extern "C" void InitializeStreaming();
extern "C" void DeinitializeStreaming();
extern "C" void SendVSync(float frameIntervalS);
extern "C" void RequestIDR();
extern "C" void SetTracking(unsigned long long targetTimestampNs,
                            float controllerPoseTimeOffsetS,
                            const FfiDeviceMotion *deviceMotions,
                            int motionsCount,
                            const FfiHandSkeleton *leftHand,
                            const FfiHandSkeleton *rightHand);
extern "C" void ReportNetworkLatency(unsigned long long latencyUs);
extern "C" void VideoErrorReportReceive();
extern "C" void ShutdownSteamvr();

extern "C" void SetOpenvrProperty(unsigned long long topLevelPath, FfiOpenvrProperty prop);
extern "C" void SetChaperone(float areaWidth, float areaHeight);
extern "C" void SetViewsConfig(FfiViewsConfig config);
extern "C" void SetBattery(unsigned long long topLevelPath, float gauge_value, bool is_plugged);
extern "C" void SetButton(unsigned long long path, FfiButtonValue value);

extern "C" void SetBitrateParameters(unsigned long long bitrate_mbs,
                                     bool adaptive_bitrate_enabled,
                                     unsigned long long bitrate_max);

extern "C" void CaptureFrame();

// returns true if the bitrate was updated
bool GetUpdatedBitrate(unsigned long long *outBitrateMbs);
void CountPacketABR(int bytes);

// NalParsing.cpp
void ParseFrameNals(unsigned char *buf, int len, unsigned long long targetTimestampNs);