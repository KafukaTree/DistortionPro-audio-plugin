## Why

市场上缺乏既简单易用又具备专业品质的失真效果器插件。许多专业插件功能复杂学习曲线陡峭，而简单插件又缺少专业控制功能。需要一款能够在 VST3 和 AAX 平台上运行的失真效果器，提供直观的可视化界面和预设系统，满足从爱好者到专业制作人的需求。

## What Changes

- 新增 Distortion 效果器核心引擎，支持多种失真类型
- VST3 和 AAX 插件格式支持
- 可视化 UI，实时显示输入/输出波形和频谱
- 预设系统，包含出厂预设和用户自定义预设
- 参数自动化支持
- 跨平台支持（Windows/macOS）

## Capabilities

### New Capabilities

- `distortion-engine`: 失真效果器核心算法，包括过载、饱和、失真等多种类型
- `plugin-formats`: VST3 和 AAX 插件格式的封装层
- `visual-ui`: 可视化用户界面，包含波形显示、频谱分析、参数可视化
- `preset-system`: 预设管理系统，支持保存、加载、分类预设

### Modified Capabilities

无（全新项目）

## Impact

- 新建 `src/` 目录，包含引擎、UI、插件格式代码
- 新建 `presets/` 目录存放预设文件
- 需要集成 VST3 SDK 和 AAX SDK
- 需要 GUI 框架（可选：JUCE、imgui、nanovg）
