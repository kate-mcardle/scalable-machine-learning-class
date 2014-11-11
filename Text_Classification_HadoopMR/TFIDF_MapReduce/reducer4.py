#!/usr/bin/env python

"""
Receives (doc, wordID_tfidf) tuples from mapper4
and provides final (doc_label, list(wordID:tfidf)) format
"""

import sys

current_dict = {}
current_doc = None

for line in sys.stdin:
  line = line.strip()
  doc, word_tfidf = line.split()
  word, tfidf = word_tfidf.split('_', 1)
  try:
    word = int(word)
  except ValueError:
    continue
  if current_doc == doc:
    current_dict[word] = tfidf
  else:
    # We are done with the previous doc, so write result to STDOUT
    if current_doc:
      if current_doc[1:2] == "/":
        label = current_doc[:1]
      elif current_doc[:1] == "n":
        label = "-1"
      elif current_doc[:1] == "p":
        label = "+1"
      else:
        label = current_doc
      sorted_list = sorted(current_dict.items())
      l = ""
      for pair in sorted_list:
        l += str(pair[0]) + ":" + pair[1] + "\t"
      print '%s\t%s' % (label, l)
    current_doc = doc
    current_dict = {word:tfidf}

if current_doc == doc:
  if current_doc[1:2] == "/":
    label = current_doc[:1]
  elif current_doc[:1] == "n":
    label = "-1"
  elif current_doc[:1] == "p":
    label = "+1"
  else:
    label = current_doc
  sorted_list = sorted(current_dict.items())
  l = ""
  for pair in sorted_list:
    l += str(pair[0]) + ":" + pair[1] + "\t"
  print '%s\t%s' % (label, l)