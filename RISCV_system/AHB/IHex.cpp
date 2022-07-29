/*
 * IHex.cpp
 *
 *  Created on: 17 Jun 2022
 *      Author: prodr
 */

#include "IHex.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace IHex {

const char *ihex_recordnames[] = {"DAT", "EOF", "ESA", "SSA", "ELA", "SLA"};

IHex::IHex() : m_type(IHEX_OTHER), m_data(IHEX_MAX_DATA), m_checksum(0) {
}

IHex::IHex(char *line, uint32_t base) : m_type(IHEX_OTHER), m_data(IHEX_MAX_DATA), m_checksum(0) {
	decode(line, base);
}

void IHex::_init() {
	m_data.clear();
	m_type = IHEX_OTHER;
	m_checksum = 0;
	m_address.base_address = 0;
	m_address.offset = 0;
	m_address.sla = 0;
}

bool IHex::decode(char *line, uint32_t base) {
	int i, bc, addh, addl, typ, dat;
	uint8_t chk = 0;

	_init();
	if (!isValidIHexLine(line)) {
		return false;
	}
	line++;
	if (4 != sscanf(line, "%02x%02x%02x%02x", &bc, &addh, &addl, &typ)) {
		printf("decode() ERROR: Unabe to retrieve bytecount, offset, or record type (first 8 hex digits)\n");
		return false;
	}
	chk = bc + addh + addl + typ;
	m_address.offset = composeTwoOctets(addh, addl);
	for (i=0; i < bc; i++) {
		if (1 != sscanf(&line[8+2*i], "%02x", &dat)) {
			printf("decode() ERROR: Unable to retrieve data byte %d\n", i);
			return false;
		}
		m_data.push_back(static_cast<uint8_t>(dat));
	}
	if (1 != sscanf(&line[8+2*i], "%02x", &dat)) {
		printf("decode() ERROR: Unable to retrieve checksum\n");
		return false;
	}
	m_checksum = dat;
	chk += dat;
	if (!_verifyChecksum(chk)) {
		printf("decode() ERROR: Invalid checksum\n");
		return false;
	}
	m_type = ((typ >= static_cast<int>(IHEX_DATA)) && (typ <= static_cast<int>(IHEX_SLA)) ? static_cast<ihex_recordtype_t>(typ) : IHEX_OTHER);
	switch(m_type) {
		case IHEX_DATA:
			m_address.base_address = base;
			break;
		case IHEX_EOF:
			m_address.base_address = 0;
			break;
		case IHEX_ESA:
			m_address.esa = composeTwoOctets(m_data.at(0), m_data.at(1));
			m_address.base_address = (uint32_t)m_address.esa;
			break;
		case IHEX_SSA:
			m_address.cs = composeTwoOctets(m_data.at(0), m_data.at(1));
			m_address.ip = composeTwoOctets(m_data.at(2), m_data.at(3));
			m_address.base_address = composeTwoHalfs(m_address.cs, m_address.ip);
			break;
		case IHEX_ELA:
			m_address.ela = composeTwoOctets(m_data.at(0), m_data.at(1));
			m_address.base_address = (uint32_t)m_address.ela;
			break;
		case IHEX_SLA:
			m_address.cs = composeFourOctets(m_data.at(0), m_data.at(1), m_data.at(2), m_data.at(3));
			m_address.base_address = m_address.cs;
			break;
		default:
			printf("decode() ERROR: Unknown record type\n");
			m_address.base_address = 0;
			return false;
	}
	return true;
}

uint32_t IHex::getAddress() {
	uint32_t a = m_address.base_address;
	switch(m_type) {
		case IHEX_DATA:
			a += m_address.offset;
			break;
		case IHEX_EOF:
		case IHEX_ESA:
		case IHEX_SSA:
		case IHEX_ELA:
		case IHEX_SLA:
			break;
		default:
			printf("getAddress() ERROR: Unknown record type\n");
			a = 0;
	}
	return a;
}

uint32_t IHex::getBaseAddress() {
	uint32_t a = m_address.base_address;
	switch(m_type) {
	case IHEX_DATA:
	case IHEX_EOF:
	case IHEX_ESA:
	case IHEX_SSA:
	case IHEX_ELA:
	case IHEX_SLA:
		break;
		default:
			printf("getBaseAddress() ERROR: Unknown record type\n");
			a = 0;
	}
	return a;
}


bool IHex::_verifyChecksum(uint8_t chk) {
	for (size_t i = 0; i < m_data.size(); i++) {
		chk += m_data.at(i);
	}
	return (0 == chk? true : false);
}

