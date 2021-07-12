// Tencent is pleased to support the open source community by making TNN available.
//
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "pytnn/pytnn.h"
#include "tnn/core/tnn.h"

namespace py = pybind11;

namespace TNN_NS {

    void InitTNNPy(py::module &m) {
        py::class_<TNN>(m, "TNN")
     	    .def(py::init<>())
	    .def("Init", &TNN::Init)
            .def("DeInit", &TNN::DeInit)
            .def("AddOutput", &TNN::AddOutput)
            .def("GetModelInputShapesMap", &TNN::GetModelInputShapesMap)
            .def("CreateInst", static_cast<std::shared_ptr<Instance> (TNN::*)(NetworkConfig&, Status& ,InputShapesMap)>(&TNN::CreateInst))
            .def("CreateInst", static_cast<std::shared_ptr<Instance> (TNN::*)(NetworkConfig&, Status& ,InputShapesMap)>(&TNN::CreateInst));
    }

}  // namespace TNN_NS