#!/usr/bin/env python

"""
Receives (word, doc_tf) pairs from mapper3
and provides final (doc, word, tfidf) tuples
Note: doc will be doc ID in output for classic datasets
Note: assumes only one reducer (machine) is used
"""

import sys
import math
import os

N = float(os.environ["N"])
docset = os.environ["docset"]

current_word = None
word_idx = 1
n_docs = 0
docs_to_tfs = {}
doc_indices = {}
doc_idx = 1

for line in sys.stdin:
  line = line.strip()
  word, doc_tf = line.split('\t', 1)
  doc, tf = doc_tf.split('_', 1)
  tf = float(tf)
  if doc not in doc_indices:
    doc_indices[doc] = doc_idx
    doc_idx += 1

  if current_word == word:
    n_docs += 1
    docs_to_tfs[doc] = tf
  else:
    # we are done with the previous word, so write result to STDOUT
    if current_word:
      # l = ""
      idf = math.log(N/n_docs)
      for key in docs_to_tfs:
        if docset[:7] == "classic":
        # if docset[:7] == "dlassic":
          this_doc_idx = doc_indices[key]
          print '%s\t%s\t%s' % (this_doc_idx, word_idx, str(docs_to_tfs[key]*idf))
        elif docset == "webspam/test":
          print '%s\t%s\t%s' % (key, current_word, str(docs_to_tfs[key]*idf))
        else:
          print '%s\t%s\t%s' % (key, word_idx, str(docs_to_tfs[key]*idf))
      word_idx += 1

    current_word = word
    n_docs = 1
    docs_to_tfs = {doc:tf}

# Don't forget about the last word!
if current_word == word:
  idf = math.log(N/n_docs)
  for key in docs_to_tfs:
    if docset[:7] == "classic":
    # if docset[:7] == "dlassic":
      this_doc_idx = doc_indices[key]
      print '%s\t%s\t%s' % (this_doc_idx, word_idx, str(docs_to_tfs[key]*idf))
    elif docset == "webspam/test":
      print '%s\t%s\t%s' % (key, current_word, str(docs_to_tfs[key]*idf))
    else:
      print '%s\t%s\t%s' % (key, word_idx, str(docs_to_tfs[key]*idf))