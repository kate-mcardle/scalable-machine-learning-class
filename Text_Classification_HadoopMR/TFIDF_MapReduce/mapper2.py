#!/usr/bin/env python

"""
Receives (doc_word, count) pairs from reducer1
and provides (doc, word_count) pairs for reducer2
"""

import sys

for line in sys.stdin:
  line = line.strip()
  doc_word, count = line.split()
  doc, word = doc_word.split('_', 1)
  try:
    count = int(count)
  except ValueError:
    continue
  print '%s\t%s_%s' % (doc, word, count)