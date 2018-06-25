
import sys
import os
import time

os.system('cp test.png /mnt/hgfs/share')
os.system('cp output.txt /mnt/hgfs/share')
print('test.png and output.txt is shared')
mtime = os.path.getmtime('test.png')


while True:
    time.sleep(5)
    if os.path.getmtime('test.png') > mtime:
        print('test.png and output.txt is shared!')
        os.system('cp test.png /mnt/hgfs/share')
        os.system('cp output.txt /mnt/hgfs/share')
        mtime = os.path.getmtime('test.png')
