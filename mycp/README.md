# mycp
[![Open Source Love](https://badges.frapsoft.com/os/v1/open-source.png?v=103)](https://github.com/ellerbrock/open-source-badge/)
[![stable](http://badges.github.io/stability-badges/dist/stable.svg)](http://github.com/badges/stability-badges)

## Introduction
Copy files and directories.

## Environment
+ Linux Bash Shell 
+ POSIX C

## Options 
```
-R/r：递归处理，将指定目录下的所有文件与子目录一并处理；
-s：对源文件建立符号连接，而非复制文件；
-l：对源文件建立硬连接，而非复制文件；
```

## Compile and Run
```
$ make
$ make clean
```

```
$ ./mycp -[Rrls] source dest
```

## Reference
+ https://www.shiyanlou.com/courses/577/labs/1916/document

## About me
[![forthebadge](http://forthebadge.com/images/badges/ages-20-30.svg)](http://forthebadge.com)
- WebSite：[http://www.answerywj.com](http://www.answerywj.com)
- Email：[yuanweijie1993@gmail.com](https://mail.google.com)
- GitHub：[AnSwErYWJ](https://github.com/AnSwErYWJ)
- Blog：[AnSwEr不是答案的专栏](http://blog.csdn.net/u011192270)
- Weibo：[@AnSwEr不是答案](http://weibo.com/1783591593)

## Copyright and License
**The MIT License (MIT)**  
Copyright (c) 2016 AnSwErYWJ

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
