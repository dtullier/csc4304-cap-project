//author: Jackie Bowers
//Date: October 21, 2019
#include "CapParser.hpp"

using namespace rapidxml;
using namespace std;

CapParser::CapParser(char* filename){
	cout << "parse xml";
	ifstream theFile (filename);
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);
}

void CapParser::getCapNodes() {
    xml_node<> *cap_node = doc.first_node();
    string alert_version;
    if (strncmp(cap_node->name(), "cap:alert", 100) == 0) {
        alert_version = cap_node->first_attribute()->value();
        cout << alert_version;
    }
    // else invalid format, CAP version must be specified

    this->identifier = cap_node->first_node("cap:identifier")->value();
    this->sender = cap_node->first_node("cap:sender")->value();
    this->status = cap_node->first_node("cap:status")->value();
    this->type = cap_node->first_node("cap:msgType")->value();

    xml_node<> *info_node = cap_node->first_node("cap:info");

    this->event = info_node->first_node("cap:event")->value();
    this->severity = info_node->first_node("cap:severity")->value();
    this->certainty = info_node->first_node("cap:certainty")->value();
    this->description = info_node->first_node("cap:description")->value();
    this->area = info_node->first_node("cap:area")->value();


    /*vector<vector<string> > cap_vector;
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
    }*/
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
