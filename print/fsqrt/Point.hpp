#ifndef POINT_HPP
#define POINT_HPP

template <typename M, typename E>
class Point {
  public:
  M magic;
  E err;

  Point(M ma, E er) : magic{ma}, err{er} {}

  bool operator< (Point& oth) {
    return this->err < oth.err;
  }
  bool operator> (Point& oth) {
    return this->err > oth.err;
  }

};
#endif