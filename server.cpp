/*
	Server Side
	Pass port command line arg
*/

//Written by Devin Tullier 
//Adapted from https://www.geeksforgeeks.org/socket-programming-cc/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <ctime>
#include "CapParser.hpp"

using namespace rapidxml;
using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[512];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *)&cli_addr,
                       &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");

    FILE *fp;
    FILE *fp2;
    int ch = 0;
    fp = fopen("alert_log.txt", "a");
    time_t now = time(0);
    fprintf(fp, "Log time: %s \n", ctime(&now));
    fp2 = fopen("alert.xml", "w");
    int words;
    read(newsockfd, &words, sizeof(int));

    while (ch != words)
    {
        read(newsockfd, buffer, 512);
        fprintf(fp, " %s", buffer);
        fprintf(fp2, " %s", buffer);
        ch++;
    }
    fprintf(fp, "\n\n");
    printf("The file was received successfully\n");
    printf("The data was stored in our logs \n");
    //CapParser* cap = new CapParser("demo.xml");
    //cap->getCapNodes();
    close(newsockfd);
    close(sockfd);
    return 0;
}

//The following is written by Jackie
//Uses RapidXml

CapParser::CapParser(char *filename)
{
    ifstream theFile(filename);
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
}

void CapParser::getCapNodes()
{
    xml_node<> *cap_node = doc.first_node();
    /*string alert_version;
    if (strncmp(cap_node->name(), "cap:alert", 100) == 0) {
        alert_version = cap_node->first_attribute()->value();
        printf("Message is in proper format\n");
    }
    else {
	printf("Message is not in proper format\n");
    }
    // else invalid format, CAP version must be specified

    //this->identifier = cap_node->first_node("cap:identifier")->value();
    //this->sender = cap_node->first_node("cap:sender")->value();
    //this->status = cap_node->first_node("cap:status")->value();
    //this->type = cap_node->first_node("cap:msgType")->value();

    //xml_node<> *info_node = cap_node->first_node("cap:info");

    //this->event = info_node->first_node("cap:event")->value();
    //this->severity = info_node->first_node("cap:severity")->value();
    //this->certainty = info_node->first_node("cap:certainty")->value();
    //this->description = info_node->first_node("cap:description")->value();
    //this->area = info_node->first_node("cap:area")->value();


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
    }*/
}

void CapParser::capParse(xml_node<> *cap_node, vector<vector<string>> &all_node_vector)
{
    vector<string> node_vector;
    if (cap_node->first_node())
        capParse(cap_node->first_node(), all_node_vector);
    while (cap_node != nullptr)
    {
        cap_node = doc.next_sibling();
        node_vector.push_back(cap_node->name());
        node_vector.push_back(cap_node->value());
        // for loop structure from rapidxml.sourcefourge.net/manual.html
        for (xml_attribute<> *attr = cap_node->first_attribute(); attr; attr = attr->next_attribute())
        {
            node_vector.push_back(attr->value());
        }
        all_node_vector.push_back(node_vector);
        if (cap_node->first_node())
            capParse(cap_node->first_node(), all_node_vector);
    }
}