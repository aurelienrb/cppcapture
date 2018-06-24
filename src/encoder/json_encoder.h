#pragma once

#include <cassert>
#include <string>
#include <unordered_map>

class JsonEncoder {
public:
    JsonEncoder() {
        m_json.reserve(200);
        m_json += "{";
        m_nestLevel++;
    }

    JsonEncoder & append(const std::string & key, const std::string & value) {
        assert(m_nestLevel > 0);
        if (!value.empty()) {
            closePreviousLine();

            m_json += '"';
            m_json += key;
            m_json += "\": \"";
            encodeValue(value);
            m_json += '"';
        }
        return *this;
    }

    JsonEncoder & append(const std::string & key, const std::unordered_map<std::string, std::string> & values) {
        assert(m_nestLevel > 0);
        if (!values.empty()) {
            beginBlock(key);
            for (auto it : values) {
                append(it.first, it.second);
            }
            endBlock();
        }
        return *this;
    }

    JsonEncoder & beginBlock(const std::string & key) {
        assert(m_nestLevel > 0);
        closePreviousLine();
        m_nestLevel++;

        m_json += '"';
        m_json += key;
        m_json += "\": {";

        return *this;
    }

    JsonEncoder & endBlock() {
        assert(m_nestLevel > 1);
        // close previous line without inserting a comma
        m_json += '\n';
        m_nestLevel--;
        indent();
        m_json += '}';

        return *this;
    }

    std::string complete() {
        assert(m_nestLevel == 1);
        m_nestLevel = 0;
        m_json += '\n';
        m_json += '}';
        return std::move(m_json);
    }

private:
    void indent() {
        for (int i = 0; i < m_nestLevel; i++) {
            m_json += ' ';
        }
    }

    void closePreviousLine() {
        assert(m_nestLevel > 0);
        if (m_json.back() != '{') {
            m_json += ',';
        }
        m_json += '\n';
        indent();
    }

    void encodeValue(const std::string & value) {
        for (char c : value) {
            if (c == '"') {
                m_json += '\\';
                m_json += '"';
            } else if (c == '\r') {
                // skip
            } else if (c == '\n') {
                m_json += '\\';
                m_json += 'n';
            } else if (c == '\\') {
                m_json += '\\';
                m_json += '\\';
            } else {
                m_json += c;
            }
        }
    }

private:
    int m_nestLevel = 0;
    std::string m_json;
};
