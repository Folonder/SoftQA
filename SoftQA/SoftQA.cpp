#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include <vector>
#include "Exceptions/FileNotFound.h"
#include "Exceptions/KinshipDegree.h"

using namespace std;
using namespace rapidxml;

/*! Найти звено в дереве, которое содержит атрибут с переданным именем
*\param [in] root - корень дерева
*\param [in] attributeName - имя атрибута
* return звено с нужным атрибутом
*/
tuple<int, xml_node<>*> findNodeWithAttribute(xml_node<>* node, const string& attributeName);

/*! Найти предка звена по его потомку на несколько поколений
*\param [in] parent - родитель
*\param [in] child - ребенок
*\param [in] generation - текущее поколение
* return родитель и ребенок, от которого до этого родителя дошли
*/
tuple<xml_node<>*, xml_node<>*> getParentAndChildByGeneration(xml_node<>* parent, xml_node<>* child, int generation);


void getChildrenAtTheGeneration(xml_node<>* parent, int generation, vector<xml_node<>*>& cousins);

/*! Найти кузенов человека по xml дереву
*\param [out] cousins - вектор кузенов человека
*\param [in] fileName - имя файла с деревом
*/
void getCousins(vector<xml_node<>*>& cousins, char* fileName);


void writeCousinsInFile(vector<xml_node<>*> cousins, char* fileName);



int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            throw runtime_error("Incorrect arguments");
        }

        vector<xml_node<>*> cousins;

        getCousins(cousins, argv[1]);

        writeCousinsInFile(cousins, argv[2]);
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



void getCousins(vector<xml_node<>*>& cousins, char* fileName) {
    if (!std::ifstream(fileName)) {
        throw FileNotFoundException(fileName);
    }
    file<> xmlFile(fileName);
    xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    xml_node<>* root = doc.first_node();

    auto[kinshipDegree, node] = findNodeWithAttribute(root, "kinship_degree");

    if (kinshipDegree <= 0) {
        throw KinshipDegreeException("Kinship degree is not specified");
    }

    auto[parent, bannedChild] = getParentAndChildByGeneration(node, nullptr, kinshipDegree);

    for (xml_node<>* child = parent->first_node(); child; child = child->next_sibling()) {
        if (bannedChild != child) {
            getChildrenAtTheGeneration(child, kinshipDegree - 1, cousins);
        }
    }
}


void getChildrenAtTheGeneration(xml_node<>* parent, int generation, vector<xml_node<>*>& cousins) {
    if (generation > 0) {
        for (xml_node<>* child = parent->first_node(); child; child = child->next_sibling()) {
            string name = child->name();
            getChildrenAtTheGeneration(child, generation - 1, cousins);
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


tuple<int, xml_node<>*> findNodeWithAttribute(xml_node<>* node, const string& attributeName) {
    
    xml_attribute<>* attr = node->first_attribute(attributeName.c_str());
    if (attr) {
        try {
            int attrValue = stoi(attr->value());
            return { attrValue, node };
        }
        catch (runtime_error ex) {
            throw KinshipDegreeException("Kinship degree is not int");
        }
    }
    
    for (xml_node<>* childNode = node->first_node(); childNode; childNode = childNode->next_sibling()) {
        auto[kinshipDegree, node] = findNodeWithAttribute(childNode, attributeName);
        if (kinshipDegree >= 0) {
            return { kinshipDegree, node };
        }
    }
    
    return tuple{ -1, nullptr };
}



void writeCousinsInFile(vector<xml_node<>*> cousins, char* fileName) {
    std::ofstream outfile(fileName);

    // Check if the file was opened successfully
    if (!outfile.is_open()) {

        throw FileNotFoundException(fileName);
    }
    cout << "opened" << fileName;
    for (xml_node<>* cousin : cousins) {
        cout << cousin->name() << "\n";
        outfile << cousin->name() << "\n";
    }

    outfile.close();
}
