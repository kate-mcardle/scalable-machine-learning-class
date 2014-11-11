#!/usr/bin/env python

""" 
Receives lines of documents in some collection, 
and provides (doc_word, 1) pairs for reducer1
"""

import sys
import os
import string

doc_path = None
doc = None
docset = os.environ["docset"]

if docset == "webspam/test":
  vocab = {}
  with open('vocab', 'r') as f:
    for line in f:
      word, idx = line.split()
      vocab[word] = idx

for line in sys.stdin:
  line = line.strip().lower().translate(string.maketrans("",""), string.punctuation) # remove all punctuation
  words = line.split()
  
  new_doc_path = os.environ["mapreduce_map_input_file"]
  # Only update doc if we haven't seen this doc_path yet
  if doc_path != new_doc_path:
    doc = new_doc_path[58+len(docset):]
    doc_path = new_doc_path
  
  if doc:
    for word in words:
      if docset == "webspam/test":
        if word in vocab.keys():
          print '%s_%s\t%s' % (doc, vocab[word], 1)
      elif word.isalpha():
        # Write the results to STDOUT, to be used by reducer
        print '%s_%s\t%s' % (doc, word, 1)