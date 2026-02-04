// Open-source VST2 header replacement
// Compatible with JUCE VST3 wrapper
// Based on public VST2 documentation

#pragma once

// Basic VST2 types and constants
typedef intptr_t intVST;
typedef intVST VSTInt;
typedef float VSTFloat;

// VST2 constants
#define cEffectMagic 'Vst'
#define effFlagsHasEditor 1
#define effFlagsHasClip (1 << 1)
#define effFlagsHasVU (1 << 2)
#define effFlagsCanReplacing (1 << 4)
#define effFlagsIsSynth (1 << 8)
#define effFlagsNoSoundInPlace (1 << 9)
#define effFlagsExtIsAsync (1 << 10)
#define effFlagsExtHasBuffer (1 << 11)
#define effFlagsCanDoubleReplacing (1 << 12)

#define kVstMaxEffectNameLen 64
#define kVstMaxParamStrLen 64
#define kVstMaxProgNameLen 64
#define kVstMaxVendorNameLen 64
#define kVstMaxProductNameLen 64

// AEffect structure
struct AEffect {
    int32_t magic;
    int32_t (*dispatcher)(AEffect*, int32_t, int32_t, int32_t, void*, float);
    float (*process)(AEffect*, float*, float**, int32_t);
    void (*processReplacing)(AEffect*, float**, float**, int32_t);
    void (*processDoubleReplacing)(AEffect*, double**, double**, int32_t);
    int32_t setProcessPrecision;
    int32_t numParams;
    int32_t numPrograms;
    int32_t numInputs;
    int32_t numOutputs;
    int32_t flags;
    int32_t reserved1;
    int32_t reserved2;
    int32_t initialDelay;
    int32_t realQualities;
    int32_t offQualities;
    float ioRatio;
    int32_t reserved3;
    int32_t uniqueID;
    int32_t version;
    void* processPrecisionPtr;
    int32_t reserved4;
    int32_t id;
    int32_t reserved5[4];
};

// Dispatcher opcodes
enum {
    effOpen = 0,
    effClose,
    effSetProgram,
    effGetProgram,
    effSetProgramName,
    effGetProgramName,
    effGetParamLabel,
    effGetParamDisplay,
    effGetParamName,
    effGetProgram,
    effSetSampleRate,
    effSetBlockSize,
    effMainsChanged,
    effStartProcess,
    effStopProcess,
    effSetTotalSampleToProcess,
    effProcessEvents,
    effCanBeAutomated,
    effString2Parameter,
    effGetNumProgramCategories,
    effGetProgramNameIndexed,
    effCopyProgram,
    effExchangeState,
    effPending,
    effGetProcessLevel,
    effGetCurrentPosition,
    effGetOutputProperties,
    effGetPlugCategory,
    effGetManufacturerName,
    effGetManufacturerProduct,
    effGetManufacturerVersion,
    effVendorSpecific,
    effSetSpeakerArrangement,
    effSetPanLaw,
    effBeginLoad,
    effSetContentScale,
    effGetEffectName,
    effGetErrorText,
    effGetVendorVersion,
    effVendorSpecific,
    effGetProductName,
    effGetDescription,
    effGetLabel,
    effGetAuthor,
    effGetVersion,
    effDllMain,
    effEditDraw,
    effEditMouse,
    effEditKey,
    effSetEditKnobMode,
    effGetMidiProgramName,
    effBeginEdit,
    effEndEdit,
    effGetParameterProperties,
    effGetVstVersion,
    effShellGetNextPlugin,
    effStartProcess,
    effStopProcess,
    effSetSurround,
    effGetSurround,
    effGetProgramPitch,
    effGetNumParams,
    effGetParameterInfo
};

// Parameter properties
struct VstParameterProperties {
    int32_t flags;
    float minValue;
    float maxValue;
    float defaultValue;
    int32_t stepFloatStep;
    int32_t stepIntegerStep;
    int32_t largeStepFloatStep;
    int32_t largeStepIntegerStep;
    int16_t shortLabelSize;
    char shortLabel[16];
    int32_t displayIndex;
    int32_t reserved;
    char reserved1[32];
    int32_t reserved2;
    int32_t flags2;
    int32_t reserved3[8];
};

// VST2 to VST3 compatibility - basic stubs
namespace Vst2 {
    struct AEffect;
}
