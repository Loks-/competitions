#!/usr/bin/env python3

import time
import os
import hashlib
import subprocess
import sys

def openRetry(filename, mode):
    while True:
        try:
            return open(filename, mode)
        except OSError:
            pass

def file_hash(filename):
    while not os.path.isfile(filename):
        time.sleep(0.1)
    with openRetry(filename, "rb") as fIn:
        return hashlib.md5(fIn.read()).hexdigest()

args = sys.argv
problem_name = args[1]
input_name = args[2]

binary = 'advent_of_code'
year = '20' + problem_name[:2]
path_to_cpp = '../advent_of_code/' + year + '/src/'
file_cpp = path_to_cpp + problem_name + '.cpp'
path_to_input = '../advent_of_code/' + year + '/input/'
file_input = path_to_input + input_name + '.txt'
command_make = 'make -R ' + binary
command_run = './build/' + binary + ' ' + problem_name + ' ' + file_input

hash_empty = 'd41d8cd98f00b204e9800998ecf8427e'
hash_cpp_old = ''
hash_input_old = ''
while True:
    hash_cpp_new = file_hash(file_cpp)
    if hash_cpp_new == hash_empty:
        continue
    if hash_cpp_new != hash_cpp_old:
        print('New cpp hash ', hash_cpp_new)
        subprocess.call(command_make, shell=True, stderr=subprocess.STDOUT)
    hash_input_new = file_hash(file_input)
    if hash_input_new == hash_empty:
        continue
    if hash_input_new != hash_input_old:
        print('New input hash ', hash_input_new)
    if (hash_cpp_new != hash_cpp_old) or (hash_input_new != hash_input_old):
        subprocess.call(command_run, shell=True, stderr=subprocess.STDOUT)
        hash_cpp_old = hash_cpp_new
        hash_input_old = hash_input_new
