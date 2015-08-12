#pragma once

#include <exception>
#include <string>

namespace pam_restrict {

  class exception : public std::exception {
    private:
      std::string whatStr;
    public:
      exception(const char *what) noexcept;
      virtual const char* what() const noexcept;
      virtual ~exception() = default;
  };

}
