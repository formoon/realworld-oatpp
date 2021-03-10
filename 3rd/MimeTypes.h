#ifndef MIMETYPES_H_
#define MIMETYPES_H_
#include <string.h>

#define TYPES_LENGTH 349
class MimeTypes {
  public:
    static const char* getType(const char * path);
    static const char* getExtension(const char * type, int skip = 0);

  private:
    struct entry {
      const char* fileExtension;
      const char* mimeType;
    };
    static MimeTypes::entry types[TYPES_LENGTH];
    static int strcmpi(const char *s1, const char *s2);
};

#endif
