#ifndef FRAGMENT_H
#define FRAGMENT_H

#include <QObject>

/*!
 * \brief Класс - описатель фрагмента сигнала Fragment class
 */
class Fragment
{
public:
    explicit Fragment(const int begin, const int end, const int freqency = 50);
    explicit Fragment(const double begin, const double end, const int freqency = 50);

    int frequency() const {return m_freq;}

    int begin() const {return m_begin;}
    void setBegin(const int begin);

    int end() const {return m_end;}
    void setEnd(const int end);

    double beginS() const {return m_beginS;}
    void setBeginS(const double begin);

    double endS() const {return m_endS;}
    void setEndS(const double end);

private:
    int m_freq {50};
    int m_begin {-1};
    int m_end {-1};
    double m_beginS {-1};
    double m_endS {-1};
};

#endif // FRAGMENT_H
