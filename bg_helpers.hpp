#ifndef BG_HELPERS_H
#define BG_HELPERS_H

#include "eulerian_paths.hpp"
#ifdef GEOS_VERSION
#include <geos/io/WKTReader.h>
#include <geos/io/WKTWriter.h>
#include <geos/operation/buffer/BufferOp.h>
#endif // GEOS_VERSION

template <typename polygon_type_t, typename rhs_t>
static inline bg::model::multi_polygon<polygon_type_t> operator-(const bg::model::multi_polygon<polygon_type_t>& lhs,
                                                                 const rhs_t& rhs) {
  if (bg::area(rhs) <= 0) {
    return lhs;
  }
  bg::model::multi_polygon<polygon_type_t> ret;
  bg::difference(lhs, rhs, ret);
  return ret;
}

template <typename rhs_t>
static inline multi_polygon_type_fp operator-(const polygon_type_fp& lhs, const rhs_t& rhs) {
  if (bg::area(rhs) <= 0) {
    auto ret = multi_polygon_type_fp();
    ret.push_back(lhs);
    return ret;
  }
  multi_polygon_type_fp ret;
  bg::difference(lhs, rhs, ret);
  return ret;
}

template <typename rhs_t>
static inline multi_polygon_type_fp operator-(const box_type_fp& lhs, const rhs_t& rhs) {
  auto box_mp = multi_polygon_type_fp();
  bg::convert(lhs, box_mp);
  return box_mp - rhs;
}

template <typename linestring_type_t, typename rhs_t>
static inline bg::model::multi_linestring<linestring_type_t> operator-(const bg::model::multi_linestring<linestring_type_t>& lhs,
                                                                       const rhs_t& rhs) {
  if (bg::area(rhs) <= 0) {
    return lhs;
  }
  bg::model::multi_linestring<linestring_type_t> ret;
  bg::difference(lhs, rhs, ret);
  return ret;
}

template <typename linestring_type_t, typename rhs_t>
static inline bg::model::multi_linestring<linestring_type_t> operator&(const bg::model::multi_linestring<linestring_type_t>& lhs,
                                                                       const rhs_t& rhs) {
  bg::model::multi_linestring<linestring_type_t> ret;
  if (bg::area(rhs) <= 0) {
    return ret;
  }
  if (bg::length(lhs) <= 0) {
    return ret;
  }
  bg::intersection(lhs, rhs, ret);
  return ret;
}

template <typename rhs_t>
static inline multi_linestring_type_fp operator&(const linestring_type_fp& lhs,
                                                 const rhs_t& rhs) {
  multi_linestring_type_fp ret;
  if (bg::area(rhs) <= 0) {
    return ret;
  }
  if (bg::length(lhs) <= 0) {
    return ret;
  }
  bg::intersection(lhs, rhs, ret);
  return ret;
}

template <typename polygon_type_t, typename rhs_t>
static inline bg::model::multi_polygon<polygon_type_t> operator^(const bg::model::multi_polygon<polygon_type_t>& lhs,
                                                                 const rhs_t& rhs) {
  if (bg::area(rhs) <= 0) {
    return lhs;
  }
  if (bg::area(lhs) <= 0) {
    return rhs;
  }
  bg::model::multi_polygon<polygon_type_t> ret;
  bg::sym_difference(lhs, rhs, ret);
  return ret;
}

template <typename polygon_type_t, typename rhs_t>
static inline bg::model::multi_polygon<polygon_type_t> operator&(const bg::model::multi_polygon<polygon_type_t>& lhs,
                                                                 const rhs_t& rhs) {
  bg::model::multi_polygon<polygon_type_t> ret;
  if (bg::area(rhs) <= 0) {
    return ret;
  }
  if (bg::area(lhs) <= 0) {
    return ret;
  }
  bg::intersection(lhs, rhs, ret);
  return ret;
}

template <typename point_type_t, typename rhs_t>
static inline multi_polygon_type_fp operator&(const bg::model::polygon<point_type_t>& lhs,
                                              const rhs_t& rhs) {
  multi_polygon_type_fp ret;
  if (bg::area(rhs) <= 0) {
    return ret;
  }
  if (bg::area(lhs) <= 0) {
    return ret;
  }
  bg::intersection(lhs, rhs, ret);
  return ret;
}

template <typename polygon_type_t, typename rhs_t>
static inline bg::model::multi_polygon<polygon_type_t> operator+(const bg::model::multi_polygon<polygon_type_t>& lhs,
                                                                 const rhs_t& rhs);
