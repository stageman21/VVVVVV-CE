// Copyright 2006 Nemanja Trifunovic

/*
Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/


#ifndef UTF8_FOR_CPP_UNCHECKED_H_2675DCD0_9480_4c0c_B92A_CC14C027B731
#define UTF8_FOR_CPP_UNCHECKED_H_2675DCD0_9480_4c0c_B92A_CC14C027B731

#include "core.h"

namespace utf8
{
    namespace unchecked
    {
        template <typename octet_iterator>
        uint32_t next(octet_iterator& it)
        {
            uint32_t cp = utf8::internal::mask8(*it);
            typename std::iterator_traits<octet_iterator>::difference_type length = utf8::internal::sequence_length(it);
            switch (length) {
                case 1:
                    break;
                case 2:
                    it++;
                    cp = ((cp << 6) & 0x7ff) + ((*it) & 0x3f);
                    break;
                case 3:
                    ++it; 
                    cp = ((cp << 12) & 0xffff) + ((utf8::internal::mask8(*it) << 6) & 0xfff);
                    ++it;
                    cp += (*it) & 0x3f;
                    break;
                case 4:
                    ++it;
                    cp = ((cp << 18) & 0x1fffff) + ((utf8::internal::mask8(*it) << 12) & 0x3ffff);                
                    ++it;
                    cp += (utf8::internal::mask8(*it) << 6) & 0xfff;
                    ++it;
                    cp += (*it) & 0x3f; 
                    break;
            }
            ++it;
            return cp;
        }

        template <typename octet_iterator>
        uint32_t prior(octet_iterator& it)
        {
            while (utf8::internal::is_trail(*(--it))) ;
            octet_iterator temp = it;
            return utf8::unchecked::next(temp);
        }

        template <typename octet_iterator>
        typename std::iterator_traits<octet_iterator>::difference_type
        distance (octet_iterator first, octet_iterator last)
        {
            typename std::iterator_traits<octet_iterator>::difference_type dist;
            for (dist = 0; first < last; ++dist) 
                utf8::unchecked::next(first);
            return dist;
        }
    } // namespace utf8::unchecked
} // namespace utf8 


#endif // header guard

