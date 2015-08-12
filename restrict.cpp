#include "restrict.h"

#include <errno.h>
#include <sys/prctl.h>
#include <cstring>
#include <security/pam_modules.h>
#include <syslog.h>
#include <security/pam_ext.h>
#include <pwd.h>
#include <cstdarg>
#include "exception.h"

namespace pam_restrict {

  restrict::restrict(pam_handle_t* pamHandle,
        const char *configFilename,
        logger& log)
    : pamHandle(pamHandle),
      cfg(configFilename),
      log(log) {
    if(pam_get_user(pamHandle, &username, nullptr) != PAM_SUCCESS)
      throw exception("Could not get username: pam_get_user() failed.");

    errno = 0;
    struct passwd *pw = getpwnam(username);
    if(pw == nullptr)
      throw exception("Could not look up user info: getpwnam() failed.");
    uid = pw->pw_uid;
  }
  
  bool restrict::applyRestrictions() {
    if(uid == 0)
      return true;

    if(cfg.newPrivsDisallowed(username))
      if(!setNoNewPrivs())
        return false;

    return true;
  }

  bool restrict::setNoNewPrivs() {
    errno = 0;
    if(prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) != 0) {
      log.log_error("prctl() failed: %s", strerror(errno));
      return false;
    }
    log.log_info("Applied NoNewPrivs setting to session for user %s.", username);
    return true;
  }
  
}
