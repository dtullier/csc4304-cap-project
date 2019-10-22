//author: Jackie Bowers
//Date: October 21, 2019

#ifndef CAPPARSER_HPP
#define CAPPARSER_HPP


#include <iostream>
#include <string>
#include <vector>
#include "rapidxml-1.13/rapidxml.hpp"

class CapParser
{
    private:
        string sender, status, type, event, severity, certainty, headline, description
        xml_document<> doc;
        capParse(xml_node<>* cap_node, vector<vector<string>> &node_vector)
    public:
        CapParser(string filename);
        getCapNodes();
}
#endif