void IHex::dump() {
	char data[1024];

	switch(m_type) {
		case IHEX_DATA:
			memset((void *)data, 0, sizeof(data));
			for (size_t i = 0; i < m_data.size(); i++) {
				if (i != 0) {
					data[3*i-1] = ' ';
				}
				snprintf(&data[3*i], 3, "%02x ", m_data.at(i));
			}
			printf(" %5s %08x %s\n", ihex_recordnames[static_cast<int>(IHEX_DATA)], m_address.base_address + m_address.offset, data);
			break;
		case IHEX_EOF:
			printf(" %5s %08x\n", ihex_recordnames[static_cast<int>(IHEX_EOF)], m_address.offset);
			break;
		case IHEX_ESA:
			snprintf(data, sizeof(data), "%04x (Extended segment address)", m_address.esa);
			printf(" %5s %08x %s\n", ihex_recordnames[static_cast<int>(IHEX_ESA)], m_address.offset, data);
			break;
		case IHEX_SSA:
			snprintf(data, sizeof(data), "%04x:%04x (Start segment address CS:IP)", m_address.cs, m_address.ip);
			printf(" %5s %08x %s\n", ihex_recordnames[static_cast<int>(IHEX_SSA)], m_address.offset, data);
			break;
		case IHEX_ELA:
			snprintf(data, sizeof(data), "%04x (Extended linear address)", m_address.ela);
			printf(" %5s %08x %s\n", ihex_recordnames[static_cast<int>(IHEX_ELA)], m_address.offset, data);
			break;
		case IHEX_SLA:
			snprintf(data, sizeof(data), "%04x (Start linear address)", m_address.esa);
			printf(" %5s %08x %s\n", ihex_recordnames[static_cast<int>(IHEX_SLA)], m_address.offset, data);
			break;
		default:
			printf(" Unknown record type\n");
	}
}

uint32_t IHex::getLen() {
	return m_data.size();
}

uint8_t IHex::getByte(int n) {
	if ( (n < 0) || (static_cast<size_t>(n) >= m_data.size()) ) {
		return 0;
	}
	return m_data.at(n);
}


IHex::~IHex() {
}








IHexFile::IHexFile() : m_fd(nullptr), m_fname(""), m_open(false), m_error(false) {
}

IHexFile::IHexFile(const char *fname) : m_fd(nullptr), m_fname(fname), m_open(false), m_error(false) {
	m_error = !parse(fname);
}

bool IHexFile::parse(const char *fname) {
	char buffer[1024];
	unsigned int n = 0;
	uint32_t base = 0;
	IHex record;
	bool eof_record = false;

	_close();
	m_fname = std::string(fname);
    m_fd = std::fopen(fname, "r");
    if(nullptr == m_fd) {
        std::perror("ihex file opening failed");
        return false;
    }
    m_open = true;
    m_records.clear();
    while (nullptr != std::fgets(buffer, sizeof(buffer), m_fd)) {
		n++;
		_removeNewLine(buffer);
		record.decode(buffer, base);
		if (!record.isValid()) {
			snprintf(buffer, sizeof(buffer), "Invalid record, line %d", n);
			std::perror(buffer);
			continue;
		}
		if (eof_record && record.isValid()) {
			snprintf(buffer, sizeof(buffer), "Record after EOF record, line %d", n);
			std::perror(buffer);
			continue;
		}
		base = record.getBaseAddress();
		eof_record = record.isEOF();
		m_records.push_back(record);
    }
    _close();
	return true;
}

void IHexFile::_close() {
	if (m_open) {
		fclose(m_fd);
		m_fd = nullptr;
		m_open = false;
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

int IHexFile::exportVHDL(const char *fname, const char *pkgname, const char *typname, const char *name, uint32_t start, uint32_t nwords) {
	size_t i;
	auto words = getWords(start, nwords);
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

void IHexFile::exportSystemC(uint32_t start, uint32_t nwords, sc_lv<32> contents[]) {
	size_t i;
	auto words = getWords(start, nwords);
	for (i = 0; i < words.size(); i++) {
		contents[i] = words.at(i);
	}
}

void IHexFile::_removeNewLine(char *line) {
	char *newline = nullptr;

	if (nullptr != (newline = std::strchr(line, '\n'))) {
		*newline = '\0';
	}
	if (nullptr != (newline = std::strchr(line, '\r'))) {
		*newline = '\0';
	}
}

IHexFile::~IHexFile() {
	_close();
}

void IHexFile::dump() {
	if (m_records.size() == 0) {
		perror("Nothing to dump");
		return;
	}
	printf("%5s %8s %s\n", "Type", "Address", "Data");
	printf("%5s %8s %16s\n", "====", "=======", "================");
	for (auto i = m_records.begin(); i != m_records.end(); i++) {
		i->dump();
	}
	printf("%5s %8s %16s\n", "====", "=======", "================");
}

std::vector<uint8_t> IHexFile::getBytes(uint32_t start, uint32_t nbytes) {
	std::vector<uint8_t> res(nbytes, 0);
	uint32_t addr, end = start + nbytes;
	size_t i;

	for (auto e = m_records.begin(); e != m_records.end(); e++) {
		addr = e->getAddress();
		if (e->isDAT() && (addr >= start) && (addr < end)) {
			for (i=0; (i < e->getLen()) && (addr < end); i++, addr++) {
				res[addr-start] = e->getByte(i);
			}
		}
	}
	return res;
}

std::vector<uint32_t> IHexFile::getWords(uint32_t start, uint32_t nwords) {
	size_t i;
	int j;
	uint32_t word, nword;
	auto bytes = getBytes(start, 4*nwords);
	std::vector<uint32_t> result(nwords, 0);

	for (nword = 0, i = 0; i < bytes.size(); nword++) {
		word = 0x00000000;
		for (j=0; (j < 4) && (i < bytes.size()); j++, i++) {
			word |= (uint32_t)(((uint32_t)bytes.at(i)) << (8*j));
		}
		result[nword] = word;
	}
	return result;
}


} // Namespace ends here

