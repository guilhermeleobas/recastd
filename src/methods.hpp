#pragma once

#include "encounter.hpp"

extern uint max_nodes;
extern uint max_days;

void merge_encounters (vector<encounter>&);

void sort_encounters (vector<encounter>& v);

void set_day(vector<encounter>& v);

void set_properties(vector<encounter>& v);

void calc_max_nodes(const vector<encounter>& v);