<div align=center><font size='70'>《编译原理》</font></div>

<div align=center><font size='90'>词法分析 实验报告</font></div>

---
<div align=center>
<img src="http://img.070077.xyz/202204021121013.png" width="50%" />
</div>
---
<div align=center><font size='40'> 姓名：</font></div>
<div align=center><font size='40'>学号：</font></div>
<div align=center><font size='70'>班级：</font></div>

---

### 实验内容

设计并实现 C 语言的词法分析程序，要求如下。
1) 可以识别出用 C 语言编写的源程序中的每个单词符号，并以记号的形式输出每个单词符号。
2) 可以识别并读取源程序中的注释。
3) 可以统计源程序汇总的语句行数、单词个数和字符个数，其中标点和空格不计算为单词，并输出*统计结果*
4) 检查源程序中*存在的错误*，并可以报告错误所在的行列位置。
5) 发现源程序中存在的错误后，进行适当的恢复，使词法分析可以继续进行，通过一次词法分析处理，可以检查并报告源程序中存在的所有错误

采用 C++作为实现语言，手工编写词法分析程序。

### 实验环境

PC（Windows 11）

IDE：CLion 2022.1

辅助工具：copilot、chatGPT

### 实验内容

以下按词法分析的相关模块，进行参考，流程如下：

token 模块定义了记号的数据结构，使用字典树（trie）用于快速检索关键字。

输入：使用 reader 模块，对文件输入流进行简单封装。后进入 lexer 词法分析模块。

#### 记号设计

分为以下11类

```c
    enum TokenType {
        IDENTIFIER, //标识符
        KEYWORD, //关键字
        INTEGER,
        FLOAT,
        CHAR,
        STRING,
        PUNCTUATION, //标点
        COMMENT, //注释
        PREPROCESS, //预处理内容
        ERRORLEX, //错误
        END_OF_FILE
    };
```



使用copilot得到 C99 的 关键字、标点 列表：

```cpp
    const std::vector<std::string> token_type = {
            "Identifier", "Keyword", "Integer", "Float", "Char", "String", "Punctuation", "Comment", "Preprocess",
            "ErrorLex"};

    const std::vector<std::string> keywords = {
            "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern",
            "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static",
            "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
    };

    const std::vector<std::string> punctuators = {"", "[", "]", "(", ")", "{", "}", ".", "->", "++", "--", "&", "*",
                                                  "+", "-",
                                                  "~", "!", "/", "%", "<<", ">>", "<", ">", "<=", ">=", "==", "!=", "^",
                                                  "|",
                                                  "&&", "||", "?", ":", ";", "...", "=", "*=", "/=", "%=", "+=", "-=",
                                                  "<<=",
                                                  ">>=", "&=", "^=", "|=", ","};
```



对于数字的处理：为了简化，我选择**仅支持纯数值十进制写法**，如“114514”。忽略科学记数法、其他进制等，如“1e-6”、“0x3f3f3f3f”、“23323323332333l”。

对于错误，识别类型：

```c++
enum class ErrorLex {
        NONE, 
        UNCLOSED_COMMENT,  //注释未合
        UNCLOSED_CHAR,	 //char：'g
        UNCLOSED_STRING,	//string: "genshin
    	TODO //any error that don't match
    };
```

#### 语法分析算法设计

1. 读取到EOF。为方便进行判断，设置`reader`模块。

2. 根据读取到的字符，进入各个handler。设计如下：

   | handler                | 基本思想                                                     |
   | ---------------------- | ------------------------------------------------------------ |
   | GetTokenHandler        | 统一的入口                                                   |
   | LineCommentHandler     | 单行注释，匹配`//...(\n/EOF)`。                              |
   | BlockCommentHandler    | 块级别注释，匹配`/*...*/，`检测UNCLOSED_COMMENT错误          |
   | PreprocessingHandler   | 预处理语句，匹配`#...(\n)`                                   |
   | NumberTokenHandler     | 处理数字，参考上方设计。<br />分为DecimalIntegerHandler、DecimalFloatHandler以分别处理整数和小数。 |
   | CharTokenHandler       | 处理字符变量，同时检测UNCLOSED_CHAR错误                      |
   | StringTokenHandler     | 处理字符串。同时检测UNCLOSED_STRING错误                      |
   | IdentifierTokenHandler | 处理字面量。                                                 |

3. 设计错误处理逻辑：
   如UNCLOSED_COMMENT的检测：是通过`/*`开始，直到遇到`*/`才结束。如果遇到`EOF`，说明未闭合。

4. 细节补充

   - 对于标点，词法分析器通过**向前**缓冲字符，以判断标点符号的类型。如，表达式 y+++s 会被解析为 y、++、+、s。
   - 对于行前的空格`isLeadingBlank`，即如果`  #define`类语句前方的空格，会自动忽略。通过标识哨兵实现。

### 实验结果

源码仓库：[1475505/Naive-Clex (github.com)](https://github.com/1475505/Naive-Clex)

使用方式：编译出`exe`文件，如`./clex.exe [需分析的代码文件路径]`

#### 测试文件1

```c

```

#### 测试文件2

```c
#include <stdio.h>
int main()
{
    int a, b;
    scanf("%d%d", &a, &b);
    b += 648;
    printf("%d\n, a + b);
    return 0;
}
```



### 实验总结

