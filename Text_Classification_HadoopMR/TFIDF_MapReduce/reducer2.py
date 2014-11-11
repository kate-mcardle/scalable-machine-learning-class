#!/usr/bin/env python

"""
Receives (doc, word_count) pairs from mapper2
and provides (doc, list(word_tf)) pairs for mapper3
"""

import sys

current_doc = None
n_terms = 0
current_dict = {}

for line in sys.stdin:
  line = line.strip()
  doc, word_count = line.split('\t', 1)
  word, count = word_count.split('_', 1)
  try:
    count = int(count)
  except ValueError:
    continue

  if current_doc == doc:
    n_terms += count
    if word in current_dict:
      current_dict[word] += count
    else:
      current_dict[word] = count
  else:
    # We are done with the previous doc, so write result to STDOUT
    if current_doc:
      l = ""
      for key in current_dict:
        l += "(" + key + "," + str(float(current_dict[key])/n_terms) + ") "
      print '%s\t%s' % (current_doc, l)

    current_doc = doc
    n_terms = count
    current_dict = {word:count}

# Don't forget about the last doc!
if current_doc == doc:
  l = ""
  for key in current_dict:
    l += "(" + key + "," + str(float(current_dict[key])/n_terms) + ") "
  print '%s\t%s' % (current_doc, l)