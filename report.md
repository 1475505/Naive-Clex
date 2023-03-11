<div align=center><font size='70'>《编译原理》</font></div>

<div align=center><font size='90'>词法分析 实验报告</font></div>

---
<div align=center>
<img src="http://img.070077.xyz/202204021121013.png" width="50%" />
</div>
---
<div align=center><font size='40'> 姓名：刘亮</font></div>
<div align=center><font size='40'>学号：2020211318</font></div>
<div align=center><font size='70'>班级：2020211322</font></div>

---

### 实验内容

设计并实现 C 语言的词法分析程序，要求如下。
1) 可以识别出用 C 语言编写的源程序中的每个单词符号，并以记号的形式输出每个单词符号。
2) 可以识别并读取源程序中的注释。
3) 可以统计源程序汇总的语句行数、单词个数和字符个数，其中标点和空格不计算为单词，并输出统计结果
4) 检查源程序中存在的错误，并可以报告错误所在的行列位置。
5) 发现源程序中存在的错误后，进行适当的恢复，使词法分析可以继续进行，通过一次词法分析处理，可以检查并报告源程序中存在的所有错误

采用 C++作为实现语言，手工编写词法分析程序。

### 实验环境

PC（Windows 11）

IDE：CLion 2022.1

辅助工具：copilot、chatGPT

### 实验内容

以下按词法分析的相关模块，进行参考，流程如下：

token 模块定义了记号的数据结构，使用字典树（trie）用于快速检索关键字。

输入：使用 scanner 模块，对文件输入流进行简单封装。后进入 lexer 词法分析模块。

#### 记号

使用copilot得到 C99 的 token 列表：

```
auto break case char const continue default do double else enum extern float for goto if int long register return short signed sizeof static struct switch typedef union unsigned void volatile while
```



### 实验结果

### 实验总结

