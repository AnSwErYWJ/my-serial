# mytouch
[![Open Source Love](https://badges.frapsoft.com/os/v1/open-source.png?v=103)](https://github.com/ellerbrock/open-source-badge/)
[![stable](http://badges.github.io/stability-badges/dist/stable.svg)](http://github.com/badges/stability-badges)

## Introduction
Change file timestamps.

## Environment
+ Linux Bash Shell
+ POSXI C

## Options
```
-a: change only the access time.
-c: do not create any files.
-m: change only the modification time.
```

## Compile and Run
```
make
make clean
```

```
./mytouch [-acm][-r ref_file(参照文件)|-t time(时间值)] file(文件名)
```

## Test
```
$ ./mytouch test
$ stat test
  File: `test'
  Size: 0         	Blocks: 0          IO Block: 4096   regular empty file
Device: fd02h/64770d	Inode: 11273522    Links: 1
Access: (0664/-rw-rw-r--)  Uid: (  501/aidu1602)   Gid: (  501/aidu1602)
Access: 2016-08-10 16:13:18.483201317 +0800
Modify: 2016-08-10 16:13:18.483201317 +0800
Change: 2016-08-10 16:13:18.483201317 +0800

$ ./mytouch -a test
$ stat test
  File: `test'
  Size: 0         	Blocks: 0          IO Block: 4096   regular empty file
Device: fd02h/64770d	Inode: 11273522    Links: 1
Access: (0664/-rw-rw-r--)  Uid: (  501/aidu1602)   Gid: (  501/aidu1602)
Access: 2016-08-10 16:13:28.539201291 +0800
Modify: 2016-08-10 16:13:18.483201317 +0800
Change: 2016-08-10 16:13:28.539201291 +0800

$ ./mytouch -m test
$ stat test
  File: `test'
  Size: 0         	Blocks: 0          IO Block: 4096   regular empty file
Device: fd02h/64770d	Inode: 11273522    Links: 1
Access: (0664/-rw-rw-r--)  Uid: (  501/aidu1602)   Gid: (  501/aidu1602)
Access: 2016-08-10 16:13:28.539201291 +0800
Modify: 2016-08-10 16:13:34.059201277 +0800
Change: 2016-08-10 16:13:34.059201277 +0800

$ stat testtest
stat: cannot stat `testtest': No such file or directory
```

## Reference
+ https://www.shiyanlou.com/courses/572/labs/1901/document

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
