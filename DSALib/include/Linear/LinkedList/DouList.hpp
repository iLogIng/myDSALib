#pragma once
#include "dNode.hpp"
#include <memory>

namespace myDSALib
{
namespace Linear
{

template<typename Ty>
class DouList
{
    using Node = dNode<Ty>;
    using pNode = dNode<Ty>*;
    using unique_pNode = std::unique_ptr<dNode<Ty>>;
private:
    
};

}
}