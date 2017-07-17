
bazel build src:recastd && ./bazel-bin/src/recastd -in traces/wdm/wdm.txt -st lastk -maxlen 2000 -out /tmp/wdm_last2000

cd python 

pypy recastd_edgelist.py /tmp/wdm_last2000