namespace bg_helpers {

// The below implementations of buffer are similar to bg::buffer but
// always convert to floating-point before doing work, if needed, and
// convert back afterward, if needed.  Also, they work if expand_by is
// 0, unlike bg::buffer.
static const int points_per_circle = 32;
template<typename CoordinateType>
static inline void buffer(multi_polygon_type_fp const & geometry_in, multi_polygon_type_fp & geometry_out, CoordinateType expand_by) {
  if (expand_by == 0) {
    bg::convert(geometry_in, geometry_out);
  } else {
    bg::buffer(geometry_in, geometry_out,
               bg::strategy::buffer::distance_symmetric<CoordinateType>(expand_by),
               bg::strategy::buffer::side_straight(),
               bg::strategy::buffer::join_round(points_per_circle),
               bg::strategy::buffer::end_round(points_per_circle),
               bg::strategy::buffer::point_circle(points_per_circle));
  }
}

static inline multi_polygon_type_fp buffer(multi_polygon_type_fp const & geometry_in, coordinate_type_fp expand_by) {
  if (expand_by == 0) {
    return geometry_in;
  } else {
    multi_polygon_type_fp geometry_out;
    bg::buffer(geometry_in, geometry_out,
               bg::strategy::buffer::distance_symmetric<coordinate_type_fp>(expand_by),
               bg::strategy::buffer::side_straight(),
               bg::strategy::buffer::join_round(points_per_circle),
               bg::strategy::buffer::end_round(points_per_circle),
               bg::strategy::buffer::point_circle(points_per_circle));
    return geometry_out;
  }
}

template<typename CoordinateType>
static inline void buffer(polygon_type_fp const & geometry_in, multi_polygon_type_fp & geometry_out, CoordinateType expand_by) {
  if (expand_by == 0) {
    bg::convert(geometry_in, geometry_out);
  } else {
    bg::buffer(geometry_in, geometry_out,
               bg::strategy::buffer::distance_symmetric<CoordinateType>(expand_by),
               bg::strategy::buffer::side_straight(),
               bg::strategy::buffer::join_round(points_per_circle),
               bg::strategy::buffer::end_round(points_per_circle),
               bg::strategy::buffer::point_circle(points_per_circle));
  }
}

template<typename CoordinateType>
static inline void buffer(linestring_type_fp const & geometry_in, multi_polygon_type_fp & geometry_out, CoordinateType expand_by) {
  if (expand_by == 0) {
    geometry_out.clear();
  } else {
    bg::buffer(geometry_in, geometry_out,
               bg::strategy::buffer::distance_symmetric<CoordinateType>(expand_by),
               bg::strategy::buffer::side_straight(),
               bg::strategy::buffer::join_round(points_per_circle),
               bg::strategy::buffer::end_round(points_per_circle),
               bg::strategy::buffer::point_circle(points_per_circle));
  }
}

template<typename CoordinateType>
static inline multi_polygon_type_fp buffer(polygon_type_fp const & geometry_in, CoordinateType expand_by) {
  multi_polygon_type_fp geometry_out;
  buffer(geometry_in, geometry_out, expand_by);
  return geometry_out;
}

template<typename CoordinateType>
static inline void buffer(multi_linestring_type_fp const & geometry_in, multi_polygon_type_fp & geometry_out, CoordinateType expand_by) {
  if (expand_by == 0) {
    geometry_out.clear();
    return;
  }
  // bg::buffer of multilinestring is broken in boost.  Converting the
  // multilinestring to non-intersecting paths seems to help.
  multi_linestring_type_fp mls = eulerian_paths::make_eulerian_paths(geometry_in, true, true);
#ifdef GEOS_VERSION
  geos::io::WKTReader reader;
  std::stringstream ss;
  ss << bg::wkt(mls);
  auto geos_in = reader.read(ss.str());
  std::unique_ptr<geos::geom::Geometry> geos_out(geos::operation::buffer::BufferOp::bufferOp(geos_in.get(), expand_by, points_per_circle/4));
  geos::io::WKTWriter writer;
  auto geos_wkt = writer.write(geos_out.get());
  if (strncmp(geos_wkt.c_str(), "MULTIPOLYGON", 12) == 0) {
    bg::read_wkt(geos_wkt, geometry_out);
  } else {
    polygon_type_fp poly;
    bg::read_wkt(geos_wkt, poly);
    geometry_out.clear();
    geometry_out.push_back(poly);
  }
#else
  geometry_out.clear();
  if (expand_by == 0) {
    return;
  }
  for (const auto& ls : mls) {
    multi_polygon_type_fp buf;
    buffer(ls, buf, expand_by);
    geometry_out = geometry_out + buf;
  }
#endif
}

template<typename CoordinateType>
static inline multi_polygon_type_fp buffer(multi_linestring_type_fp const & geometry_in, CoordinateType expand_by) {
  multi_polygon_type_fp geometry_out;
  buffer(geometry_in, geometry_out, expand_by);
  return geometry_out;
}

template<typename CoordinateType>
static inline void buffer(ring_type_fp const & geometry_in, multi_polygon_type_fp & geometry_out, CoordinateType expand_by) {
  if (expand_by == 0) {
    bg::convert(geometry_in, geometry_out);
  } else {
    polygon_type_fp geometry_in_fp;
    bg::convert(geometry_in, geometry_in_fp);
    multi_polygon_type_fp geometry_out_fp;
    bg::buffer(geometry_in_fp, geometry_out,
               bg::strategy::buffer::distance_symmetric<CoordinateType>(expand_by),
               bg::strategy::buffer::side_straight(),
               bg::strategy::buffer::join_round(),
               bg::strategy::buffer::end_round(),
               bg::strategy::buffer::point_circle());
  }
}

} // namespace bg_helpers

