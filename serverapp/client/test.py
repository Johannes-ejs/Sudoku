import os


with open(os.path.join('.client','nickname.txt'), 'r') as f:
    msg = f.readline()
    print(msg)