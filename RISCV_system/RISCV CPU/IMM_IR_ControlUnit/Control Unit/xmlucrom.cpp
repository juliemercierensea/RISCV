#include <iostream>
#include "xmlucrom.h"

xmlUCRom::xmlUCRom()
    : ConfigEngine()
{
    addElementDefinition("configuration");
    addElementDefinition("configuration:fields");
    addElementDefinition("configuration:fields:field", QString("name,width,pos").split(','), QString(",1,0").split(','));
    addElementDefinition("configuration:fields:field:decode", QString("name,value").split(','));
    addElementDefinition("configuration:signals");
    addElementDefinition("configuration:signals:signal", QString("name,width,pos").split(','), QString(",1,0,0").split(','));
    addElementDefinition("configuration:ucontrol", QString("depth,width,blocksize,default").split(','), QString("256,32,4").split(','));
    addElementDefinition("configuration:ucontrol:block", QString("name,pos,decode").split(','));
    addElementDefinition("configuration:ucontrol:block:entry", QString("name,signals").split(','));
}


bool xmlUCRom::readXML(const QString fname)
{
    int uc;
    int ucDepth, ucWidth, ucBsize;
    QString ucDefault;
    QList<int> blocks;
    uint64_t default_value;

    if (!setConfigFile(fname)) {
        _error = true;
        return false;
    }
    if (!read()) {
        std::cout << "ERROR at readConfigData() :: malformed XML" << std::endl;
        _error = true;
        return false;
    }
    uc = findElement("configuration:ucontrol");
    if (uc < 0) {
        std::cout << "ERROR, XML doesn't contain a <ucontrol> block!" << std::endl;
        _error = true;
        return false;
    }
    if (!getAttribute(uc, "depth", ucDepth)) {
        std::cout << "ERROR, XML internal error (ucontrol, depth)!" << std::endl;
        _error = true;
        return false;
    }
    if (!getAttribute(uc, "width", ucWidth)) {
        std::cout << "ERROR, XML internal error (ucontrol, width)!" << std::endl;
        _error = true;
        return false;
    }
    if (!getAttribute(uc, "blocksize", ucBsize)) {
        std::cout << "ERROR, XML internal error (ucontrol, blocksize)!" << std::endl;
        _error = true;
        return false;
    }
    if (!getAttribute(uc, "default", ucDefault)) {
        std::cout << "ERROR, XML internal error (ucontrol, default)!" << std::endl;
        _error = true;
        return false;
    }
    if (!_getSignalsValue(ucDefault, default_value)) {
        std::cout << "ERROR Can't get ROM default value!" << std::endl;
        _error = true;
        return false;
    }
    m_entryNames.clear();
    m_entryNames.resize(ucDepth);
    m_ROMContents.clear();
    m_ROMContents.resize(ucDepth);
    _setDefaultContents(default_value);
    if (getChildren(uc, blocks)) {
        for (int b = 0; b < blocks.size(); b++) {
            if (!_setBlockContents(ucBsize, blocks.at(b), default_value)) {
                _error = true;
                return false;
            }
        }
    }
    return true;
}

void xmlUCRom::dumpContents()
{
    std::size_t e;
    char buffer[100];

    for (e = 0; e < m_ROMContents.size(); e++) {
        snprintf(buffer, sizeof (buffer), "%30s: [0x%04Lx] 0x%016Lx", m_entryNames.at(e).c_str(), e, m_ROMContents.at(e));
        std::cout << std::string(buffer) << std::endl;
    }
}

void xmlUCRom::getNames(std::vector<std::string> &names, uint32_t start, int32_t nwords)
{
    std::size_t pos, last, i;

    names.clear();
    names.resize(m_entryNames.size());
    if (start >= m_entryNames.size()) {
        return;
    }
    if (nwords < 0) {
        last = start + m_entryNames.size();
    } else {
        last = start + nwords;
    }
    for (i=0, pos=start; (pos < m_entryNames.size()) && (pos < last); i++, pos++) {
        names[i] = m_entryNames.at(pos);
    }
}

void xmlUCRom::getContents(std::vector<std::string> &contents, uint32_t start, int32_t nwords)
{
    std::size_t pos, last, i;
    char buffer[30];

    contents.clear();
    contents.resize(m_ROMContents.size());
    if (start >= m_ROMContents.size()) {
        return;
    }
    if (nwords < 0) {
        last = start + m_ROMContents.size();
    } else {
        last = start + nwords;
    }
    for (i=0, pos=start; (pos < m_ROMContents.size()) && (pos < last); i++, pos++) {
        snprintf(buffer, sizeof(buffer), "%016Lx", m_ROMContents.at(pos));
        contents[i] = std::string(buffer);
    }
}

