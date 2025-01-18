struct MemRegion_t {
    uintptr_t start, end;
};

enum RegionType {
    ALL,
    JAVA_HEAP,
    C_HEAP,
    C_ALLOC,
    C_DATA,
    C_BSS,
    PPSSPP,
    ANONYMOUS,
    JAVA,
    STACK,
    ASHMEM,
    VIDEO,
    OTHER,
    BAD,
    CODE_APP,
    CODE_SYS
};

vector<MemRegion_t> GetMemRegions(unsigned int regionType) {
    LOGI("GetMemRegions: %d", regionType);
    vector<MemRegion_t> result;

    char line[512];
    FILE *f = fopen("/proc/self/maps", "r");

    if (!f)
        return result;

    while (fgets(line, sizeof line, f)) {
        uintptr_t start = 0, end = 0;
        char name[256], flags[64];
        if (sscanf(line, "%" PRIXPTR "-%" PRIXPTR " %s %*s %*s %*s %s", &start, &end, flags, name) > 0) {
            if (flags[0] == 'r' && flags[1] == 'w') {
                if (regionType == ALL) {
                    result.push_back({start, end});
                    LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                }
                if (regionType == JAVA_HEAP) {
                    if (strstr(name, "dalvik")) {
                        if ((strstr(name, "eap")
                             || strstr(name, "dalvik-alloc")
                             || strstr(name, "dalvik-main")
                             || strstr(name, "dalvik-large")
                             || strstr(name, "dalvik-free"))
                            && !strstr(name, "itmap")
                            && !strstr(name, "ygote")
                            && !strstr(name, "ard")
                            && !strstr(name, "jit")
                            && !strstr(name, "inear")) {
                            result.push_back({start, end});
                            LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                        }
                    }
                }
                if (regionType == C_HEAP) {
                    if (strstr(name, "[heap]")) {
                        result.push_back({start, end});
                        LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                    }
                }
                if (regionType == C_ALLOC) {
                    if (strstr(name, "malloc")) {
                        result.push_back({start, end});
                        LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                    }
                }
                if (regionType == C_DATA) {
                    if (strstr(name, "/lib") && strstr(name, ".so")) {
                        if (strstr(name, GAME_PACKAGE)) {
                            result.push_back({start, end});
                            LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                        }
                    }
                }
                if (regionType == C_BSS) {
                    if (strstr(name, "[anon:.bss]")) {
                        result.push_back({start, end});
                        LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                    }
                }
                if (regionType == PPSSPP) {
                    if (strstr(name, "PPSSPP_RAM")) {
                        result.push_back({start, end});
                        LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                    }
                }
                if (regionType == ANONYMOUS) {
                    if (strlen(line) < 42) {
                        result.push_back({start, end});
                        LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                    }
                }
                if (regionType == JAVA) {
                    if (strstr(name, "dalvik")) {
                        if (!((strstr(name, "eap")
                               || strstr(name, "dalvik-alloc")
                               || strstr(name, "dalvik-main")
                               || strstr(name, "dalvik-large")
                               || strstr(name, "dalvik-free"))
                              && !strstr(name, "itmap")
                              && !strstr(name, "ygote")
                              && !strstr(name, "ard")
                              && !strstr(name, "jit")
                              && !strstr(name, "inear"))) {
                            result.push_back({start, end});
                            LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                        }
                    }
                }
                if (regionType == STACK) {
                    if (strstr(name, "[stack]")) {
                        result.push_back({start, end});
                        LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                    }
                }
                if (regionType == ASHMEM) {
                    if (!strncmp(name, "/dev/ashmem", 0xBuLL) && !strstr(name, "MemoryHeapBase")) {
                        result.push_back({start, end});
                        LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                    }
                }
                if (regionType == VIDEO) {
                    if (!strncmp(name, "/dev/", 5uLL)
                        && (strstr(name, "/dev/mali")
                            || strstr(name, "/dev/kgsl")
                            || strstr(name, "/dev/nv")
                            || strstr(name, "/dev/tegra")
                            || strstr(name, "/dev/ion")
                            || strstr(name, "/dev/pvr")
                            || strstr(name, "/dev/render")
                            || strstr(name, "/dev/galcore")
                            || strstr(name, "/dev/fimg2d")
                            || strstr(name, "/dev/quadd")
                            || strstr(name, "/dev/graphics")
                            || strstr(name, "/dev/mm_")
                            || strstr(name, "/dev/dri/"))) {
                        result.push_back({start, end});
                        LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                    }
                }
                if (regionType == OTHER) {
                    if (!strstr(name, "system@")
                        && !strstr(name, "gralloc")
                        && strncmp(name, "[vdso]", 6uLL)
                        && strncmp(name, "[vectors]", 9uLL)
                        && (strncmp(name, "/dev/", 5uLL) || !strncmp(name, "/dev/ashmem", 0xBuLL))) {
                        result.push_back({start, end});
                        LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                    }
                }
                if (regionType == BAD) {
                    if (!strncmp(name, "/dev/", 5uLL) && strstr(name, "/dev/xLog")
                        || !strncmp(name, "/system/fonts/", 0xEuLL)
                        || !strncmp(name, "anon_inode:dmabuf", 0x11uLL)) {
                        result.push_back({start, end});
                        LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                    }
                }
                if (regionType == CODE_APP) {
                    if (strstr(name, GAME_PACKAGE)) {
                        result.push_back({start, end});
                        LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                    }
                }
                if (regionType == CODE_SYS) {
                    if (!strstr(name, GAME_PACKAGE)) {
                        result.push_back({start, end});
                        LOGI("Found for regionType %d: %s (%x-%x)", regionType, name, start, end);
                    }
                }
            }
        }
    }

    fclose(f);

    return result;
}

