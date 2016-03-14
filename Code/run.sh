#!/bin/bash

./cg_label ../out
dot -Tps graph.dot -o graph.ps
gnome-open graph.ps