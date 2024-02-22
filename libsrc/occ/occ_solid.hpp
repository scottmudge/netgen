#ifndef FILE_OCC_SOLID_INCLUDED
#define FILE_OCC_SOLID_INCLUDED

#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>

#include "meshing.hpp"

namespace netgen
{
    class OCCSolid : public GeometrySolid 
    {
        TopoDS_Solid solid;

        public:
        OCCSolid(TopoDS_Shape dshape)
            : solid(TopoDS::Solid(dshape))
        { }
#if OCC_VERSION_HEX < 0x070800
        size_t GetHash() const override { return solid.HashCode(std::numeric_limits<Standard_Integer>::max()); }
#else
        size_t GetHash() const override { return std::hash<TopoDS_Solid>{}(solid); }
#endif
    };
}

#endif // FILE_OCC_SOLID_INCLUDED
