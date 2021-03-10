//
// Created by artem on 10.03.2021.
//

#ifndef STC_FIND_LOCAL_PEAKS_H
#define STC_FIND_LOCAL_PEAKS_H

#include <vector>

void find_local_peaks(const std::vector<float>& v_in, std::vector< size_t >& idx_peak, size_t len_win, float level);

#endif //STC_FIND_LOCAL_PEAKS_H
