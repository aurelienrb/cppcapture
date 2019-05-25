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

    JsonEncoder & append(const char * key, int value) {
        return appendRawValue(key, std::to_string(value));
    }

    JsonEncoder & append(const char * key, size_t value) {
        return appendRawValue(key, std::to_string(value));
    }

    JsonEncoder & append(const char * key, const std::string & value) {
        return appendRawValue(key, "\"" + value + "\"");
    }

    JsonEncoder & appendRawValue(const std::string & key, const std::string & rawValue) {
        assert(m_nestLevel > 0);
        if (!rawValue.empty()) {
            startNewLine();

            m_json += '"';
            m_json += key;
            m_json += "\": ";
            m_json += rawValue;
        }
        return *this;
    }

    JsonEncoder & append(const char * key, const std::unordered_map<std::string, std::string> & values) {
        assert(m_nestLevel > 0);
        if (!values.empty()) {
            beginBlock(key);
            for (auto it : values) {
                append(it.first.c_str(), it.second);
            }
            endBlock();
        }
        return *this;
    }

    JsonEncoder & beginBlock(const std::string & key) {
        assert(!m_insideArray);
        return open(key, '{');
    }

    JsonEncoder & endBlock() {
        assert(!m_insideArray);
        return close('}');
    }

    JsonEncoder & beginArray(const std::string & key) {
        assert(!m_insideArray);
        m_insideArray = true;
        return open(key, '[');
    }

    JsonEncoder & endArray() {
        assert(m_insideArray);
        m_insideArray = false;
        return close(']');
    }

    JsonEncoder & beginArrayElement() {
        assert(m_insideArray);
        startNewLine();
        m_nestLevel++;
        m_json += '{';
        return *this;
    }

    JsonEncoder & endArrayElement() {
        assert(m_insideArray);
        return close('}');
    }

    std::string complete() {
        assert(m_nestLevel == 1);
        m_nestLevel = 0;
        m_json += '\n';
        m_json += '}';
        return std::move(m_json);
    }

private:
    JsonEncoder & open(const std::string & key, char delimiter) {
        assert(delimiter == '{' || delimiter == '[');
        assert(m_nestLevel > 0);
        startNewLine();
        m_nestLevel++;

        m_json += '"';
        m_json += key;
        m_json += "\": ";
        m_json += delimiter;

        return *this;
    }

    JsonEncoder & close(char delimiter) {
        assert(delimiter == '}' || delimiter == ']');
        assert(m_nestLevel > 1);

        // close previous line without inserting a comma
        m_json += '\n';
        m_nestLevel--;
        indent();
        m_json += delimiter;

        return *this;
    }

    void indent() {
        for (int i = 0; i < m_nestLevel; i++) {
            m_json += ' ';
        }
    }

    void startNewLine() {
        assert(m_nestLevel > 0);
        if (m_json.back() != '{' && m_json.back() != '[') {
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
    bool m_insideArray = false;
    std::string m_json;
};
