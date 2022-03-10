#!/usr/bin/env python3

#Global imports
import pathlib
import subprocess
import os
import sys


#Local imports
from utils import bcolors
import tests

# Config default, change them if you need it
class config:
    nm_path = "/usr/bin/nm"
    ft_nm_path = "../ft_nm"

banner = """
   __ _                       
  / _| |_     _ __  _ __ ___  
 | |_| __|   | '_ \| '_ ` _ \ 
 |  _| |_    | | | | | | | | |
 |_|  \__|___|_| |_|_| |_| |_|
        |_____|              by mclaudel 
"""

def cprint(color, string):
    print(color, string, bcolors.ENDC)

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

if __name__ == "__main__":
    print(banner)
    # Assuming that the parent directory is the project's directory
    path = pathlib.Path(__file__).parent.parent.resolve()
    # going to parent dir
    os.chdir(path)
    process = subprocess.Popen(["make"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = process.communicate()
    status = process.poll()
    if status != 0:
        err = err.decode('utf-8')
        eprint("Error, process returned : ", status)
        eprint("Error while running make : ")
        eprint(err)
    print(bcolors.OKGREEN, "Make ran successfully")

    paths = ["path1", "path2", "path3", "path4"]
    for p in paths:
        tests.run_test(p)
    
    

