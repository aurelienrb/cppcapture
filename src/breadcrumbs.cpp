#include "cppcapture/breadcrumbs.h"

#include "encoder/json_encoder.h"
#include "systemutils.h"

#include <sstream>

#ifdef CPPCAPTURE_WITH_QT
#include <QDebug>
#endif

namespace cppcapture {

    void BreadcrumbMessage::PrintToLogger() const {
#ifdef CPPCAPTURE_WITH_QT
        QString msg;

        if (!m_message.empty()) {
            msg = QString::fromStdString(m_message);
        } else {
            const auto it = m_extra.find("function");
            if (it != m_extra.end()) {
                msg = QString{ "- %1()" }.arg(QString::fromStdString(it->second));
            }
        }

        qDebug() << msg;
#endif
    }

    void BreadcrumbMessage::ToJSON(JsonEncoder & json) const {
        json.append("timestamp", makeTimestampISO8601());
        json.append("level", ToString(m_level));
        if (!m_message.empty()) {
            json.append("message", m_message);
        }
        json.append("data", m_extra);
    }

    Breadcrumbs & Breadcrumbs::Instance() {
        static Breadcrumbs s_instance;
        return s_instance;
    }

    void Breadcrumbs::ToJSON(JsonEncoder & json) const {
        std::lock_guard<std::mutex> lock{ m_mutex };

        json.beginBlock("breadcrumbs");
        json.beginArray("values");
        for (const auto & msg : m_messages) {
            json.beginArrayElement();
            msg.ToJSON(json);
            json.endArrayElement();
        }
        json.endArray();
        json.endBlock();
    }
}
