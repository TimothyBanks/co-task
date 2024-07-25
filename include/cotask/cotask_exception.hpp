#pragma once
#include <exception>
#include <string>

namespace cotask {

struct cotask_exception : public std::exception {
  std::string what_string;

  cotask_exception() = default;
  cotask_exception(const cotask_exception&) = default;
  cotask_exception(cotask_exception&&) = default;
  cotask_exception(std::string what_) : what_string{std::move(what_)} {}

  cotask_exception& operator=(const cotask_exception&) = default;
  cotask_exception& operator=(cotask_exception&&) = default;

  virtual const char* what() const noexcept override { return what_string.c_str(); }
};

}  // namespace cotask