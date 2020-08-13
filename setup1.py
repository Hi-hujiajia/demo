from distutils.core import setup, Extension

def main():
    setup(name="Test",
          version="1.0.0",
          description="Python interface for the fputs C library function",
          author="hihuajiajia",
          author_email="xxxx@xx.com",
          ext_modules=[Extension("test_module", ["test.c"])])

if __name__ == "__main__":
    main()
