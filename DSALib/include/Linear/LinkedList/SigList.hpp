#pragma once
#include "sNode.hpp"

namespace myDSALib
{
namespace Linear
{

template<typename Ty>
class SigList
{
    using Node = sNode<Ty>;
    using pNode = sNode<Ty>*;
    using unique_pNode = std::unique_ptr<sNode<Ty>>;
private:
    
};

}
}