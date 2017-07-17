#ifndef SAMPLE_H
#define SAMPLE_H 1

namespace raytr
{

    class Sample
    {
    public:
        inline Sample() : x(0.0), y(0.0) {}
        ~Sample() {};

        float x, y;
        int ix, iy;
    };

}
#endif

