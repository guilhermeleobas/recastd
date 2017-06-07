#pragma once

#include "encounter.hpp"

void merge_encounters (vector<shared_ptr<encounter>>& v);

void sort_encounters (vector<shared_ptr<encounter>>& v);

void set_min_max (const vector<shared_ptr<encounter>>& v);

void set_day(vector<shared_ptr<encounter>>& v);

void set_properties(vector<shared_ptr<encounter>>& v);
