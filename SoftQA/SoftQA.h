#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include <vector>
#include "Exceptions/FileNotFound.h"
#include "Exceptions/KinshipDegree.h"

using namespace std;
using namespace rapidxml;


vector<xml_node<>*> cousins;

/*! Найти звено в дереве, которое содержит атрибут с переданным именем
*\param [in] root - корень дерева
*\param [in] attributeName - имя атрибута
* return звено с нужным атрибутом
*/
xml_node<>* findNodeWithAttribute(xml_node<>* node, const string& attributeName);

/*! Найти предка звена по его потомку на несколько поколений
*\param [in] parent - родитель
*\param [in] child - ребенок
*\param [in] generation - текущее поколение
* return родитель и ребенок, от которого до этого родителя дошли
*/
tuple<xml_node<>*, xml_node<>*> getParentAndChildByGeneration(xml_node<>* parent, xml_node<>* child, int generation);


void getChildrenAtTheGeneration(xml_node<>* parent, int generation);

/*! Найти кузенов человека по xml дереву
*\param [out] cousins - вектор кузенов человека
*\param [in] fileName - имя файла с деревом
*/
void getCousins(char* inputFile, char* outputFile);


void writeCousinsInFile(char* fileName);



int validateNodeAttribute(xml_node<>*);



string readFile(char* fileName);