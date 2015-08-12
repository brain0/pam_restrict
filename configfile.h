#pragma once

#include <string>
#include <glib.h>

namespace pam_restrict {
  class configfile {
    private:
      static constexpr const char *keyNoNewPrivs = "NoNewPrivs";
      
      GKeyFile *config;
      bool get_boolean(const char *group, const char *key, bool *result, bool def);
    public:
      configfile(const char *filename);
      ~configfile();
      bool newPrivsDisallowed(const char *username);
  };
}
