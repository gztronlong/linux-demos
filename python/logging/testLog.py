#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import sys
import logging
from logging.handlers import RotatingFileHandler

# 获取正在执行的文件名
argv0 = sys.argv[0]
exeFile  = argv0[2:]; # remove "./"

# 输出日志格式
# 2017-07-05 14:53:14,702 testLog.py [line:45] [ERROR:40] This is error at ./testLog.py
outputFormatter = '%(asctime)s %(filename)s [line:%(lineno)d] [%(levelname)s:%(levelno)s] %(message)s at %(pathname)s'

# 回滚日志文件名格式
# 2017-07-05-testLog.py.log.xx
rotatingFileName = './{date}-{name}.log'.format(name = exeFile, date = time.strftime("%Y-%m-%d", time.localtime()))

# 通过logging.basicConfig函数对日志的输出格式及方式做相关配置
logging.basicConfig(level = logging.DEBUG,
        filename = '/dev/null')


# 定义一个streamerHandler,将ERROR级别或更高的日志信息打印到终端
console = logging.StreamHandler()
console.setLevel(logging.ERROR)
formatter = logging.Formatter(outputFormatter)
console.setFormatter(formatter)
logging.getLogger('').addHandler(console)

# 日志回滚,只对INFO或更高级别的日志进行回滚
# 最多备份10个文件，每个文件最大能存储5M日志数据
Rthandler = RotatingFileHandler(rotatingFileName, maxBytes = 5 * 1024 * 1024, backupCount = 10)
Rthandler.setLevel(logging.INFO)
formatter = logging.Formatter(outputFormatter)
Rthandler.setFormatter(formatter)
logging.getLogger('').addHandler(Rthandler)

'''
logging = logging.getLogger('')
logging.setLevel(logging.INFO)
Rthandler = RotatingFileHandler(rotatingFileName, maxBytes = 5 * 1024 * 1024, backupCount = 10)
formatter = logging.Formatter(outputFormatter)
Rthandler.setFormatter(formatter)
'''

#####################################
# ************** 测试 ************* #
#####################################

# 将 "ERROR" 及以上级别的打印到终端
# 将 "INFO " 及以上级别的进行回滚
for i in range(1,20):
    logging.error('This is error message!')
    for j in range(1,10):
        logging.info('This is info  message!')
        for k in range(1,5):
            logging.debug('This is debug message!')

