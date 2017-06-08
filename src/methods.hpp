#pragma once

#include "encounter.hpp"

extern uint max_nodes;
extern uint max_days;

void merge_encounters (vector<encounter>& v);

void sort_encounters (vector<encounter>& v);

void set_min_max (const vector<encounter>& v);

void set_day(vector<encounter>& v);

void set_properties(vector<encounter>& v);

void set_index(vector<encounter> &v);
