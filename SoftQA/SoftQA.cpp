#include "SoftQA.h"
#include <iostream>

using namespace std;
using namespace rapidxml;

#ifdef _MSC_VER
int main() {
    try {

        getCousins((char*)"data1.xml", (char*)"output.txt");

    }
    catch (FileNotFoundException ex) {
        cout << ex.what();
        return 0;
    }
    catch (KinshipDegreeException ex) {
        cout << ex.what();
        return 0;
    }
    catch (runtime_error ex) {
        cout << ex.what();
    }

    return 0;
}

#else
int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            throw runtime_error("Incorrect arguments");
        }

        getCousins(argv[1], argv[2]);

    }
    catch (FileNotFoundException ex) {
        cout << ex.what();
        return 0;
    }
    catch (KinshipDegreeException ex) {
        cout << ex.what();
        return 0;
    }
    catch (runtime_error ex) {
        cout << ex.what();
    }

    return 0;
}
#endif


void getCousins(char* inputFile, char* outputFile) {

    xml_document<> doc;
    string xml = readXmlFile(inputFile);
    doc.parse<0>((char*)xml.c_str());
    xml_node<>* root = doc.first_node();


    xml_node<>* node = findNodeWithAttribute(root, "kinship_degree");

    int kinshipDegree = validateNodeAttribute(node);

    auto[parent, bannedChild] = getParentAndChildByGeneration(node, nullptr, kinshipDegree);

    for (xml_node<>* child = parent->first_node(); child; child = child->next_sibling()) {
        if (bannedChild != child) {
            getChildrenAtTheGeneration(child, kinshipDegree - 1);
        }
    }

    writeCousinsInFile(outputFile);
}


string readXmlFile(const char* fileName) {
    if (!std::ifstream(fileName)) {
        throw FileNotFoundException(fileName);
    }
    file<> xmlFile(fileName);
    return xmlFile.data();
}



int validateNodeAttribute(xml_node<>* node) {
    if (node) {
        try {
            return  stoi(node->first_attribute()->value());
        }
        catch (runtime_error ex) {
            throw KinshipDegreeException("Kinship degree is not int");
        }
    }
    throw KinshipDegreeException("Kinship degree is not specified");

}


void getChildrenAtTheGeneration(xml_node<>* parent, int generation) {
    if (generation > 0) {
        for (xml_node<>* child = parent->first_node(); child; child = child->next_sibling()) {
            string name = child->name();
            getChildrenAtTheGeneration(child, generation - 1);
        }
    }
    else {
        cousins.push_back(parent);
    }
}


tuple<xml_node<>*, xml_node<>*> getParentAndChildByGeneration(xml_node<>* parent, xml_node<>* child, int generation) {
    if (generation > 0) {
        return getParentAndChildByGeneration(parent->parent(), parent, generation - 1);
    }
    return { parent, child };
}


xml_node<>* findNodeWithAttribute(xml_node<>* node, const string& attributeName) {

    xml_attribute<>* attr = node->first_attribute(attributeName.c_str());
    if (attr) {
        return node;
    }

    for (xml_node<>* childNode = node->first_node(); childNode; childNode = childNode->next_sibling()) {
        xml_node<>* node = findNodeWithAttribute(childNode, attributeName);
        if (node) {
            return node;
        }
    }

    return NULL;
}



void writeCousinsInFile(char* fileName) {
    std::ofstream outfile(fileName);

    if (!outfile.is_open()) {

        throw FileNotFoundException(fileName);
    }
    cout << "opened " << fileName << "\n";
    for (xml_node<>* cousin : cousins) {
        cout << cousin->name() << "\n";
        outfile << cousin->name() << "\n";
    }

    outfile.close();
}
