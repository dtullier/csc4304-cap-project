//author: Jackie Bowers
//Date: October 21, 2019

#ifndef CAPPARSER_HPP
#define CAPPARSER_HPP


#include <iostream>
#include <string>
#include <vector>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace rapidxml;
using namespace std;

class CapParser
{
    private:
        string sender, status, type, event, severity, certainty, headline, description, identifier, area;
        xml_document<> doc;
        void capParse(xml_node<>* cap_node, vector<vector<string> > &node_vector);
    public:
        CapParser(char* filename);
        void getCapNodes();
};
#endif
