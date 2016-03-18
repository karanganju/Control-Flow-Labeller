#!/bin/bash

make
./cg_label ../out
if [[ $12 -e --no-graph ]]; then
   exit 1
fi
dot -Tps graph.dot -o graph.ps
gnome-open graph.ps
make clean