#pragma once

#include <string>

#ifdef CPPCAPTURE_WITH_QT
#include <QString>
#endif

namespace cppcapture {
    // utility type used to convert various string types into std::string
    struct AnyString {
        AnyString(const char * s) : str(s) {
        }
        AnyString(std::string s) : str(std::move(s)) {
        }

#ifdef CPPCAPTURE_WITH_QT
        AnyString(const QString & s) : str(s.toStdString()) {
        }
        AnyString(const QLatin1String & s) : str(s.latin1()) {
        }
#endif
        std::string str;
    };
}
