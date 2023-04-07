#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include <vector>

using namespace std;
using namespace rapidxml;


std::vector<xml_node<>*> cousins;
std::vector<string> cousinsNames;

void printTree(xml_node<>* node, int indent);

pair<int, xml_node<>*> findNodeWithAttribute(xml_node<>* node, const string& attributeName);


pair<xml_node<>*, xml_node<>*> getParentAndChild(xml_node<>* parent, xml_node<>* child, int level);


void getChildrenAtTheLevel(xml_node<>* parent, int level);


void getCousins(xml_node<>* node, int level);



int main() {
    // the XML string to parse
    file<> xmlFile("data.xml");
    xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    xml_node<>* root = doc.first_node();


    pair<int, xml_node<>*> result = findNodeWithAttribute(root, "kinship_degree");
    if (result.first >= 0) {
        cout << "Found node with kinship_degree=" << result.first << ": " << result.second->name() << endl;
    }
    else {
        cout << "Node with kinship_degree not found." << endl;
    }

    getCousins(result.second, result.first);

    for (xml_node<>* counsin : cousins) {
        cout << counsin->first_attribute()->value() << "\n";
    }

    return 0;
}


void getCousins(xml_node<>* node, int level) {
    pair<xml_node<>*, xml_node<>*> result = getParentAndChild(node, nullptr, level);
    for (xml_node<>* child = result.first->first_node(); child; child = child->next_sibling()) {
        if (result.second != child) {
            getChildrenAtTheLevel(child, level - 1);
        }

    }
}


void getChildrenAtTheLevel(xml_node<>* parent, int level) {
    if (level > 0) {
        for (xml_node<>* child = parent->first_node(); child; child = child->next_sibling()) {
            string name = child->first_attribute()->value();
            getChildrenAtTheLevel(child, level - 1);
        }

    }
    else {
        cousins.push_back(parent);
        cousinsNames.push_back(parent->first_attribute()->value());
    }
}


pair<xml_node<>*, xml_node<>*> getParentAndChild(xml_node<>* parent, xml_node<>* child, int level) {
    if (level > 0) {
        return getParentAndChild(parent->parent(), parent, level - 1);
    }
    return make_pair(parent, child);
}


pair<int, xml_node<>*> findNodeWithAttribute(xml_node<>* node, const string& attributeName) {
    // check if the current node has the desired attribute
    xml_attribute<>* attr = node->first_attribute(attributeName.c_str());
    if (attr) {
        int attrValue = stoi(attr->value());
        return make_pair(attrValue, node);
    }
    // recursively search through child nodes
    for (xml_node<>* childNode = node->first_node(); childNode; childNode = childNode->next_sibling()) {
        pair<int, xml_node<>*> result = findNodeWithAttribute(childNode, attributeName);
        if (result.first >= 0) {
            return result;
        }
    }
    // if no node with the attribute is found, return a null pointer and a value of -1
    return make_pair(-1, nullptr);
}


void printTree(xml_node<>* node, int indent) {
    // print the node name and attributes
    for (int i = 0; i < indent; i++) {
        cout << "  ";
    }
    cout << node->name();
    for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        cout << " " << attr->name() << "=" << attr->value();
    }
    cout << endl;

    // recursively print child nodes
    for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
        printTree(child, indent + 1);
    }
}