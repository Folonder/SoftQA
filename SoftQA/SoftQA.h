#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include <vector>
#include "Exceptions/FileNotFound.h"
#include "Exceptions/KinshipDegree.h"

using namespace std;
using namespace rapidxml;


vector<xml_node<>*> cousins;

/*! ����� ����� � ������, ������� �������� ������� � ���������� ������
*\param [in] root - ������ ������
*\param [in] attributeName - ��� ��������
* return ����� � ������ ���������
*/
xml_node<>* findNodeWithAttribute(xml_node<>* node, const string& attributeName);

/*! ����� ������ ����� �� ��� ������� �� ��������� ���������
*\param [in] parent - ��������
*\param [in] child - �������
*\param [in] generation - ������� ���������
* return �������� � �������, �� �������� �� ����� �������� �����
*/
tuple<xml_node<>*, xml_node<>*> getParentAndChildByGeneration(xml_node<>* parent, xml_node<>* child, int generation);


void getChildrenAtTheGeneration(xml_node<>* parent, int generation);

/*! ����� ������� �������� �� xml ������
*\param [out] cousins - ������ ������� ��������
*\param [in] fileName - ��� ����� � �������
*/
void getCousins(char* inputFile, char* outputFile);


void writeCousinsInFile(char* fileName);



int validateNodeAttribute(xml_node<>*);



string readFile(char* fileName);