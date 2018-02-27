//
// Created by herbertqiao on 2/26/18.
//

#include "DebugTimeInfo.h"

DebugTimeInfo::DebugTimeInfo(std::string &path) :
        tree(nullptr),
        file(nullptr)
{
    file = new TFile(path.c_str(), "RECREATE");
    tree = new TTree("DebugInfo", "DebugInfo");
}

bool DebugTimeInfo::AddKeys(std::string key)
{
    if (tmp.find(key) == tmp.end()) {
        tmp.insert(std::make_pair(key, double(0)));
        tree->Branch(key.c_str(), &tmp.at(key));
    }
    return true;
}

bool DebugTimeInfo::AddValues(std::string key, double value)
{
    tmp[key] = value;
    return true;
}

bool DebugTimeInfo::Clean()
{
    for (auto &v: tmp) {
        tmp.at(v.first) = 0;
    }
    return true;
}

bool DebugTimeInfo::Save()
{
    tree->Fill();
}