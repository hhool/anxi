/**
 * @file document_interface.h
 * @author hhool (hhool@outlook.com)
 * @brief  document interface file for the solution
 * @version 0.1
 * @date 2024-07-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef APP_DOCUMENT_DOCUMENT_INTERFACE_H_
#define APP_DOCUMENT_DOCUMENT_INTERFACE_H_

#include <memory>
#include <string>

namespace anx {
namespace document {

class DocumentInterface {
 public:
  DocumentInterface() = default;
  virtual ~DocumentInterface() = default;

  virtual std::string LoadDocument(const std::string& file_path) = 0;
  virtual void SaveDocument(const std::string& file_path) = 0;
};

class DocumentFactory {
 public:
  DocumentFactory() = default;
  virtual ~DocumentFactory() = default;

  virtual std::unique_ptr<DocumentInterface> CreateDocument() = 0;
};

}  // namespace document
}  // namespace anx

#endif  // APP_DOCUMENT_DOCUMENT_INTERFACE_H_
