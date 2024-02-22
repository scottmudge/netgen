#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include <BRepGProp.hxx>
#include <BRep_Tool.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>

#pragma clang diagnostic pop

#include "occ_edge.hpp"
#include "occgeom.hpp"

namespace netgen
{
    OCCEdge::OCCEdge(TopoDS_Shape edge_, GeometryVertex & start_, GeometryVertex & end_)
        : GeometryEdge(start_, end_),
          edge(TopoDS::Edge(edge_))
    {
        curve = BRep_Tool::Curve(edge, s0, s1);
        BRepGProp::LinearProperties(edge, props);

        auto verts = GetVertices(edge);
        if(verts.size() != 2)
            throw Exception("OCC edge does not have 2 vertices");

        if(start != end)
        {
            // swap start/end if necessary
            double d00 = Dist(GetPoint(0), start->GetPoint());
            double d01 = Dist(GetPoint(0), end->GetPoint());
            if(d01 < d00)
                swap(start, end);
        }
    }

    double OCCEdge::GetLength() const
    {
        return props.Mass();
    }

    Point<3> OCCEdge::GetCenter() const
    {
        return occ2ng( props.CentreOfMass() );
    }

    Point<3> OCCEdge::GetPoint(double t) const
    {
        return occ2ng( curve->Value(s0+t*(s1-s0)) );
    }

    double OCCEdge::CalcStep(double t, double sag) const
    {
        throw Exception(ToString("not implemented") + __FILE__ + ":" + ToString(__LINE__));
    }

    size_t OCCEdge::GetHash() const
    {
#if OCC_VERSION_HEX < 0x070800
      return edge.HashCode(std::numeric_limits<Standard_Integer>::max());
#else
      return std::hash<TopoDS_Shape>{}(edge); 
#endif
    }

    void OCCEdge::ProjectPoint(Point<3>& p, EdgePointGeomInfo* gi) const
    {
        auto pnt = ng2occ(p);
        // extend the projection parameter range, else projection might fail
        // for an endpoint
        // see discussion here: https://forum.ngsolve.org/t/how-to-apply-occidentification-correctly/2555
        // I do not see a better way using occ tolerances?
        double eps = 1e-7 * (s1-s0);
        GeomAPI_ProjectPointOnCurve proj(pnt, curve, s0-eps, s1+eps);
        pnt = proj.NearestPoint();
        if(gi)
            gi->dist = (proj.LowerDistanceParameter() - s0)/(s1-s0);
        p = occ2ng(pnt);
    }

    Vec<3> OCCEdge::GetTangent(double t) const
    {
        t = s0 + t*(s1-s0);
        gp_Pnt p;
        gp_Vec v;
        curve->D1(t, p, v);
        return occ2ng(v) * (s1-s0);
    }

}
