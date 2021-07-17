#ifndef POINT_HPP
#define POINT_HPP

template <typename M, typename E>
class MyPoint {
  public:
  M magic;
  E err;


  template <typename M, typename E>
  MyPoint(M ma, E er) : magic{ma}, err{er} {}

  bool operator< (MyPoint& oth) {
    return this->err < oth.err;
  }

};
#endif