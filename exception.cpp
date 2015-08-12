#include "exception.h"

namespace pam_restrict {

  exception::exception(const char *what) noexcept
    : whatStr(what) {
  }

  const char* exception::what() const noexcept {
    return whatStr.c_str();
  }

}
