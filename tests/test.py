import os
import sys

failed = False

os.system("../parse2json test1.hpp test1.json")
os.system("../parse2hpp test2.json test2.hpp")

with open("test1.output") as desired:
    with open("test1.json") as result:
        if desired.read() != result.read():
            print("Test 1 Failed")
            failed = True
        else:
            print("Test 1 Passed")
with open("test2.output") as desired:
    with open("test2.hpp") as result:
        if desired.read() != result.read():
            print("Test 2 Failed")
            failed = True
        else:
            print("Test 2 Passed")

sys.exit(failed)
