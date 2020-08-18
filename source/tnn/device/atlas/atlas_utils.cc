// Copyright 2019 Tencent. All Rights Reserved

#include "atlas_utils.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

namespace TNN_NS {

std::vector<std::string> SplitPath(const std::string& str, const std::set<char> delimiters) {
    std::vector<std::string> result;
    char const* pch   = str.c_str();
    char const* start = pch;
    for (; *pch; ++pch) {
        if (delimiters.find(*pch) != delimiters.end()) {
            if (start != pch) {
                std::string str(start, pch);
                result.push_back(str);
            } else {
                result.push_back("");
            }
            start = pch + 1;
        }
    }
    result.push_back(start);
    return result;
}

long GetCurentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int SaveMemToFile(std::string file_name, void* data, int size) {
    FILE* fd = fopen(file_name.c_str(), "wb");
    if (fd == nullptr) {
        return -1;
    }

    int ret = fwrite(data, 1, size, fd);
    if (ret != size) {
        fclose(fd);
        return -1;
    }

    fclose(fd);
    return 0;
}

Status ConvertFromAclDataTypeToTnnDataType(aclDataType acl_datatype, DataType& tnn_datatype) {
    if (ACL_FLOAT == acl_datatype) {
        tnn_datatype = DATA_TYPE_FLOAT;
    } else if (ACL_FLOAT16 == acl_datatype) {
        tnn_datatype = DATA_TYPE_HALF;
    } else if (ACL_INT8 == acl_datatype || ACL_UINT8 == acl_datatype) {
        tnn_datatype = DATA_TYPE_INT8;
    } else if (ACL_INT32 == acl_datatype || ACL_UINT32 == acl_datatype) {
        tnn_datatype = DATA_TYPE_INT32;
    } else {
        LOGE("not support convert from acl datatype (%d) to tnn datatype\n", acl_datatype);
        return Status(TNNERR_COMMON_ERROR, "the data type is not support");
    }
    return TNN_OK;
}

Status ConvertFromAclDataFormatToTnnDataFormat(aclFormat acl_format, DataFormat& tnn_dataformat) {
    if (ACL_FORMAT_NCHW == acl_format || ACL_FORMAT_ND == acl_format) {
        tnn_dataformat = DATA_FORMAT_NCHW;
    } else if (ACL_FORMAT_NHWC == acl_format) {
        tnn_dataformat = DATA_FORMAT_NHWC;
    } else {
        LOGE("not support convert from acl dataformat (%d) to tnn datatype\n", acl_format);
        return Status(TNNERR_COMMON_ERROR, "the data format is not support");
    }
    return TNN_OK;
}

Status ConvertFromMatTypeToAippInputFormat(MatType mat_type, aclAippInputFormat& aipp_input_format) {
    if (N8UC3 == mat_type) {
        aipp_input_format = ACL_RGB888_U8;
    } else if (N8UC4 == mat_type) {
        aipp_input_format = ACL_XRGB8888_U8;
    } else if (NNV12 == mat_type || NNV21 == mat_type) {
        aipp_input_format = ACL_YUV420SP_U8;
    } else if (NGRAY == mat_type) {
        aipp_input_format = ACL_YUV400_U8;
    } else {
        LOGE("not support convert from mat type (%d) to aipp input format\n", mat_type);
        return Status(TNNERR_ATLAS_AIPP_NOT_SUPPORT, "the mat type is not support");
    }

    return TNN_OK;
}

Status ConvertFromMatTypeToDvppPixelFormat(MatType mat_type, acldvppPixelFormat& dvpp_pixel_format) {
    if (N8UC3 == mat_type) {
        dvpp_pixel_format = PIXEL_FORMAT_RGB_888;
    } else if (N8UC4 == mat_type) {
        dvpp_pixel_format = PIXEL_FORMAT_RGBA_8888;
    } else if (NNV12 == mat_type) {
        dvpp_pixel_format = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    } else if (NNV21 == mat_type) {
        dvpp_pixel_format = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    } else {
        LOGE("not support convert from mat type (%d) to dvpp pixel format\n", mat_type);
        return Status(TNNERR_ATLAS_DVPP_NOT_SUPPORT, "the mat type is not support");
    }

    return TNN_OK;
}

bool IsDynamicBatch(aclmdlDesc* model_desc, std::string input_name) {
    size_t index     = 0;
    aclError acl_ret = aclmdlGetInputIndexByName(model_desc, input_name.c_str(), &index);
    if (ACL_ERROR_NONE != acl_ret) {
        return false;
    }

    aclmdlIODims acl_dims;
    acl_ret = aclmdlGetInputDims(model_desc, index, &acl_dims);
    if (ACL_ERROR_NONE != acl_ret) {
        return false;
    }

    if (-1 == acl_dims.dims[0]) {
        return true;
    }
    return false;
}

}  // namespace TNN_NS