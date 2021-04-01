#include "ZipUtils.hpp"
#include "beatsaber-hook/shared/utils/utils.h"

static LoggerContextObject& getLogger()
{
    static LoggerContextObject logger = (new Logger({ID, VERSION}, LoggerOptions(false, true)))->WithContext("Zip Utils");
    return logger;
}

namespace ZipUtils
{
    bool GetBytesFromZipFile(std::string zipPath, std::string fileName, std::vector<uint8_t>& out)
    {
        if (zipPath.ends_with("/"))
        {
            getLogger().error("Path '%s' was a folder path!", zipPath.c_str());
            return false;
        }

        if (!fileexists(zipPath) && !direxists(zipPath)) 
        {
            getLogger().error("Path '%s' did not exist!", zipPath.c_str());
            return false;
        }
        if (fileName == "") 
        {
            getLogger().error("File Name was empty!");
            return false;
        }

        int err = 0;
        zip *z = zip_open(zipPath.c_str(), 0, &err);

        if (!ReadZipFile(z, fileName, out)) 
        {
            zip_close(z);
            return false;
        }
        zip_close(z);
        return true;
    }

    namespace 
    {
        bool ReadZipFile(zip* z, std::string fileName, std::vector<uint8_t>& out)
        {
            int index = zip_name_locate(z, fileName.c_str(), 0);
            
            if (index == -1)
            {
                return false;
                getLogger().info("file '%s' did not exist in zip, are you sure you passed in the right name?", fileName.c_str());
            }
            getLogger().info("Reading file '%s'", fileName.c_str());
            struct zip_stat st;
            zip_stat_init(&st);
            zip_stat(z, fileName.c_str(), 0, &st);

            zip_file* f = zip_fopen(z, st.name, 0);
            char* file = new char[st.size];

            zip_fread(f, file, st.size);
            getLogger().info("Found %d bytes", st.size);
            zip_fclose(f);

            for (int i = 0; i < st.size; i++)
            {
                out.push_back((uint8_t)file[i]);
            }

            delete[](file);
            return true;
        }
    }
}