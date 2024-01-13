#ifndef DAS_RESOURCE_FORMAT_H
#define DAS_RESOURCE_FORMAT_H

#include "core/io/resource_loader.h"


class DasResourceFormatLoader : public ResourceFormatLoader {
    Ref<Resource> _get_full_script(const String &p_path, Error &r_error);
public:
    Ref<Resource> load(const String &p_path, const String &p_original_path = "", Error *r_error = nullptr, bool p_use_sub_threads = false, float *r_progress = nullptr, CacheMode p_cache_mode = CACHE_MODE_REUSE) override;
    void get_recognized_extensions(List<String> *p_extensions) const override;
    bool handles_type(const String &p_type) const override;
    String get_resource_type(const String &p_path) const override;
    void get_dependencies(const String &p_path, List<String> *p_dependencies, bool p_add_types = false) override;
};


#include <core/io/resource_saver.h>

class DasResourceFormatSaver : public ResourceFormatSaver {
public:
	Error save(const Ref<Resource> &p_resource, const String &p_path, uint32_t p_flags = 0) override;
	void get_recognized_extensions(const Ref<Resource> &p_resource, List<String> *p_extensions) const override;
	bool recognize(const Ref<Resource> &p_resource) const override;
};

#endif // DAS_RESOURCE_FORMAT_H

