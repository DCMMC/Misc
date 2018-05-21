/**
 * ! some simple but useful utils, Fuck Cpp
 * * @author DCMMC
 * * @since C++ 11
 */

#ifndef __DCMMC_UTILS__

#define __DCMMC_UTILS__

#include <iostream>
#include <string>
#include <vector>

namespace utils {
    /****************************
     * string utils
     ****************************/

    /**
     * split string by delims
     * * @param text
     *      text that you want to split
     * * @param delims
     *      if is "" then return the whole sting 'text'
     */
    std::vector<std::string> split(const std::string &text, const std::string &delims = "") {
        std::vector<std::string> tokens;
        std::size_t start = text.find_first_not_of(delims), end = 0;
        while((end = text.find_first_of(delims, start)) != std::string::npos) {
            tokens.push_back(text.substr(start, end - start));
            start = text.find_first_not_of(delims, end);
        }
        if(start != std::string::npos)
            tokens.push_back(text.substr(start));

        return tokens;
    }
}


#endif // __DCMMC_UTILS__