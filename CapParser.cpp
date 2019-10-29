//author: Jackie Bowers
//Date: October 21, 2019
#include "CapParser.hpp"

using namespace rapidxml;
using namespace std;

CapParser::CapParser(char* filename){
	file xmlFile(filename);
	//file xmlFile("/Users/jackie/test.xml");
    doc.parse<0>(xmlFile.data());
}

void CapParser::getCapNodes() {
    xml_node<> *cap_node = doc.first_node();
    xml_attribute<>* alert_version;
    xml_node<> *first_node = doc.first_node();
    if (first_node->name() == "cap:alert") {
        alert_version = first_node->first_attribute();
    }
    // else invalid format, CAP version must be specified

    // For now, get all relevant CAP attributes in any order until we are certain of xml structure. 
    vector<vector<string> > cap_vector;
    capParse(first_node, cap_vector);

    for (auto &cap_iterator : cap_vector) {
        // add area and identifier.
        if (cap_iterator[0] == "sender") sender = cap_iterator[1];
        else if (cap_iterator[0] == "status") status = cap_iterator[1];
        else if (cap_iterator[0] == "type") type = cap_iterator[1];
        else if (cap_iterator[0] == "event") event = cap_iterator[1];
        else if (cap_iterator[0] == "severity") severity = cap_iterator[1];
        else if (cap_iterator[0] == "certainty") certainty = cap_iterator[1];
        else if (cap_iterator[0] == "headline") headline = cap_iterator[1];
        else if (cap_iterator[0] == "description") description = cap_iterator[1];
    }
}

void CapParser::capParse(xml_node<>* cap_node, vector<vector<string> > &all_node_vector) {
    vector<string> node_vector;
    if (cap_node->first_node()) capParse(cap_node->first_node(), all_node_vector);
    while(cap_node != nullptr) {
        cap_node = doc.next_sibling();
        node_vector.push_back(cap_node->name());
        node_vector.push_back(cap_node->value());
        // for loop structure from rapidxml.sourcefourge.net/manual.html
        for(xml_attribute<> *attr = cap_node->first_attribute(); attr; attr = attr->next_attribute()) {
            node_vector.push_back(attr->value());
        }
        all_node_vector.push_back(node_vector);
        if (cap_node->first_node()) capParse(cap_node->first_node(), all_node_vector);
    }
}