template<typename T>
vector<uintptr_t> ScanValue(vector<MemRegion_t> regions, T value) {
    stringstream s;
    s << "ScanValue: " << value << endl;
    LOGI(s.str().c_str());

    vector<uintptr_t> result;
    for (auto &region : regions) {
        for (auto addr = region.start; addr != region.end; addr += sizeof(T)) {
            if (*(T *) (addr) == value) {
                result.push_back(addr);
            }
        }
    }
    LOGI("ScanValue has scanned total %d", result.size());
    return result;
}

vector<uintptr_t> ScanString(vector<MemRegion_t> regions, const char *value) {
    vector<uintptr_t> result;
    for (auto &region : regions) {
        for (auto addr = region.start; addr != region.end; addr++) {
            if (!strncmp((const char *) (addr), value, strlen(value))) {
                result.push_back(addr);
            }
        }
    }
    LOGI("ScanString has scanned total %d", result.size());
    return result;
}

template<typename T>
vector<uintptr_t> ScanValues(vector<MemRegion_t> regions, vector<T> values) {
    stringstream s;
    s << "ScanValues: ";
    for(T value : values)
        s << value << ", ";
    string str = s.str().c_str();

    if(str.length()) {
        str.pop_back();
        str.pop_back();
    }

    LOGI(str.c_str());

    vector<uintptr_t> result;
    for (auto &region : regions) {
        for (auto addr = region.start; addr != region.end; addr += sizeof(T)) {
            vector<uintptr_t> tmp;
            for (int j = 0; j < values.size(); j++) {
                if (values[j] == *(T *)(addr)) {
                    tmp.push_back(addr);
                } else {
                    tmp.clear();
                    break;
                }
                addr += (j * sizeof(T));
            }
            if (tmp.size()) {
                result.insert(result.end(), tmp.begin(), tmp.end());
            }
        }
    }
    LOGI("ScanValues has scanned total %d", result.size());
    return result;
}

template<typename T>
vector<uintptr_t> RescanValue(vector<uintptr_t> results, T value) {
    vector<uintptr_t> result;
    for (int i = 0; i < results.size(); i++) {
        if (*(T *) (results[i]) == value) {
            result.push_back(results[i]);
        }
    }
    LOGI("RescanValue has scanned total %d", result.size());
    return result;
}

template<typename T>
vector<uintptr_t> RescanValues(vector<uintptr_t> results, vector<T> values) {
    vector<uintptr_t> result;
    for (int i = 0; i < results.size(); i++) {
        if (i > values.size())
            break;
        else {
            if (*(T *) (results[i]) == values[i]) {
                result.push_back(results[i]);
            }
        }
    }
    LOGI("RescanValues has scanned total %d", result.size());
    return result;
}