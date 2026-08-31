# GameEngine

> **基于 Qt6 + 主流 C++ 游戏中间件的"游戏编辑器"脚手架——主窗口 + 启动页 + 新建项目对话框 + 多 dock 布局，但场景/资源/层级面板都还是空指针。**

## 项目定位

GameEngine 是一个用 **Qt6 Widgets** 写桌面 GUI 外壳、内部通过 **vcpkg 拉取一整套游戏引擎常见中间件**（Bullet3 物理、SDL3、OpenAL、GLFW、glm、imgui、Vulkan、FreeType…）的**编辑器骨架**。

意图很明确：搭一个"像 Unity / Unreal Editor"那样的桌面工具——主窗口有菜单、工具栏、4 个 dock（属性检视器 / 资源浏览器 / 层级树 / 控制台）、一个中心 SceneView——但当前所有 dock 控件都还是前向声明（`SceneViewWidget / PropertyEditorWidget / AssetsBrowserWidget` 只声明了指针），没有实现。换句话说，**GUI 外壳 + 依赖接入**已经完成，**真正的引擎功能还一点都没有**。

## 仓库结构

```
GameEngine/
├── CMakeLists.txt                # C++20 + Qt6 + 一大堆 vcpkg 依赖
├── vcpkg.json                    # vcpkg 清单（11 个依赖）
├── assets/
│   ├── main.qrc                  # Qt 资源：dark.qss + 7 个工具栏图标（实际 PNG 缺失）
│   └── styles/dark.qss           # 暗色 Qt 样式
└── src/
    ├── main.cpp                  # 入口：单例 Editor::getInstance → editor.start() → exec()
    ├── Define.h                  # ExecReturn 枚举（Success/Fail/Error/Unknown/Cancel）
    └── editor/
        ├── Editor.h              # QApplication 子类，单例，start / startMainWidget / startNewProjectWidget
        ├── startwidget.h/.cpp/.ui   # 启动页：openNewProjectDialog 槽
        ├── mainwidget.h/.cpp/.ui    # QMainWindow：菜单/工具栏/状态栏/4 个 dock/中心 widget
        └── newprojectdialog.h/.cpp/.ui  # 新建项目对话框
```

## 技术栈

| 层 | 选型 |
|---|---|
| 语言 | C++20（`CMAKE_CXX_STANDARD 20`） |
| 构建 | CMake ≥ 3.31，启用 `AUTOMOC / AUTORCC / AUTOUIC` |
| 桌面 GUI | **Qt6**（Widgets + Gui + Core，通过 `find_package(Qt6 ... REQUIRED)`） |
| 物理 | **Bullet3**（`find_package(Bullet CONFIG REQUIRED)`，`target_link_libraries(... ${BULLET_LIBRARIES} Bullet3Dynamics)`） |
| 窗口 / 输入 | **GLFW3** |
| 图像加载 | **SDL3_image**（可选 shared / static 链接） |
| 多媒体 | **SDL3**（视频 + 事件） |
| 音频 | **OpenAL Soft** |
| 数学 | **glm** |
| 序列化 | **RapidJSON** |
| 调试 UI | **Dear ImGui**（`imgui::imgui`） |
| 渲染 | **Vulkan**（`find_package(Vulkan REQUIRED)`） |
| 字体（潜在） | **fmt 11**（vcpkg 引入，spdlog 间接依赖） |
| 日志 | **spdlog**（`spdlog::spdlog`） |
| 依赖管理 | **vcpkg**（项目内 `vcpkg.json` 锁定 11 个依赖，CMake 走 `find_package` 模式） |

## 核心模块

### 1. `Editor`（QApplication 子类，单例）
- 通过 `Meyers singleton` + 私有构造实现；
- 持有 `std::unique_ptr<StartWidget>` / `std::unique_ptr<MainWidget>` / `std::unique_ptr<NewProjectDialog>`；
- 公共字段：`projectName_` / `projectPath_`；
- 公开 API：
  - `start()` —— 启动第一个窗口（典型是 StartWidget）；
  - `startMainWidget()` —— 切到主编辑器；
  - `startNewProjectWidget()` —— 弹出"新建项目"对话框。

