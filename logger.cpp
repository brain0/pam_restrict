#include "logger.h"

#include <syslog.h>
#include <security/pam_ext.h>

namespace pam_restrict {

  logger::logger(pam_handle_t* pamHandle, bool silent)
    : pamHandle(pamHandle), silent(silent) {}

  void logger::log(int level, const char* format, va_list ap) {
    if(!silent) {
      pam_vsyslog(pamHandle, LOG_AUTH | level, format, ap);
    }
  }

  void logger::log_info(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    log(LOG_INFO, format, ap);
    va_end(ap);
  }

  void logger::log_error(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    log(LOG_ERR, format, ap);
    va_end(ap);
  }

}
