// Copyright (c) 2014-2015 Josh Blum
// SPDX-License-Identifier: BSL-1.0

#include <SoapySDR/Registry.hpp>
#include <iostream>

struct FunctionsEntry
{
    SoapySDR::InfoFunction info;
    SoapySDR::FindFunction find;
    SoapySDR::MakeFunction make;
};

typedef std::map<std::string, FunctionsEntry> FunctionTable;

static FunctionTable &getFunctionTable(void)
{
    static FunctionTable table;
    return table;
}

SoapySDR::Registry::Registry(const std::string &name, const InfoFunction &info, const FindFunction &find, const MakeFunction &make, const std::string &abi)
{
    if (abi != SOAPY_SDR_ABI_VERSION)
    {
        std::cerr << "SoapySDR::Registry(" << name << ") failed ABI check" << std::endl;
        std::cerr << "  Library ABI Version: " << SOAPY_SDR_ABI_VERSION << std::endl;
        std::cerr << "  Module ABI Version: " << abi << std::endl;
        std::cerr << "  Rebuild module against installed library..." << std::endl;
        return;
    }
    FunctionsEntry entry;
    entry.info = info;
    entry.find = find;
    entry.make = make;
    getFunctionTable()[name] = entry;
}

static SoapySDR::ArgInfoList infoNop(void)
{
    return SoapySDR::ArgInfoList();
}

SoapySDR::Registry::Registry(const std::string &name, const FindFunction &find, const MakeFunction &make, const std::string &abi)
{
    if (abi != SOAPY_SDR_ABI_VERSION)
    {
        std::cerr << "SoapySDR::Registry(" << name << ") failed ABI check" << std::endl;
        std::cerr << "  Library ABI Version: " << SOAPY_SDR_ABI_VERSION << std::endl;
        std::cerr << "  Module ABI Version: " << abi << std::endl;
        std::cerr << "  Rebuild module against installed library..." << std::endl;
        return;
    }
    FunctionsEntry entry;
    entry.info = &infoNop;
    entry.find = find;
    entry.make = make;
    getFunctionTable()[name] = entry;
}

SoapySDR::InfoFunctions SoapySDR::Registry::listInfoFunctions(void)
{
    InfoFunctions functions;
    for (FunctionTable::const_iterator it = getFunctionTable().begin(); it != getFunctionTable().end(); ++it)
    {
        functions[it->first] = it->second.info;
    }
    return functions;
}

SoapySDR::FindFunctions SoapySDR::Registry::listFindFunctions(void)
{
    FindFunctions functions;
    for (FunctionTable::const_iterator it = getFunctionTable().begin(); it != getFunctionTable().end(); ++it)
    {
        functions[it->first] = it->second.find;
    }
    return functions;
}

SoapySDR::MakeFunctions SoapySDR::Registry::listMakeFunctions(void)
{
    MakeFunctions functions;
    for (FunctionTable::const_iterator it = getFunctionTable().begin(); it != getFunctionTable().end(); ++it)
    {
        functions[it->first] = it->second.make;
    }
    return functions;
}