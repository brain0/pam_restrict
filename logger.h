#pragma once

#include <security/pam_modules.h>
#include <cstdarg>

namespace pam_restrict {

  class logger {
    private:
      pam_handle_t* pamHandle;
      bool silent;
      void log(int level, const char* format, va_list ap);
    public:
      logger(pam_handle_t* pamHandle, bool silent);
      void log_info(const char *format, ...);
      void log_error(const char *format, ...);
  };

}
