#include "fragment.h"

Fragment::Fragment(const int begin, const int end, const int freqency)
{
    Q_ASSERT(freqency > 0);
    m_freq = freqency;
    m_begin = begin;
    m_end = end;
    m_beginS = m_begin / m_freq;
    m_endS = m_end / m_freq;
}

Fragment::Fragment(const double begin, const double end, const int freqency)
{
    Q_ASSERT(freqency > 0);
    m_freq = freqency;
    m_beginS = begin;
    m_endS = end;
    m_begin = static_cast<int>(m_beginS * m_freq);
    m_end = static_cast<int>(m_endS * m_freq);
}

void Fragment::setBegin(const int begin)
{
    m_begin = begin;
    m_beginS = m_begin / m_freq;
}

void Fragment::setEnd(const int end)
{
    m_end = end;
    m_endS = m_end / m_freq;
}

void Fragment::setBeginS(const double begin)
{
    m_beginS = begin;
    m_begin = static_cast<int>(m_beginS * m_freq);
}

void Fragment::setEndS(const double end)
{
    m_endS = end;
    m_end = static_cast<int>(m_endS * m_freq);
}
