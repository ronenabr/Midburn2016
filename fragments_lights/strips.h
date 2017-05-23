#ifndef STRIPS_H
#define STRIPS_H  

using uint = unsigned int; 
using uint8 = unsigned char; 

constexpr unsigned int NumberOfStrips = 5;

static inline int8_t sign(int8_t val) {
 if (val < 0) return -1;
 if (val==0) return 0;
 return 1;
}

struct Strip
{
    uint first;
    uint last;    

    
    //return -1 or +1; 
    int step() const
    {
        return sign(last-first); 
    }

    uint length() const
    {
        return (last-first)*step(); 
    }
    uint index_in(uint idx)
    {
        if (idx>length())
        {
            return index_in(idx%length()); 
        }
        return first + idx * step(); 
    }
};

Strip pillars[NumberOfStrips];


#endif //STRIPS_H