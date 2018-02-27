//
// Created by herbertqiao on 2/26/18.
//

#ifndef ROOTSCRIPT_DEBUG_INFO_H
#define ROOTSCRIPT_DEBUG_INFO_H

#include "TTree.h"
#include "TFile.h"
#include "map"

class DebugTimeInfo
{
public:
    DebugTimeInfo(std::string &path);

    ~DebugTimeInfo()
    {
        if (tree != nullptr) {
            tree->Write();
        }
        if (file != nullptr) {
            file->Close();
        }
    };

    bool Clean();

    bool AddKeys(std::string key);

    bool AddValues(std::string key, double value);

    bool Save();

    TTree *tree;

private:
    std::map<std::string, double> tmp;

    TFile *file;
};


#endif //ROOTSCRIPT_DEBUG_INFO_H
