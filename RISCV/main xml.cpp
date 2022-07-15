#include <string.h>
#include <stdio.h>
#include <boost/program_options.hpp>
#include "xmlucrom.h"

namespace boostpo = boost::program_options;

class program_options {
public:
    program_options() : cmdlineOk(false), optHelp(false), optQuiet(false), optDump(false), optVerbose(false) {}
    program_options(int argc, char *argv[]) : cmdlineOk(false), optHelp(false), optQuiet(false), optDump(false), optVerbose(false) { process_cmdline(argc, argv); }
    void process_cmdline(int argc, char *argv[]) {
        std::string st, nw;
        boostpo::options_description generic("Generic options");
        generic.add_options()
            ("help",                                                                          "produce this help message")
            ("verbose",                                                                       "verbose output")
            ("dump,D",                                                                        "dump contents")
            ("quiet,Q",                                                                       "silent")
            ("config,C", boostpo::value<std::string>(&cfgFile)->default_value("rvucrom.cfg"), "set configuration file with program options")
            ;
        boostpo::options_description hidden("Hidden options");
        hidden.add_options()
            ("ifile,I", boostpo::value<std::string>(&inFile),                                "set input filename (XML), can be specified without --inFile")
            ;
        boostpo::options_description config("Configuration options (may appear in a config file)");
        config.add_options()
            ("ofile,O",  boostpo::value<std::string>(&outFile)->default_value("ucrom.vhd"),    "set output filename (VHDL)")
            ("start,S",  boostpo::value<std::string>(&st)     ->default_value("0"),           "set startAddress address, dec/hex")
            ("nwords,N", boostpo::value<std::string>(&nw)     ->default_value("256"),         "set length (in 32-bit words), dec/hex")
            ("pkg,P",    boostpo::value<std::string>(&pkgName)->default_value("packagename"), "set VHDL package name")
            ("arr,A",    boostpo::value<std::string>(&arrName)->default_value("arrayname"),   "set VHDL array name")
            ("typ,T",    boostpo::value<std::string>(&typName)->default_value("typname"),     "set VHDL array type name")
            ;
        boostpo::positional_options_description p;
        p.add("ifile", 1);
        try {
            boostpo::options_description cmdline_options;
            cmdline_options.add(generic).add(config).add(hidden);
            boostpo::options_description config_file_options;
            config_file_options.add(config).add(hidden);
            boostpo::variables_map vm;
            boostpo::store(boostpo::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
            boostpo::notify(vm);

            std::ifstream ifs(cfgFile.c_str());
            if (ifs)
            {
                boostpo::store(parse_config_file(cfgFile.c_str(), config_file_options), vm);
                boostpo::notify(vm);
            }

            if (vm.count("help")) {
                showUsage(argv[0], generic, config);
                optHelp = true;
                return;
            }
            if (vm.count("quiet")) {
                optQuiet = true;
                return;
            }
            cmdlineOk = true;
            if (vm.count("start")) {
                startAddress = parseUInt32AsDecOrHex("start", vm["start"].as<std::string>());
            }
            if (vm.count("nwords")) {
                nWords = parseUInt32AsDecOrHex("nwords", vm["nwords"].as<std::string>());
            }
        } catch(std::exception &e) {
            cout << e.what() << "\n\n";
            showUsage(argv[0], generic, config);
            return;
        }
    }
    bool askedForHelp() { return optHelp; }
    bool askedForQuiet() { return optQuiet; }
    bool askedForDump() { return optDump; }
    bool askedForVerbose() { return optVerbose; }
    bool isCmdlineOk() { return cmdlineOk; }
    uint32_t parseUInt32AsDecOrHex(const std::string &name, const std::string &s) {
        uint32_t result = 0;
        char hexdigits[30];

        if (1 == sscanf(s.c_str(), "0x%s", hexdigits)) {
            if (std::string::npos == s.find_first_not_of("0123456789abcdefABCDEF",2)) {
                if (1 == sscanf(s.c_str(), "0x%x", &result)) {
                    return result;
                }
            }
        } else {
            if (1 == sscanf(s.c_str(), "x%s", hexdigits)) {
                if (std::string::npos == s.find_first_not_of("0123456789abcdefABCDEF",1)) {
                    if (1 == sscanf(s.c_str(), "x%x", &result)) {
                        return result;
                    }
                }
            } else {
                if (std::string::npos == s.find_first_not_of("0123456789")) {
                    if (1 == sscanf(s.c_str(), "%d", &result)) {
                        return result;
                    }
                }
            }
        }
        cout << "Invalid value " << s << " for " << name << "\n";
        cmdlineOk = false;
        return 0;
    }
    void showUsage(const char *progname, boostpo::options_description &generic, boostpo::options_description &config) {
        boostpo::options_description visible("Allowed options");
        visible.add(generic).add(config);
        cout << "Usage:\n   " << progname << " input_file.hex [options]" << "\n";
        cout << visible << "\n";
    }
    std::string getCfgfile() { return (cmdlineOk? cfgFile: ""); }
    std::string getInfile() { return (cmdlineOk? inFile: ""); }
    std::string getOutfile() { return (cmdlineOk? outFile: ""); }
    std::string getPkgName() { return (cmdlineOk? pkgName: ""); }
    std::string getArrName() { return (cmdlineOk? arrName: ""); }
    std::string getTypName() { return (cmdlineOk? typName: ""); }
    uint32_t getStartAddress() { return (cmdlineOk? startAddress: 0); }
    uint32_t getNWords() { return (cmdlineOk? nWords: 0); }

protected:
    bool cmdlineOk, optHelp, optQuiet, optDump, optVerbose;
    std::string cfgFile, inFile, outFile, pkgName, arrName, typName;
    uint32_t startAddress, nWords;
};



int main(int argc, char *argv[]) {
    size_t i;
    int j;
    int result = 0;

    program_options po(argc, argv);
    if (po.askedForHelp()) {
        return 0;
    }
    if (!po.isCmdlineOk()) {
        cout << "\n Invalid option(s)\n";
        return -1;
    }

    if (!po.askedForQuiet()) {
        char buffer1[30], buffer2[30];
        snprintf(buffer1, sizeof(buffer1), "0x%08x (%u)", po.getStartAddress(), po.getStartAddress());
        snprintf(buffer2, sizeof(buffer2), "0x%08x (%u)", po.getNWords(), po.getNWords());
        cout << "\nRunning options:\n" <<
                "--config: " << po.getCfgfile() << "\n" <<
                "--ifile:  " << po.getInfile()  << "\n" <<
                "--ofile:  " << po.getOutfile() << "\n" <<
                "--start:  " << buffer1 << "\n" <<
                "--nwords: " << buffer2 << "\n" <<
                "--pkg:    " << po.getPkgName() << "\n" <<
                "--arr:    " << po.getArrName() << "\n" <<
                "--typ:    " << po.getTypName() << "\n";
    }

    xmlUCRom ucROM;

    ucROM.readXML(po.getInfile().c_str());
    if (!ucROM.hasError()) {
        if (po.askedForDump()) {
            ucROM.dumpContents();
        }
        if (po.askedForDump() && po.askedForVerbose()) {
            std::vector<uint8_t> bytes;
            std::vector<uint32_t> words;
            ucROM.getContents(bytes, po.getStartAddress(), po.getNWords());
            printf("ucROM.getContents(%08x, %u): %u bytes\n", po.getStartAddress(), po.getNWords(), (unsigned int)bytes.size());
            printf(" --- Memory dump (%08x, %u): ---\n", po.getStartAddress(), po.getNWords());
            for (i = 0; i < bytes.size(); ) {
                for (j=0; (j < 16) && (i < bytes.size()); j++, i++) {
                    printf(" %02x", bytes.at(i));
                }
                if (i < bytes.size()) {
                    printf(" - ");
                    for (; (j < 32) && (i < bytes.size()); j++, i++) {
                        printf(" %02x", bytes.at(i));
                    }
                }
                printf ("\n");
            }
            ucROM.getContents(words, po.getStartAddress(), po.getNWords());
            printf("ucROM.getContents(%08x, %u): %u words\n", po.getStartAddress(), po.getNWords(), (unsigned int)words.size());
            printf(" --- Memory dump (%08x, %u): ---\n", po.getStartAddress(), po.getNWords());
            for (i = 0; i < words.size(); ) {
                for (j=0; (j < 4) && (i < words.size()); j++, i++) {
                    printf(" %08x", words.at(i));
                }
                if (i < words.size()) {
                    printf(" - ");
                    for (; (j < 8) && (i < words.size()); j++, i++) {
                        printf(" %08x", words.at(i));
                    }
                }
                printf ("\n");
            }
        }
        if (!po.askedForQuiet()) {
            cout << "\nrvucrom.exportVHDL(" << po.getOutfile() << ", " << po.getPkgName() << ", " << po.getTypName() << ", " << po.getArrName() << ", " << po.getStartAddress() << ", " << po.getNWords() << ")\n";
        }
        if (ucROM.exportVHDL(po.getOutfile().c_str(), po.getPkgName().c_str(), po.getTypName().c_str(), po.getArrName().c_str(), po.getStartAddress(), po.getNWords()) < 0) {
            result = -1;
        }
    }
    if (ucROM.hasError()) {
        result = -1;
    }
    return result;
}
