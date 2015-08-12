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
    bool result;
    
    if(!get_boolean(username, keyNoNewPrivs, &result, false))
      throw exception("Error while reading a value from the configuration file.");
    
    return result;
  }
   
  bool configfile::get_boolean(const char *group, const char *key, bool *result, bool def) {
    bool res, ret;
    GError *err = nullptr;
    ret = false;

    res = g_key_file_get_boolean(config, group, key, &err);
    if(err == nullptr) {
      /* use value from configuration file */
      ret = true;
      *result = res;
    }
    else if(err->code == G_KEY_FILE_ERROR_KEY_NOT_FOUND || err->code == G_KEY_FILE_ERROR_GROUP_NOT_FOUND) {
      /* default value */
      ret = true;   
      *result = def;
    }
    g_clear_error(&err);
    return ret;
  }

}