void xmlUCRom::getContents(std::vector<uint8_t> &contents, uint32_t start, int32_t nwords)
{
    std::size_t pos, last, i;
    uint32_t word;

    contents.clear();
    contents.resize(4*m_ROMContents.size());
    if (start >= 4*m_ROMContents.size()) {
        return;
    }
    if (nwords < 0) {
        last = 4*start + 4*m_ROMContents.size();
    } else {
        last = 4*start + 4*nwords;
    }
    for (i=0, pos=start; (pos < m_ROMContents.size()) && (pos < last); i++, pos++) {
        word = m_ROMContents.at(pos/4);
        switch(pos%4) {
        case 0:
            word &= 0x000000FF;
            break;
        case 1:
            word &= 0x0000FF00;
            word = word >> 8;
            break;
        case 2:
            word &= 0x00FF0000;
            word = word >> 16;
            break;
        default:
            word &= 0xFF000000;
            word = word >> 24;
        }
        contents[i] = static_cast<uint8_t>(word);
    }
}

void xmlUCRom::getContents(std::vector<uint32_t> &contents, uint32_t start, int32_t nwords)
{
    std::size_t pos, last, i;

    contents.clear();
    contents.resize(m_ROMContents.size());
    if (start >= m_ROMContents.size()) {
        return;
    }
    if (nwords < 0) {
        last = start + m_ROMContents.size();
    } else {
        last = start + nwords;
    }
    for (i=0, pos=start; (pos < m_ROMContents.size()) && (pos < last); i++, pos++) {
        contents[i] = static_cast<uint32_t>(m_ROMContents.at(pos));
    }
}

void xmlUCRom::exportSystemC(sc_lv<32> contents[], uint32_t start, int32_t nwords)
{
    std::vector<uint32_t> words;
    std::size_t i;

    getContents(words, start, nwords);
    for (i = 0; i < words.size(); i++) {
        contents[i] = words.at(i);
    }
}

static const char *vhdlText[] = {
        "-- Auto-generated memory initialization file from ihex file\n",
        "package body %s is\n",
        "\tconstant %s : %s := (\n",
        "\t\tx\"%08x\",\n",
        "\t\tx\"%08x\");\n",
        "end package %s;\n",
        "-- File end\n",
};

#define VHDL_PROLOGUE_BEGIN		0
#define VHDL_PROLOGUE_END		0
#define VHDL_PACKAGE_IS			1
#define VHDL_CONSTANT			2
#define VHDL_VALUE_ANYBUTLAST	3
#define VHDL_VALUE_LAST			4
#define VHDL_END_PACKAGE		5
#define VHDL_EPILOGUE_BEGIN		6
#define VHDL_EPILOGUE_END		6

int xmlUCRom::exportVHDL(const char *fname, const char *pkgname, const char *typname, const char *name, uint32_t start, int32_t nwords) {
    size_t i;
    std::vector<uint32_t> words;

    getContents(words, start, nwords);
    std::FILE* fd;
    char textline[512];

    fd = std::fopen(fname, "w");
    if(nullptr == fd) {
        std::perror("exportVHDL() ERROR: Open file failed");
        return -1;
    }

    for (i=VHDL_PROLOGUE_BEGIN; i <= VHDL_PROLOGUE_END; i++) {
        if (std::fputs(vhdlText[i], fd) < 0) {
            std::perror("exportVHDL() ERROR: Write file failed");
            return -1;
        }
    }
    snprintf(textline, sizeof(textline), vhdlText[VHDL_PACKAGE_IS], pkgname);
    if (std::fputs(textline, fd) < 0) {
        std::perror("exportVHDL() ERROR: Write file failed");
        return -1;
    }
    snprintf(textline, sizeof(textline), vhdlText[VHDL_CONSTANT], name, typname);
    if (std::fputs(textline, fd) < 0) {
        std::perror("exportVHDL() ERROR: Write file failed");
        return -1;
    }

    for (i = 0; i < words.size(); i++) {
        if ((i+1) < words.size()) {
            snprintf(textline, sizeof(textline), vhdlText[VHDL_VALUE_ANYBUTLAST], words.at(i));
        } else {
            snprintf(textline, sizeof(textline), vhdlText[VHDL_VALUE_LAST], words.at(i));
        }
        if (std::fputs(textline, fd) < 0) {
            std::perror("exportVHDL() ERROR: Write file failed");
            return -1;
        }
    }

    snprintf(textline, sizeof(textline), vhdlText[VHDL_END_PACKAGE], pkgname);
    if (std::fputs(textline, fd) < 0) {
        std::perror("exportVHDL() ERROR: Write file failed");
        return -1;
    }
    for (i=VHDL_EPILOGUE_BEGIN; i <= VHDL_EPILOGUE_END; i++) {
        if (std::fputs(vhdlText[i], fd) < 0) {
            std::perror("exportVHDL() ERROR: Write file failed");
            return -1;
        }
    }

    fclose(fd);
    return words.size();
}

