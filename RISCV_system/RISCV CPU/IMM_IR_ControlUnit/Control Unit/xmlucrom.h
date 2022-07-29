#ifndef XMLUCROM_H
#define XMLUCROM_H

#include <systemc.h>

#include "configengine.h"

class xmlUCRom : public ConfigEngine
{
public:
    xmlUCRom();
    bool readXML(const QString fname);
    void dumpContents();
    void getNames(std::vector<std::string> &names, uint32_t start = 0, int32_t nwords = -1);
    void getContents(std::vector<std::string> &contents, uint32_t start = 0, int32_t nwords = -1);
    void getContents(std::vector<uint8_t> &contents, uint32_t start = 0, int32_t nwords = -1);
    void getContents(std::vector<uint32_t> &contents, uint32_t start = 0, int32_t nwords = -1);
    void exportSystemC(sc_lv<32> contents[], uint32_t start = 0, int32_t nwords = -1);
    int exportVHDL(const char *fname, const char *pkgname, const char *typname, const char *name, uint32_t start = 0, int32_t nwords = -1);

private:
    bool _getSignalsValue(QString expression, uint64_t &value);
    void _setDefaultContents(uint64_t defentry);
    bool _setBlockContents(int bsize, int block, uint64_t defentry);

    std::vector<uint64_t> m_ROMContents;
    std::vector<std::string> m_entryNames;
};

#endif // XMLUCROM_H