template <typename polygon_type_t, typename rhs_t>
static inline bg::model::multi_polygon<polygon_type_t> operator+(const bg::model::multi_polygon<polygon_type_t>& lhs,
                                                                 const rhs_t& rhs) {
  if (bg::area(rhs) <= 0) {
    return lhs;
  }
  if (bg::area(lhs) <= 0) {
    bg::model::multi_polygon<polygon_type_t> ret;
    bg::convert(rhs, ret);
    return ret;
  }
  // This optimization fixes a bug in boost geometry when shapes are bordering
  // somwhat but not overlapping.  This is exposed by EasyEDA that makes lots of
  // shapes like that.
  const auto lhs_box = bg::return_envelope<box_type_fp>(lhs);
  const auto rhs_box = bg::return_envelope<box_type_fp>(rhs);
  if (lhs_box.max_corner().x() == rhs_box.min_corner().x() ||
      rhs_box.max_corner().x() == lhs_box.min_corner().x() ||
      lhs_box.max_corner().y() == rhs_box.min_corner().y() ||
      rhs_box.max_corner().y() == lhs_box.min_corner().y()) {
    multi_polygon_type_fp new_rhs;
    bg::convert(rhs, new_rhs);
    return bg_helpers::buffer(lhs, 0.00001) + bg_helpers::buffer(new_rhs, 0.00001);
  }
  bg::model::multi_polygon<polygon_type_t> ret;
  bg::union_(lhs, rhs, ret);
  return ret;
}

// It's not great to insert definitions into the bg namespace but they
// are useful for sorting and maps.

namespace boost { namespace geometry { namespace model { namespace d2 {

template <typename T>
static inline bool operator<(
    const boost::geometry::model::d2::point_xy<T>& x,
    const boost::geometry::model::d2::point_xy<T>& y) {
  return std::tie(x.x(), x.y()) < std::tie(y.x(), y.y());
}

template <typename T>
static inline boost::geometry::model::d2::point_xy<T> operator-(
    const boost::geometry::model::d2::point_xy<T>& lhs,
    const boost::geometry::model::d2::point_xy<T>& rhs) {
  return {lhs.x()-rhs.x(), lhs.y()-rhs.y()};
}

template <typename T>
static inline boost::geometry::model::d2::point_xy<T> operator+(
    const boost::geometry::model::d2::point_xy<T>& lhs,
    const boost::geometry::model::d2::point_xy<T>& rhs) {
  return {lhs.x()+rhs.x(), lhs.y()+rhs.y()};
}

template <typename T, typename S>
static inline boost::geometry::model::d2::point_xy<T> operator/(
    const boost::geometry::model::d2::point_xy<T>& lhs,
    const S& rhs) {
  return {lhs.x()/static_cast<T>(rhs), lhs.y()/static_cast<T>(rhs)};
}

template <typename T, typename S>
static inline boost::geometry::model::d2::point_xy<T> operator*(
    const boost::geometry::model::d2::point_xy<T>& lhs,
    const S& rhs) {
  return {lhs.x()*static_cast<T>(rhs), lhs.y()*static_cast<T>(rhs)};
}

template <typename T>
static inline bool operator==(
    const boost::geometry::model::d2::point_xy<T>& x,
    const boost::geometry::model::d2::point_xy<T>& y) {
  return std::tie(x.x(), x.y()) == std::tie(y.x(), y.y());
}

template <typename T>
static inline bool operator!=(
    const boost::geometry::model::d2::point_xy<T>& x,
    const boost::geometry::model::d2::point_xy<T>& y) {
  return std::tie(x.x(), x.y()) != std::tie(y.x(), y.y());
}

static inline std::ostream& operator<<(std::ostream& out, const point_type_fp& t) {
  out << bg::wkt(t);
  return out;
}

}}}} // namespace boost::geometry::model::d2

namespace std {

template <typename A, typename B>
static inline std::ostream& operator<<(std::ostream& out, const pair<A, B>& p) {
  out << "{" << p.first << "," << p.second << "}";
  return out;
}

template <typename T>
static inline std::ostream& operator<<(std::ostream& out, const vector<T>& xs) {
  out << "{";
  for (const auto& x : xs) {
    out << x << ",";
  }
  out << "}";
  return out;
}

} // namespace std

#endif //BG_HELPERS_H
