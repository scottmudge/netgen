#include <BRepGProp.hxx>
#include <BRep_Tool.hxx>

#include "occ_vertex.hpp"

namespace netgen
{

    OCCVertex::OCCVertex( TopoDS_Shape s )
        : vertex(TopoDS::Vertex(s))
    {
        p = occ2ng(vertex);
    }

    Point<3> OCCVertex::GetPoint() const
    {
        return p;
    }

    size_t OCCVertex::GetHash() const
    {
#if OCC_VERSION_HEX < 0x070800
      return vertex.HashCode(std::numeric_limits<Standard_Integer>::max());
#else
      return std::hash<TopoDS_Shape>{}(vertex); 
#endif
    }
}
