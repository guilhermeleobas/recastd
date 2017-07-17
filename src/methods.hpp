#pragma once

#include "encounter.hpp"

extern uint max_nodes;
extern uint max_days;

void calc_t(const vector<encounter>&);

void subtract_min_ti(vector<encounter>&);

void merge_encounters (vector<encounter>&);

void sort_encounters (vector<encounter>& v);

void set_day(vector<encounter>& v);

void set_properties(vector<encounter>& v);

void calc_max_nodes(const vector<encounter>& v);
