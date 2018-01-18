#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Create on: earlier
Modify on: 2018-01-18
Description: None

Guangzhou Tronlong Electronic Technology Co.,Ltd.
Website: [http://tronlong.com/](http://tronlong.com/)
'''

from __future__ import division
import os
import sys
import time
import signal
import logging
from logging.handlers import RotatingFileHandler

class DiskWatcher:
    def __init__(self, path, lower_limit):
        self.path = path
        self.lower_limit = lower_limit

    def __del__(self):
        pass

    def diskStat(self, path):
        try:
            disk  = os.statvfs(path)
            total = (disk.f_blocks * disk.f_frsize)
            free  = (disk.f_bavail * disk.f_frsize)
            used  = (disk.f_blocks - disk.f_bfree) * disk.f_frsize

            free_percent = (free / total) * 100
            logging.info(path + ' free: ' + str(free_percent) + '%')

            used_percent = (used / total) * 100
            logging.info(path + ' used: ' + str(used_percent) + '%')

            disk_stat = {'free':free_percent, 'used':used_percent}

            print

        except BaseException, e:
            logging.error('fail to check free of ' + path)
            return 0.0

        return disk_stat

    def isLowerLimit(self):
        dstat = self.diskStat(self.path)

        if dstat['free'] < self.lower_limit:
            return True

        return False

    def clean(self):
        try:
            logging.info(self.path + '\'s free space is lower to limit, clean the old record ...')
            print
            cmd = 'rm -rf /media/flypei/0003-D34D/ffmpeg-15min-h264/video1/*'
            os.system(cmd)
            return True

        except BaseException, e:
            return False

def initLog():
    argv0 = sys.argv[0]
    exeFile  = argv0[2:] # remove "./"
    outputFormatter = '%(asctime)s %(filename)s [line:%(lineno)d] [%(levelname)s] %(message)s'
    logging.basicConfig(level = logging.DEBUG, filename = '/dev/null')
    console = logging.StreamHandler()
    console.setLevel(logging.DEBUG)
    formatter = logging.Formatter(outputFormatter)
    console.setFormatter(formatter)
    logging.getLogger('').addHandler(console)

global g_quit
g_quit = False

def quit(signum, frame):
    logging.info('ctrl + c ...')
    global g_quit
    g_quit = True

def diskWatcherConfigure(pathname, lowerLimit, interval):
    signal.signal(signal.SIGINT, quit)
    initLog()
    watcher = DiskWatcher(pathname, lowerLimit)

    while not g_quit:

        if watcher.isLowerLimit():
            watcher.clean()
        else:
            pass
        time.sleep(interval)

if __name__ == "__main__":
    #####################################
    # 参数1为设备挂载点                 #
    # 参数2为内存空间剩余下限           #
    # 参数3为内存检测时间间隔(s)        #
    #                                   #
    # 功能:函数以固定时间监测内存空间,  #
    # 当剩余空间低于下限时,调用clean()  #
    # 函数进行删除,clean()函数须自己定义#
    #####################################
    diskWatcherConfigure('/media/flypei/0003-D34D', 10, 3)
