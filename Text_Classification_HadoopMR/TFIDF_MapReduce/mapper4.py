#!/usr/bin/env python

"""
Receives (doc, wordID, tfidf) tuples from reducer3
and provides (doc, wordID_tfidf) pairs for reducer4
"""

import sys

for line in sys.stdin:
  doc, wordID, tfidf = line.split()
  print '%s\t%s' % (doc, wordID+"_"+tfidf)