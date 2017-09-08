"""A setuptools based setup module.
    
    See:
    https://packaging.python.org/en/latest/distributing.html
    https://github.com/pypa/sampleproject
    """

from setuptools import setup,find_packages
from setuptools.extension import Extension
from codecs import open
import os
from os import path
from setuptools.command.install import install
import subprocess


class MyInstall(install):
    def run(self):
        path = os.getcwd().replace(" ", "\ ").replace("(","\(").replace(")","\)") + "/bin/"
        subprocess.call(['chmod', '+x', os.path.join(path,'compile_library.sh')])
        subprocess.call(['sh',os.path.join(path,'compile_library.sh')])
        install.run(self)

here = path.abspath(path.dirname(__file__))

# Get the long description from the README file
with open(path.join(path.join(here, 'graph_lib'),'README.rst'), encoding='utf-8') as f:
    long_description = f.read()

setup(name='graph_lib',
      version='0.4.8',
      description='A set of useful diffusion related graph algorithm',
      url='https://github.com/MengLiuPurdue/graph_lib',
      long_description=long_description,
      author='Meng Liu, David F. Gleich',
      author_email='liu1740@purdue.edu',
      license='GPL',
      packages=['graph_lib'],
      include_package_data=True,
      install_requires=[
                        'numpy >= 1.12.0',
                        ],
      scripts=['bin/compile_library.sh'],
      cmdclass={'install':MyInstall},
      package_data={
      'graph_lib.lib': ['*'],
      'graph_lib.wrappers': ['*'],
      'graph_lib.graph': ['*.smat'],
      'graph_lib': ['*.py'],
      'graph_lib.lib.graph_lib_test': ['*'],
      'graph_lib.lib.graph_lib_test': ['Makefile'],
      },
      zip_safe=False)
