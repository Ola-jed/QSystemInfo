#ifndef QSYSTEMINFO_CONSTEXPRQSTRING_HPP
#define QSYSTEMINFO_CONSTEXPRQSTRING_HPP

struct ConstexprQString : public QLatin1String
{
    constexpr explicit ConstexprQString(const char *const s) :
            QLatin1String(s, static_cast<int>(std::char_traits<char>::length(s)))
    {
    }
};

#endif //QSYSTEMINFO_CONSTEXPRQSTRING_HPP
