#This file containes the code to run the unit tests
from shutil import which
from subprocess import Popen
import os

from utils import eprint

def run_test(path):
    print(path)
    print("Running test on ", path)


def compile_test():
    print("compiling the tests")    


def make_local_tests():
    print("Running make in ", os.getcwd())
    process = Popen(["make"])
    if process.poll() != 0:
      raise Exception("Error while running make")

# There is 2 parts to this :
#    - first we build the local binaries and test them
#    - then we run the tests on the system executables (ls, grep, etc...) 
def run_tests():
    print("Building the local tests")
    try:
        make_local_tests()
    except Exception as error:
        print(error)
        eprint("Error while building local tests")

