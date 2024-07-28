/**
 * @file tinyxml2_unittest.cc
 * @author your name (you@domain.com)
 * @brief  test file for tinyxml2
 * @version 0.1
 * @date 2024-07-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <gtest/gtest.h>

#include <iostream>

#include <tinyxml2.h>

TEST(TinyXml2Test, TestTinyXml2) {
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement* root = doc.NewElement("Root");
  doc.InsertFirstChild(root);

  tinyxml2::XMLElement* element = doc.NewElement("Element");
  element->SetText("Element Text");
  root->InsertEndChild(element);

  tinyxml2::XMLElement* element2 = doc.NewElement("Element2");
  element2->SetText("Element2 Text");
  root->InsertEndChild(element2);

  tinyxml2::XMLPrinter printer;
  doc.Print(&printer);
  std::cout << printer.CStr() << std::endl;
}