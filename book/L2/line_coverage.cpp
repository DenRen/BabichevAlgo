#include <iostream>
#include <vector>
#include <string>

bool is_line_coverage (const std::string& proposal,
                       const std::vector <std::string>& samples);

int main () {
    std::string proposal;
    std::cin >> proposal;

    std::size_t size = 0;
    std::cin >> size;
    std::vector <std::string> samples (size);

    for (auto& sample : samples) {
        std::cin >> sample;
    }

    std::cout << std::boolalpha << is_line_coverage (proposal, samples) << std::endl;
}

bool is_line_coverage_naivly (const std::string& proposal,
                              const std::vector <std::string>& samples)
{
    for (auto pos = proposal.cbegin (); pos < proposal.cend ();) {
        bool is_find = true;

        for (const auto& sample : samples) {
            auto cur_pos = pos;
            is_find = true;
            
            for (const auto sample_symb : sample) {
                if (cur_pos == proposal.cend () || *cur_pos != sample_symb) {
                    is_find = false;
                    break;
                }
                ++cur_pos;
            }

            if (is_find) {
                pos = cur_pos;
                break;
            }
        }

        if (!is_find) {
            return false;
        }
    }

    return true;
}

bool is_line_coverage (const std::string& prop,
                       const std::vector <std::string>& samples)
{
    return is_line_coverage_naivly (prop, samples);
}