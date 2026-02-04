# DistortionPro

[English](#english) | [中文](#中文)

---

## English

### Overview

DistortionPro is a professional VST3/AU audio plugin that provides four distinct distortion algorithms. Built with JUCE 7, it features real-time visualization, a comprehensive preset system, and high-DPI support.

### Features

- **4 Distortion Types:**
  - **Overdrive**: Smooth tanh-based soft clipping with warm harmonics
  - **Distortion**: Aggressive 5-stage hard clipping
  - **Fuzz**: Square wave synthesis for thick, fuzzy textures
  - **Saturation**: Tape emulation for warm, musical enhancement

- **7 Parameters:**
  - **Drive**: Input gain / distortion intensity
  - **Tone**: High frequency dampening
  - **Output**: Output level (-12 to +12 dB)
  - **Mix**: Dry/wet blend (0-100%)
  - **Depth**: Distortion softness/knee
  - **Attack**: Distortion onset speed
  - **Type**: Distortion algorithm selection
  - **Oversample**: 2x oversampling for reduced aliasing

- **Visual Features:**
  - Real-time input/output waveform display
  - Gain reduction meter
  - High-DPI / Retina support
  - Dark theme UI

- **Preset System:**
  - 6 factory presets
  - User preset save/load
  - Preset import/export (JSON)
  - A/B comparison

### Requirements

- JUCE 7.0+
- C++17 compiler
- CMake 3.15+
- VST3 SDK (included with JUCE)

### Building

#### Windows (Visual Studio)

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

Or using MinGW:

```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build . --config Release
```

#### macOS (Xcode)

```bash
mkdir build
cd build
cmake -G Xcode ..
cmake --build . --config Release
```

#### Linux

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Installation

#### Windows VST3

Copy the built `.vst3` folder to:
```
C:\Program Files\VST3\
```

#### macOS VST3/AU

```bash
# VST3
cp -r build/DistortionPro.vst3 ~/Library/Audio/Plug-Ins/VST3/

# AU (if built)
cp -r build/DistortionPro.component ~/Library/Audio/Plug-Ins/Components/
```

#### Linux VST3

```bash
cp -r build/DistortionPro.vst3 ~/.vst3/
```

### Directory Structure

```
DistortionPro/
├── CMakeLists.txt
├── README.md
├── src/
│   ├── dsp/
│   │   ├── DistortionAlgorithms.h/cpp    # Core distortion algorithms
│   │   ├── DistortionProcessor.h/cpp     # Main DSP processor
│   │   └── Oversampler.h/cpp             # 2x oversampling
│   ├── plugin/
│   │   ├── DistortionPro.h/cpp           # Plugin entry point
│   │   └── DistortionPro.h               # Plugin header
│   ├── ui/
│   │   ├── PluginEditor.h/cpp            # Main editor UI
│   │   ├── KnobComponent.h/cpp           # Rotary knob control
│   │   ├── WaveformDisplay.h/cpp         # Waveform visualization
│   │   ├── GainMeter.h/cpp               # Gain reduction meter
│   │   └── TypeSelector.h/cpp            # Distortion type selector
│   └── presets/
│       └── PresetManager.h/cpp           # Preset management
└── presets/                              # Factory preset files
```

### Parameters Reference

| Parameter | Range | Description |
|-----------|-------|-------------|
| Drive | 0-100% | Input gain before clipping |
| Tone | 0-100% | High frequency dampening |
| Output | -12 to +12 dB | Final output level |
| Mix | 0-100% | Dry/wet mix ratio |
| Depth | 0-100% | Distortion softness (knee) |
| Attack | 0-100% | Distortion onset speed |
| Type | 4 modes | Algorithm: Overdrive/Distortion/Fuzz/Saturation |
| Oversample | On/Off | 2x oversampling |

### Preset JSON Format

```json
{
  "name": "Preset Name",
  "category": "Factory",
  "type": "overdrive",
  "parameters": {
    "drive": 0.5,
    "tone": 0.6,
    "output": 0.8,
    "mix": 1.0,
    "depth": 0.5,
    "attack": 0.5
  }
}
```

### License

MIT License - See LICENSE file for details.

### Credits

- Built with [JUCE Framework](https://juce.com/)
- VST3 SDK included in JUCE

---

## 中文

### 简介

DistortionPro 是一款专业的 VST3/AU 音频插件，提供四种独特的失真算法。基于 JUCE 7 构建，具有实时可视化、完整预设系统和高 DPI 支持。

### 功能特性

- **4 种失真类型：**
  - **Overdrive（过载）**：基于 tanh 的平滑软削波，具有温暖的谐波
  - **Distortion（失真）**：激进的 5 级硬削波
  - **Fuzz（法兹）**：方波合成，厚重模糊的质感
  - **Saturation（饱和）**：磁带仿真，温暖音乐性的增强

- **7 个参数：**
  - **Drive（驱动）**：输入增益 / 失真强度
  - **Tone（音色）**：高频衰减
  - **Output（输出）**：输出电平（-12 到 +12 dB）
  - **Mix（混合）**：干湿信号混合（0-100%）
  - **Depth（深度）**：失真柔和度/拐点
  - **Attack（起音）**：失真启动速度
  - **Type（类型）**：失真算法选择
  - **Oversample（过采样）**：2 倍过采样减少失真

- **可视化功能：**
  - 实时输入/输出波形显示
  - 增益衰减表
  - 高 DPI / Retina 屏幕支持
  - 暗色主题界面

- **预设系统：**
  - 6 个工厂预设
  - 用户预设保存/加载
  - 预设导入/导出（JSON 格式）
  - A/B 对比功能

### 环境要求

- JUCE 7.0+
- C++17 编译器
- CMake 3.15+
- VST3 SDK（JUCE 内置）

### 编译方法

#### Windows（Visual Studio）

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

或使用 MinGW：

```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build . --config Release
```

#### macOS（Xcode）

```bash
mkdir build
cd build
cmake -G Xcode ..
cmake --build . --config Release
```

#### Linux

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### 安装路径

#### Windows VST3

将编译好的 `.vst3` 文件夹复制到：
```
C:\Program Files\VST3\
```

#### macOS VST3/AU

```bash
# VST3
cp -r build/DistortionPro.vst3 ~/Library/Audio/Plug-Ins/VST3/

# AU（如果编译了）
cp -r build/DistortionPro.component ~/Library/Audio/Plug-Ins/Components/
```

#### Linux VST3

```bash
cp -r build/DistortionPro.vst3 ~/.vst3/
```

### 目录结构

```
DistortionPro/
├── CMakeLists.txt
├── README.md
├── src/
│   ├── dsp/
│   │   ├── DistortionAlgorithms.h/cpp    # 核心失真算法
│   │   ├── DistortionProcessor.h/cpp     # 主 DSP 处理器
│   │   └── Oversampler.h/cpp             # 2 倍过采样
│   ├── plugin/
│   │   ├── DistortionPro.h/cpp           # 插件入口点
│   │   └── DistortionPro.h               # 插件头文件
│   ├── ui/
│   │   ├── PluginEditor.h/cpp            # 主编辑器界面
│   │   ├── KnobComponent.h/cpp           # 旋钮控件
│   │   ├── WaveformDisplay.h/cpp         # 波形显示
│   │   ├── GainMeter.h/cpp               # 增益表
│   │   └── TypeSelector.h/cpp            # 失真类型选择器
│   └── presets/
│       └── PresetManager.h/cpp           # 预设管理
└── presets/                              # 工厂预设文件
```

### 参数说明

| 参数 | 范围 | 说明 |
|------|------|------|
| Drive | 0-100% | 削波前的输入增益 |
| Tone | 0-100% | 高频衰减 |
| Output | -12 到 +12 dB | 最终输出电平 |
| Mix | 0-100% | 干湿混合比例 |
| Depth | 0-100% | 失真柔和度（拐点） |
| Attack | 0-100% | 失真启动速度 |
| Type | 4 种模式 | 算法：过载/失真/法兹/饱和 |
| Oversample | 开/关 | 2 倍过采样 |

### 预设 JSON 格式

```json
{
  "name": "预设名称",
  "category": "Factory",
  "type": "overdrive",
  "parameters": {
    "drive": 0.5,
    "tone": 0.6,
    "output": 0.8,
    "mix": 1.0,
    "depth": 0.5,
    "attack": 0.5
  }
}
```

### 开源协议

MIT License - 详见 LICENSE 文件。

### 致谢

- 基于 [JUCE Framework](https://juce.com/) 构建
- VST3 SDK 已包含在 JUCE 中
