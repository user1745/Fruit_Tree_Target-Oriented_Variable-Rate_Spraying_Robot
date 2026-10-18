<div align="center">

# Fruit Tree Target-Oriented Variable-Rate Spraying Robot

**果树对靶变量施药机器人 —— 面向果树目标的嵌入式控制工程与变量施药项目资料。**

[![Platform](https://img.shields.io/badge/Platform-Embedded%20System-informational.svg)](https://en.wikipedia.org/wiki/Embedded_system)
[![Toolchain](https://img.shields.io/badge/Toolchain-Keil%20MDK-blue.svg)](https://www.keil.com/)

</div>

## 目录

- [项目简介](#项目简介)
- [演示视频](#演示视频)
- [代码范围](#代码范围)
- [工程结构](#工程结构)
- [使用与构建](#使用与构建)
- [适用场景](#适用场景)
- [注意事项](#注意事项)
- [许可证](#许可证)

## 项目简介

本项目是一个面向果树目标的变量施药机器人控制工程，仓库主要保存嵌入式端工程文件、控制代码、硬件相关目录和 Keil 工程配置。

项目目标是根据果树目标信息实现更有针对性的施药控制，减少无目标区域的药液浪费，并为后续视觉识别、运动控制和喷洒策略集成提供嵌入式基础。

## 演示视频

[观看「果树对靶变量施药机器人」演示视频](https://www.bilibili.com/video/BV1jRLS6CEVb?vd_source=8a3cfebb5b0c7324aa168aa85736dd91)

## 代码范围

> **重要：本仓库不包含视觉识别部分代码。**

| 范围 | 状态 |
| --- | --- |
| 嵌入式控制工程 | 已包含 |
| Keil 工程文件 | 已包含 |
| 控制、系统和用户代码目录 | 已包含 |
| 视觉识别算法及其源码 | 未包含 |
| 完整机器人硬件清单与装配图 | 需结合实际项目资料确认 |

因此，克隆本仓库后可以查看和继续开发嵌入式控制部分，但不能仅凭本仓库复现完整的视觉识别和整机系统。

## 工程结构

```text
├── Control/       # 控制相关代码
├── Core/          # 核心启动与底层工程文件
├── DSPLibrary/    # DSP 或信号处理相关库
├── Hardware/      # 硬件驱动与板级支持
├── Library/       # 通用库文件
├── Objects/       # Keil 编译输出相关目录
├── System/        # 系统配置与基础代码
├── User/          # 用户应用代码
├── Project.uvprojx
├── Project.uvoptx
└── README.md
```

实际目录内容以当前分支为准；`Objects/` 等构建产物是否需要保留，取决于你的本地 Keil 工程设置。

## 使用与构建

### 环境要求

- Windows
- Keil MDK / µVision
- 与工程配置匹配的 ARM 编译器和目标芯片支持包
- 对应的机器人硬件、驱动板和传感器（若进行实机验证）

### 基本流程

1. 克隆仓库：

   ```bash
   git clone https://github.com/user1745/Fruit_Tree_Target-Oriented_Variable-Rate_Spraying_Robot.git
   cd Fruit_Tree_Target-Oriented_Variable-Rate_Spraying_Robot
   ```

2. 使用 Keil 打开 `Project.uvprojx`。
3. 检查芯片型号、下载器、时钟和外设引脚配置。
4. 根据实际硬件连接修改控制参数和传感器配置。
5. 编译工程并通过 ST-Link 或目标板支持的方式下载。
6. 在安全、可控的实验环境中验证控制逻辑，再进行整机测试。

## 适用场景

- 果树目标施药控制研究
- 嵌入式喷洒设备原型开发
- 变量施药、目标检测与执行机构联调
- Keil/STM32 类嵌入式工程学习与二次开发

## 注意事项

- 施药设备涉及机械运动、压力介质和农药安全，调试时应先进行空载和清水测试。
- 仓库不含视觉识别代码，视觉端输出的数据格式需要与控制端另行约定。
- 未确认具体硬件版本前，不要直接照搬引脚、阈值或执行机构参数。
- 构建和下载前请备份本地工程配置，避免覆盖个人调试器设置。

## 许可证

本仓库当前未声明开源许可证。除另有书面授权外，代码、工程文件和相关资料请勿擅自用于商业再分发。