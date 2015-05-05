#ifndef JOB_HPP
#define JOB_HPP

#include <QVector>

class Job
{
    public:
        Job();
        ~Job();
    private:
        int id;
        QVector< QVector<float> > inputs;
};

#endif // JOB_HPP