### 2. `StartWidget`（启动页）
- 简单 `QWidget`，布局见同名 `.ui`；
- 槽：`openNewProjectDialog()`。

### 3. `NewProjectDialog`（新建项目对话框）
- 标准模态 `QDialog`，收集项目名 / 路径；
- 提交后调用 `Editor::setProjectInfo(name, path)` 然后 `startMainWidget()`。

### 4. `MainWidget`（QMainWindow 主窗口）
完整建模了一个标准 IDE 风格的多窗口布局：

- **菜单栏**（6 个 menu）：`fileMenu` / `editMenu` / `viewMenu` / `toolsMenu` / `gameMenu` / `helpMenu`；
- **工具栏**（3 个 toolbar）：`fileToolBar` / `editToolBar` / `gameToolBar`；
- **Action**（10 个）：
  - 文件：`newAct / openAct / saveAct / saveAsAct / exitAct`；
  - 编辑：`undoAct / redoAct`；
  - 运行：`playAct / pauseAct / stopAct`；
  - 关于：`aboutAct / aboutQtAct`；
- **4 个 QDockWidget**（左右上下可停靠）：
  - `propertyInspectorDockWidget` ← 指向 `PropertyEditorWidget`（前向声明，未实现）；
  - `assetBrowserDockWidget` ← 指向 `AssetsBrowserWidget`（前向声明，未实现）；
  - `hierarchyDockWidget`（无对应类，纯占位）；
  - `consoleDockWidget` ← 指向 `m_consoleOutput`（`QTextEdit*`，也未实现）；
- **中心 widget**：`m_sceneView`（前向声明 `SceneViewWidget`，未实现）；
- **辅助 API**：`registerAction(id, action, menuPath)` / `addCustomDockWidget(area, dock)`，看起来是给后续插件系统用的扩展点；
- **私有槽**：`newProject / openFile / saveFile / saveFileAs / about / playScene / pauseScene / stopScene`；
- **持久化**：`readSettings / writeSettings`（QSettings 风格的窗口几何保存）；
- 析构中重写 `closeEvent` 处理"修改未保存"的拦截。

### 5. 资源（`assets/main.qrc`）
- Qt 资源前缀 `/styles`：嵌入 `dark.qss`（暗色主题）；
- Qt 资源前缀 `/icons`：声明 7 个工具栏图标（`new/open/save/undo/redo/play/pause/stop.png`），**实际 PNG 文件未提交**，因此 QRC 编译会失败或运行时图标显示空——这是当前最显眼的"未完成"。

## 已完成 / 进行中

- [x] CMake + vcpkg 全栈依赖清单（11 个）
- [x] Qt6 多窗口 + dock + 菜单 + 工具栏的 UI 骨架
- [x] Editor 单例 + StartWidget / MainWidget / NewProjectDialog 切换流程
- [x] dark.qss 暗色样式（资源路径已声明）
- [x] spdlog 接入
- [ ] 所有 dock 控件与 SceneView 实现（**前向声明而已**）
- [ ] 工具栏图标 PNG（**QRC 引用但文件缺失**）
- [ ] 文件 I/O（new/open/save/saveAs）
- [ ] 播放 / 暂停 / 停止 与实际场景对接到 Bullet3
- [ ] 任何具体的引擎子系统（场景图、组件、序列化）

## 本地构建

```powershell
# 假设已经安装 Qt6 + vcpkg
cmake -B build -S . \
    -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
.\build\Release\GameEngine.exe
```

预期：弹出一个 Qt 暗色启动页，点击"新建项目"会进入主窗口，菜单/工具栏可见但所有 dock 是空的，控制台是白板，工具栏图标缺失。

## 状态

- **版本**：v0.1（仅有 GUI 骨架与依赖清单）
- **架构阶段**：编辑器壳完成
- **可运行性**：可编译（依赖完整安装 Qt6 + vcpkg），运行后**仅显示空骨架窗口**

## License

仓库内未附 LICENSE 文件，源码默认遵循 "All rights reserved"。
