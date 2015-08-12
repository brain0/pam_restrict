#define PAM_SM_SESSION
#include <security/pam_modules.h>
#include <syslog.h>
#include <security/pam_ext.h>

#include <exception>
#include "restrict.h"
#include "logger.h"

#define SO_VISIBLE [[gnu::visibility("default")]]

namespace pam_restrict {
  static int open_session(pam_handle_t *pamh, int flags, int argc, const char **argv) noexcept;
  static int close_session(pam_handle_t *pamh, int flags, int argc, const char **argv) noexcept;                     
}

extern "C" {

  PAM_EXTERN int pam_sm_open_session SO_VISIBLE (pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return pam_restrict::open_session(pamh, flags, argc, argv);
  }

  PAM_EXTERN int pam_sm_close_session SO_VISIBLE (pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return pam_restrict::close_session(pamh, flags, argc, argv);
  }

}

namespace pam_restrict {

  static int open_session(pam_handle_t *pamh, int flags, int argc, const char **argv) noexcept {
    logger log(pamh, flags & PAM_SILENT);

    try {
      restrict r(pamh, CONFIGFILE_NAME, log);
      return r.applyRestrictions() ? PAM_SUCCESS : PAM_SESSION_ERR;
    }
    catch(std::exception& e)
    {
      log.log_error("caught an exception: %s", e.what());
      return PAM_SESSION_ERR;
    }
    catch(...)
    {
      log.log_error("caught an unknown exception.");
      return PAM_SESSION_ERR;
    }
  }

  static int close_session(pam_handle_t *pamh, int flags, int argc, const char **argv) noexcept {
    return PAM_SUCCESS;
  }

}
