#pragma once

#include <security/pam_modules.h>
#include <sys/types.h>
#include "configfile.h"
#include "logger.h"

namespace pam_restrict {

  class restrict {
    private:
      pam_handle_t* pamHandle;
      const char *username;
      uid_t uid;
      configfile cfg;
      logger& log;

      bool setNoNewPrivs();
    public:
      restrict(pam_handle_t* pamHandle,
        const char *configFilename,
        logger& log);
      bool applyRestrictions();
  };
  
}
