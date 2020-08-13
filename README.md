本项目用于打包成python库的setup文件

这是一个生成python库的小白版本，希望可以给你提供帮助
本项目针对网络上基于pyhton文件打包程序较多提供补充

**** 本项目实现两个功能
**1 实现python程序打包为python库
**2 实现c程序编译为python的.whl文件，用于上传pypi
**3 实现c程序编译为python的本地包


如果只有一个被编译的文件，可以使用以下方法
本项目为了方便使用，有多个setup.py文件，
1 你想被使用pypi上传文件则
setup.py 将被改变
如果使用python文件，则删除setup.py   #1语句
如果使用.c文件，请使用  #2 语句 
在控制台输入  python setup.py sdist bdist_wheel    打包为.whl + tar 文件
如何上传至pypi 请百度



2 c编译为pyd包，本地使用
则可以调用简单版本setup1.py 
ext_modules=[Extension("test_module", ["test.c"])]) 
将test.c 修改为.c文件，test_module为库的名称
并控制台输入  pyhton setup.py install         打包为.pyd文件
新生成build文件，寻找.pyd文件，可以使用import导入
