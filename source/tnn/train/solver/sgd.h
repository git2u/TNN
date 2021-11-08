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

// author: sanerzheng@tencent.com

#ifndef TNN_SOURCE_TNN_TRAIN_SOVERL_SGD_H
#define TNN_SOURCE_TNN_TRAIN_SOVERL_SGD_H
#include "tnn/train/solver/base_solver.h"
#include <map>
namespace TNN_NS {
namespace train {

class SGD : public BaseSolver {
public:
    // BaseSolver(network, config),
    SGD(AbstractNetwork *network, NetworkConfig *config, float learningrate)
        : BaseSolver(network, config), learningrate_(learningrate){};
    virtual ~SGD(){};
    virtual Status UpdateTrainableVariable(RawBuffer *resource_param,
                                           const std::shared_ptr<RawBuffer> &resource_param_grad);
    virtual Status ComputeUpdateValue(RawBuffer *resource_param, std::shared_ptr<RawBuffer> &resource_param_grad);
    virtual Status step();

private:
    float learningrate_;
};

} // namespace train
} // namespace TNN_NS
#endif // TNN_SOURCE_TNN_TRAIN_SOVERL_SGD_H