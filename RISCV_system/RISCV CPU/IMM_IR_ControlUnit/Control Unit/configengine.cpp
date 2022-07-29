#include "configengine.h"
#include <QDebug>
#include <iostream>

ConfigEngine::ConfigEngine()
{
    _error = false;
    _pending = false;
    _reading = false;
}

ConfigEngine::~ConfigEngine()
{
    if (_pending) {
        write();
    }
    if (!_error) {
        _fd.close();
    }
}

bool ConfigEngine::addElementDefinition(QString fullname, QStringList attrs, QStringList defvalues)
{
    QStringList names = fullname.split(':');
    xmlElementDef_t elemdef;
    elemdef.fullname = fullname;
    if (names.size() == 0) {
        elemdef.name = fullname;
        elemdef.context = QStringList();
    } else {
        elemdef.name = names.last(); // [names.size()-1];
        names.pop_back();
        elemdef.context = names;
    }
    elemdef.attrs = attrs;
    elemdef.defvalues = defvalues;
    for (auto d = _definitions.cbegin(); d != _definitions.cend(); d++) {
        if (d->fullname == fullname) {
            // Error, already defined
            return false;
        }
    }
    _definitions.push_back(elemdef);
    return true;
}

int ConfigEngine::findElement(QString fullname)
{
    for (int i = 0; i < _elements.size(); i++) {
        if (_elements.at(i).fullname == fullname) {
            return i;
        }
    }
    return -1;
}

int ConfigEngine::findElement(QString fullname, QString attr, QString val)
{
    int attr_index, i;
    for (i = 0; i < _elements.size(); i++) {
        if (_elements.at(i).fullname == fullname) {
            attr_index = _elements.at(i).attrs.indexOf(attr);
            if (attr_index >= 0) {
                if (_elements.at(i).values.at(attr_index) == val) {
                    return i;
                }
            }
        }
    }
    return -1;
}

bool ConfigEngine::getAttribute(int index, const QString attr, QString &val)
{
    if (index >= _elements.size()) {
        return false;
    }
    for (int i = 0; i < _elements.at(index).attrs.size(); i++) {
        if (_elements.at(index).attrs.at(i) == attr) {
            val = _elements.at(index).values.at(i);
            return true;
        }
    }
    return false;
}

bool ConfigEngine::getAttribute(int index, const QString attr, int &val)
{
    QString str;
    bool res = getAttribute(index, attr, str);
    if (res) {
        val = str.toInt(&res);
    }
    return res;
}

bool ConfigEngine::getAttribute(int index, const QString attr, uint &val)
{
    QString str;
    qulonglong qull = 0;
    bool res = getAttribute(index, attr, str);
    if (res) {
        res = parseAttributeUnsigned(str, qull);
    }
    if (res) {
        val = static_cast<uint>(qull);
    }
    return res;
}

bool ConfigEngine::getAttribute(int index, const QString attr, float &val)
{
    QString str;
    bool res = getAttribute(index, attr, str);
    if (res) {
        val = str.toFloat(&res);
    }
    return res;
}

bool ConfigEngine::getAttribute(int index, const QString attr, double &val)
{
    QString str;
    bool res = getAttribute(index, attr, str);
    if (res) {
        val = str.toDouble(&res);
    }
    return res;
}

bool ConfigEngine::getAttribute(int index, const QString attr, long &val)
{
    QString str;
    bool res = getAttribute(index, attr, str);
    if (res) {
        val = str.toLong(&res);
    }
    return res;
}

bool ConfigEngine::getAttribute(int index, const QString attr, ulong &val)
{
    QString str;
    qulonglong qull = 0;
    bool res = getAttribute(index, attr, str);
    if (res) {
        res = parseAttributeUnsigned(str, qull);
    }
    if (res) {
        val = static_cast<ulong>(qull);
    }
    return res;
}

bool ConfigEngine::getAttribute(int index, const QString attr, qlonglong &val)
{
    QString str;
    bool res = getAttribute(index, attr, str);
    if (res) {
        val = str.toLongLong(&res);
    }
    return res;
}

bool ConfigEngine::getAttribute(int index, const QString attr, qulonglong &val)
{
    QString str;
    bool res = getAttribute(index, attr, str);
    if (res) {
        res = parseAttributeUnsigned(str, val);
    }
    return res;
}

bool ConfigEngine::getChildren(int index, QList<int> &children)
{
    if (index >= _elements.size()) {
        return false;
    }
    children = _elements.at(index).children;
    return true;
}

bool ConfigEngine::write()
{
    if (!_error) {
        _fd.seek(0);
        _fd.resize(_fd.pos());
        _writeFile();
    }
    return !_error;
}

bool ConfigEngine::read()
{
    if (!_error) {
        _fd.seek(0);
        _readFile();
        //_dumpElements();
    }
    return !_error;
}

bool ConfigEngine::setConfigFile(const QString fname)
{
    if (_fd.isOpen()) {
        _fd.close();
    }
    _fd.setFileName(fname);
    _error = !_fd.open(QIODevice::ReadWrite);
    if (!_error) {
        _reader.setDevice(&_fd);
        _writer.setDevice(&_fd);
        _writer.setAutoFormatting(true);
    }
    return !_error;
}

