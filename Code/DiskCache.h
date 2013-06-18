
// thread safe collection

#ifndef DISKCACHE_H
#define	DISKCACHE_H

#define HTTP_DISKCACHE_DEFAULT_EXTENSIONS       ".bc,.bct,.css,.png,.jpg"

#include <map>
#include <mutex>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "Definitions.h"
#include "ServiceController.h"
#include "DiskCacheItem.h"
#include "Utils.h"
#include "Configurator.h"

using std::map;
using std::mutex;
using std::unique_lock;
using std::string;
using std::vector;
using std::iostream;
using std::ifstream;

using BC::ServiceController;
using BC::Utils;
using BC::Services::Configurator;

// Forward declarations
namespace BC
{
    namespace Web
    {
        namespace Http
        {
            class DiskCacheItem;
        }
    }
}
using BC::Web::Http::DiskCacheItem;

namespace BC
{
    namespace Web
    {
        namespace Http
        {
            class DiskCache
            {
            private:
                string basePath;                        // The base path for files.
                mutex mlock;                            // Used for locking mutual access to the cache.
                map<string, DiskCacheItem*> items;      // Cached items.
                vector<string> cachedExtensions;        // The extensions to be cached.
            public:
                DiskCache(ServiceController *con, string basePath);
                ~DiskCache();
                // -- Fetches an item from cache; returns zero if not found.
                // -- If the item is not in the cache, it'll check the
                // -- file-system and cache the item if its extension is
                // -- to be cached.
                DiskCacheItem* fetch(string relativePath);
                // -- Fetches an item as a string; returns errorMessage if
                // -- the item could not be loaded.
                string fetch(string relativePath, string errorMessage);
                // -- Fetches an item and places it in dest if found; returns
                // -- true if found, false if not found.
                bool fetchLoad(string relativePath, string &dest);
                // -- Indicates if the item at the path ends with an
                // -- extension which is in the cache list.
                bool isCacheAppropriate(string path);
                // -- Wipes and reloads the cache with items from the local
                // -- file-system.
                void reload(ServiceController *con);
                // -- Disposes all of the cached items.
                void dispose();
            private:
                // -- Attempts to load an item at the specified path; returns
                // -- 0 if it could not be loaded. This is an internal
                // -- function. Does not load items into the cache, but
                // -- rather the invoking function should add it to the cache.
                DiskCacheItem* load(string relativePath);
            };
        }
    }
}

#endif	/* DISKCACHE_H */