bool xmlUCRom::_getSignalsValue(QString expression, uint64_t &value)
{
    QStringList signal_list;
    QStringList value_list;
    int signal_elem, signal_width, signal_pos;
    qulonglong signal_value, signal_mask;

    signal_list = expression.split(',');
    value = 0;
    for (auto s = signal_list.cbegin(); s != signal_list.cend(); s++) {
        value_list = s->trimmed().split('=');
        if (value_list.size() > 2) {
            std::cout << "ERROR, XML internal error (signal expression)!" << std::endl;
            _error = true;
            return false;
        }
        signal_elem = findElement("configuration:signals:signal", "name", value_list.at(0).trimmed());
        if (signal_elem < 0) {
            std::cout << "ERROR, XML internal error (signal expression, signal " << value_list.at(0).trimmed().toLatin1().toStdString() << " not found)!" << std::endl;
            _error = true;
            return false;
        }
        if (!getAttribute(signal_elem, "width", signal_width)) {
            std::cout << "ERROR, XML internal error (signal expression, signal" << value_list.at(0).trimmed().toLatin1().toStdString() << ", width)!" << std::endl;
            _error = true;
            return false;
        }
        if (!getAttribute(signal_elem, "pos", signal_pos)) {
            std::cout << "ERROR, XML internal error (signal expression, signal" << value_list.at(0).trimmed().toLatin1().toStdString() << ", pos)!" << std::endl;
            _error = true;
            return false;
        }
        if (value_list.size() > 1) {
            if (!parseAttributeUnsigned(value_list.at(1).trimmed(), signal_value)) {
                std::cout << "ERROR, XML internal error (entry, signal value)!" << std::endl;
                _error = true;
                return false;
            }
        } else {
            if (signal_width == 1) {
                signal_value = 1;
            } else {
                signal_value = 0;
            }
        }
        signal_mask = (1 << signal_width) - 1;
        signal_value = (signal_value & signal_mask) << signal_pos;
        signal_mask = signal_mask << signal_pos;
        value &= ~signal_mask;
        value |= signal_value;
    }
    return true;
}

void xmlUCRom::_setDefaultContents(uint64_t defentry)
{
    std::size_t i;
    char buffer[30];

    snprintf(buffer, sizeof(buffer), "0x%016Lx", defentry);
    //std::cout << "DEBUG default entry = " << std::string(buffer) << std::endl;
    for (i = 0; i < m_ROMContents.size(); i++) {
        snprintf(buffer, sizeof(buffer), "(0x%06Lx)", i);
        m_entryNames[i] = std::string(buffer);
        m_ROMContents[i] = defentry;
    }
}

bool xmlUCRom::_setBlockContents(int bsize, int block, uint64_t defentry)
{
    int pos;
    QList<int> entry_list;
    QString entry_name;
    QString signal_list;
    uint64_t entry_value;
    char buffer[30];
    QString block_name;
    qsizetype entry = 0;

    if (!getAttribute(block, "pos", pos)) {
        std::cout << "ERROR, XML internal error (block, pos)!" << std::endl;
        _error = true;
        return false;
    }
    pos = pos * bsize;
    if (!getAttribute(block, "name", block_name)) {
        std::cout << "ERROR, XML internal error (block, name)!" << std::endl;
        _error = true;
        return false;
    }
    if (block_name.isEmpty()) {
        snprintf(buffer, sizeof(buffer), "(0x%04x)", pos);
        block_name = QString(buffer);
    }
    entry = 0;
    if (getChildren(block, entry_list)) {
        for (; entry < entry_list.size(); entry++, pos++) {
            if (!getAttribute(entry_list.at(entry), "name", entry_name)) {
                std::cout << "ERROR, XML internal error (entry, name)!" << std::endl;
                _error = true;
                return false;
            }
            if (entry_name.isEmpty()) {
                entry_name = block_name + "," + QString::number(entry);
            }
            if (!getAttribute(entry_list.at(entry), "signals", signal_list)) {
                std::cout << "ERROR, XML internal error (entry, signals)!" << std::endl;
                _error = true;
                return false;
            }
            if (signal_list == "") {
                entry_value = defentry;
                //snprintf(buffer, sizeof(buffer), "0x%016Lx", entry_value);
                //std::cout << "DEBUG entry " << entry_name.toStdString() << ", signals '' results in " << std::string(buffer) << std::endl;
            } else {
                if (!_getSignalsValue(signal_list, entry_value)) {
                    std::cout << "ERROR, XML internal error (entry, signal expression)!" << std::endl;
                    _error = true;
                    return false;
                }
                //snprintf(buffer, sizeof(buffer), "0x%016Lx", entry_value);
                //std::cout << "DEBUG entry " << entry_name.toStdString() << ", signals '" << signal_list.toStdString() << "' results in " << std::string(buffer) << std::endl;
            }
            m_entryNames[pos] = entry_name.toStdString();
            m_ROMContents[pos] = entry_value;
        }
    }
    for (; entry < bsize; entry++, pos++) {
        entry_name = block_name + "," + QString::number(entry);
        m_entryNames[pos] = entry_name.toStdString();
    }
    return true;
}
