#ifndef DAS_RESOURCE_FORMAT_LOADER_H
#define DAS_RESOURCE_FORMAT_LOADER_H

#include "core/io/resource_loader.h"

class DasResourceFormatLoader : public ResourceFormatLoader {
public:
    Ref<Resource> load(const String &p_path, const String &p_original_path = "", Error *r_error = nullptr, bool p_use_sub_threads = false, float *r_progress = nullptr, CacheMode p_cache_mode = CACHE_MODE_REUSE) override;
    void get_recognized_extensions(List<String> *p_extensions) const override;
    bool handles_type(const String &p_type) const override;
    String get_resource_type(const String &p_path) const override;
    void get_dependencies(const String &p_path, List<String> *p_dependencies, bool p_add_types = false) override;
};

#endif // DAS_RESOURCE_FORMAT_LOADER_H

