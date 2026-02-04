## Context

开发一款支持 VST3 和 AAX 格式的失真效果器插件，需要在 Windows 和 macOS 上运行。项目从零开始，需要选择合适的跨平台框架和音频 SDK。

## Goals / Non-Goals

**Goals:**
- 使用 JUCE 框架实现跨平台（Windows/macOS）支持
- 支持 VST3 和 AAX 插件格式
- 实现多种失真算法（过载、饱和、失真、碎化等）
- 提供可视化 UI，包括波形显示、频谱分析、增益表
- 实现预设管理系统，支持出厂预设和用户预设
- 确保低延迟实时处理

**Non-Goals:**
- 不支持 AU 格式（用户未要求）
- 不支持 Linux 平台（首期）
- 不包含复杂的多段失真
- 不包含在线更新或授权系统

## Decisions

### 1. 框架选择: JUCE

**选择 JUCE 而非自建音频引擎**

JUCE 提供了完整的音频插件开发框架：
- 原生支持 VST3、AU、AAX、CLAP 格式
- 跨平台（Windows/macOS/Linux）
- 完整的音频处理基础设施
- 丰富的 GUI 组件系统

**替代方案考虑：**
- 自建 + VST3 SDK：需要自行处理平台兼容、插件加载、参数管理
- IPlug2：更轻量但生态较小

### 2. UI 框架: JUCE Components + OpenGL

**选择 JUCE 原生组件 + OpenGL 加速**

- JUCE Components：原生外观，一致的用户体验
- OpenGL：用于波形和频谱显示的高性能渲染

**替代方案考虑：**
- imgui：轻量但风格不够专业
- Qt：重量级，许可复杂

### 3. 预设格式: JSON

**选择 JSON 作为预设文件格式**

- 人类可读，易于调试
- 跨平台兼容
- 易于扩展

预设结构：
```json
{
  "name": "Warm Overdrive",
  "type": "overdrive",
  "parameters": {
    "drive": 0.5,
    "tone": 0.7,
    "output": 0.8
  }
}
```

### 4. 音频处理架构

```
Input → Pre-Gain → Distortion Stage → Post-Gain → Output
              ↓
        Parameter Automation
```

## Risks / Trade-offs

| 风险 | 影响 | 缓解措施 |
|------|------|----------|
| AAX SDK 许可复杂 | 需购买 Pro Tools SDK | 确认用户已具备许可或延后 AAX 开发 |
| 性能瓶颈 | 实时音频处理延迟 | 使用双缓冲、SIMD 优化 |
| UI 渲染性能 | 复杂可视化导致卡顿 | OpenGL 加速、按需更新 |
| macOS 签名/公证 | 分发困难 | 提供详细签名流程文档 |

## Open Questions

1. 是否需要 MIDI 控制映射功能？
2. 预设是否需要按类别组织（过载、失真、碎化）？
3. 是否需要支持立体声和单声道两种模式？
