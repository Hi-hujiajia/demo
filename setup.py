from setuptools import setup
from distutils.core import setup, Extension

def main():
    setup(
        name='test_module_railgun',  # 需要打包的名字,即本模块要发布的名字
        version='v0.1.1',  # 版本
        description='A  module for test',  # 简要描述
        #py_modules=['mySelfSumonlymyrailgun'],  # 需要打包的模块 1 如果是。py文件被打包，请使用这句话
        author='hi-hujiajia',  # 作者名
        author_email='aaa@xxx.com',  # 作者邮件
        url='',  # 项目地址,一般是代码托管的网站
        # requires=['requests','urllib3'], # 依赖包,如果没有,可以不要
        license='MIT',
        ext_modules=[Extension("test_module", ["test.c"])],   #2 如果是py文件发布，请删除这句话，
    )

if __name__ == "__main__":
    main()
