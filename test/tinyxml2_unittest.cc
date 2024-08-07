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
#include <tinyxml2.h>
#include <iostream>

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

TEST(TinyXml2Test, TestTinyXml2Load) {
  tinyxml2::XMLDocument doc;
  doc.LoadFile("d:\\0_0.xml");
  tinyxml2::XMLElement* header_element = doc.FirstChildElement("header");
  tinyxml2::XMLElement* base_param_element =
      doc.FirstChildElement("base_param");
  tinyxml2::XMLElement* result_element = doc.FirstChildElement("result");
  const char* text = result_element->Attribute("type");
  std::cout << text << std::endl;
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
