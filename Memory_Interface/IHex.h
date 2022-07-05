/*
 * IHex.h
 *
 *  Created on: 17 Jun 2022
 *      Author: prodr
 */

#ifndef IHEX_H_
#define IHEX_H_

#include <stdint.h>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>

#include <systemc.h>

namespace IHex {

constexpr unsigned int IHEX_MAX_DATA = 16;


class IHex {

	typedef enum ihex_recordtype_e {IHEX_DATA = 0, IHEX_EOF, IHEX_ESA, IHEX_SSA, IHEX_ELA, IHEX_SLA, IHEX_OTHER = -1} ihex_recordtype_t;

	typedef struct ihex_address_s {
		uint32_t base_address;
		uint16_t offset;
		union {
			uint16_t esa;
			struct {
				uint16_t cs;
				uint16_t ip;
			};
			uint16_t ela;
			uint32_t sla;
		} ;
		ihex_address_s(): base_address(0), offset(0), sla(0) {}
	} ihex_address_t;

	constexpr uint16_t composeTwoOctets(uint8_t msb, uint8_t lsb) {
		return (uint16_t)(((uint16_t)msb << 8) | ((uint16_t)lsb));
	}
	constexpr uint32_t composeFourOctets(uint8_t msb, uint8_t mid1, uint8_t mid2, uint8_t lsb) {
		return (uint32_t)((uint32_t)((uint32_t)msb  << 24) |
						  (uint32_t)((uint32_t)mid1 << 16) |
						  (uint32_t)((uint32_t)mid2 <<  8) |
						  (uint32_t)((uint32_t)lsb  <<  0));
	}
	constexpr uint32_t composeTwoHalfs(uint16_t msh, uint16_t lsh) {
		return (uint32_t)(((uint32_t)msh << 16) | ((uint32_t)lsh));
	}

	constexpr int isValidIHexLine(char *line) {
		char *ptr = line;

		if (ptr[0] != ':') {
			return 0;
		}
		ptr++;
		for (; *ptr != '\0'; ptr++) {
			if ((*ptr >= '0') && (*ptr <= '9')) {
				continue;
			}
			if ((*ptr >= 'a') && (*ptr <= 'f')) {
				continue;
			}
			if ((*ptr >= 'A') && (*ptr <= 'F')) {
				continue;
			}
			return 0;
		}
		return 1;
	}


public:

	IHex();
	IHex(char *line, uint32_t base = 0);
	bool decode(char *line, uint32_t base = 0);
	virtual ~IHex();
	uint32_t getAddress();
	uint32_t getBaseAddress();
	uint32_t getLen();
	uint8_t getByte(int n);
	void dump();
	bool isEOF() { return m_type == IHEX_EOF; }
	bool isDAT() { return m_type == IHEX_DATA; }
	bool isValid() { return m_type != IHEX_OTHER; }
private:
	bool _verifyChecksum(uint8_t chk);
	void _init();

	ihex_address_t m_address;
	ihex_recordtype_t  m_type;
	std::vector<uint8_t> m_data;
	uint8_t  m_checksum;
};






class IHexFile {
public:
	IHexFile();
	IHexFile(const char *fname);
	bool parse(const char *fname);
	virtual ~IHexFile();
	bool hasError() { return m_error; }
	void dump();
	std::vector<uint8_t> getBytes(uint32_t start, uint32_t nbytes);
	std::vector<uint32_t> getWords(uint32_t start, uint32_t nwords);
	int exportVHDL(const char *fname, const char *pkgname, const char *typname, const char *name, uint32_t start, uint32_t nwords);
	void exportSystemC(uint32_t start, uint32_t nwords, sc_lv<32> contents[]);
private:
	void _close();
	void _removeNewLine(char *line);
	std::vector<IHex> m_records;
	std::FILE* m_fd;
	std::string m_fname;
	bool m_open, m_error;
};

} // Namespace ends here

#endif /* IHEX_H_ */
