#ifndef CONFIGENGINE_H
#define CONFIGENGINE_H

#include <stdint.h>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QString>
#include <QList>
#include <QStringList>
#include <QVector>

/** @TODO: Change the configuration engine to be thread-safe? */

typedef struct xmlElementDef_s {
    QString fullname, name;
    QStringList context;
    QStringList attrs;
    QStringList defvalues;
} xmlElementDef_t;

typedef struct xmlElement_s {
    int me, parent;
    QVector<int> children;
    QString fullname, name;
    QStringList attrs;
    QStringList values;
    xmlElement_s() : me(-1), parent(-1), children(10,-1), name(""), attrs(), values() {}
} xmlElement_t;

/**
 * @brief The ConfigEngine class
 *
 * Helper class to read/write configuration values from/to XML file.
 * The application must provide sections and values to the ConfigEngine
 * through @sa addSection(), @sa addValue() before reading or writing.
 *
 * Values added before reading are considered default values and will be
 * used in absence such values in the configuration file.
 * After the @sa read() operation values are updated from config file;
 * if any default value is *not* found in the configuration file
 * the configuration is marked as pending so as to save those values
 * onto the configuration file either when @sa write() is called
 * or during desctructor execution.
 *
 * The ConfigEngine class is data agnostic, all values are QString;
 * it is the resposibility of the application to translate its
 * configuration values from/to QString to use this class.
 */
class ConfigEngine
{
public:
    ConfigEngine();
    ~ConfigEngine();

    bool addElementDefinition(QString fullname, QStringList attrs = QStringList(), QStringList defvalues = QStringList());
    int findElement(QString fullname);
    int findElement(QString fullname, QString attr, QString val);
    bool getChildren(int index, QList<int> &children);
    bool getAttribute(int index, const QString attr, QString &val);
    bool getAttribute(int index, const QString attr, int &val);
    bool getAttribute(int index, const QString attr, uint &val);
    bool getAttribute(int index, const QString attr, float &val);
    bool getAttribute(int index, const QString attr, double &val);
    bool getAttribute(int index, const QString attr, long &val);
    bool getAttribute(int index, const QString attr, ulong &val);
    bool getAttribute(int index, const QString attr, qlonglong &val);
    bool getAttribute(int index, const QString attr, qulonglong &val);

    bool parseAttributeUnsigned(const QString attr, qulonglong &val);

    bool hasError() const { return _error; }

    /**
     * @brief Write configuration onto file
     */
    bool write();

    /**
     * @brief Read configuration from file
     */
    bool read();

    bool setConfigFile(const QString fname);

protected:
    bool _error;
private:
    void _dumpElements();

    void _readFile();
    bool _readElement(QStringView name, QXmlStreamAttributes attrs);
    void _writeFile();

    QXmlStreamWriter _writer;
    QXmlStreamReader _reader;
    QFile _fd;

    bool _pending;
    bool _reading;

    QVector<xmlElementDef_t> _definitions;
    QVector<xmlElement_t> _elements;
    QStringList _context;
    QList<int> _parent;
};

#endif // CONFIGENGINE_H
