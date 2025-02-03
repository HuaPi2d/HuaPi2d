# 画皮`2D`

---

## 写在前面

在学习 `Qt` 图形编程，并尝试自己做出第一个图形界面程序后，创作的热情就难以消减，一切都是因游戏脚本而燃起热情，但热情不会止步于游戏脚本的制作。对于大学上学习的课程内容，以及任何可以想到的小玩意，我都有将那些东西全部写进这个项目的冲动。这一切可能会耗费我不止大学的几年时间，但趁时间尚早，人还年轻，无论对错，尽管尝试。

## 一、模块功能

### 1.1 生死狙击

本模块集成了识图 ，键盘鼠标模拟，进程控制等游戏脚本常见功能。实现了全自动挂机、生死狙击武器加成手法辅助功能。计划添加游戏脚本文本编程及图形编程等功能。

#### 1.1.1 全自动脚本挂机

**界面展示：**

<img src="README_img/image-20241120164729481-1732094259784-2.png" alt="image-20241120164729481" style="zoom:67%;" />

**单次运行结果展示：**

<img src="README_img/result.png" alt="result" style="zoom:67%;" />

#### 1.1.2 虚拟机测试脚本

开启过程：

- 连接虚拟机

	<img src="README_img/image-20241125224750305.png" alt="image-20241125224750305" style="zoom: 80%;" /> 

- 修改分辨率为100

- 立即注销

	<img src="README_img/image-20241125224905192.png" alt="image-20241125224905192" style="zoom:67%;" /> 

- 进入桌面

- 增强会话

#### 1.1.3 辅助加成

**界面展示：**

<img src="README_img/image-20241120165032208.png" alt="image-20241120165032208" style="zoom:67%;" />

使用情景：

- 角斗场：角斗场每一关使用的武器配置都可以进行保存，方便下次直接进行选择。

优势：

- 在最短时间内，根据顺序开启加成，达到短时间的爆发输出

### 1.2 音乐盒

本模块主要收集的是网络音乐网站的内容，目前提供了基础的歌曲搜索、歌曲播放器、歌曲下载功能。

#### 1.2.1 搜索页

**界面展示：**

<img src="README_img/image-20241120165656383.png" alt="image-20241120165656383" style="zoom:67%;" />

音乐源：

- 歌曲宝

**部分歌曲的下载存在问题**

#### 1.2.2 本地页

**界面展示：**

<img src="README_img/image-20241120165910156.png" alt="image-20241120165910156" style="zoom:67%;" />

### 1.3 墨韵声扬

本模块主要接入了阿里云语音模型，提供了一些文字转语音的功能。

**界面展示：**

<img src="README_img/image-20241120170340173.png" alt="image-20241120170340173" style="zoom:67%;" />

语音场景：

<img src="README_img/image-20241120170459667.png" alt="image-20241120170459667" style="zoom:67%;" /> 

语音音色：

<img src="README_img/image-20241120170534389.png" alt="image-20241120170534389" style="zoom:67%;" /> 

## 二、其他功能

### 2.1 主题风格切换

软件应用了开源项目 https://github.com/githubuser0xFFFF/Qt-Advanced-Stylesheets 进行界面主题风格的构建，分为暗色和亮色两大主题。也提供了自定义 `.qss` 风格文件导入的功能，可以切换界面到 `MacOs` 或 `Ubuntu` 等操作系统的界面风格。

**亮蓝色系风格：**

<img src="README_img/image-20241120171524831.png" alt="image-20241120171524831" style="zoom:67%;" />

**暗黄色系风格：**

<img src="README_img/image-20241120171559744.png" alt="image-20241120171559744" style="zoom:67%;" />

**`Ubuntu` 界面：**

<img src="README_img/image-20241120171402115.png" alt="image-20241120171402115" style="zoom:67%;" />

### 2.2 多语言

> 利用 `Qt` 提供的翻译模块实现了对于界面大部分内容的批量机翻工作，主要处理利用的是 `lupdate` 生成的 `.ts` 文件以及 `Qt Linguist` 生成的 `.qm` 文件。
>
>  由于利用 `Qt Linguist` 手动翻译的工作量巨大，因此采用 `Python` 脚本进行了批量的处理工作，具体可以参考项目： https://github.com/HuaPi2d/QT-Liguist-Promax 
>
> <img src="README_img/language.gif" alt="image-20241120171402115" style="zoom:67%;" />
