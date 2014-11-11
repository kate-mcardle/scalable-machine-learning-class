#!/usr/bin/env python

import subprocess
import sys

def main(argv):
  docset = argv[1]

  subprocess.call(["hadoop", "fs", "-rm", "-r", "/user/kmcardle/"+docset])

  # MapReduce 1:
  if docset == "webspam/train":
    splits = ['neg/[03456789]', 'neg/[12]', 'pos/[3456789]', 'pos/1', 'pos/2']
    # splits = ['neg/[3]']
    count = 0
    for split in splits:
      output_dir = "/user/kmcardle/"+docset+"/temp_output/"+str(count)
      subprocess.call(["hadoop", "jar", "/usr/lib/hadoop-mapreduce/hadoop-streaming.jar", 
        "-mapper", "/home/kmcardle/ScalableML/HW6/mapper1.py", 
        "-reducer", "/home/kmcardle/ScalableML/HW6/reducer1.py", 
        "-input", "/user/naga86/hw6/"+docset+"/"+split+"*", 
        "-output", output_dir,
        "-cmdenv", "docset="+docset])
      subprocess.call(["hadoop", "fs", "-mv", output_dir+"/part-00000",
        output_dir+"/part-00000"+str(count)])
      if count == 0:
        subprocess.call(["hadoop", "fs", "-mkdir", "/user/kmcardle/webspam/train/r1_output"])
      subprocess.call(["hadoop", "fs", "-mv", output_dir+"/part-00000"+str(count),
        "/user/kmcardle/"+docset+"/r1_output"])
      subprocess.call(["hadoop", "fs", "-rm", "-r", output_dir])
      count += 1
  else:
    subprocess.call(["hadoop", "jar", "/usr/lib/hadoop-mapreduce/hadoop-streaming.jar", 
      "-mapper", "/home/kmcardle/ScalableML/HW6/mapper1.py", 
      "-reducer", "/home/kmcardle/ScalableML/HW6/reducer1.py", 
      "-input", "/user/naga86/hw6/"+docset+"/*/*", 
      "-output", "/user/kmcardle/"+docset+"/r1_output",
      "-cmdenv", "docset="+docset])


    # subprocess.call(["hadoop", "jar", "/usr/lib/hadoop-mapreduce/hadoop-streaming.jar", 
    #   "-mapper", "/home/kmcardle/ScalableML/HW6/mapper1.py", 
    #   "-reducer", "/home/kmcardle/ScalableML/HW6/reducer1.py", 
    #   "-input", "/user/naga86/hw6/"+docset+"/pos/[2678]*", 
    #   "-output", "/user/kmcardle/"+docset+"/r1_output",
    #   "-cmdenv", "docset="+docset])

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

  # if docset == "webspam/train":
  subprocess.call(["hadoop", "fs", "-rm", "/user/kmcardle/"+docset+"/r3_output/_SUCCESS"])
  # MapReduce 4:
  subprocess.call(["hadoop", "jar", "/usr/lib/hadoop-mapreduce/hadoop-streaming.jar", 
    "-mapper", "/home/kmcardle/ScalableML/HW6/mapper4.py", 
    "-reducer", "/home/kmcardle/ScalableML/HW6/reducer4.py", 
    "-input", "/user/kmcardle/"+docset+"/r3_output/*", 
    "-output", "/user/kmcardle/"+docset+"/r4_output",])

if __name__ == '__main__':
  main(sys.argv)