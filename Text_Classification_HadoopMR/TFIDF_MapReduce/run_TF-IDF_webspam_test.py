#!/usr/bin/env python

import subprocess
import sys

def main(argv):
  docset = "webspam/test"

  subprocess.call(["hadoop", "fs", "-rm", "-r", "/user/kmcardle/"+docset])

  # Get vocabulary from webspam/train by running modified MapReduce 3 on webspam/train:
  subprocess.call(["hadoop", "fs", "-rm", "-r", "/user/kmcardle/webspam/vocab"])
  subprocess.call(["hadoop", "jar", "/usr/lib/hadoop-mapreduce/hadoop-streaming.jar", 
    "-mapper", "/home/kmcardle/ScalableML/HW6/mapper3.py", 
    "-reducer", "/home/kmcardle/ScalableML/HW6/reducer3_vocab.py", 
    "-input", "/user/kmcardle/webspam/train/r2_output/*", 
    "-output", "/user/kmcardle/webspam/vocab"])

  subprocess.call(["hadoop", "fs", "-copyToLocal", "/user/kmcardle/webspam/vocab/part-00000",
    "/home/kmcardle/ScalableML/HW6/vocab"])

  # Run MR process on webspam/test
  # MapReduce 1:
  subprocess.call(["hadoop", "jar", "/usr/lib/hadoop-mapreduce/hadoop-streaming.jar", 
    "-files", "/home/kmcardle/ScalableML/HW6/vocab",
    "-mapper", "/home/kmcardle/ScalableML/HW6/mapper1.py", 
    "-reducer", "/home/kmcardle/ScalableML/HW6/reducer1.py", 
    "-input", "/user/naga86/hw6/"+docset+"/*/*", 
    "-output", "/user/kmcardle/"+docset+"/r1_output",
    "-cmdenv", "docset="+docset])

  subprocess.call(["hadoop", "fs", "-rm", "/user/kmcardle/"+docset+"/r1_output/_SUCCESS"])

  # MapReduce 2:
  subprocess.call(["hadoop", "jar", "/usr/lib/hadoop-mapreduce/hadoop-streaming.jar", 
    "-mapper", "/home/kmcardle/ScalableML/HW6/mapper2.py", 
    "-reducer", "/home/kmcardle/ScalableML/HW6/reducer2.py", 
    "-input", "/user/kmcardle/"+docset+"/r1_output/*", 
    "-output", "/user/kmcardle/"+docset+"/r2_output"])

  subprocess.call(["hadoop", "fs", "-rm", "/user/kmcardle/"+docset+"/r2_output/_SUCCESS"])

  # Determine number of files in collection:
  p = subprocess.Popen(["hadoop", "fs", "-ls", "-R", "/user/naga86/hw6/"+docset], stdout=subprocess.PIPE)
  out, err = p.communicate()
  a = len(out.split('\n'))

  p = subprocess.Popen(["hadoop", "fs", "-ls", "/user/naga86/hw6/"+docset], stdout=subprocess.PIPE)
  out, err = p.communicate()
  b = len(out.split('\n'))
  N = a - b + 1

  # MapReduce 3:
  subprocess.call(["hadoop", "jar", "/usr/lib/hadoop-mapreduce/hadoop-streaming.jar", 
    "-mapper", "/home/kmcardle/ScalableML/HW6/mapper3.py", 
    "-reducer", "/home/kmcardle/ScalableML/HW6/reducer3.py", 
    "-input", "/user/kmcardle/"+docset+"/r2_output/*", 
    "-output", "/user/kmcardle/"+docset+"/r3_output",
    "-cmdenv", "N="+str(N),
    "-cmdenv", "docset="+docset])

  subprocess.call(["hadoop", "fs", "-rm", "/user/kmcardle/"+docset+"/r3_output/_SUCCESS"])
  # MapReduce 4:
  subprocess.call(["hadoop", "jar", "/usr/lib/hadoop-mapreduce/hadoop-streaming.jar", 
    "-mapper", "/home/kmcardle/ScalableML/HW6/mapper4.py", 
    "-reducer", "/home/kmcardle/ScalableML/HW6/reducer4.py", 
    "-input", "/user/kmcardle/"+docset+"/r3_output/*", 
    "-output", "/user/kmcardle/"+docset+"/r4_output",])

if __name__ == '__main__':
  main(sys.argv)