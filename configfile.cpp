#include "configfile.h"
#include "exception.h"

namespace pam_restrict {

  configfile::configfile(const char *filename) {
    config = g_key_file_new();
    if(config == nullptr)
      throw exception("g_key_file_new() failed.");
    if(!g_key_file_load_from_file(config, filename, G_KEY_FILE_NONE, nullptr)) {
      g_key_file_free(config);
      throw exception("Unable to parse configuration file.");
    }
  }
  
  configfile::~configfile() {
    g_key_file_free(config);
  }

  bool configfile::newPrivsDisallowed(const char *username) {
    return get_boolean(username, keyNoNewPrivs, false);
  }
   
  bool configfile::get_boolean(const char *group, const char *key, bool def) {
    bool res, good;
    GError *err = nullptr;
    good = false;

    res = g_key_file_get_boolean(config, group, key, &err);
    if(err == nullptr) {
      /* use value from configuration file */
      good = true;
    }
    else if(err->code == G_KEY_FILE_ERROR_KEY_NOT_FOUND || err->code == G_KEY_FILE_ERROR_GROUP_NOT_FOUND) {
      /* default value */
      good = true;
      res = def;
    }
    g_clear_error(&err);
    if(!good)
      throw exception("Error while reading a value from the configuration file.");
    return res;
  }

}
