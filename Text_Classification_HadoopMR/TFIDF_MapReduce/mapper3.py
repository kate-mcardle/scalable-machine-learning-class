#!/usr/bin/env python

"""
Receives (doc, list(word_tf)) pairs from reducer2
and provides (word, doc_tf) pairs for reducer3
"""

import sys

for line in sys.stdin:
  line = line.strip()
  doc, word_tf_string = line.split('\t', 1)
  # doc = word_tfs[0]
  # word_tfs = word_tfs[1:]
  word_tfs = word_tf_string.split()
  for word_tf in word_tfs: # (word, tf)
    word_tf = word_tf[1:-1]
    word, tf = word_tf.split(',',1)
    print '%s\t%s_%s' % (word, doc, tf)