void ConfigEngine::_dumpElements()
{
    std::string avs, chs;
    for (auto e = _elements.cbegin(); e != _elements.cend(); e++) {
        QString av, ch;
        qsizetype i;
        avs.clear(); chs.clear();
        for (i = 0; i < e->attrs.size(); i++) {
            if (i != 0) {
                av.append("; ");
                avs.append("; ");
            }
            av.append(e->attrs.at(i));
            av.append(" = ");
            av.append(e->values.at(i));
            avs.append(e->attrs.at(i).toStdString());
            avs.append(" = ");
            avs.append(e->values.at(i).toStdString());
        }
        for (i = 0; i < e->children.size(); i++) {
            if (i != 0) {
                ch.append("; ");
                chs.append("; ");
            }
            ch.append(QString::number(e->children.at(i)));
            chs.append(QString::number(e->children.at(i)).toStdString());
        }
        std::cout << "DUMP: " << e->name.toStdString() << "(" << e->fullname.toStdString() << ") at index " << e->me << ", parent " << e->parent << std::endl;
        if (e->attrs.size() > 0) {
            std::cout << "      " << avs << std::endl;
        }
        if (e->children.size() > 0) {
            std::cout << "      " << chs << std::endl;
        }
    }
}

void ConfigEngine::_readFile()
{
    QXmlStreamReader::TokenType token;

    _reading = true;
    _context.clear();
    _parent.clear();
    _parent.push_back(-1);
    while (!_reader.atEnd() && !_error) {
        token = _reader.readNext();
        if (token == QXmlStreamReader::Invalid) {
            break;
        }
        if (_reader.isStartDocument()) {
            // skip but enter
            continue;
        }
        if (_reader.isComment()) {
            // skip completely
            _reader.skipCurrentElement();
            continue;
        }
        if (_reader.isEndDocument()) {
            // finish
            break;
        }
        if (_reader.isStartElement()) {
            if (!_readElement(_reader.name(), _reader.attributes())) {
                break;
            }
        }
    }
    if (_reader.hasError()) {
        if (!((_reader.lineNumber() == 1) && (_reader.columnNumber() == 0))) {
            _error = true;
            qCritical() << "ERROR ConfigEngine::readConfig() at " + QString::number(_reader.lineNumber()) + ":" + QString::number(_reader.columnNumber());
            qCritical() << "ERROR description is: " + _reader.errorString();
        }
    }
    _reading = false;
}

bool ConfigEngine::_readElement(QStringView name, QXmlStreamAttributes attrs)
{
    QXmlStreamReader::TokenType token;
    bool found = false;
    int i;
    auto elemdef = _definitions.cbegin();
    for (; elemdef != _definitions.cend(); elemdef++) {
        if ((elemdef->name == name) && (elemdef->context == _context)) {
            found = true;
            break;
        }
    }
    if (found) {
        // 1. Add element to the XML parsed data
        xmlElement_t elem;
        elem.me = _elements.size();
        elem.parent = _parent.last();
        elem.children.clear();
        elem.fullname = elemdef->fullname;
        elem.name = elemdef->name;
        elem.attrs = elemdef->attrs;
        i = 0;
        for (auto a = elem.attrs.cbegin(); a != elem.attrs.cend(); i++, a++) {
            if (attrs.hasAttribute(*a)) {
                elem.values.push_back(attrs.value(*a).toString());
            } else {
                if (elemdef->defvalues.size() > i) {
                    elem.values.push_back(elemdef->defvalues.at(i));
                } else {
                    elem.values.push_back("");
                }

            }
        }
        _elements.push_back(elem);
        // 2. Add element to parent's children list
        if (elem.parent >= 0) {
            _elements[elem.parent].children.push_back(elem.me);
        }
        // 3. Continue parsing, increasing depth of context
        _parent.push_back(elem.me);
        _context.push_back(elemdef->name);
        while (!_reader.atEnd() && !_error) {
            token = _reader.readNext();
            if (token == QXmlStreamReader::Invalid) {
                _error = true;
                return false;
            }
            if (_reader.isComment()) {
                // Skip completely
                _reader.skipCurrentElement();
                continue;
            }
            if (_reader.isEndDocument() || _reader.isEndElement()) {
                // Finish
                _parent.pop_back();
                _context.pop_back();
                return true;
            }
            if (_reader.isStartElement()) {
                if (!_readElement(_reader.name(), _reader.attributes())) {
                    break;
                }
            }
        }
    } else {
        _reader.skipCurrentElement();
    }
    return true;
}

void ConfigEngine::_writeFile()
{
#if 0
    _writer.writeStartDocument();
    _writer.writeStartElement("configuration");
    _writeConfig();
    _writer.writeEndElement();
    _writer.writeEndDocument();
#endif
}

bool ConfigEngine::parseAttributeUnsigned(const QString attr, qulonglong &val)
{
    bool ok;
    int base = 10;
    QString value;

    if (attr.contains("0x")) {
        base = 16;
        value = attr.sliced(2);
    } else if (attr.contains("x")) {
        base = 16;
        value = attr.sliced(1);
    } else if (attr.contains("b")) {
        base = 2;
        value = attr.sliced(0,attr.length()-1);
    } else {
        base = 10;
        value = attr;
    }
    val = value.toULongLong(&ok,base);
    return ok;
}
