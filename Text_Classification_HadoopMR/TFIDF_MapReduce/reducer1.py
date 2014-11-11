#!/usr/bin/env python

"""
Receives (doc_word, 1) pairs from mapper1
and provides (doc_word, count) pairs for mapper2
"""

import sys

current_doc_word = None
current_count = 0
doc_word = None

for line in sys.stdin:
  line = line.strip()
  # Parse the input we got from mapper1.py
  doc_word, count = line.split('\t', 1)
  try:
    count = int(count)
  except ValueError:
    # If count was not a number, silently ignore this line
    continue
  # Note: this IF-switch only works bc Hadoop sorts map output by key before giving to reducer
  if current_doc_word == doc_word:
    current_count += count
  else:
    # We are done with the previous doc_word (if there was one), so write result to STDOUT
    if current_doc_word:
      print '%s\t%s' % (current_doc_word, current_count)
    current_doc_word = doc_word
    current_count = count

# Don't forget about the last word
if current_doc_word == doc_word:
  print '%s\t%s' % (current_doc_word, current_count)