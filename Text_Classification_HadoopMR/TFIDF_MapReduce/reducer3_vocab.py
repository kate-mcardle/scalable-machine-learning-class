#!/usr/bin/env python

"""
Receives (word, doc_tf) pairs from mapper3
and provides (word, idx) pairs for vocab
Note: assumes only one reducer (machine) is used
"""

import sys

last_word = None
word_idx = 1

for line in sys.stdin:
  line = line.strip()
  word = line.split()[0]
  if last_word != word:
    print '%s\t%s' % (word, str(word_idx))
    last_word = word
    word_idx += 1